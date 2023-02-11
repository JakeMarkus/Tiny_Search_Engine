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
					return false;
				}
			else if (n > 90 && n < 97 )
				{
					return false;
				}
		}

	return true;
}

int NormalizeWord(char* input)
{
	if(strlen(input) < 3 || containsNonChars(input))
		{
			return 1; 
		}

	for(int i = 0; i < strlen(input); i++)
		{
			input[i] = tolower(input[i]);
		}
	return 0;
	
}

int main(void)
{
	webpage_t* first = pageload(1, "../pages/");

	char* savedword = "";
	int pos = 0;

	
	while(savedword != NULL)
		{
			if(webpage_getNextWord(first, pos, &savedword))
				{
					NormalizeWord(savedword);
					
					printf("%s\n", savedword);
					
					free(savedword);
				}
		}
	
}
