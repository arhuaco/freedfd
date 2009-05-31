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

  //FIXME: rename Actual borrandolo
  const char *GetActualIdentificador ()  {   return Identificador;  }

  unsigned int *GetActualVectorIndices ()  { return VectorIndices; }

  ListaExpresiones & GetActualLista ()  { return Lista; }

  void EvaluaActualesIndices ();

  void Vacear ();

  void AlmacenaVector (const char *);
};

#endif
