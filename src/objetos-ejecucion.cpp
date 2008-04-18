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
