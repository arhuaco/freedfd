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

/* Esta funcion accesa al valor de un arreglo cuyos
 * nombre y subindices se encuentran en la pila que
 * se recibe como parametro. Por ejemplo, si se va a
 * evaluar a(1,2,3), la pila contendrá:
 *
 * Antes de llamar esta función, la pila tendría:
 * ... 'a' 1 2 3 '$3' (tope de la pila)
 *   y al llamarla tendría:
 * .... 'a' 1 2 3' (tope de la pila) (int Dim valdrá 3 en este caso)
 *
 * Después de llamara, tendrá el valor que
 * está en esa casilla del vector, o un valor
 * predeterminado si no había nada.
 *
 */

#ifndef __ARREGLOS_CPP
#define __ARREGLOS_CPP

#include <token.h>
#include <errores.h>


#include <math.h>

void
AccesoArreglo (Token ** Pila, int Dim)
{
  unsigned *Indices = new unsigned[Dim];
  Token **TokenIndice = new Token *[Dim];
  Token *YoRecorro = *Pila;

  // Se guarda la direccion de los Token's que serviran de indices en
  // el vector de apuntadores TokenIndice
  for (int i = 0; i < Dim; ++i)
    {
      TokenIndice[i] = ObtieneTokenPila (YoRecorro);
      if (Buzon.GetHuboError ())
        {
          for (int j = 0; j < i; ++j)
            BorrarToken (TokenIndice[j]);
          delete[]TokenIndice;
          delete[]Indices;
          return;
        }
      YoRecorro = YoRecorro->GetSig ();
    }
  bool Error = false;

  // Se revisa que los indices san valores adecuados
  for (int i = 0; i < Dim; ++i)
    {
      if (TokenIndice[i]->GetTipoDato () != REAL)
        {
          Error = true;
          break;
        }
      long double ValorReal = TokenIndice[i]->GetDatoReal ();
      if (ValorReal < 0.0L || ValorReal > 65535.0L ||
          (floorl (ValorReal) != ValorReal))
        {
          Error = true;
          break;
        }
      Indices[Dim - 1 - i] = (unsigned) ValorReal;
    }

  if (Error)
    {
      Buzon.SetIdentificadorAsociado (YoRecorro->GetDatoStr ());
      Buzon.Error (INDICES);
      for (int j = 0; j < Dim; ++j)
        BorrarToken (TokenIndice[j]);
      delete[]TokenIndice;
      delete[]Indices;
      return;
    }

  Token *TokenRetorno = PilaDeTablas.Leer (YoRecorro->GetDatoStr (),
                                           Indices, Dim);
  for (int i = 0; i < Dim; ++i)
    BorrarToken (TokenIndice[i]);

  delete[]Indices;
  delete[]TokenIndice;

  if (Buzon.GetHuboError ())
    return;

  for (int i = 0; i < Dim + 1; ++i)
    delete Desapila (Pila);

  Apila (Pila, TokenRetorno);

  return;
}

#endif
