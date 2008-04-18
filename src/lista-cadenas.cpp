#include <lista-cadenas.h>

/* Creo que esta clase va a desaparecer */

void
ListaCadenas::InsertarEn (int Pos, char *Str)
{
  Nodo *Nuevo = new Nodo;
  Nuevo->Dato = Str;
  if (Pos == 0)
    {

      Nuevo->Sig = Inicio;
      Inicio = Nuevo;
      if (NItems == 0)
        Ultimo = Nuevo;
    }
  else
    {
      Nodo *Ant = Inicio;
      for (int i = 1; i < Pos; ++i)
        Ant = Ant->Sig;
      Nuevo->Sig = Ant->Sig;
      Ant->Sig = Nuevo;
      if (Ultimo == Ant)
        Ultimo = Nuevo;
    }
  ++NItems;
}

const char *
ListaCadenas::Itera ()
{
  const char *Retorno = Iterador->Dato;
  Iterador = Iterador->Sig;
  return Retorno;
}

void
ListaCadenas::Eliminar (int Pos)
{
  Nodo *Ant = 0, *Actual = Inicio;
  for (int i = 0; i < Pos; ++i)
    {
      Ant = Actual;
      Actual = Actual->Sig;
    }
  if (Ant == 0)
    Inicio = Actual->Sig;
  else
    Ant->Sig = Actual->Sig;
  if (Ultimo == Actual)
    Ultimo = Ant;
  delete[]Actual->Dato;
  delete Actual;
  --NItems;
}


void
ListaCadenas::Insertar (const char *Str)
{
  Nodo *Nuevo = new Nodo;
  Nuevo->Dato = Str;
  Nuevo->Sig = 0;
  if (!Inicio)
    Inicio = Nuevo;
  else
    Ultimo->Sig = Nuevo;
  Ultimo = Nuevo;
  Iterador = Inicio;
  ++NItems;
}

void
ListaCadenas::Vacear ()
{
  while (Inicio)
    {
      Nodo *Aux = Inicio;
      Inicio = Inicio->Sig;
      delete[]Aux->Dato;
      delete Aux;
    }
  NItems = 0;
}
