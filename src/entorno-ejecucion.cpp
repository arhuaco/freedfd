#include <entorno-ejecucion.h>

void
PilaDeTablas::Vacear ()
{
  while (Tope)
    {
      Tabla *Aux = Tope;
      Tope = Tope->GetSig ();
      delete Aux;
    }
}

Token *
PilaDeTablas::Leer (char *Id, unsigned *Indices, int Dim)
{
  if (!Tope)
    {
      Buzon.SetIdentificadorAsociado (Id);
      Buzon.Error (VARIABLE_NO_EXISTE);
      return 0;
    }
  return Tope->Leer (Id, Indices, Dim);
}


Variable *
PilaDeTablas::Buscar (char *Id)
{
  if (!Tope)
    {
      Buzon.SetIdentificadorAsociado (Id);
      Buzon.Error (VARIABLE_NO_EXISTE);
      return 0;
    }
  return Tope->Buscar (Id);
}

void
PilaDeTablas::ActualizarVariables (Variable * Muestra)
{
  Variable *Primera = Muestra->GetPrimerPadre ();
  bool HastaAqui = false;
  for (Tabla * t = Tope; t && !HastaAqui; t = t->GetSig ())
    for (Variable * v = t->GetInicio (); v; v = v->GetSig ())
      {

        if (v->GetPrimerPadre () == Primera)
          {
            v->SetFU (true);
            v->SetCampo (Muestra->GetCampo ());
            v->SetTipo (Muestra->GetCampo ()->GetTipo ());
          }
        if (v == Primera)
          {
            HastaAqui = true;
            break;
          }

      }
}

void
ActualizarVariables (Variable * Muestra)
{
  PilaDeTablas.ActualizarVariables (Muestra);
}

