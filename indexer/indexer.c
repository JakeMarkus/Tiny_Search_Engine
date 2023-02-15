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

typedef struct {
	char* word;
	int count;

} word_t;

void freeWord(void* word) {

	free(((word_t*) word)->word);
}

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

bool word_search(void* p, const void* s) {
	word_t* cp = (word_t*) p;

	char* sp = (char*) s;

	return !(strcmp(cp->word, sp));
}

int sum = 0;

void freq_counter(void* word) {
	word_t* word_c = (word_t*) word;
	sum += word_c->count;
}

int main(void) {

	webpage_t* first = pageload(1, "../pages/");

	char* savedword = NULL;
	int pos = 0;

	hashtable_t* freqtable = hopen(2000);
	
	while((pos = webpage_getNextWord(first, pos, &savedword)) > 0) {

		if(NormalizeWord(savedword)) {

			word_t* curr_word;

			if ((curr_word = hsearch(freqtable, word_search, savedword, strlen(savedword))) == NULL) {
				curr_word = (word_t*) malloc(sizeof(word_t));

				curr_word->word = savedword;

				curr_word->count = 1;
				
				hput(freqtable, curr_word, curr_word->word, strlen(curr_word->word));
			}

			else {
				curr_word->count++;
				free(savedword);
			}

		}

		else free(savedword);

	}

	free(savedword);

	//printf("Url: %s, HTML %s", webpage_getURL(first), webpage_getHTML(first));

	happly(freqtable, freq_counter);

	printf("%d\n", sum);

	
	
	webpage_delete(first);
	//printf("After\n");

	happly(freqtable, freeWord);

	hclose(freqtable);
	
	exit(EXIT_SUCCESS);
			
}

