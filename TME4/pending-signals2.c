#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
  sigset_t sig;
  struct sigaction sigact;
  
  sigemptyset(&sig);
  sigaddset(&sig,SIGINT);
  sigaddset(&sig,SIGQUIT);
  sigact.sa_mask = sig;
  sigact.sa_flags = 0;
  sigact.sa_handler = SIG_IGN;
  sigaction (SIGINT, &sigact, NULL);
  sigaction (SIGQUIT, &sigact, NULL);
  sleep(5);
  
  sigpending(&sig);
  if(sigismember(&sig,SIGINT)) printf("\nSIGINT est un signal pendant !!\n");
  if(sigismember(&sig,SIGQUIT)) printf("\nSIGQUIT est un signal pendant !!\n");
  
  return 0;
}

//cette fois ci on n'a pas de signals pendants car les signaux sont ignorés à cause du handler donc on ne peut afficher les signaux envoyés non pris en compte.