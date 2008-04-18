#include <dfd.h>
#include <errores.h>
#include <lista-expresiones.h>
#include <token.h>
#include <tokeniza.h>

#include <string.h>

void
ListaExpresiones::OE_SubPrograma_RevisarParametros ()
{
  Nodo *p1, *p2;

  for (p1 = Inicio; p1; p1 = p1->Sig)
    {
      Token *Aux = p1->Expresion;
      if ((Aux->GetTipoAlmacenamiento () != VARIABLE) || (Aux->GetSig ()))
        {
          Buzon.Error (PARAMETRO_ILEGAL);
          return;
        }
    }

  for (p1 = Inicio; p1; p1 = p1->Sig)
    {
      Token *q1 = p1->Expresion;
      for (p2 = p1->Sig; p2; p2 = p2->Sig)
        {
          Token *q2 = p2->Expresion;
          if (!strcasecmp (q1->GetDatoStr (), q2->GetDatoStr ()))
            {
              Buzon.SetIdentificadorAsociado (q2->GetDatoStr ());
              Buzon.Error (PARAMETRO_REPETIDO);
              return;
            }
        }
    }
  return;
}


void
ListaExpresiones::Insertar (Token * t)
{
  Nodo *Nuevo = new Nodo;
  Nuevo->Expresion = t;
  Nuevo->Sig = 0;
  if (!Inicio)
    Inicio = Nuevo;
  else
    Ultimo->Sig = Nuevo;
  Ultimo = Nuevo;
  Iterador = Inicio;
  ++NItems;
}

void
ListaExpresiones::Vacear ()
{
  while (Inicio)
    {
      Nodo *Temp = Inicio;
      Inicio = Inicio->Sig;
      LiberarListaToken (Temp->Expresion);
      delete Temp;
    }
  NItems = 0;
}


void
ListaExpresiones::GetListaExpresiones (char *Str)
{
  if (!Str)
    {
      Buzon.Error (ILEGAL_COMA_O_CADENA_VACIA);
      return;
    }


  int Parent = 0;
  int IniChar = 0;
  int Largo = strlen (Str);
  int ActualChar;
  int EsCadena = 0;
  for (ActualChar = 0; ActualChar <= Largo; ++ActualChar)
    {
      char &Casilla = Str[ActualChar];
      if (Casilla == '(' && !EsCadena)
        ++Parent;
      else if (Casilla == ')' && !EsCadena)
        --Parent;
      else if (Casilla == '\'')
        EsCadena = 1 - EsCadena;
      else if (Casilla == 0 || (Casilla == ',' && !Parent && !EsCadena))
        {
          char Temp = Casilla;
          Casilla = 0;
          //VentanaPrincipal->MessageBox( Str+IniChar,"" );
          Token *t = GetPostfijo (Str + IniChar);
          Casilla = Temp;
          IniChar = 1 + ActualChar;
          if (Buzon.GetHuboError ())
            return;
          if (!t)
            {
              // Uso ilegal de la coma o cadena vacía
              // Un error pa'l buzón
              Buzon.Error (ILEGAL_COMA_O_CADENA_VACIA);
              return;
            }
          Insertar (t);
        }
    }
  return;
}
