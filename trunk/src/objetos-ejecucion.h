#ifndef __OBJETOS__EJECUCION__H__
#define __OBJETOS__EJECUCION__H__

/* Dejemos este código acá temporalmente, para poder
 * hacer el REPL. No sabemos si lo vamos a usar. */

/* Esto está complicado. Creo que se puede simplificar, y separar
 * completamente de la parte gráfica. */

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

  void NuevaAsignacion(const char *destino, const char *fuente)
  {
    /* esto de tener listas de cadenas no se va a necesitar */
    CadenasDestino.Vacear();
    CadenasDestino.Insertar(destino);
    CadenasFuente.Vacear();
    CadenasFuente.Insertar(fuente);
  }

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
