#define _POSIX_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv){
	int i;	
	pid_t *pid = calloc(11,sizeof(pid_t));

	for(i=0; i<11; i++){
		pid[i] = getpid();

		if(fork() == 0){			
			if(i == 10){
				printf("Pid pere = %d\n",pid[0]);
				
				for(i=1; i<11; i++){
					printf("Pid fils %d = %d\n",i,pid[i]);	
				}
			}
		}
		else break;
	}

	for(i=0; i<10; i++){
		wait(NULL);
	}

	return 0;
}




