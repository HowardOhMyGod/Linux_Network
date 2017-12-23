/*
 * lookup8 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet TCP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int sd;
	static struct sockaddr_in server;
	struct hostent *host;
	static int first_time = 1;
	Dictrec temp;

	if (first_time) {        /* connect to socket ; resource is server name */
		first_time = 0;

		/* Set up destination address. */
		memset((char *)&server, '\0', sizeof(server));
		server.sin_family = AF_INET;
		server.sin_port = PORT;

		if((host = gethostbyname(resource)) == NULL){
			perror("gethostbyname");
			exit(1);
		}

		memcpy((char *) &server.sin_addr, host -> h_addr, host -> h_length);

		/* Allocate socket.
		 * Fill in code. */
		sd = socket(AF_INET, SOCK_STREAM, 0);
 		if(sd < 0){
 			perror("socket");
 			exit(1);
 		}

		/* Connect to the server.
		 * Fill in code. */
		if(connect(sd, (struct sockaddr *) &server, sizeof(server))){
			perror("connect");
			exit(1);
		}
	}

	/* write query on socket ; await reply
	 * Fill in code. */
	strcpy(temp.word, sought -> word);
 	send(sd, &temp, sizeof(Dictrec), 0);
 	recv(sd, &temp, sizeof(Dictrec), 0);
 	strcpy(sought -> text, temp.text);

	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
