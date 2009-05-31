#ifndef __VAR__VECTORES__
#define __VAR__VECTORES__

/* Esto se puede mejorar, no creo que uno necesite
 * hacer listas */

#include <lista-expresiones.h>

class VariableVector
{
  char *Identificador;
  ListaExpresiones Lista;
  unsigned int *VectorIndices;

  void AlmacenaVectorInternal (char *);

public:

  VariableVector ()
  {
    ContadorVariableVector++;
    Identificador = 0;
    VectorIndices = 0;
  }
   ~VariableVector ()
  {
    ContadorVariableVector--;
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
