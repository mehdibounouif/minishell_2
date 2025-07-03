#include "../includes/minishell.h"
/*
int check_sides(t_node *list)
{
	if (list->type == PIPE || list->content[0] == '#')
		return (1);
	while(list->next)
		list = list->next;
	if (list->type == PIPE)
		return (1);
	return (0);
}
*/
int     check_syntax(t_mini *mini, t_node *list)
{
	if ((list->next == NULL && list->type == PIPE) || (list->prev == NULL && list->type == PIPE))
	{
                ft_putendl_fd("bash: syntax error near unexpected token |", 2);
		mini->ret = 258;
		return (0);
	}
        while (list)
        {
                if ((list->type == PIPE && (list->next->type == PIPE || list->next->type == END))
				|| (list->type == END && list->next
					&& (list->next->type == PIPE || list->next->type == END))
				|| (is_redirection(list) && (!list->next || list->next->type != WORD)))
                {
			if (list->next)
			{
                        	ft_putstr_fd("bash: syntax error near unexpected token ", 2);
				ft_putendl_fd(list->next->content, 2);
			}
			else
			{
                        	ft_putstr_fd("bash: syntax error near unexpected token ", 2);
				ft_putendl_fd(list->content, 2);
			}
			mini->ret = 258;
                        return (0);
                }
                list = list->next;
        }
        return (1);
}
