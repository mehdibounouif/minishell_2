/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_herdoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha_laylay <taha_laylay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:55 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/10 18:14:20 by taha_laylay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

typedef struct	s_share3
{
	int	fd;
	char	*line;
	char	*file_name;
	char	*expand_line;
}	t_share3;

// Fonction for stock 
t_share3 *get_current_share(t_share3 *share)
{
	static t_share3 *current_share = NULL;
	if (share != (t_share3 *)-1) // -1 for recup
		current_share = share;
	return current_share;
}

t_env *get_current_env(t_env *env)
{
	static t_env *current_env = NULL;
	if (env != (t_env *)-1) 
		current_env = env;
	return current_env;
}

void  check_line(t_share3 *share)
{
	if(share->file_name)
	{
		share->file_name = NULL;
	}
	if(share->fd >= 0)
	{
		close(share->fd);
		share->fd = -1;
	}
}

// gestion
void heredoc_sigint_handler(int sig __attribute__((unused)))
{
	t_share3 *current_share;
	t_env *current_env;
	
	ft_putchar_fd('\n', 1);
	current_share = get_current_share((t_share3 *)-1);
	current_env = get_current_env((t_env *)-1);
	if (current_share && current_share->line)
	{
		free(current_share->line);
		current_share->line = NULL;
	}
	if (current_share)
		check_line(current_share);
	if (current_env)
	{
		ft_free_garbage(ft_function());
		free_env(current_env);
	}
	exit(130); 
}

static char	*generate_file_name(t_env *env)
{
	unsigned char	c;
	char			*buffer;

	int fd, i;
	buffer = ft_malloc(sizeof(char), 9);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		protect(env, "Open failed");
	i = 0;
	while (i < 8)
	{
		if (read(fd, &c, 1) == -1)
		{
			close(fd);
			protect(env, "Read faild");
		}
		buffer[i++] = 'a' + (c % 26);
	}
	buffer[i] = '\0';
	close(fd);
	return (buffer);
}

char	*get_last_herdoc(t_herdoc *list)
{
	if (!list)
		return (NULL);
	while (list->next)
	{
		list = list->next;
	}
	return (list->delimeter);
}

char	*get_last_file(char **list)
{
	if (!(*list) || !list)
		return (NULL);
	int	i;

	i = 0;
	while (list[i+1])
	{
		i++;
	}
	return (list[i]);
}

int	get_lastfd(int *list)
{
	int	i;

	i = 0;
	while (list[i] != -1)
		i++;
	return (list[i-1]);
}

void	protect_line(t_share3 *share, t_env *env, t_redirection *list)
{
	printf("bash: warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", list->herdoc->delimeter);
	ulink_files(list->heredocs);
	close(share->fd);
	ft_free_garbage(ft_function());
	free_env(env);
	exit(1);
}

void  get_line(t_share3 *share, t_redirection *list, t_env *env)
{
  if (!list->herdoc->quoted)
  {
    share->expand_line = expansion(share->line, env, 0);
    write(share->fd, share->expand_line, ft_strlen(share->expand_line));
    write(share->fd, "\n", 1);
    share->expand_line = NULL;
  }
  else
  {
    write(share->fd, share->line, ft_strlen(share->line));
    write(share->fd, "\n", 1);
    free(share->line);
  }
  share->line = readline(">");
  if(!share->line)
    protect_line(share, env, list);
}

void	read_lines(int *flag, t_share3 *share, t_redirection *list, t_env *env)
{
  pid_t pid;
	int status;

	sig_ctrl(1); //mode execution
	pid = fork();
 	if (pid < 0)
    	protect(env, "Fork failed");
	else if(pid == 0)
	{
		// Stocker les pointeurs pour le gestionnaire de signal
		get_current_share(share);
		get_current_env(env);
		signal(SIGINT, heredoc_sigint_handler);
    	share->line = readline(">");
    	if(!share->line)
      	protect_line(share, env, list);
    while (share->line && ft_strcmp(list->herdoc->delimeter, share->line))
      get_line(share, list, env);
    //free the last line if it exists
    if (share->line)
      free(share->line);
    close(share->fd);
    ft_free_garbage(ft_function());
    free_env(env);
    exit(0);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
      protect(env, "Waitpid faild");
		*flag = ft_return_signal(status);
		sig_ctrl(0);
	}
}

int	create_heredoc(t_redirection *list, t_env *env, int i)
{
	t_share3 *share;
  int sig_flag;

	share = ft_malloc(sizeof(t_share3), 1);
	share->expand_line = NULL;
	share->line = NULL;
	share->fd = -1;
	share->file_name = generate_file_name(env);
	share->fd = open(share->file_name, O_RDWR | O_CREAT | O_TRUNC,  0777);
  if (share->fd == -1)
    protect(env, "Open failed");
	list->heredocs[i] = ft_strdup(share->file_name);
	read_lines(&sig_flag, share, list, env);
	close(share->fd);
	// signal here stop excution
	if (sig_flag)
		return (0);
	return (1);
}

int	open_her(t_tree *tree, t_env *env)
{
	int	i;

	i = 0;
	tree->redirect->heredocs = ft_malloc(20, sizeof(char *));
	ft_memset(tree->redirect->heredocs, 0, sizeof(char *) * 20);
	while (tree->redirect->herdoc)
	{
		if (!create_heredoc(tree->redirect, env, i))
     		return (0);
		tree->redirect->herdoc = tree->redirect->herdoc->next;
		i++;
	}
	tree->redirect->hrc_file = get_last_file(tree->redirect->heredocs);
	return (1);
}

int	open_herdocs(t_tree *tree, t_env *env)
{
	if (!tree)
		return (0);
	if (tree->type == REDIRECT_NODE)
	{
		if (!open_her(tree, env))
      return (0);
	}
	else if (tree->type == PIPE_NODE)
	{
		if (!open_herdocs(tree->pipe->left, env))
			return (0);
		if (!open_herdocs(tree->pipe->right, env))
			return (0);
	}
	return (1);
}

void	take_heredoc(t_tree *node, t_node *list)
{
	t_herdoc *h_node;

	h_node = ft_malloc(sizeof(t_herdoc), 1);
	h_node->quoted = 0;
	if (node->redirect->in)
	{
		node->redirect->hdc = 1;
		node->redirect->in = 0;
	}
	else
		node->redirect->hdc = 1;
	h_node->herdoc = ft_strdup(list->content);
	list = list->next;
	h_node->delimeter = ft_strdup(list->content);
	if (list->contain_quoted || list->between_quoted)
		h_node->quoted = 1;
	h_node->next = NULL;
	add_back3(&node->redirect->herdoc, h_node);
}

void	collect_herdoc(t_tree *node, t_node *list)
{
	while (list && list->type != PIPE)
	{
		if (list->type == R_IN)
			node->redirect->in_count++;
		else if (list->type == R_OUT || list->type == R_APPEND)
			node->redirect->out_count++;
		if (list->type == HEREDOC && list->next)
			take_heredoc(node, list);
		if (list->type == R_IN && node->redirect->hdc)
		{
			node->redirect->hdc = 0;
			node->redirect->in = 1;
		}
		else if (list->type == R_IN && !node->redirect->hdc)
			node->redirect->in = 1;
		list = list->next;
	}
}
