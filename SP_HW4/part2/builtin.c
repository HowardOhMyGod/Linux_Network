/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include <sys/param.h>
#include <string.h>
#include "shell.h"

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/
static void bi_builtin(char ** argv);	/* "builtin" command tells whether a command is builtin or not. */
static void bi_cd(char **argv) ;		/* "cd" command. */
static void bi_echo(char **argv);		/* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_hostname(char ** argv);	/* "hostname" command. */
static void bi_id(char ** argv);		/* "id" command shows user and group of this process. */
static void bi_pwd(char ** argv);		/* "pwd" command. */
static void bi_quit(char **argv);		/* quit/exit/logout/bye command. */




/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
  	char * keyword;					/* When this field is argv[0] ... */
  	void (* do_it)(char **);		/* ... this function is executed. */
} inbuilts[] = {
  	{ "builtin",    bi_builtin },   /* List of (argv[0], function) pairs. */

    /* Fill in code. */
    { "echo",       bi_echo },
    { "quit",       bi_quit },
    { "exit",       bi_quit },
    { "bye",        bi_quit },
    { "logout",     bi_quit },
    { "cd",         bi_cd },
    { "pwd",        bi_pwd },
    { "id",         bi_id },
    { "hostname",   bi_hostname },
    {  NULL,        NULL }          /* NULL terminated. */
};


static void bi_builtin(char ** argv) {
	struct cmd *tableCommand;

  for(tableCommand = inbuilts; tableCommand -> keyword; tableCommand++){
    if(strcmp(tableCommand -> keyword, argv[1]) == 0){
      printf("%s is a builtin feature.\n", argv[1]);
      return;
    }
  }
  printf("%s is NOT a builtin feature.\n", argv[1]);
  return;
}

static void bi_cd(char **argv) {
	/* Fill in code. */
  if(chdir(argv[1]) == -1){
    exit(1);
  }
}

static void bi_echo(char **argv) {
	/* Fill in code. */
  if (strcmp(argv[1], "-n") != 0){
			for(int i = 1; argv[i]; ++i){
				printf("%s ", argv[i]);
			}
			printf("\n");
		} else {
			// printf("detect -n\n");
			int num = atoi(argv[2]);
			printf("%s\n", argv[2 + num]);

		}
}

static void bi_hostname(char ** argv) {
	/* Fill in code. */
  struct utsname uts;

  if(uname(&uts) == -1){
    perror("hostinfo.c:main:uname");
    exit(1);
  }

  printf("hostname: %s\n", uts.nodename);
  return;
}

static void bi_id(char ** argv) {
 	/* Fill in code. */
  printf("UserID = %d, GroupID = %d\n", getuid(), geteuid());
  return;
}

static void bi_pwd(char ** argv) {
	/* Fill in code. */
  char *dir;
  long pathmaxlen = pathconf(".", _PC_PATH_MAX);

  dir = getcwd((char *)NULL, pathmaxlen + 1);

  if(dir == NULL){
    perror("getcwd");
    exit(1);
  }

  printf("%s\n", dir);
  free(dir);
}

static void bi_quit(char **argv) {
	exit(0);
}


/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd * this; /* close coupling between is_builtin & do_builtin */

/* Check to see if command is in the inbuilts table above.
Hold handle to it if it is. */
int is_builtin(char *cmd) {
	struct cmd *tableCommand;

  	for (tableCommand = inbuilts ; tableCommand->keyword != NULL; tableCommand++)
    	if (strcmp(tableCommand->keyword,cmd) == 0) {
      		this = tableCommand;
      		return 1;
    	}
  return 0;
}


/* Execute the function corresponding to the builtin cmd found by is_builtin. */
int do_builtin(char **argv) {
	this->do_it(argv);
}
