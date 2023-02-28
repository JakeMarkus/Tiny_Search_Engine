/* query.c --- 
0;136;0c0;136;0c0;136;0c * 
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
#include "pageio.h"
#include "webpage.h"
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_LINE 100


int min(int a, int b)
{
	if(a <= b)
		return a;
	return b;
			
}

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

typedef struct {
	char* word;
	int count; 

} word_t;
	
typedef struct {
	char* url; 
	queue_t* words;
	int rank;
	int id;
} page_t;

static word_t* makeWord(char* w, int count)
{
	word_t* output = (word_t*) malloc(sizeof(word_t));

	output->word = (char*)malloc(sizeof(char) *(strlen(w)+5));

	strcpy(output->word, w);
	output->count = count;

	return output;
	
}


static void freeWord(void* word_s)
{

	word_t* wordT = (word_t*) word_s;
	free(wordT->word);
	//	free(word_s);
}

static page_t* makePage(char* url, int id)
{
	page_t* output = (page_t*) malloc(sizeof(page_t));
	output->id = id;
	output->url = (char*)malloc(sizeof(char)*(strlen(url)+5));
	
	strcpy(output->url, url);

	output->words = qopen();
	output->rank = 99999;
	return output; 
}

static void freePageAts(void* page)
{

	page_t* p = (page_t*)page;
	
	free(p->url);

	qapply(p->words, freeWord);
	
	qclose(p->words);
	
	//free(page);
}

static void printPage(void* page)
{

	page_t* pageT = (page_t*) page;
	printf("rank: %i: doc: %i: %s\n", pageT->rank, pageT->id, pageT->url);
}


bool pageTruer(void* e, const void* yee)
{
	return true;
}
queue_t* page_q; 


static char* cleanInput(char* input, hashtable_t* table, char* pagedir)
{

	if (page_q != NULL) qclose(page_q);

	page_q = qopen();

	int limit = getNumPages();

	for(int i = 1; i <= limit; i++) {

		//printf("idk\n");
			
			char* url = getUrl(pagedir, i);

			//printf("%s\n", url);

			page_t* page = makePage(url, i);

			free(url);
			qput(page_q, page);

		}
	

	
	int count; 
	//int min = 99999999;
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

							if(strcmp(block, "or") == 0 || strlen(block) >= 3)
								{

									if(strcmp(block, "or") != 0 && strcmp(block, "and") != 0)
										strcat(output, block);
									
									if(i!= strlen(input) && strcmp(block, "or")!=0 && strcmp(block, "and") != 0)
										{
											strcat(output, " ");
										}
									
									for(int i = 1; i <= getNumPages(); i++)
										{

											page_t* curr_page = (page_t*)qget(page_q);

											count = getCount(table, block, i);

											word_t* w = makeWord(block, count);

											qput(curr_page->words, w);
											
											
											//											printf("%s count: %i\n", curr_page->url, count);
											//if(count < curr_page->rank)
												//{
													//curr_page->rank = count; 
													//}

											qput(page_q, curr_page);
											
										}
									
									//printf("%s:%i ", block, count);
								}

							//else if(strcmp(block, "or") == 0)
								//{
									
									//}
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

	//printf(" -%i\n", min);
	free(block);

	return output; 
}

void rankPage(void* p)
{
	page_t* page = (page_t*)p;
		

  int rank = 0;

  word_t* prev_word = NULL;

  for(word_t* word = qget(page->words); word != NULL; word = qget(page->words))
    {
			if(prev_word != NULL)
				{
					printf("Word count: %i\n", word->count);
					if(strcmp(prev_word->word, "or") == 0)
						{
							rank += word->count;
						}
					else if( strcmp(word->word, "or") != 0 && strcmp(word->word, "and") != 0)
						{
							rank = min(rank, word->count);
						}
				}
			else
				{
					rank = word->count;
				}

			if(prev_word != NULL)
				{
					freeWord(prev_word);
					free(prev_word);
				}
			prev_word = word;
			
    }


	if(prev_word != NULL)
		{
			freeWord(prev_word);
			free(prev_word);
		}
	page->rank = rank;
}

bool isValid(page_t* page)
{
	bool result = true;
	
	queue_t* replacement = qopen();

	word_t* prev_word = NULL;

	int i = 0;
	
	for(word_t* word = qget(page->words); word != NULL; word = qget(page->words))
		{

			if(i ==0 && (strcmp(word->word, "and") == 0 || strcmp(word->word, "or") == 0))
				{
					result = false;
				}

			if(prev_word != NULL)
				{
					if(strcmp(prev_word->word, "and") == 0 || strcmp(prev_word->word, "or") == 0)
						{
							if(strcmp(word->word, "and") == 0 || strcmp(word->word, "or") == 0)
								{
									result = false;
								}
						}
				}
			
			
			prev_word = word;
			qput(replacement, word);
			i++;
		}

	if(strcmp(prev_word->word, "and") == 0 || strcmp(prev_word->word, "or") ==0)
		result = false;
	
	qclose(page->words);
	page->words = replacement;

	return result;
}

bool quiet;

bool checkArgs(int argnum, char* args[]) {

	if (argnum < 3 || argnum > 6) {
		printf("usage: query <pageDirectory> <indexFile> [-q]\n");
		return false;
	}

	if(access(args[2], R_OK) != 0) {
		printf("error: cannot access index file\n");
		return false;
	}

	if (argnum > 3) {

		if (strcmp(args[3], "-q") != 0) {
			printf("usage: query <pageDirectory> <indexFile> [-q]\n");
			return false;
		}

		else {

			if (argnum != 6) {
				printf("usage: query <pageDirectory> <indexFile> [-q]\n");
				return false;
			}

			if (access(args[4], R_OK) != 0) {
				printf("error in reading input file\n");
				return false;
			}

			quiet = true;
			return true;
		}
	}

	quiet = false;
	return true;
}

	
int main(int argc, char* argv[]) {

	bool error = checkArgs(argc, argv);

	if (!error) {
		printf("error with code %d, aborting\n", error);
		exit(EXIT_FAILURE);
	}
	
	char* currline;

	char user_input[MAX_LINE];
	//strcpy(currline, "");
	char* currwords;
	hashtable_t* table = indexload(argv[2]);
	
	while(true)
		{
			printf("> ");
			if((currline = fgets(user_input, MAX_LINE, stdin)) == NULL)
				{
					printf("\n");
					freeIndexTable(table);
					//qapply(page_q, freePageAts);
					if(page_q != NULL)
						qclose(page_q);
					//freePageQueue(page_q);
					exit(EXIT_SUCCESS);

					
				}

			currline[strcspn(currline, "\n")] = 0;
			
			if((currwords = (char*)cleanInput(currline, table, argv[1])) == NULL || strcmp(currwords, "") == 0)
				{
					printf("Invalid Input!\n");
					//qapply(page_q, freePageAts);
					free(currwords);
					qapply(page_q, freePageAts);
					if(page_q != NULL)
						{
							qclose(page_q);
						}
					continue;
				}

			else if (!isValid(qsearch(page_q, pageTruer, NULL)))
				{
					printf("Invalid Input!\n");
					free(currwords);
					qapply(page_q, freePageAts);
					//					qclose(page_q);
					continue;
				}
			
			qapply(page_q, rankPage);
			qapply(page_q, printPage);
			printf("Cleaned Input: %s\n", currwords);

			free(currwords);
			strcpy(currline, "");

			
			qapply(page_q, freePageAts);
			
		}
}
