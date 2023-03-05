/* lhash.c ---
 *
 *
 * Author: Jia Chen-2
 * Created: Thu Mar  2 11:21:38 2023 (-0500)
 * Version:
 *
 * Description:
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "lhash.h"
#include "hash.h"
#include <stdlib.h>
#include <pthread.h>

typedef struct lhashtable_mod{
	
	hashtable_t* table;
	pthread_mutex_t lock;

} lhashtable_mod_t;

lhashtable_t* lhopen(uint32_t lhsize) {

	lhashtable_mod_t* ip;

	if (!(ip =  (lhashtable_mod_t*) malloc(sizeof(lhashtable_mod_t)))) {
		printf("[Error: malloc failed allocating lhashtable]\n");
		return NULL;
	}

	pthread_mutex_init(&ip->lock, NULL);

	pthread_mutex_lock(&ip->lock);

	ip->table = hopen(lhsize);

	pthread_mutex_unlock(&ip->lock);

	return (lhashtable_t*) ip;
}

void lhclose(lhashtable_t *lhtp) {

	lhashtable_mod_t* ip = (lhashtable_mod_t*) lhtp;

	pthread_mutex_lock(&ip->lock);
	hclose(ip->table);
	pthread_mutex_unlock(&ip->lock);

	pthread_mutex_destroy(&ip->lock);
	free(ip);

}

int32_t lhput(lhashtable_t* lhtp, void* ep, const char *key, int keylen) {

	lhashtable_mod_t* ip = (lhashtable_mod_t*) lhtp;

	pthread_mutex_lock(&ip->lock);

	int32_t output = hput(ip->table, ep, key, keylen);

	pthread_mutex_unlock(&ip->lock);

	return output;

}

void lhapply(lhashtable_t* lhtp, void (*fn)(void* ep)) {

	lhashtable_mod_t* ip = (lhashtable_mod_t*) lhtp;

	pthread_mutex_lock(&ip->lock);

	happly(ip->table, fn);

	pthread_mutex_unlock(&ip->lock);

}

void* lhsearch(lhashtable_t* lhtp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char* key, int32_t keylen) {

	lhashtable_mod_t* ip = (lhashtable_mod_t*) lhtp;

	pthread_mutex_lock(&ip->lock);

	void* output = hsearch(ip->table, searchfn, key, keylen);

	pthread_mutex_unlock(&ip->lock);

	return output;

}

void* lhremove(lhashtable_t* lhtp, bool(*searchfn)(void* elementp, const void* searchkeyp), const char* key, int32_t keylen) {

	lhashtable_mod_t* ip = (lhashtable_mod_t*) lhtp;

	pthread_mutex_lock(&ip->lock);

	void* output = hremove(ip->table, searchfn, key, keylen);

	pthread_mutex_unlock(&ip->lock);

	return output;
}
