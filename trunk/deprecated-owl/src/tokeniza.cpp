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

#ifndef __TOKENIZA_CPP
#define __TOKENIZA_CPP


 /*
  Analizador- Primera pasada:
	- Obtener una lista de Token en Infijo a partir de una cadena de
	  caracteres.
	- Revisar errores basicos en la cadena.
 */


 // Funcion que determina si el caracter Ch se encuentra entre la cadena Str.
 bool EstaEn( char Ch, char *Str ){

	for( ; *Str ; ++Str )
	  if( *Str == Ch )
		 return true;
	return false;
 }

 inline int EsLetra (char Caracter){
	Caracter=(char)toupper(Caracter);
	return (Caracter>='A' && Caracter<='Z')
			  ||EstaEn(Caracter,"çÇãâêõôÃÂÊÕÔñÑüáéíóúÁÉÍÓÚàÀ");
 }
 inline int EsDigito(char Caracter){
	return (Caracter>='0')&&(Caracter<='9');
 }
 inline int EsEspacio( char Caracter ){
	 return Caracter == ' ';
 }




 // Rutina que de acuerdo a sus parametros crea un Token y lo añade a la
 // lista controlada con *Inicio y *Ultimo, los dos primeros parametros.

 void Insertar ( Token **Inicio, Token **Ultimo, bool ValorLogico,
					  TipoToken UnTipoToken, TipoAlmacenamiento UnTipoA ){

	Token *Nuevo;
	Nuevo= new Token( ValorLogico, UnTipoToken, UnTipoA );
	Nuevo->SetSig( 0 );
	if( *Inicio == 0 )
	  *Inicio= Nuevo;
	else
	  (*Ultimo)->SetSig( Nuevo );
	*Ultimo= Nuevo;
 }

 void Insertar ( Token **Inicio, Token **Ultimo, long double ValorReal,
					  TipoToken UnTipoToken, TipoAlmacenamiento UnTipoA ){

	Token *Nuevo;
	Nuevo= new Token( ValorReal, UnTipoToken, UnTipoA );
	Nuevo->SetSig( 0 );
	if( *Inicio == 0 )
	  *Inicio= Nuevo;
	else
	  (*Ultimo)->SetSig( Nuevo );
	*Ultimo= Nuevo;
 }

 void Insertar ( Token **Inicio, Token **Ultimo, char *ValorStr,
					  TipoToken UnTipoToken, TipoAlmacenamiento UnTipoA,
					  int Pdp= 0, int Pfp= 0, AlcanceOperador UnAlcance= BINARIO){

	Token *Nuevo;
	Nuevo= new Token( ValorStr, UnTipoToken, UnTipoA, Pdp, Pfp, UnAlcance );
	Nuevo->SetSig( 0 );
	if( *Inicio == 0 )
	  *Inicio= Nuevo;
	else
	  (*Ultimo)->SetSig( Nuevo );
	*Ultimo= Nuevo;
 }

 // Rutina que libera la lista de Token controlada con Inicio.
 void LiberarListaToken( Token *Inicio ){

	Token *Aux;
	while( Inicio ){
	  Aux= Inicio;
	  Inicio= Inicio->GetSig();
	  delete Aux;
	}
 }


 // Funcion que retorna una cadena dinamica cuyo contenido son los n primeros
 // caracteres de la cadena Fuente.
 char *Copiar( char *Fuente, int n ){
	char *NuevaCadena= new char[n+1];
	for( int i= 0 ; i<n ; ++i )
	  NuevaCadena[i]= Fuente[i];
	NuevaCadena[n]= '\0';
	return NuevaCadena;
 }

 // Funcion que recibe para revisar errores de sintaxis y formar
 // la lista de Token con la que se trabajara.
 Token *GetListaToken( char *Expresion ){

	int p,q,Caso;
	Token *Inicio= 0, *Ultimo;
	for( p= 0 ;  EstaEn(Expresion[p]," ") ; ++p );
	while ( Expresion[p]!='\0' ){
	  if( EsDigito( Expresion[p] ) )         Caso= 1;
	  else if( EsLetra( Expresion[p] ) )     Caso= 2;
	  else if( Expresion[p]=='\'' )          Caso= 3;
	  else if( Expresion[p]=='.' )           Caso= 4;
	  else if( Expresion[p]=='(' )           Caso= 5;
	  else if( Expresion[p]==')' )           Caso= 6;
	  else if( EstaEn( Expresion[p],"+-"))   Caso= 7;
	  else if( EstaEn( Expresion[p],"*/^"))  Caso= 8;
	  else if( EstaEn( Expresion[p],"><"))   Caso= 9;
	  else if( Expresion[p]=='=')            Caso= 10;
	  else if( Expresion[p]=='!')            Caso= 11;
	  else if( Expresion[p]==',')            Caso= 12;
	  else                                   Caso= 13;

	  q= p;
	  ++p;

	  switch( Caso ){

		 case 1:{    //Numero
			bool HuboPunto= false;
			for( ; EsDigito( Expresion[p] )||Expresion[p]=='.' ; ++p )
			  if( Expresion[p]=='.' )
				 if( HuboPunto )
					break;
				 else
					HuboPunto= true;
			char *Nuevo= Copiar( &Expresion[q], p-q );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			if( EsLetra( Expresion[p] ) ||
				 EstaEn( Expresion[p],"._'(" ) ){
			  char *Aux= Copiar( Nuevo, strlen( Nuevo )+2 );
			  int Tamanyo= strlen( Aux );
			  Aux[Tamanyo]= Expresion[p];
			  Aux[Tamanyo+1]='\0';
			  Buzon.SetIdentificadorAsociado( Aux );
			  Buzon.Error( CONSTANTE_NUMERICA_ILEGAL );
			  delete[] Nuevo;
			  delete[] Aux;
			  LiberarListaToken( Inicio );
			  return 0;
			}

			Insertar( &Inicio, &Ultimo, _atold( Nuevo ),OPERANDO,CONSTANTE );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  delete [] Nuevo;
			  return 0;
			}
			for( ; EsEspacio( Expresion[p] ); ++p);
			if( EstaEn( Expresion[p],"._'(" ) ){
			  char *Aux= Copiar( Nuevo, strlen( Nuevo )+2 );
			  int Tamanyo= strlen( Aux );
			  Aux[Tamanyo]= Expresion[p];
			  Aux[Tamanyo+1]='\0';
			  Buzon.SetIdentificadorAsociado( Aux );
			  Buzon.Error( CONSTANTE_NUMERICA_ILEGAL );
			  delete[] Nuevo;
			  delete[] Aux;
			  LiberarListaToken( Inicio );
			  return 0;
			}
			if( EsDigito( Expresion[p] ) ){
			  Buzon.SetIdentificadorAsociado( Nuevo );
			  Buzon.Error( FALTA_OPERADOR );
			  delete[] Nuevo;
			  LiberarListaToken( Inicio );
			  return 0;
			}
			delete[] Nuevo;
			break;
		 }

		 case 2:{       //Letra
			for( ;EsDigito( Expresion[p] )||EsLetra( Expresion[p] )||
					Expresion[p]=='_' ; ++p );
			char *Nuevo= Copiar( &Expresion[q], p-q );
			NodoOperador *Op= Operadores.Buscar( Nuevo );
			for( ; EsEspacio( Expresion[p] ) ; ++p );
			char CaracterEnP[2]={ Expresion[p],'\0' };
			NodoOperador *Op2= Operadores.Buscar( CaracterEnP );
			bool EsFuncion= Operadores.BuscarFuncion( Nuevo );
			if ( EsFuncion && CaracterEnP[0] != '(' ){
			  Buzon.SetIdentificadorAsociado( Nuevo );
			  Buzon.Error( FUNCION_REQUIERE_PARAMETRO );
			  delete[] Nuevo;
			  LiberarListaToken( Inicio );
			  return 0;
			}

			if( Op && ( Expresion[p]=='\0' || Expresion[p]=='_' ||
					 ( Op2 && !EstaEn( Expresion[p], "(-+" ) ) ) ){
			  Buzon.SetIdentificadorAsociado( Nuevo );
			  Buzon.Error( FUNCION_REQUIERE_PARAMETRO );
			  delete[] Nuevo;
			  LiberarListaToken( Inicio );
			  return 0;
			}
			if( !Op && ( EsDigito( Expresion[p] ) || Expresion[p]=='_' ) ){
			  Buzon.SetIdentificadorAsociado( Nuevo );
			  Buzon.Error( FALTA_OPERADOR );
			  delete[] Nuevo;
			  LiberarListaToken( Inicio );
			  return 0;
			}
			if( Op )
			  Insertar( &Inicio, &Ultimo, Nuevo, OPERADOR, CONSTANTE,
							Op->GetPdp(), Op->GetPfp(), Op->GetAlcance() );
			else
			  Insertar( &Inicio, &Ultimo, Nuevo, OPERANDO, VARIABLE );
			delete[] Nuevo;
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }

		 case 3:{       // Comilla simple
			for( ; Expresion[p] && Expresion[p] != '\''; ++p );
			if( Expresion[p]== '\0' ){
			  Buzon.SetIdentificadorAsociado( &Expresion[q+1] );
			  Buzon.Error( CADENA_INDETERMINADA );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			char *Nuevo= Copiar( &Expresion[q+1],p-q-1 );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			for( p=p+1 ; EsEspacio( Expresion[p] ) ; ++p );
			char CaracterEnP[2]={ Expresion[p],'\0' };
			if( EstaEn( Expresion[p],"^/*" ) ){
			  Buzon.SetIdentificadorAsociado( CaracterEnP );
			  Buzon.Error( OPERADOR_ILEGAL );
			  LiberarListaToken( Inicio );
			  delete[] Nuevo;
			  return 0;
			}
			if( EsDigito( Expresion[p] ) || EstaEn( Expresion[p],"'.(_" ) ){
			  char *CadenaParaError=Copiar(&Expresion[q],strlen(Nuevo)+2);
			  Buzon.SetIdentificadorAsociado( CadenaParaError );
			  Buzon.Error( FALTA_OPERADOR );
			  if(CadenaParaError)
				 delete[] CadenaParaError;
			  delete[] Nuevo;
			  LiberarListaToken( Inicio );
			  return 0;
			}
			Insertar( &Inicio, &Ultimo, Nuevo, OPERANDO, CONSTANTE );
			delete[] Nuevo;
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }

		 case 4:{     // Punto
			if( !EstaEn( (char)toupper( Expresion[p] ),"VF" ) ||
				 Expresion[p+1] != '.' ){
			  Buzon.SetIdentificadorAsociado( "." );
			  Buzon.Error( CONSTANTE_LOGICA_ILEGAL );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			bool ValorLogico= toupper( Expresion[p] )== 'V'?true:false;
			for( p= p+2 ; EsEspacio( Expresion[p] ) ; ++p );
			if( EsDigito( Expresion[p] ) || EstaEn( Expresion[p],"_.'(" ) ){
			  Buzon.SetIdentificadorAsociado( (ValorLogico) ? (".V."):(".F.") );
			  Buzon.Error( FALTA_OPERADOR );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			char CaracterEnP[2]={ Expresion[p],'\0' };
			NodoOperador *Op= Operadores.Buscar( CaracterEnP );
			if( Op && !EstaEn( Expresion[p], "=!,)" ) ){
			  Buzon.SetIdentificadorAsociado( CaracterEnP );
			  Buzon.Error( OPERADOR_LOGICO_ILEGAL );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			Insertar( &Inicio, &Ultimo, ValorLogico, OPERANDO, CONSTANTE );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }
		 case 5:{     // Parentesis (
			for( ; EsEspacio( Expresion[p] ) ; ++p );
			Buzon.SetIdentificadorAsociado( "(" );
			char CaracterEnP[2]={ Expresion[p],'\0' };
			NodoOperador *Op= Operadores.Buscar( CaracterEnP );
			if( !Expresion[p] || Expresion[p]=='_' ||
				 ( Op && !EstaEn( Expresion[p],"+-(" ) ) ){
			  Buzon.Error(FALTA_OPERANDO);
			  LiberarListaToken( Inicio );
			  return 0;
			}
			NodoOperador *Op2= Operadores.Buscar( "(" );
			Insertar( &Inicio, &Ultimo, "(", OPERADOR, CONSTANTE,
						  Op2->GetPdp(), Op2->GetPfp(), Op2->GetAlcance() );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }

		 case 6:{     // Parentesis )
			for( ; EsEspacio( Expresion[p] ) ; ++p );
			Buzon.SetIdentificadorAsociado( ")" );
			if( EsDigito( Expresion[p] ) || EstaEn( Expresion[p],"'.(_" ) ){
			  Buzon.Error(FALTA_OPERADOR );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			NodoOperador *Op= Operadores.Buscar( ")" );
			Insertar( &Inicio, &Ultimo, ")", OPERADOR, CONSTANTE,
						  Op->GetPdp(), Op->GetPfp(), Op->GetAlcance() );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }

		 case 7:{  // Operadores +-
			char OpUnitario[2]={ Expresion[p-1],'\0' };
			for( ; EsEspacio( Expresion[p] ) ; ++p );
			char CaracterEnP[2]={ Expresion[p],'\0' };
			NodoOperador *Op= Operadores.Buscar( CaracterEnP );
			NodoOperador *Op2= Operadores.Buscar( OpUnitario );
			if( EstaEn( Expresion[p],"._)" ) || !Expresion[p] ||
				 ( Op && !EstaEn( Expresion[p],"(+-" ) ) ){
			  Buzon.SetIdentificadorAsociado( OpUnitario );
			  Buzon.Error( FALTA_OPERANDO );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			Insertar( &Inicio, &Ultimo, OpUnitario, OPERADOR, CONSTANTE,
						 Op2->GetPdp(), Op2->GetPfp(), Op2->GetAlcance() );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }

		 case 8:{	   //Operadores */^
			char OpBinario[2]={ Expresion[p-1],'\0' };
			for( ; EsEspacio( Expresion[p] ) ; ++p );
			char CaracterEnP[2]={ Expresion[p],'\0' };
			NodoOperador *Op= Operadores.Buscar( CaracterEnP );
			NodoOperador *Op2= Operadores.Buscar( OpBinario );
			if( EstaEn( Expresion[p],".'_" ) ){
			  Buzon.SetIdentificadorAsociado( OpBinario );
			  Buzon.Error( OPERADOR_ILEGAL );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			if( !Expresion[p] || ( Op && !EstaEn( Expresion[p],"(+-" ) ) ){
			  Buzon.SetIdentificadorAsociado( OpBinario );
			  Buzon.Error( FALTA_OPERANDO );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			Insertar( &Inicio, &Ultimo, OpBinario, OPERADOR, CONSTANTE,
						 Op2->GetPdp(), Op2->GetPfp(), Op2->GetAlcance() );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }

		 case 9:{   // Operadores relacionales < >
			char OpBinario[3]={ Expresion[p-1],'\0' };
			if( Expresion[p]=='=' ){
			  OpBinario[1]= Expresion[p];
			  OpBinario[2]= '\0';
			  ++p;
			}
			for( ; EsEspacio( Expresion[p] ) ; ++p );
			char CaracterEnP[2]={ Expresion[p],'\0' };
			NodoOperador *Op= Operadores.Buscar( CaracterEnP );
			NodoOperador *Op2= Operadores.Buscar( OpBinario );
			if( ( Op && !EstaEn( Expresion[p],"+-(" ) ) || Expresion[p]=='_' ||
				  !Expresion[p] ){
			  Buzon.SetIdentificadorAsociado( OpBinario );
			  Buzon.Error( FALTA_OPERANDO );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			Insertar( &Inicio, &Ultimo, OpBinario, OPERADOR, CONSTANTE,
						 Op2->GetPdp(), Op2->GetPfp(), Op2->GetAlcance() );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }

		 case 10:{   // Operador =
			char OpBinario[2]={ Expresion[p-1],'\0' };
			for( ; EsEspacio( Expresion[p] ) ; ++p );
			char CaracterEnP[2]={ Expresion[p],'\0' };
			NodoOperador *Op= Operadores.Buscar( CaracterEnP );
			NodoOperador *Op2= Operadores.Buscar( OpBinario );
			if( ( Op && !EstaEn( Expresion[p],"+-(" ) ) || Expresion[p]=='_' ||
				  !Expresion[p] ){
			  Buzon.SetIdentificadorAsociado( OpBinario );
			  Buzon.Error( FALTA_OPERANDO );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			Insertar( &Inicio, &Ultimo, OpBinario, OPERADOR, CONSTANTE,
						 Op2->GetPdp(), Op2->GetPfp(), Op2->GetAlcance() );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }

		 case 11:{   //Operador  !
			char OpBinario[3]={ Expresion[p-1],Expresion[p],'\0' };
			if( Expresion[p]!='=' ){
			  Buzon.SetIdentificadorAsociado( OpBinario );
			  Buzon.Error( OPERADOR_ILEGAL );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			for( p=p+1 ; EsEspacio( Expresion[p] ) ; ++p );
			char CaracterEnP[2]={ Expresion[p],'\0' };
			NodoOperador *Op= Operadores.Buscar( CaracterEnP );
			NodoOperador *Op2= Operadores.Buscar( OpBinario );
			if( !Expresion[p] || ( Expresion[p]=='_' ) ||
				 ( Op && !EstaEn( Expresion[p],"(+-" ) ) ){
			  Buzon.SetIdentificadorAsociado( OpBinario );
			  Buzon.Error( FALTA_OPERANDO );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			Insertar( &Inicio, &Ultimo, OpBinario, OPERADOR, CONSTANTE,
						 Op2->GetPdp(), Op2->GetPfp(), Op2->GetAlcance() );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }

		 case 12:{  //  Operador	,
			char OpComa[2]= { Expresion[p-1], '\0' };
			for( ; EsEspacio( Expresion[p] ) ; ++p );
			char CaracterEnP[2]={ Expresion[p], '\0' };
			NodoOperador *Op= Operadores.Buscar( CaracterEnP );
			if( !Expresion[p] || ( Expresion[p]=='_' ) ||
				 ( Op && !EstaEn( Expresion[p],"(+-" ) ) ){
			  Buzon.SetIdentificadorAsociado( OpComa );
			  Buzon.Error( FALTA_OPERANDO );
			  LiberarListaToken( Inicio );
			  return 0;
			}
			Insertar( &Inicio, &Ultimo, OpComa, OPERADOR, CONSTANTE );
			if( Buzon.GetHuboError() ){
			  LiberarListaToken( Inicio );
			  return 0;
			}
			break;
		 }
		 default:{
			char Extranyo[2]={ Expresion[p-1],'\0' };
			Buzon.SetIdentificadorAsociado( Extranyo );
			Buzon.Error( CARACTER_ILEGAL );
			LiberarListaToken( Inicio );
			return 0;
		 }

	  } // Fin del switch
	} //Cierre del while
	return Inicio;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// PreprocesarExpresión : Esta función realiza una segunda pasada sobre
// la lista de Token's para encontrar errores que no fueron detectados en
// la primera etapa, incluir el operador de acceso de arrays y cambiar
// el alcance de los operadores mixtos ( +, - )

 void PreprocesarExpresion( Token *Expresion ){
	Token Ficticio(",", OPERADOR, CONSTANTE);
	Token *Actual= Expresion, *Anterior= &Ficticio;
	int BalanzaParentesis= 0, NumeroDeParentesisAbiertos= 0;
	bool PrimeraVez= true;
	while( Actual ){

	  if( Actual->GetTipoToken() == OPERADOR ){
		 char *Operador= Actual->GetOperador();
		 if( !strcmp( Operador, "(") ){
			++NumeroDeParentesisAbiertos;
			++BalanzaParentesis;
		 }
		 else if( !strcmp( Operador, ")") )
			--BalanzaParentesis;
		 if( BalanzaParentesis<0 ){
			Buzon.SetIdentificadorAsociado( ")" );
			Buzon.Error( CERRADO_PARENTESIS_NO_ABIERTO );
			return ;
		 }
		 if( Anterior->GetTipoToken()==OPERADOR ){
			if( (!strcmp( Operador,"+") || !strcmp( Operador, "-" )
				 ) && strcmp( Anterior->GetOperador(),")") ){
			  Actual->SetAlcanceOperador( UNITARIO );
			  Actual->SetPdp( 80 );
			  Actual->SetPfp( 81 );
			}
			if(Actual->GetAlcanceOperador()==BINARIO &&
				strcmp(Actual->GetOperador(),"(")&&
				strcmp(Anterior->GetOperador(),")") ){
			  Buzon.SetIdentificadorAsociado(
			  ((PrimeraVez)?Actual:Anterior)->GetOperador());
			  Buzon.Error( FALTA_OPERANDO );
			  return ;
			}
			if( Anterior->GetAlcanceOperador()==BINARIO &&
				 strcmp( Anterior->GetOperador(), ")") )
				if( Actual->GetAlcanceOperador()==BINARIO &&
					 strcmp(Actual->GetOperador(),"(") ){
				 Buzon.SetIdentificadorAsociado( (PrimeraVez)?Actual->GetOperador():
															  Anterior->GetOperador() );
				 Buzon.Error( FALTA_OPERANDO );
				 return ;
				}
			if( !strcmp( Anterior->GetOperador(), ")") &&
				 Actual->GetAlcanceOperador()==UNITARIO ){
				Buzon.SetIdentificadorAsociado( Actual->GetOperador() );
				Buzon.Error( FALTA_OPERADOR );
			  return ;
			}
			if( Anterior->GetTipoToken()==OPERANDO &&
				  Actual->GetAlcanceOperador()==UNITARIO ){
			  if( Anterior->GetTipoAlmacenamiento()==VARIABLE )
				 Buzon.SetIdentificadorAsociado( Anterior->GetDatoStr() );
				else{
				 char Buffer[512];
				 switch( Anterior->GetTipoDato() ){
					case REAL:
					  gcvt( Anterior->GetDatoReal(), 4, Buffer );
					  break;
					case LOGICO:
					  Buffer[0]= Buffer[2]= '.';
					  Buffer[1]= (Anterior->GetDatoLogico())?'V':'F';
					  Buffer[3]= '\0';
					  break;
					case STRING:
					  strcpy( Buffer, Anterior->GetDatoStr() );
					  break;
					default:
					  //Imprime("No entiendoooooo 154122");
					  exit(1);
					  break;
				 };
			  Buzon.SetIdentificadorAsociado( Buffer );
			  }
			  Buzon.Error( FALTA_OPERADOR );
			  return;
			}
		 }
		 else{
		  if( Actual->GetAlcanceOperador()==UNITARIO &&
				Anterior->GetTipoToken()==OPERANDO)
		  {
			 if( Anterior->GetTipoAlmacenamiento()==VARIABLE )
			  Buzon.SetIdentificadorAsociado( Anterior->GetDatoStr() );
			else{
			  char Buffer[512];
			  switch( Anterior->GetTipoDato() ){
				 case REAL:
					gcvt( Anterior->GetDatoReal(), 8, Buffer );  ///// ojooo 8!
					break;
				 case LOGICO:
					Buffer[0]= Buffer[2]= '.';
					Buffer[1]= (Anterior->GetDatoLogico())?'V':'F';
					Buffer[3]= '\0';
					break;
				 case STRING:
					strcpy( Buffer, Anterior->GetDatoStr() );
					break;
				 default:
					//Imprime("No entiendoooooo 1541288");
					exit(1);
					break;
			  };
			  Buzon.SetIdentificadorAsociado( Buffer );
			}
			Buzon.Error( FALTA_OPERADOR );
			return ;
		  }
		 }
	  }
	  else{ // Actual == Operando
		 if( Anterior->GetTipoToken()==OPERANDO ){
			if( Anterior->GetTipoAlmacenamiento()==VARIABLE )
			  Buzon.SetIdentificadorAsociado( Anterior->GetDatoStr() );
			else{
			  char Buffer[512];
			  switch( Anterior->GetTipoDato() ){
				 case REAL:
					gcvt( Anterior->GetDatoReal(), 4, Buffer );
					break;
				 case LOGICO:
					Buffer[0]= Buffer[2]= '.';
					Buffer[1]= (Anterior->GetDatoLogico())?'V':'F';
					Buffer[3]= '\0';
					break;
				 case STRING:
					strcpy( Buffer, Anterior->GetDatoStr() );
					break;
				 default:
					//Imprime("No entiendoooooo 1541288");
					exit(1);
					break;
			  };
			  Buzon.SetIdentificadorAsociado( Buffer );
			}
			Buzon.Error( FALTA_OPERADOR );
			return ;
		 }
		 else if( !strcmp( Anterior->GetOperador(),")" ) ){
			Buzon.SetIdentificadorAsociado( ")" );
			Buzon.Error( FALTA_OPERADOR );
			return ;
		 }
	  }//Else
	  Anterior= Actual;
	  Actual= Actual->GetSig();
	  PrimeraVez= true;
	}// While

	if( BalanzaParentesis ){
	  Buzon.SetIdentificadorAsociado( "(" );
	  Buzon.Error( ABIERTO_PARENTESIS_NO_CERRADO );
	  return ;
	}
	Actual= Expresion;

	while( Actual ){
	  if( Actual->GetTipoToken()==OPERANDO && Actual->GetSig() ){
		 Token *Siguiente= Actual->GetSig();
		/* if( Siguiente->GetTipoToken()!=OPERADOR ){// BORRAR
			Imprime("No entiendooo 888854");   // BORRAR
			exit(1);                               // BORRAR
		 }*/                                        // BORRAR
		 if( !strcmp( Siguiente->GetOperador(), "(" ) ){
			Token *YoRecorro= Siguiente;
			BalanzaParentesis= 0;
			int Dimension= 1;
			do{
			  if( YoRecorro->GetTipoToken()==OPERADOR ){
				 if( !strcmp( YoRecorro->GetOperador(), "(") ){
					++BalanzaParentesis;
					YoRecorro=YoRecorro->GetSig();
					continue;
				 }
				 if( !strcmp( YoRecorro->GetOperador(), ")") ){
					--BalanzaParentesis;
					YoRecorro=YoRecorro->GetSig();
					continue;
				 }
				 if( BalanzaParentesis==1 )
					if( !strcmp( YoRecorro->GetOperador(), ",") )
					  ++Dimension;
			  }
			YoRecorro=YoRecorro->GetSig();
			}while( BalanzaParentesis );
			Token *Nuevo;
			{//BLOQUE
			  char Buffer[16]={'$','\0'};
			  if( Operadores.BuscarFuncion( Actual->GetOperador() ) )
				 Buffer[0]= '@';
			  itoa(Dimension,Buffer+1,10);
			  Nuevo= new Token( Buffer, OPERADOR, CONSTANTE, 0, 0);
			}//BLOQUE
			Nuevo->SetSig( Siguiente );
			Actual->SetSig( Nuevo );
			Actual=Siguiente;
		 }//IF
	  } //IF
	  Actual= Actual->GetSig();
	}

	//Aquí se comprueba el correcto uso de la coma (,)
	int Tope= 0;
	bool *PilaParentesis= new bool[ NumeroDeParentesisAbiertos + 2 ];
	PilaParentesis[0]= false;
	Actual= Expresion;
	while( Actual ){
	  if( Actual->GetTipoToken()==OPERADOR ){
		 if( *Actual->GetOperador()=='(' )
			PilaParentesis[ ++Tope ]= false;
		 else if( *Actual->GetOperador()==')' )
			--Tope;
		 else if( *Actual->GetOperador()=='$' || *Actual->GetOperador()=='@'){
			PilaParentesis[++Tope]= true;
			Actual= Actual->GetSig();
		 }
		 else if( *Actual->GetOperador()==',' )
			if( !PilaParentesis[Tope] ){
			  Buzon.SetIdentificadorAsociado( "," );
			  Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
			  delete[] PilaParentesis;
			  return ;
			}
	  }
	  Actual= Actual->GetSig();
	}
	delete[] PilaParentesis;
	return ;
 }




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// InsertarDeUltimo : Esta función inserta un nodo al final de una lista
// simplemente ligada.
// Al comenzar a insertar Inicio debe valer 0 y Ultimo tambien.

 void InsertarDeUltimo( Token *Nuevo, Token **Inicio, Token **Ultimo ){
	if(!(*Ultimo))
	  *Inicio=Nuevo;
	else
	  (*Ultimo)->SetSig(Nuevo);
	*Ultimo=Nuevo;
	Nuevo->SetSig(0);
 }


 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// Apila,Desapila : Estas funciones se utilizan en la evaluación de
// expresiones para manejar una pila como lista ligada.

  void Apila( Token **Inicio, Token *Nuevo ){
	Nuevo->SetSig(*Inicio);
	*Inicio=Nuevo;
	return;
 }

 inline Token * Desapila( Token **Inicio ){
	Token *Ret=*Inicio;
	(*Inicio)=(*Inicio)->GetSig();
	return Ret;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// InfijoAPostfijo : Esta función convierte una lista de Token's
// organizados en Infijo a su correspondiente notación PostFijo

 Token *InfijoAPostfijo( Token * InicioInfijo ){
	Token *Pila= 0;
	Token *InicioPostfijo= 0, *UltimoPostfijo= 0;

	while( InicioInfijo ){

	  Token *Actual= InicioInfijo;
	  InicioInfijo= InicioInfijo->GetSig();

	  switch( Actual->GetTipoToken() ){
		 case OPERADOR:

			if( !strcmp(Actual->GetOperador(),")") ){
			  delete Actual;
			  Token *Auxiliar= Desapila(&Pila);
			  while(! ( !strcmp(Auxiliar->GetOperador(),"(")
					  && (Auxiliar->GetTipoToken()==OPERADOR)) ){
				 InsertarDeUltimo( Auxiliar, &InicioPostfijo, &UltimoPostfijo );
				 Auxiliar= Desapila(&Pila);
			  }
			  delete Auxiliar;
			  if(Pila)
				 if((*Pila->GetOperador()=='$'|| *Pila->GetOperador()=='@')
					 &&(Pila->GetTipoToken()==OPERADOR))
					InsertarDeUltimo(Desapila(&Pila),&InicioPostfijo,
										  &UltimoPostfijo);
			}
			else if( !strcmp(Actual->GetOperador(),",") ){
			  delete Actual;
			  while( !( (!strcmp(Pila->GetOperador(),"("))
					  &&(Pila->GetTipoToken()==OPERADOR) ) )
				 InsertarDeUltimo(Desapila(&Pila),&InicioPostfijo,
										&UltimoPostfijo);
			}
			else if( *Actual->GetOperador()=='$'|| *Actual->GetOperador()=='@'){
			  Apila(&Pila,Actual);
			}
			else{
			  bool ContinuarExtrayendo= true;
			  while( Pila && ContinuarExtrayendo ){
				 if( Pila->GetPdp() >= Actual->GetPfp() )
					InsertarDeUltimo(Desapila(&Pila),&InicioPostfijo,
										  &UltimoPostfijo);
				 else ContinuarExtrayendo= false;
			  }
			  Apila(&Pila,Actual);
			}
			break;

		 case OPERANDO:
			InsertarDeUltimo(Actual,&InicioPostfijo,&UltimoPostfijo);
			break;
	  };//Switch
	}//While
	while(Pila)
	  InsertarDeUltimo(Desapila(&Pila),&InicioPostfijo,&UltimoPostfijo);
 return InicioPostfijo;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Token *GetPostfijo( char *Expresion ){

	Token *Retorno= GetListaToken( Expresion );
	if( !Retorno || Buzon.GetHuboError() )
	  return 0;

	PreprocesarExpresion( Retorno );

	if( Buzon.GetHuboError() ){
	  if(Retorno)                                 // nelson 29/08/97
		 LiberarListaToken(Retorno);              // nelson 29/08/97
	  return 0;
	}
	Retorno= InfijoAPostfijo( Retorno );
	if( Buzon.GetHuboError() )
	  return 0;
	return Retorno;
 }


#endif
