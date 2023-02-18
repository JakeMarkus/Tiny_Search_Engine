/* crawler.c --- 
0;136;0c0;136;0c * 
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
#include <unistd.h>

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


int32_t pagesave(webpage_t *pagep, int id, char *dirname)
{
	FILE * fp;
	
	char id_str [250]; //id/10 + 1

	char path[250] = "";
	
	sprintf(id_str, "%d", id);

	strcat(path, dirname);
	strcat(path, id_str);

	if(access(dirname, W_OK)== -1){
		printf("Cannot access %s to save pages!\n", dirname);
		return 1;
		}
	fp = fopen(path, "w+");

	if (fp == NULL){
			return 2;
		}
	
	char* html = webpage_getHTML(pagep);

	char html_len[250];
	sprintf(html_len, "%d", (int)webpage_getHTMLlen(pagep));
	
	int err = fprintf(fp, "%s\n%s\n%s\n%s", webpage_getURL(pagep), id_str,html_len, html );

	if (err < 0){
		printf("Failed to write to file!\n");
		return 3;
	}
	
	fclose(fp);
	
	return 0;
}

queue_t* url_scanner(webpage_t* my_page, hashtable_t* url_table) {

	int pos = 0;

	char* next_url;
	
	queue_t* internalLinks = qopen();
	
	while ((pos = webpage_getNextURL(my_page, pos, &next_url)) > 0) {

		if (hsearch(url_table, url_matcher, next_url, strlen(next_url)) == NULL) {

			char* copy = malloc(sizeof(next_url));

			strcpy(copy, next_url);

			hput(url_table, next_url, next_url, strlen(next_url));

			if (IsInternalURL(next_url)) {
			
				qput(internalLinks, copy);
			
			}
			else
				{
					free(copy);
				}
			
		}
		else
			{
				free(next_url);
			}
	}
	
	return internalLinks;
}



int main(int argc, char *argv[]) {

	char* USAGE = "usage: crawler <seedurl> <pagedir> <maxdeph>\n";

		if(argc != 4)
			{
				printf("%s", USAGE);
				exit(EXIT_FAILURE);
			}
	
	char* my_url = argv[1];
	char* pagedir = argv[2];
	int deph = atoi(argv[3]);

	printf("Url: %s\nPagedir: %s\nDeph: %i\n", my_url, pagedir, deph);

	webpage_t* my_page = webpage_new(my_url, 0, NULL);

	if(webpage_fetch(my_page)) {

		webpage_getHTML(my_page);

	}

	else {
		printf("error in fetching html\n");
		exit(EXIT_FAILURE);
	}

	queue_t* mainqueue = qopen();
	hashtable_t* url_table = hopen(1000);
	
	qput(mainqueue, my_page);

	pagesave(my_page, 1, pagedir);
	

	int id = 1;

	webpage_t* x;
	
	while( (x = (webpage_t*)qget(mainqueue) ) != NULL && webpage_getDepth(x) < deph)
		{

			webpage_fetch(x);
			
			queue_t* nextLayer = url_scanner(x, url_table);
			
			webpage_t* next =  webpage_new((char*)qget(nextLayer), webpage_getDepth(x)+1, NULL);
			
			while ( next != NULL)
				{

					webpage_fetch(next);

					
					printf("Saving  page: %s || ID: %i\n", (char*)webpage_getURL(next), id);
				 
					qput(mainqueue, next);

					char* url_copy =  malloc(sizeof(webpage_getURL(next))));

					strcpy(url_copy, webpage_getURL(next));
					
					hput(url_table, url_copy, url_copy, strlen(url_copy));
			
					pagesave(next, id, pagedir);
					
					id++;
					
					next =  webpage_new((char*)qget(nextLayer), webpage_getDepth(x)+1, NULL);
				}

			qclose(nextLayer);
			printf("CLOSING\n");
			webpage_delete(x);
		}

	//webpage_delete(my_page);

	//free(my_url);
	//free(pagedir);
	
	for(webpage_t* leftover = (webpage_t*)qget(mainqueue); leftover != NULL; leftover = (webpage_t*)qget(mainqueue))
		{
			printf("CLOSING LEFTOVER\n");
			webpage_delete(leftover);
		}

	printf("yee\n");
	qclose(mainqueue);
	hclose(url_table);
	exit(EXIT_SUCCESS);

}
