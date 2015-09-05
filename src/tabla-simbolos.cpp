#include <dfd.h>
#include <tabla-simbolos.h>
#include <errores.h>

#include <string.h>

Tabla::Tabla() {
  ContadorTabla++;
}

Variable *Tabla::Buscar(const char *Id) {
  /* TODO: hacer que sea más rápido con hashing */
  for (Variable *var: simbolos_) {
      if (!strcasecmp(Id, var->GetIdentificador())) {
          return var;
      }
  }
  return NULL;
}

Token *Tabla::Leer(const char *Id, unsigned *Indices, int Dim) {
  Variable *Aux = Buscar(Id);
  if (!Aux) {
    Buzon.SetIdentificadorAsociado(Id);
    Buzon.Error(VARIABLE_NO_EXISTE);
    return 0;
  }
  return Aux->Leer(Indices, Dim);
}

Tabla::~Tabla() {
  ContadorTabla--;
  for (Variable *var: simbolos_) {
    if ((var->GetFU()) && (!var->GetFP())) {
        delete var->GetCampo();
    }
  }
}

Variable *Tabla::Crear(const char *Id, Token *UnToken, Variable *Vengo,
                       unsigned *Indices, int Dim) {
  Variable *NuevaVariable;
  NuevaVariable = new Variable(Id, UnToken, Vengo, Indices, Dim);
  if (Buzon.GetHuboError()) {
    delete NuevaVariable;
    return 0;
  }
  simbolos_.push_back(NuevaVariable);
  return NuevaVariable;
}

Variable *Tabla::Crear(const char *Id, Variable *Vengo, bool FP) {
  Variable *NuevaVariable;
  NuevaVariable = new Variable(Id, Vengo);
  NuevaVariable->SetFP(FP);
  if (Buzon.GetHuboError()) {
    delete NuevaVariable;
    return 0;
  }
  simbolos_.push_back(NuevaVariable);
  return NuevaVariable;
}

void Tabla::AsignarValor(const char *Id, Token *UnToken, unsigned *Indices,
                         int Dim) {
  Variable *Aux = Buscar(Id);

  if (!Aux)
    Crear(Id, UnToken, 0 /*VengoDe */, Indices, Dim);
  else
    Aux->AsignarValor(UnToken, Indices, Dim);

  return;
}
