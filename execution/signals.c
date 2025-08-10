/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:06:57 by moraouf           #+#    #+#             */
/*   Updated: 2025/08/10 17:51:50 by moraouf      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	sig_ctrl(int state)
{
	static int	value;

	if (state != -1)
		value = state;
	return (value);
}

int	ft_return_signal(int status)
{
	int	sig;

	sig = 0;
	if (WIFSIGNALED(status))
	{
		ft_putchar_fd('\n', 1);
		sig = WTERMSIG(status);
		sig += 128;
		global(sig);
		return (1);
	}
	else if (WIFEXITED(status))
	{
		sig = WEXITSTATUS(status);
		if (sig == 130)
		{
			global(sig);
			return (1);
		}
		global(sig);
		return (0);
	}
	return (0);
}

void	handler_sigint(int sig __attribute__((unused)))
{
	if (sig_ctrl(-1) != 1)
	{
		ft_putchar_fd('\n', 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		global(130);
	}
}

void	handle_signal(void)
{
	signal(SIGINT, handler_sigint);
	signal(SIGQUIT, SIG_IGN);
}
