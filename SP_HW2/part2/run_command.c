/*
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"
#include <string.h>

void run_command(char **myArgv) {
  	pid_t pid;
  	int stat;
    int exe_stat;
  	int run_in_background;

  	/*
   	* Check for background processing.
   	* Do this before fork() as the "&" is removed from the argv array
   	* as a side effect.
   	*/
  	run_in_background = is_background(myArgv);

  	switch(pid = fork()) {

    	/* Error. */
    	case -1 :
      		perror("fork");
      		exit(errno);

    	/* Child. */
    	case 0 :
      		/* Redirect input and update argv. */
          redirect_in(myArgv);
          // printf("after rI\n");
      		// /* Redirect output and update argv. */
          redirect_out(myArgv);

      		// pipe_and_exec(myArgv);

          exe_stat = execvp(myArgv[0], myArgv);

          if(exe_stat < 0) {
            exit(errno);
          }

          return;
      /* Parent. */
    	default :
          // wait(&pid);
      		if (!run_in_background) {

        		waitpid(pid,&stat,0);	/* Wait for child to terminate. */

        		if (WIFEXITED(stat) && WEXITSTATUS(stat)) {
          			fprintf(stderr, "Child %d exited with error status %d: %s\n",
	      				pid, WEXITSTATUS(stat), (char*)strerror(WEXITSTATUS(stat)));

        		} else if (WIFSIGNALED(stat) && WTERMSIG(stat)) {
	  				fprintf (stderr, "Child %d exited due to signal %d: %s\n",
	      				pid, WTERMSIG(stat), (char*)strsignal(WTERMSIG(stat)));
        		}
      		}
      		return;
	}
}
