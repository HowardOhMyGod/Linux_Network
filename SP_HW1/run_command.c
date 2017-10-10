/*
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"
#include <string.h>

void run_command(char **myArgv) {
    pid_t pid;
    int stat;
    char **inputArgv;
    /* Create a new child process.
     * Fill in code.
	 */
    pid = fork();

    switch (pid) {

        /* Error. */
        case -1 :
            perror("fork");
            exit(errno);

        /* Parent. */
        default :
          if(!is_background(myArgv)){
            wait(&pid);
          }

            /* Wait for child to terminate.
             * Fill in code.
			 */

            /* Optional: display exit status.  (See wstat(5).)
             * Fill in code.
			 */

            return;

        /* Child. */
        case 0 :
          if(is_background(myArgv)){
            for(int i = 0; myArgv[i]; i++){
              if(strcmp(myArgv[i], "&") == 0){
                free(myArgv[i]);
                myArgv[i] = NULL;
              }
            }
          }
          stat = execvp(myArgv[0], myArgv);

          if(stat < 0) {
            exit(errno);
          }
          /* Run command in child process.
             * Fill in code.
			 */

            /* Handle error return from exec */

    }
}
