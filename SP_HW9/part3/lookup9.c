/*
 * lookup9 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet UDP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static struct sockaddr_in server, client;
	static int sockfd, namesize;
	struct hostent *host;
	static int first_time = 1;
	Dictrec temp;

	if (first_time) {  /* Set up server address & create local UDP socket */
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

		/*set client address*/
		memset((char *)&client, '\0', sizeof(client));
		client.sin_family = AF_INET;
		client.sin_port = htons(0);
		client.sin_addr.s_addr = htonl(INADDR_ANY);


		/* Allocate a socket.
		 * Fill in code. */
	 sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	 if(sockfd < 0){
			perror("socket");
			exit(1);
		}

		if(bind(sockfd, (struct sockaddr*) &client, sizeof(client)) == -1){
			perror("bind");
			exit(1);
		}
	}

	/* Send a datagram & await reply
	 * Fill in code. */
	strcpy(temp.word, sought -> word);
	sendto(sockfd, &temp, sizeof(Dictrec), 0, (struct sockaddr*)&server, sizeof(server));
	recvfrom(sockfd, &temp, sizeof(Dictrec), 0, (struct sockaddr*)&server, &namesize);
	strcpy(sought -> text, temp.text);

	if (strcmp(sought->text,"XXXX") != 0) {
		return FOUND;
	}

	return NOTFOUND;
}
