#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef REMOVE_LINE
#define REMOVE_LINE




void removing(char * path, char * toRemove){
    int BUFFER_SIZE = 1000;

    FILE * fPtr;
    FILE * fTemp;
    
    char buffer[1000];

    fPtr  = fopen(path, "r");
    fTemp = fopen("delete.tmp", "w"); 

    if (fPtr == NULL || fTemp == NULL)
    {
        printf("\nUnable to open file.\n");
        printf("Please check whether file exists and you have read/write privilege.\n");
        exit(EXIT_SUCCESS);
    }

    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL)
    {
        // Remove all occurrence of word from current line
        removeAll(buffer, toRemove);
        // Write to temp file
        fputs(buffer, fTemp);
    }

    /* Close all files to release resource */
    fclose(fPtr);
    fclose(fTemp);

    /* Delete original source file */
    remove(path);

    /* Rename temp file as original file */
    rename("delete.tmp", path);


    printf("\nAll occurrence of '%s' removed successfully.\n", toRemove);
}

/**
 * Remove all occurrences of a given word in string.
 */
void removeAll(char * str, const char * toRemove)
{
    int i, j, stringLen, toRemoveLen;
    int found;

    stringLen   = strlen(str);      // Length of string
    toRemoveLen = strlen(toRemove); // Length of word to remove


    for(i=0; i <= stringLen - toRemoveLen; i++)
    {
        /* Match word with string */
        found = 1;
        for(j=0; j < toRemoveLen; j++)
        {
            if(str[i + j] != toRemove[j])
            {
                found = 0;
                break;
            }
        }

        /* If it is not a word */
        if(str[i + j] != ' ' && str[i + j] != '\t' && str[i + j] != '\n' && str[i + j] != '\0') 
        {
            found = 0;
        }

        /*
         * If word is found then shift all characters to left
         * and decrement the string length
         */
        if(found == 1)
        {
            for(j=i; j <= stringLen - toRemoveLen; j++)
            {
                str[j - 1] = str[j + toRemoveLen + 1];
            }

            stringLen = stringLen - toRemoveLen;

            // We will match next occurrence of word from current index.
            i--;
        }
    }
}

#endif