C_FLAGS = -g -Wall

all: Exercice1 frok-exec Exo3 Exercice4 spy-incomplete

Exercice1.o : Exercice1.c
	gcc $(C_FLAGS) -c Exercice1.c

fork-exec.o : fork-exec.c
	gcc $(C_FLAGS) -c fork-exec.c

Exercice4.o : Exercice4.c
	gcc $(C_FLAGS) -c Exercice4.c

Exercice3.o : TME3-EX3/Exercice3.c
	gcc $(C_FLAGS) -c TME3-EX3/Exercice3.c
    
spy-incomplete.o : spy-incomplete.c
	gcc $(C_FLAGS) -c spy-incomplete.c

Exercice1    : Exercice1.o
	gcc $(C_FLAGS) -o Exercice1 Exercice1.o

frok-exec : fork-exec.o
	gcc $(C_FLAGS) -o fork-exec fork-exec.o

Exercice4 : Exercice4.o
	gcc $(C_FLAGS) -o Exercice4 Exercice4.o

Exo3 : Exercice3.o
	gcc $(C_FLAGS) -o Exo3 Exercice3.o	
    
spy-incomplete : spy-incomplete.o
	gcc $(C_FLAGS) -o spy-incomplete spy-incomplete.o

clean   :
	rm -f *.o Exercice1 fork-exec Exo3 Exercice4 spy-incomplete
