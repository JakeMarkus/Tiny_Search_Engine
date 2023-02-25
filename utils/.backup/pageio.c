/* pageio.c --- 
 * 
 * 
 * Author: Eren Berke Saglam
 * Created: Fri Feb 10 18:50:49 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include "webpage.h"


int32_t pagesave(webpage_t *pagep, int id, char *dirname) {
	
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
                                                                                                                                              
  //char* html = webpage_getHTML(pagep);                                                                                                        
                                                                                                                                              
  char html_len[25000];                                                                                                                         
  sprintf(html_len, "%d", (int)webpage_getHTMLlen(pagep));                                                                                    
                                                                                                                                              
  char dephstr[250];                                                                                                                          
  sprintf(dephstr, "%d", (int)webpage_getDepth(pagep));  
	int err = fprintf(fp, "%s\n%s\n%s\n%s", webpage_getURL(pagep), dephstr,html_len,webpage_getHTML(pagep) );                                                   
                                                                                                                                             
  if (err < 0){                                                                                                                              
    printf("Failed to write to file!\n");                                                                                                    
    return 3;                                                                                                                                
  }                                                                                                                                          
                                                                                                                                             
	// fclose(fp);                                                                                                                                
	
  return 0;                                                                                                                                  
}

webpage_t* pageload(int id, char* dirnm) {


  FILE* curr_file;
	
	char id_str [100] = "";

	char path[100] = "";

	sprintf(id_str, "%d", id);

	//printf("inpageload/n");
	
	strcat(path, dirnm);
	strcat(path, id_str);

	if(access(dirnm, W_OK) == -1) {
		printf("cannot access %s to save pages\n", dirnm);
		return NULL;

	}

	//printf("\n%s\n", path);
	
	curr_file = fopen(path, "r");
	
	if(curr_file == NULL){
		return NULL;
		}

	
	char curr_line[1000] = "";

	char* url = (char*) malloc(300*sizeof(char));
	char depth[300] = "";
	char* HTML = (char*) malloc(99999*sizeof(char));
	


	//printf("yee0\n");
	
	int num_line = 0;

	//printf("yee");

	while(fgets(curr_line, 1000, curr_file)){

		//	printf("hello\n");
		if(num_line == 0){
			strtok(curr_line, "\n");
			strcat(url, curr_line);
			//printf("%s", url);
		}
		
		else if(num_line == 1){
			strcat(depth, curr_line);
		}

		else if(num_line == 2) {
			num_line++;
			continue;
		}
		
		else{
			strcat(HTML, curr_line);	
		}
		
		num_line++;

		//		printf("hry");
	}
	
	int int_depth = atoi(depth);
	
	webpage_t* new_page = webpage_new(url, int_depth, HTML);

	
	
	return new_page;
}

		
		














		
