# Este es un makefile temporal que luego reemplazaremos con Autoconf


CFLAGS=-Wall -I.

all: main
	g++ main.cpp token.o errores.o operador.o tokeniza.o dfd-wrappers.o ${CFLAGS} -g -lreadline

main: token.o errores.o operador.o tokeniza.o
	echo Done

token.o: token.cpp errores.o 
	g++ -c token.cpp -I. 

errores.o: errores.cpp dfd-wrappers.o
	g++ -c errores.cpp -I.

operador.o : operador.cpp operador.h
	g++ -c operador.cpp -I.

tokeniza.o:
	g++ -c tokeniza.cpp -I.

dfd-wrappers.o:
	g++ -c dfd-wrappers.cpp -I.

.PHONY:clean
clean:
	rm -f *.o a.out
