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
	//containsNonChars(): check whether a word contains characters that's
	//other than the 26 alphabet.
	if(strlen(input) < 3 || containsNonChars(input))
		{
			return false; 
		}
	//tolower() takes an uppercase alphabet and convert it to a lowercase character. 
	for(int i = 0; i < strlen(input); i++)
		{
			input[i] = tolower(input[i]);
		}
	
	return true;
	
}

bool word_search(void* p, const void* s) {
	word_t* cp = (word_t*) p;

	char* sp = (char*) s;
	//check whether the world in struct cp matches,
	//the other input parameter string, sp. 
	return !(strcmp(cp->word, sp));
}

//declared a global variable here?
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
			//if curr_word does not exist as a key in the hash table, which means we encountered
			//a brand new word. Create a new hash table section for it. 
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
		//if the word contain letters other than the 26 letter, just ignore it. 
		else free(savedword);

	}

	free(savedword);

	//printf("Url: %s, HTML %s", webpage_getURL(first), webpage_getHTML(first));
	//Question: how will happly know which world to find in this case? 
	happly(freqtable, freq_counter);

	printf("%d\n", sum);

	
	
	webpage_delete(first);
	//printf("After\n");

	happly(freqtable, freeWord);

	hclose(freqtable);
	
	exit(EXIT_SUCCESS);
			
}

