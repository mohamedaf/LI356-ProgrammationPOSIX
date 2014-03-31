CC=gcc -Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

all: ${BIN}/test1_myqueue ${BIN}/test2_myqueue


${OBJ}/myqueue.o: ${INC}/myqueue.h ${SRC}/myqueue.c
	${CC} -c -o ${OBJ}/myqueue.o ${SRC}/myqueue.c -I${INC}

${LIB}/libmyqueue.a:	${OBJ}/myqueue.o
	ar rs ${LIB}/libmyqueue.a ${OBJ}/myqueue.o

${OBJ}/test1_myqueue.o: ${SRC}/test1_myqueue.c ${LIB}/libmyqueue.a
	${CC} -c -o ${OBJ}/test1_myqueue.o ${SRC}/test1_myqueue.c -lmyqueue -L./${LIB}/ -I${INC}

${OBJ}/test2_myqueue.o: ${SRC}/test2_myqueue.c ${LIB}/libmyqueue.a
	${CC} -c -o ${OBJ}/test2_myqueue.o ${SRC}/test2_myqueue.c -lmyqueue -L./${LIB}/ -I${INC}

${BIN}/test1_myqueue: ${OBJ}/test1_myqueue.o ${LIB}/libmyqueue.a
	${CC} -o ${BIN}/test1_myqueue ${OBJ}/test1_myqueue.o -lmyqueue -L./${LIB}/ -lrt

${BIN}/test2_myqueue: ${OBJ}/test2_myqueue.o ${LIB}/libmyqueue.a
	${CC} -o ${BIN}/test2_myqueue ${OBJ}/test2_myqueue.o -lmyqueue -L./${LIB}/ -lrt

clean:
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*
