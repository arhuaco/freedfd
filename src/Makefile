# Este es un makefile temporal que luego reemplazaremos con Autoconf


CFLAGS=-Wall -I.

all: main
	g++ main.cpp campos.o errores.o operador.o tokeniza.o ${CFLAGS}

main: campos.o errores.o operador.o tokeniza.o
	echo Done

campos.o: campos.cpp errores.o 
	g++ -c campos.cpp -I. 

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
	rm -f errores.o dfd-wrappers.o campos.o operador.o tokeniza.o
