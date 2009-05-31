#include <dfd.h>
#include <objetos-ejecucion.h>
#include <lista-expresiones.h>
#include <lista-vectores.h>
#include <errores.h>
#include <token.h>
#include <tokeniza.h>
#include <evaluacion.h>
#include <entorno-ejecucion.h>
#include <lista-cadenas.h> // Usamos STL?

#include <string.h>

/* Esto está complicado. Creo que se puede simplificar, y separar
 * completamente de la parte gráfica. */

// FIXME: what do we do here? Don't allow it?
OE_Asignacion::OE_Asignacion ()
{
}

OE_Asignacion::~OE_Asignacion ()
{
       delete []Destino;
       delete []Fuente;
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
      Fuentes.Insertar (Temp);

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
