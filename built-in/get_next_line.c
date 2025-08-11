
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 10
#endif




#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>






char *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE];
    static int read_byt;
    static int position;
    int i = 0;
    char *line = malloc(1000);

    if (fd < 0 || BUFFER_SIZE <= 0 || !line)
        return NULL;
    while (1)
    {
        if (position >= read_byt)
        {
            read_byt = read(fd, buffer, BUFFER_SIZE);
            position = 0;
            if (read_byt <= 0)
                break;
        }
        if ((line[i++] = buffer[position++]) == '\n')
            break;
    }
    line[i] = '\0';
    if (i == 0)
    {
        free (line);
        return NULL;
    }
    return line;
}