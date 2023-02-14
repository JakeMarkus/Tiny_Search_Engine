/* indexer.c --- 
 * 
 * 
 * Author: Jacob R. Markus
 * Created: Fri Feb 10 18:51:16 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include "pageio.h"
#include "webpage.h"

static bool containsNonChars(char * word)
{
	size_t leng = strlen(word);
	
	for(int i = 0; i < leng; i++)
		{
			int n = (int)word[i]; //decimal rep. of each char in word
			
			if ( n < 65  || n  > 122)
				{
					return true;
				}
			else if (n > 90 && n < 97 )
				{
					return true;
				}
		}

	return false;
}

static bool NormalizeWord(char* input)
{
	//printf("Len: %i, containsNonChars: %d", strlen(input), containsNonChars(input));

	if(input == NULL)
		{
			return false;
		}
	
	if(strlen(input) < 3 || containsNonChars(input))
		{
			return false; 
		}

	for(int i = 0; i < strlen(input); i++)
		{
			input[i] = tolower(input[i]);
		}
	
	return true;
	
}

int main(void)
{

	webpage_t* first = pageload(1, "../pages/");

	char* savedword = "";
	int pos = 0;
	
	while((pos = webpage_getNextWord(first, pos, &savedword)) > 0)
				{
					if(NormalizeWord(savedword))
						{
							printf("%s\n", savedword);
						}

					if(savedword != NULL)
						{
							free(savedword);
						}
					
				}


	printf("%p\n", (void*)first);

	//printf("Url: %s, HTML %s", webpage_getURL(first), webpage_getHTML(first));
	
	//webpage_delete(first);
	//printf("After\n");
	
	exit(EXIT_SUCCESS);
			
}

