CXX = g++
CXXFLAGS = -Wall -I. # -ggdb3 -DDEBUG
PROGRAM = freedfd
SOURCES = main.cpp token.cpp errores.cpp operador.cpp tokeniza.cpp variables.cpp tabla-simbolos.cpp evaluacion.cpp funcion.cpp dfd-wrappers.cpp entorno-ejecucion.cpp arreglos.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
HEADERS := $(filter-out main.h,$(patsubst %.cpp,%.h,$(SOURCES))) 

.SUFFIXES:      .o .cpp
.cpp.o : $(HEADERS)
	$(CXX) $(CXXFLAGS) -c  -o $@ $<
all:	$(PROGRAM)
$(PROGRAM): $(OBJECTS) $(HEADERS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) -lreadline

clean:
	rm -f $(OBJECTS)
	rm -f $(PROGRAM)

