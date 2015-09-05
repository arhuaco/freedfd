#include <dfd.h>
#include <tabla-simbolos.h>
#include <errores.h>

#include <string.h>

Tabla::Tabla() {
  Inicio = 0;
  ContadorTabla++;
}

Variable *Tabla::Buscar(const char *Id) {
  /* TODO: hacer que sea más rápido con hashing */
  Variable *Aux = Inicio;
  while (Aux) {
    if (!strcasecmp(Id, Aux->GetIdentificador())) break;
    Aux = Aux->GetSig();
  }
  return Aux;
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
  Variable *Aux;
  while (Inicio) {
    Aux = Inicio;
    Inicio = Inicio->GetSig();
    if ((Aux->GetFU()) && (!Aux->GetFP())) delete Aux->GetCampo();
    delete Aux;
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
  NuevaVariable->SetSig(Inicio);
  Inicio = NuevaVariable;
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
  NuevaVariable->SetSig(Inicio);
  Inicio = NuevaVariable;
  return NuevaVariable;
}

void Tabla::AsignarValor(const char *Id, Token *UnToken, unsigned *Indices,
                         int Dim) {
  Variable *Aux = Buscar(Id);

  if (!Aux)
    Crear(Id, UnToken, 0 /*VengoDe */, Indices, Dim);
  else
    Aux->AsignarValor(UnToken, Indices, Dim);

  return; }
