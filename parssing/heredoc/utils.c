#include "../../includes/minishell.h"

char	*get_last_file(char **list)
{
	int	i;

	if (!(*list) || !list)
		return (NULL);
	i = 0;
	while (list[i+1])
		i++;
	return (list[i]);
}

void	protect_line(t_share3 *share, t_env *env, t_redirection *list)
{
	printf("bash: warning: here-document at line 1 delimited by end-of-file (wanted `%s')\n", list->herdoc->delimeter);
	//ulink_files(list->heredocs);
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

void  child(t_share3 *share, t_redirection *list, t_env *env)
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

void	read_lines(int *flag, t_share3 *share, t_redirection *list, t_env *env)
{
  pid_t pid;
	int status;

	sig_ctrl(1); //mode execution
	pid = fork();
 	if (pid < 0)
    	protect(env, "Fork failed");
	else if(pid == 0)
    child(share, list, env);
	else
	{
		if (waitpid(pid, &status, 0) == -1)
      protect(env, "Waitpid faild");
		*flag = ft_return_signal(status);
		sig_ctrl(0);
	}
}