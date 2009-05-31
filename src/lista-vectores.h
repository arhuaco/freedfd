#ifndef __LISTA__VECTORES__
#define __LISTA__VECTORES__

/* Esto se puede mejorar, no creo que uno necesite
 * hacer listas */

#include <lista-expresiones.h>

class ListaVectores
{
  char *Identificador;
  ListaExpresiones Lista;
  unsigned int *VectorIndices;

  void AlmacenaVectorInternal (char *);

public:

  ListaVectores ()
  {
    ContadorNodoListaVectores++;
    Identificador = 0;
    VectorIndices = 0;
  }
   ~ListaVectores ()
  {
    ContadorNodoListaVectores--;
    Vacear();
  }

  const char *GetIdentificador ()  {   return Identificador;  }

  unsigned int *GetVectorIndices ()  { return VectorIndices; }

  ListaExpresiones & GetLista ()  { return Lista; }

  void EvaluaIndices ();

  void Vacear ();

  void AlmacenaVector (const char *);
};

#endif
