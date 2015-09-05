#ifndef __ENTORNO_EJECUCION_H__
#define __ENTORNO_EJECUCION_H__

#include <tabla-simbolos.h>
#include <token.h>

class PilaDeTablas {
  Tabla *Tope;

 public:
  PilaDeTablas() { Tope = 0; }
  void Apilar(Tabla *UnaTabla) {
    UnaTabla->SetSig(Tope);
    Tope = UnaTabla;
  }
  void Desapilar() {
    Tabla *Aux;
    Aux = Tope;
    Tope = Tope->GetSig();
    delete Aux;
  }

  Variable *Crear(char *Id, Token *UnToken, Variable *Vengo, unsigned *Indices,
                  int Dim) {
    return Tope->Crear(Id, UnToken, Vengo, Indices, Dim);
  }
  Variable *Crear(char *Id, Variable *Vengo, bool FP) {
    return Tope->Crear(Id, Vengo, FP);
  }

  Variable *Buscar(const char *Id);

  void AsignarValor(const char *Id, Token *UnToken, unsigned *Indices,
                    int Dim) {
    Tope->AsignarValor(Id, UnToken, Indices, Dim);
    return;
  }
  Tabla *GetTope() { return Tope; }
  Token *Leer(const char *Id, unsigned *Indices, int Dim);
  void ActualizarVariables(Variable *);
  void Vacear();
};

/* Busca el valor de una variable en el entorno de ejecuci�n actual. */
Token *EntornoEjecucion_BuscaSimbolo(const char *symbol);
/* Busca el valor de una variable en el entorno de ejecuci�n actual.
 * Si el token que llega es una constante, retorna el mismo token */
Token *EntornoEjecucion_BuscaSimbolo(Token *symbol);

void ActualizarVariables(Variable *);

#endif
