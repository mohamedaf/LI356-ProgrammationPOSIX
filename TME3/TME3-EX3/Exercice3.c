#define _POSIX_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char* argv[]){
	int i,statut,exec = 1;

	for(i=1; i<argc; i++){
		if(fork() == 0){
		  execlp("gcc","gcc","-c","-Wall","-I.",argv[i],NULL);
		}
	}

	for(i=1; i<argc; i++){
		wait(&statut);

		if(!WIFEXITED(statut)){
			exec = 0;
		}
	}

	if(exec){
      system("gcc -o afficher -Wall -I. *.o");
      system("rm Exercice3.o");
  }
	else printf("Un ou plusieurs fichiers n'ont pas pu etre compile !\n");

	return EXIT_SUCCESS;
}
