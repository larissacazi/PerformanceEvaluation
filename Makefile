#Makefile
IDIR=./
CC=gcc
CFLAGS=-I. -lm
RM=rm -f

DEPS=smplx.h declaracoes.h randpar.h
OBJ=projeto.o smplx.o randpar.o

PROJECT_NAME=projeto
EXE_NAME=projeto

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJECT_NAME): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm *.o *.exe *.out