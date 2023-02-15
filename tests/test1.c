
/* test1.c --- 
 * 
 * 
 * Author: Eren Berke Saglam
 * Created: Mon Feb 13 19:01:09 2023 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include "webpage.h"
#include <stdio.h>
#include "pageio.h"
#include <string.h>

int compare_webpage(webpage_t* file1, webpage_t* file2) {

	if (webpage_getDepth(file1) == webpage_getDepth(file2)) {
		if (strcmp(webpage_getHTML(file1), webpage_getHTML(file2)) == 0) {
			if (strcmp(webpage_getURL(file1), webpage_getURL(file2)) == 0) {
				return 0;
			}
		}
	}

	return 1;
}

int main(void) {
	
	webpage_t* my_page = pageload(1, "../pages/");

	pagesave(my_page, 10000000, "../pages/");

	webpage_t* my_page_2 = pageload(10000000, "../pages/");

	int num = compare_webpage(my_page, my_page_2);

  printf("%d\n", num);

	webpage_delete(my_page);
	webpage_delete(my_page_2);

}
