/*
    FreeDFD: Editor e intérprete de diagramas de flujo.
    Copyright (C) 1997-2006

    Nelson Castillo Izquierdo <arhuaco@freaks-unidos.net>
    Eduardo Daza Castillo <eduardodazacastillo@gmail.com>
    Fabián Cárdenas Varela <f_cardenas@yahoo.com>
                                    
    **********

    Necesitamos ayuda para terminar la nueva versión, que funcionará dentro
    de GNU/Linux y dentro de Windows, y que incluirá múltiples mejoras.
    Visítenos en:

    http://freedfd.freaks-unidos.net


 Español:

    Este programa es software libre. Puede redistribuirlo y/o modificarlo
    bajo los términos de la Licencia Pública General de GNU según es
    publicada por la Free Software Foundation, bien de la versión 2 de dicha
    Licencia o bien (según su elección) de cualquier versión posterior.

    Este programa se distribuye con la esperanza de que sea útil, pero SIN
    NINGUNA GARANTÍA, incluso sin la garantía MERCANTIL implícita o sin
    garantizar la CONVENIENCIA PARA UN PROPÓSITO PARTICULAR. Véase la
    Licencia Pública General de GNU para más detalles.

    Debería haber recibido una copia de la Licencia Pública General junto
    con este programa. Si no ha sido así, escriba a la Free Software
    Foundation, Inc., en 675 Mass Ave, Cambridge, MA 02139, EEUU.

 English:

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


#ifndef __ERRORES_CPP
#define __ERRORES_CPP


 class BuzonDeErrores{

	  struct Mensaje{
		 char *Info;
		 TipoError Tipo;
		 Mensaje *Sig;
	  }*Inicio;

	  char *IdentificadorAsociado;
	  bool HuboError;
	  void IntroducirError( char *Id , TipoError UnError, int Asociado= 1 );

	public:

	  BuzonDeErrores(){
		 Inicio=0; IdentificadorAsociado= 0;
		 HuboError= false;
	  }
	  void Error(TipoError UnError);
	  bool GetHuboError(){ return HuboError; }
	  char *GetIdentificadorAsociado(){ return IdentificadorAsociado; }
	  char *GetError();
	  void SetIdentificadorAsociado( char  *Id );
	  void Mostrar();
	  void Vacear();
	  ~BuzonDeErrores();

 }Buzon;


 void BuzonDeErrores::Vacear(){
	Mensaje *Aux;
	while( Inicio ){
	  Aux= Inicio;
	  Inicio=Inicio->Sig;
	  delete[] Aux->Info;
	  delete Aux;
	}
	HuboError= false;
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void BuzonDeErrores::IntroducirError( char *Id, TipoError UnError, int Asociado ){

	HuboError= true;
	Mensaje *NuevoError;
	NuevoError= new Mensaje;

	NuevoError->Tipo= UnError;
	int LargoIdentificador= (IdentificadorAsociado)?
									 strlen( IdentificadorAsociado ) : 0;
	int LargoError= strlen( Id );
	NuevoError->Sig= Inicio;
	Inicio= NuevoError;

	NuevoError->Info= new char[LargoIdentificador+LargoError +2];
	NuevoError->Info[0]= 0;
	if( Asociado )
	  strcpy( NuevoError->Info, IdentificadorAsociado );
	strcat( NuevoError->Info, Id );
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void BuzonDeErrores::SetIdentificadorAsociado(char  *Id){
		 if( IdentificadorAsociado )
			 delete[] IdentificadorAsociado;
		 IdentificadorAsociado= new char[strlen(Id)+1];
		 strcpy( IdentificadorAsociado, Id );
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
BuzonDeErrores::Error (TipoError UnError)
{

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
      IntroducirError (TXT_INDICES, INDICES, 0);
      break;

    case ILEGAL_COMA_O_CADENA_VACIA:
      IntroducirError (TXT_ILEGAL_COMA_O_CADENA_VACIA, ILEGAL_COMA_O_CADENA_VACIA, 0);
      break;

    case SOLO_VARIABLES:
      IntroducirError (TXT_SOLO_VARIABLES, SOLO_VARIABLES, 0);
      break;


    case NO_INDICES:
      IntroducirError (TXT_NO_INDICES, NO_INDICES, 0);
      break;

    case TIPO_DEBE_SER_LOGICO:
      IntroducirError (TXT_TIPO_DEBE_SER_LOGICO, TIPO_DEBE_SER_LOGICO, 0);
      break;

    case VINICIAL_CONTADOR:
      IntroducirError (TXT_VINICIAL_CONTADOR,  VINICIAL_CONTADOR, 0);
      break;

    case VFINAL_CONTADOR:
      IntroducirError (TXT_VFINAL_CONTADOR,  VFINAL_CONTADOR, 0);
      break;

    case INCREMENTO_CONSTANTE:
      IntroducirError (TXT_INCREMENTO_CONSTANTE,  INCREMENTO_CONSTANTE, 0);
      break;

    case CONTADOR_REAL:
      IntroducirError (TXT_CONTADOR_REAL, CONTADOR_REAL, 0);
      break;

    case LIMITE_REAL:
      IntroducirError (TXT_LIMITE_REAL, LIMITE_REAL, 0);
      break;

    case PARAMETRO_NO_VARIABLE:
      IntroducirError (TXT_PARAMETRO_NO_VARIABLE,  PARAMETRO_NO_VARIABLE, 0);
      break;

    case ILEGAL_NOMBRE_SUBPROGRAMA:
      IntroducirError (TXT_ILEGAL_NOMBRE_SUBPROGRAMA, ILEGAL_NOMBRE_SUBPROGRAMA, 0);
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


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 char *BuzonDeErrores::GetError(){

	if( !Inicio ){
	  char *ErrorFicticio= new char[6];
	  strcpy( ErrorFicticio, "Error" );
	  return ErrorFicticio;
   }
	char *Retorno= new char[strlen(Inicio->Info )+1];
	strcpy( Retorno, Inicio->Info );
	Vacear();
	return Retorno;

 }

 void BuzonDeErrores::Mostrar(){

	if( !Inicio )
	  return;
	HuboError= false;
	//Mensaje *Aux;
	//while( Inicio ){
	  if( Estado.Accion==EDICION )
		 VentanaPrincipal->MessageBox( Inicio->Info,
						"Error de revisión" , MB_ICONSTOP );
	  else
		 VentanaPrincipal->MessageBox( Inicio->Info,
						"Error en ejecución", MB_ICONSTOP );

	  Vacear();
	/*
	  Aux= Inicio;
	  Inicio= Inicio->Sig;
	  delete[] Aux->Info;
	  delete Aux;

	}
	*/
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 BuzonDeErrores::~BuzonDeErrores(){

	Mensaje *Aux;
	while( Inicio ){
	  Aux= Inicio;
	  Inicio= Inicio->Sig;
	  delete[] Aux->Info;
	  delete Aux;
	}
	if( IdentificadorAsociado )
	  delete[] IdentificadorAsociado;
 }

#endif
