#include <dfd.h>
#include <entorno-ejecucion.h>
#include <errores.h>

void PilaDeTablas::Vacear() {
  while (pila_.size() > 0) {
    Desapilar();
  }
}

Token *PilaDeTablas::Leer(const char *Id, unsigned *Indices, int Dim) {
  if (pila_.size() == 0) {
    Buzon.SetIdentificadorAsociado(Id);
    Buzon.Error(VARIABLE_NO_EXISTE);
    return 0;
  }
  return GetTablaActual()->Leer(Id, Indices, Dim);
}

Variable *PilaDeTablas::Buscar(const char *Id) {
  if (pila_.size() == 0) {
    Buzon.SetIdentificadorAsociado(Id);
    Buzon.Error(VARIABLE_NO_EXISTE);
    return 0;
  }
  return GetTablaActual()->Buscar(Id);
}

void PilaDeTablas::ActualizarVariables(Variable *Muestra) {
  Variable *Primera = Muestra->GetPrimerPadre();
  bool HastaAqui = false;
  for (int idx = pila_.size() - 1; idx >= 0; --idx) {
    if (pila_[idx]->ActualizarVariables(Muestra, Primera)) {
      break; /* El primer padre ha sido actualizado. */
    }
  }
}

Token *EntornoEjecucion_BuscaSimbolo(const char *symbol) {
  Token *Retorno = Entorno.Leer(symbol, 0, 0);
  if (Buzon.GetHuboError()) return 0;
  Retorno->SetTipoAlmacenamiento(VARIABLE);
  return Retorno;
}

Token *EntornoEjecucion_BuscaSimbolo(Token *UnToken) {
  if (UnToken->GetTipoAlmacenamiento() == CONSTANTE) return UnToken;
  return EntornoEjecucion_BuscaSimbolo(UnToken->GetDatoStr());
}
