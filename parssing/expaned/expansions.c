#include "../../includes/minishell.h"

int	is_dollar(char *cmd, int i)
{
	if (cmd[i] == '$' && cmd[i+1] && check_quotes(cmd, i) != 2
			&& cmd[i-1] != '\\')
		return (1);
	return (0);
}

int	get_env_len(char *cmd, int i)
{
	int	j;

	j = 0;
	while (cmd[i] && (ft_isalpha(cmd[i]) || cmd[i] == '_'))
	{
		i++;
		j++;
	}
	return (j);
}


char	*get_env_key(char *cmd, int i)
{
	int	len; char	*key;

	i++;
	len = get_env_len(cmd, i);
	key = ft_substr(cmd, i, len);
	return (key);
}


int	get_full_len(char *cmd, t_env *list)
{
	int	i;
	size_t	full_len;
	size_t	cmd_len;
	char	*value;
	char	*key;

	i = 0;
	full_len = 0;
	cmd_len = ft_strlen(cmd);
	key = NULL;
	while (cmd[i])
	{
		if (is_dollar(cmd, i) && (ft_isalpha(cmd[i+1]) || cmd[i+1] == '_'))
		{
			key = get_env_key(cmd, i++);
			value = ft_getenv(key, list);
			if (!value)
				value = ft_strdup("");
			cmd_len -= (ft_strlen(key) + 1);
			full_len += ft_strlen(value);
			free(key);
		}
		i++;
	}
	return (cmd_len += full_len);
}

void	replace_key(char *cmd, char *expanded_cmd, int *i, int *j, t_env *list)
{
	char	*value;
	char	*key;
	int	f;
	int	l;

	l = 0;
	f = 0;
	key = get_env_key(cmd, *i);
	value = ft_getenv(key, list);
	if (cmd[*i-1] == '\"' && l == 0)
		f = 1;
	if (value)
	{
		while (value[l])
		{
			if (value[l] == '\'' && f == 0)
			{
				expanded_cmd[(*j)++] = '\"';
				f = 2;
			}
			expanded_cmd[(*j)++] = value[l++];
			if (value[l] == '\'' && f == 2)
			{
				expanded_cmd[(*j)++] = value[l++];
				expanded_cmd[(*j)++] = '\"';
				f = 0;
			}
		}
	}
	(*i) += (ft_strlen(key) + 1);
	free(key);
}

void	expand_exit_status(char *expanded_cmd, int *i, int *j)
{
	int	exit_status;
	char *value;
	int	l;

	exit_status = global(-1);
	value = ft_itoa(exit_status);
	l = 0;
	while (value[l])
		expanded_cmd[(*j)++] = value[l++];
	(*i) += 2;
}

char	*expansion(char *cmd, t_env *list)
{
	int	full_len;
	char	*expanded_cmd;
	int	i;
	int	quote;
	int	j;
	int	l;

	i = 0;
	j = 0;
	l = 0;
	quote = 0;
	full_len = get_full_len(cmd, list);
	if (full_len == -1)
		return (NULL);
	expanded_cmd = malloc(full_len + 1);
	if (!expanded_cmd)
	{
		free_env(list);
		free(cmd);
		exit(2);
	}
	while (cmd[i])
	{
		if (cmd[i] == '<' && cmd[i+1] == '<')
		{
			l = 1;
			expanded_cmd[j++]  = cmd[i++];
			expanded_cmd[j++]  = cmd[i++];
			while (is_space(cmd[i]))
				expanded_cmd[j++] = cmd[i++];
			while (cmd[i] == '\'' || cmd[i] == '\"')
			{
				i++;
				quote = 1;
			}
		}
		while (l == 0 && is_dollar(cmd, i) && (ft_isalpha(cmd[i+1]) || cmd[i+1] == '_'))
				replace_key(cmd, expanded_cmd, &i, &j, list);
		if (is_dollar(cmd, i) && cmd[i+1] == '?')
			expand_exit_status(expanded_cmd, &i, &j);
		if (is_dollar(cmd, i) && !check_quotes(cmd, i) // exmaple $"HOME"
			&& (cmd[i+1] == '\'' || cmd[i+1] == '\"'))
			i++;
//		if (quote && (cmd[i] == '\'' || cmd[i] == '\"'))
//			i++;
		else
			expanded_cmd[j++] = cmd[i++];
		l = 0;
	}
	free(cmd);
	expanded_cmd[j] = '\0';
	return (expanded_cmd);
}
/*
char	*find_mark(t_tree *tree, char *str)
{
	int	full_len;

	full_len = get_full_len();
}

void	expand_question_mark(t_tree *tree, t_node **list)
{
	t_node *tmp;
	int	len;

	tmp = *list;
	while (tmp)
	{
		len = ft_strlen(tmp->content);
		if (between_quoted(tmp->content, len) == 2)
		{
//			find_mark(tree, tmp->content);
		}
		tmp = tmp->next;
 	}
}
*/
