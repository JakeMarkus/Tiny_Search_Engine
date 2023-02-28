/* lqueue.h --- 
 * 
 * 
 * Author: Jacob R. Markus
 * Created: Tue Feb 28 11:59:08 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#pragma once
/*
 * queue.h -- public interface to the queue module
 */
#include <stdint.h>
#include <stdbool.h>

/* the queue representation is hidden from users of the module */
typedef void lqueue_t;

/* create an empty queue */
queue_t* lqopen(void);

/* deallocate a queue, frees everything in it */
void lqclose(queue_t *qp);

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise
 */
int32_t lqput(queue_t *qp, void *elementp);

/* get the first first element from queue, removing it from the queue */
void* lqget(queue_t *qp);

/* apply a function to every element of the queue */
void lqapply(queue_t *qp, void (*fn)(void* elementp));

/* search a queue using a supplied boolean function
 * skeyp -- a key to search for
 * searchfn -- a function applied to every element of the queue
 *          -- elementp - a pointer to an element
 *          -- keyp - the key being searched for (i.e. will be
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h
 * returns a pointer to an element, or NULL if not found
 */

void* lqsearch(queue_t *qp,
              bool (*searchfn)(void* elementp,const void* keyp),
              const void* skeyp);

/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
