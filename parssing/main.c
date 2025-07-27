/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/27 17:18:34 by mbounoui         ###   ########.fr       */
/*   Updated: 2025/07/26 13:51:28 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../includes/minishell.h"

int global(int state)
{
	static int value;
	if(state != -1)
		value = state;
	return value;
}


char	*ft_prompt(t_env *env)
{
	char	*prompt;
	char	*first_part;
	char	*last_part;
	char	*path;
	char	*user;
	char	*sep;

	path = getcwd(NULL, 0); // get current working directory (cwd)
	if (!path)
		return (NULL);
	user = ft_getenv("USER", env);
	if (!user)
		user = ft_strdup("user");
	sep = ft_strdup(":~");
	first_part = ft_strjoin(user, sep);
	last_part = ft_strjoin(first_part, path);
	prompt = ft_strjoin(last_part, "$ ");
	return (prompt);
}


int	open_her(int *flag, t_mini minishell, t_env *envp)
{
	pid_t pid;
 	int status;

	sig_ctrl(1); //mode execution
	pid = fork();
	if(pid == 0)
	{
		signal(SIGINT, SIG_DFL);
	//	signal(SIGQUIT, SIG_DFL);
		if (!open_herdocs(minishell.tree, envp))
      return (0);
	}
	else if(pid > 0)
	{
		waitpid(pid, &status, 0);
		// printf("%s\n", WIFSIGNALED(status) ? "yes" : "no");
		*flag = ft_return_signal(status);
		sig_ctrl(0);
	}
  return (1);
}

int	check_heredoc(t_tree *tree)
{
	if (tree->type == REDIRECT_NODE)
	{
		if (tree->redirect->herdoc)
			return (1);
	}
	if (tree->type == PIPE_NODE)
	{
		return (check_heredoc(tree->pipe->left));
		return (check_heredoc(tree->pipe->right));
	}
	return (0);
}

int	main(int c, char **v __attribute__((unused)), char **env)
{
	t_mini  minishell;
	t_env *envp;

	if (c != 1)
	{
		ft_putendl_fd("This program does not accept arguments", 2);
		exit(0);
	}
	envp = NULL;
	ft_bzero(&minishell, sizeof(t_mini));
	if (!get_env(&envp, env))
		exit (0);
	global(0); //Initial state, interactive mode 
	while (1)
	{
		handle_signal();
		if (!readline_and_parssing(&minishell, envp))
			continue;
		int flag = 0;
		if (check_heredoc(minishell.tree))
			if (!open_her(&flag, minishell, envp))
			{
				free_tree(&minishell.tree);
				exit(global(-1));
			}
		if (flag)
			continue;
		sig_ctrl(1); // Set execution mode
		execute_full_command(minishell.tree, envp, env);
		sig_ctrl(0); // Back to interactive mode
		//  	free_tree(&minishell.tree);
		// // print_ast(minishell.tree, 0);
	}
	exit(global(-1));
	return (0);
}
