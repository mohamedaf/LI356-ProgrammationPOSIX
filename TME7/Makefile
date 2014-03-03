CC=gcc -Wall
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src

all: ${BIN}/autotestfic


${OBJ}/ficindex_enonce.o: ${INC}/ficindex_enonce.h ${SRC}/ficindex_enonce.c
	${CC} -c -o ${OBJ}/ficindex_enonce.o ${SRC}/ficindex_enonce.c -I${INC}

${LIB}/libficindex.a:	${OBJ}/ficindex_enonce.o
	ar rs ${LIB}/libficindex.a ${OBJ}/ficindex_enonce.o

${BIN}/autotestfic: ${OBJ}/autotestfic.o ${LIB}/libficindex.a
	${CC} -o ${BIN}/autotestfic ${OBJ}/autotestfic.o -lficindex -L./${LIB}/

clean:
	rm -f ${OBJ}/ficindex_enonce.o ${BIN}/* ${LIB}/libficindex.a
