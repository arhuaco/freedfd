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

 class VentanaDepurador : public TFrameWindow {

	  ListaCadenas Cadenas1, Cadenas2;
	  int ActualX, ActualY;
	  int AltoRenglon;
	  int Seleccionado;
	  int MaximoAncho();

	  TPen *LapizNegroDebug;
	  TBrush *BrochaAzulDebug;
	  TPen *LapizGrisDebug;
	  TBrush *BrochaGrisDebug;

	public:

	  void Actualizar();
	  VentanaDepurador(TWindow* parent);
	  void ArreglarBarras();
	  void AcomodaEnPantalla( int Item );
	  void DibujarPunto( bool Modo );
	  void CmEdicionNuevo();
	  void CmEdicionEditar();
	  void CmEdicionEliminar();
	  void CmDepurarEvaluar();

	protected:
	  void Paint(TDC& dc, bool, TRect&);
	  void SetupWindow();
	  bool CanClose();

	  void EvLButtonDown(uint, TPoint&);
	  void EvHScroll(uint , uint , HWND );
	  void EvVScroll(uint , uint , HWND );
	  void EvSize(uint , TSize& );
	  void EvLButtonDblClk(uint, TPoint&);
	  void CmFlechaArriba();
	  void CmFlechaAbajo();
	  void CmFlechaIzquierda();
	  void CmFlechaDerecha();
	  void LlamaACanClose(){ CanClose(); }
	  void CmDepurarPasoSimple();
	  void CmDepurarEjecutar();
	  void CmDepurarDetener();
	  void CmDepurarPausar();
	  void CmDepurarEjecutarHasta();

	  void CeEdicionEditarDepurador ( TCommandEnabler &Ce );
	  void CeEdicionEliminarDepurador ( TCommandEnabler &Ce );

	DECLARE_RESPONSE_TABLE(VentanaDepurador);
 };

 DEFINE_RESPONSE_TABLE1(VentanaDepurador, TWindow)
	EV_WM_LBUTTONDOWN,
	EV_WM_HSCROLL,
	EV_WM_VSCROLL,
	EV_WM_SIZE,
	EV_WM_LBUTTONDBLCLK,
	EV_COMMAND( CM_EDICIONNUEVO, CmEdicionNuevo),
	EV_COMMAND( CM_EDICIONEDITARDEPURADOR, CmEdicionEditar),
	EV_COMMAND( CM_EDICIONELIMINARDEPURADOR, CmEdicionEliminar),
	EV_COMMAND( CM_FLECHAARRIBADEPURADOR, CmFlechaArriba),
	EV_COMMAND( CM_FLECHAABAJODEPURADOR, CmFlechaAbajo),
	EV_COMMAND( CM_FLECHAIZQUIERDADEPURADOR, CmFlechaIzquierda),
	EV_COMMAND( CM_FLECHADERECHADEPURADOR, CmFlechaDerecha),
	EV_COMMAND( CM_DEPURARPASOSIMPLE, CmDepurarPasoSimple),
	EV_COMMAND( CM_DEPURAREJECUTAR, CmDepurarEjecutar ),
	EV_COMMAND( CM_DEPURARDETENER, CmDepurarDetener ),
	EV_COMMAND( CM_DEPURARPAUSAR, CmDepurarPausar ),
	EV_COMMAND( CM_DEPURAREJECUTARHASTA, CmDepurarEjecutarHasta),
	EV_COMMAND( CM_DEPURAREVALUAR, CmDepurarEvaluar ),

	EV_COMMAND(CM_F6, LlamaACanClose),

	EV_COMMAND_ENABLE(CM_EDICIONEDITARDEPURADOR, CeEdicionEditarDepurador),
	EV_COMMAND_ENABLE(CM_EDICIONELIMINARDEPURADOR, CeEdicionEliminarDepurador),


 END_RESPONSE_TABLE;


 void VentanaDepurador::AcomodaEnPantalla( int Item ){

	TRect Region= GetClientRect();
	int Caben= Region.bottom/AltoRenglon;
	if( Caben==0 )
	  return;
	if( !( Item>=ActualY && Item<ActualY+Caben ) ){
	  ActualY= Seleccionado+1-Caben;
	  if( ActualY<0 )
		 ActualY= 0;
	  ArreglarBarras();
	  Invalidate();
	}
 }


 int VentanaDepurador::MaximoAncho(){

	TClientDC G( *this );
	G.SetBkMode(TRANSPARENT);
	G.SelectObject( TGadgetWindowFont(7, true, false) );
	//TFont( /*14 ,6*/ ) );

	Cadenas1.Reset();
	Cadenas2.Reset();
	int n= Cadenas1.GetNItems();
	int n2= Cadenas2.GetNItems();
	int Ancho= 0;
	for( int i= 0; i<n; ++i ){
	  char *Cad1= Cadenas1.Itera();
	  char *Cad2= (n2>i)?Cadenas2.Itera():"";
	  TSize T1= G.GetTextExtent( Cad1, strlen( Cad1 ) );
	  TSize T2= G.GetTextExtent( Cad2, strlen( Cad2 ) );
	  TSize T3= G.GetTextExtent( " \x04 ", 3 );

	  int NuevoAncho= T1.cx + T2.cx + T3.cx ;
	  if( NuevoAncho>Ancho )
		 Ancho= NuevoAncho ;
	}
	return Ancho;

 }



 VentanaDepurador::VentanaDepurador(TWindow* parent)
	 : TFrameWindow(parent, "Depurador")
 {

  ActualX= ActualY= 0;
  AltoRenglon= 20;
  Seleccionado= -1;

  Attr.Style = WS_VISIBLE | WS_CAPTION | WS_BORDER
				  | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME
				  |WS_HSCROLL| WS_VSCROLL | WS_POPUP | WS_OVERLAPPEDWINDOW
					;

  //Attr.Style = WS_VISIBLE | WS_POPUP | WS_OVERLAPPEDWINDOW;
  Attr.X = 20;
  Attr.Y = 110;
  Attr.W = 300;
  Attr.H = 150;
  AssignMenu("MENUDEPURADOR");

  //SetIconSm(GetModule(),"IDI_DEPURADOR"); // este fue para probar
  //SetIcon(GetModule(),"IDI_DEPURADOR");  //este funcionaba en BORLAND45

  LapizNegroDebug= new TPen( TColor::Black, 1 );
  BrochaAzulDebug= new TBrush( TColor(0,0,128) );
  LapizGrisDebug= new TPen( TColor::White, 1 );
  BrochaGrisDebug= new TBrush( TColor::White );
  SetBkgndColor(TColor::White);
  Attr.AccelTable= "MENUDEPURADOR";
  Seleccionado= -1;
 }

 void VentanaDepurador::SetupWindow(){
	ArreglarBarras();
 }


 void VentanaDepurador::DibujarPunto( bool Modo ){
	TClientDC G( *this );

	G.SelectObject( (Modo)? *LapizNegroDebug : *LapizGrisDebug );
	G.SelectObject( (Modo)? *BrochaAzulDebug : *BrochaGrisDebug );

/*
	TPoint Puntos[]= {
	  TPoint( -ActualX*AltoRenglon+7, (Seleccionado-ActualY)*AltoRenglon+6 ),
	  TPoint( -ActualX*AltoRenglon+11, (Seleccionado-ActualY)*AltoRenglon+10 ),
	  TPoint( -ActualX*AltoRenglon+7, (Seleccionado-ActualY)*AltoRenglon+14 ),
	  TPoint( -ActualX*AltoRenglon+3, (Seleccionado-ActualY)*AltoRenglon+10 )
	};


	G.Polygon( Puntos, 4 );
*/
	TRect Area= GetClientRect();
	G.Rectangle( 0,(Seleccionado-ActualY)*AltoRenglon,
					 Area.right,(Seleccionado-ActualY+1)*AltoRenglon );

	//ahora el texto...
	G.SetBkMode(TRANSPARENT);
	G.SelectObject( TGadgetWindowFont(7, true, false) );
	Cadenas1.Reset();
	Cadenas2.Reset();
   int i= 0;
	for( ; i<Seleccionado; ++i ){
	  Cadenas1.Itera();
	  Cadenas2.Itera();
	}
	char *Cad1= Cadenas1.Itera();
	char *Cad2= Cadenas2.Itera();
	TSize Espacio= G.GetTextExtent( Cad1, strlen( Cad1 ) );
	TSize Espacio2= G.GetTextExtent( " \x04 ", 3 );
	G.SetTextColor( TColor( Modo? TColor::White : TColor::Black ) );
	G.TextOut( Espacio2.cx-ActualX*AltoRenglon+2, (i-ActualY)*AltoRenglon+4, Cad1 );
	G.TextOut( Espacio2.cx-ActualX*AltoRenglon + Espacio.cx+2, (i-ActualY)*AltoRenglon+4, Cad2 );
	G.SetTextColor( TColor( TColor::Black ) );

 }


 void VentanaDepurador::Paint(TDC &G , bool, TRect&){
	int n= Cadenas1.GetNItems();
	Cadenas1.Reset();
	Cadenas2.Reset();

	G.SetBkMode(TRANSPARENT);
	G.SelectObject( TGadgetWindowFont(7, true, false) );
	//G.SelectObject( TFont( /*14, 6 */ );
	//G.SelectObject( *LapizNegroDebug );
	//G.SelectObject( *BrochaAzulDebug );

	TRect Ventana= GetClientRect();

	for( int i= 0; i<n; ++i ){
	  char *Cad1= Cadenas1.Itera();
	  char *Cad2= Cadenas2.Itera();
	  TSize Espacio= G.GetTextExtent( Cad1, strlen( Cad1 ) );
	  TSize Espacio2= G.GetTextExtent(" \x04 ", 3 );

	  G.TextOut( Espacio2.cx-ActualX*AltoRenglon+2, (i-ActualY)*AltoRenglon+4, Cad1 );
	  G.TextOut( Espacio2.cx-ActualX*AltoRenglon + Espacio.cx+2, (i-ActualY)*AltoRenglon+4, Cad2 );
	  if( Seleccionado==i ){
     	  DibujarPunto(true);

/*
		 TPoint Puntos[]= {
			TPoint( -ActualX*AltoRenglon+7, (i-ActualY)*AltoRenglon+6 ),
			TPoint( -ActualX*AltoRenglon+11, (i-ActualY)*AltoRenglon+10 ),
			TPoint( -ActualX*AltoRenglon+7, (i-ActualY)*AltoRenglon+14 ),
			TPoint( -ActualX*AltoRenglon+3, (i-ActualY)*AltoRenglon+10 )
		 };
		 G.Polygon( Puntos, 4 );
*/
	  }

	}
 }




 void VentanaDepurador::EvSize(uint , TSize& ){
	Invalidate();
 }


 void VentanaDepurador::ArreglarBarras(){

	int n= Cadenas1.GetNItems();
	int Ancho= MaximoAncho();

	SetScrollRange( SB_VERT, 0, (n==0)?0:n-1, false );
	SetScrollPos( SB_VERT, ActualY );
	SetScrollRange( SB_HORZ, 0, Ancho/AltoRenglon, false );
	SetScrollPos( SB_HORZ, ActualX );

 }


 bool VentanaDepurador::CanClose(){
	Estado.DepuradorVisible= false;
	ShowWindow( SW_HIDE );
	return false;
 }

 void VentanaDepurador::Actualizar(){
	Cadenas2.Vacear();
	int n= Cadenas1.GetNItems();
	Cadenas1.Reset();
	for( int i= 0; i<n; ++i ){
	  Token *Pos= GetPostfijo( Cadenas1.Itera() );
	  if( Buzon.GetHuboError() ){
		 char *Error= Buzon.GetError();
		 char *Nueva= new char[ strlen("(Error)")+strlen( Error )+2 ];
		 strcpy( Nueva, "(Error)" );
		 strcat( Nueva, Error );
		 delete[] Error;
		 Cadenas2.Insertar( Nueva );
		 continue;
	  }
	  if( !Pos ){
		 Buzon.Error( ILEGAL_COMA_O_CADENA_VACIA );
		 char *Error= Buzon.GetError();
		 char *Nueva= new char[ strlen(" : (Error) ")+strlen( Error )+2 ];
		 strcpy( Nueva, " : (Error) " );
		 strcat( Nueva, Error );
		 delete[] Error;
		 Cadenas2.Insertar( Nueva );
		 continue;
	  }
	  Token *Res= EvaluaPostfijo( Pos );
	  LiberarListaToken( Pos );

	  if( Buzon.GetHuboError() ){
		 char *Error= Buzon.GetError();
		 char *Nueva= new char[ strlen(" : (Error) ")+strlen( Error )+2 ];
		 strcpy( Nueva, " : (Error) " );
		 strcat( Nueva, Error );
		 delete[] Error;
		 Cadenas2.Insertar( Nueva );
		 continue;
	  }

	  char *Valor= RetornaValorDeTokenEnCadena( Res );
	  char *NuevaCadena;
	  if( Res->GetTipoDato()==STRING ){
		 NuevaCadena= new char[ strlen(" : (Cadena) '")+strlen( Valor )+4 ];
		 strcpy( NuevaCadena, " : (Cadena) '" );
	  }
	  else if( Res->GetTipoDato()==REAL ){
		 NuevaCadena= new char[ strlen(" : (Real) ")+strlen( Valor )+1 ];
		 strcpy( NuevaCadena, " : (Real) " );
	  }
	  else{
		 NuevaCadena= new char[ strlen(" : (Lógico) ")+strlen( Valor )+1 ];
		 strcpy( NuevaCadena, " : (Lógico) " );
	  }

	  strcat( NuevaCadena, Valor );
	  if( Res->GetTipoDato()==STRING )
		 strcat( NuevaCadena, "'" );
	  Cadenas2.Insertar( NuevaCadena );
	  delete Res;
	}
	ArreglarBarras();
	Invalidate();
 }

 void VentanaDepurador::EvLButtonDblClk(uint, TPoint&){
	CmEdicionEditar();
 }


 void VentanaDepurador::EvLButtonDown(uint, TPoint &Punto){


	if( Seleccionado!=-1 )
	  DibujarPunto(false);

	Seleccionado= Punto.y/AltoRenglon + ActualY;
	if( Seleccionado>=Cadenas1.GetNItems() )
	  Seleccionado= -1;
	else
	  DibujarPunto( true );
 }

 void VentanaDepurador::EvHScroll(uint ScrollCode, uint ThumboPos, HWND /*hWndCtl*/){
	int MinPos, MaxPos;
	GetScrollRange( SB_HORZ, MinPos, MaxPos );
	int Posicion= GetScrollPos( SB_HORZ );

	switch( ScrollCode ){
	  case SB_LINEDOWN:
			 if( Posicion < MaxPos ){
				SetScrollPos( SB_HORZ, Posicion+1 );
				ActualX= ActualX+1 ;
				Invalidate();
			 }
			 break;
	  case SB_LINEUP:
			 if( Posicion > MinPos ){
				SetScrollPos( SB_HORZ, Posicion-1 );
				ActualX= ActualX-1 ;
				Invalidate();
			 }
			 break;
	  case SB_THUMBTRACK:
	  case SB_THUMBPOSITION:
			 SetScrollPos( SB_HORZ, ThumboPos );
			 ActualX= ThumboPos;
			 Invalidate();
			 break;

	}
 }

 void VentanaDepurador::EvVScroll(uint ScrollCode, uint ThumboPos, HWND /*hWndCtl*/){
	int MinPos, MaxPos;
	GetScrollRange( SB_VERT, MinPos, MaxPos );
	int Posicion= GetScrollPos( SB_VERT );

	switch( ScrollCode ){
	  case SB_LINEDOWN:
			 if( Posicion < MaxPos ){
				SetScrollPos( SB_VERT, Posicion+1 );
				ActualY= ActualY+1 ;
				Invalidate();
			 }
			 break;
	  case SB_LINEUP:
			 if( Posicion > MinPos ){
				SetScrollPos( SB_VERT, Posicion-1 );
				ActualY= ActualY-1 ;
				Invalidate();
			 }
			 break;
	  case SB_THUMBTRACK:
	  case SB_THUMBPOSITION:
			 SetScrollPos( SB_VERT, ThumboPos );
			 ActualY= ThumboPos;
			 Invalidate();
			 break;

	}
 }



 void VentanaDepurador::CmEdicionNuevo(){
	char *LeerExpresion( char *IniCad= 0 );
	char *NuevaExpresion= LeerExpresion();
	if( !NuevaExpresion )
	  return;
	Cadenas1.Insertar( NuevaExpresion );
	Seleccionado= Cadenas1.GetNItems()-1;
	AcomodaEnPantalla( Seleccionado );
	Actualizar();
 }

 void VentanaDepurador::CmEdicionEditar(){
	if( Seleccionado == -1 )
	  return;
	Cadenas1.Reset();
	char *IniCad;
	for( int i= 0; i< Seleccionado; ++i )
	  Cadenas1.Itera();
	IniCad= Cadenas1.Itera();
	char *NuevaExpresion= LeerExpresion( IniCad );
	if( !NuevaExpresion )
	  return;
	Cadenas1.Eliminar( Seleccionado );
	Cadenas1.InsertarEn( Seleccionado, NuevaExpresion );
	Actualizar();

 }

 void VentanaDepurador::CmEdicionEliminar(){
	if( Seleccionado==-1 )
	  return;
	Cadenas1.Eliminar( Seleccionado );
	if( Cadenas1.GetNItems()<Seleccionado+1 )
	  --Seleccionado;
	ActualX= 0;
	if( ActualY> Cadenas1.GetNItems()-1 )
	  --ActualY;
	if( ActualY<0 )
	  ++ActualY;
	Actualizar();
 }

 void VentanaDepurador::CmFlechaArriba(){
	int n=Cadenas1.GetNItems();
	if(!n)
	  return;

	TClientDC G( *this );
	G.SetBkMode(TRANSPARENT);
	G.SelectObject( TGadgetWindowFont(7, true, false) );

	if(Seleccionado==-1){
	  Seleccionado= ActualY;
	  DibujarPunto(true);
	  return;
	}
	if(Seleccionado>0){
	  DibujarPunto( false );
	  --Seleccionado;
	  DibujarPunto(true);
	  if(ActualY>Seleccionado){
		 ActualY=Seleccionado;
		 ArreglarBarras();
		 Invalidate();
	  }
	  else
		 AcomodaEnPantalla(Seleccionado );
	}

 }

 void VentanaDepurador::CmFlechaAbajo(){

	int n=Cadenas1.GetNItems();
	if(!n)
	  return;


	if(Seleccionado==-1){
	  Seleccionado= ActualY;
	  DibujarPunto( true );
	  return;
	}
	if(Seleccionado<n-1){
	  DibujarPunto( false );
	  ++Seleccionado;
	  DibujarPunto( true );
	  if(ActualY>Seleccionado){
		 ActualY=Seleccionado;
		 ArreglarBarras();
		 Invalidate();
	  }
	  else
		 AcomodaEnPantalla(Seleccionado );
	}

 }
 void VentanaDepurador::CmFlechaIzquierda(){
	EvHScroll( SB_LINEUP, 0, 0);
 }
 void VentanaDepurador::CmFlechaDerecha(){
	EvHScroll( SB_LINEDOWN, 0, 0);
 }

 void VentanaDepurador::CmDepurarPasoSimple(){
	((TDrawWindow*)VentanaPrincipal)->CmDepurarPasoSimple();
 }

 void VentanaDepurador::CmDepurarEjecutar(){
	((TDrawWindow*)VentanaPrincipal)->CmDepurarEjecutar();
 }

 void VentanaDepurador::CmDepurarDetener(){
	((TDrawWindow*)VentanaPrincipal)->CmDepurarDetener();
 }

 void VentanaDepurador::CmDepurarPausar(){
	((TDrawWindow*)VentanaPrincipal)->CmDepurarPausar();
 }


 void VentanaDepurador::CmDepurarEjecutarHasta(){
	((TDrawWindow*)VentanaPrincipal)->CmDepurarEjecutarHasta();
 }

 void VentanaDepurador::CmDepurarEvaluar(){
	((TDrawWindow*)VentanaPrincipal)->CmDepurarEvaluar();
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void VentanaDepurador::CeEdicionEliminarDepurador ( TCommandEnabler &Ce ){
	Ce.Enable( Seleccionado!=-1 );
 }

 void VentanaDepurador::CeEdicionEditarDepurador ( TCommandEnabler &Ce ){
	Ce.Enable( Seleccionado!=-1 );
 }


