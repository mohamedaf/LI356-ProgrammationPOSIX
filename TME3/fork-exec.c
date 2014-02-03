#define _POSIX_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>


int main(int argc, char **argv) {

	struct timeval tv;
	
	printf("Resultat de la commande 'date'\n");
	
	//execlp("date", "date", "+%s", NULL);          //affiche nombre de secondes depuis ../../1970
	system("date +%s");

	printf("Comparaison avec la fonction 'gettimeofday'\n");
	
	gettimeofday(&tv, NULL);
	printf("%d\n", (int)tv.tv_sec);
	
	return 0;
}

