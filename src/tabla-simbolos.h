#ifndef __TABLA__SIMBOLOS__H__
#define __TABLA__SIMBOLOS__H__

#include <variables.h>

class Tabla {
  Variable *Inicio;

 public:
  Tabla();

  Variable *Crear(const char *Id, Token *Untoken, Variable *Vengo,
                  unsigned *Indices, int Dim);
  Variable *Crear(const char *Id, Variable *Vengo, bool FP);
  Variable *Buscar(const char *Id);
  Variable *GetInicio() { return Inicio; }
  Token *Leer(const char *Id, unsigned *Indices, int Dim);
  void AsignarValor(const char *Id, Token *UnToken, unsigned *Indices, int Dim);
  void SetSig(Tabla *UnSig) { Sig = UnSig; }
  //Tabla *GetSig() { return Sig; }
  ~Tabla();
};

#endif
