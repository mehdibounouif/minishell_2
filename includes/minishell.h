/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:55:09 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/24 11:32:06 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../libft/libft.h"

# define PIPE 1
# define REDIRECTION_OUT 2
# define REDIRECTION_IN 3
# define REDIRECTION_APPEND 4
# define REDIRECTION_ERR 5
# define RUN_BACKGROUND 6
# define HEREDOC 7
# define COMMAND1 8
# define COMMAND2 9
# define ARG 10
# define OUTFILE 11
# define INFILE 12
# define ERRFILE 13

typedef	struct s_node
{
	int	type;
	char	*content;
	struct	s_node *next;
	struct	s_node *prev;
}	t_node;

void	tokenize(char *command, t_node **list);
int	is_space(char c);
void	add_back(t_node **list, t_node *node);
void	print_list(t_node *list);
