/*
  FreeDFD: Editor e intérprete de diagramas de flujo.
  Copyright (C) 1997-2006
  http://freedfd.freaks-unidos.net/

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

#include <operador.h>

#include<stdlib.h>
#include<string.h>


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

NodoOperador::NodoOperador (const char *Nombre, int UnPdp, int UnPfp,
			    AlcanceOperador UnAlcance)
{
  Dato = Nombre;
  Pdp = UnPdp;
  Pfp = UnPfp;
  Alcance = UnAlcance;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



Operadores::Operadores ()
{
  InicioOperadores = 0;
  InicioFunciones = 0;

  Insertar ("^", 80, 81, BINARIO);


  Insertar ("SIN", 80, 81, UNITARIO);

  Insertar ("COS", 80, 81, UNITARIO);

  Insertar ("TAN", 80, 81, UNITARIO);

  Insertar ("NOT", 80, 81, UNITARIO);

  Insertar ("SINH", 80, 81, UNITARIO);

  Insertar ("COSH", 80, 81, UNITARIO);

  Insertar ("TANH", 80, 81, UNITARIO);

  Insertar ("ASIN", 80, 81, UNITARIO);

  Insertar ("ACOS", 80, 81, UNITARIO);

  Insertar ("ATAN", 80, 81, UNITARIO);

  Insertar ("TRUNC", 80, 81, UNITARIO);

  Insertar ("ROUND", 80, 81, UNITARIO);

  Insertar ("ABS", 80, 81, UNITARIO);

  Insertar ("SQRT", 80, 81, UNITARIO);

  Insertar ("RANDOM", 80, 81, UNITARIO);

  Insertar ("LEN", 80, 81, UNITARIO);

  Insertar ("LN", 80, 81, UNITARIO);

  Insertar ("LOG", 80, 81, UNITARIO);

  Insertar ("EXP", 80, 81, UNITARIO);



  Insertar ("MOD", 70, 70, BINARIO);

  Insertar ("*", 70, 70, BINARIO);

  Insertar ("/", 70, 70, BINARIO);

  Insertar ("+", 60, 60, BINARIO);

  Insertar ("-", 60, 60, BINARIO);

  Insertar ("<", 50, 50, BINARIO);

  Insertar (">", 50, 50, BINARIO);

  Insertar (">=", 50, 50, BINARIO);

  Insertar ("<=", 50, 50, BINARIO);

  Insertar ("=", 40, 40, BINARIO);

  Insertar ("!=", 40, 40, BINARIO);

  Insertar ("AND", 20, 20, BINARIO);

  Insertar ("NAND", 20, 20, BINARIO);

  Insertar ("OR", 10, 10, BINARIO);

  Insertar ("NOR", 20, 20, BINARIO);

  Insertar ("XOR", 10, 10, BINARIO);

  Insertar ("XNOR", 10, 10, BINARIO);

  Insertar ("(", 0, 100, BINARIO);

  Insertar (")", -1, -1, BINARIO);

  Insertar (",", -1, -1, BINARIO);

  Insertar ("SUBSTRING");

}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Operadores::~Operadores ()
{

  NodoOperador *Aux;
  while (InicioOperadores)
  {
    Aux = InicioOperadores;
    InicioOperadores = InicioOperadores->GetSig ();
      delete Aux;
  }
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

NodoOperador *
Operadores::Buscar (const char *Operador)
{
  NodoOperador *Aux = InicioOperadores;
  for (; Aux; Aux = Aux->GetSig ())
    if (!strcasecmp (Aux->GetDato (), Operador))
      return Aux;
  return 0;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bool
Operadores::BuscarFuncion (const char *Str)
{
  NodoFuncion *Aux = InicioFunciones;
  for (; Aux; Aux = Aux->Sig)
    if (!strcasecmp (Aux->Dato, Str))
      return true;
  return false;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


void
Operadores::Insertar (const char *Str, int UnPdp, int UnPfp,
		      AlcanceOperador UnAlcance)
{
  NodoOperador *NuevoNodoOperador =
    new NodoOperador (Str, UnPdp, UnPfp, UnAlcance);
  NuevoNodoOperador->SetSig (InicioOperadores);
  InicioOperadores = NuevoNodoOperador;
}


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Operadores::Insertar (const char *Str)
{
  NodoFuncion *Nuevo = new NodoFuncion;
  Nuevo->Dato = Str;
  Nuevo->Sig = InicioFunciones;
  InicioFunciones = Nuevo;
}

