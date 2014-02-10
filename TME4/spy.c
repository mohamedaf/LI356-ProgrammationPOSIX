/**** spy.c ****/
#define _POSIX_SOURCE 1

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFSZ 1024
#define WDSZ 24
#define NBUF 3


int cut_down(char *buf, char **cmd) {
	char* s = strtok(buf, " \n");
	int i = 0;
	while (s != NULL) {
		cmd[i] = (char*) malloc(sizeof(char) * WDSZ);
		strcpy(cmd[i], s);
		s = strtok(NULL, " \n");
		i++;
	}
	cmd[i] = NULL;
	return i;
}


int main(int argc, char **argv)
{
  int or = 1, command_size, i, cpt=1;
	char buf[BUFSZ];
	char *command[WDSZ];
  char *tabcom[NBUF];
  struct sigaction sigact;
  pid_t fils[NBUF];
  
  sigact.sa_handler = SIG_IGN;
  
  for(i=0; i<32; i++){
    sigaction (i,&sigact,NULL);
  }

  while (or > 0)
  {
    for (i = 0; i < BUFSZ ; i++)
      buf[i] = '\0';
    if ((or = read(STDIN_FILENO, buf, BUFSZ)) == -1)
      perror("read");
    else if (or > 0) {
      command_size = cut_down(buf, (char**)command);
        
      tabcom[cpt-1] = *command;
      printf("cpt = %d\n",cpt);
      
      if(cpt == NBUF){
        for(i=0; i<NBUF; i++) kill(fils[i],SIGINT);
        cpt = 1;
        printf("kill\n");
        for(i=0; i<NBUF; i++) waitpid(fils[i],NULL,0);  //probleme ici
      }
      else cpt++;
    
      if(command_size > 1){
        if((fils[cpt-1] = fork()) == 0){
          pause();
          execvp(command[0],command);
        }
      }
      else{
        if((fils[cpt-1] = fork()) == 0){
          pause();
          execlp(command[0],command[0],NULL);
        }
      }
    }
  }
	
	return 0;
}
