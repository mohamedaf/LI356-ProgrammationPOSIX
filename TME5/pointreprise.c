#define _POSIX_SOURCE 1

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>
#include <setjmp.h>

#define NFIN 3

void nothingtodo_handler(int sig){
}

int main(int argc, char* argv[]){

  int r,a=1,cpt = 0;
  sigjmp_buf env;
  sigset_t sig,sig2,sig3;
  struct sigaction sigact;

  sigfillset(&sig);
  sigdelset(&sig,SIGINT);
  sigdelset(&sig,SIGALRM);   //pour sigsuspend

  sigemptyset(&sig2);
  sigaddset(&sig2,SIGINT);
  sigaddset(&sig2,SIGALRM);
  sigprocmask(SIG_SETMASK, &sig2, NULL);   //SIGINT ET SIGALRM Ajoutés au mask

  sigact.sa_handler = nothingtodo_handler;
  sigaction(SIGINT,&sigact,NULL);

  alarm(5);

  r = sigsetjmp(env,1);

  if(r == SIGINT && ++cpt == NFIN) a=0;

  while(a){
    sigsuspend(&sig);
    sigpending(&sig3);
    if(sigismember(&sig3,SIGALRM)) siglongjmp(env,SIGALRM);
    else siglongjmp(env,SIGINT);
  }

  return 0;
}
