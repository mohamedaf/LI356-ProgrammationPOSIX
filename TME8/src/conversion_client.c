#define _POSIX_SOURCE 1

#include <converters.h>
#include <string.h>

int main(int argc, char **argv){
  conversion_message req;
  results_array rep;
  int ind_write, ind_read;

  if((ind_write = open(argv[1],O_WRONLY)) == -1){  //ouverture ecriture tube req
    perror("Probleme open ecriture tube requete\n");
    return EXIT_FAILURE;
  }

  strcpy(req.currency, argv[3]);
  req.amount = atof(argv[4]);

  if(write(ind_write,&req,sizeof(conversion_message)) == -1){  //ecriture requete
    perror("Probleme write requete !\n");
    return EXIT_FAILURE;
  }

  if((ind_read = open(argv[2],O_RDONLY)) == -1){     //ouverture lecture tuberep
    perror("Probleme open lecture tube requete\n");
    return EXIT_FAILURE;
  }

  if(read(ind_read,&rep,sizeof(results_array)) == -1){   //lecture reponse
    perror("Probleme read reponse !\n");
    return EXIT_FAILURE;
  }

  display_results(req, rep);

  close(ind_read);
  close(ind_write);

  return EXIT_SUCCESS;
}
