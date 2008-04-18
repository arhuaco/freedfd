# Este es un makefile temporal que luego reemplazaremos con Autoconf


CFLAGS=-Wall -I.

all: main
	g++ main.cpp token.o errores.o operador.o tokeniza.o \
	variables.o dfd-wrappers.o ${CFLAGS} -g -lreadline

main: token.o errores.o operador.o tokeniza.o variables.o
	echo Done

token.o: token.cpp errores.o 
	g++ -c token.cpp  ${CFLAGS}

errores.o: errores.cpp dfd-wrappers.o
	g++ -c errores.cpp  ${CFLAGS}

operador.o : operador.cpp operador.h
	g++ -c operador.cpp  ${CFLAGS}

tokeniza.o:
	g++ -c tokeniza.cpp  ${CFLAGS}

dfd-wrappers.o:
	g++ -c dfd-wrappers.cpp  ${CFLAGS}

variables.o: variables.cpp variables.h
	g++ -c variables.cpp  ${CFLAGS}

.PHONY:clean
clean:
	rm -f *.o a.out
