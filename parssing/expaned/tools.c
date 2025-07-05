#include "../../includes/minishell.h"
/*
int     get_special_len(char *cmd, int i)
{
        int     j;

        j = i;
        if (cmd[i] == '[')
        {
                i++;
                while (cmd[i] && cmd[i] != ']' && !is_dollar(cmd, i))
                        i++;
                i++;
        }
        else if (cmd[i] == '{')
        {
                i++;
                while (cmd[i] && cmd[i] != '}' && !is_dollar(cmd, i))
                        i++;
                i++;
        }
        else
                return (1);
        return (i - j);
}

char    *check_square_bracket_alpha(char *key, int i)
{
        while (key[i] && key[i] != ']')
        {
                if (is_space(key[i]))
                {
                        while(key[i] && is_space(key[i]))
                                i++;
                        if (key[i] != ']')
                                return ("failed"); // error like $[1234#]
                }
                if (!ft_isalpha(key[i]) && !ft_isdigit(key[i]) && key[i] != ']')
                        return ("failed"); // error like $[aasjfkd#]
                i++;
        }
        return (ft_strdup("0"));
}

char    *check_square_bracket_digit(char *key, int i)
{
        int     x;
        int     j;
        char    *word;

        x = 0;
        j = ft_strlen(key);
        word = malloc(j - 1);
        while (key[i] && key[i] != ']')
        {
                if (is_space(key[i]))
                {
                        while(key[i] && is_space(key[i]))
                                i++;
                        if (key[i] != ']')
                        {
                                free(word);
                                return ("failed"); // error like $[1234#]
                        }
                }
                if (!ft_isdigit(key[i]) && key[i] != ']')
                {
                        free(word);
                        return ("failed"); // error like $[1234#]
                }
                if (key[i] == ']')
                        break;
                word[x++] = key[i++];
        }
        word[x] = '\0';
        return (word);
}

char    *check_square_bracket(char *key)
{
        int     i;

        i = 1;
        while(is_space(key[i]))
                i++;
        if (ft_isdigit(key[i]))
                return (check_square_bracket_digit(key, i));
        else if (ft_isalpha(key[i]) || key[i] == ']')
                return (check_square_bracket_alpha(key, i));
        else
                return ("failed");
}

char    *check_curly_brace(char *key)
{
        int     i;

        i = 0;
        while (key[i] && key[i] != '}')
        {
                if ((i == 0 && key[i+1] == '}')
                                || is_space(key[i]))
                        return ("failed");
                i++;
        }
        return (ft_strdup(""));
}
*/
