CC=gcc -Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

all: ${BIN}/capitalize ${BIN}/conversion_client ${BIN}/multi_converter

${OBJ}/%.o : ${SRC}/%.c
	${CC} -c -o $@ $< -I${INC}

${BIN}/capitalize: ${OBJ}/capitalize.o
	${CC} -o ${BIN}/capitalize ${OBJ}/capitalize.o 

${BIN}/conversion_client: ${OBJ}/conversion_client.o ${OBJ}/converters.o
	${CC} -o ${BIN}/conversion_client ${OBJ}/conversion_client.o ${OBJ}/converters.o 

${BIN}/multi_converter: ${OBJ}/multi_converter.o ${OBJ}/converters.o
	${CC} -o ${BIN}/multi_converter ${OBJ}/multi_converter.o ${OBJ}/converters.o 

clean:
	rm -f ${BIN}/* ${OBJ}/*

