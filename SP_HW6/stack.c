#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <time.h>
#include "share.h"

int main(){
  Stack *ss;

  ss = getShareStruct();

  /*init shared struct, mutex and cond*/
  ss -> index = 0; //set init stack index

  /*init mutex*/
  pthread_mutexattr_t mutexAttr;
  pthread_mutexattr_init(&mutexAttr);
  pthread_mutexattr_setpshared(&mutexAttr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(&ss -> mutex, &mutexAttr);

  /*init cond*/
  pthread_condattr_t condAttr;
  pthread_condattr_init(&condAttr);
  pthread_condattr_setpshared(&condAttr, PTHREAD_PROCESS_SHARED);
  pthread_cond_init(&ss -> condition, &condAttr);

  /*ramdom time */
  srand(time(NULL));
  int delay[3] = {1, 2, 3};

  /*create child process to push and pop*/
  int pid;
  for(int i = 0; i < 5; i++){
    pid = fork();

    if(pid < 0){
      perror("fork");
      exit(1);
    } else if (pid == 0){
      if(i < 4){
        for(;;){
          int randInt = (rand() % 3);
          push('A', ss, getpid());
          sleep(delay[randInt]);
        }

      } else {
        for(;;){
          int randInt = (rand() % 3);
          pop(ss, getpid());
          sleep(delay[randInt]);
        }
      }
    }
  }
  wait(NULL);

}
