# Este es un makefile temporal que luego reemplazaremos con Autoconf

campos.o: campos.cpp errores.o
	g++ -c campos.cpp -I.

errores.o: errores.cpp dfd-wrappers.o
	g++ -c errores.cpp -I.

dfd-wrappers.o:
	g++ -c dfd-wrappers.cpp -I.

.PHONY:clean
clean:
	rm -f errores.o dfd-wrappers.o campos.o
