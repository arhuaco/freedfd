#ifndef __VARIABLE__H__
#define __VARIABLE__H__

class CampoVariable;
class Variable
{

  char *Identificador;
  CampoVariable *Campo;
  TipoDato Tipo;
  bool FP;
  bool FU;
  Variable *VengoDe;
  Variable *PrimerPadre;
  Variable *Sig;

public:
    Variable (char *Id, Token * UnToken, Variable * Vengo, unsigned *Indices,
              int Dim);
    Variable (char *Id, Variable * Vengo);
  void AsignarValor (Token * UnToken, unsigned *Indices, int Dim);
  Token *Leer (unsigned *Indices, int Dim);

  TipoDato GetTipo ()
  {
    return Tipo;
  }
  bool GetFP ()
  {
    return FP;
  }
  bool GetFU ()
  {
    return FU;
  }
  Variable *GetVengoDe ()
  {
    return VengoDe;
  }
  Variable *GetSig ()
  {
    return Sig;
  }
  Variable *GetPrimerPadre ()
  {
    return PrimerPadre;
  }
#if 0
  CampoVariable *GetCampo ()
  {
    return Campo;
  }
  void SetCampo (CampoVariable * UnCampo)
  {
    Campo = UnCampo;
  }
#endif
  char *GetIdentificador ()
  {
    return Identificador;
  }
  void SetSig (Variable * UnSig)
  {
    Sig = UnSig;
  }
  void SetFP (bool UnFP)
  {
    FP = UnFP;
  }
  void SetFU (bool UnFU)
  {
    FU = UnFU;
  }
  void SetTipo (TipoDato UnTipo)
  {
    Tipo = UnTipo;
  }
  ~Variable ();
  CampoVariable *GetCampo ()
  {
    return Campo;
  }

};

#endif
