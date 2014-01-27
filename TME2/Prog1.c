#define _POSIX_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void N_FILS_Iter(int n){
    int i;
    pid_t pid_fils = -1;
    
    for(i=0; i<n; i++){
        if(pid_fils != 0) pid_fils = fork();           //afin de faire en sorte que ça soit seulement le processus pere qui cree des fils
        
        if(pid_fils == -1){
            perror("fork");
            exit(1);
        }
        else if(pid_fils == 0){
			printf("Je suis un processus fils cree dans la fonction N_FILS_Iter. PID = %d\n",getpid());
			exit(0);
		}
    }
    
    for(i=0; i<10; i++){    //attendre tous les fils
        wait(NULL);
    }

    /*printf("%d\n",pid_fils);
     waitpid(pid_fils,NULL,0);     //attendre le dernier fils crée uniquement*/
}


void N_FILS_Rec(int n){
    pid_t pid_fils;
    
    if(n > 0){
        pid_fils = fork();
        
        if(pid_fils == -1){
            perror("fork");
            exit(1);
        }
        else if(pid_fils == 0){
            printf("Je suis un processus fils cree dans la fonction N_FILS_Rec. PID = %d\n",getpid());
        }
        else{
          /*if(n == 1){
           printf("%d\n",pid_fils);
           waitpid(pid_fils,NULL,0);     //attendre le dernier fils crée uniquement
           }*/

          N_FILS_Rec(n-1);
          wait(NULL);                    //attendre tout les fils
        }
    }
}

int main(){
    int iter = -1;
    
    while(iter != 0 && iter != 1){
        printf("Tapez 1 pour la méthode Iterative ou 0 pour la méthode Recursive\n");
        scanf("%d",&iter);
        printf("%d\n",iter);
    }
    
    if(iter == 1) N_FILS_Iter(10);
    else N_FILS_Rec(10);
    
    //pid_fils_wait = wait(NULL);    //attendre un fils quelconque
    
    return EXIT_SUCCESS;
}
