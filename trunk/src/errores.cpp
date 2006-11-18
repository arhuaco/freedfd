#include <dfd.h>
#include <dfd-wrappers.h>
#include <errores.h>

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
BuzonDeErrores::IntroducirError (char *msg, TipoError UnError, int Asociado)
{
  Mensaje *NuevoError = new Mensaje;

  NuevoError->Tipo = UnError;

  NuevoError->Info = new char[strlen (msg) + strlen(IdentificadorAsociado) + 3];
  strcpy(NuevoError->Info, IdentificadorAsociado);
  strcat(NuevoError->Info, ": ");
  strcat(NuevoError->Info, msg);

  NuevoError->Sig = Inicio;
  Inicio = NuevoError;

  HuboError = true;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
BuzonDeErrores::SetIdentificadorAsociado (char *str)
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
  ASSERT(0);

  if (!Inicio)
     IntroducirError ("Error no definido en BuzonDeErrores::GetError", ERROR_INDEFINIDO, 0);

  // TODO: ¿Quén usa este valor retornado? 
  // ¿Por qué se le da una nueva cadena? ¿Causa esto un memory leak?
  
  char *Retorno = dfd_strdup(Inicio->Info);

  Vacear ();

  return Retorno;
}

void
BuzonDeErrores::Mostrar ()
{

  if (!Inicio)
    return;

  fprintf(stderr, "%s error: %s\n", program_name, Inicio->Info);

  Vacear ();

  HuboError = false;

#if 0

  // Código viejo
  // No debemos mezclar la presentación con la lógica del
  // intérprete. Usar un callback mejor.

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
      IntroducirError ("Desbordamiento en la operación", DESBORDAMIENTO);
      break;

    case DESBORDAMIENTO_NEGATIVO:
      IntroducirError ("Desbordamiento negativo en la operació",
		       DESBORDAMIENTO_NEGATIVO);
      break;

    case LOS_TIPOS_NO_COINCIDEN:
      IntroducirError ("Los tipos no coinciden", LOS_TIPOS_NO_COINCIDEN);
      break;

    case ID_NO_ES_ARREGLO:
      IntroducirError ("El identificador no es es un arreglo",
		       ID_NO_ES_ARREGLO);
      break;

    case DIMENSION_NO_VALIDA:
      IntroducirError ("Dimensión no válida", DIMENSION_NO_VALIDA);
      break;

    case CONSTANTE_NUMERICA_ILEGAL:
      IntroducirError ("Constante numérica ilegal",
		       CONSTANTE_NUMERICA_ILEGAL);
      break;

    case NOMBRE_VARIABLE_INCORRECTO:
      IntroducirError ("Nombre de variable incorrecto",
		       NOMBRE_VARIABLE_INCORRECTO);
      break;

    case FUNCION_REQUIERE_PARAMETRO:
      IntroducirError ("La función requiere argumentos",
		       FUNCION_REQUIERE_PARAMETRO);
      break;

    case FALTA_OPERADOR:
      IntroducirError ("Se esperaun operador", FALTA_OPERADOR);
      break;

    case CADENA_INDETERMINADA:
      IntroducirError ("Se espera ' para delimitar el fin de la cadena",
		       CADENA_INDETERMINADA);
      break;

/*	  case OPERADOR_ILEGAL_DE_CADENA:
		 IntroducirError( "No es un operador vßlido para una cadena",
								OPERADOR_ILEGAL_DE_CADENA );
		 break;
*/

    case CONSTANTE_LOGICA_ILEGAL:
      IntroducirError ("Se esperaba una constante lógica( .V. o .F. )",
		       CONSTANTE_LOGICA_ILEGAL);
      break;

    case OPERADOR_LOGICO_ILEGAL:
      IntroducirError ("Operador lógico ilegal", OPERADOR_LOGICO_ILEGAL);
      break;

    case FALTA_OPERANDO:
      IntroducirError ("Se esperaba un operando", FALTA_OPERANDO);
      break;

      /* case OPERANDO_ILEGAL:
         IntroducirError( ": El operador recibió un operando ilegal",
         OPERANDO_ILEGAL );
         break;
       */

    case OPERADOR_ILEGAL:
      IntroducirError ("Operador no válido", OPERADOR_ILEGAL);
      break;

    case CARACTER_ILEGAL:
      IntroducirError ("Caracter no válido", CARACTER_ILEGAL);
      break;

    case CERRADO_PARENTESIS_NO_ABIERTO:
      IntroducirError ("Cerrado paréntesis no abierto",
		       CERRADO_PARENTESIS_NO_ABIERTO);
      break;

    case ABIERTO_PARENTESIS_NO_CERRADO:
      IntroducirError ("Abierto paréntesis no cerrado",
		       ABIERTO_PARENTESIS_NO_CERRADO);
      break;

      /* case COMA_ILEGAL:
         IntroducirError( "Uso ilegal del operador", COMA_ILEGAL );
         break; */

/*	  case ERROR_MATEMATICO:
		 IntroducirError( "Error Matemßtico", ERROR_MATEMATICO );
		 break;*/

    case LLAMADO_DE_FUNCION_NO_VALIDO:
      IntroducirError ("Llamado de función no válido",
		       LLAMADO_DE_FUNCION_NO_VALIDO);
      break;

    case VARIABLE_NO_INICIALIZADA:
      IntroducirError ("Variable no inicializada",
		       VARIABLE_NO_INICIALIZADA);
      break;

    case VARIABLE_NO_EXISTE:
      IntroducirError ("Variable no existe", VARIABLE_NO_EXISTE);
      break;

    case DIVISION_POR_CERO:
      IntroducirError ("División por cero", DIVISION_POR_CERO);
      break;

/*	  case SUBINDICE_NO_VALIDO:
		 IntroducirError( ": Indice no vßlido", SUBINDICE_NO_VALIDO );
		 break;*/

    case INDICES:
      IntroducirError
	("Los índices deben ser enteros no negativos menores o iguales a 65535.",
	 INDICES, 0);
      break;

    case ILEGAL_COMA_O_CADENA_VACIA:
      IntroducirError ("Uso ilegal de la coma ó falta expresión",
		       ILEGAL_COMA_O_CADENA_VACIA, 0);
      break;

    case SOLO_VARIABLES:
      IntroducirError
	("La asignación de valores solo se realiza en campos variables",
	 SOLO_VARIABLES, 0);
      break;


    case NO_INDICES:
      IntroducirError ("No hay índices entre los paréntesis."
                       "Los paréntesis se usan para acceder a elementos de un arreglo.",
		       NO_INDICES, 0);
      break;

    case TIPO_DEBE_SER_LOGICO:
      IntroducirError ("La condición debe ser de tipo lógico.",
		       TIPO_DEBE_SER_LOGICO, 0);
      break;

    case VINICIAL_CONTADOR:
      IntroducirError ("Se necesita un valor inicial para el contador.",
		       VINICIAL_CONTADOR, 0);
      break;

    case VFINAL_CONTADOR:
      IntroducirError ("Se necesita un valor final para el contador.",
		       VFINAL_CONTADOR, 0);
      break;

    case INCREMENTO_CONSTANTE:
      IntroducirError ("El incremento debe ser una constante real.",
		       INCREMENTO_CONSTANTE, 0);
      break;

    case CONTADOR_REAL:
      IntroducirError ("El valor del contador debe ser de tipo real.",
		       CONTADOR_REAL, 0);
      break;

    case LIMITE_REAL:
      IntroducirError
	("El valor del límite para el contador debe ser de tipo real.",
	 LIMITE_REAL, 0);
      break;

    case PARAMETRO_ILEGAL:
      IntroducirError ("Los parámetros deben ser campos variables.",
		       PARAMETRO_ILEGAL, 0);
      break;

    case ILEGAL_NOMBRE_SUBPROGRAMA:
      IntroducirError ("Nombre no válido para un subprograma.",
		       ILEGAL_NOMBRE_SUBPROGRAMA, 0);
      break;

    case PARAMETRO_REPETIDO:
      IntroducirError ("Parámetro Repetido.", PARAMETRO_ILEGAL);
      break;

    case SUBPROGRAMA_NO_EXISTE:
      IntroducirError (": No se encontró el Subprograma.",
		       SUBPROGRAMA_NO_EXISTE);
      break;
    case SUBPROGRAMA_REPETIDO:
      IntroducirError (": Subprograma repetido.", SUBPROGRAMA_REPETIDO);
      break;

    case MUCHOS_ARGUMENTOS:
      IntroducirError (": Demasiados argumentos.", MUCHOS_ARGUMENTOS);
      break;

    case POCOS_ARGUMENTOS:
      IntroducirError (": Muy pocos argumentos.", POCOS_ARGUMENTOS);
      break;

    default:
      IntroducirError (": ERROR NO DEFINIDO", ERROR_INDEFINIDO);
      break;
    }
}

