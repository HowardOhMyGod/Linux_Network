/*
 * shell.c  : test harness for parse routine
 */

#define LONGLINE 255

#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include <signal.h>

int main(int argc, char* argv[]) {
		char line[LONGLINE];
  	char **myArgv;

		sigset_t set, toBlock;

		//initialize toblock to all 0's
		sigemptyset(&toBlock);
		sigaddset(&toBlock, SIGINT);
		sigaddset(&toBlock, SIGQUIT);

		sigprocmask(SIG_BLOCK, &toBlock, &set);
		// sigprocmask(SIG_SETMASK, &set, (sigset_t *) NULL);

  	fputs("myshell -> ",stdout);
  	while (fgets(line,LONGLINE,stdin)) {

    	/* Create argv array based on commandline. */
    	if ((myArgv = parse(line))!= NULL) {

      		/* If command is recognized as a builtin, do it. */
      		if (is_builtin(myArgv[0])) {
        		do_builtin(myArgv);

			/* Non-builtin command. */
			} else {
				run_command(myArgv);
			}

			/* Free argv array. */
			free_argv(myArgv);
		}

    	fputs("myshell -> ",stdout);
	}
  return 0;
}
