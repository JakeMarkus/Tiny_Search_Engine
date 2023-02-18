/* 
 * hash.c -- implements a generic hash table as an indexed set of queues.
 *
 */
#include <stdint.h>
#include "queue.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
/* 
 * SuperFastHash() -- produces a number between 0 and the tablesize-1.
 * 
 * The following (rather complicated) code, has been taken from Paul
 * Hsieh's website under the terms of the BSD license. It's a hash
 * function used all over the place nowadays, including Google Sparse
 * Hash.
 */
#define get16bits(d) (*((const uint16_t *) (d)))

static uint32_t SuperFastHash (const char *data,int len,uint32_t tablesize) {
  uint32_t hash = len, tmp;
  int rem;
  
  if (len <= 0 || data == NULL)
		return 0;
  rem = len & 3;
  len >>= 2;
  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }
  /* Handle end cases */
  switch (rem) {
  case 3: hash += get16bits (data);
    hash ^= hash << 16;
    hash ^= data[sizeof (uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2: hash += get16bits (data);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1: hash += *data;
    hash ^= hash << 10;
    hash += hash >> 1;
  }
  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;
  return hash % tablesize;
}

typedef struct hash_mod {
	uint32_t tablesize;
	queue_t** table_p;

} hash_mod_t;

hashtable_t* hopen(uint32_t hsize) {
	hash_mod_t* ip;

	if ( ! ( ip = (hash_mod_t*) malloc(sizeof(hash_mod_t)))) {
		printf("[Error: malloc failed allocating hash table]\n");
		return NULL;
	}

	ip->tablesize = hsize;

	if ( ! (ip->table_p = (queue_t**) malloc(sizeof(queue_t*) * hsize))) {
		printf("[Error: malloc failed allocating queues of the hash table]\n");
		return NULL;
	}

	for(int i=0; i < hsize; i++) {
		(ip->table_p)[i] = qopen();
	}

	return (hashtable_t*) ip;

}

void hclose(hashtable_t *htp) {

	hash_mod_t* ip = (hash_mod_t*) htp;

	for(int i=0; i < ip->tablesize; i++) {
		queue_t* currq = (ip->table_p)[i];
		qclose(currq);

	}

	free(ip->table_p);

	free(ip);

}

int32_t hput(hashtable_t* htp, void* ep, const char *key, int keylen) {

	hash_mod_t* ip = (hash_mod_t*) htp;

	uint32_t queue_index = SuperFastHash(key, keylen, ip->tablesize);

	queue_t* curr_queue = (ip->table_p)[queue_index];

	int32_t return_val = qput(curr_queue, ep);

	return return_val;

}

void happly(hashtable_t* htp, void (*fn)(void* ep)) {

	hash_mod_t* ip = (hash_mod_t*) htp;
	
	for(int i=0; i < ip->tablesize; i++) {
		queue_t* currq = (ip->table_p)[i];
		qapply(currq, fn);
	}

}

	void *hsearch(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp),const char *key, int32_t keylen){
		
		hash_mod_t* ip = (hash_mod_t*) htp;
		uint32_t queue_index = SuperFastHash(key, keylen, ip->tablesize);
		
		queue_t* currq = (ip->table_p)[queue_index];
		void* result =qsearch(currq, searchfn, key);
		return result;
	}

void *hremove(hashtable_t *htp,
							bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
	hash_mod_t* ip = (hash_mod_t*) htp;
	uint32_t queue_index = SuperFastHash(key, keylen, ip->tablesize); 
	queue_t* currq = (ip->table_p)[queue_index];
	void* result =qremove(currq, searchfn, key);
	return result;
}



	
	
