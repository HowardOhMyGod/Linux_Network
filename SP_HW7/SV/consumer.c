#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "../packet.h"

int main(int argc, char **argv){
  int shm_id, shm_id_idx;
  Packet *init_shm_addr;
  Packet readed;
  int *index;

  if(argc != 2){
    fprintf(stderr, "Usage: ./consumer [shmblocks]\n");
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

  if(init_shm_addr == (Packet *) -1 || index == (int *) - 1){
    perror("shmat");
    exit(1);
  }

  /*wrtie packet to shm*/
  int total_read = 0;

  // printf("Starting to read from shm...\n");

  for(int i = 0; i < shmblocks; i++){
    while(*index == -1){
      /*wait for producer write*/
    }

    memcpy(&readed, &init_shm_addr[i], sizeof(Packet));
    printf("read: %d\n", readed.id);

    total_read += 1;
    if(i == shmblocks - 1){
      i = -1;
    }

    /* client has read the last pkt, terminate*/
    if(readed.id == (ID_MAX - 1)){
      break;
    }

    sleep(0.2);

  }

  printf("\n%d lost: %d datas\n", getpid(), ID_MAX - total_read);
  // printf("total: %d\n", total_read);

  return 0;
}
