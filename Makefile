CC = gcc
SOURCE_DIR = src/
FLAGS = -Wall -O2
BIN = rsh
MAIN = main.c

${BIN} : ${SOURCE_DIR}
	${CC} -c -g ${SOURCE_DIR}*
	${CC} -g ${MAIN} *.o

	rm ${SOURCE_DIR}*.gch
	rm *.o
#	mv rsh /usr/bin