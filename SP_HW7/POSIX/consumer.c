#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "../packet.h"


int main(int argc, char**argv){
  int shm_fd, shm_idx_fd;
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

  /* create shm obj*/
  shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  shm_idx_fd = shm_open(IDX_SHARE, O_CREAT | O_RDWR, 0666);

  /*determine size of shm*/
  ftruncate(shm_fd, SIZE);
  ftruncate(shm_idx_fd, sizeof(int));

  /*map the shared obj and get addr*/
  init_shm_addr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  index = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_idx_fd, 0);

  if(init_shm_addr == MAP_FAILED || index == MAP_FAILED){
    perror("mmap");
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
