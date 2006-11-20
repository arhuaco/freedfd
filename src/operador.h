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


#ifndef __CAMPOS__H__
#define __CAMPOS__H__

#include <dfd.h>

class NodoOperador;

class Operadores
{
  NodoOperador *InicioOperadores;
  void Insertar (char *Str, int UnPdp, int UnPfp, AlcanceOperador UnAlcance);
  void Insertar (char *Str);

  struct NodoFuncion
  {
    char *Dato;
    NodoFuncion *Sig;
  } *InicioFunciones;


public:
    Operadores ();
   ~Operadores ();
  NodoOperador *Buscar (char *Operador);
  bool BuscarFuncion (char *Funcion);

};

#endif
