C_FLAGS = -g -Wall

all: voodoo pending-signals pending-signals2 pending-signals3 accountant spy

voodoo.o : voodoo.c
	gcc $(C_FLAGS) -c voodoo.c

pending-signals.o : pending-signals.c
	gcc $(C_FLAGS) -c pending-signals.c
  
pending-signals2.o : pending-signals2.c
	gcc $(C_FLAGS) -c pending-signals2.c
  
pending-signals3.o : pending-signals3.c
	gcc $(C_FLAGS) -c pending-signals3.c
  
accountant.o : accountant.c
	gcc $(C_FLAGS) -c accountant.c
  
spy.o : spy.c
	gcc $(C_FLAGS) -c spy.c

voodoo    : voodoo.o
	gcc $(C_FLAGS) -o voodoo voodoo.o

pending-signals    : pending-signals.o
	gcc $(C_FLAGS) -o pending-signals pending-signals.o
  
pending-signals2    : pending-signals2.o
	gcc $(C_FLAGS) -o pending-signals2 pending-signals2.o
  
pending-signals3    : pending-signals3.o
	gcc $(C_FLAGS) -o pending-signals3 pending-signals3.o
  
accountant    : accountant.o
	gcc $(C_FLAGS) -o accountant accountant.o
  
spy    : spy.o
	gcc $(C_FLAGS) -o spy spy.o

clean   :
	rm -f *.o voodoo pending-signals pending-signals2 pending-signals3 accountant spy
