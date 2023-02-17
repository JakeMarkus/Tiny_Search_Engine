/* indexio.c ---
65;6800;1c *
 *
 * Author: Eren Berke Saglam
 * Created: Thu Feb 16 10:25:14 2023 (-0500)
 * Version:
 *
 * Description:
 *
 */

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
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

static bool containsNonChars(char * word) {
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

int32_t indexsave(char* pages_dir  , char* index_dir, int n) {
	
	FILE* index_file;

	//char id_str[250] = "1";
	char path[250] = "";

	strcat(path, index_dir);
	strcat(path, "/index");
	strcat(path, "_");

	char n_str[250] = "";
	sprintf(n_str, "%d", n);

	strcat(path, n_str);


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

hashtable_t* indexload(char* file_path) {
	hashtable_t* table;
	

	FILE* index_file = fopen(file_path, "r");

	if (index_file == NULL) {
		printf("failed to open the file\n");
		return NULL;
	}

	table = hopen(5000);

	char buff;
	char* word_str;
	char* idAndFreq;

	while ((buff = (char) fgetc(index_file)) != EOF) {
		word_str = "";
		strcat(word_str,(char*) buff);
		if(buff == " ") {
			word_t* curr_word = (word_t*) malloc(sizeof(word_t));

			curr_word->word = word_str;

			curr_word->queue_doc = qopen();

			int doc_id;
			int count;
			//      doc_t* wordInfo = (doc_t*)malloc(size0f(doc_t));
			while((buff = fgetc(index_file))!='\0') {
				strcat(idAndFreq, buff);
			}
				//for(int space_count = 0; space_count < 2; i++) {
			char* broke = strtok(idAndFreq, " ");

			printf("yee\n");
			//construct a queue for a word
			while(broke != NULL){
				doc_t* wordInfo = (doc_t*)malloc(sizeof(doc_t));  
				doc_id = atoi(broke);
				broke = strtok(NULL, " ");
				count = atoi(broke);
				broke = strtok(NULL, " ");
				wordInfo->doc_id = doc_id;
				wordInfo->count = count;
				qput(curr_word->queue_doc, (void*)wordInfo);
			}

			hput(table, (void*)curr_word, curr_word->word, strlen(curr_word->word));
		}
	}

	fclose(index_file);
	
	return table;
}
