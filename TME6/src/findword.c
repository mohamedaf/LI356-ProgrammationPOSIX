#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char** argv){

  if(argc > 2){           /*au moins 2 parametres*/
    struct stat fic;
    int nbocc=0,cpt=0,i,f,fin=1;   /*cpt compte chaque mot pour donner les bonnes valeurs à pos*/
    char c=' ';
    char mot[30];

    if(stat(argv[1],&fic) == -1){
      perror("Erreur stat !!\n");
      return EXIT_FAILURE;
    }
    else{
      if(S_ISREG(fic.st_mode)){
	if((f = open(argv[1],O_RDONLY)) == -1){
	  perror("Erreur ouverture fichier !!\n");
	  return EXIT_FAILURE;
	}

	while(fin){
	  i=0;

	  do{
	    fin = read(f,&c,sizeof(char));
	    if(c != ' ' && c != '\n'){
	      mot[i] = c;
	      i++;
	    }
	  }while(c != ' ' &&  c != '\n' && fin != 0);       /*tant que je ne lit pas un espace, un retour à la ligne et que je ne sois pas en fin de fichier*/

	  cpt++;         /*on l'incremente pour chaque mot lu*/
	  if(!strcmp(argv[2],mot)){    /*si c'est une occurence du mots en parametre*/
	    printf("Position : %d\n",cpt);
	    nbocc++;
	  }

	}

	close(f);
	printf("Nombre occurences trouvees %d\n",nbocc);
      }
      else{
	perror("Le parametre n'est pas un fichier regulier !!\n");
	return EXIT_FAILURE;
      }
    }
  }
  else{
    perror("Nombre de parametres incorrect !!\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
