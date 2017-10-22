/*
 * pipe_command.c  :  deal with pipes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#include "shell.h"

#define STD_OUTPUT 1
#define STD_INPUT  0

void pipe_and_exec(char **myArgv) {
  	int pipe_argv_index = pipe_present(myArgv);
  	int pipefds[2];
	  char **left_argv;
	  char **right_argv;

    int left_count = 1;
    int right_count = 1;

    int i, j;

    // printf("Pipe at %d\n", pipe_argv_index);

  	switch (pipe_argv_index) {

    	case -1:	/* Pipe at beginning or at end of argv;  See pipe_present(). */
      		fputs ("Missing command next to pipe in commandline.\n", stderr);
      		errno = EINVAL;	/* Note this is NOT shell exit. */
      		break;

    	case 0:	/* No pipe found in argv array or at end of argv array.
			See pipe_present().  Exec with whole given argv array. */
      		break;

    	default:	/* Pipe in the middle of argv array.  See pipe_present(). */

      		/* Split arg vector into two where the pipe symbol was found.
       		 * Terminate first half of vector.
			 *
       		 * Fill in code. */

          left_argv = malloc(sizeof(char * ) * (left_count + 1));
          right_argv = malloc(sizeof(char * ) * (right_count + 1));

          //  left argvs
          for(j = 0; j != pipe_argv_index; ++j){
            if (left_count != 1){
              left_argv = realloc(left_argv, (left_count + 1)*sizeof(char *));
            }

            left_argv[j] = malloc((strlen(myArgv[j]) + 1)*sizeof(char));
            strcpy(left_argv[j], myArgv[j]);
            // printf("left : %s\n", left_argv[j]);
            left_count++;
          }
          left_argv[j] = NULL;

          //  right argvs
          for(i = pipe_argv_index + 1, j = 0; myArgv[i]; ++i, ++j){
            if (right_count != 1){
              right_argv = realloc(right_argv, (right_count + 1)*sizeof(char *));
            }

            right_argv[j] = malloc((strlen(myArgv[i]) + 1)*sizeof(char));
            strcpy(right_argv[j], myArgv[i]);
            // printf("right : %s\n", right_argv[j]);
            right_count++;
          }
          right_argv[j] = NULL;



      		/* Create a pipe to bridge the left and right halves of the vector.
			 *
			 * Fill in code. */
          pipe(pipefds);

      		/* Create a new process for the right side of the pipe.
       		 * (The left side is the running "parent".)
       		 *
			 * Fill in code to replace the underline. */
          pid_t pid;


      		switch(pid = fork()) {

        		case -1 :
	  				break;

        		/* Listening parent.  Remember this is a child forked from shell. */
        		default :
                // wait for child to wirte command result to pipe
                wait(&pid);

                close(pipefds[1]);
                close(0);

                dup2(pipefds[0], 0);
                execvp(right_argv[0], right_argv);

          			pipe_and_exec(&myArgv[pipe_argv_index+1]);

    	  				break;

        		/* Writing child. */
        		case 0 :
                // write result to pipe
                close(pipefds[0]);
                close(1);

                dup2(pipefds[1], 1);
                execvp(left_argv[0], left_argv);

                exit(1);
                break;
			}
	}
	// perror("Couldn't fork or exec child process");
  // 	exit(errno);
}
