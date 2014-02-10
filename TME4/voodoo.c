#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main () {

	printf("PERE: %d\n", getpid());
	if (fork() != 0) {
		pause();
		pause();
		wait(0);
	} else
		printf("FILS: %d\n", getpid());

	printf("%d> Fin\n", getpid());
	return 0;
}

//Le programme s'arrete SIGKILL provoque la terminaison du programme apres le premier pause()
