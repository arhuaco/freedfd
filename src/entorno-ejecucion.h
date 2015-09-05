#ifndef __ENTORNO_EJECUCION_H__
#define __ENTORNO_EJECUCION_H__

#include <tabla-simbolos.h>
#include <token.h>
#include <vector>

/* Using a bit of new c++ to make things simpler. */

class PilaDeTablas {
  std::vector <Tabla *> pila_;

 public:
  void Apilar(Tabla *tabla) {
    pila_.push_back(tabla);
  }

  void Desapilar() {
    delete pila_.back();
    pila_.pop_back();
  }

  Tabla * GetTablaActual() {
      return pila_.back();
  }

  Variable *Crear(char *Id, Token *UnToken, Variable *Vengo, unsigned *Indices,
                  int Dim) {
    return GetTablaActual()->Crear(Id, UnToken, Vengo, Indices, Dim);
  }

  Variable *Crear(char *Id, Variable *Vengo, bool FP) {
    return GetTablaActual()->Crear(Id, Vengo, FP);
  }

  Variable *Buscar(const char *Id);

  void AsignarValor(const char *Id, Token *UnToken, unsigned *Indices,
                    int Dim) {
    GetTablaActual()->AsignarValor(Id, UnToken, Indices, Dim);
    return;
  }

  Token *Leer(const char *Id, unsigned *Indices, int Dim);

  void ActualizarVariables(Variable *);
  void Vacear();
};

/* Busca el valor de una variable en el entorno de ejecución actual. */
Token *EntornoEjecucion_BuscaSimbolo(const char *symbol);
/* Busca el valor de una variable en el entorno de ejecución actual.
 * Si el token que llega es una constante, retorna el mismo token */
Token *EntornoEjecucion_BuscaSimbolo(Token *symbol);

void ActualizarVariables(Variable *);

#endif
