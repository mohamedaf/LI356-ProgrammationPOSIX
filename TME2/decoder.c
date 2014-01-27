/**** decoder.c ****/

#define _POSIX_SOURCE 1

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <decoder.h>


int magicsq[3][3] = {{4, -1, 8}, {-1, -1, -1}, {2, -1, 6}};            // a=b=c=d=e= -1


int check() {                          //si carre magique retourne 1 sinon retourne 0
	int i, j, sums[8];
	for (i = 0; i < 8; i++)
		sums[i] = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			sums[i] += magicsq[i][j];
			sums[i+3] += magicsq[j][i];
			if (i == j) sums[6] += magicsq[i][j];
			if ((i+j) == 2) sums[7] += magicsq[i][j];
		}
	}
	int result = 1;
	i = 1;
	while ((i < 8) && (result == 1)) {
		if (sums[0] != sums[i])
			result = 0;
		i++;
	}
	return result;
}


void display() {                       //Affiche le carre magique
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("  %d", magicsq[i][j]);
		printf("\n");
	}
	printf("\n");
}


int solve(int x) {
	int nbcarre = 0;
	magicsq[0][1] = x;
	int somme = magicsq[0][0] +  magicsq[0][1] +  magicsq[0][2];

	/* CODE A EXECUTER PAR CHAQUE PROCESSUS FILS */

	magicsq[1][0] = somme - magicsq[0][0] - magicsq[2][0];   //b = somme - 4 - 2
	magicsq[1][2] = somme - magicsq[0][2] - magicsq[2][2];   //d = somme - 8 - 6
	magicsq[2][1] = somme - magicsq[2][2] - magicsq[2][0];   //e = somme - 6 - 2
	magicsq[1][1] = somme - magicsq[1][0] - magicsq[1][2];   //c = somme - b - d

	if(check()){
	  display();
	  nbcarre++;
	}

	return nbcarre;
}

int main(int argc, char **argv)
{
  int i;
  pid_t pid_fils = -1;

  /*LANCEMENT ET ATTENTE DES PROCESSUS FILS */

    for(i=0; i<10; i++){
      if(pid_fils != 0){
        pid_fils = fork();
        if(pid_fils == 0) printf("a = %d, Nb Carre magiques affiche = %d\n",i,solve(i));
      }
    }

    for(i=0; i<10; i++) wait(NULL);

    return (0);
}




