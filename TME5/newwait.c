#define _POSIX_SOURCE 1

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>

sigset_t sig;

void sigact_handler(int sig){
  printf("Le fils est mort !!\n");
}

void mywait(){
  sigfillset(&sig);
  sigdelset(&sig,SIGCHLD);

  printf("sigsuspend debut\n");
  sigsuspend(&sig);                   //attend SIGCHLD
  printf("sigsuspend fini\n");
}

int main(int argc, char* argv[]){
  struct sigaction sigact;

  sigemptyset(&sig);
  sigaddset(&sig,SIGCHLD);
  sigprocmask(SIG_SETMASK, &sig, NULL);   //ajouter SIGCHLD au masque

  sigact.sa_handler = sigact_handler;
  sigaction(SIGCHLD,&sigact,NULL);    //changement comportement SIGCHLD

  if(fork() == 0){
    return EXIT_SUCCESS;
  }
  else{
    sleep(5);
    mywait();
    return EXIT_SUCCESS;
  }
}
