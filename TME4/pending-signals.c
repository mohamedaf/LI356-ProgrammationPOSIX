#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
  sigset_t sig;

  sigemptyset(&sig);
  sigaddset(&sig,SIGINT);
  sigaddset(&sig,SIGQUIT);
  sigprocmask(SIG_SETMASK,&sig,NULL);
  sleep(5);

  sigpending(&sig);
  if(sigismember(&sig,SIGINT)) printf("\nSIGINT est un signal pendant !!\n");
  if(sigismember(&sig,SIGQUIT)) printf("\nSIGQUIT est un signal pendant !!\n");
  
  return 0;
}


//pas d'affichage car au moment ou on demasque l'un des deux signaux va etre pris en compte et dans les 2 cas interrompre le processus et terminer le programme.
