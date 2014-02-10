#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;
struct sigaction sigact;
int n = 0;

void sigint_handler (int sig) {
	printf ("n = %d; Signal recu : %d\n", n, sig);
	if (n == 0) {
		sigact.sa_handler = SIG_IGN;
		sigaction (SIGINT, &sigact, NULL);    
		n = 1;
	}
	else {
		sigact.sa_handler = SIG_DFL;
		sigaction (SIGINT, &sigact, NULL);
	}
}

int main () {
	sigfillset (&sig);     //iclue tout les signaux dans sig
	sigdelset (&sig, SIGINT);    //supprime SIGINT de sig
	sigprocmask (SIG_SETMASK, &sig, NULL);   //ignore tout les signaux de sig donc tous sauf SIGINT 
	sigact.sa_mask = sig;      //ignore tout les signaux de sig donc tout sauf SIGINT (dons le cas d'envois d'un signal vers cette structure en utilisant sigaction 
	sigact.sa_flags = 0;
	sigact.sa_handler = sigint_handler;
	sigaction (SIGINT, &sigact, NULL);    //execute sigint_handler si SIGINT reçu
	printf ("AVANT sigsuspend\n");
	sigsuspend (&sig);                 //attend signal SIGINT , n=0 rien ne se passe
	printf ("APRES 1er sigsuspend\n"); //affiche
	sigsuspend (&sig);                //attend un signal SIGINT , n=1 programme termine
	printf ("APRES 2e sigsuspend\n");
	sigsuspend (&sig);
	printf ("APRES 3e sigsuspend\n");
	return 0;
}
