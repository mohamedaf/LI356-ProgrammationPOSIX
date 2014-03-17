#define _POSIX_SOURCE 1

#include <converters.h>
#include <string.h>
#include <signal.h>

int ind_write, ind_read, a=1;
char *ficreq, *ficrep;

void fin_handler(int sig){
  close(ind_read);
  close(ind_write);

  if(unlink(ficreq)<0){
    perror("unlink\n");
    exit(EXIT_FAILURE);
  }

  if(unlink(ficrep)<0){
    perror("unlink\n");
    exit(EXIT_FAILURE);
  }

  printf("SIGINT recu fin programme\n");

  a=0;   /* fin boucle */
}

int main(int argc, char **argv){
  conversion_message req;
  results_array rep;
  int i;
  sigset_t sig;
  struct sigaction sigact;

  ficreq = argv[1];
  ficrep = argv[2];

  sigfillset(&sig);
  sigdelset(&sig,SIGINT);
  sigprocmask(SIG_SETMASK, &sig, NULL);

  sigact.sa_handler = fin_handler;
  sigaction(SIGINT,&sigact,NULL);     /* changer comportement SIGINT terminer programme */

  if(mkfifo(ficreq,S_IRUSR|S_IWUSR) == -1 || mkfifo(ficrep,S_IRUSR|S_IWUSR) == -1){
    perror("Probleme MKFIFO !\n");
    return EXIT_FAILURE;
  }

  while(a){
    if((ind_read = open(ficreq,O_RDONLY)) == -1){    //ouverture lecture bloquante tubereq
      perror("Probleme open lecture tube requete\n");
      return EXIT_FAILURE;
    }

    if(read(ind_read,&req,sizeof(conversion_message)) == -1){  //lecture requete
      perror("Probleme read requete !\n");
      return EXIT_FAILURE;
    }

    for(i = 0; i < NB_CONVERTERS; i++)
      handle_conversion_request(req, &rep[i], i);

    if((ind_write = open(ficrep,O_WRONLY)) == -1){     //ouverture ecriture tuberep
      perror("Probleme open ecriture tube requete\n");
      return EXIT_FAILURE;
    }


    if(write(ind_write,&rep,sizeof(results_array)) == -1){  //ecriture reponse
      perror("Probleme write reponse!\n");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
