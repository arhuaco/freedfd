#ifndef __OBJETOS__EJECUCION__H__
#define __OBJETOS__EJECUCION__H__


#include <lista-expresiones.h>
#include <lista-vectores.h>
#include <lista-cadenas.h>

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

#endif
