#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define MAX_INTR 5

sigset_t sig;
struct sigaction sigact;
int glob_cpt = 0;
int cpt_sig[30];

void compteur (int sig) {
  glob_cpt++;
  cpt_sig[sig-1]++;
}

int main(int argc, char* argv[]){
  int i;
  
  for(i=0; i<30; i++){
    cpt_sig[i] = 0;
  }

  sigact.sa_handler = compteur;
  
  for(i=1; i<31; i++){
    sigaction(i,&sigact, NULL);
  }
  
  while (1) {
		pause();
    if(cpt_sig[1] == MAX_INTR) break;
	}
  
  printf("Valeur compteur global : %d\n",glob_cpt);
  
  for(i=0; i<30; i++){
    printf("Valeur compteur signal %d : %d\n",i+1,cpt_sig[i]);
  }
  
  return 0;
}