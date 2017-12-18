/*
 * lookup6 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by shared memory
 *
 * Synchronization is by two semaphores:
 *   One excludes other clients while we talk to server
 *   The other is used BOTH to alert the server and to await his reply
 *   Client raises a semaphore by 2. Server drops by 1 before and
 *   after doing lookup
 * The same key (which is passed as resource) is used BOTH
 * for shared memory & semaphores.
 */

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	static int shmid,semid;
	long key = strtol(resource,(char **)NULL,0);
	static Dictrec * shm;
	struct sembuf grab    = {0,-1,SEM_UNDO};   /* mutex other clients  */
	struct sembuf release = {0,1,SEM_UNDO};    /* release mtx to other clients */
	struct sembuf alert   = {1,2,SEM_UNDO};    /* wake up server       */
	struct sembuf await   = {1,0,0};           /* wait for server      */
	static int first_time = 1;

	if (first_time) {        /* attach to shared memory & semaphores */
		first_time = 0;

		/* Connect to shared memory.
		 * Fill in code. */
		// printf("key: %ld\n", key);
		shmid = shmget(key, sizeof(Dictrec), IPC_CREAT | 0666);
	  if(shmid < 0){
			perror("shmget");
			exit(1);
		}

		// printf("shmid: %d\n", shmid);

		/* Get shared memory virtual address.
		 * Fill in code. */
	  shm = (Dictrec *) shmat(shmid, NULL, 0);

		if(shm == (Dictrec *) - 1){
			perror("shmat");
			// exit(1);
		}

		/* Get semaphore.
		 * Fill in code. */
		semid = semget(key, 2, 0666 | IPC_CREAT);
		// printf("semid: %d\n", semid);
	}

	/* Reserve semaphore so other clients will wait.
	 * Fill in code. */
	//  printf("grab!\n");
	 if(semop(semid, &grab, 1) < 0){
 		perror("59 semop");
 	}
	// printf("grab gggg!\n");

	strcpy(shm->word,sought->word);
	// printf("shm -> word: %s", shm->word);

	/* Alert server.  Bump semaphore 1 up by 2.
	 * Fill in code. */
	 if(semop(semid, &alert, 1) < 0){
 		perror("68 semop");
 	}

	/* Wait for server to finish.  Server will have set sem 1 to zero.
	 * Fill in code. */
	 if(semop(semid, &await, 1) < 0){
 		perror("74 semop");
 	}

	/* Done using the server.  Release to other clients.
	 * Fill in code. */
	 if(semop(semid, &release, 1) < 0){
		 perror("80 semop");
	 }

	if (strcmp(shm->text,"XXXX") != 0) {
		strcpy(sought->text,shm->text);
		return FOUND;
	}

	return NOTFOUND;
}
