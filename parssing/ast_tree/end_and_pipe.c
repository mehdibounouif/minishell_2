#include "../../includes/minishell.h"

t_tree  *pars_pipe(t_node **list)
{
        t_tree *left;

        left = pars_one_side(list);
        if (!left)
                return (NULL);
        while (*list && (*list)->type == PIPE)
        {
                *list = (*list)->next;
                t_tree  *right = pars_one_side(list);
                t_tree  *pipe_cmd = malloc(sizeof(t_tree));
                if (!pipe_cmd)
                {
                        free_tree(&right);
                        free_tree(&left);
                        return (NULL);
                }
                pipe_cmd->pipe = malloc(sizeof(t_pipe));
                if (!pipe_cmd->pipe)
                {
                        free_tree(&right);
                        free_tree(&left);
                        free(pipe_cmd);
                        return (NULL);
                }
                pipe_cmd->pipe->right = right;
                pipe_cmd->pipe->left = left;
                pipe_cmd->type = PIPE_NODE;
                left = pipe_cmd;
        }
        return left;
}

t_tree  *pars_end(t_node **list)
{
        t_tree  *left;

        left = pars_pipe(list);
        if (!left)
                return (NULL);
        while (*list && (*list)->type == END)
        {
                if ((*list)->next && (*list)->type == END)
                {
                        *list = (*list)->next;
                        t_tree  *right = pars_pipe(list);
                        t_tree  *end_cmd = malloc(sizeof(t_tree));
                        if (!end_cmd)
                        {
                                free_tree(&right);
                                free_tree(&left);
                                return (NULL);
                        }
                        end_cmd->end = malloc(sizeof(t_end));
                        if (!end_cmd->end)
                        {
                                free_tree(&right);
                                free_tree(&left);
                                free(end_cmd);
                                return (NULL);
                        }
                        end_cmd->end->right = right;
                        end_cmd->end->left = left;
                        end_cmd->type = END_NODE;
                        left = end_cmd;
                }
                else if (!(*list)->next && (*list)->type == END)
                {
                        t_tree  *end_cmd = malloc(sizeof(t_tree));
                        if (!end_cmd)
                        {
                                free_tree(&left);
                                return (NULL);
                        }
                        end_cmd->end = malloc(sizeof(t_end));
                        if (!end_cmd->end)
                        {
                                free_tree(&left);
                                free(end_cmd);
                                return (NULL);
                        }
                        end_cmd->end->left = left;
                        end_cmd->end->right = NULL;
                        end_cmd->type = END_NODE;
                        left = end_cmd;
                        break;
                }
        }
        return (left);
}
