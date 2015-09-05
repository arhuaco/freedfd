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

  // Usado para actualizar variables de un subprograma desde otra tabla.
  // Retorna verdadero cuando se ha actualizado el primer padre.
  bool ActualizarVariables(Variable *muestra, Variable *primera) {
    for (Variable *var : simbolos_) {
      if (var->GetPrimerPadre() == primera) {
        var->SetFU(true);
        var->SetCampo(muestra->GetCampo());
        var->SetTipo(muestra->GetCampo()->GetTipo());
      }
      if (var == primera) {
        return true;
      }
    }
    return false;
  }

  ~Tabla();
};

#endif
