CC=gcc -Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

all: ${BIN}/single_converter ${BIN}/conversion_server ${BIN}/conversion_client ${BIN}/multi_converter

${OBJ}/%.o : ${SRC}/%.c
	$(CC) -c -o $@ $< -I${INC}

${BIN}/%: ${OBJ}/%.o ${OBJ}/converters.o
	${CC} -o $@ $^

clean:
	rm -f ${OBJ}/* ${BIN}/*