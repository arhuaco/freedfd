#ifndef __LISTA__VECTORES__
#define __LISTA__VECTORES__

/* Esto se puede mejorar, no creo que uno necesite
 * hacer listas */

#include <lista-expresiones.h>

class ListaVectores
{
  struct Nodo
  {
    Nodo ()
    {
      ContadorNodoListaVectores++;
    }
     ~Nodo ()
    {
      ContadorNodoListaVectores--;
    }
    char *Identificador;
    ListaExpresiones Lista;
    unsigned int *VectorIndices;
    Nodo *Sig;
  } *Inicio, *Iterador, *Ultimo;

  int NItems;

  void AlmacenaVectorInternal (char *);

public:

  ListaVectores ()
  {
    Inicio = 0;
    NItems = 0;
  }
  int GetNItems ()
  {
    return NItems;
  }

  char *GetActualIdentificador ()
  {
    return Iterador->Identificador;
  }

  unsigned int *GetActualVectorIndices ()
  {
    return Iterador->VectorIndices;
  }

  ListaExpresiones & GetActualLista ()
  {
    return Iterador->Lista;
  }

  ListaExpresiones & GetNuevaLista ()
  {
    return Ultimo->Lista;
  }

  void EvaluaActualesIndices ();

  void SetNuevoIdentificador (char *Identificador);

  void Reset ()
  {
    Iterador = Inicio;
  }

  void Itera ()
  {
    Iterador = Iterador->Sig;
  }

  void Insertar ();

  void Vacear ();

  void AlmacenaVector (const char *);
};

#endif
