CC = gcc
SOURCE_DIR = src/
FLAGS = -Wall -O2
BIN = rsh
MAIN = main.c

${BIN} : ${SOURCE_DIR}
	${CC} -c ${SOURCE_DIR}*
	${CC} -c ${MAIN} *.o
	${CC} *.o -o ${BIN}

	rm ${SOURCE_DIR}*.gch
	rm *.o
	mv rsh /usr/bin
