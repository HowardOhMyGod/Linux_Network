#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>

int main(){
  char *dir;
  long pathmaxlen = pathconf(".", _PC_PATH_MAX);

  dir = getcwd((char *)NULL, pathmaxlen + 1);

  if(dir == NULL){
    perror("getcwd");
    exit(1);
  }

  printf("%s\n", dir);
  free(dir);
}
