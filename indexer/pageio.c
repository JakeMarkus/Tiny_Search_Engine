/* pageio.c --- 
 * 
 * 
 * Author: Eren Berke Saglam, Jia(James) Chen, Jacob R. Markus
 * Created: Fri Feb 10 18:32:18 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */


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
                                                                                 
  int err = fprintf(fp, "%s\n%s\n%s\n%s", webpage_getURL(pagep), id_str,html_len\
, html );                                                                        
                                                                                 
  if (err < 0){                                                                  
    printf("Failed to write to file!\n");                                        
    return 3;                                                                    
  }                                                                              
                                                                                 
  fclose(fp);                                                                    
                                                                                 
  return 0;                                                                      
}


int main(void){
	return 0;
}
