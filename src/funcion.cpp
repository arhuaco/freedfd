/*
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/*
Las siguientes son las funciones que llevan a cabo las operaciones
logicas, reales y de cadena entre datos en un DFD, y algunas funciones
auxiliares.
*/

static bool
FueraDeRango (Token * UnToken)
{

  long double Valor = fabsl (UnToken->GetDatoReal ());
  if (fabsl (Valor) > 1e2000L)
    {
      Buzon.Error (DESBORDAMIENTO);
      delete UnToken;
      return true;
    }
  if (fabsl (Valor) < 1e-2000L && Valor != 0.0L)
    {
      Buzon.Error (DESBORDAMIENTO_NEGATIVO);
      delete UnToken;
      return true;
    }
  return false;
}

static bool
NoEsReal (Token * UnToken)
{

  if (UnToken->GetTipoDato () != REAL)
    {
      Buzon.Error (OPERADOR_ILEGAL);
      return true;
    }
  return false;
}

static bool
NoEsLogico (Token * UnToken)
{

  if (UnToken->GetTipoDato () != LOGICO)
    {
      Buzon.Error (OPERADOR_LOGICO_ILEGAL);
      return true;
    }
  return false;
}

static bool
NoEsString (Token * UnToken)
{

  if (UnToken->GetTipoDato () != STRING)
    {
      Buzon.Error (OPERADOR_ILEGAL);
      return true;
    }
  return false;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Token *
ObtieneTokenPila (Token * UnToken)
{

  if (UnToken->GetTipoAlmacenamiento () == CONSTANTE)
    return UnToken;
  Token *Retorno = PilaDeTablas.Leer (UnToken->GetDatoStr (), 0, 0);
  if (Buzon.GetHuboError ())
    return 0;
  Retorno->SetTipoAlmacenamiento (VARIABLE);
  return Retorno;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
BorrarToken (Token * UnToken)
{
  if (UnToken->GetTipoAlmacenamiento () == VARIABLE)
    delete UnToken;
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

template < class TIPO > static Token *
ConsigueToken (TIPO Valor)
{
  Token *Nuevo = new Token (Valor, OPERANDO, CONSTANTE);
  return Nuevo;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 // Las siguientes son las funciones utilizadas por el evaluador de
 // expresiones.


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
MenosUnitario (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }
  long double ValorRetorno = -1.0L * Operando->GetDatoReal ();
  BorrarToken (Operando);
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;

}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Seno (Token ** Pila)
{
  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  ValorDominio = Estado.AngulosEnGrados ? ValorDominio * (M_PI / 180.0L) :
    ValorDominio;
  long double ValorRetorno;
  if (fabsl (cosl (ValorDominio)) == 1.0L)
    ValorRetorno = 0.0L;
  else
    ValorRetorno = sinl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  if (FueraDeRango (TokenRetorno))
    return;
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Coseno (Token ** Pila)
{
  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }
  long double ValorRetorno;
  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  ValorDominio = Estado.AngulosEnGrados ? ValorDominio * (M_PI / 180.0L) :
    ValorDominio;
  if (fabsl (sinl (ValorDominio)) == 1.0L)
    ValorRetorno = 0.0L;
  else
    ValorRetorno = cosl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  if (FueraDeRango (TokenRetorno))
    return;
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Tangente (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  ValorDominio = Estado.AngulosEnGrados ? ValorDominio * M_PI / 180.0L :
    ValorDominio;
  long double ValorSeno = sinl (ValorDominio);
  long double ValorCoseno = cosl (ValorDominio);
  long double ValorRetorno;
  if (fabsl (ValorSeno) == 1.0L)
    {
      Buzon.Error (LLAMADO_DE_FUNCION_NO_VALIDO);
      return;
    }
  else if (ValorCoseno == 1.0L)
    ValorRetorno = 0.0L;
  else
    ValorRetorno = tanl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  if (FueraDeRango (TokenRetorno))
    return;
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
SenoHiperbolico (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  long double ValorRetorno = sinhl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  if (FueraDeRango (TokenRetorno))
    return;
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
CosenoHiperbolico (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  long double ValorRetorno = coshl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  if (FueraDeRango (TokenRetorno))
    return;
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
TangenteHiperbolica (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  long double ValorRetorno = tanhl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  if (FueraDeRango (TokenRetorno))
    return;
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
ArcoSeno (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  long double ValorRetorno = asinl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  ValorRetorno = Estado.AngulosEnGrados ? ValorRetorno * (180.0L / M_PI) :
    ValorRetorno;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
ArcoCoseno (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  long double ValorRetorno = acosl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  ValorRetorno = Estado.AngulosEnGrados ? ValorRetorno * (180.0L / M_PI) :
    ValorRetorno;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
ArcoTangente (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  long double ValorRetorno = atanl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  ValorRetorno = Estado.AngulosEnGrados ? ValorRetorno * (180.0L / M_PI) :
    ValorRetorno;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Exp (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  long double ValorRetorno = expl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  if (FueraDeRango (TokenRetorno))
    return;
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Log (Token ** Pila)
{
  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  long double ValorRetorno = log10l (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);

  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Ln (Token ** Pila)
{
  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }

  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);
  long double ValorRetorno = logl (ValorDominio);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);

  return;
}

/// nelson

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Trunc (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }
  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);

  long double ValorRetorno = floorl (fabsl (ValorDominio)) *
    (ValorDominio < 0 ? (-1.0L) : (1.0L));

  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;

}


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Round (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }
  long double ValorDominio = Operando->GetDatoReal ();
  BorrarToken (Operando);

  long double ValorRetorno = floorl (fabsl (ValorDominio) + 0.5L) *
    (ValorDominio < 0 ? (-1.0L) : (1.0L));

  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;

}


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


void
Abs (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }
  long double ValorRetorno = fabsl (Operando->GetDatoReal ());
  BorrarToken (Operando);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;

}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Sqrt (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }
  long double ValorRetorno = sqrtl (Operando->GetDatoReal ());
  BorrarToken (Operando);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;

}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


void
Random (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsReal (Operando))
    {
      BorrarToken (Operando);
      return;
    }
  long double ValorDominio = (Operando->GetDatoReal ());
  BorrarToken (Operando);
  long double ValorAleatorio = (long double) rand ();
  if (ValorDominio > 32767.0L)
    ValorAleatorio *= ValorDominio / 32767.0L;
  long double ValorRetorno = ceill (fmod (ValorAleatorio, ValorDominio));
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;

}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Not (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsLogico (Operando))
    {
      BorrarToken (Operando);
      return;
    }
  bool ValorRetorno = (Operando->GetDatoLogico ())? false : true;
  BorrarToken (Operando);
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;

}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Len (Token ** Pila)
{

  Token *Operando = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  if (NoEsString (Operando))
    {
      BorrarToken (Operando);
      return;
    }
  long double ValorRetorno = (long double) strlen (Operando->GetDatoStr ());
  BorrarToken (Operando);
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;

}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// La funcion relacion es utilizada por los operadores relacionales

void
Relacion (Token ** Pila, int Caso)
{
  Token *Op2 = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  Token *Op1 = ObtieneTokenPila ((*Pila)->GetSig ());
  if (Buzon.GetHuboError ())
    {
      BorrarToken (Op2);
      return;
    }
  if (Op1->GetTipoDato () != Op2->GetTipoDato ())
    {
      BorrarToken (Op2);
      BorrarToken (Op1);
      Buzon.Error (LOS_TIPOS_NO_COINCIDEN);
      return;
    }
  TipoDato Tipo = Op1->GetTipoDato ();
  bool Respuesta;
  switch (Caso)
    {

    case 1:
    case 4:
      switch (Tipo)
        {

        case REAL:
          Respuesta = (Op1->GetDatoReal () < Op2->GetDatoReal ())?
            true : false;
          break;

        case STRING:
          Respuesta = (strcmp (Op1->GetDatoStr (), Op2->GetDatoStr ()) < 0) ?
            true : false;
          break;

        default:
          Buzon.Error (OPERADOR_LOGICO_ILEGAL);
          break;
        }
      break;

    case 2:
    case 3:
      switch (Tipo)
        {

        case REAL:
          Respuesta = (Op1->GetDatoReal () > Op2->GetDatoReal ())?
            true : false;
          break;

        case STRING:
          Respuesta = (strcmp (Op1->GetDatoStr (), Op2->GetDatoStr ()) > 0) ?
            true : false;
          break;

        default:
          Buzon.Error (OPERADOR_LOGICO_ILEGAL);
          break;
        }
      break;
    case 5:
    case 6:
      switch (Tipo)
        {

        case REAL:
          Respuesta = (Op1->GetDatoReal () == Op2->GetDatoReal ())?
            true : false;
          break;

        case STRING:
          Respuesta = (strcmp (Op1->GetDatoStr (), Op2->GetDatoStr ()))?
            false : true;
          break;

        case LOGICO:
          Respuesta = (Op1->GetDatoLogico () == Op2->GetDatoLogico ())?
            true : false;
          break;

        }
      break;
    }

  BorrarToken (Op1);
  BorrarToken (Op2);
  if (Buzon.GetHuboError ())
    return;
  if (Caso == 3 || Caso == 4 || Caso == 6)
    Respuesta = Respuesta ? false : true;
  Token *TokenRetorno = ConsigueToken (Respuesta);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;

}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Menor (Token ** Pila)
{
  Relacion (Pila, 1);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Mayor (Token ** Pila)
{
  Relacion (Pila, 2);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
MenorIgual (Token ** Pila)
{
  Relacion (Pila, 3);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
MayorIgual (Token ** Pila)
{
  Relacion (Pila, 4);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Igual (Token ** Pila)
{
  Relacion (Pila, 5);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
NoIgual (Token ** Pila)
{
  Relacion (Pila, 6);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// La rutina LogicosBinarios es usada por los operadores logicos binarios

void
LogicosBinarios (Token ** Pila, int Caso)
{
  Token *Operando2 = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  Token *Operando1 = ObtieneTokenPila ((*Pila)->GetSig ());
  if (Buzon.GetHuboError ())
    {
      BorrarToken (Operando2);
      return;
    }
  if (NoEsLogico (Operando1))
    {
      BorrarToken (Operando2);
      BorrarToken (Operando1);
      return;
    }
  if (NoEsLogico (Operando2))
    {
      BorrarToken (Operando2);
      BorrarToken (Operando1);
      return;
    }
  bool Op2 = Operando2->GetDatoLogico ();
  BorrarToken (Operando2);
  bool Op1 = Operando1->GetDatoLogico ();
  BorrarToken (Operando1);
  bool Respuesta;
  switch (Caso)
    {

    case 1:
    case 2:
      Respuesta = (Op1 && Op2) ? true : false;
      break;

    case 3:
    case 4:
      Respuesta = (Op1 || Op2) ? true : false;
      break;

    case 5:
    case 6:
      Respuesta = ((Op1 || Op2) && !(Op1 && Op2)) ? true : false;
      break;

    }
  if (!(Caso % 2))              // Par
    Respuesta = Respuesta ? false : true;
  Token *TokenRetorno = ConsigueToken (Respuesta);
  if (Buzon.GetHuboError ())
    return;
  delete Desapila (Pila);
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
And (Token ** Pila)
{
  LogicosBinarios (Pila, 1);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Nand (Token ** Pila)
{
  LogicosBinarios (Pila, 2);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Or (Token ** Pila)
{
  LogicosBinarios (Pila, 3);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Nor (Token ** Pila)
{
  LogicosBinarios (Pila, 4);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Xor (Token ** Pila)
{
  LogicosBinarios (Pila, 5);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Xnor (Token ** Pila)
{
  LogicosBinarios (Pila, 6);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


void
Sumar (Token ** Pila)
{
  Token *Op2 = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  Token *Op1 = ObtieneTokenPila ((*Pila)->GetSig ());
  if (Buzon.GetHuboError ())
    {
      BorrarToken (Op2);
      return;
    }
  if (Op1->GetTipoDato () != Op2->GetTipoDato ())
    {
      BorrarToken (Op2);
      BorrarToken (Op1);
      Buzon.Error (LOS_TIPOS_NO_COINCIDEN);
      return;
    }
  TipoDato Tipo = Op1->GetTipoDato ();
  long double ValorRetorno;
  char *Temp;

  switch (Tipo)
    {
    case REAL:
      ValorRetorno = Op1->GetDatoReal () + Op2->GetDatoReal ();
      break;
    case STRING:

      int LongitudCadena;
      LongitudCadena = strlen (Op1->GetDatoStr ()) +
        strlen (Op2->GetDatoStr ()) + 1;
      Temp = new char[LongitudCadena];
      strcpy (Temp, Op1->GetDatoStr ());
      strcat (Temp, Op2->GetDatoStr ());
      break;

    default:
      Buzon.Error (OPERADOR_LOGICO_ILEGAL);
      break;
    };
  BorrarToken (Op1);
  BorrarToken (Op2);
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno;
  if (Tipo == STRING)
    {
      TokenRetorno = ConsigueToken (Temp);
      delete[]Temp;
    }
  else
    TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  if (Tipo == REAL)
    if (FueraDeRango (TokenRetorno))
      return;

  delete Desapila (Pila);
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
NumericosBinarios (Token ** Pila, int Caso)
{
  Token *Operando2 = ObtieneTokenPila (*Pila);
  if (Buzon.GetHuboError ())
    return;
  Token *Operando1 = ObtieneTokenPila ((*Pila)->GetSig ());
  if (Buzon.GetHuboError ())
    {
      BorrarToken (Operando2);
      return;
    }
  if (NoEsReal (Operando1))
    {
      BorrarToken (Operando1);
      BorrarToken (Operando2);
      return;
    }
  if (NoEsReal (Operando2))
    {
      BorrarToken (Operando1);
      BorrarToken (Operando2);
      return;
    }
  long double Op1 = Operando1->GetDatoReal ();
  BorrarToken (Operando1);
  long double Op2 = Operando2->GetDatoReal ();
  BorrarToken (Operando2);
  long double ValorRetorno;
  switch (Caso)
    {
    case 1:
      ValorRetorno = Op1 - Op2;
      break;
    case 2:
      ValorRetorno = Op1 * Op2;
      break;
    case 3:
      if (Op2)
        ValorRetorno = Op1 / Op2;
      else
        Buzon.Error (DIVISION_POR_CERO);
      break;
    case 4:
      if (Op2)
        ValorRetorno = fmodl (Op1, Op2);
      else
        Buzon.Error (DIVISION_POR_CERO);
      break;
    case 5:
      ValorRetorno = powl (Op1, Op2);
      break;
    };
  if (Buzon.GetHuboError ())
    return;
  Token *TokenRetorno = ConsigueToken (ValorRetorno);
  if (Buzon.GetHuboError ())
    return;
  if (FueraDeRango (TokenRetorno))
    return;

  delete Desapila (Pila);
  delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Restar (Token ** Pila)
{
  NumericosBinarios (Pila, 1);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Multiplicar (Token ** Pila)
{
  NumericosBinarios (Pila, 2);
  return;
}


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Dividir (Token ** Pila)
{
  NumericosBinarios (Pila, 3);
  return;
}


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Modulo (Token ** Pila)
{
  NumericosBinarios (Pila, 4);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

inline void
Elevar (Token ** Pila)
{
  NumericosBinarios (Pila, 5);
  return;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Substring (Token ** Pila, int NArgumentos)
{
  Token *TokCadena, *TokInicio, *TokHasta;
  Token *YoRecorro = *Pila;

  char *RetornaValorDeTokenEnCadena (Token *);  //Debug

  if (NArgumentos != 3)
    {
      Buzon.SetIdentificadorAsociado ("SUBSTRING");
      if (NArgumentos > 3)
        Buzon.Error (MUCHOS_ARGUMENTOS);
      else
        Buzon.Error (POCOS_ARGUMENTOS);
      return;
    }

  TokHasta = ObtieneTokenPila (YoRecorro);
  if (Buzon.GetHuboError ())
    {
      return;
    }
  YoRecorro = YoRecorro->GetSig ();

  TokInicio = ObtieneTokenPila (YoRecorro);
  if (Buzon.GetHuboError ())
    {
      BorrarToken (TokHasta);
      return;
    }
  YoRecorro = YoRecorro->GetSig ();


  TokCadena = ObtieneTokenPila (YoRecorro);
  if (Buzon.GetHuboError ())
    {
      BorrarToken (TokInicio);
      BorrarToken (TokHasta);
      return;
    }

  int Error = 0;

  // Se revisa que los argumentos san valores adecuados

  if (TokCadena->GetTipoDato () != STRING)
    {
      Error = 2;
    }
  if (TokInicio->GetTipoDato () != REAL && !Error)
    {
      Error = 2;
    }
  else if (!Error)
    {
      long double ValorReal = TokInicio->GetDatoReal ();
      if (ValorReal < 1.0L || ValorReal > 32000.0L ||
          (floorl (ValorReal) != ValorReal))
        {
          Error = 1;
        }
    }
  if (TokHasta->GetTipoDato () != REAL && !Error)
    {
      Error = 2;
    }
  else if (!Error)
    {
      long double ValorReal = TokHasta->GetDatoReal ();
      if (ValorReal < 0.0L || ValorReal > 32000.0L ||
          (floorl (ValorReal) != ValorReal))
        {
          Error = 1;
        }
    }

  if (Error)
    {
      Buzon.SetIdentificadorAsociado ("SUBSTRING");
      if (Error == 2)
        Buzon.Error (OPERADOR_ILEGAL);
      else
        Buzon.Error (LLAMADO_DE_FUNCION_NO_VALIDO);
      BorrarToken (TokCadena);
      BorrarToken (TokInicio);
      BorrarToken (TokHasta);
      //Aquiretorna
      return;
    }
  char *Cadena = TokCadena->GetDatoStr ();
  int Hasta = (int) TokHasta->GetDatoReal ();
  int Inicio = (int) TokInicio->GetDatoReal ();
  BorrarToken (TokInicio);
  BorrarToken (TokHasta);

  int Largo = strlen (Cadena);
  int NAux = 0;
  char *Aux = new char[Largo + 1];

  for (register int Cont = Inicio - 1;
       (Cont < Inicio + Hasta - 1) && (Cont < Largo); ++Cont)
    Aux[NAux++] = Cadena[Cont];
  Aux[NAux] = 0;

  Token *TokenRetorno = new Token (Aux, OPERANDO, CONSTANTE);
  delete[]Aux;

  BorrarToken (TokCadena);
  for (int i = 0; i < 4; ++i)
    delete Desapila (Pila);
  Apila (Pila, TokenRetorno);
  return;
}
