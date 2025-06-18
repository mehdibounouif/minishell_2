#include "../includes/minishell.h"

int check_quotes(char *cmd, size_t i)
{
  size_t j;
  int flag;

  j = 0;
  flag = 0;
  while (cmd[j] && j != i)
  {
    if (j > 0 && cmd[j - 1] == '\\')
    {
      j++;
      continue;
    }
    else if (flag == 0 && cmd[j] == '\"')
      flag = 1;
    else if (flag == 0 && cmd[j] == '\'')
      flag = 2;
    else if (flag == 1 && cmd[j] == '\"')
      flag = 0;
    else if (flag == 2 && cmd[j] == '\'')
      flag = 0;
    j++;
  }
  return (flag);
}

char  *remove_quotes(char *cmd)
{
  int i;
  int j;
  char  *clear_cmd;

  i = 0;
  j = 0;
  while (cmd[i])
  {
    if (cmd[i] == '\'' || cmd[i] == '\"')
      j++;
    i++;
  }
  clear_cmd = malloc((i - j) + 1);
  i = 0;
  j = 0;
  while (cmd[i])
  {
    if (cmd[i] == '\'' || cmd[i] == '\"')
      i++;
    else
      clear_cmd[j++] = cmd[i++];
  }
  clear_cmd[j] = '\0';
  return (clear_cmd);
}
