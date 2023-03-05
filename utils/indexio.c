/* indexio.c ---
0;136;0c65;6800;1c *
 *
 * Author: Eren Berke Saglam
 * Created: Thu Feb 16 10:25:14 2023 (-0500)
 * Version:
 *
 * Description:
 *
 */

#define _GNU_SOURCE

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>

#include "lqueue.h"
#include "lhash.h"
#include "pageio.h"
#include "webpage.h"
#include "hash.h"
#include "queue.h"
#include "indexio.h"

typedef struct {
  char* word;
  queue_t* queue_doc;

} word_t;

typedef struct {
	int doc_id;
  int count;

} doc_t;


char* getUrl(char* pagedir, int id) {

	
	char* path = (char*) malloc(sizeof(char) * 1000);

	strcpy(path, "");
	
	strcat(path, pagedir);
	
	char* id_str = (char*) malloc(1000 * sizeof(char));

	strcpy(id_str, "");
	sprintf(id_str, "%i", id);
	
	strcat(path, id_str);
	
	//printf("%s\n", path);
	FILE* f = fopen(path, "r");
	if(f == NULL){
		printf("Failed to open file.\n");
		return NULL;
	}
	char* line = (char*)(malloc(sizeof(char)*1000));
	strcpy(line, "");

	size_t len = 1000;
	
	getline(&line, &len, f);

	free(path);
	free(id_str);
	fclose(f);
	line[strcspn(line, "\n")] = 0;
	return line; 
}



void freeWord(void* word) {

  free(((word_t*) word)->word);
}

void freeDoc(void* word) {

  word_t* word_ty = (word_t*) word;
  qclose(word_ty->queue_doc);
}


bool word_search(void* f, const void* s) {
  word_t* fp = (word_t*) f;

  char* sp = (char*) s;

  return !(strcmp(fp->word, sp));
}

bool doc_search(void* f, const void* s) {
  doc_t* doc = (doc_t*) f;

  int* id = (int*) s;

  return (doc->doc_id == *id);
}

