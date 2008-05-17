# Warning. If you modify a .h you have to execute make clean && make

# TODO: Remove ccache if you are not using it.
CXX = ccache g++
#CXX = i586-mingw32msvc-g++ 

CXXFLAGS = -Wall -I. -ggdb3 -DDEBUG
PROGRAM = freedfd

EXTRA_LIBS=  -lm

# Use this to debug malloc and free, install electic-fence in Debian or Ubuntu
# EXTRA_LIBS += -lefence

#Better line edition, define and install libreadline5-dev in Debian or Ubuntu
EXTRA_LIBS += -lreadline
CXXFLAGS += -DHAVE_READLINE


SOURCES = main.cpp token.cpp errores.cpp operador.cpp \
          tokeniza.cpp variables.cpp tabla-simbolos.cpp \
          evaluacion.cpp funcion.cpp dfd-wrappers.cpp \
          entorno-ejecucion.cpp arreglos.cpp lista-expresiones.cpp \
          lista-vectores.cpp lista-cadenas.cpp objetos-ejecucion.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
HEADERS := $(filter-out main.h,$(patsubst %.cpp,%.h,$(SOURCES))) 

.SUFFIXES:      .o .cpp
.cpp.o : $(HEADERS)
	$(CXX) $(CXXFLAGS) -c  -o $@ $<
all:	$(PROGRAM)
$(PROGRAM): $(OBJECTS) $(HEADERS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) ${EXTRA_LIBS}

clean:
	rm -f $(OBJECTS)
	rm -f $(PROGRAM)

