#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_node {
    char            *data;
    struct s_node   *next;
    struct s_node   *prev;
} t_node;

// Create a new node
t_node *create_node(const char *data)
{
    t_node *node = malloc(sizeof(t_node));
    if (!node)
        return NULL;
    node->data = strdup(data); // duplicate string
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Print the list
void print_list(t_node *head)
{
    while (head)
    {
        printf("[%s]", head->data);
        if (head->next)
            printf(" <=> ");
        head = head->next;
    }
    printf("\n");
}

// Free the list
void free_list(t_node *head)
{
    t_node *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->data);
        free(tmp);
    }
}

// Insert sublist after a target node in main list
void insert_sublist_after(t_node *target, t_node *sub_head)
{
    if (!target || !sub_head)
        return;

    t_node *after = target->next;

    target->next = sub_head;
    sub_head->prev = target;

    sub_tail->next = after;
    if (after)
        after->prev = sub_tail;
}

int main(void)
{
    // Main list: A <=> B <=> C
    t_node *a = create_node("A");
    t_node *b = create_node("B");
    t_node *c = create_node("C");

    a->next = b;
    b->prev = a;
    b->next = c;
    c->prev = b;

    printf("Main List:\n");
    print_list(a);

    // Sublist: X <=> Y
    t_node *x = create_node("X");
    t_node *y = create_node("Y");
    x->next = y;
    y->prev = x;

    printf("Sublist:\n");
    print_list(x);

    // Insert sublist after node B
    insert_sublist_after(b, x);

    printf("List After Inserting Sublist After B:\n");
    print_list(a);

    // Clean up
    free_list(a); // includes all nodes now
    return 0;
}

