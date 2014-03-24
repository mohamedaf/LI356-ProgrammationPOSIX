#define _XOPEN_SOURCE 777

#include <converters.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>
#include <errno.h>

typedef struct echange_msg {
  conversion_message rq;
  results_array rp;
} conversion;

conversion *sp;
results_array *lp;
sem_t *smutexf,*smutexp,*smutexclient,*smutexserver;
pid_t pere;

void fin_handler(int sig){
  /* detacher les segments */
    munmap(sp,sizeof(conversion));
    munmap(lp,sizeof(results_array));

  /* Fermer les semaphores */
    sem_close(smutexf);
    sem_close(smutexp);
    sem_close(smutexclient);
    sem_close(smutexserver);

  if(pere == getpid()){
    /* detruire les segments */
    shm_unlink("echange");
    shm_unlink("conv");

    /* Detruire les semaphores */
    sem_unlink("semfils");
    sem_unlink("sempere");
    sem_unlink("semserver");
    sem_unlink("semclient");
  }

    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv){
  int i,fd1,fd2;
  sigset_t sig;
  struct sigaction sigact;

  sigfillset(&sig);
  sigdelset(&sig,SIGINT);
  sigprocmask(SIG_SETMASK, &sig, NULL);

  sigact.sa_handler = fin_handler;
  sigaction(SIGINT,&sigact,NULL);     /* changer comportement SIGINT terminer programme */

  /* Creation segment memoire partagee echange client serveur ouverture en lecture et ecriture*/
  if((fd1 = shm_open(argv[1],O_RDWR|O_CREAT|O_EXCL,S_IRWXU)) == -1){
    perror("shm_open fd1\n");
    return EXIT_FAILURE;
  }

  /* Allouer au segment une taille pour stocker la chaine */
  if((ftruncate(fd1,sizeof(conversion))) == -1){
    perror("ftruncate\n");
    return EXIT_FAILURE;
  }

  /* mapper le segment en R/W partage */
  if((sp = mmap(NULL,sizeof(conversion),PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0)) == MAP_FAILED){
    perror("mmap\n");
    return EXIT_FAILURE;
  }

  /* Creation segment memoire partagee echange pere fils ouverture en lecture et ecriture*/
  if((fd2 = shm_open("conv",O_RDWR|O_CREAT|O_EXCL,S_IRWXU)) == -1){
    perror("shm_open fd2\n");
    return EXIT_FAILURE;
  }

  /* Allouer au segment une taille pour stocker la chaine */
  if((ftruncate(fd2,sizeof(results_array))) == -1){
    perror("ftruncate\n");
    return EXIT_FAILURE;
  }

  /* mapper le segment en R/W partage */
  if((lp = mmap(NULL,sizeof(results_array),PROT_READ|PROT_WRITE,MAP_SHARED,fd2,0)) == MAP_FAILED){
    perror("mmap\n");
    return EXIT_FAILURE;
  }

   /* creation semaphore pour synchro client => serveur  initialise a 1 */
  if((smutexclient = sem_open("semclient",O_RDWR|O_CREAT|O_EXCL,S_IRWXU,1)) == SEM_FAILED){
    perror("sem_open clent");
    return EXIT_FAILURE;
  }

   /* creation semaphore pour synchro serveur => client  initialise a 0 */
  if((smutexserver = sem_open("semserver",O_RDWR|O_CREAT|O_EXCL,S_IRWXU,0)) == SEM_FAILED){
    perror("sem_open server");
    return EXIT_FAILURE;
  }

  /* creation semaphore fils pour synchro pere fils initialise a 1 */
  if((smutexf = sem_open("semfils",O_RDWR|O_CREAT|O_EXCL,S_IRWXU,1)) == SEM_FAILED){
    perror("sem_open fils");
    return EXIT_FAILURE;
  }

  /* creation semaphore pere pour syncho pere fils initialise a 0 */
  if((smutexp = sem_open("sempere",O_RDWR|O_CREAT|O_EXCL,S_IRWXU,0)) == SEM_FAILED){
    perror("sem_open pere");
    return EXIT_FAILURE;
  }

  pere = getpid();
  
  while(1){

    if(sem_wait(smutexserver) == -1){   /* V */
      perror("sem_wait fils\n");
      return EXIT_FAILURE;
    }

    for(i = 0; i < NB_CONVERTERS; i++){
      if(fork() == 0){
	/* le fils envois au pere le resultat de la conversion */

	if(sem_wait(smutexf) == -1){   /* P */
	  perror("sem_wait fils\n");
	  return EXIT_FAILURE;
	}

	handle_conversion_request(sp -> rq, lp[i], i);

	if(sem_post(smutexp) == -1){   /* V */
	  perror("sem_wait fils\n");
	  return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
      }
      else{
	/* le pere lit la conversion du fils */

	if(sem_wait(smutexp) == -1){    /* P */
	  perror("sem_wait fils\n");
	  return EXIT_FAILURE;
	}

	sp -> rp[i] = *lp[i];

	if(sem_post(smutexf) == -1){    /* V */
	  perror("sem_wait fils\n");
	  return EXIT_FAILURE;
	}
      }
    }

    if(sem_post(smutexclient) == -1){   /* V */
      perror("sem_wait fils\n");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
