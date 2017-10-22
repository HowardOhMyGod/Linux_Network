/*
 *  pipe_present.c :  check for |
 */

#include <stdio.h>
#include "shell.h"
#include <string.h>

/*
 * Return index offset into argv of where "|" is,
 * -1 if in an illegal position (first or last index in the array),
 * or 0 if not present.
 */
int pipe_present(char ** myCurrentArgv) {
	int index = 0;
	int notExist = 1;

  	/* Search through myCurrentArgv for a match on "|". */
		for(index = 0; myCurrentArgv[index]; index++){
			if(strcmp(myCurrentArgv[index], "|") == 0){
				notExist = 0;
				break;
			}
		}

		// occur the first or the end
  	if ((index == 0 && !notExist) || (!notExist && myCurrentArgv[index + 1] == NULL)) {
    	return -1;

		// not exist
	} else if (notExist) {
    	return 0;

  	} else {
    	/* In the middle. */
    	return index;
  	}
}

// int main(){
// 	char *myargv1[] = {"|", "ls", NULL};
// 	char *myargv2[] = {"ls", "d", "|", "ld", NULL};
// 	char *myargv3[] = {"ls", "|", NULL};
//
// 	printf("Head : %d\n", pipe_present(myargv1));
// 	printf("Middle: %d\n", pipe_present(myargv2));
// 	printf("End : %d\n", pipe_present(myargv3));
//
// 	return 0;
// }
