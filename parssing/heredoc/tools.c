#include "../../includes/minishell.h"

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
    //free(share->file_name);
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

char	*generate_file_name(t_env *env)
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