#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


/*define resource */
#define CPU_CORES 8
#define RAM 16
#define DISK 500

void printAllVm(void);

/*vm use in cores, ram, disk */
struct vm {
  int cores;
  int ram;
  int disk;
} vms[] = {
  {6, 5, 125},
  {2, 2, 500},
  {4, 5, 300},
  {2, 8, 125},
  {2, 8, 300},
};

/*Number of vms */
int numVms = sizeof(vms)/sizeof(struct vm);

/*threads main function*/
void* threadMain(void * vmIndex);

/************************************/
/* main function to init semaphore and create threads */
/************************************/
int main(){
  sem_t cores_sem, ram_sem, disk_sem;
  pthread_t thread_ids[numVms];

  /* initialize semaphore*/
  if(sem_init(&cores_sem, 0, CPU_CORES) < 0 ||
  sem_init(&ram_sem, 0, CPU_CORES) < 0 ||
  sem_init(&disk_sem, 0, CPU_CORES) < 0){
    perror("sem_init");
    exit(1);
  }

  /* create five threads for vm request */
  for(int j = 0; j < numVms; j++){
    if(pthread_create(&thread_ids[j], NULL, threadMain, (void *) j)){
      perror("pthread_create");
      exit(1);
    };
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

  printf("cores: %d\n", vms[idx].cores);
  printf("RAM: %d\n", vms[idx].ram);
  printf("cores: %d\n", vms[idx].disk);
  printf("---------\n");
}
/************************************/
void printAllVm(void){
  for(int i = 0; vms[i].cores != 0; i++){
    printf("cores: %d\n", vms[i].cores);
    printf("RAM: %d\n", vms[i].ram);
    printf("cores: %d\n", vms[i].disk);
    printf("---------\n");
  }
}
