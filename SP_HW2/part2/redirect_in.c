/*
 * redirect_in.c  :  check for <
 */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "shell.h"
#define STD_OUTPUT 1
#define STD_INPUT  0

/*
 * Look for "<" in myArgv, then redirect input to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_in(char ** myArgv) {
  	int i;
  	int fd;
    int equal = 0;


  	/* search forward for <
  	 *
	 * Fill in code. */
    for( i = 0; myArgv[i] != NULL; ++i){
      if(strcmp(myArgv[i], "<") == 0){
        equal = 1;
        break;
      }
    }

  	if (equal) {	/* found "<" in vector. */
      	/* 1) Open file.
       	 * 2) Redirect stdin to use file for input.
     		 * 3) Cleanup / close unneeded file descriptors.
     		 * 4) Remove the "<" and the filename from myArgv.
  		 *
     		 * Fill in code. */
         fd = open(myArgv[i + 1], O_RDONLY, 0660);

  			 if(fd == -1){
  				 perror("open");
  				 exit(1);
  			 }
  			//  redirect for output
  			 close(0);

  			 if(dup(fd) != 0){
  				 fprintf(stderr, "dup fail to return 1");
  				 exit(1);
  			 }

  			 close(fd);

  			// remove > and filename
  			free(myArgv[i]);
  			free(myArgv[i + 1]);
  			myArgv[i] = NULL;
  			myArgv[i + 1] = NULL;
  	}
  	return 0;
}
