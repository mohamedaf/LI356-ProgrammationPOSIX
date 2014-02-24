#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc,char** argv){

  struct stat buf;

  if(argc > 2){        /*au moins deux parametres*/

    if(stat(argv[2],&buf) == -1){
      perror("erreur stat\n");
      return EXIT_SUCCESS;
    }

    if(S_ISREG(buf.st_mode)){   /*on verifie que c'est bien un fichier*/

      if((!strcmp(argv[1],"e")) || (!strcmp(argv[1],"E"))){    /*si les deux chaines sont egales*/
        remove(argv[2]);
      }
      else if(argc > 3){       /*on verifie que le 3eme parametre optionnel existe pour pouvoir renommer ou changer les droits*/
        if((!strcmp(argv[1],"r")) || (!strcmp(argv[1],"R"))){     /*si les deux chaines sont egales*/
          rename(argv[2],argv[3]);
        }
        else if((!strcmp(argv[1],"c")) || (!strcmp(argv[1],"C"))){    /*si les deux chaines sont egales*/
          if((!strcmp(argv[3],"r")) || (!strcmp(argv[3],"R"))){       /*lecture*/
            chmod(argv[2],S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR);           /*pour le read only on concerve le droit d'ecriteur pour le proprio*/
          }
          else if((!strcmp(argv[3],"w")) || (!strcmp(argv[3],"W"))){  /*ecriture*/
            chmod(argv[2],S_IWUSR|S_IWGRP|S_IWOTH);
          }
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
