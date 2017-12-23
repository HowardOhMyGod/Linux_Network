/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include "dict.h"
#include <string.h>

int main(int argc, char **argv) {
    struct sockaddr_un server, client;
    int sd,cd,n;
    Dictrec tryit;
    int rval;

    if (argc != 3) {
      fprintf(stderr,"Usage : %s <dictionary source>"
          "<Socket name>\n",argv[0]);
      exit(errno);
    }

    /* Setup socket.
     * Fill in code. */
    sd = socket(AF_UNIX, SOCK_STREAM, 0);

    /* Initialize address.
     * Fill in code. */
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, argv[2]);
    unlink(server.sun_path);

    /* Name and activate the socket.
     * Fill in code. */
    if(bind(sd, (struct sockaddr *) &server, sizeof(server)) == -1){
      perror("bind");
      exit(errno);
    }

    if(listen(sd, 5 * sizeof(Dictrec)) == -1){
      perror("listen");
      exit(errno);
    }

    /* main loop : accept connection; fork a child to have dialogue */
    for (;;) {
		/* Wait for a connection.
		 * Fill in code. */
    n = sizeof(client);
    cd = accept(sd, (struct sockaddr *)&client, &n);

		/* Handle new client in a subprocess. */
		switch (fork()) {
			case -1 :
				DIE("fork");
			case 0 :
				close (sd);	/* Rendezvous socket is for parent only. */
				/* Get next request.
				 * Fill in code. */
        rval = recv(cd, &tryit, sizeof(Dictrec), 0);

				while (rval > 0 || rval >= sizeof(Dictrec)) {

					/* Lookup request. */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to client. */
						case FOUND:
							/* Fill in code. */
              rval = send(cd, &tryit, sizeof(Dictrec), 0);
              if (rval < 0){
                perror("send");
              }
							break;
						case NOTFOUND:
							/* Fill in code. */
              strcpy(tryit.text, "XXXX");
              rval = send(cd, &tryit, sizeof(Dictrec), 0);
              if (rval < 0){
                perror("send");
              }
							break;
						case UNAVAIL:
							DIE(argv[1]);
					} /* end lookup switch */
          
          /* get new request*/
          rval = recv(cd, &tryit, sizeof(Dictrec), 0);

				} /* end of client dialog */

				/* Terminate child process.  It is done. */
				exit(0);

			/* Parent continues here. */
			default :
				close(cd);
				break;
		} /* end fork switch */
    } /* end forever loop */
} /* end main */
