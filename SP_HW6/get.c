#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "share.h"

int main(){
  Stack *ss;

  ss = getShareStruct();
  push('o', ss);
  printShare(ss);
}
