#define _XOPEN_SOURCE 777

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

char *sp;
sem_t *smutexf,*smutexp;
pid_t pere;

void fin_handler(int sig){
  /* detacher le segment */
    munmap(sp,100*sizeof(char));

  /* Fermer les semaphores */
    sem_close(smutexf);
    sem_close(smutexp);

  if(pere == getpid()){
    /* detruire le segment */
    shm_unlink("messagepart");

    /* Detruire les semaphores */
    sem_unlink("semfils");
    sem_unlink("sempere");
  }

    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv){
  int fd,i;
  sigset_t sig;
  struct sigaction sigact;

  sigfillset(&sig);
  sigdelset(&sig,SIGINT);
  sigprocmask(SIG_SETMASK,&sig,NULL);

  sigact.sa_handler = fin_handler;
  sigaction(SIGINT,&sigact,NULL);    /* modifier comportement SIGINT */

  pere = getpid();

  /* Creation segment memoire partagee ouverture en lecture et ecriture*/
  if((fd = shm_open("messagepart",O_RDWR|O_CREAT|O_EXCL,S_IRWXU)) == -1){
    perror("shm_open\n");
    return EXIT_FAILURE;
  }

  /* Allouer au segment une taille pour stocker la chaine */
  if((ftruncate(fd,100*sizeof(char))) == -1){
    perror("ftruncate\n");
    return EXIT_FAILURE;
  }

  /* mapper le segment en R/W partage */
  if((sp = (char*) mmap(NULL,100*sizeof(char),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0)) == MAP_FAILED){
    perror("mmap\n");
    return EXIT_FAILURE;
  }

  /* creation semaphore initialise a 1 */
  if((smutexf = sem_open("semfils",O_RDWR|O_CREAT|O_EXCL,S_IRWXU,1)) == SEM_FAILED){
    perror("sem_open");
    return EXIT_FAILURE;
  }

  /* creation semaphore initialise a 1 */
  if((smutexp = sem_open("sempere",O_RDWR|O_CREAT|O_EXCL,S_IRWXU,0)) == SEM_FAILED){
    perror("sem_open");
    return EXIT_FAILURE;
  }

  if(fork() == 0){
    while(1){
      if(sem_wait(smutexf) == -1){   /* P */
        perror("sem_wait fils\n");
        return EXIT_FAILURE;
      }

      scanf("%s",sp);

      if(sem_post(smutexp) == -1){   /* V */
        perror("sem_post pere\n");
        return EXIT_FAILURE;
      }
    }
  }
  else{
    while(1){
      if(sem_wait(smutexp) == -1){   /* P */
        perror("sem_wait pere\n");
        return EXIT_FAILURE;
      }

      for(i=0; i<100; i++){
        if(sp[i]>='a' && sp[i]<='z')
          sp[i] = sp[i]-32;
      }

      printf("%s\n",sp);

      if(sem_post(smutexf) == -1){   /* V */
        perror("sem_post fils\n");
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}
