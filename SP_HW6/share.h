/*share.h*/
#include <pthread.h>

/* shared struct in memory */
#define SIZE 3

typedef struct {
  int index;
  char stack[SIZE];
  pthread_mutex_t mutex;
  pthread_cond_t condition;
} Stack;

#define S_SIZE sizeof(Stack)

/*shared menory object*/
#define STACK_KEY 1234

/*funtions define*/
Stack *getShareStruct();
void push(char oneChar, Stack *shareStruct, int pid);
char pop(Stack *shareStruct, int pid);
void printShare(Stack *ss);
