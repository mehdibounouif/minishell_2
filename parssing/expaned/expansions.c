/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:23:34 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/02 23:34:25 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	replace_key(char *cmd, t_share *share, t_env *list)
{
	char	*value;
	char	*key;
	int	l;

	l = 0;
	key = get_env_key(cmd, share->i);
	value = ft_getenv(key, list);
	if (!value)
		value = ft_strdup("");
	while (value[l])
		share->expanded_cmd[(share->j)++] = value[l++];
	share->i += (ft_strlen(key) + 1);
}

void	expand_cmd(char *cmd, t_share *share, t_env *env, int b_q)
{
	while (cmd[share->i])
	{
		while (is_dollar(cmd, share->i) && b_q != 1
			&& (ft_isalpha(cmd[share->i + 1]) || cmd[share->i + 1] == '_'))
				replace_key(cmd, share, env);
    if (!cmd[share->i])
      break;
		if (is_dollar(cmd, share->i) && cmd[share->i + 1] == '?' && b_q != 1)
			expand_exit_status(share);
		else
			share->expanded_cmd[share->j++] = cmd[share->i++];
	}
}

char	*expansion(char *cmd, t_env *env, int b_q)
{
	int		full_len;
	t_share	*share;

	share = ft_malloc(sizeof(t_share), 1);
	share->i = 0;
	share->j = 0;
	share->h = 0;
	full_len = get_full_len(cmd, env);
	if (full_len == 0)
		return (ft_strdup(""));
	share->expanded_cmd = ft_malloc(sizeof(char), full_len + 1);
	expand_cmd(cmd, share, env, b_q);
	share->expanded_cmd[share->j] = '\0';
	return (share->expanded_cmd);
}

t_node *insert_sublist(t_node *start, t_node *new, t_node *next)
{
  t_node *last;

    if (!new)
        return start;
    if (start)
    {
        start->next = new;
        new->prev = start;
    }
    last = new;
    while (last->next)
        last = last->next;
    last->next = next;
    if (next)
        next->prev = last;
    if (start)
        return start;
    else
        return new;
}

t_node *expand_list(t_node *tmp, t_env *env)
{
    t_node *head;
    t_node *node;
    int i;
    char *content;
    char **list;

    head = NULL;
    content = NULL;
    content = expansion(tmp->content, env, tmp->between_quoted);
    if (tmp->between_quoted)
    {
        node = create_node(content, tmp->b_space, tmp->between_quoted);
        //token_type(node);
        return (node);
    }
    i = 0;
    list = ft_split(content, ' ');
    while (list[i])
    {
        node = create_node2(list[i], tmp->b_space, tmp->type, tmp->between_quoted);
        //token_type(node);
        add_back(&head, node);
        i++;
    }
    return head;
}

void  free_node(t_node *node)
{
  if (node)
  {
    if (node->content)
      free(node->content);
    free(node);
  }
}

// void expand(t_node **list, t_env *env)
// {
//     int flag;
//     t_node (*start), (*next), (*sub_list), (*new_list), (*end), (*tmp), (*to_free);

//     tmp = *list;
//     flag = 0;
//     while (tmp)
//     {
//         if (tmp->type == HEREDOC)
//           flag = 1;
//         else if (flag)
//         {
//             tmp = tmp->next;
//             flag = 0;
//             continue;
//         }
//         start = tmp->prev;
//         next = tmp->next;
//         to_free = tmp;
//         sub_list = expand_list(tmp, env);
//         new_list = insert_sublist(start, sub_list, next);
//         if (!start)
//             *list = new_list;
//         end = new_list;
//         while (end && end->next != next)
//             end = end->next;
//         tmp = next;
//         free_node(to_free);
//     }
// }

// void expand(t_node **list, t_env *env)
// {
//     t_node *tmp = *list;
//     t_node *start, *next, *sub_list, *new_list, *end;

//     while (tmp)
//     {
//         if (tmp->type == HEREDOC)
//         {
//             tmp = tmp->next;
//             if (tmp)
//                 tmp = tmp->next;
//             continue;
//         }

//         start = tmp->prev;
//         next = tmp->next;

//         sub_list = expand_list(tmp, env);
//         if (!sub_list)
//         {
//             tmp = next;
//             continue;
//         }

//         new_list = insert_sublist(start, sub_list, next);
//         if (!start)
//             *list = new_list;

//         // Move tmp to the end of the newly inserted list
//         end = new_list;
//         while (end && end->next != next)
//             end = end->next;

//         tmp = next;
//         free_node(tmp->prev);  // assuming the old node is at tmp->prev now
//     }
// }


void expand(t_node **list, t_env *env)
{
    t_node *tmp = *list;
    t_node *start, *next, *sub_list, *new_list, *to_free;

    while (tmp)
    {
        if (tmp->type == HEREDOC)
        {
            tmp = tmp->next;
            if (tmp)
                tmp = tmp->next;
            continue;
        }

        start = tmp->prev;
        next = tmp->next;
        to_free = tmp;

        sub_list = expand_list(tmp, env);
        if (!sub_list)
        {
            tmp = next;
            continue;
        }

        new_list = insert_sublist(start, sub_list, next);

        if (!start)
            *list = new_list;

        tmp = next;
        free_node(to_free);  // now safe
    }
}

