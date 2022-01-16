#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef REMOVE_SOCK
#define REMOVE_SOCK


void delete_line(char * filename){
    FILE * fd1;
    FILE * fd2;
    char * line = NULL;
    int result;
    size_t len = 0;
    ssize_t read;

    fd1 = fopen(filename, "r");
    fd2 = fopen("temp.txt", "w");

    if (fd1 == NULL)
        exit(EXIT_FAILURE);

    for (int i = 1; i <= 1; i++){
        read = getline(&line, &len, fd1);
        // fputs(line, fd2);
        strtok(line, "\n");
        fprintf(fd2, line);
    }
    fclose(fd1);
    fclose(fd2);
    remove(filename);
    rename("temp.txt", filename);
}
#endif