#ifndef __VARIABLE__H__
#define __VARIABLE__H__

#include <dfd.h>
#include <token.h>
//#include <tabla-simbolos.h>

class CampoVariable
{
  TipoDato Tipo;
  int Dimension;
  struct Cajita
  {
    Cajita ()
    {
      ContadorCajita++;
    }
     ~Cajita ()
    {
      ContadorCajita--;
    }
    unsigned int *Indices;
    union
    {
      char *DatoStr;
      long double DatoReal;
      bool DatoLogico;
    };
    Cajita *Sig;
  } *Inicio;

public:
  CampoVariable (Token * UnToken, unsigned *Indices, int Dim, const char *Id);
  Token *Leer (unsigned *Indices, int Dim, char *Id);
  void AsignarValor (Token * UnToken, unsigned *Indices, int Dim, const char *Id);
  ~CampoVariable ();
  TipoDato GetTipo ()
  {
    return Tipo;
  }

};

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
    Variable (const char *Id, Token * UnToken, Variable * Vengo, unsigned *Indices,
              int Dim);
    Variable (const char *Id, Variable * Vengo);
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

  CampoVariable *GetCampo ()
  {
    return Campo;
  }
  void SetCampo (CampoVariable * UnCampo)
  {
    Campo = UnCampo;
  }

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

};

#endif
