#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include <sys/param.h>


int main(){
    int rand1,rand2;
    int fd[2];// declare a two-d array, store file_descriptor of the pipe (two side)
              // fd[0] mease read side, fd[1] means write side
    pid_t pid;//child process的pid
    pipe(fd); //call system call (pipe) to create a pipe
    //use fork to create a child process
    //child process will wrtie data to pipe, and parent will read data from pipe
    //child process
    if((pid=fork())==0){
        srand(getpid());
        close(fd[READ_END]);//child won't use read size, so close it
        rand1=rand()%RANGE; //create random number
        write(fd[WRITE_END],&rand1,sizeof(rand1)); //write to pipe
        close(fd[WRITE_END]);//close write side when write over
        printf("%d has been created In Child Process \n",rand1);
        exit(1);
    }
    else if(pid>0){
        srand(getpid());
        close(fd[WRITE_END]);//parent won't use write size, so close it。
        rand2=rand()%RANGE;//create random number
        read(fd[READ_END],&rand1,sizeof(rand1));//read the data from pipe
        printf("%d has been created In Parent Process \n",rand2);
        wait();
        printf("Parent Process calulate sum is :%d \n",rand1+rand2);
        close(fd[READ_END]);//close read side
        exit(1);
    }
return 0;
}
