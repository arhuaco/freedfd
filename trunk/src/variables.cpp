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

#include <dfd.h>
#include <token.h>
#include <errores.h>
#include <variables.h>

#include <string.h>

void
CampoVariable::AsignarValor (Token * UnToken, unsigned *Indices,
                             int Dim, char *Id)
{

  int ResultadoInsercion = 1;
  if (UnToken->GetTipoDato () != Tipo)
    {
      Buzon.SetIdentificadorAsociado (Id);
      Buzon.Error (LOS_TIPOS_NO_COINCIDEN);
      ResultadoInsercion = 0;
    }
  if (Dim != Dimension)
    {
      if (!Dimension)
        {
          Buzon.SetIdentificadorAsociado (Id);
          Buzon.Error (ID_NO_ES_ARREGLO);
        }
      else
        {
          Buzon.SetIdentificadorAsociado (Id);
          Buzon.Error (DIMENSION_NO_VALIDA);
        }
      ResultadoInsercion = 0;
    }
  if (!ResultadoInsercion)
    return;

  // No se encontro errores de tipo ni de dimension

  Cajita *AsignarEn = Inicio;

  for (; AsignarEn; AsignarEn = AsignarEn->Sig)
    {
      bool EsEste = true;
      for (int i = 0; i < Dimension; ++i)
        if (Indices[i] != AsignarEn->Indices[i])
          {
            EsEste = false;
            break;
          }
      if (EsEste)
        break;
    }
  //No se encontro la posicion
  if (!AsignarEn)
    {
      AsignarEn = new Cajita;
      if (Dimension)
        {
          AsignarEn->Indices = new unsigned int[Dimension];
        }
      AsignarEn->Sig = Inicio;
      Inicio = AsignarEn;
      if (Tipo == STRING)
        {
          AsignarEn->DatoStr = new char[strlen (UnToken->GetDatoStr ()) + 1];
          strcpy (AsignarEn->DatoStr, UnToken->GetDatoStr ());
        }
      else
        AsignarEn->DatoReal = UnToken->GetDatoReal ();
      for (int i = 0; i < Dimension; ++i)
        AsignarEn->Indices[i] = Indices[i];
      return;
    }
  //      Se va a asignar en una posicion ya existente
  if (Tipo == STRING)
    {
      if (AsignarEn->DatoStr)
        delete[]AsignarEn->DatoStr;
      AsignarEn->DatoStr = new char[strlen (UnToken->GetDatoStr ()) + 1];
      strcpy (AsignarEn->DatoStr, UnToken->GetDatoStr ());
    }
  else
    AsignarEn->DatoReal = UnToken->GetDatoReal ();

  return;
}


CampoVariable::CampoVariable (Token * UnToken, unsigned *Indices, int Dim,
                              char *Id)
{
  ContadorCampoVariable++;      //Depuracion
  Inicio = 0;
  Dimension = Dim;
  Tipo = UnToken->GetTipoDato ();
  AsignarValor (UnToken, Indices, Dim, Id);
  // Aqui no se necesita saber el valor de retorno de AsignarValor
}

 // Esta funcion accede a un elemento del arreglo cuya posicion esta
 // especificada por el parametro Indices. Devuelve un puntero a un Token
 // con el valor del elemento.

Token *
CampoVariable::Leer (unsigned *Indices, int Dim, char *Id)
{
  Cajita *LeerDe = Inicio;
  if (Dim != Dimension)
    {
      Buzon.SetIdentificadorAsociado (Id);
      if (Dimension)
        Buzon.Error (DIMENSION_NO_VALIDA);
      else
        Buzon.Error (ID_NO_ES_ARREGLO);
      return 0;
    }
  if (Indices)
    for (; LeerDe; LeerDe = LeerDe->Sig)
      {
        bool EsEste = true;
        for (int i = 0; i < Dimension; ++i)
          if (Indices[i] != LeerDe->Indices[i])
            {
              EsEste = false;
              break;
            }
        if (EsEste)
          break;
      }
  Token *Nuevo;
  if (Tipo == STRING)
    {
      const char *CadenaParaToken = (LeerDe) ? LeerDe->DatoStr : "\x01 BASURA \x01";
      Nuevo = new Token (CadenaParaToken, OPERANDO, CONSTANTE);
      if (Buzon.GetHuboError () == true)
        {
          delete Nuevo;
          return 0;
        }
      return Nuevo;
    }
  if (Tipo == REAL)
    {
      long double RealParaToken = (LeerDe) ? LeerDe->DatoReal : 99999.99999L;
      Nuevo = new Token (RealParaToken, OPERANDO, CONSTANTE);
      if (Buzon.GetHuboError () == true)
        {
          delete Nuevo;
          return 0;
        }
      return Nuevo;
    }
  bool LogicoParaToken = (LeerDe) ? LeerDe->DatoLogico : false;
  Nuevo = new Token (LogicoParaToken, OPERANDO, CONSTANTE);
  if (Buzon.GetHuboError () == true)
    {
      delete Nuevo;
      return 0;
    }
  return Nuevo;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Destruye el vector disperso y la memoria reservada por cada elemento
// en caso de Tipo==STRING

CampoVariable::~CampoVariable ()
{
  ContadorCampoVariable--;      //Depuracion

  while (Inicio)
    {
      Cajita *Aux = Inicio->Sig;
      if (Tipo == STRING)
        if (Inicio->DatoStr)
          delete[]Inicio->DatoStr;
      if (Dimension)
        if (Inicio->Indices)
          delete[]Inicio->Indices;
      delete Inicio;
      Inicio = Aux;
    }
}

Variable::Variable (char *Id, Token * UnToken, Variable * Vengo,
                    unsigned *Indices, int Dim)
{
  ContadorVariable++;
  Identificador = new char[strlen (Id) + 1];
  strcpy (Identificador, Id);
  Campo = new CampoVariable (UnToken, Indices, Dim, Id);
  if (Buzon.GetHuboError () == true)
    {
      delete Campo;
      delete[]Identificador;
      return;
    }
  Tipo = UnToken->GetTipoDato ();
  FU = true;
  FP = false;
  VengoDe = Vengo;

  if (!VengoDe)
    PrimerPadre = this;
  else
    PrimerPadre = VengoDe->PrimerPadre;
}

Variable::Variable (char *Id, Variable * Vengo)
{
  ContadorVariable++;
  Identificador = new char[strlen (Id) + 1];
  strcpy (Identificador, Id);
  FU = false;
  FP = false;
  VengoDe = Vengo;

  if (!VengoDe)
    PrimerPadre = this;
  else
    PrimerPadre = VengoDe->PrimerPadre;

}

Variable::~Variable()
{
  ContadorVariable--;
  delete[]Identificador;
}

void
Variable::AsignarValor (Token * UnToken, unsigned *Indices, int Dim)
{
  void ActualizarVariables (Variable *);
  if (FU)
    Campo->AsignarValor (UnToken, Indices, Dim, Identificador);
  else
    {
      Campo = new CampoVariable (UnToken, Indices, Dim, Identificador);
      FU = true;
      Tipo = Campo->GetTipo ();
      if (FP)
        ActualizarVariables (this);
    }

  return;
}


Token *
Variable::Leer (unsigned *Indices, int Dim)
{
  if (!FU)
    {
      Buzon.SetIdentificadorAsociado (Identificador);
      Buzon.Error (VARIABLE_NO_INICIALIZADA);
      return 0;
    }
  return Campo->Leer (Indices, Dim, Identificador);
}

