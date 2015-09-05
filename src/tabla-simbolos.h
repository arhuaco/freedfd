#ifndef __TABLA__SIMBOLOS__H__
#define __TABLA__SIMBOLOS__H__

#include <variables.h>
#include <vector>

/* Using a bit of new C++ here to simplify. */

class Tabla {
  std::vector<Variable *> simbolos_;

 public:
  Tabla();
  Variable *Crear(const char *Id, Token *Untoken, Variable *Vengo,
                  unsigned *Indices, int Dim);
  Variable *Crear(const char *Id, Variable *Vengo, bool FP);
  Variable *Buscar(const char *Id);
  Token *Leer(const char *Id, unsigned *Indices, int Dim);
  void AsignarValor(const char *Id, Token *UnToken, unsigned *Indices, int Dim);
  ~Tabla();
};

#endif
