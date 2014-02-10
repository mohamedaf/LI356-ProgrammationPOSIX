#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

sigset_t sig;
struct sigaction sigact_int;
struct sigaction sigact_alrm;
int n_int = 0;
int n_alrm = 0;

void sigalrm_handler (int sig) {
	printf ("reception de SIGALRM\n");
	alarm (5);
}

void sigint_handler (int sig) {
	printf ("reception de SIGINT\n");
}


int main () {
	sigfillset (&sig);
	sigdelset (&sig, SIGINT);
	sigdelset (&sig, SIGALRM);     //supression SIGINT et SIGALRM de sig
	sigprocmask (SIG_SETMASK, &sig, NULL);   //ignore tout sauf SIGINT et SIGALRM
	sigact_int.sa_handler = sigint_handler;
	sigaction (SIGINT, &sigact_int, NULL);   //affichage fonction sigint_handler si SIGINT reçu
	sigact_alrm.sa_handler = sigalrm_handler;
	sigaction (SIGALRM, &sigact_alrm, NULL);   //affichage fonction sigint_handler si SIGALRM reçu
	alarm (5);
	while (1) {
		sigsuspend (&sig);
	}
	return 0;
}

//on donne un nouveau comportement aux 2 commandes definies dans les fonctions, le handler contient donc les nouveaux comportement
