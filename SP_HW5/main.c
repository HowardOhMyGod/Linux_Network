#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>


/*define num of sem and sem_id */
#define NUM_SEMS_IN_GROUP 3
#define CPU_SEM 0
#define RAM_SEM 1
#define DISK_SEM 2

/*init sem with resource num*/
#define NUM_CORES 8
#define NUM_RAM 16
#define NUM_DISK 50

/* flags passed thru sembuf struct; wait for resource*/
#define WAIT 0
#define FALSE 0
#define TRUE (!FALSE)

#define STRING_SIZE 120



/*vm use in cores, ram, disk */
struct vm {
  int cores;
  int ram;
  int disk;
} vms[] = {
  {6, 5, 12},
  {2, 2, 50},
  {4, 5, 30},
  {2, 8, 12},
  {2, 8, 30},
};

/*Number of vms */
int numVms = sizeof(vms)/sizeof(struct vm);

/*IPC semaphoe identifier*/
int semid;

/*threads main function*/
void* threadMain(void * vmIndex);

int reserve(int semid, struct vm thisVm);
int release(int semid, struct vm thisVm);
int playWithSemaphore(int semid, int numCores, int numRam, int numDisk);

/************************************/
/* main function to init semaphore and create threads */
/************************************/
int main(){
  pthread_t thread_ids[numVms];

  /*need to convert constant for semctl call */
  int numCores = NUM_CORES;
  int numRam = NUM_RAM;
  int numDisk = NUM_DISK;


  /* allocte a single semaphore group with three semaphore in it*/
  if((semid = semget(IPC_PRIVATE, NUM_SEMS_IN_GROUP, IPC_CREAT | 0666) == -1)){
    perror("semget");
    exit(1);
  }

  /* init each semaphore in the group*/
  if((semctl(semid, CPU_SEM, SETVAL, &numCores)) ||
     (semctl(semid, RAM_SEM, SETVAL, &numRam)) ||
     (semctl(semid, DISK_SEM, SETVAL, &numDisk))) {
       perror("Error init semaphores");
     }

  /* init random number generator used for delay*/
  srand48(time(NULL));
  printf("Suere\n");

  /* create five threads for vm request */
  for(int j = 0; j < numVms; j++){
    if(pthread_create(&thread_ids[j], NULL, threadMain, (void *) j)){
      perror("pthread_create");
      exit(1);
    }

    sleep(500);
  }

  /* wait for threads to finish */
  for(int j = 0; j < numVms; j++){
    pthread_join(thread_ids[j], (void **) NULL);
  }

  return 0;

}
/************************************/
/* thread main function */
/************************************/
void* threadMain(void * vmIndex){
  int idx = (int) vmIndex;

  char string[STRING_SIZE];

  sprintf(string, "Job # %d requesting %d core, %d ram, $%d disk\n",
          idx, vms[idx].cores, vms[idx].ram, vms[idx].disk);

  /*get resource needed, wait if needed*/
  if(reserve(semid, vms[idx])){
    perror("reserve");
    return (NULL);
  }

  sprintf(string, "Job # %d got %d core, %d ram, $%d disk and is running\n",
        idx, vms[idx].cores, vms[idx].ram, vms[idx].disk);

  sleep(500);

  sprintf(string, "Job # %d done; return %d core, %d ram, $%d disk and is running\n",
        idx, vms[idx].cores, vms[idx].ram, vms[idx].disk);

  /*release resource*/
  if(release(semid, vms[idx])){
    perror("release");
  }

}
/************************************/
/******reserver resource*************
/************************************/
int reserve(int semid, struct vm thisVm){
  return (playWithSemaphore(semid, -thisVm.cores, -thisVm.ram, -thisVm.disk));
}

/* release resource*/
int release(int semid, struct vm thisVm){
  return (playWithSemaphore(semid, thisVm.cores, thisVm.ram, thisVm.disk));
}

/*allocate and deallocate resources*/
int playWithSemaphore(int semid, int numCores, int numRam, int numDisk){
  struct sembuf ops[NUM_SEMS_IN_GROUP];

  // memset(ops, '\0', sizeof(ops));

  ops[0].sem_num = CPU_SEM;
  ops[0].sem_op = numCores;
  ops[1].sem_num = RAM_SEM;
  ops[1].sem_op = numRam;
  ops[2].sem_num = DISK_SEM;
  ops[2].sem_op = numDisk;

  ops[0].sem_flg = ops[1].sem_flg = ops[2].sem_flg = 0;

  return (semop(semid, ops, NUM_SEMS_IN_GROUP));

}
