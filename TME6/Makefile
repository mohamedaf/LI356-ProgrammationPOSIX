CC=gcc -Wall -ansi
BIN=bin
OBJ=obj
SRC=src

all: ${BIN}/liste-rep ${BIN}/setfilestats ${BIN}/filefusion ${BIN}/findword ${BIN}/redirection

# regle generale pour les OBJ :
$(OBJ)/%.o : $(SRC)/%.c
	$(CC) -c -o $@ $<

# regle generale pour les BIN
$(BIN)/% : $(OBJ)/%.o
	$(CC) -o $@ $<

clean :
	rm -f ${OBJ}/* ${BIN}/*
