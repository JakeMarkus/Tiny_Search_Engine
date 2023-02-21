/* query.c --- 
 * 
 * 
 * Author: Jacob R. Markus
 * Created: Sun Feb 19 16:29:08 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "indexio.h"
#include <string.h>
#define MAX_LINE 100

bool isChar(char c)
{
	int n = (int)c;
	
	if ( n < 65  || n  > 122)
		{
			return false;
		}
	else if (n > 90 && n < 97 )
		{
			return false;
		}
	
	return true;
}

static char* cleanInput(char* input)
{
	char* block = (char*)malloc(100*sizeof(char));
	strcpy(block, "");

	char* output = (char*)malloc(10000 * sizeof(char));
	
	for(int i = 0; i <= strlen(input); i++)
		{
			if((char)input[i] == ' ' || i == strlen(input))
				{
					if(strcmp(block, "") != 0)
						{
							strcat(output, block);
							
							if(i!= strlen(input))
								strcat(output, " ");
						}

					//free(block);
					strcpy(block, "");
				}
			else
				{
					if(!isChar((char)input[i]) && input[i] != 10)
						{
							return NULL;
						}
					else
						{
							char letter = tolower(input[i]);
							strncat(block, &letter, 1);
						}
				}
		}

	return output; 
}
int main(void)
{
	char* currline;

	char user_input[MAX_LINE];
	//strcpy(currline, "");
	char* currwords;
	
	while(true)
		{
			printf("> ");
			if((currline = fgets(user_input, MAX_LINE, stdin)) == NULL)
				{
					continue; 
				}
	 
			if((currwords = (char*)cleanInput(currline)) == NULL)
				{
					printf("Invalid Input!\n");
					continue;
				}
			
			//if(strcmp(currline, "q") == 0)
				//{
					//printf("yee");
					//FREE STUFF
					//exit(EXIT_SUCCESS);
					//break;
					//}
			printf("Cleaned Input: %s", currwords);
			
			strcpy(currline, "");
		}
}
