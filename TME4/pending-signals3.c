#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int cpt = 0;

void aff_int(int sig){
  printf("SIGINT reçu numéro %d\n",++cpt);
}

int main(int argc, char* argv[]){
  sigset_t sig;
  struct sigaction sigact_Quit,sigact_Int;
  
  sigemptyset(&sig);
  sigaddset(&sig,SIGINT);
  sigaddset(&sig,SIGQUIT);
  
  sigact_Quit.sa_flags = 0;
  sigact_Quit.sa_handler = SIG_IGN;
  
  sigaction (SIGQUIT, &sigact_Quit, NULL);
  
  sigact_Int.sa_flags = 0;
  sigact_Int.sa_handler = aff_int;
  
  sigaction (SIGINT, &sigact_Int, NULL);
  
  sleep(5);
  
  sigpending(&sig);
  if(sigismember(&sig,SIGINT)) printf("\nSIGINT est un signal pendant !!\n");
  if(sigismember(&sig,SIGQUIT)) printf("\nSIGQUIT est un signal pendant !!\n");
  
  return 0;
}

//val max affiché 1 car pendant le sleep executé en mode system le bit de SIGINT se transfome en 1 après avoir reçu 1 signal SIGINT ensuite même s'il en reçoit d'autres ça ne change pas le bit donc pendant le sleep c'est à dire tant qu'on est pas encore revenu en mode utilisateur on ne peut savoir si on a reçu plus d'un signal SIGINT

//redefinitions comportements du signal SIGINT et on ignore le signal SIGQUIT donc aucun signal pendant