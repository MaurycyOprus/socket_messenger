#include <stdlib.h>
#include <string.h>
#ifndef GET_SECOND
#define GET_SECOND


int second_line(char * filename){
    FILE * fd;
    char * line = NULL;
    int result;
    size_t len = 0;
    ssize_t read;

    fd = fopen(filename, "r");
    if (fd == NULL)
        exit(EXIT_FAILURE);

    for (int i = 1; i <= 2; i++){
        read = getline(&line, &len, fd);
    }
    result = atoi(line);
    fclose(fd);

    return result;
}

#endif