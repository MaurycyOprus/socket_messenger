#include <stdio.h>

#ifndef IF_EXISTS
#define IF_EXISTS

int search(char * word)
{     

	FILE * filePointer;
	int wordExist=0;
	int bufferLength = 255;
	char line[bufferLength];
	filePointer = fopen("users_online.txt", "r");
	while(fgets(line, bufferLength, filePointer))
	{
		char *ptr = strstr(line, word);
		if (ptr != NULL) 
		{
			wordExist=1;
			break;
		}
	}
	fclose(filePointer);
	return wordExist;
}

#endif