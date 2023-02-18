/* test_indexsave.c --- 
 * 
 * 
 * Author: Eren Berke Saglam
 * Created: Thu Feb 16 11:39:12 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include "indexio.h"


int main(void) {
	indexsave("../pages/", "../indexes/", "index_3", 3);

	hashtable_t* table = indexload("../indexes/index_3");

	freeIndexTable(table);
	
}
