/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_herdoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha_laylay <taha_laylay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:55 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/09 23:13:34 by taha_laylay      ###   ########.fr       */
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

static char	*generate_file_name(void)
{
	unsigned char	c;
	char			*buffer;

	int fd, i;
	buffer = ft_malloc(sizeof(char), 9);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (buffer);
	i = 0;
	while (i < 8)
	{
		if (read(fd, &c, 1) != 1)
		{
			close(fd);
			return (NULL);
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

void	check_line(t_share3 *share)
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

int	read_lines(int *flag, t_share3 *share, t_redirection *list, t_env *env)
{
  pid_t pid;
	int status;

	sig_ctrl(1); //mode execution
	pid = fork();
  if (pid < 0)
  {
    ft_free_garbage(ft_function());
    free_env(env);
    exit(1);
  }
	else if(pid == 0)
	{
		signal(SIGINT, SIG_DFL);
    share->line = readline(">");
    if(!share->line)
    {
      printf("bash: warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", list->herdoc->delimeter);
      check_line(share);
      ft_free_garbage(ft_function());
      free_env(env);
      exit(global(-1));
    }
    while (share->line && ft_strcmp(list->herdoc->delimeter, share->line))
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
      }
      // free(share->line);
      share->line = NULL;
      share->line = readline(">");
      if(!share->line)
      {
        printf("bash: warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", list->herdoc->delimeter);
        check_line(share);
        ft_free_garbage(ft_function());
        free_env(env);
        exit(global(-1));
      }
    }
    check_line(share);
    ft_free_garbage(ft_function());
    free_env(env);
    exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
    /// printf("%s\n", WIFSIGNALED(status) ? "yes" : "no");
		*flag = ft_return_signal(status);
		sig_ctrl(0);
		//ft_free_garbage(ft_function());
	}
	return (1);
}

int	create_heredoc(int *flag, t_redirection *list, t_env *env, int i)
{
	t_share3 *share;

	share = ft_malloc(sizeof(t_share3), 1);
	share->expand_line = NULL;
	share->line = NULL;
	share->fd = -1;
	share->file_name = generate_file_name();
	share->fd = open(share->file_name, O_RDWR | O_CREAT | O_TRUNC,  0777);
	if (share->fd == -1)
	{
		// free(share->file_name);
		printf("open heredoc file failed!\n");
    free_env(env);
    ft_free_garbage(ft_function());
		exit(0);
	}
	list->heredocs[i] = ft_strdup(share->file_name);
	read_lines(flag, share, list, env);
	close(share->fd);
	// signal here stop excution
	if (*flag)
		return (0);
	return (1);
}

int	open_her(int *flag, t_tree *tree, t_env *env)
{
	int	i;

	i = 0;
	tree->redirect->heredocs = ft_malloc(20, sizeof(char *));
	ft_memset(tree->redirect->heredocs, 0, sizeof(char *) * 20);
	while (tree->redirect->herdoc)
	{
		if (!create_heredoc(flag, tree->redirect, env, i))
		{
			//free(tree->redirect->heredocs);
			//free(tree->redirect);
			//exit(global(-1));
      return (0);
		}
		// Stop here
		if (*flag)
			return (0);
		tree->redirect->herdoc = tree->redirect->herdoc->next;
		i++;
	}
	tree->redirect->hrc_file = get_last_file(tree->redirect->heredocs);
	return (1);
}

int	open_herdocs(int *flag, t_tree *tree, t_env *env)
{
	if (!tree)
		return (0);
	if (tree->type == REDIRECT_NODE)
	{
		if (!open_her(flag, tree, env))
      return (0);
		// Stop here
		if (*flag)
			return (0);
	}
	else if (tree->type == PIPE_NODE)
	{
		if (!open_herdocs(flag, tree->pipe->left, env))
			return (0);
		if (*flag)
			return (0);
		if (!open_herdocs(flag, tree->pipe->right, env))
			return (0);
		if (*flag)
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
