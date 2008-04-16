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

#ifndef __OE_CPP
#define __OE_CPP


#include "includes.cpp"


 enum TipoObjeto{ INICIO= 0, FIN, SUBPROGRAMA, REGRESE, IMPRESION, LECTURA,
						ASIGNACION, DECISION, CICLOPARA, CICLOMIENTRAS, CIERREPARA,
						CIERREMIENTRAS, LLAMADA, PUNTO, NINGUNO };

 int AnchoIzquierdo[]={ 2, 2, 8, 2, 4, 4, 4, 4, 7, 7, 3, 3, 7, 0 };
 int AnchoDerecho[]=  { 2, 2, 8, 2, 4, 4, 4, 4, 7, 7, 3, 3, 7, 0 };
 int AltoArriba[]=    { 2, 2, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0 };
 int AltoAbajo[]=     { 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0 };

 char *RetornaValorDeTokenEnCadena( Token *UnToken ){
	static char v[64];
	switch (UnToken->GetTipoDato()){
	  case REAL:
		 sprintf(v,"%.10Lg",UnToken->GetDatoReal());
		 return v;
	  case STRING:
		 return UnToken->GetDatoStr();
	  case LOGICO:
		 return (UnToken->GetDatoLogico())?".V.":".F.";
	  default:
		 return "Error en RetornaValorDeTokenEnCadena";
	};
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  class ListaCadenas{

	  struct Nodo{
		 char *Dato;
		 Nodo *Sig;
	  }*Inicio, *Iterador, *Ultimo;

	  int NItems;

	public:

	  ListaCadenas(){
		 Inicio= 0;
		 NItems= 0;
	  }
	  int GetNItems(){ return NItems; }
	  void Reset(){ Iterador= Inicio; }
	  char *Itera();
	  void Insertar( char * );
	  void InsertarEn( int, char * );
	  void Eliminar( int );
	  void Vacear();
 };

 void ListaCadenas::InsertarEn( int Pos, char *Str ){
	Nodo *Nuevo= new Nodo;
	Nuevo->Dato= Str;
	if( Pos == 0 ){

	  Nuevo->Sig= Inicio;
	  Inicio= Nuevo;
	  if( NItems==0 )
		 Ultimo= Nuevo;
	}
	else{
	  Nodo *Ant= Inicio;
	  for( int i= 1; i<Pos; ++i )
		 Ant= Ant->Sig;
	  Nuevo->Sig= Ant->Sig;
	  Ant->Sig= Nuevo;
	  if( Ultimo==Ant )
		 Ultimo= Nuevo;
	}
	++NItems;
 }

 char *ListaCadenas::Itera(){
	char *Retorno= Iterador->Dato;
	Iterador= Iterador->Sig;
	return Retorno;
 }


 void ListaCadenas::Eliminar( int Pos ){
	Nodo *Ant= 0, *Actual= Inicio ;
	for( int i= 0; i<Pos; ++i ){
	  Ant= Actual;
	  Actual= Actual->Sig;
	}
	if( Ant==0 )
	  Inicio= Actual->Sig;
	else
	  Ant->Sig= Actual->Sig;
	if( Ultimo==Actual )
     Ultimo= Ant;
	delete[] Actual->Dato;
	delete Actual;
	--NItems;
 }


 void ListaCadenas::Insertar( char *Str ){
	Nodo *Nuevo= new Nodo;
	Nuevo->Dato= Str;
	Nuevo->Sig= 0;
	if( !Inicio )
	  Inicio= Nuevo;
	else
	  Ultimo->Sig= Nuevo;
	Ultimo= Nuevo;
	Iterador= Inicio;
	++NItems;
 }

 void ListaCadenas::Vacear(){
	while( Inicio ){
	  Nodo *Aux= Inicio;
	  Inicio= Inicio->Sig;
	  delete[] Aux->Dato;
	  delete Aux;
	}
	NItems= 0;
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// ListaExpresiones ha sido diseñada para manejar un conjunto de listas
// de Tokens en una lista ligada.

 class ListaExpresiones{

	  struct Nodo{
		 Token *Expresion;
		 Nodo *Sig;
		 Nodo(){
			ContadorNodoListaExpresiones++;
		 }
		 ~Nodo(){
			ContadorNodoListaExpresiones--;
	    }

	  }*Inicio, *Iterador, *Ultimo;

	  int NItems;

	public:

	  ListaExpresiones(){
		 Inicio= 0;
		 NItems= 0;
	  }
	  void GetListaExpresiones( char* );
	  int GetNItems(){ return NItems; }
	  void Reset(){ Iterador= Inicio; }
	  Token *Itera(){
		 Token *Retorno= Iterador->Expresion;
		 Iterador= Iterador->Sig;
		 return Retorno;
	  }
	  void Insertar( Token * );
	  void Vacear();
	  void OE_SubPrograma_RevisarParametros();
 };

 void ListaExpresiones::OE_SubPrograma_RevisarParametros(){
	Nodo *p1, *p2;

	for( p1= Inicio; p1; p1= p1->Sig ){
	  Token *Aux= p1->Expresion;
	  if( (Aux->GetTipoAlmacenamiento()!=VARIABLE)|| (Aux->GetSig()) ){
		 Buzon.Error( PARAMETRO_ILEGAL );
		 return;
	  }
	}

	for( p1=Inicio ; p1 ; p1=p1->Sig ){
	  Token *q1=p1->Expresion;
	  for( p2= p1->Sig ; p2 ; p2=p2->Sig){
		 Token *q2= p2->Expresion;
		 if(!strcmpi(q1->GetDatoStr(),q2->GetDatoStr())){
			Buzon.SetIdentificadorAsociado(q2->GetDatoStr());
			Buzon.Error( PARAMETRO_REPETIDO );
			return;
		 }
	  }
	}
 return ;
 }


 void ListaExpresiones::Insertar( Token *t ){
	Nodo *Nuevo= new Nodo;
	Nuevo->Expresion= t;
	Nuevo->Sig= 0;
	if( !Inicio )
	  Inicio= Nuevo;
	else
	  Ultimo->Sig= Nuevo;
	Ultimo= Nuevo;
	Iterador= Inicio;
	++NItems;
 }

 void ListaExpresiones::Vacear(){
	while( Inicio ){
	  Nodo *Temp= Inicio;
	  Inicio= Inicio->Sig;
	  LiberarListaToken(Temp->Expresion);
	  delete Temp;
	}
	NItems= 0;
 }


 void ListaExpresiones::GetListaExpresiones( char *Str ){
	if( !Str ){
	  Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
	  return;
	}


	int Parent= 0;
	int IniChar= 0;
	int Largo= strlen(Str);
	int ActualChar;
	int EsCadena= 0;
	for( ActualChar=0 ; ActualChar<=Largo ; ++ActualChar ){
	  char &Casilla=Str[ActualChar];
	  if( Casilla=='(' && !EsCadena )
		 ++Parent;
	  else if( Casilla==')' && !EsCadena  )
		 --Parent;
	  else if( Casilla=='\'' )
		 EsCadena= 1-EsCadena;
	  else if( Casilla==0 || (Casilla==','&& !Parent && !EsCadena )){
		 char Temp= Casilla;
		 Casilla= 0;
		 //VentanaPrincipal->MessageBox( Str+IniChar,"" );
		 Token *t = GetPostfijo(Str+IniChar);
		 Casilla=Temp;
		 IniChar=1+ActualChar;
		 if(Buzon.GetHuboError())
			return;
		 if(!t){
			// Uso ilegal de la coma o cadena vacía
			// Un error pa'l buzón
			Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
			return;
		 }
		 Insertar(t);
	  }
	}
	return;
 }








;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// ListaVectores ha sido diseñada para manejar un conjunto de vectores
// (Nombre, dimension e indices por separado) en una lista ligada.

 class ListaVectores{

	  struct Nodo{
		 Nodo(){
			ContadorNodoListaVectores++;
		 }
		 ~Nodo(){
			ContadorNodoListaVectores--;
		 }


		 char *Identificador;
		 ListaExpresiones Lista;
		 unsigned int *VectorIndices;
		 Nodo *Sig;
	  }*Inicio, *Iterador, *Ultimo;

	  int NItems;

	public:

	  ListaVectores(){
		 Inicio= 0;
		 NItems= 0;
	  }
	  int GetNItems(){ return NItems; }

	  char *GetActualIdentificador(){ return Iterador->Identificador; }

	  unsigned int *GetActualVectorIndices(){ return Iterador->VectorIndices; }

	  ListaExpresiones &GetActualLista(){ return Iterador->Lista; }

	  ListaExpresiones &GetNuevaLista(){ return Ultimo->Lista; }

	  void EvaluaActualesIndices();

	  void SetNuevoIdentificador( char *Identificador ){
		 Ultimo->Identificador= new char[strlen(Identificador)+1];
		 strcpy( Ultimo->Identificador , Identificador );
	  }


	  void Reset(){ Iterador= Inicio; }

	  void Itera(){
		 Iterador= Iterador->Sig;
	  }

	  void Insertar( );

	  void Vacear();

	  void AlmacenaVector( char* );

 };


 void ListaVectores::Insertar( ){
	Nodo *Nuevo= new Nodo;
	Nuevo->Sig= 0;
	Nuevo->VectorIndices= 0;
	Nuevo->Identificador= 0;
	if( !Inicio )
	  Inicio= Nuevo;
	else
	  Ultimo->Sig= Nuevo;
	Ultimo= Nuevo;
	Iterador= Inicio;
	++NItems;
 }


 void ListaVectores::Vacear(){
	while( Inicio ){
	  Nodo *Temp= Inicio;
	  Inicio= Inicio->Sig;
	  if( Temp->Identificador )
		 delete[] Temp->Identificador;
	  if( Temp->VectorIndices )
		 delete[] Temp->VectorIndices;
	  Temp->Lista.Vacear();
	  delete Temp;
	}
	NItems= 0;
 }


 void ListaVectores::EvaluaActualesIndices(){
	  ListaExpresiones &L= Iterador->Lista;// Ultimo->Lista;
	  L.Reset();
	  int NIndices= L.GetNItems();
	  unsigned int *Vec= Iterador->VectorIndices;//Ultimo->VectorIndices;
	  for( int j= 0; j<NIndices; ++j ){
		 Token *t= EvaluaPostfijo( L.Itera() );
		 if( Buzon.GetHuboError() )
			return ;
		 if( t->GetTipoDato()!= REAL ){
			//los indices deben ser enteros positivos
			// un error pa' el buzon
			delete t;
			Buzon.Error( INDICES );
			return ;
		 }
		 long double v= t->GetDatoReal();
		 delete t;
		 if( v<0L || v>65535L || ( floor( v )!= v	) ){
			// los indices deben ser enteros positivos
			//menores o iguales a 65535; un error pa' el buzon
			Buzon.Error( INDICES );
			return ;
		 }
		 Vec[j]= v;
	  }
 }


 void ListaVectores::AlmacenaVector( char *Cadena ){

	if( !Cadena){
	  Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
	  return;
	}

	int Largo= strlen( Cadena );
	Insertar();

   int ActualChar= 0;
	for( ; ActualChar<Largo && ( Cadena[ActualChar]!='(' );
		  ++ActualChar );
	char Temp= Cadena[ActualChar];
	Cadena[ActualChar]= 0;
	Token *t= GetPostfijo( Cadena );
	Cadena[ActualChar]= Temp;
	if( Buzon.GetHuboError() )
	  return;
	if( !t ){
	  //Uso ilegal de la coma o cadena vacia; un error pa'l buzon
	  Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
	  return;
	}
	if( ( t->GetTipoAlmacenamiento()!=VARIABLE ) || ( t->GetSig() ) ){
	  //Solo se admiten camos variables; un error pa'l buzon
	  LiberarListaToken( t );
	  Buzon.Error( SOLO_VARIABLES );
	  return;
	}
	SetNuevoIdentificador( t->GetDatoStr() );
	delete t;
	if( Cadena[ActualChar]==0 )
	  return;

	// Ahora los indices...

	int EsCadena= 0;
	int IniChar= ActualChar+1, Parent= 1;
	for( ++ActualChar; (ActualChar<Largo) && (Parent>0); ++ActualChar ){
	  char &Casilla= Cadena[ActualChar];
	  if( Casilla=='(' && !EsCadena )  ++Parent;
	  if( Casilla==')' && !EsCadena  ) --Parent;
	  if( Casilla=='\'' )
		 EsCadena= 1-EsCadena;

	  if( !EsCadena &&  (
			((Casilla==',') && (Parent==1)) || ((Casilla==')') && (!Parent))
								) ){
		 Temp= Casilla;
		 Casilla= 0;
		 t= GetPostfijo( Cadena+IniChar );
		 Casilla= Temp;
		 IniChar= ActualChar+1;
		 if( Buzon.GetHuboError() ) return;
		 if( !t ){
			//Uso ilegal de la coma o cadena vacia; un error pa'l buzon
			Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
			return;
		 }
		 GetNuevaLista().Insertar( t );
	  }
	}
	if( Parent ){
	  //No se cerraron todos los parentesis abiertos; un error pa'l buzon
	  Buzon.Error( ABIERTO_PARENTESIS_NO_CERRADO );
	  return;
	}
	int n= GetNuevaLista().GetNItems();
	if( !n ){
	  //No hay indices entre los parentesis; un error pa'l buzon
	  Buzon.Error( NO_INDICES );
	  return;
	}
	Ultimo->VectorIndices= new unsigned int[n];
	if( ActualChar>=Largo )return;
	t= GetPostfijo( Cadena+ActualChar );
	if( t )
	  // Solo se admiten campos variables; un error pa'l buzon
	  Buzon.Error( SOLO_VARIABLES );
	return;
 }




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class ObjetoEjecucion{

	protected:

	  TipoObjeto Tipo;
	  ObjetoEjecucion *PunteroEnHoja ;
	  int PosicionX;
	  int PosicionY;
	  void DibujarFlecha( int, int );
	  void DibujarFlechaAsignacion( int, int );
	  void DibujarLineaEnlace( int, int );

	  //Dibujar en impresora...
	  void DibujarFlechaImp( int , int );
	  void DibujarFlechaAsignacionImp( int, int );
	  void DibujarLineaEnlaceImp( int, int );
	  //End dibujar en impresora.

	public:

	  virtual void Preprocesar()= 0;
	  virtual void Despreprocesar()= 0;
	  virtual ObjetoEjecucion *Ejecutar()= 0;
	  virtual void Dibujar( int ActualX, int ActualY, int Sw= 0 )= 0;
	  //Dibujar en impresora...
	  virtual void DibujarImp( int ActualX, int ActualY )= 0;
	  //End dibujar en impresora.
	  void Editar();
	  void SetSiguienteParaClickInsertar( ObjetoEjecucion *);
	  void SetPunteroEnHoja( ObjetoEjecucion *s ){ PunteroEnHoja= s; }
	  void SetPosicionX( int p ){ PosicionX= p; }
	  void SetPosicionY( int p ){ PosicionY= p; }
	  void DibujarSeleccionado( int ActualX, int ActualY, int Sw= 1 );
	  int GetPosicionX(){ return PosicionX; }
	  int GetPosicionY(){ return PosicionY; }
	  ObjetoEjecucion *EstaEnZona( int x, int y, int ActualX, int ActualY );
	  ObjetoEjecucion *GetSiguienteParaAcomoda();
	  int EstaEnZonaInsercion( int x, int y, int ActualX, int ActualY );
	  int ClickInsercion( int, int, int, int );

	  ObjetoEjecucion *GetPunteroEnHoja(){ return PunteroEnHoja; }
	  TipoObjeto GetTipo(){ return Tipo; }

	  ~ObjetoEjecucion();

 };



 ObjetoEjecucion* ObjetoEjecucion::
 EstaEnZona( int x, int y, int ActualX, int ActualY ){

	if( Tipo==FIN || Tipo==REGRESE )
	  return 0;



	int cx= (PosicionX-ActualX)*Estado.DistanciaPuntos;
	int cy= (PosicionY-ActualY)*Estado.DistanciaPuntos;

	int Izquierda, Derecha, Abajo, Arriba;
	if( Tipo==PUNTO ){
	  Arriba= cy-Estado.DistanciaPuntos;
	  Izquierda= cx-(MinimoAnchoBrazo/2)*Estado.DistanciaPuntos;
	  Derecha= cx+(MinimoAnchoBrazo/2)*Estado.DistanciaPuntos;
	  Abajo= cy+Estado.DistanciaPuntos;
	}
	else{
	  Arriba= cy-AltoArriba[Tipo]*Estado.DistanciaPuntos;
	  Izquierda= cx-AnchoIzquierdo[Tipo]*Estado.DistanciaPuntos;
	  Derecha= cx+AnchoDerecho[Tipo]*Estado.DistanciaPuntos;
	  Abajo= cy+AltoAbajo[Tipo]*Estado.DistanciaPuntos;
	}

	return( x>=Izquierda && x<=Derecha && y>=Arriba && y<=Abajo )?this:0;
 }



 void ObjetoEjecucion::DibujarFlecha( int x, int y ){
	TPoint VectorVertices[3]={ TPoint(x,y),
										TPoint(x-Estado.DistanciaPuntos/3,y-Estado.DistanciaPuntos/2),
										TPoint(x+Estado.DistanciaPuntos/3,y-Estado.DistanciaPuntos/2),
									 };

	Graficador->SetPolyFillMode( WINDING );
	Graficador->Polygon(VectorVertices, 3);

 }

 void ObjetoEjecucion::DibujarFlechaImp( int x, int y ){
	TPoint VectorVertices[3]={ TPoint(x,y),
										TPoint(x-Estado.DistanciaPuntos/3,y-Estado.DistanciaPuntos/2),
										TPoint(x+Estado.DistanciaPuntos/3,y-Estado.DistanciaPuntos/2),
									 };

	Impresor->SetPolyFillMode( WINDING );
	Impresor->Polygon(VectorVertices, 3);

 }

 void ObjetoEjecucion::DibujarFlechaAsignacion( int x, int y ){
	TPoint VectorVertices[3]={ TPoint(x,y),
							 TPoint(x+int(float(Estado.DistanciaPuntos)/4.0),
									  y-int(float(Estado.DistanciaPuntos)/4.0)),
							 TPoint(x+int(float(Estado.DistanciaPuntos)/4.0),
									  y+int(float(Estado.DistanciaPuntos)/4.0))
									  };

	Graficador->SetPolyFillMode( WINDING );
	Graficador->Polygon(VectorVertices, 3);
	Graficador->MoveTo( x, y );
	Graficador->LineTo( x+Estado.DistanciaPuntos, y );

 }

 void ObjetoEjecucion::DibujarFlechaAsignacionImp( int x, int y ){
	TPoint VectorVertices[3]={ TPoint(x,y),
							 TPoint(x+int(float(Estado.DistanciaPuntos)/4.0),
									  y-int(float(Estado.DistanciaPuntos)/4.0)),
							 TPoint(x+int(float(Estado.DistanciaPuntos)/4.0),
									  y+int(float(Estado.DistanciaPuntos)/4.0))
									  };

	Impresor->SetPolyFillMode( WINDING );
	Impresor->Polygon(VectorVertices, 3);
	Impresor->MoveTo( x, y );
	Impresor->LineTo( x+Estado.DistanciaPuntos, y );

 }

 void ObjetoEjecucion::DibujarLineaEnlace( int ActualX, int ActualY ){
	ObjetoEjecucion *Sig= GetSiguienteParaAcomoda();
	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;
	int InicioLinea= cy+AltoAbajo[Tipo]*Estado.DistanciaPuntos;
	TipoObjeto TipoSiguiente= Sig->GetTipo();
	Graficador->MoveTo( cx, InicioLinea );
	if( TipoSiguiente==PUNTO ){
	  Graficador->LineTo( cx, int(Sig->GetPosicionY()-ActualY)*Estado.DistanciaPuntos );
	  Graficador->LineTo( int(Sig->GetPosicionX()-ActualX)*Estado.DistanciaPuntos,
								 int(Sig->GetPosicionY()-ActualY)*Estado.DistanciaPuntos );
	}
	else
	  Graficador->LineTo( cx, InicioLinea+LargoLinea*Estado.DistanciaPuntos
								 +AltoArriba[TipoSiguiente] );

 }

 void ObjetoEjecucion::DibujarLineaEnlaceImp( int ActualX, int ActualY ){
	ObjetoEjecucion *Sig= GetSiguienteParaAcomoda();
	int cx= PosicionX*Estado.DistanciaPuntos - ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos - ActualY ;
	int InicioLinea= cy+AltoAbajo[Tipo]*Estado.DistanciaPuntos;
	TipoObjeto TipoSiguiente= Sig->GetTipo();
	Impresor->MoveTo( cx, InicioLinea );
	if( TipoSiguiente==PUNTO ){
	  Impresor->LineTo( cx, Sig->GetPosicionY()*Estado.DistanciaPuntos-ActualY );
	  Impresor->LineTo( Sig->GetPosicionX()*Estado.DistanciaPuntos-ActualX,
								 Sig->GetPosicionY()*Estado.DistanciaPuntos-ActualY );
	}
	else
	  Impresor->LineTo( cx, InicioLinea+LargoLinea*Estado.DistanciaPuntos
								 +AltoArriba[TipoSiguiente] );

 }




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



 class OE_Inicio:public ObjetoEjecucion{
	  ObjetoEjecucion *Siguiente;
	  char *Descripcion;

	public:

	  OE_Inicio(){
		 Tipo= INICIO;
		 Descripcion= 0;
	  }

	  void Preprocesar(){}
	  void Despreprocesar(){}
	  void SetDescripcion( char *d ){
		 if( Descripcion )
			delete[] Descripcion;
		 Descripcion= d;
	  }
	  ObjetoEjecucion *Ejecutar(){
		 return Siguiente;
	  }
	  void Dibujar( int ActualX, int ActualY, int Rojo= 0 );
	  void DibujarImp( int ActualX, int ActualY );

	  void SetSiguiente( ObjetoEjecucion *Ob ){ Siguiente= Ob ; }
	  char *GetDescripcion(){ return Descripcion; }
	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }

 };


 void OE_Inicio::Dibujar( int ActualX, int ActualY, int Sw ){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;
	if( Sw==1 )      Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->Arc( cx-2*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos ,
						  cx,cy-2*Estado.DistanciaPuntos,cx+1,cy-2*Estado.DistanciaPuntos );
	if( Sw ) Graficador->SelectObject( *LapizSolido );
	DibujarLineaEnlace( ActualX, ActualY );
	TSize Tamanyo= Graficador->GetTextExtent(TXT_RUNNING_OBJECT_BEGIN, strlen(TXT_RUNNING_OBJECT_BEGIN));
	Graficador->TextOut( cx-Tamanyo.cx/2, cy-Tamanyo.cy/2, TXT_RUNNING_OBJECT_BEGIN);

 }


 void OE_Inicio::DibujarImp( int ActualX, int ActualY ){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;
	Impresor->Arc( cx-2*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos,
						cx+2*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos ,
						cx,cy-2*Estado.DistanciaPuntos,cx+1,cy-2*Estado.DistanciaPuntos );
	DibujarLineaEnlaceImp( ActualX, ActualY );
	TSize Tamanyo= Impresor->GetTextExtent(TXT_RUNNING_OBJECT_BEGIN, strlen(TXT_RUNNING_OBJECT_BEGIN));
	Impresor->TextOut( cx-Tamanyo.cx/2, cy-Tamanyo.cy/2, TXT_RUNNING_OBJECT_BEGIN);

 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class OE_Fin:public ObjetoEjecucion{

	public:
	  OE_Fin(){
		 Tipo= FIN;
	  }

	  void Preprocesar(){}
	  void Despreprocesar(){}
	  ObjetoEjecucion *Ejecutar(){
		 return 0;
	  }
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );


 };

 void OE_Fin::Dibujar( int ActualX, int ActualY, int Sw ){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 ) Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->Arc( cx-2*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos ,
						  cx,cy-2*Estado.DistanciaPuntos,cx+1,cy-2*Estado.DistanciaPuntos );
	if( Sw ) Graficador->SelectObject( *LapizSolido );

	DibujarFlecha( cx, cy-2*Estado.DistanciaPuntos-1 );
	TSize Tamanyo= Graficador->GetTextExtent(TXT_RUNNING_OBJECT_END, 3 );
	Graficador->TextOut( cx-Tamanyo.cx/2, cy-Tamanyo.cy/2, TXT_RUNNING_OBJECT_END);

 }

 void OE_Fin::DibujarImp( int ActualX, int ActualY ){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;


	Impresor->Arc( cx-2*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos,
						cx+2*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos ,
						cx,cy-2*Estado.DistanciaPuntos,cx+1,cy-2*Estado.DistanciaPuntos );

	DibujarFlechaImp( cx, cy-2*Estado.DistanciaPuntos-1 );
	TSize Tamanyo= Impresor->GetTextExtent(TXT_RUNNING_OBJECT_END, strlen(TXT_RUNNING_OBJECT_END));
	Impresor->TextOut( cx-Tamanyo.cx/2, cy-Tamanyo.cy/2, TXT_RUNNING_OBJECT_END);

 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class OE_Impresion:public ObjetoEjecucion{
	  char *Cadena;
	  ListaExpresiones Lista;
	  ObjetoEjecucion *Siguiente;

	public:
	  OE_Impresion(  ){
		 Tipo= IMPRESION;
		 Cadena= 0;
	  }
	  OE_Impresion( OE_Impresion* Ob);

	  void Preprocesar();
	  void Despreprocesar();
	  ObjetoEjecucion *Ejecutar();
	  void Dibujar(int ActualX, int ActualY, int Sw= 0);
	  void DibujarImp(int ActualX, int ActualY);

	  void SetCadena( char *c ){
		 if( Cadena ) delete[] Cadena;
		 Cadena= c;
	  }
	  void SetSiguiente( ObjetoEjecucion *Ob ){ Siguiente= Ob ; }
	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }
	  char *GetCadena(){ return Cadena; }

 };

 OE_Impresion::OE_Impresion( OE_Impresion* Ob ){

	Tipo= IMPRESION;
	if( Ob->Cadena ){
	  Cadena= new char[ strlen( Ob->Cadena )+1 ];
	  strcpy( Cadena, Ob->Cadena );
	}
	else
	  Cadena= 0;
 }

 void OE_Impresion::Preprocesar(){
	Lista.GetListaExpresiones( Cadena );
	return ;
 }

 void OE_Impresion::Despreprocesar(){
	Lista.Vacear();
	return;
 }



 bool  Imprime( char *Cadena );


 ObjetoEjecucion *OE_Impresion::Ejecutar()
 {
	Lista.Reset();
	int Cuantos= Lista.GetNItems();
	char *CadenaImpresion= 0;
	for( int i=0 ; i<Cuantos ; ++i ){
	  Token *Valor= EvaluaPostfijo( Lista.Itera() );
	  if(Buzon.GetHuboError()){
		 if( CadenaImpresion )
			delete[] CadenaImpresion;
		 return 0;
	  }
	  char *Aux= CadenaImpresion;
	  char *SiguienteValor= RetornaValorDeTokenEnCadena( Valor );
	  int Largo= strlen( SiguienteValor );
	  if( Aux ){
		 Largo+=strlen( Aux );
		 CadenaImpresion= new char[Largo+1];
		 sprintf( CadenaImpresion, "%s%s", Aux, SiguienteValor );
		 delete[] Aux;
	  }
	  else{
		 CadenaImpresion= new char[Largo+1];
		 strcpy( CadenaImpresion, SiguienteValor );
	  }

	  delete Valor;
	}
	if( CadenaImpresion ){
	  bool Detenido= Imprime( CadenaImpresion );
	  delete[] CadenaImpresion;
	  if( Detenido ){
		 Estado.Accion= DEPURACION;
		 return this;
	  }
	}

	return Siguiente;
 }

 void OE_Impresion::Dibujar(int ActualX, int ActualY, int Sw){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 ) Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->MoveTo( cx+3*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos );
	Graficador->LineTo( cx-4*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx+3*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Graficador->Arc( cx+2*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos,
						  cx+4*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos ,
						  cx+3*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos,
						  cx+3*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos );
	if( Sw ) Graficador->SelectObject( *LapizSolido );

	Graficador->MoveTo( cx, cy+int( 6.0/5.0*float(Estado.DistanciaPuntos ) ) );
	Graficador->LineTo( cx, cy+2*Estado.DistanciaPuntos );
	Graficador->MoveTo( cx, cy-int( 6.0/5.0*float(Estado.DistanciaPuntos ) ) );
	Graficador->LineTo( cx, cy-2*Estado.DistanciaPuntos-2 );
	DibujarFlecha( cx, cy-int(7.0/5.0*float(Estado.DistanciaPuntos)) );
	DibujarLineaEnlace( ActualX, ActualY );
	if( Cadena ){
	  TRect Region
	  ( cx-int(1.5*float(Estado.DistanciaPuntos)),cy-Estado.DistanciaPuntos/2,
		 cx+int(3.5*float(Estado.DistanciaPuntos)),cy+Estado.DistanciaPuntos/2);
	  TSize Tamanyo= Graficador->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx<5*Estado.DistanciaPuntos )
		 Graficador->DrawText(Cadena, -1, Region, DT_CENTER  );
	  else
		 Graficador->DrawText(Cadena, -1, Region );
	}
 }


 void OE_Impresion::DibujarImp(int ActualX, int ActualY){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;

	Impresor->MoveTo( cx+3*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos );
	Impresor->LineTo( cx-4*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx+3*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Impresor->Arc( cx+2*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos,
						  cx+4*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos ,
						  cx+3*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos,
						  cx+3*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos );

	Impresor->MoveTo( cx, cy+int( 6.0/5.0*float(Estado.DistanciaPuntos ) ) );
	Impresor->LineTo( cx, cy+2*Estado.DistanciaPuntos );
	Impresor->MoveTo( cx, cy-int( 6.0/5.0*float(Estado.DistanciaPuntos ) ) );
	Impresor->LineTo( cx, cy-2*Estado.DistanciaPuntos-2 );
	DibujarFlechaImp( cx, cy-int(7.0/5.0*float(Estado.DistanciaPuntos)) );
	DibujarLineaEnlaceImp( ActualX, ActualY );
	if( Cadena ){
	  TRect Region
	  ( cx-int(1.5*float(Estado.DistanciaPuntos)),cy-Estado.DistanciaPuntos/2,
		 cx+int(3.5*float(Estado.DistanciaPuntos)),cy+Estado.DistanciaPuntos/2);
	  TSize Tamanyo= Impresor->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx<5*Estado.DistanciaPuntos )
		 Impresor->DrawText(Cadena, -1, Region, DT_CENTER  );
	  else
		 Impresor->DrawText(Cadena, -1, Region );
	}
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class OE_Punto:public ObjetoEjecucion{
	ObjetoEjecucion *Siguiente;
	public:
	  OE_Punto(){   Tipo= PUNTO;  }
	  void Preprocesar(){}
	  void Despreprocesar(){}
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );
	  ObjetoEjecucion *Ejecutar(){ return Siguiente; }

	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }
	  void SetSiguiente( ObjetoEjecucion *s ){ Siguiente= s; }

 };

 void OE_Punto::Dibujar( int ActualX, int ActualY, int Sw ){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;
	if( Sw )
	  Graficador->SelectObject( *LapizAzul );
	else
	  Graficador->SelectObject( *LapizBlanco );
	TPoint Vector[4]={
	 TPoint( cx-Estado.DistanciaPuntos/2, cy-Estado.DistanciaPuntos/2 ),
	 TPoint( cx+Estado.DistanciaPuntos/2, cy-Estado.DistanciaPuntos/2 ),
	 TPoint( cx+Estado.DistanciaPuntos/2, cy+Estado.DistanciaPuntos/2 ),
	 TPoint( cx-Estado.DistanciaPuntos/2, cy+Estado.DistanciaPuntos/2 )
	};
	Graficador->MoveTo( Vector[0] );
	Graficador->LineTo( Vector[1] );
	Graficador->LineTo( Vector[2] );
	Graficador->LineTo( Vector[3] );
	Graficador->LineTo( Vector[0] );
	Graficador->SelectObject( *LapizSolido );
	if( !Sw ){
	  Graficador->MoveTo( cx-Estado.DistanciaPuntos/2-1, cy );
	  Graficador->LineTo( cx+Estado.DistanciaPuntos/2+1, cy );
	}
	DibujarLineaEnlace( ActualX, ActualY );
 }



 void OE_Punto::DibujarImp( int ActualX, int ActualY ){
	DibujarLineaEnlaceImp( ActualX, ActualY );
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


 class OE_Decision:public ObjetoEjecucion{
	  ObjetoEjecucion *Si;
	  ObjetoEjecucion *No;
	  OE_Punto *Punto;

	  char *Cadena;
	  Token *Expresion;
	  int BrazoI;
	  int BrazoD;
	  int AltoBrazos;
	  int SiIzquierda;

	public:
	  OE_Decision( ){
		 Tipo= DECISION;
		 Cadena= 0;
		 Expresion= 0;
		 SiIzquierda= 0;
	  }
	  OE_Decision( OE_Decision* Ob );
	  void Preprocesar();
	  void Despreprocesar();
	  ObjetoEjecucion *Ejecutar();
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );

	  void SetPunto( OE_Punto *p ){ Punto= p; }
	  void SetSi( ObjetoEjecucion *o ){ Si= o; }
	  void SetNo( ObjetoEjecucion *o ){ No= o; }
	  void SetCadena( char *c ){
		 if( Cadena ) delete[] Cadena;
		 Cadena= c;
	  }
	  void SetSiIzquierda( int Lado ){ SiIzquierda= Lado; }
	  ObjetoEjecucion *GetSi(){ return Si; }
	  ObjetoEjecucion *GetNo(){ return No; }
	  void SetBrazoI(int v){ BrazoI= v; }
	  void SetBrazoD(int v){ BrazoD= v; }
	  void SetAltoBrazos( int a ){ AltoBrazos= a; }
	  int GetBrazoI(){ return BrazoI; }
	  int GetBrazoD(){ return BrazoD; }
	  int GetAltoBrazos(){ return AltoBrazos; }
	  int GetSiIzquierda(){ return SiIzquierda; }
	  char *GetCadena(){ return Cadena; }
	  OE_Punto *GetPunto(){ return Punto; }

 };

 OE_Decision::OE_Decision( OE_Decision* Ob ){
	Tipo= DECISION;
	if( Ob->Cadena ){
	  Cadena= new char[ strlen( Ob->Cadena )+1 ];
	  strcpy( Cadena,Ob->Cadena );
	}
	else
	  Cadena= 0;
	SiIzquierda= Ob->SiIzquierda;
	Expresion= 0;
 }

 void OE_Decision::Preprocesar(){
	if( !Cadena ){
	  Buzon.Error(ILEGAL_COMA_O_CADENA_VACIA);
	  return;
	}
	Expresion= GetPostfijo( Cadena );
	if( Buzon.GetHuboError() )
	  return ;
	if( !Expresion )
	  //Un error pal buzon
	  Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );

	return;
 }


 void OE_Decision::Despreprocesar(){
	LiberarListaToken( Expresion );
	Expresion= 0;
	return;
 }


 ObjetoEjecucion *OE_Decision::Ejecutar(){
	Token *Res= EvaluaPostfijo( Expresion );
	if( Buzon.GetHuboError() )
	  return 0;
	if( Res->GetTipoDato()!=LOGICO ){
	  //Un error pal buzon
	  Buzon.Error( TIPO_DEBE_SER_LOGICO );
	  delete Res;
	  return 0;
	}
	bool r= Res->GetDatoLogico();
	delete Res;
	if( SiIzquierda )
	  return ( r==false )? No : Si ;
	return ( r==false )? Si : No ;
 }


 void OE_Decision::Dibujar( int ActualX, int ActualY, int Sw ){
	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 ) Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->MoveTo( cx-4*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx,cy+2*Estado.DistanciaPuntos );
	Graficador->LineTo( cx+4*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx,cy-2*Estado.DistanciaPuntos );
	Graficador->LineTo( cx-4*Estado.DistanciaPuntos,cy );
	if( Sw ) Graficador->SelectObject( *LapizSolido );

	TipoObjeto TipoDelSi= Si->GetTipo();
	TipoObjeto TipoDelNo= No->GetTipo();


	Graficador->LineTo( cx-(4+BrazoI)*Estado.DistanciaPuntos, cy );
	if( TipoDelSi==PUNTO ){
	  Graficador->LineTo( cx-(4+BrazoI)*Estado.DistanciaPuntos,
								 int(Punto->GetPosicionY()-ActualY)*Estado.DistanciaPuntos);
	  Graficador->LineTo( cx,int(Punto->GetPosicionY()-ActualY)*Estado.DistanciaPuntos );
	}
	else
	  Graficador->LineTo( cx-(4+BrazoI)*Estado.DistanciaPuntos,
								 cy+LargoLineaDecision*Estado.DistanciaPuntos );
	Graficador->MoveTo( cx+4*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx+(4+BrazoD)*Estado.DistanciaPuntos, cy );
	if( TipoDelNo==PUNTO ){
	  Graficador->LineTo( cx+(4+BrazoD)*Estado.DistanciaPuntos,
								 int(Punto->GetPosicionY()-ActualY)*Estado.DistanciaPuntos );
	  Graficador->LineTo( cx,int(Punto->GetPosicionY()-ActualY)*Estado.DistanciaPuntos );
	}
	else
	  Graficador->LineTo( cx+(4+BrazoD)*Estado.DistanciaPuntos,
								 cy+LargoLineaDecision*Estado.DistanciaPuntos );
	DibujarFlecha( cx, cy-2*Estado.DistanciaPuntos-1 );
	if( Cadena ){
	  TRect Region
	  ( cx-int(2.5*float(Estado.DistanciaPuntos)),cy-Estado.DistanciaPuntos/2,
		 cx+int(2.5*float(Estado.DistanciaPuntos)),cy+Estado.DistanciaPuntos/2);
	  TSize Tamanyo= Graficador->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx<5*Estado.DistanciaPuntos )
		 Graficador->DrawText(Cadena, -1, Region, DT_CENTER  );
	  else
		 Graficador->DrawText(Cadena, -1, Region );
	}
	if( SiIzquierda ){
	  Graficador->TextOut( cx-4*Estado.DistanciaPuntos,
									cy-2*Estado.DistanciaPuntos,
									TXT_RUNNING_OBJECT_YES);
	  Graficador->TextOut( cx+3*Estado.DistanciaPuntos,
									cy-2*Estado.DistanciaPuntos,
									TXT_RUNNING_OBJECT_NO);
	}
	else{
	  Graficador->TextOut( cx-4*Estado.DistanciaPuntos,
									cy-2*Estado.DistanciaPuntos,
									TXT_RUNNING_OBJECT_NO);
	  Graficador->TextOut( cx+3*Estado.DistanciaPuntos,
									cy-2*Estado.DistanciaPuntos,
									TXT_RUNNING_OBJECT_YES);
	}

 }



 void OE_Decision::DibujarImp( int ActualX, int ActualY ){
	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;

	Impresor->MoveTo( cx-4*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx,cy+2*Estado.DistanciaPuntos );
	Impresor->LineTo( cx+4*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx,cy-2*Estado.DistanciaPuntos );
	Impresor->LineTo( cx-4*Estado.DistanciaPuntos,cy );

	TipoObjeto TipoDelSi= Si->GetTipo();
	TipoObjeto TipoDelNo= No->GetTipo();


	Impresor->LineTo( cx-(4+BrazoI)*Estado.DistanciaPuntos, cy );
	if( TipoDelSi==PUNTO ){
	  Impresor->LineTo( cx-(4+BrazoI)*Estado.DistanciaPuntos,
										 Punto->GetPosicionY()*Estado.DistanciaPuntos-ActualY );
						  Impresor->LineTo( cx,Punto->GetPosicionY()*Estado.DistanciaPuntos-ActualY );
						 //int(Punto->GetPosicionY()-ActualY)*Estado.DistanciaPuntos);
	  //Impresor->LineTo( cx,int(Punto->GetPosicionY()-ActualY)*Estado.DistanciaPuntos );
	}
	else
	  Impresor->LineTo( cx-(4+BrazoI)*Estado.DistanciaPuntos,
								 cy+LargoLineaDecision*Estado.DistanciaPuntos );
	Impresor->MoveTo( cx+4*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx+(4+BrazoD)*Estado.DistanciaPuntos, cy );
	if( TipoDelNo==PUNTO ){
		  Impresor->LineTo( cx+(4+BrazoD)*Estado.DistanciaPuntos,
								 Punto->GetPosicionY()*Estado.DistanciaPuntos-ActualY );
	  Impresor->LineTo( cx,Punto->GetPosicionY()*Estado.DistanciaPuntos-ActualY );

	//	  Impresor->LineTo( cx+(4+BrazoD)*Estado.DistanciaPuntos,
//								 int(Punto->GetPosicionY()-ActualY)*Estado.DistanciaPuntos );
//	  Impresor->LineTo( cx,int(Punto->GetPosicionY()-ActualY)*Estado.DistanciaPuntos );
	}
	else
	  Impresor->LineTo( cx+(4+BrazoD)*Estado.DistanciaPuntos,
								 cy+LargoLineaDecision*Estado.DistanciaPuntos );
	DibujarFlechaImp( cx, cy-2*Estado.DistanciaPuntos-1 );
	if( Cadena ){
	  TRect Region
	  ( cx-int(2.5*float(Estado.DistanciaPuntos)),cy-Estado.DistanciaPuntos/2,
		 cx+int(2.5*float(Estado.DistanciaPuntos)),cy+Estado.DistanciaPuntos/2);
	  TSize Tamanyo= Impresor->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx<5*Estado.DistanciaPuntos )
		 Impresor->DrawText(Cadena, -1, Region, DT_CENTER  );
	  else
		 Impresor->DrawText(Cadena, -1, Region );
	}
	if( SiIzquierda ){
	  Impresor->TextOut( cx-4*Estado.DistanciaPuntos,
									cy-2*Estado.DistanciaPuntos,
									TXT_RUNNING_OBJECT_YES);
	  Impresor->TextOut( cx+3*Estado.DistanciaPuntos,
									cy-2*Estado.DistanciaPuntos,
									TXT_RUNNING_OBJECT_NO);
	}
	else{
	  Impresor->TextOut( cx-4*Estado.DistanciaPuntos,
									cy-2*Estado.DistanciaPuntos,
									TXT_RUNNING_OBJECT_NO);
	  Impresor->TextOut( cx+3*Estado.DistanciaPuntos,
									cy-2*Estado.DistanciaPuntos,
									TXT_RUNNING_OBJECT_YES);
	}

 }




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class OE_Lectura:public ObjetoEjecucion{
	  char *Cadena;
	  ListaVectores Lista;
	  ObjetoEjecucion *Siguiente;
	public:
	  OE_Lectura( ){
		 Tipo= LECTURA;
		 Cadena= 0;
	  }
	  OE_Lectura( OE_Lectura* Ob );
	  void Preprocesar();
	  void Despreprocesar();
	  ObjetoEjecucion *Ejecutar();
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );

	  void SetCadena( char *c ){
		 if( Cadena ) delete[] Cadena;
		 Cadena= c;
	  }
	  void SetSiguiente( ObjetoEjecucion *Ob ){ Siguiente= Ob; }
	  char *GetCadena(){ return Cadena; }
	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }

 };

 OE_Lectura::OE_Lectura( OE_Lectura* Ob ){
	Tipo= LECTURA;

	if( Ob->Cadena ){
	  Cadena= new char[ strlen( Ob->Cadena )+1 ];
	  strcpy( Cadena,Ob->Cadena );
	}
	else
	  Cadena= 0;
 }

 void OE_Lectura::Preprocesar(){
	if( !Cadena ){
	  Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
	  return ;
	}
	int Parent= 0;
	int IniChar= 0;
	int Largo= strlen(Cadena);
	int ActualChar;
	for( ActualChar=0 ; ActualChar<=Largo ; ++ActualChar ){
	  char &Casilla=Cadena[ActualChar];
	  if( Casilla=='(' )
		 ++Parent;
	  else if( Casilla==')' )
		 --Parent;
	  else if( Casilla==0 || (Casilla==','&& !Parent )){
		 char Temp= Casilla;
		 Casilla= 0;
		 Lista.AlmacenaVector( Cadena+IniChar );
		 Casilla=Temp;
		 IniChar=1+ActualChar;
		 if(Buzon.GetHuboError())
			return;
	  }
	}
	return;
 }


 void OE_Lectura::Despreprocesar(){
	Lista.Vacear();
 }



 Token *EntradaTeclado();



 ObjetoEjecucion *OE_Lectura::Ejecutar(){
	Token *Entrada;
	int n= Lista.GetNItems();
	Lista.Reset();
	for( int i= 0; i<n; ++i ){
	  Lista.EvaluaActualesIndices();


	  if( Buzon.GetHuboError() )
		 return 0;


	  Entrada= EntradaTeclado();

	  if( !Entrada ){
		 Estado.Accion= DEPURACION;
		 return this;
	  }
	  unsigned int *Vec= Lista.GetActualVectorIndices();
	  int NIndices= Lista.GetActualLista().GetNItems();
	  PilaDeTablas.AsignarValor( Lista.GetActualIdentificador() , Entrada, Vec,
									  NIndices );

	  delete Entrada;

	  if( Buzon.GetHuboError() )
		 return 0;
	  Lista.Itera();

	}//ciclo de preprocesamiento de los indices y lectura


	return Siguiente;

 }


 void OE_Lectura::Dibujar( int ActualX, int ActualY, int Sw ){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 ) Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->MoveTo( cx-4*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx-4*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Graficador->LineTo( cx+4*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Graficador->LineTo( cx+4*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos );
	Graficador->LineTo( cx-4*Estado.DistanciaPuntos,cy );
	if( Sw )
	  Graficador->SelectObject( *LapizSolido );

	Graficador->MoveTo( cx, cy-Estado.DistanciaPuntos );
	Graficador->LineTo( cx, cy-2*Estado.DistanciaPuntos-2 );
	DibujarFlecha( cx, cy-Estado.DistanciaPuntos-2 );
	DibujarLineaEnlace(ActualX, ActualY);
	if( Cadena ){
	  TRect Region
	  ( cx-int(3.5*float(Estado.DistanciaPuntos)),cy,
		 cx+int(3.5*float(Estado.DistanciaPuntos)),cy+Estado.DistanciaPuntos);
	  TSize Tamanyo= Graficador->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx<7*Estado.DistanciaPuntos )
		 Graficador->DrawText(Cadena, -1, Region, DT_CENTER  );
	  else
		 Graficador->DrawText(Cadena, -1, Region );
	}

 }


 void OE_Lectura::DibujarImp( int ActualX, int ActualY ){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;

	Impresor->MoveTo( cx-4*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx-4*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Impresor->LineTo( cx+4*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Impresor->LineTo( cx+4*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos );
	Impresor->LineTo( cx-4*Estado.DistanciaPuntos,cy );

	Impresor->MoveTo( cx, cy-Estado.DistanciaPuntos );
	Impresor->LineTo( cx, cy-2*Estado.DistanciaPuntos-2 );
	DibujarFlechaImp( cx, cy-Estado.DistanciaPuntos-2 );
	DibujarLineaEnlaceImp(ActualX, ActualY);
	if( Cadena ){
	  TRect Region
	  ( cx-int(3.5*float(Estado.DistanciaPuntos)),cy,
		 cx+int(3.5*float(Estado.DistanciaPuntos)),cy+Estado.DistanciaPuntos);
	  TSize Tamanyo= Impresor->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx<7*Estado.DistanciaPuntos )
		 Impresor->DrawText(Cadena, -1, Region, DT_CENTER  );
	  else
		 Impresor->DrawText(Cadena, -1, Region );
	}

 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



 class OE_Asignacion:public ObjetoEjecucion{
	  ListaVectores Destinos;
	  ListaExpresiones Fuentes;
	  ListaCadenas CadenasDestino, CadenasFuente;
	  ObjetoEjecucion *Siguiente;
	public:
	  OE_Asignacion( ){
		 Tipo= ASIGNACION;
	  }
	  OE_Asignacion( OE_Asignacion* Ob );
	  void Preprocesar();
	  void Despreprocesar();
	  ObjetoEjecucion *Ejecutar();

	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );

	  void SetSiguiente( ObjetoEjecucion *Ob ){ Siguiente= Ob ; }
	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }
	  ListaCadenas &GetCadenasDestino(){ return CadenasDestino; }
	  ListaCadenas &GetCadenasFuente() { return CadenasFuente;  }

 };

  OE_Asignacion::OE_Asignacion( OE_Asignacion* Ob ){
	 Tipo= ASIGNACION;
	 Ob->CadenasDestino.Reset();
	 Ob->CadenasFuente.Reset();
	 int Items= Ob->CadenasDestino.GetNItems();
	 char *AuxCd1,*AuxCf1,*AuxCd2,*AuxCf2;
	 for( int i=0; i<Items; ++i ){
		AuxCd1= Ob->CadenasDestino.Itera();
		AuxCf1= Ob->CadenasFuente.Itera();
		AuxCd2= new char[ strlen( AuxCd1 )+1 ];
		AuxCf2= new char[ strlen( AuxCf1 )+1 ];
		strcpy( AuxCd2, AuxCd1 );
		strcpy( AuxCf2, AuxCf1 );
		CadenasDestino.Insertar( AuxCd2 );
		CadenasFuente.Insertar( AuxCf2 );
	 }

  }

  void OE_Asignacion::Preprocesar(){
	 int n= CadenasDestino.GetNItems();
	 if( n==0 ){
		Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
		return;
	 }
	 CadenasDestino.Reset();
	 CadenasFuente.Reset();

	 for( int i=0 ; i<n ; ++i ){
		Destinos.AlmacenaVector( CadenasDestino.Itera());
		if( Buzon.GetHuboError())
		  return ;
		Token *Temp= GetPostfijo( CadenasFuente.Itera());
		if( Buzon.GetHuboError())
		  return;
		if( !Temp ){
		  Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
		  return;
		}
		Fuentes.Insertar( Temp );
	 }

  return;
  }

  void OE_Asignacion::Despreprocesar(){
	 Destinos.Vacear();
	 Fuentes.Vacear();
  }

  ObjetoEjecucion *OE_Asignacion::Ejecutar(){
	 int n= Destinos.GetNItems();
	 Destinos.Reset();
	 Fuentes.Reset();
	 for( int i= 0; i<n; ++i ){
		Destinos.EvaluaActualesIndices();
		if( Buzon.GetHuboError() )
		  return 0;
		char *Id= Destinos.GetActualIdentificador();
		unsigned int *Vec= Destinos.GetActualVectorIndices();
		int NInd= Destinos.GetActualLista().GetNItems();
		Token *t= EvaluaPostfijo( Fuentes.Itera() );
		if( Buzon.GetHuboError() )
		  return 0;


		PilaDeTablas.AsignarValor( Id, t, Vec, NInd );
		delete t;
		if( Buzon.GetHuboError() )
		  return 0;
		Destinos.Itera();
	 }
	 return Siguiente;
  }

 void OE_Asignacion::Dibujar(int ActualX, int ActualY, int Sw ){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 ) Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->MoveTo( cx-4*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos );
	Graficador->LineTo( cx-4*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Graficador->LineTo( cx+4*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Graficador->LineTo( cx+4*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos );
	Graficador->LineTo( cx-4*Estado.DistanciaPuntos,cy -2*Estado.DistanciaPuntos );
	if( Sw )
	  Graficador->SelectObject( *LapizSolido );

	DibujarFlecha( cx, cy-2*Estado.DistanciaPuntos-1 );
	DibujarLineaEnlace(ActualX, ActualY);

	int AvanceEnY;
	int n= CadenasDestino.GetNItems();
	CadenasFuente.Reset();
	CadenasDestino.Reset();
	if( n==3 )
	  AvanceEnY=  cy-int(1.75*float(Estado.DistanciaPuntos));
	else if( n==2 )
	  AvanceEnY=  cy-int(1.12*float(Estado.DistanciaPuntos));
	else
	  AvanceEnY=  cy-int(0.5*float(Estado.DistanciaPuntos));

	//int( 1.25*float(Estado.DistanciaPuntos));


	for( int i= 0; i<n ; ++i ){

	  //Cadenas destino
	  TRect Region( cx-int(3.75*float(Estado.DistanciaPuntos)), AvanceEnY,
						 cx-Estado.DistanciaPuntos,
						 AvanceEnY+Estado.DistanciaPuntos );
	  char *Cadena= CadenasDestino.Itera();
	  TSize Tamanyo= Graficador->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx< int(2.75*float(Estado.DistanciaPuntos)) )
		 Graficador->DrawText( Cadena,-1, Region, DT_CENTER );
	  else
		 Graficador->DrawText( Cadena,-1, Region );

	  //Cadenas Fuente
	  Region.Offset( int(4.75*float(Estado.DistanciaPuntos)), 0 );
	  Cadena= CadenasFuente.Itera();
	  Tamanyo= Graficador->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx< int(2.75*float(Estado.DistanciaPuntos)) )
		 Graficador->DrawText( Cadena,-1, Region, DT_CENTER );
	  else
		 Graficador->DrawText( Cadena,-1, Region );

	  // Flecha
	  DibujarFlechaAsignacion( cx-Estado.DistanciaPuntos/2,
										AvanceEnY+Estado.DistanciaPuntos/2 );
	  AvanceEnY+= int( 1.25*float(Estado.DistanciaPuntos));
	}
 }


 void OE_Asignacion::DibujarImp(int ActualX, int ActualY){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;

	Impresor->MoveTo( cx-4*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos );
	Impresor->LineTo( cx-4*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Impresor->LineTo( cx+4*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Impresor->LineTo( cx+4*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos );
	Impresor->LineTo( cx-4*Estado.DistanciaPuntos,cy -2*Estado.DistanciaPuntos );

	DibujarFlechaImp( cx, cy-2*Estado.DistanciaPuntos-1 );
	DibujarLineaEnlaceImp(ActualX, ActualY);

	int AvanceEnY;
	int n= CadenasDestino.GetNItems();
	CadenasFuente.Reset();
	CadenasDestino.Reset();
	if( n==3 )
	  AvanceEnY=  cy-int(1.75*float(Estado.DistanciaPuntos));
	else if( n==2 )
	  AvanceEnY=  cy-int(1.12*float(Estado.DistanciaPuntos));
	else
	  AvanceEnY=  cy-int(0.5*float(Estado.DistanciaPuntos));

	//int( 1.25*float(Estado.DistanciaPuntos));


	for( int i= 0; i<n ; ++i ){

	  //Cadenas destino
	  TRect Region( cx-int(3.75*float(Estado.DistanciaPuntos)), AvanceEnY,
						 cx-Estado.DistanciaPuntos,
						 AvanceEnY+Estado.DistanciaPuntos );
	  char *Cadena= CadenasDestino.Itera();
	  TSize Tamanyo= Impresor->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx< int(2.75*float(Estado.DistanciaPuntos)) )
		 Impresor->DrawText( Cadena,-1, Region, DT_CENTER );
	  else
		 Impresor->DrawText( Cadena,-1, Region );

	  //Cadenas Fuente
	  Region.Offset( int(4.75*float(Estado.DistanciaPuntos)), 0 );
	  Cadena= CadenasFuente.Itera();
	  Tamanyo= Impresor->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx< int(2.75*float(Estado.DistanciaPuntos)) )
		 Impresor->DrawText( Cadena,-1, Region, DT_CENTER );
	  else
		 Impresor->DrawText( Cadena,-1, Region );

	  // Flecha
	  DibujarFlechaAsignacionImp( cx-Estado.DistanciaPuntos/2,
										AvanceEnY+Estado.DistanciaPuntos/2 );
	  AvanceEnY+= int( 1.25*float(Estado.DistanciaPuntos));
	}
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class OE_CicloPara;

 class OE_CierrePara:public ObjetoEjecucion{
	  ObjetoEjecucion *Siguiente;
	  OE_CicloPara *Ciclo;

	public:
	  void Preprocesar(){}
	  void Despreprocesar(){}
	  OE_CierrePara(){
		 Tipo= CIERREPARA;
	  }
	  ObjetoEjecucion *Ejecutar();
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );

	  void SetSiguiente( ObjetoEjecucion *Ob ){ Siguiente= Ob ; }
	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }
	  void SetCiclo( OE_CicloPara *c ){ Ciclo= c; }

 };



 class OE_CicloPara:public ObjetoEjecucion{
	  char *CadenaId, *CadenaVInicial, *CadenaVFinal, *CadenaIncremento;
	  ListaVectores Contador;
	  Token *VInicial, *VFinal, *Id;
	  long double Incremento;
	  ObjetoEjecucion *Siguiente;
	  OE_CierrePara *Cierre;

	  struct Nodo{
		 Nodo(){
			ContadorFor++;
		 }
		 ~Nodo(){
			ContadorFor--;
		 }
		 bool Valor;
		 Nodo *Siguiente;
	  }*PrimeraVez;


	public:
	  OE_CicloPara( ){
		 Tipo= CICLOPARA;
		 VInicial= VFinal= Id= 0;
		 CadenaId= 0;
		 CadenaVInicial= 0;
		 CadenaVFinal= 0;
		 CadenaIncremento= 0;
		 PrimeraVez= 0;
	  }
	  OE_CicloPara( OE_CicloPara* Ob);
	  void Preprocesar();
	  void Despreprocesar();
	  ObjetoEjecucion *Ejecutar();
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );
	  void SetCadenaId( char *c ){
		 if( CadenaId ) delete[] CadenaId;
		 CadenaId= c;
	  }
	  void SetCadenaVInicial( char *c ){
		 if( CadenaVInicial ) delete[] CadenaVInicial;
		 CadenaVInicial= c;
	  }
	  void SetCadenaVFinal( char *c ){
		 if( CadenaVFinal ) delete[] CadenaVFinal;
		 CadenaVFinal= c;
	  }
	  void SetCadenaIncremento( char *c ){
		 if( CadenaIncremento ) delete[] CadenaIncremento;
		 CadenaIncremento= c;
	  }
	  void SetSiguiente( ObjetoEjecucion *Ob ){ Siguiente= Ob ; }
	  void SetCierre( OE_CierrePara *c ){ Cierre= c; }
	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }
	  char *GetCadenaId(){ return CadenaId; }
	  char *GetCadenaVInicial(){ return CadenaVInicial; }
	  char *GetCadenaVFinal(){ return CadenaVFinal; }
	  char *GetCadenaIncremento(){ return CadenaIncremento; }
	  OE_CierrePara *GetCierre(){return Cierre; }
	  void Incrementar();
	  void ApilaPrimeraVez();
	  void DesapilaPrimeraVez();
 };

 void OE_CicloPara::ApilaPrimeraVez(){
	Nodo *Nuevo= new Nodo;
	Nuevo->Valor= true;
	Nuevo->Siguiente= PrimeraVez;
	PrimeraVez= Nuevo;
 }


 void OE_CicloPara::DesapilaPrimeraVez(){
	Nodo *Aux= PrimeraVez;
	PrimeraVez= PrimeraVez->Siguiente;
	delete Aux;
 }



 OE_CicloPara::OE_CicloPara( OE_CicloPara* Ob ){
	Tipo= CICLOPARA;
	VInicial= VFinal= Id= 0;
	PrimeraVez= 0;
	if( Ob->CadenaId ){
	  CadenaId= new char[ strlen( Ob->CadenaId )+1 ];
	  strcpy( CadenaId, Ob->CadenaId );
	}
	else
	  CadenaId= 0;

	if( Ob->CadenaVInicial ){
	  CadenaVInicial= new char[ strlen( Ob->CadenaVInicial )+1 ];
	  strcpy( CadenaVInicial, Ob->CadenaVInicial );
	}
	else
	  CadenaVInicial= 0;

	if( Ob->CadenaVFinal ){
	  CadenaVFinal= new char[ strlen( Ob->CadenaVFinal )+1 ];
	  strcpy( CadenaVFinal, Ob->CadenaVFinal );
	}
	else
	  CadenaVFinal= 0;

	if( Ob->CadenaIncremento ){
	  CadenaIncremento= new char[ strlen( Ob->CadenaIncremento )+1 ];
	  strcpy( CadenaIncremento, Ob->CadenaIncremento );
	}
	else
	  CadenaIncremento= 0;
 }




 void OE_CicloPara::Dibujar(int ActualX, int ActualY, int Sw ){
	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 ) Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->MoveTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->LineTo( cx-7*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx-6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx+6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx+7*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx+6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->LineTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	if( Sw )
	  Graficador->SelectObject( *LapizSolido );
	DibujarFlecha( cx, cy-Estado.DistanciaPuntos-1 );
	DibujarLineaEnlace(ActualX, ActualY);
	Graficador->
	TextOut(cx-6*Estado.DistanciaPuntos, cy-Estado.DistanciaPuntos/2, TXT_RUNNING_OBJECT_FOR);

	if( CadenaId ){
	  TRect Region
	  ( cx-4*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos/2,
		 cx-2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos/2 );

	  //CadenaId
	  TSize Tamanyo= Graficador->GetTextExtent( CadenaId, strlen( CadenaId ) );
	  if( Tamanyo.cx<2*Estado.DistanciaPuntos )
		 Graficador->DrawText(CadenaId, -1, Region, DT_CENTER  );
	  else
		 Graficador->DrawText(CadenaId, -1, Region );

	  //CadenaVInicial
	  Region.Offset( 4*Estado.DistanciaPuntos, 0);
	  Tamanyo= Graficador->GetTextExtent
								 ( CadenaVInicial, strlen( CadenaVInicial ) );
	  if( Tamanyo.cx<2*Estado.DistanciaPuntos )
		 Graficador->DrawText(CadenaVInicial, -1, Region, DT_CENTER  );
	  else
		 Graficador->DrawText(CadenaVInicial, -1, Region );

	  //CadenaVFinal
	  Region.Offset( int(5.0/2.0*float(Estado.DistanciaPuntos)), 0);
	  Tamanyo= Graficador->GetTextExtent
								 ( CadenaVFinal, strlen( CadenaVFinal ) );
	  if( Tamanyo.cx<2*Estado.DistanciaPuntos )
		 Graficador->DrawText(CadenaVFinal, -1, Region, DT_CENTER  );
	  else
		 Graficador->DrawText(CadenaVFinal, -1, Region );

	  //CadenaIncremento
	  Region.Offset( 2*Estado.DistanciaPuntos, 0);
	  Region.Inflate( -Estado.DistanciaPuntos/2, 0 );
	  Tamanyo= Graficador->GetTextExtent
								 ( CadenaIncremento, strlen( CadenaIncremento ) );
	  if( Tamanyo.cx<Estado.DistanciaPuntos )
		 Graficador->DrawText(CadenaIncremento, -1, Region, DT_CENTER  );
	  else
		 Graficador->DrawText(CadenaIncremento, -1, Region );

	  // Comas y flecha de asignacion
	  Graficador->
	  TextOut(cx+int(2.25*float(Estado.DistanciaPuntos)),
				 cy-Estado.DistanciaPuntos/2, ",");
	  if( CadenaIncremento )
		 if( CadenaIncremento[0] )
			Graficador->
			TextOut(cx+int(4.75*float(Estado.DistanciaPuntos)),
					cy-Estado.DistanciaPuntos/2, ",");
	  DibujarFlechaAsignacion( cx-int(1.5*float(Estado.DistanciaPuntos)),
										cy-int(float(Estado.DistanciaPuntos)/6.0));

	}

 }


 void OE_CicloPara::DibujarImp(int ActualX, int ActualY ){
	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;

	Impresor->MoveTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->LineTo( cx-7*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx-6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx+6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx+7*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx+6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->LineTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	DibujarFlechaImp( cx, cy-Estado.DistanciaPuntos-1 );
	DibujarLineaEnlaceImp(ActualX, ActualY);
	Impresor->
	TextOut(cx-6*Estado.DistanciaPuntos, cy-Estado.DistanciaPuntos/2, TXT_RUNNING_OBJECT_FOR);

	if( CadenaId ){
	  TRect Region
	  ( cx-4*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos/2,
		 cx-2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos/2 );

	  //CadenaId
	  TSize Tamanyo= Impresor->GetTextExtent( CadenaId, strlen( CadenaId ) );
	  if( Tamanyo.cx<2*Estado.DistanciaPuntos )
		 Impresor->DrawText(CadenaId, -1, Region, DT_CENTER  );
	  else
		 Impresor->DrawText(CadenaId, -1, Region );

	  //CadenaVInicial
	  Region.Offset( 4*Estado.DistanciaPuntos, 0);
	  Tamanyo= Impresor->GetTextExtent
								 ( CadenaVInicial, strlen( CadenaVInicial ) );
	  if( Tamanyo.cx<2*Estado.DistanciaPuntos )
		 Impresor->DrawText(CadenaVInicial, -1, Region, DT_CENTER  );
	  else
		 Impresor->DrawText(CadenaVInicial, -1, Region );

	  //CadenaVFinal
	  Region.Offset( int(5.0/2.0*float(Estado.DistanciaPuntos)), 0);
	  Tamanyo= Impresor->GetTextExtent
								 ( CadenaVFinal, strlen( CadenaVFinal ) );
	  if( Tamanyo.cx<2*Estado.DistanciaPuntos )
		 Impresor->DrawText(CadenaVFinal, -1, Region, DT_CENTER  );
	  else
		 Impresor->DrawText(CadenaVFinal, -1, Region );

	  //CadenaIncremento
	  Region.Offset( 2*Estado.DistanciaPuntos, 0);
	  Region.Inflate( -Estado.DistanciaPuntos/2, 0 );
	  Tamanyo= Impresor->GetTextExtent
								 ( CadenaIncremento, strlen( CadenaIncremento ) );
	  if( Tamanyo.cx<Estado.DistanciaPuntos )
		 Impresor->DrawText(CadenaIncremento, -1, Region, DT_CENTER  );
	  else
		 Impresor->DrawText(CadenaIncremento, -1, Region );

	  // Comas y flecha de asignacion
	  Impresor->
	  TextOut(cx+int(2.25*float(Estado.DistanciaPuntos)),
				 cy-Estado.DistanciaPuntos/2, ",");
	  if( CadenaIncremento )
		 if( CadenaIncremento[0] )
			Impresor->
			TextOut(cx+int(4.75*float(Estado.DistanciaPuntos)),
					cy-Estado.DistanciaPuntos/2, ",");
	  DibujarFlechaAsignacionImp( cx-int(1.5*float(Estado.DistanciaPuntos)),
										cy-int(float(Estado.DistanciaPuntos)/6.0));

	}

 }


 void OE_CicloPara::Preprocesar(){

	Contador.AlmacenaVector( CadenaId );
	ApilaPrimeraVez();
	if( Buzon.GetHuboError() )
	  return;
	Id= GetPostfijo( CadenaId );
	if( Buzon.GetHuboError() )
	  return;
	if(!Id ){
	  Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
	  return ;
	}
	VInicial= GetPostfijo( CadenaVInicial );
	if( Buzon.GetHuboError() )
	  return;
	if( !VInicial ){
	  //Se necesita un valor inicial para el contador
	  //un error pa'l buzon
	  Buzon.Error( VINICIAL_CONTADOR );
	  return ;
	}

	VFinal= GetPostfijo( CadenaVFinal );
	if( Buzon.GetHuboError() )
	  return;
	if( !VFinal ){
	  //Se necesita un valor final para el contador
	  //un error pa'l buzon
	  Buzon.Error( VFINAL_CONTADOR );
	  return ;
	}

	Token *t= GetPostfijo( CadenaIncremento );
	if( Buzon.GetHuboError() )
	  return;
	if( !t ){
	  Incremento= 1.0L;
	  return;
	}
	Token *Aux= t->GetSig();
	if( Aux ){
	  if( (Aux->GetTipoToken()==OPERANDO) || Aux->GetSig() ||
			( (Aux->GetDatoStr()[0]!='-')  && (Aux->GetDatoStr()[0]!='+') ) ){
		 //El incremento debe ser una constante real
		 //un error pa'l buzon
		 LiberarListaToken(t);
		 Buzon.Error( INCREMENTO_CONSTANTE );
		 return ;
	  }
	}
	if( ( t->GetTipoAlmacenamiento()!=CONSTANTE ) || t->GetTipoDato()!=REAL ){
	  //El incremento debe ser una constante real
	  //un error pa'l buzon
	  LiberarListaToken(t);
	  Buzon.Error( INCREMENTO_CONSTANTE );
	  return ;
	}
	Token *Aux2= t;
	t= EvaluaPostfijo( t );
	LiberarListaToken( Aux2 );
	if( Buzon.GetHuboError() )
	  return;
	Incremento= t->GetDatoReal();
	delete t;
 }



 void OE_CicloPara::Despreprocesar(){
	while( PrimeraVez )
	  DesapilaPrimeraVez();
	LiberarListaToken( VInicial );
	LiberarListaToken( VFinal );
	LiberarListaToken( Id );
	VInicial= VFinal= Id= 0;
	Contador.Vacear();
 }


 ObjetoEjecucion *OE_CicloPara::Ejecutar(){
	if( PrimeraVez->Valor ){
	  Contador.EvaluaActualesIndices();
	  Token *t= EvaluaPostfijo( VInicial );
	  if( Buzon.GetHuboError() )
		 return 0;
	  if( t->GetTipoDato()!=REAL ){
		 //El valor del contador debe ser de tipo real
		 //un error pa'l buzon
		 Buzon.Error( CONTADOR_REAL );
		 delete t;
		 return 0;
	  }

	  PilaDeTablas.AsignarValor( Contador.GetActualIdentificador(),
						t, Contador.GetActualVectorIndices(),
						Contador.GetActualLista().GetNItems() );
	  delete t;
	  if( Buzon.GetHuboError() )
		 return 0;
	  PrimeraVez->Valor= false;
	}

	Token *t= EvaluaPostfijo( VFinal );
	if( Buzon.GetHuboError() )
	  return 0;
	if( t->GetTipoDato()!=REAL ){
	  //el valor limite para contador debe ser de tipo real
	  //un error pal buzon
	  Buzon.Error( LIMITE_REAL );
	  delete t;
	  return 0;
	}
	long double ValorFinal= t->GetDatoReal();
	delete t;
	Contador.EvaluaActualesIndices();
	if( Buzon.GetHuboError() )
	  return 0;
	t= PilaDeTablas.Leer( Contador.GetActualIdentificador(),
								 Contador.GetActualVectorIndices(),
								 Contador.GetActualLista().GetNItems() );
	if( Buzon.GetHuboError() )
	  return 0;
	long double ValorVariable= t->GetDatoReal();
	delete t;
	if( Incremento<0L ){
	  if( ValorVariable<ValorFinal ){
		 PrimeraVez->Valor= true;
		 return Cierre->GetSiguiente();
	  }
	  return Siguiente;
	}
	if( ValorVariable>ValorFinal ){
	  PrimeraVez->Valor= true;
	  return Cierre->GetSiguiente();
	}
	return Siguiente;
 }



 void OE_CicloPara::Incrementar(){
	Token *t;
	Contador.EvaluaActualesIndices();
	if( Buzon.GetHuboError() )
	  return;
	t= PilaDeTablas.Leer( Contador.GetActualIdentificador(),
								 Contador.GetActualVectorIndices(),
								 Contador.GetActualLista().GetNItems() );
	if( Buzon.GetHuboError() )
	  return;
	long double ValorVariable= t->GetDatoReal();
	delete t;
	ValorVariable+= Incremento;
	Token t2( ValorVariable, OPERANDO, CONSTANTE );
	Contador.EvaluaActualesIndices();
	PilaDeTablas.AsignarValor( Contador.GetActualIdentificador(),
						&t2, Contador.GetActualVectorIndices(),
						Contador.GetActualLista().GetNItems() );
 }


 ObjetoEjecucion *OE_CierrePara::Ejecutar(){
	Ciclo->Incrementar();
	if( Buzon.GetHuboError() )
	  return 0;
	return Ciclo;
 }

 void OE_CierrePara::Dibujar(int ActualX, int ActualY, int Sw ){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 )Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->MoveTo( cx+2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->LineTo( cx-2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->Arc( cx-3*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx-Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx-2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx-2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos	);
	Graficador->MoveTo( cx-2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx+2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->Arc( cx+Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx+3*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos	);
	if( Sw )
	  Graficador->SelectObject( *LapizSolido );

	DibujarFlecha( cx, cy-Estado.DistanciaPuntos-1 );

	DibujarLineaEnlace(ActualX, ActualY);
	TSize Tamanyo= Graficador->GetTextExtent(TXT_RUNNING_OBJECT_BLOCK_END, strlen(TXT_RUNNING_OBJECT_BLOCK_END));
	Graficador->TextOut( cx-Tamanyo.cx/2, cy-Tamanyo.cy/2, TXT_RUNNING_OBJECT_BLOCK_END);

 }


 void OE_CierrePara::DibujarImp(int ActualX, int ActualY ){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;

	Impresor->MoveTo( cx+2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->LineTo( cx-2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->Arc( cx-3*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx-Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx-2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx-2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos	);
	Impresor->MoveTo( cx-2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx+2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->Arc( cx+Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx+3*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos	);

	DibujarFlechaImp( cx, cy-Estado.DistanciaPuntos-1 );

	DibujarLineaEnlaceImp(ActualX, ActualY);
	TSize Tamanyo= Impresor->GetTextExtent(TXT_RUNNING_OBJECT_BLOCK_END, strlen(TXT_RUNNING_OBJECT_BLOCK_END));
	Impresor->TextOut( cx-Tamanyo.cx/2, cy-Tamanyo.cy/2, TXT_RUNNING_OBJECT_BLOCK_END);

 }



 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class OE_CicloMientras;

 class OE_CierreMientras:public ObjetoEjecucion{
	  ObjetoEjecucion *Siguiente;
	  OE_CicloMientras *Ciclo;
	public:
	  OE_CierreMientras(){
		 Tipo= CIERREMIENTRAS;
	  }

	  void Preprocesar(){}
	  void Despreprocesar(){}
	  void SetCiclo(OE_CicloMientras *c){ Ciclo= c; }
	  ObjetoEjecucion *Ejecutar(){ return (ObjetoEjecucion *)Ciclo; }
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );

	  void SetSiguiente( ObjetoEjecucion *Ob ){ Siguiente= Ob; }
	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }
 };


 void OE_CierreMientras::Dibujar(int ActualX, int ActualY, int Sw ){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 ) Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->MoveTo( cx+2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->LineTo( cx-2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->Arc( cx-3*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx-Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx-2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx-2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos	);
	Graficador->MoveTo( cx-2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx+2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->Arc( cx+Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx+3*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos	);
	if( Sw )
	  Graficador->SelectObject( *LapizSolido );
	DibujarFlecha( cx, cy-Estado.DistanciaPuntos-1 );
	DibujarLineaEnlace(ActualX, ActualY);
	TSize Tamanyo= Graficador->GetTextExtent(TXT_RUNNING_OBJECT_BLOCK_END, strlen(TXT_RUNNING_OBJECT_BLOCK_END));
	Graficador->TextOut(cx-Tamanyo.cx/2, cy-Tamanyo.cy/2, strlen(TXT_RUNNING_OBJECT_BLOCK_END));

 }


 void OE_CierreMientras::DibujarImp(int ActualX, int ActualY ){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;

	Impresor->MoveTo( cx+2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->LineTo( cx-2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->Arc( cx-3*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx-Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx-2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx-2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos	);
	Impresor->MoveTo( cx-2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx+2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->Arc( cx+Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos,
						  cx+3*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos	);
	DibujarFlechaImp( cx, cy-Estado.DistanciaPuntos-1 );
	DibujarLineaEnlaceImp(ActualX, ActualY);
	TSize Tamanyo= Impresor->GetTextExtent(TXT_RUNNING_OBJECT_BLOCK_END, strlen(TXT_RUNNING_OBJECT_BLOCK_END) );
	Impresor->TextOut( cx-Tamanyo.cx/2, cy-Tamanyo.cy/2, TXT_RUNNING_OBJECT_BLOCK_END);

 }



 class OE_CicloMientras:public ObjetoEjecucion{
	  char *Cadena;
	  Token *Condicion;
	  ObjetoEjecucion *Siguiente;
	  OE_CierreMientras *Cierre;
	public:
	  OE_CicloMientras( ){
		 Tipo= CICLOMIENTRAS;
		 Condicion= 0;
		 Cadena= 0;
	  }
	  OE_CicloMientras( OE_CicloMientras* Ob);
	  void Preprocesar();
	  void Despreprocesar();
	  ObjetoEjecucion *Ejecutar();
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );

	  void SetCadena( char *c ){
		 if( Cadena ) delete[] Cadena;
		 Cadena= c;
	  }
	  void SetSiguiente( ObjetoEjecucion *Ob ){ Siguiente= Ob ; }
	  void SetCierre( OE_CierreMientras *c ){ Cierre= c; }
	  OE_CierreMientras *GetCierre(){return Cierre; }
	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }
	  char *GetCadena(){ return Cadena; }

 };


 OE_CicloMientras::OE_CicloMientras( OE_CicloMientras* Ob){
	Tipo= Ob->Tipo;
	Condicion= 0;
	if( Ob->Cadena ){
	  Cadena= new char[ strlen( Ob->Cadena )+1 ];
	  strcpy( Cadena, Ob->Cadena );
	}
	else
	  Cadena= 0;
 }


 void OE_CicloMientras::Preprocesar(){
	if( !Cadena ){
	  Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA ) ;
	  return;
	}
	Condicion= GetPostfijo( Cadena );
	if(Buzon.GetHuboError())
	  return ;
	if(!Condicion)
	  Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
 }

 void OE_CicloMientras::Despreprocesar(){
	LiberarListaToken(Condicion);
	Condicion= 0;
 }

 ObjetoEjecucion *OE_CicloMientras::Ejecutar(){
	Token *t= EvaluaPostfijo(Condicion);

	if(Buzon.GetHuboError()){
	  delete t;
	  return 0;
	}


	if( t->GetTipoDato()!=LOGICO ){
	  Buzon.Error( TIPO_DEBE_SER_LOGICO );
	  delete t;
	  return 0;
	}

	bool ValorDeCondicion= t->GetDatoLogico();
	delete t;
	return (ValorDeCondicion==true)?Siguiente:Cierre->GetSiguiente();
 }


 void OE_CicloMientras::Dibujar(int ActualX, int ActualY, int Sw){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 )Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->MoveTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->LineTo( cx-7*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx-6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx+6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx+7*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx+6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->LineTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	if( Sw )
	  Graficador->SelectObject( *LapizSolido );
	DibujarFlecha( cx, cy-Estado.DistanciaPuntos-1 );
	DibujarLineaEnlace(ActualX, ActualY);
	Graficador->TextOut( cx-6*Estado.DistanciaPuntos,
								cy-Estado.DistanciaPuntos/2, "MQ" );


	if( Cadena ){
	  TRect Region
	  ( cx-int(4.5*float(Estado.DistanciaPuntos)),cy-Estado.DistanciaPuntos/2,
		 cx+6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos/2);
	  TSize Tamanyo= Graficador->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx<int(9.5*float(Estado.DistanciaPuntos)) )
		 Graficador->DrawText(Cadena, -1, Region, DT_CENTER  );
	  else
		 Graficador->DrawText(Cadena, -1, Region );
	}

 }



 void OE_CicloMientras::DibujarImp(int ActualX, int ActualY){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;

	Impresor->MoveTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->LineTo( cx-7*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx-6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx+6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx+7*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx+6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->LineTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );

	DibujarFlechaImp( cx, cy-Estado.DistanciaPuntos-1 );
	DibujarLineaEnlaceImp(ActualX, ActualY);
	Impresor->TextOut( cx-6*Estado.DistanciaPuntos,
								cy-Estado.DistanciaPuntos/2, "MQ" );


	if( Cadena ){
	  TRect Region
	  ( cx-int(4.5*float(Estado.DistanciaPuntos)),cy-Estado.DistanciaPuntos/2,
		 cx+6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos/2);
	  TSize Tamanyo= Impresor->GetTextExtent( Cadena, strlen( Cadena ) );
	  if( Tamanyo.cx<int(9.5*float(Estado.DistanciaPuntos)) )
		 Impresor->DrawText(Cadena, -1, Region, DT_CENTER  );
	  else
		 Impresor->DrawText(Cadena, -1, Region );
	}

 }


 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class OE_Subprograma:public ObjetoEjecucion {
	  char *CadenaNombre;
	  char *CadenaParametros;
	  char *CadenaDescripcion;
	  ListaExpresiones Parametros;
	  ObjetoEjecucion *Siguiente;


	public:

	  OE_Subprograma( ){
		 Tipo= SUBPROGRAMA;
		 CadenaNombre= 0;
		 CadenaParametros= 0;
		 CadenaDescripcion= 0;
	  }
	  void Preprocesar();
	  void Despreprocesar();
	  ObjetoEjecucion *Ejecutar();
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 ) ;
	  void DibujarImp( int ActualX, int ActualY ) ;

	  void SetSiguiente( ObjetoEjecucion *Ob ){ Siguiente= Ob ; }
	  void SetCadenaNombre( char *c ){
		 if( CadenaNombre ) delete[] CadenaNombre;
		 CadenaNombre= c;
	  }
	  void SetCadenaDescripcion( char *c ){
		 if( CadenaDescripcion ) delete[] CadenaDescripcion;
		 CadenaDescripcion= c;
	  }
	  void SetCadenaParametros( char *c ){
		 if( CadenaParametros ) delete[] CadenaParametros;
		 CadenaParametros= c;
	  }
	  ListaExpresiones &GetParametros(){ return Parametros; }
	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }
	  char *GetCadenaNombre(){ return CadenaNombre; }
	  char *GetCadenaParametros(){ return CadenaParametros; }
	  char *GetCadenaDescripcion(){ return CadenaDescripcion; }
	  int GetNParametros(){ return Parametros.GetNItems(); }

 };


 void OE_Subprograma::Preprocesar(){
	if( !CadenaNombre ){
	  Buzon.Error( ILEGAL_NOMBRE_SUBPROGRAMA );
	  return;
	}
	Token *t= GetPostfijo( CadenaNombre );

	if( Buzon.GetHuboError() )
	  return;
	if( !t ){
	  Buzon.Error( ILEGAL_NOMBRE_SUBPROGRAMA );
	  return;
	}
	if( t->GetSig()|| t->GetTipoAlmacenamiento()==CONSTANTE ){
	  Buzon.Error( ILEGAL_NOMBRE_SUBPROGRAMA );
	  LiberarListaToken( t );
	  return;
	}
	char *CadAux= new char[strlen( t->GetDatoStr() )+1];
	strcpy( CadAux, t->GetDatoStr() );
	SetCadenaNombre( CadAux );
	LiberarListaToken( t );

	if(!CadenaParametros){
	  return;
	}
   int i= 0;
	for( ; CadenaParametros[i]==' '; ++i );
	if( CadenaParametros[i]==0 )
	  return;

	Parametros.GetListaExpresiones(CadenaParametros);
	if(Buzon.GetHuboError())
	  return;
	Parametros.OE_SubPrograma_RevisarParametros();
	// si se anyade codigo se debe revisar el buzon
 }

 void OE_Subprograma::Despreprocesar(){
	Parametros.Vacear();
 }

 ObjetoEjecucion *OE_Subprograma::Ejecutar(){
	return Siguiente;
 }

 void OE_Subprograma::Dibujar(int ActualX, int ActualY, int Sw){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 ) Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->MoveTo( cx-8*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx+8*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx+8*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->LineTo( cx-8*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );

	if( Sw==1 )Graficador->SelectObject(*LapizPunteadoRojo);
	else if( Sw==2 )Graficador->SelectObject(*LapizPunteadoAzul);
	else Graficador->SelectObject(*LapizPunteado);
	Graficador->LineTo( cx-8*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );

	Graficador->SelectObject(*LapizSolido);
	Graficador->MoveTo( cx, cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx, cy+2*Estado.DistanciaPuntos );

	if( Sw )
	  Graficador->SelectObject( *LapizSolido );
	DibujarLineaEnlace(ActualX, ActualY);


	char Nombre[84];  Nombre[0]= 0;
	char Par   [84];  Par   [0]= 0;

	bool ParentAbre= false;

	if( CadenaNombre ){
	  if( CadenaNombre[0] ){
		 strncpy( Nombre, CadenaNombre, 49 );
		 Nombre[strlen( Nombre )+1]= 0;
		 Nombre[strlen( Nombre )]= '(';
		 ParentAbre= true;
	  }
	}

	if( CadenaParametros ){
	  if( CadenaParametros[0] ){
		 if( ParentAbre==false ){
			ParentAbre= true;
			strcpy( Nombre,"(" );
		 }
		 strncpy( Par, CadenaParametros, 45 );
		 strcat( Nombre, Par );
	  }
	}
	if( ParentAbre )
	  strcat( Nombre, ")" );

	if( Nombre[0] ){
	  TRect Region( cx-int(/*4.5*/7.5*float(Estado.DistanciaPuntos)),
						 cy-Estado.DistanciaPuntos/2,
						 cx+int(7.5/*4.5*/*float(Estado.DistanciaPuntos)),
						 cy+Estado.DistanciaPuntos/2
					  );
	  TSize Tamanyo= Graficador->GetTextExtent( Nombre, strlen( Nombre ) );
	  if( Tamanyo.cx</*9*/15*Estado.DistanciaPuntos )
		 Graficador->DrawText( Nombre, -1, Region, DT_CENTER );
	  else
		 Graficador->DrawText( Nombre, -1,Region );
	}
 }




 void OE_Subprograma::DibujarImp(int ActualX, int ActualY){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;

	Impresor->MoveTo( cx-8*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx+8*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx+8*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->LineTo( cx-8*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );

	Impresor->SelectObject(*LapizPunteado);
	Impresor->LineTo( cx-8*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos );
	Impresor->SelectObject(*LapizSolido);

	Impresor->MoveTo( cx, cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx, cy+2*Estado.DistanciaPuntos );

	DibujarLineaEnlaceImp(ActualX, ActualY);


	char Nombre[84];  Nombre[0]= 0;
	char Par   [84];  Par   [0]= 0;

	bool ParentAbre= false;

	if( CadenaNombre ){
	  if( CadenaNombre[0] ){
		 strncpy( Nombre, CadenaNombre, 49 );
		 Nombre[strlen( Nombre )+1]= 0;
		 Nombre[strlen( Nombre )]= '(';
		 ParentAbre= true;
	  }
	}

	if( CadenaParametros ){
	  if( CadenaParametros[0] ){
		 if( ParentAbre==false ){
			ParentAbre= true;
			strcpy( Nombre,"(" );
		 }
		 strncpy( Par, CadenaParametros, 45 );
		 strcat( Nombre, Par );
	  }
	}
	if( ParentAbre )
	  strcat( Nombre, ")" );

	if( Nombre[0] ){
	  TRect Region( cx-int(/*4.5*/7.5*float(Estado.DistanciaPuntos)),
						 cy-Estado.DistanciaPuntos/2,
						 cx+int(7.5/*4.5*/*float(Estado.DistanciaPuntos)),
						 cy+Estado.DistanciaPuntos/2
					  );
	  TSize Tamanyo= Impresor->GetTextExtent( Nombre, strlen( Nombre ) );
	  if( Tamanyo.cx</*9*/15*Estado.DistanciaPuntos )
		 Impresor->DrawText( Nombre, -1, Region, DT_CENTER );
	  else
		 Impresor->DrawText( Nombre, -1,Region );
	}
 }


 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class OE_Regrese:public ObjetoEjecucion{
	  ObjetoEjecucion *Retorno;
	public:
	  OE_Regrese(){
		 Tipo= REGRESE;
	  }
	  void Preprocesar(){};
	  void Despreprocesar(){};
	  ObjetoEjecucion *Ejecutar();
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );
 };

 ObjetoEjecucion* BuscarSubprograma( ObjetoEjecucion* );

 ObjetoEjecucion *OE_Regrese::Ejecutar(){
	Retorno=(PilaDeTablas.GetTope())->GetRetorno();
	ObjetoEjecucion *Ob= BuscarSubprograma( Retorno );
	for( ;Ob;Ob= Ob->GetPunteroEnHoja())
	  if( Ob->GetTipo()== CICLOPARA )
		 ((OE_CicloPara*)Ob)->DesapilaPrimeraVez();

	PilaDeTablas.Desapilar();

	return Retorno;
 }


 void OE_Regrese::Dibujar(int ActualX, int ActualY, int Sw){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;
	if( Sw==1 )Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->Arc( cx-2*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos ,
						  cx,cy-2*Estado.DistanciaPuntos,cx+1,cy-2*Estado.DistanciaPuntos );
	if( Sw )
	  Graficador->SelectObject( *LapizSolido );
	DibujarFlecha( cx, cy-2*Estado.DistanciaPuntos-1 );
	TSize Tamanyo= Graficador->GetTextExtent("Regrese", 7 );
	Graficador->TextOut( cx-Tamanyo.cx/2, cy-Tamanyo.cy/2, "Regrese" );

 }

 void OE_Regrese::DibujarImp(int ActualX, int ActualY){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;
	Impresor->Arc( cx-2*Estado.DistanciaPuntos,cy-2*Estado.DistanciaPuntos,
						  cx+2*Estado.DistanciaPuntos,cy+2*Estado.DistanciaPuntos ,
						  cx,cy-2*Estado.DistanciaPuntos,cx+1,cy-2*Estado.DistanciaPuntos );
	DibujarFlechaImp( cx, cy-2*Estado.DistanciaPuntos-1 );
	TSize Tamanyo= Impresor->GetTextExtent("Regrese", 7 );
	Impresor->TextOut( cx-Tamanyo.cx/2, cy-Tamanyo.cy/2, "Regrese" );

 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class OE_Llamada:public ObjetoEjecucion {

	  char *CadenaNombre;
	  char *CadenaArgumentos;
	  ListaExpresiones Argumentos;
	  OE_Subprograma *Subprograma;
	  ObjetoEjecucion *Siguiente;
	public:
	  OE_Llamada( ){
		 Tipo= LLAMADA;
		 CadenaNombre= 0;
		 CadenaArgumentos= 0;
	  }
	  OE_Llamada( OE_Llamada* Ob );
	  void Preprocesar();
	  void Despreprocesar();
	  ObjetoEjecucion *Ejecutar();
	  void Dibujar( int ActualX, int ActualY, int Sw= 0 );
	  void DibujarImp( int ActualX, int ActualY );

	  void SetCadenaNombre( char *c ){
		 if( CadenaNombre ) delete[] CadenaNombre;
		 CadenaNombre= c;
	  }
	  void SetCadenaArgumentos( char *c ){
		 if( CadenaArgumentos ) delete[] CadenaArgumentos;
		 CadenaArgumentos= c;
	  }
	  void SetSubprograma( OE_Subprograma *Sub ){ Subprograma= Sub; }
	  void SetSiguiente( ObjetoEjecucion *Ob ){ Siguiente= Ob ; }
	  ObjetoEjecucion *GetSiguiente(){ return Siguiente; }
	  char *GetCadenaNombre(){ return CadenaNombre; }
	  char *GetCadenaArgumentos(){ return CadenaArgumentos; }
	  int GetNArgumentos(){ return Argumentos.GetNItems(); }

 };

 OE_Llamada::OE_Llamada( OE_Llamada* Ob ){
	Tipo= Ob->Tipo;
	if( Ob->CadenaNombre ){
	  CadenaNombre= new char[ strlen( Ob->CadenaNombre )+1 ];
	  strcpy( CadenaNombre, Ob->CadenaNombre );
	}
	else
	  CadenaNombre= 0;
	if( Ob->CadenaArgumentos ){
	  CadenaArgumentos= new char[ strlen( Ob->CadenaArgumentos )+1 ];
	  strcpy( CadenaArgumentos, Ob->CadenaArgumentos );
	}
	else
	  CadenaArgumentos= 0;
 }

 void OE_Llamada::Preprocesar(){

	if( !CadenaNombre ){
	  Buzon.Error( ILEGAL_NOMBRE_SUBPROGRAMA );
	  return;
	}
	Token *t= GetPostfijo( CadenaNombre );
	if( Buzon.GetHuboError() )
	  return;
	if( !t ){
	  Buzon.Error( ILEGAL_NOMBRE_SUBPROGRAMA );
	  return;
	}
	if( t->GetSig()|| t->GetTipoAlmacenamiento()==CONSTANTE ){
	  Buzon.Error( ILEGAL_NOMBRE_SUBPROGRAMA );
	  LiberarListaToken( t );
	  return;
	}
	char *CadAux= new char[strlen( t->GetDatoStr() )+1];
	strcpy( CadAux, t->GetDatoStr() );
	SetCadenaNombre( CadAux );
	delete t;

	if(!CadenaArgumentos)
	  return;
   int i= 0;
	for( ; CadenaArgumentos[i]==' '; ++i );
	if( CadenaArgumentos[i]==0 )
	  return;

	Argumentos.GetListaExpresiones(CadenaArgumentos);

 }

 void OE_Llamada::Despreprocesar(){
	Argumentos.Vacear();
 }


 ObjetoEjecucion *OE_Llamada::Ejecutar(){

	Tabla *T= new Tabla;
	T->SetRetorno(Siguiente);
	Token *Param, *Args;
	ListaExpresiones &Parametros= Subprograma->GetParametros();
	Argumentos.Reset();
	Parametros.Reset();
	int n=Argumentos.GetNItems();

	for( int i=0 ; i<n ; ++i ){
	  Param= Parametros.Itera();
	  Args=  Argumentos.Itera();
	  if( Args->GetTipoAlmacenamiento()==VARIABLE && !Args->GetSig() ){
		 Variable *Var= PilaDeTablas.Buscar(Args->GetDatoStr());
		 if(!Var)
			Var=PilaDeTablas.Crear(Args->GetDatoStr(),0,false);
		 Variable *Local= T->Crear(Param->GetDatoStr(),Var,true);
		 Local->SetCampo(Var->GetCampo());
		 Local->SetTipo(Var->GetTipo());
		 Local->SetFU(Var->GetFU());
	  }
	  else{
		 Token *ValorConstante=EvaluaPostfijo(Args);
		 if(Buzon.GetHuboError()){
			delete T;
			return 0;
		 }
		 Variable *Local= T->Crear(Param->GetDatoStr(), ValorConstante, 0,0,0);
		 delete ValorConstante;
		 Local->SetFP(false);
	  }
	}
	ObjetoEjecucion *Ob= BuscarSubprograma( this );
	for( ;Ob;Ob= Ob->GetPunteroEnHoja() )
	  if( Ob->GetTipo()== CICLOPARA )
		 ((OE_CicloPara*)Ob)->ApilaPrimeraVez();
	PilaDeTablas.Apilar(T);
	return Subprograma;
 }

 void OE_Llamada::Dibujar(int ActualX, int ActualY, int Sw ){

	int cx= int(PosicionX-ActualX)*Estado.DistanciaPuntos ;
	int cy= int(PosicionY-ActualY)*Estado.DistanciaPuntos ;

	if( Sw==1 ) Graficador->SelectObject( *LapizRojo );
	else if( Sw==2 ) Graficador->SelectObject( *LapizAzul );
	Graficador->MoveTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->LineTo( cx-7*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx-6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx+6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Graficador->LineTo( cx+7*Estado.DistanciaPuntos,cy );
	Graficador->LineTo( cx+6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Graficador->LineTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	if( Sw )
	  Graficador->SelectObject( *LapizSolido );
	DibujarFlecha( cx, cy-Estado.DistanciaPuntos-1 );
	DibujarLineaEnlace(ActualX, ActualY);

	char Nombre[64];  Nombre[0]= 0;
	char Arg   [64];  Arg   [0]= 0;

	bool ParentAbre= false;

	if( CadenaNombre ){
	  if( CadenaNombre[0] ){
		 strncpy( Nombre, CadenaNombre, 29 );
		 Nombre[strlen( Nombre )+1]= 0;
		 Nombre[strlen( Nombre )]= '(';
		 ParentAbre= true;
	  }
	}

	if( CadenaArgumentos ){
	  if( CadenaArgumentos[0] ){
		 if( ParentAbre==false ){
			ParentAbre= true;
			strcpy( Nombre,"(" );
		 }
		 strncpy( Arg, CadenaArgumentos, 25 );
		 strcat( Nombre, Arg );
	  }
	}
	if( ParentAbre )
	  strcat( Nombre, ")" );

	if( Nombre[0] ){
	  TRect Region( cx-6*Estado.DistanciaPuntos,
						 cy-Estado.DistanciaPuntos/2,
						 cx+6*Estado.DistanciaPuntos,
						 cy+Estado.DistanciaPuntos/2
					  );
	  TSize Tamanyo= Graficador->GetTextExtent( Nombre, strlen( Nombre ) );
	  if( Tamanyo.cx<12*Estado.DistanciaPuntos )
		 Graficador->DrawText( Nombre, -1, Region, DT_CENTER );
	  else
		 Graficador->DrawText( Nombre, -1,Region );
	}
 }


 void OE_Llamada::DibujarImp(int ActualX, int ActualY){

	int cx= PosicionX*Estado.DistanciaPuntos-ActualX ;
	int cy= PosicionY*Estado.DistanciaPuntos-ActualY ;

	Impresor->MoveTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->LineTo( cx-7*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx-6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx+6*Estado.DistanciaPuntos,cy+Estado.DistanciaPuntos );
	Impresor->LineTo( cx+7*Estado.DistanciaPuntos,cy );
	Impresor->LineTo( cx+6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	Impresor->LineTo( cx-6*Estado.DistanciaPuntos,cy-Estado.DistanciaPuntos );
	DibujarFlechaImp( cx, cy-Estado.DistanciaPuntos-1 );
	DibujarLineaEnlaceImp(ActualX, ActualY);

	char Nombre[64];  Nombre[0]= 0;
	char Arg   [64];  Arg   [0]= 0;

	bool ParentAbre= false;

	if( CadenaNombre ){
	  if( CadenaNombre[0] ){
		 strncpy( Nombre, CadenaNombre, 29 );
		 Nombre[strlen( Nombre )+1]= 0;
		 Nombre[strlen( Nombre )]= '(';
		 ParentAbre= true;
	  }
	}

	if( CadenaArgumentos ){
	  if( CadenaArgumentos[0] ){
		 if( ParentAbre==false ){
			ParentAbre= true;
			strcpy( Nombre,"(" );
		 }
		 strncpy( Arg, CadenaArgumentos, 25 );
		 strcat( Nombre, Arg );
	  }
	}
	if( ParentAbre )
	  strcat( Nombre, ")" );

	if( Nombre[0] ){
	  TRect Region( cx-6*Estado.DistanciaPuntos,
						 cy-Estado.DistanciaPuntos/2,
						 cx+6*Estado.DistanciaPuntos,
						 cy+Estado.DistanciaPuntos/2
					  );
	  TSize Tamanyo= Impresor->GetTextExtent( Nombre, strlen( Nombre ) );
	  if( Tamanyo.cx<12*Estado.DistanciaPuntos )
		 Impresor->DrawText( Nombre, -1, Region, DT_CENTER );
	  else
		 Impresor->DrawText( Nombre, -1,Region );
	}
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
							//Continuacion de ObjetoEjecucion
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




 ObjetoEjecucion* ObjetoEjecucion::GetSiguienteParaAcomoda(){
	switch( Tipo ){
	  case ASIGNACION:
		 return ((OE_Asignacion*)this)->GetSiguiente();
	  case LECTURA:
		 return ((OE_Lectura*)this)->GetSiguiente();
	  case IMPRESION:
		 return ((OE_Impresion*)this)->GetSiguiente();
	  case LLAMADA:
		 return ((OE_Llamada*)this)->GetSiguiente();
	  case CICLOPARA:
		 return ((OE_CicloPara*)this)->GetSiguiente();
	  case CICLOMIENTRAS:
		 return ((OE_CicloMientras*)this)->GetSiguiente();
	  case PUNTO:
		 return ((OE_Punto*)this)->GetSiguiente();
	  case CIERREPARA:
		 return ((OE_CierrePara*)this)->GetSiguiente();
	  case CIERREMIENTRAS:
		 return ((OE_CierreMientras*)this)->GetSiguiente();
	  case INICIO:
		 return ((OE_Inicio*)this)->GetSiguiente();
	  case SUBPROGRAMA:
		 return ((OE_Subprograma*)this)->GetSiguiente();
	}
	return 0;
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



 void ObjetoEjecucion::SetSiguienteParaClickInsertar( ObjetoEjecucion *Ob ){
	switch( Tipo ){
	  case ASIGNACION:
		 ((OE_Asignacion*)this)->SetSiguiente(Ob);
		 break;
	  case LECTURA:
		 ((OE_Lectura*)this)->SetSiguiente(Ob);
		 break;
	  case IMPRESION:
		 ((OE_Impresion*)this)->SetSiguiente(Ob);
		 break;
	  case LLAMADA:
		 ((OE_Llamada*)this)->SetSiguiente(Ob);
		 break;
	  case CICLOPARA:
		 ((OE_CicloPara*)this)->SetSiguiente(Ob);
		 break;
	  case CICLOMIENTRAS:
		 ((OE_CicloMientras*)this)->SetSiguiente(Ob);
		 break;
	  case PUNTO:
		 ((OE_Punto*)this)->SetSiguiente(Ob);
		 break;
	  case CIERREPARA:
		 ((OE_CierrePara*)this)->SetSiguiente(Ob);
		 break;
	  case CIERREMIENTRAS:
		 ((OE_CierreMientras*)this)->SetSiguiente(Ob);
		 break;
	  case INICIO:
		 ((OE_Inicio*)this)->SetSiguiente(Ob);
		 break;
	  case SUBPROGRAMA:
		 ((OE_Subprograma*)this)->SetSiguiente(Ob);
	}
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 int ObjetoEjecucion::
	  EstaEnZonaInsercion( int x, int y, int ActualX, int ActualY ){

	if( Tipo==REGRESE || Tipo==FIN )
	  return 0;
	int cx= (PosicionX-ActualX)*Estado.DistanciaPuntos;
	int cy= (PosicionY-ActualY)*Estado.DistanciaPuntos;

	int Arriba= cy-AltoArriba[Tipo]*Estado.DistanciaPuntos;
	int Izquierda, Derecha, Abajo;
	if( Tipo==DECISION ){
	  OE_Decision *Objeto= (OE_Decision *)this;
	  Izquierda= cx-(AnchoIzquierdo[DECISION]+Objeto->GetBrazoI()+1)
						 *Estado.DistanciaPuntos;
	  Derecha= cx+(AnchoDerecho[DECISION]+Objeto->GetBrazoD()+1)
					  *Estado.DistanciaPuntos;
	  Abajo= cy+(LargoLineaDecision-1)*Estado.DistanciaPuntos;
	}
	else if( Tipo==PUNTO ){
	  Izquierda= cx-(MinimoAnchoBrazo/2)*Estado.DistanciaPuntos;
	  Derecha= cx+(MinimoAnchoBrazo/2)*Estado.DistanciaPuntos;
	  Abajo= cy+(LargoLinea-1)*Estado.DistanciaPuntos;
	}

	else{
	  Izquierda= cx-AnchoIzquierdo[Tipo]*Estado.DistanciaPuntos;
	  Derecha= cx+AnchoDerecho[Tipo]*Estado.DistanciaPuntos;
	  Abajo= cy+(AltoAbajo[Tipo]+LargoLinea-1)*Estado.DistanciaPuntos;
	}

	return( x>=Izquierda && x<=Derecha && y>=Arriba && y<=Abajo );
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


 void InsertarObjetoEnListaHojas( ObjetoEjecucion* );
 void SetSeleccionadoEnListaHojas( ObjetoEjecucion * );



 int ObjetoEjecucion::
 ClickInsercion( int x, int y, int ActualX, int ActualY ){

	if( !EstaEnZonaInsercion( x, y, ActualX, ActualY ) )
	  return 0;

	// Se va a insertar a continuacion un objeto de tipo Estado.ObjetoEnCursor

	ObjetoEjecucion *NuevoObjeto;
	ObjetoEjecucion *Acompanyante;

	switch ( Estado.ObjetoEnCursor ){
	  case ASIGNACION:
		 NuevoObjeto= Acompanyante= new OE_Asignacion;
		 break;
	  case CICLOMIENTRAS:
		 NuevoObjeto= new OE_CicloMientras;
		 Acompanyante= new OE_CierreMientras;
		 InsertarObjetoEnListaHojas( Acompanyante );
		 ((OE_CicloMientras*)NuevoObjeto)->SetCierre((OE_CierreMientras*)Acompanyante);
		 ((OE_CierreMientras*)Acompanyante)->SetCiclo((OE_CicloMientras*)NuevoObjeto );
		 ((OE_CicloMientras*)NuevoObjeto)->SetSiguiente( Acompanyante );
		 break;
	  case CICLOPARA:
		 NuevoObjeto= new OE_CicloPara;
		 Acompanyante= new OE_CierrePara;
		 InsertarObjetoEnListaHojas( Acompanyante );
		 ((OE_CicloPara*)NuevoObjeto)->SetCierre((OE_CierrePara*)Acompanyante );
		 ((OE_CierrePara*)Acompanyante)->SetCiclo((OE_CicloPara*)NuevoObjeto );
		 ((OE_CicloPara*)NuevoObjeto)->SetSiguiente( Acompanyante );
		 break;
	  case DECISION:
		 NuevoObjeto= new OE_Decision;
		 Acompanyante= new OE_Punto;
		 InsertarObjetoEnListaHojas( Acompanyante );
		 ((OE_Decision*)NuevoObjeto)->SetPunto((OE_Punto*)Acompanyante );
		 ((OE_Decision*)NuevoObjeto)->SetSi( Acompanyante );
		 ((OE_Decision*)NuevoObjeto)->SetNo( Acompanyante );
		 break;
	  case LECTURA:
		 NuevoObjeto= Acompanyante= new OE_Lectura;
		 break;
	  case IMPRESION:
		 NuevoObjeto= Acompanyante= new OE_Impresion;
		 break;
	  case LLAMADA:
		 NuevoObjeto= Acompanyante= new OE_Llamada;
	}

	InsertarObjetoEnListaHojas( NuevoObjeto );

	if( Tipo==DECISION ){
	  int cx= (PosicionX-ActualX)*Estado.DistanciaPuntos;
	  if( x<cx ){
		 Acompanyante->SetSiguienteParaClickInsertar(
															 ((OE_Decision*)this)->GetSi() );
		 ((OE_Decision*)this)->SetSi( NuevoObjeto );
	  }
	  else{
		 Acompanyante->SetSiguienteParaClickInsertar(
															 ((OE_Decision*)this)->GetNo() );
		 ((OE_Decision*)this)->SetNo( NuevoObjeto );
	  }
	}
	else{
	  Acompanyante->SetSiguienteParaClickInsertar( GetSiguienteParaAcomoda() );
	  SetSiguienteParaClickInsertar( NuevoObjeto );
	}
	SetSeleccionadoEnListaHojas( NuevoObjeto );
	return 1;
 }


 void EditarLectura( OE_Lectura* );
 void EditarImpresion( OE_Impresion* );
 void EditarCicloMientras( OE_CicloMientras* );
 void EditarInicio( OE_Inicio* );
 void EditarDecision( OE_Decision* );
 void EditarLlamada( OE_Llamada* );
 void EditarSubprograma( OE_Subprograma* );
 void EditarCicloPara( OE_CicloPara* );
 void EditarAsignacion( OE_Asignacion* );

 void ObjetoEjecucion::Editar(){
	switch ( Tipo ){
	  case LECTURA:
		 EditarLectura( (OE_Lectura*)this );
		 break;
	  case IMPRESION:
		 EditarImpresion((OE_Impresion*)this );
		 break;
	  case DECISION:
		 EditarDecision((OE_Decision*)this );
		 break;
	  case ASIGNACION:
		 EditarAsignacion( (OE_Asignacion*)this );
		 break;
	  case CICLOPARA:
		 EditarCicloPara( (OE_CicloPara*)this );
		 break;
	  case CICLOMIENTRAS:
		 EditarCicloMientras((OE_CicloMientras*)this);
		 break;
	  case LLAMADA:
		 EditarLlamada( (OE_Llamada*)this );
		 break;
	  case SUBPROGRAMA:
		 EditarSubprograma( (OE_Subprograma*)this );
		 break;
	  case INICIO:
		 EditarInicio((OE_Inicio*)this);
		 break;

	}
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void ObjetoEjecucion::DibujarSeleccionado( int ActualX, int ActualY,
														  int Sw ){

	ObjetoEjecucion *GetActualSubprograma();
	int cx= (PosicionX-ActualX)*Estado.DistanciaPuntos;
	int cy= (PosicionY-ActualY)*Estado.DistanciaPuntos;
	TPoint Punto1(cx-(AnchoIzquierdo[Tipo]+1)*Estado.DistanciaPuntos,
					  cy-(AltoArriba[Tipo]+1)*Estado.DistanciaPuntos );
		Punto1.x-=Estado.DistanciaPuntos/4;
		Punto1.y-=Estado.DistanciaPuntos/4;
	TPoint Punto2( Punto1.x+Estado.DistanciaPuntos/2, Punto1.y );
	TPoint Punto3( Punto2.x, Punto1.y+Estado.DistanciaPuntos/2 );
	TPoint Punto4( Punto1.x, Punto3.y );


	TPoint VectorVertices[4]={ Punto1, Punto2, Punto3, Punto4};

	TBrush BrochaAzul( TColor::LtBlue );
	if( Sw==1 )
	  Graficador->SelectObject( BrochaAzul );
	else{
	  Graficador->SelectObject( *BrochaBlanca );
	  Graficador->SelectObject( *LapizBlanco );
	}
	Graficador->SetPolyFillMode( WINDING );

	//Punto superior izquierdo...
	Graficador->Polygon(VectorVertices, 4);

	//Punto superior central...
	for( int i= 0; i<4; ++i )
	  VectorVertices[i].x+= (1+AnchoDerecho[Tipo])*Estado.DistanciaPuntos;
	if(Tipo!=PUNTO)
	  Graficador->Polygon(VectorVertices, 4);


	//Punto superior derecho...
	for( int i= 0; i<4; ++i )
	  VectorVertices[i].x+= (1+AnchoDerecho[Tipo])*Estado.DistanciaPuntos;
	Graficador->Polygon(VectorVertices, 4);


	//Punto inferior derecho...
	for( int i= 0; i<4; ++i )
	  VectorVertices[i].y+= (2+AltoArriba[Tipo]+AltoAbajo[Tipo])
									*Estado.DistanciaPuntos;
	Graficador->Polygon(VectorVertices, 4);


	//Punto inferior central...
	for( int i= 0; i<4; ++i )
	  VectorVertices[i].x-= (1+AnchoDerecho[Tipo])*Estado.DistanciaPuntos;
	if(Tipo!=PUNTO)
	  Graficador->Polygon(VectorVertices, 4);


	//Punto inferior izquierdo...
	for( int i= 0; i<4; ++i )
	  VectorVertices[i].x-= (1+AnchoDerecho[Tipo])*Estado.DistanciaPuntos;
	Graficador->Polygon(VectorVertices, 4);

	if( Sw==0 ){
	  ObjetoEjecucion *Anterior= GetActualSubprograma();
	  while( Anterior ){
		 if( Anterior->GetTipo()== DECISION ){
			if( ((OE_Decision*)Anterior)->GetSi()==this  ||
				 ((OE_Decision*)Anterior)->GetNo()==this    )
			  break;
		 }
		 else
			if( Anterior->GetSiguienteParaAcomoda()==this )
			  break;
		 Anterior= Anterior->PunteroEnHoja;
	  }
	  Graficador->SelectObject( *LapizSolido );
     Graficador->SelectObject( *Brocha );
	  if( Anterior ){
		 if( Anterior->Tipo==DECISION )
			Anterior->Dibujar( ActualX, ActualY );
		 else
			Anterior->DibujarLineaEnlace( ActualX, ActualY );
     }
	  if( Tipo==DECISION )
		 Dibujar( ActualX, ActualY );
	  else
	    DibujarLineaEnlace( ActualX, ActualY );

	}

 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




 ObjetoEjecucion::~ObjetoEjecucion(){

	switch ( Tipo ){
	  case LECTURA:
		 if(  ((OE_Lectura*)this)->GetCadena() )
			delete[] ((OE_Lectura*)this)->GetCadena();
		 break;
	  case IMPRESION:
		 if(  ((OE_Impresion*)this)->GetCadena() )
			delete[] ((OE_Impresion*)this)->GetCadena();
		 break;
	  case DECISION:
		 if(  ((OE_Decision*)this)->GetCadena() )
			delete[] ((OE_Decision*)this)->GetCadena();
		 break;
	  case ASIGNACION:
		 ((OE_Asignacion*)this)->GetCadenasDestino().Vacear();
		 ((OE_Asignacion*)this)->GetCadenasFuente().Vacear();
		 break;
	  case CICLOPARA:
		 if(  ((OE_CicloPara*)this)->GetCadenaId() )
			delete[] ((OE_CicloPara*)this)->GetCadenaId();
		 if(  ((OE_CicloPara*)this)->GetCadenaVInicial() )
			delete[] ((OE_CicloPara*)this)->GetCadenaVInicial();
		 if(  ((OE_CicloPara*)this)->GetCadenaVFinal() )
			delete[] ((OE_CicloPara*)this)->GetCadenaVFinal();
		 if(  ((OE_CicloPara*)this)->GetCadenaIncremento() )
			delete[] ((OE_CicloPara*)this)->GetCadenaIncremento();
		 break;
	  case CICLOMIENTRAS:
		 if(  ((OE_CicloMientras*)this)->GetCadena() )
			delete[] ((OE_CicloMientras*)this)->GetCadena();
		 break;
	  case LLAMADA:
		 if(  ((OE_Llamada*)this)->GetCadenaNombre() )
			delete[] ((OE_Llamada*)this)->GetCadenaNombre();
		 if(  ((OE_Llamada*)this)->GetCadenaArgumentos() )
			delete[] ((OE_Llamada*)this)->GetCadenaArgumentos();
		 break;
	  case SUBPROGRAMA:
		 if(  ((OE_Subprograma*)this)->GetCadenaNombre() )
			delete[] ((OE_Subprograma*)this)->GetCadenaNombre();
		 if(  ((OE_Subprograma*)this)->GetCadenaParametros() )
			delete[] ((OE_Subprograma*)this)->GetCadenaParametros();
		 break;
	  case INICIO:
		 if(  ((OE_Inicio*)this)->GetDescripcion() )
			delete[] ((OE_Inicio*)this)->GetDescripcion();
		 break;

	}
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 #endif
