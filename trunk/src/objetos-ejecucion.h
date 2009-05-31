#ifndef __OBJETOS__EJECUCION__H__
#define __OBJETOS__EJECUCION__H__

/* Dejemos este c�digo ac� temporalmente, para poder
 * hacer el REPL. No sabemos si lo vamos a usar. */

/* Esto est� complicado. Creo que se puede simplificar, y separar
 * completamente de la parte gr�fica. */

#include <lista-expresiones.h>
#include <lista-vectores.h>
#include <lista-cadenas.h>
#include <dfd-wrappers.h>

class OE_Asignacion
{
  ListaVectores Destinos;
  ListaExpresiones Fuentes;
  char *Destino, *Fuente;
public:
  OE_Asignacion ();
  ~OE_Asignacion ();
  void Preprocesar ();
  void Despreprocesar ();
  void Ejecutar();

  void NuevaAsignacion(const char *destino, const char *fuente)
  {
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