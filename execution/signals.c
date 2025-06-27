/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 11:06:57 by moraouf           #+#    #+#             */
/*   Updated: 2025/06/25 14:09:45 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void handler_sigint(int sig)
{
    if(sig == SIGINT)
    {
        // g_ctrl_c = TRUE;
		write(1, "\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
    }

}

void handle_signal()
{	
	rl_catch_signals = 0;
    signal(SIGINT,handler_sigint);
    signal(SIGQUIT,handler_sigint);
}
