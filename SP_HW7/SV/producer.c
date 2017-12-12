#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "../packet.h"

int main(int argc, char** argv){
  int shm_id, shm_id_idx;
  Packet *init_shm_addr;
  int *index;

  if(argc != 2){
    fprintf(stderr, "Usage: ./producer [shmblocks]\n");
    exit(1);
  }

  int shmblocks = atoi(argv[1]);

  if(shmblocks > 30 || shmblocks < 5){
    fprintf(stderr, "shmblocks should between 5 - 30\n");
    exit(1);
  }

  const int SIZE = shmblocks * sizeof(Packet);

  /*set shared memory if exist or create a new one*/
  shm_id = shmget(SHM_KEY, SIZE, IPC_CREAT | 0666);
  shm_id_idx = shmget(SHM_KEY_IDX, sizeof(int), IPC_CREAT | 0666);

  if(shm_id < 0 || shm_id_idx < 0){
    perror("shmget");
    exit(1);
  }

  /*get the shared struct*/
  init_shm_addr = (Packet *)shmat(shm_id, NULL, 0);
  index = (int *)shmat(shm_id_idx, NULL, 0);

  *index = -1;

  if(init_shm_addr == (Packet *) -1 || index == (int *) - 1){
    perror("shmat");
    exit(1);
  }

  /*wrtie packet to shm*/
  int i, id;
  sleep(15);
  for(id = 0, i = 0; i < ID_MAX; id++, i++){
    Packet thisPkt;

    /*rewirte init*/
    if(id == shmblocks){
      id = 0;
    }

    thisPkt.id = i;
    sprintf(thisPkt.dataByte, "pkt%d", i);

    init_shm_addr[id] = thisPkt;
    // printf("id: %d\n", init_shm_addr[id].id);
    // sleep(0.2);

    (*index)++;
    sleep(0.2);
  }

  // printf("Producer index: %d\n", *index);
  sleep(5);
  *index = -1;

  return 0;
}
