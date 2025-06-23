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
	// CHECK SIDES | IN START | IN END
	if (check_sides(list))
	{
		printf("bash: syntax error near unexpected token |\n");
		return (0);
	}
        while (list)
        {
                // CHECK ( | before | OR IN END OR AT START)
                if ((list->type == PIPE && (list->next->type == PIPE || list->next->type == END)))
                {
                        printf("bash: syntax error near unexpected token %s\n", list->next->content);
                        return (0);
                }
                // CHECK ( ; before ; or |)
                if (list->type == END && list->next && (list->next->type == PIPE || list->next->type == END))
                {
                        printf("bash: syntax error near unexpected token %s\n", list->next->content);
                        return (0);
                }
		if (is_redirection(list) && (!list->next || list->next->type != WORD))
		{
			if (list->next)
				printf("bash: syntax error near unexpected token %s\n", list->next->content);
			else
				printf("bash: syntax error near unexpected token %s\n", list->content);
			return (0);
		}
                list = list->next;
        }
        return (1);
}
