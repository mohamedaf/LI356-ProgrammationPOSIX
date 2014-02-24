#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc,char** argv){

  if(argc > 3){                        /*si les trois parametres existent*/
    struct stat f1,f2;
    int i,d1,d2,d3,fin1=1,fin2=1;
    char c=' ';
    char mot[30];

    if((stat(argv[1],&f1) == -1) || (stat(argv[2],&f2) == -1)){
      perror("Erreur stat\n");
      return EXIT_FAILURE;
    }
    else{
      if(S_ISREG(f1.st_mode) && S_ISREG(f2.st_mode)){      /*verifier que ce sont bien des fichiers reguliers*/
        d1 = open(argv[1],O_RDONLY);
        d2 = open(argv[2],O_RDONLY);                       /*ouverture des 2 fichiers en lecture*/
        d3 = open(argv[3],O_CREAT|O_WRONLY,S_IRWXU);       /*ouverture en lecture ecriture creation s'il n'existe pas*/

        while(fin1 && fin2){
          
          i=0;
          
          do{
            fin1 = read(d1,&c,sizeof(char));
            if(c != ' ' && c != '\n'){
              mot[i] = c;
              i++;
            }
          }while(c != ' ' &&  c != '\n' && fin1 != 0);   /*tant que je ne lit pas un espace, un retour à la ligne et que je ne sois pas en fin de fichier*/

          mot[i] = ' ';
          i++;

          write(d3,mot,i*sizeof(char));      /*ecrire mot dans d3*/

          i=0;
	  
          do{
            fin2 = read(d2,&c,sizeof(char));
            if(c != ' ' && c != '\n'){
              mot[i] = c;
              i++;
            }
          }while(c != ' ' && c != '\n' && fin2 != 0);        /*tant que je ne lit pas un espace*/

          if(fin2){                            /*pour ne pas avoir un espace a la fin du fihier*/
            mot[i] = ' ';
            i++;
          }

          write(d3,mot,i*sizeof(char));        /*ecrire mot dans d3*/
        }

        close(d1);
        close(d2);
        close(d3);
      }
    }
  }

  return EXIT_SUCCESS;
}
