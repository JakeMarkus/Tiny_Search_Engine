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
#include <pthread.h>

void* func1(void* argp)
{
	lqueue_t* qp = (lqueue_t*)argp;

	for(int i = 0; i < 100; i++)
		{
			char* yee = malloc(sizeof(char)*100);
			strcpy(yee, "");
			sprintf(yee, "%d", i);
			lqput(qp, yee);
		}
}

void* func2(void* argp)
{
	lqueue_t* qp = (lqueue_t*)argp;

	for(int i = 100; i < 200; i++)
		{
			char* yee = malloc(sizeof(char)*100);
			strcpy(yee, "");
			sprintf(yee, "%d", i);
			lqput(qp, yee);
		}
}

void print(void* i)
{
	char* c = (char*)i;
	printf("%s\n", c);
}

int main()
{
	pthread_t tid1,tid2;

	lqueue_t* q = lqopen();
	
	if(pthread_create(&tid1, NULL, func1, q) != 0)
		exit(EXIT_FAILURE);
	
	if(pthread_create(&tid2, NULL, func2, q) != 0)
		exit(EXIT_FAILURE);

	if(pthread_join(tid1, NULL) != 0)
		exit(EXIT_FAILURE);
	
	if(pthread_join(tid2, NULL) != 0)
		exit(EXIT_FAILURE);

	lqapply(q, print);

	lqclose(q);
	exit(EXIT_SUCCESS);
	
	
}
