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


#include <dfd.h>
#include <dfd-wrappers.h>
#include <errores.h>
#include <messages.h>
#include <operador.h>

#include <stdio.h>
#include <string.h>

void BuzonDeErrores::Vacear ()
{
  Mensaje *Aux;

  while (Inicio)
    {
      Aux    = Inicio;
      Inicio = Inicio->Sig;

      delete[]  Aux->Info;
      delete    Aux;
    }

  HuboError = false;

  if (IdentificadorAsociado)
    delete []IdentificadorAsociado;

  IdentificadorAsociado = NULL;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// TODO: ¿El último parámetro no sirve para algo? Si no, eliminarlo.

void
BuzonDeErrores::IntroducirError (const char *msg, TipoError UnError)
{
  Mensaje *NuevoError = new Mensaje;

  NuevoError->Tipo = UnError;

  int len;

  if (IdentificadorAsociado)
    len = strlen (msg) + strlen(IdentificadorAsociado) + 3;
  else
    len = strlen (msg) + 3;

  NuevoError->Info = new char[len];
  strcpy(NuevoError->Info, IdentificadorAsociado);
  strcat(NuevoError->Info, ": ");
  strcat(NuevoError->Info, msg);

  NuevoError->Sig = Inicio;
  Inicio = NuevoError;

  HuboError = true;
}

bool
BuzonDeErrores::GetHuboError ()
{
  return HuboError;
}

char *
BuzonDeErrores::GetIdentificadorAsociado ()
{
  return IdentificadorAsociado;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
BuzonDeErrores::SetIdentificadorAsociado (const char *str)
{
  if (IdentificadorAsociado)
    delete []IdentificadorAsociado;

  IdentificadorAsociado = dfd_strdup(str);
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

BuzonDeErrores::~BuzonDeErrores ()
{
  Mensaje *Aux;

  while (Inicio)
    {
      Aux = Inicio;
      Inicio = Inicio->Sig;

      delete[] Aux->Info;
      delete   Aux;
    }

  if (IdentificadorAsociado)
    delete[] IdentificadorAsociado;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

char *
BuzonDeErrores::GetError ()
{
  ASSERT(0); // ¿Por qué está este assert aca?

  if (!Inicio)
     IntroducirError (TXT_ERROR_INDEFINIDO " en BuzonDeErrores::GetError", ERROR_INDEFINIDO);

  // TODO: ¿Quén usa este valor retornado?
  // ¿Por qué se le da una nueva cadena? ¿Causa esto un memory leak?
  // Creo que es mejor usar la clase string aca y no hacer esta
  // copia de memoria.

  char *Retorno = dfd_strdup(Inicio->Info);

  Vacear ();

  return Retorno;
}

void
BuzonDeErrores::Mostrar ()
{

/*
 * Originalmente esta rutina llamaba un cuadro de diálogo. Eso
 * es un error. Debe existir un callback para esto, ya que
 * en esta clase tan básica no debería estar la presentación.
 * Tener un callback permite que Dfd corra en un
 * modo en el que no se necesiten los gráficos.
 */

  if (!Inicio)
    return;

  fprintf(stderr, "%s error: %s\n", program_name, Inicio->Info);

  Vacear ();

  HuboError = false;

#if 0
  Note que se diferencia entre Edición y Ejecución para mostrar el error.
  if (Estado.Accion == EDICION)
    VentanaPrincipal->MessageBox (Inicio->Info,
				  "Error de revisi›n", MB_ICONSTOP);
  else
    VentanaPrincipal->MessageBox (Inicio->Info,
				  "Error en ejecuci›n", MB_ICONSTOP);
#endif

}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
BuzonDeErrores::Error (TipoError UnError)
{

// TODO:
//      * -- Quitar los dos puntos iniciales en los mensajes 
//      * -- Mejorar los mensajes 
//      * -- Usar o quitar el punto final en todos
//      * -- Usar 'no válido' en lugar de 'ilegal'. En los estándares GNU
//           se especifica que el uso de 'ilegal' debe ser sólo para leyes.
//           ilegal para cosas que tienen que ver con leyes.

  switch (UnError)
    {

    case DESBORDAMIENTO:
      IntroducirError (TXT_DESBORDAMIENTO, DESBORDAMIENTO);
      break;

    case DESBORDAMIENTO_NEGATIVO:
      IntroducirError (TXT_DESBORDAMIENTO_NEGATIVO,  DESBORDAMIENTO_NEGATIVO);
      break;

    case LOS_TIPOS_NO_COINCIDEN:
      IntroducirError (TXT_LOS_TIPOS_NO_COINCIDEN, LOS_TIPOS_NO_COINCIDEN);
      break;

    case ID_NO_ES_ARREGLO:
      IntroducirError (TXT_ID_NO_ES_ARREGLO, ID_NO_ES_ARREGLO);
      break;

    case DIMENSION_NO_VALIDA:
      IntroducirError (TXT_DIMENSION_NO_VALIDA, DIMENSION_NO_VALIDA);
      break;

    case CONSTANTE_NUMERICA_ILEGAL:
      IntroducirError (TXT_CONSTANTE_NUMERICA_ILEGAL, CONSTANTE_NUMERICA_ILEGAL);
      break;

    case NOMBRE_VARIABLE_INCORRECTO:
      IntroducirError (TXT_NOMBRE_VARIABLE_INCORRECTO, NOMBRE_VARIABLE_INCORRECTO);
      break;

    case FUNCION_REQUIERE_PARAMETRO:
      IntroducirError (TXT_FUNCION_REQUIERE_PARAMETRO, FUNCION_REQUIERE_PARAMETRO);
      break;

    case FALTA_OPERADOR:
      IntroducirError (TXT_FALTA_OPERADOR, FALTA_OPERADOR);
      break;

    case CADENA_INDETERMINADA:
      IntroducirError (TXT_CADENA_INDETERMINADA,  CADENA_INDETERMINADA);
      break;

    case CONSTANTE_LOGICA_ILEGAL:
      IntroducirError (TXT_CONSTANTE_LOGICA_ILEGAL, CONSTANTE_LOGICA_ILEGAL);
      break;

    case OPERADOR_LOGICO_ILEGAL:
      IntroducirError (TXT_OPERADOR_LOGICO_ILEGAL, OPERADOR_LOGICO_ILEGAL);
      break;

    case FALTA_OPERANDO:
      IntroducirError (TXT_FALTA_OPERANDO, FALTA_OPERANDO);
      break;

    case OPERADOR_ILEGAL:
      IntroducirError (TXT_OPERADOR_ILEGAL, OPERADOR_ILEGAL);
      break;

    case CARACTER_ILEGAL:
      IntroducirError (TXT_CARACTER_ILEGAL, CARACTER_ILEGAL);
      break;

    case CERRADO_PARENTESIS_NO_ABIERTO:
      IntroducirError (TXT_CERRADO_PARENTESIS_NO_ABIERTO, CERRADO_PARENTESIS_NO_ABIERTO);
      break;

    case ABIERTO_PARENTESIS_NO_CERRADO:
      IntroducirError (TXT_ABIERTO_PARENTESIS_NO_CERRADO, ABIERTO_PARENTESIS_NO_CERRADO);
      break;

    case LLAMADO_DE_FUNCION_NO_VALIDO:
      IntroducirError (TXT_LLAMADO_DE_FUNCION_NO_VALIDO,   LLAMADO_DE_FUNCION_NO_VALIDO);
      break;

    case VARIABLE_NO_INICIALIZADA:
      IntroducirError (TXT_VARIABLE_NO_INICIALIZADA, VARIABLE_NO_INICIALIZADA);
      break;

    case VARIABLE_NO_EXISTE:
      IntroducirError (TXT_VARIABLE_NO_EXISTE, VARIABLE_NO_EXISTE);
      break;

    case DIVISION_POR_CERO:
      IntroducirError (TXT_DIVISION_POR_CERO, DIVISION_POR_CERO);
      break;

    case INDICES:
      IntroducirError (TXT_INDICES, INDICES);
      break;

    case ILEGAL_COMA_O_CADENA_VACIA:
      IntroducirError (TXT_ILEGAL_COMA_O_CADENA_VACIA, ILEGAL_COMA_O_CADENA_VACIA);
      break;

    case SOLO_VARIABLES:
      IntroducirError (TXT_SOLO_VARIABLES, SOLO_VARIABLES);
      break;


    case NO_INDICES:
      IntroducirError (TXT_NO_INDICES, NO_INDICES);
      break;

    case TIPO_DEBE_SER_LOGICO:
      IntroducirError (TXT_TIPO_DEBE_SER_LOGICO, TIPO_DEBE_SER_LOGICO);
      break;

    case VINICIAL_CONTADOR:
      IntroducirError (TXT_VINICIAL_CONTADOR,  VINICIAL_CONTADOR);
      break;

    case VFINAL_CONTADOR:
      IntroducirError (TXT_VFINAL_CONTADOR,  VFINAL_CONTADOR);
      break;

    case INCREMENTO_CONSTANTE:
      IntroducirError (TXT_INCREMENTO_CONSTANTE,  INCREMENTO_CONSTANTE);
      break;

    case CONTADOR_REAL:
      IntroducirError (TXT_CONTADOR_REAL, CONTADOR_REAL);
      break;

    case LIMITE_REAL:
      IntroducirError (TXT_LIMITE_REAL, LIMITE_REAL);
      break;

    case PARAMETRO_NO_VARIABLE:
      IntroducirError (TXT_PARAMETRO_NO_VARIABLE,  PARAMETRO_NO_VARIABLE);
      break;

    case ILEGAL_NOMBRE_SUBPROGRAMA:
      IntroducirError (TXT_ILEGAL_NOMBRE_SUBPROGRAMA, ILEGAL_NOMBRE_SUBPROGRAMA);
      break;

    case PARAMETRO_REPETIDO:
      IntroducirError (TXT_PARAMETRO_REPETIDO, PARAMETRO_REPETIDO);
      break;

    case SUBPROGRAMA_NO_EXISTE:
      IntroducirError (TXT_SUBPROGRAMA_NO_EXISTE, SUBPROGRAMA_NO_EXISTE);
      break;
    case SUBPROGRAMA_REPETIDO:
      IntroducirError (TXT_SUBPROGRAMA_REPETIDO, SUBPROGRAMA_REPETIDO);
      break;

    case MUCHOS_ARGUMENTOS:
      IntroducirError (TXT_MUCHOS_ARGUMENTOS, MUCHOS_ARGUMENTOS);
      break;

    case POCOS_ARGUMENTOS:
      IntroducirError (TXT_POCOS_ARGUMENTOS, POCOS_ARGUMENTOS);
      break;

    default:
      IntroducirError (TXT_ERROR_INDEFINIDO, ERROR_INDEFINIDO);
      break;
    }
}

