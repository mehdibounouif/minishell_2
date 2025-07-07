#include "../../includes/minishell.h"

t_tree  *pars_one_side(t_node **list, t_env *env)
{
	t_tree	*node;
	int	i;
	char    *cmd;
	char    **args;
	int len;

	if (is_redirection(*list))
		return parss_redirection_in_start(list, env);
	i = 0;
	len = count_args(*list);
	cmd = (*list)->content;
	args = malloc(sizeof(char *) * len + 1);
	args[i++] = cmd;
	*list = (*list)->next;
	while ((*list) && (*list)->type == WORD)
	{
		args[i] = (*list)->content;
		i++;
		*list = (*list)->next;
	}
	args[i] = NULL;
	node = malloc(sizeof(t_tree));
	if (!node)
	{
		free_str(args);
		return (NULL);
	}
	node->command = malloc(sizeof(t_command));
	if (!node->command)
	{
		free_str(args);
		free(node);
		return (NULL);
	}
	node->command->command = cmd;
	node->command->args = args;
	node->type = COMMAND_NODE;
	if (is_redirection(*list))
		node = parss_redirection(node, list, env);
	return (node);
}


