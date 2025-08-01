#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

typedef struct s_node {
	int	contain_quoted;
	int	between_quoted;
	int type;
	char *content;
	struct s_node *next;
	struct s_node *prev;
}	t_node;

size_t	ft_strlen(char const *str)
{
	size_t i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	exact_len;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return ((char *) malloc(sizeof(char)));
	if (ft_strlen(s) <= start + len)
		exact_len = ft_strlen(s) - start;
	else
		exact_len = len;
	substr = malloc(sizeof(char) * exact_len + 1);
	if (!substr)
		return (NULL);
	while (s[start] && i < len)
		substr[i++] = s[start++];
	substr[i] = '\0';
	return (substr);
}

char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*res;
	size_t	len;

	if(!s)
		return NULL;
	len = ft_strlen(s) + 1;
	res = malloc(sizeof(char) * len);
	if (!res)
		return (NULL);
	i = 0;
	while (s[i])
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
int	is_qoute(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

size_t	is_sep(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

size_t	get_close_token(char *cmd, char c)
{
	int	len;

	len = 1;
	while (cmd[len] != c)
	{
		len++;
	}
	return (len + 1);
}

size_t	get_separetor(char *cmd, char c)
{
	int	len;

	len = 0;
	while (cmd[len] == c)
	{
		len++;
	}
	return (len);
}

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

void	skip_space(char *cmd, int *i)
{
	while (cmd[*i] && is_space(cmd[*i]))
		(*i)++;
}

size_t	get_token_len(char *cmd)
{
	int	len;

	len = 0;
	while (cmd[len] && !is_qoute(cmd[len]) && !is_space(cmd[len]) && !is_sep(cmd[len]))
	{
		len++;
	}
	return (len);
}

t_node	*new_node(char *content)
{
	t_node	*node;
	int	len;

	node = malloc(sizeof(t_node));
	node->content = ft_strdup(content);
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	add_back(t_node **list, t_node *node)
{
	t_node	*tmp;
	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else 
	{
		tmp = *list;
		while (tmp && tmp->next)
			tmp = tmp->next;
		tmp->next = node;
		node->prev = tmp;
	}
}
int	between_quoted(char *cmd, int len)
{
	if (!cmd)
		return (0);
	if (cmd[0] == '\'' && cmd[len - 1] == '\'')
		return (1);
	else if (cmd[0] == '\"' && cmd[len - 1] == '\"')
		return (2);
	else
		return (0);
}
void	tokenize2(char *cmd, t_node **list)
{
	int	i;
	int	start;
	size_t	len;
	char	*content;
	t_node *token;

	i = 0;
	token = NULL;
	len = 0;
	while (cmd[i])
	{
		while (cmd[i] == ' ')
			i++;
//		skip_space(&cmd[i], &i);
		if (is_qoute(cmd[i]))
			len = get_close_token(&cmd[i], cmd[i]);
		else if (is_sep(cmd[i]))
		{
			len = get_separetor(&cmd[i], cmd[i]);
			if (len > 2)
			{
				printf("syntax error\n");
				break;
			}
		}
		else
			len = get_token_len(&cmd[i]);
		start = i;
		i += len;
		content = ft_substr(cmd, start, len);
		add_back(list, new_node(content));
	}
}

void	print_list(t_node *list)
{
	while (list)
	{
		printf("%s\n", list->content);
		int	 a = between_quoted(list->content, ft_strlen(list->content));
		if (a)
			printf("qouted with %d\n", a);

		list = list->next;
	}
}
int main(int c, char **v)
{
	t_node	*list;

	list = NULL;
	
	tokenize2("echo \"'\"HOME\"'\" \"mehdi\"  'bounouif'", &list);
	print_list(list);
}
