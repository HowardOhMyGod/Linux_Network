#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  FILE *file;
  int c;

  // check the amount of input args is equal to 2
  if(argc != 2){
    printf("Usage: mycat filename\n");
    exit(1);
  }

  file = fopen(argv[1], "r");

  if(file){
    while ((c = getc(file)) != EOF)
        putchar(c);
    fclose(file);
  }



  return 0;
}
