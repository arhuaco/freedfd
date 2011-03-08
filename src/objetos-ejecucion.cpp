#include <dfd.h>
#include <objetos-ejecucion.h>
#include <variable-o-vector.h>
#include <errores.h>
#include <token.h>
#include <tokeniza.h>
#include <evaluacion.h>
#include <entorno-ejecucion.h>

#include <string.h>

OE_Asignacion::OE_Asignacion ()
{
  Fuentes = NULL;
  Destino = Fuente = NULL;
}

OE_Asignacion::~OE_Asignacion ()
{
  delete []Destino;
  delete []Fuente;
  LiberarListaToken(Fuentes);
}

void
OE_Asignacion::Preprocesar ()
{
      Destinos.AlmacenaVector(Destino);
      if (Buzon.GetHuboError ())
        return;
      Token *Temp = GetPostfijo (Fuente);
      if (Buzon.GetHuboError ())
        return;
      if (!Temp)
        {
          Buzon.Error (ILEGAL_COMA_O_CADENA_VACIA);
          return;
        }

      Fuentes = Temp;

  return;
}

void
OE_Asignacion::Despreprocesar ()
{
  Destinos.Vacear ();
  LiberarListaToken(Fuentes);
  Fuentes = NULL;
}

void
OE_Asignacion::Ejecutar ()
{
   Destinos.EvaluaIndices ();
   if (Buzon.GetHuboError ())
      return;
   const char *Id = Destinos.GetIdentificador ();
   unsigned int *Vec = Destinos.GetVectorIndices ();
   int NInd = Destinos.GetLista().GetNItems();
   Token *t = EvaluaPostfijo (Fuentes);
   if (Buzon.GetHuboError ())
     return;

   PilaDeTablas.AsignarValor (Id, t, Vec, NInd);
   delete t;

    //if (Buzon.GetHuboError ())
    //  return;
    // Uncomment if you add more code.
}
