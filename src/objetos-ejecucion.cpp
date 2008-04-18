#include <dfd.h>
#include <lista-expresiones.h>
#include <lista-vectores.h>
#include <errores.h>
#include <token.h>
#include <tokeniza.h>
#include <evaluacion.h>
#include <entorno-ejecucion.h>

#include <string.h>


/* Parece que esta clase se podría evitar usando STL. La usamos? */

class ListaCadenas
{

  struct Nodo
  {
    char *Dato;
    Nodo *Sig;
  } *Inicio, *Iterador, *Ultimo;

  int NItems;

public:

    ListaCadenas ()
  {
    Inicio = 0;
    NItems = 0;
  }
  int GetNItems ()
  {
    return NItems;
  }
  void Reset ()
  {
    Iterador = Inicio;
  }
  char *Itera ();
  void Insertar (char *);
  void InsertarEn (int, char *);
  void Eliminar (int);
  void Vacear ();
};

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

char *
ListaCadenas::Itera ()
{
  char *Retorno = Iterador->Dato;
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
ListaCadenas::Insertar (char *Str)
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

class OE_Asignacion
{
  ListaVectores Destinos;
  ListaExpresiones Fuentes;
  ListaCadenas CadenasDestino, CadenasFuente;
public:
  OE_Asignacion ();
  void Preprocesar ();
  void Despreprocesar ();
  void Ejecutar();

  ListaCadenas & GetCadenasDestino ()
  {
    return CadenasDestino;
  }
  ListaCadenas & GetCadenasFuente ()
  {
    return CadenasFuente;
  }
};

OE_Asignacion::OE_Asignacion ()
{
  CadenasDestino.Reset ();
  CadenasFuente.Reset ();
  int Items = CadenasDestino.GetNItems ();
  char *AuxCd1, *AuxCf1, *AuxCd2, *AuxCf2;
  for (int i = 0; i < Items; ++i)
    {
      AuxCd1 = CadenasDestino.Itera ();
      AuxCf1 = CadenasFuente.Itera ();
      AuxCd2 = new char[strlen (AuxCd1) + 1];
      AuxCf2 = new char[strlen (AuxCf1) + 1];
      strcpy (AuxCd2, AuxCd1);
      strcpy (AuxCf2, AuxCf1);
      CadenasDestino.Insertar (AuxCd2);
      CadenasFuente.Insertar (AuxCf2);
    }
    void Ejecutar (); /* esta era virtual, definida en una clase base*/
}

void
OE_Asignacion::Preprocesar ()
{
  int n = CadenasDestino.GetNItems ();
  if (n == 0)
    {
      Buzon.Error (ILEGAL_COMA_O_CADENA_VACIA);
      return;
    }
  CadenasDestino.Reset ();
  CadenasFuente.Reset ();

  for (int i = 0; i < n; ++i)
    {
      Destinos.AlmacenaVector (CadenasDestino.Itera ());
      if (Buzon.GetHuboError ())
        return;
      Token *Temp = GetPostfijo (CadenasFuente.Itera ());
      if (Buzon.GetHuboError ())
        return;
      if (!Temp)
        {
          Buzon.Error (ILEGAL_COMA_O_CADENA_VACIA);
          return;
        }
      Fuentes.Insertar (Temp);
    }

  return;
}

void
OE_Asignacion::Despreprocesar ()
{
  Destinos.Vacear ();
  Fuentes.Vacear ();
}

void
OE_Asignacion::Ejecutar ()
{
  int n = Destinos.GetNItems ();
  Destinos.Reset ();
  Fuentes.Reset ();
  for (int i = 0; i < n; ++i)
    {
      Destinos.EvaluaActualesIndices ();
      if (Buzon.GetHuboError ())
        return;
      char *Id = Destinos.GetActualIdentificador ();
      unsigned int *Vec = Destinos.GetActualVectorIndices ();
      int NInd = Destinos.GetActualLista ().GetNItems ();
      Token *t = EvaluaPostfijo (Fuentes.Itera ());
      if (Buzon.GetHuboError ())
        return;


      PilaDeTablas.AsignarValor (Id, t, Vec, NInd);
      delete t;
      if (Buzon.GetHuboError ())
        return;
      Destinos.Itera ();
    }
}
