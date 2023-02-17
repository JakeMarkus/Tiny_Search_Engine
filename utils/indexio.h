#pragma once
/* 
 * pageio.h --- saving and loading crawler webpage files
 * 
 * Author: Stephen Taylor
 * Created: Fri Oct 19 08:30:15 2018 (-0400)
 * Version: 1.0
 * 
 * Description: pagesave saves an existing webpage to a file with a
 * numbered name (e.g. 1,2,3 etc); pageload creates a new page by
 * loading a numbered file. For pagesave, the directory must exist and
 * be writable; for loadpage it must be readable.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <webpage.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>

#include "pageio.h"
#include "webpage.h"
#include "hash.h"
#include "queue.h"


int32_t indexsave(char* pages_dir, char* index_dir, int n);

/* 
 * pageload -- loads the numbered filename <id> in direcory <dirnm>
 * into a new webpage
 *
 * returns: non-NULL for success; NULL otherwise
 */
hashtable_t* indexload(char* file_path);
 
