#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>

int main(int argc,char** argv){

  DIR *dir;
  struct dirent *cour;

  if(argc > 1){
    if((dir = opendir(argv[1]))){      /*ouvre le repertoire en lecture*/
      while((cour = readdir(dir))){    /*lit l'entree courante du repertoire assicie a dir*/
        printf("%s\n",cour->d_name);
      }
    }
    else mkdir(argv[1],S_IRWXU|S_IRWXG|S_IRWXO);  /*le repertoire n'existe pas*/
  }
  else{
    dir = opendir(".");
    while((cour = readdir(dir))){    /*lit l'entree courante du repertoire assicie a dir*/
      printf("%s\n",cour->d_name);
    }
  }

  closedir(dir);

  return EXIT_SUCCESS;
}
