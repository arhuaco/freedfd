#include <dfd.h>
#include <token.h>
#include <dfd-wrappers.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

Token::~Token ()
{
  --ContadorToken;

  if (TipoD == STRING && DatoStr)
    delete[]DatoStr;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Token::Token (const Token & UnToken)
{

  // Depuración, para buscar tokens reservados pero no liberados.
  // Al terminar una evaluación, el número de tokens debe ser conocido.
  ++ContadorToken;

  TipoD = UnToken.TipoD;
  TipoT = UnToken.TipoT;
  Alcance = UnToken.Alcance;
  Almacenamiento = UnToken.Almacenamiento;
  Pdp = UnToken.Pdp;
  Pfp = UnToken.Pfp;

  if (TipoD == STRING)
    DatoStr = dfd_strdup (UnToken.DatoStr);
  else
    DatoReal = UnToken.DatoReal;
}

Token::Token (bool ValorLogico, TipoToken UnTipoToken,
              TipoAlmacenamiento UnTipoAlmacenamiento ){
  ++ContadorToken;
  DatoLogico= ValorLogico;
  TipoT= UnTipoToken;
  Almacenamiento= UnTipoAlmacenamiento;
  TipoD= LOGICO; }

Token::Token (long double ValorReal, TipoToken UnTipoToken,
              TipoAlmacenamiento UnTipoAlmacenamiento)
{
  ++ContadorToken;              //Depuración
  DatoReal = ValorReal;
  TipoT = UnTipoToken;
  Almacenamiento = UnTipoAlmacenamiento;
  TipoD = REAL;
}

Token::Token (const char *ValorStr, TipoToken UnTipoToken,
              TipoAlmacenamiento UnTipoAlmacenamiento, int UnPdp, int UnPfp,
              AlcanceOperador UnAlcance)
{

  ++ContadorToken;              //Depuración
  TipoT = UnTipoToken;
  Almacenamiento = UnTipoAlmacenamiento;
  TipoD = STRING;
  Alcance = UnAlcance;
  Pdp = UnPdp;
  Pfp = UnPfp;
  DatoStr = dfd_strdup (ValorStr);
}

const char *
Token::AsString(char *buff, int buff_size)
{
  switch(TipoD)
  {
    const char *tmp;
    case REAL:
      snprintf(buff, buff_size, "%Lg", DatoReal);
      break;
    case STRING:
    case LOGICO:
      tmp = TipoD == STRING ? DatoStr :
            (DatoLogico ? ".f." : ".v.");
      snprintf(buff, buff_size, "%s", tmp);
      break;
    default:
      assert(0);
  }
  return buff;
}

void
Apila (Token ** Inicio, Token * Nuevo)
{
  Nuevo->SetSig (*Inicio);
  *Inicio = Nuevo;
  return;
}

Token *
Desapila (Token ** Inicio)
{
  Token *Ret = *Inicio;
  *Inicio = (*Inicio)->GetSig ();
  return Ret;
}

void
LiberarListaToken (Token * Inicio)
{

  Token *Aux;
  while (Inicio)
    {
      Aux = Inicio;
      Inicio = Inicio->GetSig ();
      delete Aux;
    }
}


