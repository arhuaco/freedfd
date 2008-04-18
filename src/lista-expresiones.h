#ifndef __LISTA__EXPRESIONES__H__
#define __LISTA__EXPRESIONES__H__

#include <token.h>

class ListaExpresiones
{

  struct Nodo
  {
    Token *Expresion;
    Nodo *Sig;
      Nodo ()
    {
      ContadorNodoListaExpresiones++;
    }
     ~Nodo ()
    {
      ContadorNodoListaExpresiones--;
    }

  } *Inicio, *Iterador, *Ultimo;

  int NItems;

public:

  ListaExpresiones ()
  {
    Inicio = 0;
    NItems = 0;
  }
  void GetListaExpresiones (char *);
  int GetNItems ()
  {
    return NItems;
  }
  void Reset ()
  {
    Iterador = Inicio;
  }
  Token *Itera ()
  {
    Token *Retorno = Iterador->Expresion;
    Iterador = Iterador->Sig;
    return Retorno;
  }
  void Insertar (Token *);
  void Vacear ();
  void OE_SubPrograma_RevisarParametros ();
};

#endif
