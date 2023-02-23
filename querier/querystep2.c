/* query.c --- 
0;136;0c * 
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
#include <string.h>
#include "hash.h"
#include "indexio.h"

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

static char* cleanInput(char* input, hashtable_t* table)
{

	int min = 99999999;
	char* block = (char*)malloc(100*sizeof(char));
	strcpy(block, "");

	char* output = (char*)malloc(10000 * sizeof(char));

	strcpy(output, "");
	
	for(int i = 0; i <= strlen(input); i++)
		{
			if((char)input[i] == ' ' || i == strlen(input))
				{
					if(strcmp(block, "") != 0)
						{
							strcat(output, block);
							
							if(i!= strlen(input))
								{
									strcat(output, " ");
								}

							//							printf("block: |%s|\n", block);
							int count = getCount(table, block, 1);
							//printf("yee0.1\n");
							if(count < min)
								{
									min = count; 
								}
							
							printf("%s:%i ", block, count);
							//printf("yee0.2\n");
						}
					
					free(block);
					
					block = (char*)malloc(100*sizeof(char));
					strcpy(block, "");
					//printf("yee1\n");
				}
			else
				{
					if(!isChar((char)input[i]) && input[i] != 10)
						{
							free(block);
							free(output);
							return NULL;
						}
					else
						{
							char letter = tolower(input[i]);
							strncat(block, &letter, 1);
						}
				}
		}
	//printf("yee2\n");

	printf(" - min: %i\n", min);
	free(block);
	return output; 
}
int main(void)
{
	char* currline;

	char user_input[MAX_LINE];
	//strcpy(currline, "");
	char* currwords;
	hashtable_t* table = indexload("../indexer/here");
	
	while(true)
		{
			printf("> ");
			if((currline = fgets(user_input, MAX_LINE, stdin)) == NULL)
				{
					printf("\n");
					freeIndexTable(table);
					exit(EXIT_SUCCESS);
				}

			currline[strcspn(currline, "\n")] = 0;
			
			if((currwords = (char*)cleanInput(currline, table)) == NULL)
				{
					printf("Invalid Input!\n");
					continue;
				}
			
			printf("Cleaned Input: %s\n", currwords);

			free(currwords);
			strcpy(currline, "");
		}
}
