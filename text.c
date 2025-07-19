#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

int main(int c, char **v) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(v[1]);  // Replace with your directory path
    if (dir == NULL) {
		write(1, "minishell : ", 20);
        perror(v[1]);
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);  // Print the name of each entry
    }

    closedir(dir);  // Always close the directory stream
    return EXIT_SUCCESS;
}


/*
int global(int state)
{
	static int value;
	if(state != -1)
		value = state;
	return value;
}

t_mini *get_global_pointer(t_mini *pointer)
{
	static t_mini *returned_ptr;
	if(pointer != NULL)
		returned_ptr = pointer;
	return returned_ptr;
}

int value = 0;
global(0);
globl(-1);
*/
