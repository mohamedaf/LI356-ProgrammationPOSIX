#define _POSIX_SOURCE 1

#include <converters.h>
#include <string.h>

int main(int argc, char **argv){
  conversion_message req;
  results_array rep;
  int i;
  int tubeDesc[2];

  strcpy(req.currency, argv[1]);
  req.amount = atof(argv[2]);

  if(pipe(tubeDesc) == -1){          /*creation tube*/
    perror("Probleme Pipe !\n");
    return EXIT_FAILURE;
  }

  if(fork() == 0){
    for(i = 0; i < NB_CONVERTERS; i++)
      handle_conversion_request(req, &rep[i], i);

    if(write(tubeDesc[1],&rep,sizeof(results_array)) == -1){   /*ecriture sur le tube*/
      perror("Probleme write fils!\n");
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  }
  else{
    if(read(tubeDesc[0],&rep,sizeof(results_array)) == -1){  /*lecture sur le tube, bloqué j'usqua écriture du fils*/
      perror("Probleme write fils!\n");
      return EXIT_FAILURE;
    }

    display_results(req, rep);
  }

  return EXIT_SUCCESS;
}
