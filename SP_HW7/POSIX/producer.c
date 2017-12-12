#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "../packet.h"

int main(int argc, char**argv){
  int shm_fd, shm_idx_fd;
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

  /* create shm obj*/
  shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  shm_idx_fd = shm_open(IDX_SHARE, O_CREAT | O_RDWR, 0666);

  /*determine size of shm*/
  ftruncate(shm_fd, SIZE);
  ftruncate(shm_idx_fd, sizeof(int));

  /*map the shared obj and get addr*/
  init_shm_addr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  index = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_idx_fd, 0);

  /* -1 indicate no packet written*,
    index mean id 0~index packet has been written
  */
  *index = -1;

  if(init_shm_addr == MAP_FAILED || index == MAP_FAILED){
    perror("mmap");
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

}
