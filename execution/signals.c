/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:06:57 by moraouf           #+#    #+#             */
/*   Updated: 2025/07/30 16:43:56 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



int sig_ctrl(int state)
{
	static int value;
	if(state != -1)
		value = state;
	return value;
}

int	ft_return_signal(int status)
{
	int sig;

	sig = 0;
	if(WIFSIGNALED(status))
	{
		printf("\n");
		sig = WTERMSIG(status);
		sig += 128;
		global(sig);
		return (1);
	}
	else if(WIFEXITED(status))
	{
		sig = WTERMSIG(status);
		global(sig);
		return (1);
	}
	return (0);
}

void	handler_sigint(int sig)
{
	(void)sig;

	if(sig_ctrl(-1) != 1) // interactive mode 
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		global(130);
	}
	// In execution mode, do nothing - let child process handle it
}

void	handle_signal()
{
	signal(SIGINT,handler_sigint);
	signal(SIGQUIT,SIG_IGN);
}
