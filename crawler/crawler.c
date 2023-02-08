/* crawler.c --- 
 * 
 * 
 * Author: Jacob R. Markus
 * Created: Thu Feb  2 12:01:02 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "webpage.h"
#include "queue.h"
#include "hash.h"
#include <stdbool.h>
#include <string.h>

void printUrl(void* Url){
	char* UrlC= (char*) Url;
	printf("%s\n", UrlC);
}

bool url_matcher(void* url1, const void* url2) {
	char* urlstr1 = (char*) url1;
	char* urlstr2 = (char*) url2;

	if (strcmp(urlstr1, urlstr2) == 0) {
		return true;
	}

	else {
		return false;
	}
}

queue_t* url_scanner(webpage_t* my_page) {

	int pos = 0;

	char* next_url;
	
	queue_t* internalLinks = qopen();

	hashtable_t* url_table = hopen(100);
	
	while ((pos = webpage_getNextURL(my_page, pos, &next_url)) > 0) {

		if (hsearch(url_table, url_matcher, next_url, sizeof(next_url)) == NULL) {

			char* copy = malloc(strlen(next_url) +1);

			strcpy(copy, next_url);

			hput(url_table, next_url, next_url, sizeof(next_url));

			if (IsInternalURL(next_url)) {
			
				qput(internalLinks, copy);
			//			printf("%s, internal\n", next_url);
			}
			else
				{
					free(copy);
				}

		//		else {
			//			printf("%s, external\n", next_url);
		//		}
			
		}
		else
			{
				free(next_url);
			}
	}

	hclose(url_table);
		
	return internalLinks;
}


int main(void) {

	char* my_url = "https://thayer.github.io/engs50/";

	webpage_t* my_page = webpage_new(my_url, 0, NULL);

	if(webpage_fetch(my_page)) {

		webpage_getHTML(my_page);

	}

	else {
		printf("error in fetching html\n");
		exit(EXIT_FAILURE);
	}

	queue_t* links = url_scanner(my_page);
	qapply(links, printUrl);
	webpage_delete(my_page);

	qclose(links);
	
	exit(EXIT_SUCCESS);

}
