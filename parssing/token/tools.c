#include "../../includes/minishell.h"

t_node  *check_pipe(int *i, int flag)
{
        t_node *node;

        node = malloc(sizeof(t_node));
        if (!node)
                return NULL;
        node->content = ft_strdup("|");
        if (flag)
                node->type = PIPE;
        else
                node->type = WORD;
        node->next = NULL;
        node->prev = NULL;
        (*i)++;
        return (node);
}

t_node  *check_word(char *command, int *i)
{
        t_node  *node;
        int     start;
        char *word;
        start = *i;

        node = malloc(sizeof(t_node));
        if (!node)
                return NULL;
        if (command[*i] == '\"' || command[*i] == '\'')
                (*i)++;
        if (!check_quotes(command, *i+1))
                while (command[*i] && !is_space(command[*i])
                          && command[*i] != '>' && command[*i] != '<'
                        && command[*i] != '&' && command[*i] != '|' && command[*i] != ';')
                  (*i)++;
        else
        {
                while (command[*i] && check_quotes(command, *i+1) != 0)
                        (*i)++;
                (*i)++;
        }
        word = malloc(sizeof(int) * (*i - start));
        if (!word)
                return NULL;
        ft_strlcpy(word, command+start, (*i - start) + 1);
        node->content = word;
        node->next = NULL;
        node->prev = NULL;
        node->type = WORD;
        return (node);
}

t_node  *check_redirection(char *command, int *i, int flag)
{
        t_node *node;

        node = malloc(sizeof(t_node));
  if (!node)
      return NULL;
        if (command[*i] == '>' && command[*i+1] == '>')
        {
                node->content = ft_strdup(">>");
    if (flag)
                  node->type = R_APPEND;
                *i += 2;
        }
        else if (command[*i] == '<' && command[*i+1] == '<')
        {
                node->content = ft_strdup("<<");
    if (flag)
                  node->type = HEREDOC;
                *i += 2;
        }
        else if (command[*i] == '2' && command[*i+1] == '>')
        {
                node->content = ft_strdup("2>");
    if (flag)
                  node->type = R_ERR;
                *i += 2;
        }
        else
        {
                if (command[*i] == '>')
                {
                        node->content = ft_strdup(">");
      if (flag)
                        node->type = R_OUT;
                }
                else
                {
                        node->content = ft_strdup("<");
      if (flag)
                          node->type = R_IN;
                }
                (*i)++;
        }
  if (!flag)
    node->type = WORD;
        node->next = NULL;
        node->prev = NULL;
        return (node);
}

t_node  *check_end(int *i, int flag)
{
        t_node *node;

        node = malloc(sizeof(t_node));
        if (!node)
                return NULL;
        node->content = ft_strdup(";");
        if (flag)
                node->type = END;
        else
                node->type = WORD;
        node->next = NULL;
        node->prev = NULL;
        (*i)++;
        return (node);
}
