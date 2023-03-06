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


#include <unistd.h>
#include <stdint.h>
#include <dirent.h>

#include "indexio.h"


int main(int argc, char* argv[])
{
	if(argc != 4)
		{		
			printf("usage: indexer <pagedir> <indexnm> <threadnm>\n");
			return 1; 
		}

	DIR *pagedir = opendir(argv[1]);
	
	if(pagedir == NULL)
		{
			printf("Could not open directory: %s\n", argv[1]);
			return 2;

		}
	int max = 0;
	char pathname[500];
	strcpy(pathname, "");
	char id_str[100];
	strcpy(id_str, "");

	for(int i = 1; i < 9999; i++)
		{
			strcat(pathname, argv[1]);
			sprintf(id_str, "%i", i);
			strcat(pathname, id_str);
			
			if (access(pathname, R_OK)==-1)
				break;
				
			else
				{
					max = i;
				}
			strcpy(pathname, "");
		}
	
	closedir(pagedir);

	int threadnm = atoi(argv[3]);

	if(threadnm <1)
		{
			printf("Error: invalid threadcount!\n");
			return 4;
		}
	printf("Thread nm: %i\n", threadnm);
	
	
	
	
	if (threadedindexsave(argv[1], "./", argv[2], max, threadnm) != 0)
		{
			printf("Error in creating index!\n");
			return 4; 
		}
	
	return 0; 
}
