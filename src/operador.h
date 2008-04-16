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

#ifndef __OPERADOR__H__
#define __OPERADOR__H__

enum AlcanceOperador { UNITARIO, BINARIO, UNITARIOBINARIO, ARRAY };

class NodoOperador
{
  const char *Dato;
  int Pdp;  /* prioridad dentro de la pila */
  int Pfp;  /* prioridad fuera de la pila */

  NodoOperador *Sig;  /* el siguiente, para una lista */
  AlcanceOperador Alcance;  /* unitario, binario, etc. */

public:

  NodoOperador (const char *Nombre, int UnPdp, int UnPfp,
  AlcanceOperador UnAlcance);

  ~NodoOperador (){}

  NodoOperador *GetSig ()
  {
    return Sig;
  }

  void SetSig (NodoOperador * UnSig)
  {
    Sig = UnSig;
  }

  void SetAlcance (AlcanceOperador UnAlcance)
  {
    Alcance = UnAlcance;
  }

  AlcanceOperador GetAlcance ()
  {
    return Alcance;
  }

  const char *GetDato ()
  {
    return Dato;
  }

  int GetPdp ()
  {
    return Pdp;
  }

  int GetPfp ()
  {
    return Pfp;
  }
};

NodoOperador *operador_buscar (const char *Operador);
bool operador_funcion_buscar   (const char *Funcion);

#endif
