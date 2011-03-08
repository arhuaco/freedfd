#ifndef __OBJETOS__EJECUCION__H__
#define __OBJETOS__EJECUCION__H__

#include <variable-o-vector.h>
#include <dfd-wrappers.h>

class OE_Asignacion
{
  VariableVector Destinos;
  Token * Fuentes;
  char *Destino, *Fuente;

public:
  OE_Asignacion ();
  ~OE_Asignacion ();
  void Preprocesar ();
  void Despreprocesar ();
  void Ejecutar();

  void NuevaAsignacion(const char *destino, const char *fuente)
  {
    delete [] Destino;
    delete [] Fuente;
    LiberarListaToken(Fuentes);
    Destino = dfd_strdup(destino);
    Fuente = dfd_strdup(fuente);
  }

  const char *GetDestino ()
  {
    return Destino;
  }
  const char * GetFuente ()
  {
    return Fuente;
  }
};

#endif
