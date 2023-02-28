/* lockedqueue.c --- 
 * 
 * 
 * Author: Jacob R. Markus
 * Created: Tue Feb 28 11:21:17 2023 (-0500)
 * Version: 
 * 
 * Description: A Queue and a Mutex: a Locked queue for safe multithreading.
 * 
 */


#include "queue.h"
#include "lqueue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct lqueue
{
	queue_t* q;
	pthread_mutex_t mut; 
} lqueue_mod_t;



lqueue_t* lqopen()
{
	lqueue_mod_t* ip;

	if ( ! ( ip = (lqueue_mod_t*) malloc(sizeof(lqueue_mod_t)))) {
		printf("[Error: malloc failed allocating lqueue]\n");
    return NULL;
  }

	ip->q = qopen();
	pthread_mutex_init(&ip->mut, NULL);

	return (lqueue_t*)ip;
	
}


int32_t lqput(lqueue_t* qp, void* elementp)
{
	lqueue_mod_t* ip = (lqueue_mod_t*) qp;

	pthread_mutex_lock(&ip->mut);
	int32_t output = qput(ip->q, elementp);
	pthread_mutex_unlock(&ip->mut);

	return output;
	
}

void* lqget(lqueue_t* qp)
{
	lqueue_mod_t* ip = (lqueue_mod_t*) qp;

	pthread_mutex_lock(&ip->mut);

	void* output = qget(ip->q);

	pthread_mutex_unlock(&ip->mut);

	return output;
}

void lqapply(lqueue_t* qp, void(*fn)(void* elementp))
{
	lqueue_mod_t* ip = (lqueue_mod_t*)qp;

	pthread_mutex_lock(&ip->mut);
	qapply(ip, fn);
	pthread_mutex_unlock(&ip->mut);
}

void* lqsearch(lqueue_t* qp, bool(*searchfn)(void* elementp, const void* keyp),
							 const void* skeyp)
{
	lqueue_mod_t* ip = (lqueue_mod_t*)qp;
	pthread_mutex_lock(&ip->mut);
	void* output = qsearch(ip, searchfn, skeyp);
	pthread_mutex_unlock(&ip->mut);
	return output;
}


void lqclose(lqueue_t* qp)
{
	lqueue_mod_t* ip= (lqueue_mod_t*)qp;
	pthread_mutex_lock(&ip->mut);
	qclose(ip->q);
	pthread_mutex_unlock(&ip->mut);
	pthread_mutex_destroy(&ip->mut);

	free(ip);
}

