#include "../../includes/minishell.h"

t_tree  *parss_redirection(t_tree *node, t_node **list)
{
        t_tree  *redirect_node;

        redirect_node = NULL;
        if (is_redirection(*list))
        {
                redirect_node = malloc(sizeof(t_tree));
                if (!redirect_node)
                {
                        free_tree(&node);
                        return (NULL);
                }
                redirect_node->redirect = malloc(sizeof(t_redirection));
                if (!redirect_node->redirect)
                {
                        free_tree(&node);
                        free(redirect_node);
                        return (NULL);
                }
                redirect_node->type = REDIRECT_NODE;
                redirect_node->redirect->redirect = (*list)->content;
                *list = (*list)->next;
                while ((*list)->type == WORD && is_redirection((*list)->next))
                {
                        *list = (*list)->next;
                        *list = (*list)->next;
                }
                redirect_node->redirect->file = (*list)->content;
                redirect_node->redirect->redirection_type = (*list)->type;
                redirect_node->redirect->prev = node;
                *list = (*list)->next;
        }
        return (redirect_node);
}

t_tree  *pars_one_side(t_node **list)
{
        t_tree  *node;
        t_tree  *redirct_node;
        int     i;
        char    *cmd;
        char    **args;
        int len;

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
        redirct_node = parss_redirection(node, list);
        if (redirct_node)
                node = redirct_node;
        return (node);
}
