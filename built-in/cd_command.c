#include "../includes/minishell.h"

static int	update_oldpwd(t_env *env)
{
	char	*oldpwd;
	char	*current_dir;

	current_dir = getcwd(NULL, 0); // get current working directory (cwd)
	if (!current_dir)
		return (1);
	oldpwd = ft_strjoin("OLDPWD=", current_dir);
	free(current_dir);
	if (!oldpwd)
		return (1);
	if (update_env_var(env, oldpwd) == 1)
	{
		free(oldpwd);
		return (1);
	}
	free(oldpwd);
	return (0);
}

static int	update_pwd(t_env *env)
{
	char	*pwd;
	char	*current_dir;

	current_dir = getcwd(NULL, 0); // get current working directory (cwd)
	if (!current_dir)
		return (1);
	pwd = ft_strjoin("PWD=", current_dir); // join PWD= and current_dir
	free(current_dir);
	if (!pwd)
		return (1);
	if (update_env_var(env, pwd) == 1) // update the PWD environment variable
	{
		free(pwd);
		return (1);
	}
	free(pwd); // free the pwd variable
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

int	cd_command(t_env *env, char **args)
{
	if (!args[1] || (args[1][0] == '~' && !args[1][1]))
	{
		if (change_to_home(env) == 1)
			return (1);
	}
	else if (chdir(args[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	if (update_oldpwd(env) == 1 || update_pwd(env) == 1)
		return (1);
	return (0);
}
