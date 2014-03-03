#define _POSIX_SOURCE 1

#include <ficindex_enonce.h>

int indx[_POSIX_OPEN_MAX];

int indopen(const char *path, int flags, int ind, mode_t mode){   /*ind taille donnees stockees*/
  int f,ind2,r;

  if((f = open(path,flags,mode)) != -1){
    if((r = read(f,&ind2,sizeof(int))) > 0)
      indx[f] = ind2;
    else if(r == 0){
      write(f,&ind,sizeof(int));
      indx[f] = ind;
    }
    else{
      perror("erreur lecture dans indopen !!\n");
      return -1;
    }

    return f;
  }

  return -1;
}


int indclose(int fd){
  return close(fd);
}


ssize_t indread(int fd, void *buf, size_t nbytes){
  int n;

  if(nbytes >= indx[fd]){
    if((n = read(fd,buf,nbytes)) != -1){
      return n;            /*retourne nb caracteres lus, 0 si fin de fichier (rien n'est lu)*/
    }
    else return -1;
  }
  else{
    perror("nbytes < a la taille d'index dans indread !!\n");
    return -1;
  }
}


ssize_t indwrite(int fd, const void *buf, size_t nbytes){
  int n;

  if(nbytes >= indx[fd]){
    if((n = write(fd,buf,nbytes)) != -1){
      return n;            /*retourne nb octets ecrits*/
    }
    else return -1;
  }
  else{
    perror("nbytes < a la taille d'index dans indwrite !!\n");
    return -1;
  }
}


off_t indlseek(int fd, off_t offset, int whence){

  if(whence == SEEK_SET)
    return lseek(fd,4+(indx[fd]*offset),whence);
  else
    return lseek(fd,indx[fd]*offset,whence);
}


void *indsearch(int fd, int (*cmp)(void *, void *), void *key){
  void* buf = malloc(sizeof(indx[fd]));
  ssize_t ret;

  while((ret=indread(fd,buf,indx[fd]))){   /*renvoie 0 si fin de fichier*/
    if(ret == -1)
      return NULL;
    if(cmp(buf,key))
      return buf;
  }

  return NULL;
}


int indxchg(char *oldfic, char *newfic, unsigned int newind){
  struct stat fic;
  int f1,f2;
  ssize_t ret;
  void *buf = NULL;

  if(stat(oldfic,&fic) == -1){
    perror("Fichier old inexistant !!\n");
    return -1;
  }

  f1 = indopen(oldfic, O_RDONLY, -1, fic.st_mode);
  f2 = indopen(newfic, O_CREAT | O_TRUNC | O_RDWR, newind, fic.st_mode | 0444);

  if(indx[f1] > newind)
    buf = malloc(indx[f1]);
  else
    buf = malloc(newind);

  while((ret=indread(f1,buf,indx[f1]))){   /*renvoie 0 si fin de fichier*/
    if(ret == -1){
      perror("Erreur lecture dans indxchg !!\n");
      return -1;
    }


    ret=indwrite(f2,buf,newind);

    if(ret == -1){
      perror("Erreur ecriture dans indxchg !!\n");
      return -1;
    }
  }

  indclose(f1);
  indclose(f2);

  return 0;
}
