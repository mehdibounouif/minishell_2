#include "../includes/minishell.h"

t_tree *parssing(char *cmd, t_node *list)
{
  t_tree  *tree;

  if (check_quotes(cmd, ft_strlen(cmd)))
  {
      printf("qoutes not closed!\n");
      return (NULL);
  }
  tokenize(cmd, &list);
  if (check_sides(list))
  {
    printf("-minishell: syntax error near unexpected token !\n");
	  ft_free(&list, NULL);
    return (NULL);
  }
	tree = pars_command(&list);
  return (tree);
}
