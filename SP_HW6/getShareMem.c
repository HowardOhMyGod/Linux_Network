#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "share.h"

/*get shared struct address and return it */
Stack *getShareStruct(){
  int shm_id;
  Stack *ss;

  /*set shared memory if exist or create a new one*/
  shm_id = shmget(STACK_KEY, S_SIZE, IPC_CREAT | 0666);

  if(shm_id < 0){
    perror("shmget");
    exit(1);
  }

  /*get the shared struct*/
  ss = (Stack *)shmat(shm_id, NULL, 0);

  if(ss == (Stack *) -1){
    perror("shmat");
    exit(1);
  }

  /*return shared struct address*/
  return ss;
}

void push(char oneChar, Stack *shareStruct, int pid){
  pthread_mutex_lock(&shareStruct -> mutex);

  /*stack is not pushable, wait and release lock use cond_wait*/
  while(shareStruct -> index == SIZE){
    printf("Stack full! #%d Waiting....\n", pid);
    pthread_cond_wait(&shareStruct -> condition, &shareStruct -> mutex);
  }

  /*stack is pushable*/
  (shareStruct -> stack)[shareStruct -> index] = oneChar;
  (shareStruct -> index)++;

  printf("#%d Pushed: char %c, index: %d\t", pid, oneChar, (shareStruct -> index) - 1);
  printf("Stack: %s\n", shareStruct -> stack);

  pthread_cond_signal(&shareStruct -> condition);
  pthread_mutex_unlock(&shareStruct -> mutex);

}

char pop(Stack *shareStruct, int pid){
  char toReturn;

  pthread_mutex_lock(&shareStruct -> mutex);

  while(shareStruct -> index == 0){
    printf("Stack empty! #%d Waiting....\n", pid);
    pthread_cond_wait(&shareStruct -> condition, &shareStruct -> mutex);
  }

  /*pop chat from stack*/
  toReturn = (shareStruct -> stack)[(shareStruct -> index) - 1];
  (shareStruct -> stack)[(shareStruct -> index) - 1] = '\0';
  (shareStruct -> index)--;

  printf("#%d Poped: char %c, index: %d\t", pid, toReturn, (shareStruct -> index));
  printf("Stack: %s\n", shareStruct -> stack);

  pthread_cond_signal(&shareStruct -> condition);
  pthread_mutex_unlock(&shareStruct -> mutex);

  return toReturn;
}

void printShare(Stack *ss){
  printf("index: %d\n", ss -> index);
  printf("name: %s\n", ss -> stack);
}
