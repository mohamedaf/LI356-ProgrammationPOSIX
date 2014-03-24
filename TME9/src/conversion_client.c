#define _POSIX_SOURCE 1

#include <converters.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>

typedef struct echange_msg {
  conversion_message rq;
  results_array rp;
} conversion;

int main(int argc, char **argv){
  conversion *sp;
  int fd;
  sem_t *smutexclient,*smutexserver;

  /* Ouverture segment memoire partagee echange client serveur ouverture en lecture et ecriture*/
  if((fd = shm_open(argv[1],O_RDWR,S_IRWXU)) == -1){
    perror("shm_open\n");
    return EXIT_FAILURE;
  }

  /* mapper le segment en R/W partage */
  if((sp = mmap(NULL,sizeof(conversion),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0)) == MAP_FAILED){
    perror("mmap\n");
    return EXIT_FAILURE;
  }

   /* ouverture semaphore pour synchro client => serveur  initialise a 1 */
  if((smutexclient = sem_open("semclient",O_RDWR,S_IRWXU,1)) == SEM_FAILED){
    perror("sem_open client");
    return EXIT_FAILURE;
  }

   /* ouverture semaphore pour synchro serveur => client  initialise a 0 */
  if((smutexserver = sem_open("semserver",O_RDWR,S_IRWXU,0)) == SEM_FAILED){
    perror("sem_open server");
    return EXIT_FAILURE;
  }


  if(sem_wait(smutexclient) == -1){   /* P */
    perror("sem_wait fils\n");
    return EXIT_FAILURE;
  }

  strcpy(sp -> rq.currency, argv[2]);
  sp -> rq.amount = atof(argv[3]);

  if(sem_post(smutexserver) == -1){   /* V */
    perror("sem_wait fils\n");
    return EXIT_FAILURE;
  }

  //recpu res du serveur

  if(sem_wait(smutexclient) == -1){   /* P */
    perror("sem_wait fils\n");
    return EXIT_FAILURE;
  }

  display_results(sp -> rq, sp -> rp);    //la seamphore sert a attender que le serveur termine la conversion

  if(sem_post(smutexclient) == -1){   /* V */
    perror("sem_wait fils\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
