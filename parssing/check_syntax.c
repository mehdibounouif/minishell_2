#include "../includes/minishell.h"

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

int     check_syntax(t_node *list)
{
        while (list)
        {
                // CHECK ( | before | OR IN END OR AT START)
                if ((list->type == PIPE && (list->next->type == PIPE || list->next->type == END))
				|| (list->prev == NULL && list->type == PIPE)
				|| (list->next == NULL && list->type == PIPE))
                {
                        printf("bash: syntax error near unexpected token %s\n", list->next->content);
                        return (0);
                }
                // CHECK ( ; before ; or |)
                if (list->type == END && (list->next->type == PIPE || list->next->type == END))
                {
                        printf("bash: syntax error near unexpected token %s\n", list->next->content);
                        return (0);
                }
		// CHECK IF (> OR < OR >> OR >2) BEFORE (> OR < OR >> OR >2)
		if ((list->type == R_IN || list->type == R_OUT
				|| list->type == R_APPEND
				|| list->type == R_ERR) 
				&& list->type
			       	&&(list->next->type == R_IN
					|| list->next->type == R_OUT
					|| list->next->type == R_APPEND
					|| list->next->type == R_ERR
					|| list->next->type == PIPE
					|| list->next->type == END))
		{
			printf("-minishell: syntax error near unexpected token %s\n", list->next->content);
			return (0);
		}
                list = list->next;
        }
        return (1);
}
