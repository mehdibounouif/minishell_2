/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:42:07 by moraouf           #+#    #+#             */
/*   Updated: 2025/08/08 22:01:54 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	update_oldpwd(t_env *env)
{
	char	*oldpwd;
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (1);
	oldpwd = ft_strjoin("OLDPWD=", current_dir);
	free(current_dir);
	if (update_env_var(env, oldpwd) == 1)
		return (1);
	return (0);
}

static int	update_pwd(t_env *env)
{
	char	*pwd;
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (1);
	*(back_up()) = current_dir;
	pwd = ft_strjoin("PWD=", current_dir);
  free(current_dir);
	if (update_env_var(env, pwd) == 1)
		return (1);
	return (0);
}

static int	change_to_home(t_env *env)
{
	char	*home;

	home = get_env_value(env, "HOME");
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	if (chdir(home) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(home, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}

char	**back_up(void)
{
	static char	*value;

	return (&value);
}

int	cd_command(t_env *env, char **args)
{
	if (args[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (global(1));
	}
	else if (!args[0] || (args[0][0] == '-' && !args[0][1]))
	{
		ft_putstr_fd("minishell: cd: option not supported\n", 2);
		global(1);
	}
	else if (!args[1])
	{
		if (change_to_home(env) == 1)
			return (1);
		return(global(0));
		
	}
	if (update_oldpwd(env) == 1)
		return (1);
	if (chdir(args[1]) == -1)
	{
		perror("minishell: cd");
		global(1);
	}
	if (update_pwd(env) == 1)
		return (1);
	return (0);
}
