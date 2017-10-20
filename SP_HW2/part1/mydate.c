#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

int main(){
  struct tm *tm;
  time_t t;
  char date[200];

  t = time(NULL);
  tm = localtime(&t);

  strftime(date, 200, "%b %d(%a), %G %I:%M %p", tm);
  printf("%s\n", date);
}
