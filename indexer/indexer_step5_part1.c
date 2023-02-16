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

#include "hash.h"
#include "queue.h"

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

int sum = 0;

void count_freq_q(void* qp) {
	doc_t* doc = (doc_t*) qp;
	sum += doc->count;
}

void freq_counter(void* word) {
	word_t* word_c = (word_t*) word;
	qapply(word_c->queue_doc, count_freq_q);
}

int main(int argc, char* argv[]) {

	if(argc != 2)
		{
			exit(EXIT_FAILURE);
		}

	
 
	
	int curr_id = atoi(argv[1]);

	webpage_t* first = pageload(curr_id, "../pages/");

	char* savedword = NULL;
	int pos = 0;

	hashtable_t* freqtable = hopen(2000);
	
	while((pos = webpage_getNextWord(first, pos, &savedword)) > 0) {

		if(NormalizeWord(savedword)) {

			word_t* curr_word;

			if ((curr_word = hsearch(freqtable, word_search, savedword, strlen(savedword))) == NULL) {

				curr_word = (word_t*) malloc(sizeof(word_t));

				curr_word->word = savedword;

				curr_word->queue_doc = qopen();

				doc_t* curr_doc = (doc_t*) malloc(sizeof(doc_t));

				curr_doc->doc_id = curr_id;

				curr_doc->count = 1;

				qput(curr_word->queue_doc, curr_doc);
				
				hput(freqtable, curr_word, curr_word->word, strlen(curr_word->word));
			}

			else {

				doc_t* curr_doc;

				if((curr_doc = qsearch(curr_word->queue_doc, doc_search, &curr_id)) == NULL) {
					curr_doc = (doc_t*) malloc(sizeof(doc_t));
					curr_doc->doc_id = curr_id;
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

	happly(freqtable, freq_counter);

	printf("%d\n", sum);

	
	
	webpage_delete(first);
	//printf("After\n");

	happly(freqtable, freeWord);
 	happly(freqtable, freeDoc);

	hclose(freqtable);
	
	exit(EXIT_SUCCESS);
			
}

