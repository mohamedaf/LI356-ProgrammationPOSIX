#define _POSIX_SOURCE 1

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>

void calcul_1 ( ) {
	int i;
	for (i = 0; i < 1E8; i++);
}

void calcul_2 () {
	int i;
	for (i = 0; i < 1E8; i++);
}

void sigusr1_handler(int sig){
}

void sigusr2_handler(int sig){
}

int main (int argc, char * argv[]) {
	int i=0;
	pid_t pid_fils[2],pere = getpid();
	sigset_t sig1,sig2,sig3;
	struct sigaction sigact1,sigact2;

	sigact1.sa_handler = sigusr1_handler;
	sigaction(SIGUSR1,&sigact1,NULL);

	sigact2.sa_handler = sigusr2_handler;
	sigaction(SIGUSR2,&sigact2,NULL);

	sigemptyset(&sig3);
	sigaddset(&sig3,SIGUSR1);
	sigaddset(&sig3,SIGUSR2);
	sigprocmask(SIG_SETMASK, &sig3, NULL);   //SIGUSR1 ET SIGUSR2 Ajoutés au mask

	sigfillset(&sig1);
	sigdelset(&sig1,SIGUSR1);           //pour le sigsuspend qui attend SIGUSR1

	sigfillset(&sig2);
	sigdelset(&sig2,SIGUSR2);           //pour le sigsuspend qui attend SIGUSR1

	while ((i<2) && ((pid_fils[i] = fork())!=0))
		i++;

	calcul_1 ();

	printf("fin calcul 1 dans processus %d \n",i);

	if(i == 0){
	  kill(pere,SIGUSR1);
	  sigsuspend(&sig1);
	}
	else if(i == 1){
	  kill(pere,SIGUSR2);
	  sigsuspend(&sig2);
	}
	else{
	  sigsuspend(&sig1);   //attente des signaux envoyes par les 2 fils
	  sigsuspend(&sig2);
	  kill(pid_fils[0],SIGUSR1);
	  kill(pid_fils[1],SIGUSR2);
	}

	printf("debut calcul 2 dans processus %d \n",i);

	calcul_2 ();
	printf ("fin processus %d \n",i);

	return EXIT_SUCCESS;
}
