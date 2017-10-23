#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

int main () {

  int pipefds[2];
  int stat;
  pid_t pid;
  char *argv[] = {"ls", "-l", NULL};
  char *output[1024];

  // create pipe

  if(pipe(pipefds) == -1){
      fprintf(stderr, "Error creating pipe\n");
  }

  switch (pid = fork()) {
    case -1:
      printf("Fork Fail.\n");
      exit(1);

    // child process write result to pipe
    case 0:
      close(pipefds[0]);
      // close(1);

      dup2(pipefds[1], 1);
      stat = execvp("ls", argv);

      if(stat < 0) {
        printf("execvp fail.\n");
        exit(1);
      }

      exit(1);
      break;

    // parent process read result to pipe
    default:
      wait(&pid);
      close(pipefds[1]);

      char buf[1];

      while(read(pipefds[0], buf, 1) > 0){
          write(1, buf, 1); // 1 -> stdout
      }

      close(pipefds[0]);
  }

  return 0;
}
