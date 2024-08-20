CC = gcc
SOURCE_DIR = src/
FLAGS = -Wall -O2
BIN = tsh
MAIN = main.c

${BIN} : ${SOURCE_DIR}
	${CC} -c ${SOURCE_DIR}*
	${CC} ${MAIN} *.o -o ${BIN}

	rm ${SOURCE_DIR}*.gch
	rm *.o
	mv tsh /usr/bin
