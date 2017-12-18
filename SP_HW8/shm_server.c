 /*
 * shm_server : wait on a semaphore; do lookup ; use shared
 *              memory for communication ; notify using same
 *              semaphore (client raises by 2; server lowers
 *              by 1 both to wait and to notify)
 *
 *              argv[1] is the name of the local file
 *              argv[2] is the key for semaphores + shared mem
 */

#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"

int main(int argc, char **argv) {
	int shmid,semid;
	long key;
	int fd;
	Dictrec * shm;
	struct stat stbuff;
	extern int errno;
	unsigned short vals[2] = { 1 , 0 };        /* Initial values of semaphores */

	union semun {                        /* Needed for semctl for setup. */
		int             val;
		struct semid_ds *buf;
		unsigned short  *array;
	} setit ;

	struct sembuf wait = {1,-1,SEM_UNDO}; /* used BOTH to wait & notify */

	if (argc != 3) {
		fprintf(stderr,"Usage : %s <dictionary source> <Resource/Key>\n",argv[0]);
		exit(errno);
	}

	/* Get key from commandline argument. */
	key = strtol(argv[2],(char **)NULL,0);
	// printf("key: %ld\n", key);
	/* Create & initialize shared memory & semaphores */

	/* Allocate a group of two semaphores.  Use same key as for shmem.
	 * Fill in code. */
	semid = semget(key, 2, 0666 | IPC_CREAT);
	// printf("semid: %d\n", semid);

	/* Set up semaphore group. */
	setit.val = 1;
	if(semctl(semid, 0, SETVAL, setit) < 0){
		perror("semctl 58");
		exit(1);
	}
	setit.val = 0;
	if(semctl(semid, 1, SETVAL, setit) < 0){
		perror("semctl 58");
		exit(1);
	}


	/* Verify database resource is present. */
	if (stat(argv[1],&stbuff) == -1)
		DIE(argv[1]);

	/* Map one record's worth of shared memory.
	 * The word of the sent value will be the request,
	 * and the text of the result will be returned as the answer.
	 *
	 * Fill in code. */
	shmid = shmget(key, sizeof(Dictrec), IPC_CREAT | 0666);
 	if(shmid < 0){
		perror("shmget");
		exit(errno);
	}
	// printf("shmid: %d\n", shmid);

	/* Get shared memory virtual address.
	 * Fill in code. */
  shm = (Dictrec *)shmat(shmid, NULL, 0);

 	if(shm == (Dictrec *) - 1){
 		perror("shmat");
 		exit(errno);
 	}

	/* Main working loop */
	for (;;) {

		/* When we are first started, value is zero.  Client sets to two to wake us up.
		 * Try to decrement sem 1.
		 * Then we will wait here until the semaphore is non-zero
		 *
		 * Fill in code. */
		// printf("for loop...\n");

		if(semop(semid, &wait, 1) < 0){
			perror("semop");
		}

		// printf("request word: %s\n", shm -> word);

		/* Do the lookup here.  Write result directly into shared memory. */
		switch(lookup(shm,argv[1]) ) {
			case FOUND:
				break;
			case NOTFOUND:
				strcpy(shm->text,"XXXX");
				break;
			case UNAVAIL:
				DIE(argv[1]);
		}

		/* Decrement again so that we will block at the top of the for loop again until a client wakes us up again.
		 *
		 * Fill in code. */
	  if(semop(semid, &wait, 1) < 0){
			 perror("semop");
 		}

	} /* end for */
}
