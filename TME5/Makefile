C_FLAGS = -g -Wall

all: newwait newsleep synchro3 pointreprise

newwait.o : newwait.c
	gcc $(C_FLAGS) -c newwait.c

newsleep.o : newsleep.c
	gcc $(C_FLAGS) -c newsleep.c

synchro3.o : synchro3.c
	gcc $(C_FLAGS) -c synchro3.c

pointreprise.o : pointreprise.c
	gcc $(C_FLAGS) -c pointreprise.c

newwait    : newwait.o
	gcc $(C_FLAGS) -o newwait newwait.o

newsleep    : newsleep.o
	gcc $(C_FLAGS) -o newsleep newsleep.o

synchro3    : synchro3.o
	gcc $(C_FLAGS) -o synchro3 synchro3.o

pointreprise    : pointreprise.o
	gcc $(C_FLAGS) -o pointreprise pointreprise.o

clean   :
	rm -f *.o newwait newsleep synchro3 pointreprise
