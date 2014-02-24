#define _POSIX_SOURCE 1

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

int rediriger_stdout(char* fichier){
  int f;

  if((f = open(fichier,O_WRONLY)) == -1){
    perror("Erreur ouverture fichier !!\n");
    exit(EXIT_FAILURE);
  }

  int old = dup(STDOUT_FILENO);    /*on recupere le descripteur ou la restauration de STDOUT*/

  dup2(f,STDOUT_FILENO);   /*on redirige la sortie standard vers f*/
  close(f);

  return old;
}

void restaurer_stdout(int old){
  dup2(old,STDOUT_FILENO);     /*on restaure stdout*/
}

int main(int argc,char** argv){
  int old;

  printf("avant la redirection \n");
  old = rediriger_stdout(argv[1]);
  printf("après la redirection \n");
  restaurer_stdout(old);
  printf("après avoir restauré stdout \n");
  return EXIT_SUCCESS;
}
