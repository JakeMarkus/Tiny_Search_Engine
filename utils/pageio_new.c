/* pageio_new.c --- 
 * 
 * 
 * Author: Eren Berke Saglam
 * Created: Tue Feb 14 12:43:35 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "pageio.h"
#include <unistd.h>

/*                                                                                                 
 * pagesave -- save the page in filename id in directory dirnm                                     
 *                                                                                                 
 * returns: 0 for success; nonzero otherwise                                                       
 *                                                                                                 
 * The suggested format for the file is:                                                           
 *   <url>                                                                                         
 *   <depth>                                                                                       
 *   <html-length>                                                                                 
 *   <html>                                                                                        
 */       
int32_t pagesave(webpage_t* pagep, int id, char* dirname) {
	//get all the info about the webpage
	char* html = webpage_getHTML(pagep);
	int len = webpage_getHTMLlen(pagep);

	char* url = webpage_getURL(pagep);
	int depth = webpage_getDepth(pagep);

	char id_str[250];
	char path[250] = "";
	//convert int id into a String
	sprintf(id_str, "%d", id);
	//concatonate dirname and string version of id together,
	//with id in the end
	strcat(path, dirname);
	strcat(path, id_str);
	//access(): Check to see if a file or directory can be accessed
	//W_OK: test for write permission
	if (access(dirname, W_OK) == -1) {
		printf("cannot access the directory to save pages");
		return 1;
	}

	FILE* f = fopen(path, "w");

	if (f == NULL) {
		printf("failed to open the file");
		return 2;
	}
	//print url, depth,len, html into file f. 
	fprintf(f, "%s\n", url);
	fprintf(f, "%d\n", depth);
	fprintf(f, "%d\n", len);
	fprintf(f, "%s", html);

	fclose(f);

	return 0;
}

/*                                                                                                 
 * pageload -- loads the numbered filename <id> in direcory <dirnm>                                
 * into a new webpage                                                                              
 *                                                                                                 
 * returns: non-NULL for success; NULL otherwise                                                   
 */    
webpage_t* pageload(int id, char* dirnm) {

	char id_str[1000] = "";

	char path[2000] = "";

	sprintf(id_str, "%d", id);

	strcat(path, dirnm);
	strcat(path, id_str);
	//R_OK: test for reas permission
	if (access(dirnm, R_OK) != 0) {
		printf("cannot access directory");
		return NULL;
	}
	

	FILE* f = fopen(path, "r");

	if (f == NULL) {
		printf("failed to read file");

		return NULL;
	}

	int depth, len;

	char url[128];
	
	//scan the url, depth, len from file f
	fscanf(f, "%s\n%d\n%d\n", url, &depth, &len);

	char* html = malloc(sizeof(char) * (len + 1));
	//put all the html information from file f into char* HTML
	for (int i = 0; i < len; i++) {
		char buff = fgetc(f);
		if (buff == EOF) break;

		html[i] = buff;
	}

	html[len] = '\0';

	webpage_t* newpage = webpage_new(url, depth, html);
	fclose(f);

	return newpage;
}

	
			

	
