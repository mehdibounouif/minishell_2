#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "../includes/minishell.h"

// CD command
int	cd_command(t_env *env, char **args);

// Echo command (to be implemented)
int echo_command(t_env *env, char **args);

// PWD command (to be implemented)
int	pwd_command(t_env *env, char **args);

// Export command (to be implemented)
int	export_command(t_env *env, char **args);

// Unset command (to be implemented)
int	unset_command(t_env *env, char **args);

// Env command (to be implemented)
int	env_command(t_env *env, char **args);

// Exit command (to be implemented)
int	exit_command(t_env *env, char **args);

#endif 