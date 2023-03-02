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
#include "pageio.h"
#include "webpage.h"
#include <dirent.h>
#include <sys/types.h>

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

bool checkAndOr(char* prev, char* curr_buff) {

	char curr[100];

	strcpy(curr, curr_buff);

	if (strcmp(curr, "or") == 0 || strcmp(curr, "and") == 0) {

		if (strcmp(prev, "or") == 0 || strcmp(prev, "and") == 0 || strcmp(prev, "") == 0) {
			return false;
		}
	}

	return true;
}

bool page_search(void* page, const void* id) {
	return ((page_t*) page)->id == *((int*)id);
}

void rankOr(queue_t* curr_queue, queue_t* result_queue) {
	page_t* buffer;

	while ((buffer = (page_t*) qget(curr_queue)) != NULL) {

		page_t* new_page;

		if ((new_page = qsearch(result_queue, page_search, &(buffer->id))) == NULL) {
			qput(result_queue, buffer);
		}

		else {
			new_page->rank += buffer->rank;
			free(buffer);
		}
	}
}

queue_t* page_q;
queue_t* result_q;

static char* cleanInput(char* input, hashtable_t* table, char* pagedir)
{

	page_q = qopen();
	result_q = qopen();

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

	char* prev_block = (char*) malloc(100*sizeof(char));
	strcpy(prev_block, "");

	char* output = (char*)malloc(10000 * sizeof(char));

	strcpy(output, "");



	for(int i = 0; i <= strlen(input); i++)
		{
			if((char)input[i] == ' ' || i == strlen(input))
				{
					if(strcmp(block, "") != 0)
						{

							if (!checkAndOr(prev_block, block)) {
								free(block);
								free(prev_block);
								free(output);
								return NULL;
							}

							strcat(output, block);

								if(i!= strlen(input))
									{
										strcat(output, " ");
									}

								if (strcmp(block, "and") != 0) {


									if(strcmp(block, "or") != 0) {

										if(strlen(block) >= 3) {

										for(int i = 1; i <= getNumPages(); i++)
											{

												page_t* curr_page = (page_t*)qget(page_q);

												count = getCount(table, block, i);


												if(strcmp(prev_block, "or")==0)
													{
														word_t* orWord = makeWord("or", count);
														qput(curr_page->words, orWord);
													}
												word_t* w = makeWord(block, count);

												qput(curr_page->words, w);


												//											printf("%s count: %i\n", curr_page->url, count);
												if(count < curr_page->rank)
													{
														curr_page->rank = count;
													}

												qput(page_q, curr_page);

											}
										}
									
									}
										
								}
							

								//else if(strcmp(block, "or") == 0)
								//{

								//}

							free(prev_block);
							prev_block = (char*) malloc(100*sizeof(char));

							strcpy(prev_block, block);

							free(block);

							block = (char*)malloc(100*sizeof(char));

							if (strcmp(prev_block, "\n") != 0)
								strcpy(block, "");
							//printf("yee1\n")
						
						}
				}
				

				else
			 		{
		 				if(!isChar((char)input[i]) && input[i] != 10)
	 						{
								free(prev_block);
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

	printf("%s\n", prev_block);

	if (strcmp(prev_block, "and") == 0 || strcmp(prev_block, "or") == 0) {
		free(block);
		free(prev_block);
		free(output);

		return NULL;
	}


	free(block);
	free(prev_block);

	return output;
}
 //where I edited 


void rankPage(page_t* page )
{

	int rank = 0;

	word_t* prev_word = 
	
	for(word_t* word = qget(page->words); word != NULL; word = qget(page->words))
		{
			
		}
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
					//		qapply(page_q, freePageAts);
				  qclose(page_q);
					//freePageQueue(page_q);
					exit(EXIT_SUCCESS);


				}

			currline[strcspn(currline, "\n")] = 0;

			if((currwords = (char*)cleanInput(currline, table, "../pages/")) == NULL)
				{
					printf("Invalid Input!\n");
					continue;
				}

			qapply(page_q, printPage);
			printf("Cleaned Input: %s\n", currwords);

			free(currwords);
			strcpy(currline, "");

			qapply(page_q, freePageAts);
		}
	//	return 0;
}
