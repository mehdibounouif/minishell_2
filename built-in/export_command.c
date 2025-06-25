#include "../includes/minishell.h"

static int is_valid_identifier(char *str)
{
    int i;

    if (!str || !str[0])
        return (0);
    
    // First character must be a letter or underscore
    if (!ft_isalpha(str[0]) && str[0] != '_')
        return (0);
    
    // Rest of the characters must be letters, numbers, or underscores
    i = 1;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

static void print_env_export(t_env *env)
{
    t_env *current = env;
    while (current)
    {
        ft_putstr_fd("declare -x ", 1);
        ft_putstr_fd(current->key, 1);
        if (current->value)
        {
            ft_putstr_fd("=\"", 1);
            ft_putstr_fd(current->value, 1);
            ft_putstr_fd("\"", 1);
        }
        ft_putchar_fd('\n', 1);
        current = current->next;
    }
}

void	ft_lstadd_node(t_env **lst, t_env *node)
{
	t_env	*temp;

	if (lst)
	{
		if (*lst)
		{
			temp = *lst;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = node;
		}
		else
			*lst = node;
	}
}



int export_command(t_env *env, char **args)
{
    int i;
    char *key;
    char *value;
    char *equal_sign;

    if (!args[1]) //no arg with export 
    {
        print_env_export(env);
        return (EXIT_SUCCESS);
    }
    i = 1;
    while (args[i])
    {
        if (!is_valid_identifier(args[i])) // check 
        {
            ft_putstr_fd("minishell: export: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            return (EXIT_FAILURE);
        }

        equal_sign = ft_strchr(args[i], '=');
        if (equal_sign)
        {
            key = ft_substr(args[i], 0, equal_sign - args[i]);
            // printf("%s\n", key);
            value = ft_strdup(equal_sign + 1);
            // printf("%s\n", value);
            if (!key || !value)
            {
                free(key);
                free(value);
                return (EXIT_FAILURE);
            }
            set_env_var(&env, key,value);
            print_env_export(env);
            free(key);
            free(value);
        }
        i++;
    }
    return (EXIT_SUCCESS);
}
