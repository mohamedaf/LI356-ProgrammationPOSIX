C_FLAGS = -g -Wall

all: init-sig.1 init-sig.2 init-sig.3 init-sig.4 init-sig.5

init-sig.1.o : init-sig.1.c
	gcc $(C_FLAGS) -c init-sig.1.c

init-sig.2.o : init-sig.2.c
	gcc $(C_FLAGS) -c init-sig.2.c
  
init-sig.3.o : init-sig.3.c
	gcc $(C_FLAGS) -c init-sig.3.c
  
init-sig.4.o : init-sig.4.c
	gcc $(C_FLAGS) -c init-sig.4.c
  
init-sig.5.o : init-sig.5.c
	gcc $(C_FLAGS) -c init-sig.5.c

init-sig.1    : init-sig.1.o
	gcc $(C_FLAGS) -o init-sig.1 init-sig.1.o

init-sig.2    : init-sig.2.o
	gcc $(C_FLAGS) -o init-sig.2 init-sig.2.o
  
init-sig.3    : init-sig.3.o
	gcc $(C_FLAGS) -o init-sig.3 init-sig.3.o
  
init-sig.4    : init-sig.4.o
	gcc $(C_FLAGS) -o init-sig.4 init-sig.4.o

init-sig.5    : init-sig.5.o
	gcc $(C_FLAGS) -o init-sig.5 init-sig.5.o

clean   :
	rm -f *.o init-sig.1 init-sig.2 init-sig.3 init-sig.4 init-sig.5