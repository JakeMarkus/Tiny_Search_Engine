/* testlqueue.c --- 
 * 
 * 
 * Author: Jacob R. Markus
 * Created: Wed Mar  1 14:53:25 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lqueue.h"


void print(void* i)
{
	char* c = (char*)i;
	printf("%s\n", c);
}

int main()
{

	lqueue_t* q = lqopen();

	printf("1\n");
	char* strone = malloc(sizeof(char)*10);
	strcpy(strone, "yee1");

	char* strtwo = malloc(sizeof(char)*10);
	strcpy(strtwo, "yee2");
	
		
	lqput(q,strone);
	lqput(q,strtwo);
	printf("2\n");

	lqapply(q, print);
	printf("2.5\n");
	char* o = lqget(q);

	printf("Got: %s\n", o);

	free(o);
	
	printf("3\n");
	lqclose(q);
	printf("4\n");
	return 0;
}
