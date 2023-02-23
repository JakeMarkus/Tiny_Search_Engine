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

typedef struct
{
	char* word;
	int count; 
} word_t;
	
typedef struct
{
	char* url; 
	queue_t* words;
} page_t;

static word_t* makeWord(char* w, int count)
{
	word_t* output = (word_t*) malloc(sizeof(word_t));

	strcpy(output->word, w);
	output->count = count;

	return output;
	
}


static void freeWord(word_t* word_s)
{
	free(word_s->word);
	free(word_s);
}

static page_t* makePage(char* url)
{
	page_t* output = (page_t*) malloc(sizeof(page_t));
	
	strcpy(output->url, url);

	return output; 
}

static void freePage(page_t* page)
{
	free(page->url);
	free(page);
}


queue_t* page_q; 

static char* cleanInput(char* input, hashtable_t* table, char* pagedir)
{

	page_q = qopen();

	for(int i = 0; i < getNumPages(); i++)
		{
			qput(page_q, makePage(getUrl(pagedir, i)));
		}
	

	
	int count; 
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

							if(strlen(block) >= 3)
								{
									
									for(int i = 0; i < getNumPages(); i++)
										{

											page_t* curr_page = (page_t*)qget(page_q);
											
											count = getCount(table, block, i);

											word_t* w = makeWord(block, count);

											qput(curr_page->words, w);
											
											
											
											//if(count < min)
												//{
													//min = count; 
													//}
											
										}
									
									//printf("%s:%i ", block, count);
								}

							else if(strcmp(block, "or") == 0)
								{
									
								}
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

	printf(" -%i\n", min);
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
