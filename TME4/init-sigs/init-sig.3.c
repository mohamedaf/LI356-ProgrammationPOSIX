#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;
struct sigaction sigact;

int main () {
  sigemptyset (&sig);        //vide tous les signaux dans set
  sigaddset (&sig, SIGINT);  //ajoute SIGINT a set
  sigact.sa_mask = sig;      //SIGINT ajoute au mask (le mask bloque un signal qui devient pendant)
  sigact.sa_flags = 0;       //pas de flags particulier precise
  sigact.sa_handler = SIG_IGN;     //ignore le signal (contrairement au mask le signal n'est  pas pendant comme si on ne l'avais pas reçu)
  sigaction (SIGALRM, &sigact, NULL);    //execute le handler de sigact si SIGALRM recu
  alarm (5);               //SIGALRM reçu execution du handler tous les signaux sont ignores
  sigsuspend (&sig);
  printf ("APRES sigsuspend\n");
  return 0;
}

//pour arreter le programme autre signal de terminaison que SIGINT, SIGALARM ne fait pas effet à cause du handler de sigact et sigsuspend attend un autre signal que SIGINT.
