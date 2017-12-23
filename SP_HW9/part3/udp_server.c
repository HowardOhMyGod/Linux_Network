/*
 * udp_server : listen on a UDP socket ;reply immediately
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
	static struct sockaddr_in server,client;
	int sockfd, rval,clen = sizeof(client);
	struct hostent *host;
	Dictrec tryit;

	if (argc != 2) {
		fprintf(stderr,"Usage : %s <datafile>\n",argv[0]);
		exit(errno);
	}

	/* Create a UDP socket.
	 * Fill in code. */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
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
	 memcpy((char *) &server.sin_addr, host -> h_addr, host -> h_length);

 	/*bind*/
 	if(bind(sockfd, (struct sockaddr*) &server, sizeof(server))){
 		perror("bind");
 		exit(1);
 	}

	for (;;) { /* await client packet; respond immediately */

		/* Wait for a request.
		 * Fill in code. */
		rval = recvfrom(sockfd, &tryit, sizeof(Dictrec), 0, (struct sockaddr *)&client, &clen);
		
		while (rval > 0 || rval >= sizeof(Dictrec)) {
			/* Lookup request and respond to user. */
			switch(lookup(&tryit,argv[1])) {
				case FOUND:
					/* Send response.
					 * Fill in code. */

					rval = sendto(sockfd, &tryit, sizeof(Dictrec), 0, (struct sockaddr *)&client, clen);
 					if (rval < 0){
 						perror("send");
 					}
					break;
				case NOTFOUND :
					/* Send response.
					 * Fill in code. */
					strcpy(tryit.text, "XXXX");
					rval = sendto(sockfd, &tryit, sizeof(Dictrec), 0, (struct sockaddr *)&client, clen);
 					if (rval < 0){
 						perror("send");
 					}
					break;
				case UNAVAIL:
					DIE(argv[1]);
			} /* end lookup switch */

				rval = recvfrom(sockfd, &tryit, sizeof(Dictrec), 0, (struct sockaddr *)&client, &clen);

		} /* end while */
	} /* end forever loop */
} /* end main */
