#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>

int main(){
  struct utsname uts;

  if(uname(&uts) == -1){
    perror("hostinfo.c:main:uname");
    exit(1);
  }

  printf("hostname: %s\n", uts.nodename);
  printf("%s\n", uts.sysname);
  printf("hostid: %ld\n", gethostid());
}
