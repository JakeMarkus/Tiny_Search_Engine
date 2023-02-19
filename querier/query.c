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
#include <ctype.h>
#include <stdbool.h>
static void cleanInput(char* input)
{
	char* block = (char*)malloc(100*sizeof(char));
	strcpy(block, "");

	char*[] output = (char*[])malloc(10000 * sizeof(char));

	
	for(int i = 0; i <= strlen(input); i++)
		{
			if((char)input[i] == ' ' || i == strlen(input))
				{
					if(strcmp(block, "") != 0)
						{
							char* cpy;
							strcpy(cpy, block);
							output[i] = cpy; 
						}

					free(block);
					strcpy(block, "");
				}
			else
				{
					char letter = tolower(input[i]);
					strncat(block, letter, 1);
				}
		}

	return output; 
}
int main(void)
{
	char* currline;

	char** currwords;
	
	while(true)
		{
			printf("> ");
			scanf("%s", currline);
			currwords = (char**)cleanInput(currline);
			
			
			strcpy(currline, "");
		}
}
