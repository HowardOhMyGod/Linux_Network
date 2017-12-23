/*
 * isock_server : listen on an internet socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

#include "dict.h"

int main(int argc, char **argv) {
	static struct sockaddr_in server, client;
	struct hostent *host;
	int sd,cd,rval, clen = sizeof(client);
	Dictrec tryit;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(1);
	}

	/* Create the socket.
	 * Fill in code. */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		perror("socket");
		exit(1);
	}

	/* Initialize address.
	 * Fill in code. */
	memset((char *)&server, '\0', sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = PORT;

	if((host = gethostbyname("localhost")) == NULL){
		perror("gethostbyname");
		exit(1);
	}

	/* Name and activate the socket.
	 * Fill in code. */
	/* set addr*/
	memcpy((char *) &server.sin_addr, host -> h_addr, host -> h_length);

	/*bind*/
	if(bind(sd, (struct sockaddr*) &server, sizeof(server))){
		perror("bind");
		exit(1);
	}

	/*listen*/
	if(listen(sd, 5* sizeof(Dictrec))){
		perror("listen");
		exit(1);
	}
	/* main loop : accept connection; fork a child to have dialogue */
	for (;;) {

		/* Wait for a connection.
		 * Fill in code. */
		 cd = accept(sd, (struct sockaddr *)&client, &clen);
		if(cd == -1){
			perror("accept");
			exit(1);
		}

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
					/* Lookup the word , handling the different cases appropriately */
					switch(lookup(&tryit,argv[1]) ) {
						/* Write response back to the client. */

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
				exit(0); /* child does not want to be a parent */

			default :
				close(cd);
				break;
		} /* end fork switch */
	} /* end forever loop */
} /* end main */
