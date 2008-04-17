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


#ifndef __OPERADOR_CPP
#define __OPERADOR_CPP


#include<stdlib.h>
#include<string.h>

 class NodoOperador {
		 char *Dato;  /*no se destruye especificamente*/
		 int Pdp;
		 int Pfp;
		 NodoOperador *Sig;
		 AlcanceOperador Alcance;
	  public:
		 NodoOperador( char * Nombre, int UnPdp, int UnPfp,
							AlcanceOperador UnAlcance);
		 ~NodoOperador(){
		 }
		 NodoOperador *GetSig(){ return Sig; }
		 void SetSig( NodoOperador * UnSig ){ Sig= UnSig; }
		 char *GetDato(){ return Dato; }
		 void SetAlcance(AlcanceOperador UnAlcance){ Alcance=UnAlcance; }
		 AlcanceOperador GetAlcance(){ return Alcance; }
		 int GetPdp(){ return Pdp; }
		 int GetPfp(){ return Pfp; }
 };

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 NodoOperador::NodoOperador( char * Nombre, int UnPdp, int UnPfp,
									AlcanceOperador UnAlcance){
		Dato= Nombre;
		Pdp= UnPdp;
		Pfp= UnPfp;
		Alcance= UnAlcance;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



 class Operadores{
	  NodoOperador *InicioOperadores;
	  void Insertar( char * Str, int UnPdp, int UnPfp,
						  AlcanceOperador UnAlcance );
	  void Insertar( char * Str );

	  struct NodoFuncion{
		 char *Dato;
		 NodoFuncion *Sig;
	  } *InicioFunciones;


	public:
	  Operadores(){
		  InicioOperadores= 0;
		  InicioFunciones= 0;
		  Insertar("^",80,81,BINARIO);


		  Insertar("SIN",80,81,UNITARIO);

		  Insertar("COS",80,81,UNITARIO);

		  Insertar("TAN",80,81,UNITARIO);

		  Insertar("NOT",80,81,UNITARIO);

		  Insertar("SINH",80,81,UNITARIO);

		  Insertar("COSH",80,81,UNITARIO);

		  Insertar("TANH",80,81,UNITARIO);

		  Insertar("ASIN",80,81,UNITARIO);

		  Insertar("ACOS",80,81,UNITARIO);

		  Insertar("ATAN",80,81,UNITARIO);

		  Insertar("TRUNC",80,81,UNITARIO);

		  Insertar("ROUND",80,81,UNITARIO);

		  Insertar("ABS",80,81,UNITARIO);

		  Insertar("SQRT",80,81,UNITARIO);

		  Insertar("RANDOM",80,81,UNITARIO);

		  Insertar("LEN",80,81,UNITARIO);

		  Insertar("LN",80,81,UNITARIO);

		  Insertar("LOG",80,81,UNITARIO);

		  Insertar("EXP",80,81,UNITARIO);



		  Insertar("MOD",70,70,BINARIO);

		  Insertar("*",70,70,BINARIO);

		  Insertar("/",70,70,BINARIO);

		  Insertar("+",60,60,BINARIO);

		  Insertar("-",60,60,BINARIO);

		  Insertar("<",50,50,BINARIO);

		  Insertar(">",50,50,BINARIO);

		  Insertar(">=",50,50,BINARIO);

		  Insertar("<=",50,50,BINARIO);

		  Insertar("=",40,40,BINARIO);

		  Insertar("!=",40,40,BINARIO);

		  Insertar("AND",20,20,BINARIO);

		  Insertar("NAND",20,20,BINARIO);

		  Insertar("OR",10,10,BINARIO);

		  Insertar("NOR",20,20,BINARIO);

		  Insertar("XOR",10,10,BINARIO);

		  Insertar("XNOR",10,10,BINARIO);

		  Insertar("(",0,100,BINARIO);

		  Insertar(")",-1,-1,BINARIO);

		  Insertar(",",-1,-1,BINARIO);

		  Insertar("SUBSTRING");

	  }
	  ~Operadores();
	  NodoOperador *Buscar ( char *Operador );
	  bool BuscarFuncion( char *Funcion );

 }Operadores;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Operadores::~Operadores(){

	NodoOperador *Aux;
	while( InicioOperadores ){
	  Aux= InicioOperadores;
	  InicioOperadores= InicioOperadores->GetSig();
	  delete Aux;
	}
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 NodoOperador *Operadores::Buscar ( char *Operador ){
		NodoOperador *Aux= InicioOperadores;
		for( ; Aux ; Aux= Aux->GetSig() )
			if(!strcmpi( Aux->GetDato(),Operador) )
			  return Aux;
		return 0;
  }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 bool Operadores::BuscarFuncion( char *Str ){
	NodoFuncion *Aux= InicioFunciones;
	for( ; Aux ; Aux= Aux->Sig)
	  if( !strcmpi( Aux->Dato,Str ) )
		 return true;
	  return false;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


 void Operadores::Insertar( char * Str, int UnPdp, int UnPfp,
									AlcanceOperador UnAlcance){
	 NodoOperador *NuevoNodoOperador= new NodoOperador(Str, UnPdp, UnPfp
																		,UnAlcance);
	 NuevoNodoOperador->SetSig(InicioOperadores);
	 InicioOperadores= NuevoNodoOperador;
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void Operadores::Insertar( char *Str ){
	NodoFuncion *Nuevo= new NodoFuncion;
	Nuevo->Dato= Str;
	Nuevo->Sig= InicioFunciones;
	InicioFunciones= Nuevo;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#endif
