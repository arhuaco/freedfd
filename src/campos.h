#ifndef __CAMPOS__H__
#define __CAMPOS__H__

#include <dfd.h>
#include <operador.h>

  // TODO: Usar esto en depuración. Para evitar memoria usada y no devuelta
  // TODO: Cambiar por una clase o algo. Ahora mismo es una variable global.
  // extern int ContadorToken;

class Token
{

  TipoDato TipoD;
  TipoToken TipoT;

  AlcanceOperador Alcance;
  TipoAlmacenamiento Almacenamiento;

  // Con estas prioridades se define la precedencia de los operadores
  int Pdp;                      // Prioridad dentro de la pila
  int Pfp;                      // Prioridad fuera de la pila

  union
  {
    long double DatoReal;
    char *DatoStr;
    bool DatoLogico;
    char *Operador;
  };
  Token *Sig;
public:

   ~Token ();

    Token (bool ValorLogico, TipoToken UnTipoToken,
           TipoAlmacenamiento UnTipoAlmacenamiento);
    Token (long double ValorReal, TipoToken UnTipoToken,
           TipoAlmacenamiento UnTipoAlmacenamiento);
    Token (const char *ValorStr, TipoToken UnTipoToken,
           TipoAlmacenamiento UnTipoAlmacenamiento, int Pdp = 0, int Pfp = 0,
           AlcanceOperador UnAlcance = UNITARIO);
    Token (const Token & UnToken);

  bool GetDatoLogico ()
  {
    return DatoLogico;
  }
  char *GetDatoStr ()
  {
    return DatoStr;
  }
  long double GetDatoReal ()
  {
    return DatoReal;
  }
  char *GetOperador ()
  {
    return Operador;
  }
  TipoDato GetTipoDato ()
  {
    return TipoD;
  }
  TipoToken GetTipoToken ()
  {
    return TipoT;
  }
  TipoAlmacenamiento GetTipoAlmacenamiento ()
  {
    return Almacenamiento;
  }
  AlcanceOperador GetAlcanceOperador ()
  {
    return Alcance;
  }
  Token *GetSig ()
  {
    return Sig;
  }
  int GetPfp ()
  {
    return Pfp;
  }
  int GetPdp ()
  {
    return Pdp;
  }
  void SetPdp (int UnPdp)
  {
    Pdp = UnPdp;
  }
  void SetPfp (int UnPfp)
  {
    Pfp = UnPfp;
  }
  void setdato (char *valorstr);
  void SetDato (bool ValorLogico)
  {
    DatoLogico = ValorLogico;
  }
  void SetDato (long double ValorReal)
  {
    DatoReal = ValorReal;
  }
  void SetTipoAlmacenamiento (TipoAlmacenamiento UnTipo)
  {
    Almacenamiento = UnTipo;
  }
  void SetAlcanceOperador (AlcanceOperador UnAlcance)
  {
    Alcance = UnAlcance;
  }

  void SetSig (Token * UnSig)
  {
    Sig = UnSig;
  }

  /* return the string resepretation of the token */
  void asString(char *buff, int buff_size);
};

#endif
