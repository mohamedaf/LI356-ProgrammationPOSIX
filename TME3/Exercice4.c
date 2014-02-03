#define _POSIX_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
  char* val[5];

  printf("Mon PID : %d\n",getpid());

  if(atoi(argv[1]) == 0) return EXIT_SUCCESS;
  else{
    sprintf(val,"%d",atoi(argv[1])-1);
    execl("./Exercice4","Exercice4",val,NULL);
  }

  return EXIT_SUCCESS;

}
