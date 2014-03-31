#define _XOPEN_SOURCE 700

#include <myqueue.h>

struct mq_attr* tab_seg[_POSIX_OPEN_MAX];

mqd_t mq_open(const char* nom, int mq_flags, int mode){
  char fd = -1;
  struct mq_attr* at;
  sem_t *smutexw, *smutexr;

  /* Creation segment memoire partagee contenant la file*/
  if((fd = shm_open(nom,mq_flags,mode)) == -1){
    perror("shm_open fd1\n");
    return -1;
  }

  /* Allouer au segment une taille pour stocker la file */
  if((ftruncate(fd,sizeof(struct mq_attr))) == -1){
    perror("ftruncate\n");
    return -1;
  }

  /* mapper le segment en R/W partage */
  if((at = mmap(NULL,sizeof(struct mq_attr),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0)) == MAP_FAILED){
    perror("mmap\n");
    return -1;
  }

  tab_seg[(int)fd] = at;

  at -> mq_flags = mq_flags;
  at -> mq_maxmsg = 32;
  at -> mq_msgsize = (32*sizeof(char));
  at -> mq_curmsgs = 0;

  sprintf(at -> blocked_writers,"%s.w",nom);
  sprintf(at -> blocked_readers,"%s.r",nom);

  at -> nb_blocked_writers = 0;
  at -> nb_blocked_readers = 0;

  /* initialiser semaphore mutex */
  if((at -> smutex = sem_open(nom,O_RDWR|O_CREAT|O_EXCL,S_IRWXU,1)) == SEM_FAILED){
    perror("mq_open sem_open smutex\n");
    return -1;
  }

  /* initialiser semaphore pour bloquer l'ecriture */
  if((smutexw = sem_open(at -> blocked_writers,O_RDWR|O_CREAT|O_EXCL,S_IRWXU,32)) == SEM_FAILED){
    perror("mq_open sem_open smutexw\n");
    return -1;
  }

  /* initialiser semaphore pour bloquer la lecture */
  if((smutexr = sem_open(at -> blocked_readers,O_RDWR|O_CREAT|O_EXCL,S_IRWXU,0)) == SEM_FAILED){
    perror("mq_open sem_open smutexr\n");
    return -1;
  }

  return fd;
}

int mq_close(mqd_t mqdes){
  struct mq_attr* at;
  sem_t *smutexw, *smutexr;

  at = tab_seg[mqdes];

  /* ouverture semaphore pour bloquer l'ecriture */
  if((smutexw = sem_open(at -> blocked_writers,O_RDWR)) == SEM_FAILED){
    perror("mq_open sem_open smutexw\n");
    return -1;
  }

  /* ouverture semaphore pour bloquer la lecture */
  if((smutexr = sem_open(at -> blocked_readers,O_RDWR)) == SEM_FAILED){
    perror("mq_open sem_open smutexr\n");
    return -1;
  }

  sem_close(at -> smutex);
  sem_close(smutexw);
  sem_close(smutexr);

  if(munmap(at,sizeof(struct mq_attr)) == -1){
    perror("mq_close\n");
    return -1;
  }

  return 0;
}

int mq_unlink(const char* name){

  char *tmp = (char*) malloc(60*sizeof(char));

  if(shm_unlink(name) == -1){
    perror("mq_unlink\n");
    return -1;
  }

  sem_unlink(name);

  strcpy(tmp,name);
  strcat(tmp,".w");
  sem_unlink(tmp);

  free(tmp);
  tmp = (char*) malloc(60*sizeof(char));

  strcpy(tmp,name);
  strcat(tmp,".w");
  sem_unlink(tmp);

  free(tmp);

  return 0;
}

int mq_getattr(mqd_t mq_des, struct mq_attr* mqstat){
  struct mq_attr* at;

  at = tab_seg[mq_des];

  if(sem_wait(at -> smutex) == -1){   /* P */
    perror("sem_wait mq_getattr\n");
    return -1;
  }

  /* copier le contenu du segment memoire partagee dans mqstat */
  memmove(mqstat,at,sizeof(struct mq_attr));

  if(sem_post(at -> smutex) == -1){   /* V */
    perror("sem_post mq_getattr\n");
    return -1;
  }

  return 0;
}

int mq_setattr(mqd_t mq_des, struct mq_attr *mqstat, struct mq_attr *omqstat){

  if(mqstat == NULL){
    perror("mq_setattr mqstat NULL\n");
    return -1;
  }

  struct mq_attr* at;

  at = tab_seg[mq_des];

  if(omqstat != NULL){
    mq_getattr(mq_des,omqstat);
  }

  if(sem_wait(at -> smutex) == -1){   /* P */
    perror("sem_wait mq_setattr\n");
    return -1;
  }

  at->mq_flags |= mqstat->mq_flags & O_NONBLOCK;

  if(sem_post(at -> smutex) == -1){   /* V */
    perror("sem_post mq_setattr\n");
    return -1;
  }

  return 0;
}

