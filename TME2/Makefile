C_FLAGS = -g -Wall

all: testprog testExo3 testdecoder testconverters

Prog1.o : Prog1.c
	gcc $(C_FLAGS) -c Prog1.c

Exo3.o : Exo3.c
	gcc $(C_FLAGS) -c Exo3.c

decoder.o : decoder.c
	gcc $(C_FLAGS) -c decoder.c

converters.o : converters.c
	gcc $(C_FLAGS) -c converters.c

testprog    : Prog1.o
	gcc $(C_FLAGS) -o testprog Prog1.o

testExo3	: Exo3.o
	gcc $(C_FLAGS) -o testExo3 Exo3.o

testdecoder : decoder.o
	gcc $(C_FLAGS) -o testdecoder decoder.o

testconverters : converters.o
	gcc $(C_FLAGS) -o testconverters converters.o

clean   :
	rm -f *.o testprog testExo3 testdecoder testconverters
