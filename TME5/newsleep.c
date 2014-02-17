#define _POSIX_SOURCE 1

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>


void sigalrm_handler(int sig){
  printf("SIGALRM reçu !!\n");
}

void sigother_handler(int sig){
  printf("Un autre signal autre que SIGALRM est reçu !!\n");
}

int mysleep(int sec){
  struct sigaction sigact,sigact2;
  int res,i;

  sigact.sa_handler = sigalrm_handler;
  sigaction(SIGALRM,&sigact,NULL);

  sigact2.sa_handler = sigother_handler;

  for(i=1; i<32; i++){
    if(i != 14) sigaction(i,&sigact2,NULL);     //tout les signaux sauf SIGALRM
  }

  alarm(sec);

  pause();

  res = alarm(0);

  sigact.sa_handler = SIG_DFL;
  sigact2.sa_handler = SIG_DFL;

  if(res == sec) return 0;
  else return res;
}

int main(int argc, char* argv[]){
  int res,att = 5;

  res = mysleep(att);

  if(res == 0) res = att;

  printf("temps attente demande %dsec temps reel attendue %dsec\n",att,att-res);

  return 0;
}