int mq_send(mqd_t mqdes, const char* msg_ptr, size_t msg_len, unsigned msg_prio){
  struct mq_attr* at;
  msg *m = NULL;
  int i=0;
  sem_t *smutexw, *smutexr;

  at = tab_seg[mqdes];

  if(sem_wait(at -> smutex) == -1){   /* P */
    perror("sem_wait mq_send\n");
    return -1;
  }

  if(msg_len > (at -> mq_maxmsg)){
    perror("mq_send message trop volumineux\n");
    sem_post(at -> smutex);  /* je libere avant de quitter */
    return -1;
  }

  if(at -> mq_flags & O_NONBLOCK && (at -> mq_curmsgs == at -> mq_maxmsg)){
    sem_post(at -> smutex);  /* je libere avant de quitter */
    return -1;
  }

  /* ouverture semaphore pour bloquer l'ecriture */
  if((smutexw = sem_open(at -> blocked_writers,O_RDWR)) == SEM_FAILED){
    perror("mq_open sem_open smutexw\n");
    sem_post(at -> smutex);  /* je libere avant de quitter */
    return -1;
  }

  /* ouverture semaphore pour bloquer la lecture */
  if((smutexr = sem_open(at -> blocked_readers,O_RDWR)) == SEM_FAILED){
    perror("mq_open sem_open smutexr\n");
    sem_post(at -> smutex);  /* je libere avant de quitter */
    return -1;
  }

  if(sem_wait(smutexw) == -1){   /* P */
    perror("sem_wait mq_send\n");
    sem_post(at -> smutex);  /* je libere avant de quitter */
    return -1;
  }

  if(sem_post(smutexr) == -1){   /* V */
    perror("sem_post mq_send\n");
    sem_post(at -> smutex);  /* je libere avant de quitter */
    sem_post(smutexw);       /* je libere avant de quitter */
    return -1;
  }

  m = (msg*) malloc(sizeof(msg));
  strcpy(m -> data,msg_ptr);
  m -> prio = msg_prio;
  m -> length = msg_len;

  if(at -> mq_curmsgs == 0)                      /* si aucun message n'est dans la file */
    memmove(&(at -> queue[0]),m,sizeof(msg));

  else{

    while(i < (at -> mq_curmsgs)){
      if(at -> queue[i].prio < msg_prio)      /* passer les cases +prio ou +ancienne si meme prio */
	break;
      i++;
    }

    if(i < (at -> mq_curmsgs))           /* decaler les elements moins prioritaires */
      memmove(&(at -> queue[i+1]),&(at -> queue[i]),((at -> mq_curmsgs)-(i+1))*sizeof(msg));

    memmove(&(at -> queue[i]),m,sizeof(msg));  /* ajouter l'element en question */
  }

  if(sem_post(at -> smutex) == -1){   /* V */
    perror("sem_post mq_send\n");
    sem_post(smutexw);       /* je libere avant de quitter */
    sem_wait(smutexr);       /* je libere avant de quitter */
    return -1;
  }

  (at -> mq_curmsgs)++;
  free(m);

  return 0;
}

ssize_t mq_receive(mqd_t mqdes, char* msg_ptr, size_t msg_len, unsigned *msg_prio){
  struct mq_attr* at;
  sem_t *smutexw, *smutexr;

  at = tab_seg[mqdes];

  //printf("aaaaaaaaaaa\n");

  if(sem_wait(at -> smutex) == -1){   /* P */   /* probleme interblocage ici */
    perror("sem_wait mq_receive\n");
    return -1;
  }

  if(msg_len < at -> mq_msgsize){
    perror("msg_len trop petit ! mq_receive\n");
    return -1;
  }

  /* initialiser semaphore pour bloquer l'ecriture */
  if((smutexw = sem_open(at -> blocked_writers,O_RDWR)) == SEM_FAILED){
    perror("mq_open sem_open smutexw\n");
    sem_post(at -> smutex);  /* je libere avant de quitter */
    return -1;
  }

  /* initialiser semaphore pour bloquer la lecture */
  if((smutexr = sem_open(at -> blocked_readers,O_RDWR)) == SEM_FAILED){
    perror("mq_open sem_open smutexr\n");
    sem_post(at -> smutex);  /* je libere avant de quitter */
    return -1;
  }

  if(sem_wait(smutexr) == -1){   /* P */
    perror("sem_wait mq_receive\n");
    sem_post(at -> smutex);  /* je libere avant de quitter */
    return -1;
  }

  if(sem_post(smutexw) == -1){   /* V */
    perror("sem_post mq_receive\n");
    sem_post(at -> smutex);  /* je libere avant de quitter */
    sem_post(smutexr);       /* je libere avant de quitter */
    return -1;
  }

  strcpy(msg_ptr,at -> queue[0].data);

  if(msg_prio != NULL)
    *msg_prio = at -> queue[0].prio;

  /* supprimer le message en question de la file */

  (at -> mq_curmsgs)--;

  if(at -> mq_curmsgs == 1)
    memmove(at -> queue,&(at -> queue[1]),sizeof(msg));
  else
    memmove(at -> queue,&(at -> queue[1]),(at -> mq_curmsgs)*sizeof(msg));

  if(sem_post(at -> smutex) == -1){   /* V */
    perror("sem_post mq_receive\n");
    sem_post(smutexr);       /* je libere avant de quitter */
    sem_wait(smutexw);       /* je libere avant de quitter */
    return -1;
  }

  return 0;
}

mqd_t mq_notify(mqd_t mqdes, const struct sigevent *notification){
  struct mq_attr* at;

  at = tab_seg[mqdes];

  if(sem_wait(at -> smutex) == -1){   /* V */
    perror("sem_wait mq_notify\n");
    return -1;
  }

  if(notification != NULL){
    at -> subscriber = getpid();
    at -> notification_sig = notification -> sigev_notify;

    if(at -> notification_sig == SIGEV_THREAD){
      notification -> sigev_notify_function(notification -> sigev_value);
    }
    else if(at -> notification_sig == SIGEV_SIGNAL){
      kill(at -> subscriber,notification -> sigev_signo);
    }
  }
  else{
    if(at -> subscriber == getpid())
      at -> subscriber = -1;
  }

  if(sem_post(at -> smutex) == -1){   /* V */
    perror("sem_post mq_notify\n");
    return -1;
  }

  return 0;
}
