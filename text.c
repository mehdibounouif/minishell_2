#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
typedef struct s_node {
	int	contain_quoted;
	int	between_quoted;
	int type;
	int b_space;
	char *content;
	struct s_node *next;
	struct s_node *prev;
}	t_node;


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *strjoin_and_free(char *s1, char *s2)
{
	size_t  len1 = 0;
	size_t  len2 = 0;
	if (strlen(s1))
		len1 = strlen(s1);
	if (strlen(s2))    
		len2 = strlen(s2);
    char *joined = malloc(len1 + len2 + 1);
    if (!joined)
        return NULL;
    if (s1)
        strcpy(joined, s1);
    if (s2)
        strcpy(joined + len1, s2);
    free(s1);
    return joined;
}

t_node *create_node(const char *content, int b_space)
{
    t_node *new = malloc(sizeof(t_node));
    if (!new)
        return NULL;
    new->content = strdup(content);
    new->b_space = b_space;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void remove_node(t_node **head, t_node *node)
{
    if (!node)
        return;
    if (node->prev)
        node->prev->next = node->next;
    else
        *head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    free(node->content);
    free(node);
}

void join_b_space_nodes(t_node **head)
{
    t_node *curr = *head;
    while (curr)
    {
        if (curr->b_space == 1)
        {
            t_node *start = curr;
            char *joined = NULL;
            // Join all consecutive b_space == 1
            while (curr && curr->b_space == 1)
            {
                joined = strjoin_and_free(joined, curr->content);
                curr = curr->next;
            }
            // Create new node with joined content
            t_node *new_node = create_node(joined, 0);
            free(joined);
            if (!new_node)
                return;
            // Insert new node before start
            new_node->prev = start->prev;
            new_node->next = curr;
            if (start->prev)
                start->prev->next = new_node;
            else
                *head = new_node;
            if (curr)
                curr->prev = new_node;
            // Remove original nodes
            t_node *tmp;
            while (start != curr)
            {
                tmp = start->next;
                remove_node(head, start);
                start = tmp;
            }
        }
        else
        {
            curr = curr->next;
        }
    }
}
