/*
 * fifo_server : listen on a named pipe; do lookup ; reply
 *               down another named pipe, the name of which
 *               will be sent by the client (in cli.id)
 *               argv[1] is the name of the local fil
 *               argv[2] is the name of the "well-known" FIFO
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dict.h"

int main(int argc, char **argv) {
	struct stat stbuff;
	int read_fd,write_fd;
	Client cli;
	Dictrec tryit;

	if (argc != 3) {
		fprintf(stderr,"Usage : %s <dictionary source> ""<resource / FIFO>\n",argv[0]);
		exit(errno);
	}

	/* Check for existence of dictionary and FIFO (both must exist)
	 *
	 * Fill in code. */
	 if(access("fixrec", F_OK) == -1 || access(argv[2], F_OK) == -1){
		 fprintf(stderr, "FIFO or dict not found!\n");
		 exit(1);
	 }


	/* Open FIFO for reading (blocks until a client connects)
	 *
	 * Fill in code. */
	 read_fd = open(argv[2], O_RDONLY);

	 if(read_fd < 0){
		 perror("open");
		 exit(1);
	 }

	/* Sit in a loop. lookup word sent and send reply down the given FIFO */
	for (;;) {

		/* Read request.
		 *
		 * Fill in code. */
		 read(read_fd, &cli, sizeof(Client));
	   strcpy(tryit.word, cli.word);

		/* Get name of reply fifo and attempt to open it.
		 *
		 * Fill in code. */
		write_fd = open(cli.id, O_WRONLY);

		if(write_fd < 0){
			perror("open cli.id");
			exit(1);
		}

		/* lookup the word , handling the different cases appropriately
		 *
		 * Fill in code. */
		switch(lookup(&tryit, argv[1])) {
			case FOUND:
				/* Fill in code. */
				// printf("%s\n", tryit.text);
				write(write_fd, tryit.text, TEXT);
				break;
			case NOTFOUND:
				/* Fill in code. */
				write(write_fd, "XXXX", TEXT);
				break;
			case UNAVAIL:
				/* Fill in code.  */
				break;

		}

		/* close connection to this client (server is stateless)
		 *
		 * Fill in code. */
		 close(write_fd);
	}
}
