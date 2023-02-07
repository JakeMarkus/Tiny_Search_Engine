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

void url_scanner(webpage_t* my_page) {

	int pos = 0;

	char* next_url;

	while ((pos = webpage_getNextURL(my_page, pos, &next_url)) > 0) {

		if (IsInternalURL(next_url)) {
			printf("%s, internal\n", next_url);
		}

		else {
			printf("%s, external\n", next_url);
		}

		free(next_url);
	}
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

	url_scanner(my_page);

	webpage_delete(my_page);

	exit(EXIT_SUCCESS);

}

