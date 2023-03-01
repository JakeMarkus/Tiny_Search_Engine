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
#include "lqueue.h"

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
	lqueue_t* words;
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

	lqapply(p->words, freeWord);
	
	lqclose(p->words);
	
	//free(page);
}

static void printPage(void* page)
{

	page_t* pageT = (page_t*) page;
	printf("rank: %i: doc: %i: %s\n", pageT->rank, pageT->id, pageT->url);
}

FILE* queries_output;

static void printPageFile(void* page) {
	page_t* pageT = (page_t*) page;
	fprintf(queries_output, "rank: %i: doc: %i: %s\n", pageT->rank, pageT->id, pageT->url);
}


bool pageTruer(void* e, const void* yee)
{
	return true;
}
lqueue_t* page_q; 


static char* cleanInput(char* input, hashtable_t* table, char* pagedir)
{

	if (page_q != NULL) lqclose(page_q);

	page_q = lqopen();

	int limit = getNumPages();

	for(int i = 1; i <= limit; i++) {

		//printf("idk\n");
			
			char* url = getUrl(pagedir, i);

			//printf("%s\n", url);

			page_t* page = makePage(url, i);

			free(url);
			lqput(page_q, page);

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

											page_t* curr_page = (page_t*)lqget(page_q);

											count = getCount(table, block, i);

											word_t* w = makeWord(block, count);

											lqput(curr_page->words, w);
											
											
											//											printf("%s count: %i\n", curr_page->url, count);
											//if(count < curr_page->rank)
												//{
													//curr_page->rank = count; 
													//}

											lqput(page_q, curr_page);
											
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

  for(word_t* word = lqget(page->words); word != NULL; word = lqget(page->words))
    {
			if(prev_word != NULL)
				{
					//printf("Word count: %i\n", word->count);
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
	
	queue_t* replacement = lqopen();

	word_t* prev_word = NULL;

	int i = 0;
	
	for(word_t* word = lqget(page->words); word != NULL; word = lqget(page->words))
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
			lqput(replacement, word);
			i++;
		}

	if(strcmp(prev_word->word, "and") == 0 || strcmp(prev_word->word, "or") ==0)
		result = false;
	
	lqclose(page->words);
	page->words = replacement;

	return result;
}

bool checkArgs(int argnum, char* args[]) {

	if (argnum < 3 || argnum > 6) {
		printf("usage: query <pageDirectory> <indexFile> [-q <queriesFile> <outputFile>]\n");
		return false;
	}

	if(access(args[1], R_OK) != 0) {
		printf("error: cannot access the page directory\n");
		return false;
	}

	if(access(args[2], R_OK) != 0) {
		printf("error: cannot access index file\n");
		return false;
	}

	if (argnum > 3) {

		if (strcmp(args[3], "-q") != 0) {
			printf("usage: query <pageDirectory> <indexFile> [-q <queriesFile> <outputFile>]\n");
			return false;
		}

		else {

			if (argnum != 6) {
				printf("usage: query <pageDirectory> <indexFile> [-q <queriesFile> <outputFile>]\n");
				return false;
			}

			if (access(args[4], R_OK) != 0) {
				printf("error in reading input file\n");
				return false;
			}
			return true;
		}
	}
	
	return true;
}

	
int main(int argc, char* argv[]) {

	bool correct = checkArgs(argc, argv);

	if (!correct) {
		exit(EXIT_FAILURE);
	}
	
	char* currline;

	char user_input[MAX_LINE];
	//strcpy(currline, "");
	char* currwords;
	hashtable_t* table = indexload(argv[2]);

	if (argc == 3) {
	
	while(true)
		{
			printf("> ");
			if((currline = fgets(user_input, MAX_LINE, stdin)) == NULL)
				{
					printf("\n");
					freeIndexTable(table);
					//qapply(page_q, freePageAts);
					if(page_q != NULL)
						lqclose(page_q);
					//freePageQueue(page_q);
					exit(EXIT_SUCCESS);

					
				}

			currline[strcspn(currline, "\n")] = 0;
			
			if((currwords = (char*)cleanInput(currline, table, argv[1])) == NULL || strcmp(currwords, "") == 0)
				{
					printf("Invalid Input!\n");
					//qapply(page_q, freePageAts);
					free(currwords);
					lqapply(page_q, freePageAts);
					if(page_q != NULL)
						{
							lqclose(page_q);
						}
					continue;
				}

			else if (!isValid(lqsearch(page_q, pageTruer, NULL)))
				{
					printf("Invalid Input!\n");
					free(currwords);
					lqapply(page_q, freePageAts);
					//					qclose(page_q);
					continue;
				}
			
			lqapply(page_q, rankPage);
			lqapply(page_q, printPage);
			printf("Keywords: %s\n", currwords);
			

			free(currwords);
			strcpy(currline, "");

			
			lqapply(page_q, freePageAts);
			
		}
	}

	else {

		FILE* queries = fopen(argv[4], "r");
		queries_output = fopen(argv[5], "w");

		char file_input[MAX_LINE];

		while ((currline = fgets(file_input, 100, queries)) != NULL) {

			currline[strcspn(currline, "\n")] = 0;

			if ((currwords = (char*) cleanInput(currline, table, argv[1])) == NULL || strcmp(currwords, "") == 0) {
				printf("Invalid input\n");

				free(currwords);
				lqapply(page_q, freePageAts);

				if(page_q != NULL) lqclose(page_q);

				continue;
			}

			else if (!isValid(qsearch(page_q, pageTruer, NULL))) {

				printf("Invalid input\n");
				free(currwords);
				lqapply(page_q, freePageAts);

				continue;
			}

			lqapply(page_q, rankPage);
			
			lqapply(page_q, printPageFile);
			fprintf(queries_output, "\n");

			free(currwords);
			strcpy(currline, "");

			lqapply(page_q, freePageAts);
		}

		fclose(queries);
		fclose(queries_output);

		freeIndexTable(table);

		if(page_q != NULL) lqclose(page_q);

		exit(EXIT_SUCCESS);
	}
}
