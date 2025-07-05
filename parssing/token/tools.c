#include "../../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i])
	{
		if (s1[i] == '\0' && s2[i] == '\0')
			return (0);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	token_type(t_node *node, int flag)
{
	if (!node)
		return ;
	if (ft_strcmp(node->content, "") == 0)
		node->type = EMPTY;
	else if (ft_strcmp(node->content, ">") == 0 && flag == 0)
		node->type = R_OUT;
	else if (ft_strcmp(node->content, ">>") == 0 && flag == 0)
		node->type = R_APPEND;
	else if (ft_strcmp(node->content, "<<") == 0 && flag == 0)
		node->type = HEREDOC;
	else if (ft_strcmp(node->content, "<") == 0 && flag == 0)
		node->type = R_IN;
	else if (ft_strcmp(node->content, "|") == 0 && flag == 0)
		node->type = PIPE;
	else if (ft_strcmp(node->content, ";") == 0 && flag == 0)
		node->type = END;
	else
		node->type = WORD;
}

int		calc_token_byte(char *line, int *i)
{
	int		count;
	int		j;
	char	c;

	count = 0;
	j = 0;
	c = ' ';
	while (line[*i + j] && (line[*i + j] != ' ' || c != ' '))
	{
		if (c == ' ' && (line[*i + j] == '\'' || line[*i + j] == '\"'))
			c = line[*i + j++];
		else if (c != ' ' && line[*i + j] == c)
		{
			count += 2;
			c = ' ';
			j++;
		}
		else
			j++;
		if (line[*i + j - 1] == '\\')
			count--;
	}
	return (j - count + 1);
}

t_node	*get_token(char *cmd, int *i)
{
	t_node	*node;
	int		j;
	char	c;

	j = 0;
	c = ' ';
	if (!(node = malloc(sizeof(t_node))))
		return (NULL);
	if (!(node->content = malloc(sizeof(char) * calc_token_byte(cmd, i))))
		return (NULL);
	while (cmd[*i] && (cmd[*i] != ' ' || c != ' '))
	{
		if (c == ' ' && (cmd[*i] == '\'' || cmd[*i] == '\"'))
			c = cmd[(*i)++];
		else if (c != ' ' && cmd[*i] == c)
		{
			c = ' ';
			(*i)++;
		}
		else if (cmd[*i] == '\\' && (*i)++)
			node->content[j++] = cmd[(*i)++];
		else
			node->content[j++] = cmd[(*i)++];
	}
	node->content[j] = '\0';
	return (node);
}

int	check_real_sep(char *line, int i)
{
	if (line[i] && line[i + 1] && ft_strncmp(&line[i], "\\;", 2) == 0)
		return (1);
	else if (line[i] && line[i + 1] && ft_strncmp(&line[i], "\\|", 2) == 0)
		return (1);
	else if (line[i] && line[i + 1] && ft_strncmp(&line[i], "\\>", 2) == 0)
		return (1);
	else if (line[i] && line[i + 1] && line[i + 2] && ft_strncmp(&line[i], "\\>>", 3) == 0)
		return (1);
	return (0);
}
