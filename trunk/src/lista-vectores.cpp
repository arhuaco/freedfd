#include <dfd.h>
#include <lista-vectores.h>
#include <token.h>
#include <tokeniza.h>
#include <errores.h>
#include <lista-expresiones.h>
#include <evaluacion.h>
#include <dfd-wrappers.h>

#include <string.h>
#include <math.h>


/* Este código no parece tan necesario. Uno debería meter la
 * asignación en la evaluación de expresiones.
 */

void
VariableVector::Vacear ()
{
   if (Identificador) {
        delete[] Identificador;
        Identificador = NULL;
   }
    if (VectorIndices) {
        delete[] VectorIndices;
        VectorIndices = NULL;
   }
   Lista.Vacear ();
}


void
VariableVector::EvaluaIndices ()
{
  ListaExpresiones & L = Lista;
  L.Reset ();
  unsigned int *Vec = VectorIndices;
  for (int j = 0; j < L.GetNItems(); ++j)
    {
      Token *t = EvaluaPostfijo (L.Itera());
      if (Buzon.GetHuboError ())
        return;
      if (t->GetTipoDato () != REAL)
        {
          //los indices deben ser enteros positivos
          // un error pa' el buzon
          delete t;
          Buzon.Error (INDICES);
          return;
        }
      long double v = t->GetDatoReal ();
      delete t;
      if (v < 0L || v > 65535L || (floor (v) != v))
        {
          // los indices deben ser enteros positivos
          //menores o iguales a 65535; un error pa' el buzon
          Buzon.Error (INDICES);
          return;
        }
      Vec[j] = v;
    }
}


/* We do touch orig. */
void VariableVector::AlmacenaVectorInternal(char *Cadena)
{
  int Largo = strlen (Cadena);

  int ActualChar = 0;
  for (; ActualChar < Largo && (Cadena[ActualChar] != '('); ++ActualChar);
  char tmp = Cadena[ActualChar];
  Cadena[ActualChar] = 0;
  Token *t = GetPostfijo (Cadena);
  Cadena[ActualChar] = tmp;

  if (Buzon.GetHuboError ())
    return;

  if (!t)
    {
      //Uso ilegal de la coma o cadena vacia; un error pa'l buzon
      Buzon.Error (ILEGAL_COMA_O_CADENA_VACIA);
      return;
    }
  if ((t->GetTipoAlmacenamiento () != VARIABLE) || (t->GetSig ()))
    {
      //Solo se admiten camos variables; un error pa'l buzon
      LiberarListaToken (t);
      Buzon.Error (SOLO_VARIABLES);
      return;
    }

  Identificador = dfd_strdup(t->GetDatoStr());

  delete t;

  if (Cadena[ActualChar] == 0)
    return;

  // Ahora los indices...

  int EsCadena = 0;
  int IniChar = ActualChar + 1, Parent = 1;
  for (++ActualChar; (ActualChar < Largo) && (Parent > 0); ++ActualChar)
    {
      char &Casilla = Cadena[ActualChar];
      if (Casilla == '(' && !EsCadena)
        ++Parent;
      if (Casilla == ')' && !EsCadena)
        --Parent;
      if (Casilla == '\'')
        EsCadena = 1 - EsCadena;

      if (!EsCadena && (((Casilla == ',') && (Parent == 1))
                        || ((Casilla == ')') && (!Parent))))
        {
          char Temp = Casilla;
          Casilla = 0;
          t = GetPostfijo (Cadena + IniChar);
          Casilla = Temp;
          IniChar = ActualChar + 1;
          if (Buzon.GetHuboError ())
            return;
          if (!t)
            {
              //Uso ilegal de la coma o cadena vacia; un error pa'l buzon
              Buzon.Error (ILEGAL_COMA_O_CADENA_VACIA);
              return;
            }
          Lista.Insertar (t);
        }
    }
  if (Parent)
    {
      //No se cerraron todos los parentesis abiertos; un error pa'l buzon
      Buzon.Error (ABIERTO_PARENTESIS_NO_CERRADO);
      return;
    }
  int n = Lista.GetNItems ();
  if (!n)
    {
      //No hay indices entre los parentesis; un error pa'l buzon
      Buzon.Error (NO_INDICES);
      return;
    }
  VectorIndices = new unsigned int[n];
  if (ActualChar >= Largo)
    return;
  t = GetPostfijo (Cadena + ActualChar);
  if (t)
    // Solo se admiten campos variables; un error pa'l buzon
    Buzon.Error (SOLO_VARIABLES);
  return;
}

void
VariableVector::AlmacenaVector (const char *orig)
{
  if (!orig)
    {
      Buzon.Error (ILEGAL_COMA_O_CADENA_VACIA);
      return;
    }

  char * Cadena = dfd_strdup(orig);
  AlmacenaVectorInternal (Cadena);
  delete []Cadena;
}

