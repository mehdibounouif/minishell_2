/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/12 06:31:22 by mbounoui         ###   ########.fr       */
/*   Updated: 2025/08/11 17:27:15 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	global(int state)
{
	static int	value;

	if (state != -1)
		value = state;
	return (value);
}

void  mini_shell(t_mini *minishell, char **env, t_env *envp)
{
  while (1)
	{
		handle_signal();
		if(isatty(0))
		{
			
		}
		if (!readline_and_parssing(minishell, envp))
			continue ;
		//print_ast(minishell->tree, 0);
		if (!open_herdocs(minishell->tree, envp))
			continue ;
		sig_ctrl(1);
		execute_full_command(minishell->tree, &envp, env, 0);
		sig_ctrl(0);
	}
}

static void init_pwd()
{
	char *cwd;
	
	cwd = getcwd(NULL , 0);
	if(!cwd)
		perror("minishell ::");
	else
		*(back_up()) = ft_strdup(cwd);
	free(cwd);
}

int	main(int c, char **v __attribute__((unused)), char **env)
{
	t_mini	minishell;
	t_env	*envp;

	if (c != 1)
	{
		ft_putendl_fd("This program does not accept arguments", 2);
		exit(0);
	}
	init_pwd();
	envp = NULL;
	ft_bzero(&minishell, sizeof(t_mini));
	if (!get_env(&envp, env))
		exit(0);
	global(0);
	mini_shell(&minishell, env, envp);
	ft_free_garbage(ft_function());
	exit(global(-1));
}
