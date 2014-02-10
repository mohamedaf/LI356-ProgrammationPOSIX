#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;

int main () {
  sigfillset (&sig);    //iclue tout les signaux dans sig
  sigdelset (&sig, SIGINT);       //suprime SIGINT de sig
  sigprocmask (SIG_SETMASK, &sig, NULL);   //ignore tout les signaux de sig donc tous sauf SIGINT
  sigsuspend (&sig);      //bloque et attend SIGINT pour etre debloque
  printf ("1er signal\n");
  sigsuspend (&sig);
  printf ("2e signal\n");
  return 0;
}


//aucun affichage car reception d'un signal SIGINT apres avoir ete bloque en ligne 13