bool containsNonChars(char * word) {
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

int page_num;

int getNumPages()
{
	return page_num;
}

int getCount(hashtable_t* table, char*word, int id)
{
	 word_t* wordObj = hsearch(table, word_search, word, strlen(word));

	 if(wordObj == NULL)
		 return 0;
	 
	 doc_t* doc  = qsearch(wordObj->queue_doc, doc_search, &id);

	 if(doc == NULL)
		 return 0;
	 
	 return doc->count;
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

int32_t indexsave(char* pages_dir  , char* index_dir, char* indexnm, int n) {
	
	FILE* index_file;

	//char id_str[250] = "1";
	char path[250] = "";

	strcat(path, index_dir);
	strcat(path, indexnm);
	
	//strcat(path, "/index");
	//strcat(path, "_");

	char n_str[250] = "";
	sprintf(n_str, "%d", n);

	//strcat(path, n_str);


	if (access(pages_dir, R_OK) == -1) {
		printf("ERROR: cannot access the directory to read pages\n");
		return 1;
	}

	if (access(index_dir, W_OK) == -1) {
		printf("ERROR: cannot access the directory to save index\n");
		return 1;
	}

	index_file = fopen(path, "w");

	if (index_file == NULL) {
		printf("failed to open the file\n");
		return 2;
	}

	//COPYPASTA BELOW:

 hashtable_t* freqtable = hopen(5000);
 queue_t* words = qopen();

	for(int i =1; i <= n; i ++ )
    {

      webpage_t* first = pageload(i, pages_dir);

      char* savedword = NULL;
      int pos = 0;

      while((pos = webpage_getNextWord(first, pos, &savedword)) > 0) {

        if(NormalizeWord(savedword)) {

          word_t* curr_word;

          if ((curr_word = hsearch(freqtable, word_search, savedword, strlen(savedword))) == NULL) {

						qput(words, savedword);

            curr_word = (word_t*) malloc(sizeof(word_t));

            curr_word->word = savedword;

            curr_word->queue_doc = qopen();

            doc_t* curr_doc = (doc_t*) malloc(sizeof(doc_t));

            curr_doc->doc_id = i;

            curr_doc->count = 1;


            qput(curr_word->queue_doc, curr_doc);

            hput(freqtable, curr_word, curr_word->word, strlen(curr_word->word));
          }

          else {

            doc_t* curr_doc;

            if((curr_doc = qsearch(curr_word->queue_doc, doc_search, &i)) == NULL) {
              curr_doc = (doc_t*) malloc(sizeof(doc_t));
              curr_doc->doc_id = i;
              curr_doc->count = 1;

              qput(curr_word->queue_doc, curr_doc);
            }

            else {
              curr_doc->count++;
            }

            free(savedword);

          }

        }

        else free(savedword);


				}

      //printf("Url: %s, HTML %s", webpage_getURL(first), webpage_getHTML(first));

      //happly(freqtable, freq_counter);

      //total += sum;
      //      printf("Intermediate sum: %d\n", sum);
      webpage_delete(first);

    }


	char* counted_word = "";
	doc_t* doc;
	
	while((counted_word =  (char*)qget(words)) != NULL)
			{
				char* line = (char*)malloc(500*sizeof(char));
				strcpy(line, "");
				strcat(line, counted_word);

				word_t* word_struct = hsearch(freqtable, word_search, counted_word, strlen(counted_word));
				
				while((doc = (doc_t*)qget(word_struct->queue_doc)) != NULL)
					{
						char id_str[250];
						sprintf(id_str, "%d", doc->doc_id);

						strcat(line, " ");
						strcat(line, id_str);

						char count_str[250];
						sprintf(count_str, "%d", doc->count);

						strcat(line, " ");
						strcat(line, count_str);

						free(doc);
					}

				fprintf(index_file, "%s\n", line);
				free(line);
				//free(counted_word);
			}

	
			fclose(index_file);

      happly(freqtable, freeWord);
			happly(freqtable, freeDoc);
			qclose(words);
			hclose(freqtable);


			return 0;
}
/////////////MULTITHREADED BONK/////////////////////////////

typedef struct tableinput{

	lhashtable_t* freqtable;
	lqueue_t* words;
	char* pages_dir;
	int i;
	pthread_mutex_t mut;
} sharedIndexInfo_t;


sharedIndexInfo_t* makeSharedIndexInfo(lhashtable_t* f, lqueue_t* w, char* p, int i, pthread_mutex_t m)
{
	sharedIndexInfo_t* output = (sharedIndexInfo_t*)malloc(sizeof(sharedIndexInfo_t));

	output->freqtable = f;
	output->words = w;
	output->pages_dir = p;
	output->i = i;
	output->mut = m;

	return output;
}

void freeSharedIndexInfo(sharedIndexInfo_t* in)
{
	lhclose(in->freqtable);
	lqclose(in->words);
	free(in->pages_dir);
	pthread_mutex_destroy(in->mut);
}


void addToTable(void* in)
{

	sharedIndexInfo_t* input = (sharedIndexInfo_t*)in;
	
	
	webpage_t* first = pageload(input->i,input-> pages_dir);
	
	char* savedword = NULL;
	int pos = 0;

      while((pos = webpage_getNextWord(first, pos, &savedword)) > 0)
				{

					if(NormalizeWord(savedword))
						{

							word_t* curr_word;
							
							if ((curr_word = lhsearch(input->freqtable, word_search, savedword, strlen(savedword))) == NULL)
								{
									pthread_mutex_lock(&input->mut);
									
									lqput(input->words, savedword);
									
									curr_word = (word_t*) malloc(sizeof(word_t));
									
									curr_word->word = savedword;
									
									curr_word->queue_doc = qopen();
									
									doc_t* curr_doc = (doc_t*) malloc(sizeof(doc_t));
									
									curr_doc->doc_id = i;
									
									curr_doc->count = 1;
									
									
									qput(curr_word->queue_doc, curr_doc);
									
									lhput(freqtable, curr_word, curr_word->word, strlen(curr_word->word));
									
									pthread_mutex_unlock(&input->mut);
								}

							else
								{
									pthread_mutex_lock(&input->mut);
									doc_t* curr_doc;

									if((curr_doc = qsearch(curr_word->queue_doc, doc_search, &i)) == NULL)
										{
											curr_doc = (doc_t*) malloc(sizeof(doc_t));
											curr_doc->doc_id = i;
											curr_doc->count = 1;
											
											qput(curr_word->queue_doc, curr_doc);
										}
									
									else
										{
											curr_doc->count++;
										}
									
									free(savedword);
									pthread_mutex_unlock(&input->mut);
									
								}
							
						}
					
					else free(savedword);
					
					
				}
			
      //printf("Url: %s, HTML %s", webpage_getURL(first), webpage_getHTML(first));

      //happly(freqtable, freq_counter);

      //total += sum;
      //      printf("Intermediate sum: %d\n", sum);
      webpage_delete(first);

    

}


int32_t threadedindexsave(char* pages_dir  , char* index_dir, char* indexnm, int n, int n_threads) {
	
	FILE* index_file;

	//char id_str[250] = "1";
	char path[250] = "";

	strcat(path, index_dir);
	strcat(path, indexnm);
	
	//strcat(path, "/index");
	//strcat(path, "_");

	char n_str[250] = "";
	sprintf(n_str, "%d", n);

	//strcat(path, n_str);


	if (access(pages_dir, R_OK) == -1) {
		printf("ERROR: cannot access the directory to read pages\n");
		return 1;
	}

	if (access(index_dir, W_OK) == -1) {
		printf("ERROR: cannot access the directory to save index\n");
		return 1;
	}

	index_file = fopen(path, "w");

	if (index_file == NULL) {
		printf("failed to open the file\n");
		return 2;
	}

	//COPYPASTA BELOW:

 lhashtable_t* freqtable = lhopen(5000);
 lqueue_t* words = lqopen();

 queue_t* threads = 
	for(int i =1; i <= n; i ++ )
    {
			
		}

	char* counted_word = "";
	doc_t* doc;
	
	while((counted_word =  (char*)qget(words)) != NULL)
			{
				char* line = (char*)malloc(500*sizeof(char));
				strcpy(line, "");
				strcat(line, counted_word);

				word_t* word_struct = hsearch(freqtable, word_search, counted_word, strlen(counted_word));
				
				while((doc = (doc_t*)qget(word_struct->queue_doc)) != NULL)
					{
						char id_str[250];
						sprintf(id_str, "%d", doc->doc_id);

						strcat(line, " ");
						strcat(line, id_str);

						char count_str[250];
						sprintf(count_str, "%d", doc->count);

						strcat(line, " ");
						strcat(line, count_str);

						free(doc);
					}

				fprintf(index_file, "%s\n", line);
				free(line);
				//free(counted_word);
			}

	
			fclose(index_file);

      happly(freqtable, freeWord);
			happly(freqtable, freeDoc);
			qclose(words);
			hclose(freqtable);


			return 0;
}


void freeIndexTable(hashtable_t* table)
{
	happly(table, freeWord);
	happly(table, freeDoc);
	hclose(table);
}

hashtable_t* indexload(char* file_path) {
	

	FILE* index_file = fopen(file_path, "r");

	if (index_file == NULL) {
		printf("failed to open the file\n");
		return NULL;
	}

	hashtable_t* indextable = hopen(5000);

	char* line = "";
	size_t len = 0;
	bool word_found = false; 

	bool isId = true;

	doc_t* currdoc;
	char* block;
	while(getline(&line, &len, index_file) != -1)
		{
			line[strcspn(line, "\n")] = 0;
			
			//printf("%s\n", line);
			word_t* curr_word = (word_t*)malloc(sizeof(word_t));

			block = (char*)malloc(100*sizeof(char));
			strcpy(block, "");
			
			for(int i = 0; i <= strlen(line); i++)
				{
					if((char)line[i] == ' ' || i == strlen(line))
						{
							if(!word_found)
								{
									char* cpy = (char*)malloc(100*sizeof(char));
									strcpy(cpy, block);
									//printf("word: %s\n", cpy);
									curr_word->word = cpy;
									curr_word->queue_doc = qopen();
									word_found = true;
								}
							else if(isId)
								{
									currdoc = (doc_t*)malloc(sizeof(doc_t));
									currdoc->doc_id = atoi(block);

									if(atoi(block) > page_num)
										{
											page_num = atoi(block);
										}
									//printf("id: %i\n",currdoc->doc_id);
									isId = false;
								}
							else
								{
									currdoc->count = atoi(block);
									qput(curr_word->queue_doc, currdoc);
									//printf("count: %i\n", currdoc->count);
									isId = true;
								}
							
							free(block);
							
							block = (char*)malloc(100*sizeof(char));
							strcpy(block, "");
			
						}

					else
						{
							strncat(block, &line[i], 1);
						}
				}

			hput(indextable, curr_word, curr_word->word, strlen(curr_word->word));
			word_found = false;
			free(block);

		}

	fclose(index_file);

	if(line){
			free(line);
		}


	return indextable;
}
