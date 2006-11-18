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



 #define  NOMBREPROGRAMA "FreeDFD"
 #define  TAMANYOPAGINA 10
 #define  MAXZOOM   60

 #include <owl/printer.h>
 #include <owl/applicat.h>
 #include <owl/decframe.h>
 #include <owl/dialog.h>
 #include <owl/opensave.h>
 #include <owl/controlb.h>
 #include <owl/buttonga.h>
 #include <owl/statusba.h>
 #include <owl/gadgetwi.h>
 #include <stdlib.h>
 #include <string.h>
 #include<windows.h>

 #include "Dfd.rc"
 #include "Dfd.rh"

 //#define
 char PrimerDirectorio[256];
 char ClaveYaGenerada[16];
 int ContadorToken= 0;  //Depuracion
 int ContadorCampoVariable= 0; //Depuracion
 int ContadorCajita=0; //Depuracion
 int ContadorVariable=0; //Depuracion
 int ContadorTabla= 0; //Depuracion
 int ContadorFor= 0;
 int ContadorNodoListaVectores= 0;
 int ContadorNodoListaExpresiones= 0;

 int MinimoAnchoBrazo=    5;
 int LargoLinea=          2;
 int LargoLineaDecision=  4;
 bool HayDialogo= false;

 class ObjetoEjecucion;

 struct Estado{

	int DistanciaPuntos;
	int ObjetoEnCursor;
	int Accion;

	bool AngulosEnGrados;
	bool EsNuevo,SeModifico;
	bool DepuradorVisible;
   bool Registrado;
	ObjetoEjecucion *ObjetoEnEjecucion;
	ObjetoEjecucion *ObjetoHasta;


	Estado(){
	  ObjetoHasta= 0;
	  Accion= 0;
     Registrado= false;
	  DepuradorVisible= false;
	  EsNuevo= true;
	  SeModifico= false;
	  ObjetoEnCursor= 14;
	  DistanciaPuntos= 12;
	  AngulosEnGrados= true;
	}

 }Estado;


 class TDrawWindow : public TWindow {

	 TPrinter* Printer;

	public:
	  TDrawWindow( TWindow *Parent= 0 );
	  ~TDrawWindow(){
		  delete Archivo;
        if(!Estado.Registrado){
          KillTimer(0);
        }
	  }

	  void CmDepurarEjecutar();
	  void CmDepurarDetener();
	  void CmDepurarPausar();
	  void CmDepurarPasoSimple();
	  void CmDepurarEjecutarHasta();
	  void CmDepurarEvaluar();


	protected:
	  TOpenSaveDialog::TData* Archivo;

	  bool CanClose();

	  void SetupWindow();

	  void GuardarArchivo();
	  bool AbrirArchivo();
	  void FileSaveAux();
	  void FileSaveAsAux();

	  void EvLButtonDown(uint, TPoint&);
	  void EvMouseMove(uint, TPoint&);
	  void EvLButtonDblClk(uint, TPoint&);
	  void Paint(TDC&, bool, TRect&);

	  void EvHScroll(uint , uint , HWND );
	  void EvVScroll(uint , uint , HWND );
	  void EvSize(uint , TSize& );

     void Cm_QuitaSeleccionado();
	  void CmFileNew();
	  void CmFileOpen();
	  void CmFileSave();
	  void CmFileSaveAs();
	  void CmArchivoImprimir();
	  void CmFileExit();
	  void CmVerZoomMas();
	  void CmVerZoomMenos();
	  void CmVerSubprogramaMas();
	  void CmVerSubprogramaMenos();
	  void CmVerDepurador();
	  void CmObjetoSalida();
	  void CmObjetoLlamada();
	  void CmObjetoLectura();
	  void CmObjetoDesicion();
	  void CmObjetoMientras();
	  void CmObjetoPara();
	  void CmObjetoAsignacion();
	  void CmAvanzaPagina();
	  void CmRetrocedePagina();
     void CmAyudaLicencia();
     void CmAyudaRegistrarAhora();
     void CmAyudaUsoAyuda();

	  void CeObjetoSalidaMenu ( TCommandEnabler & );
	  void CeObjetoLlamadaMenu ( TCommandEnabler & );
	  void CeObjetoLecturaMenu ( TCommandEnabler & );
	  void CeObjetoDesicionMenu ( TCommandEnabler & );
	  void CeObjetoParaMenu ( TCommandEnabler & );
	  void CeObjetoMientrasMenu ( TCommandEnabler & );
	  void CeObjetoAsignacionMenu ( TCommandEnabler & );
	  void CeSeleccionarCursorMenu ( TCommandEnabler &Ce );

	  void CmAbout();
	  void CmAyudaContenido();
	  void CmSeleccionarCursor();
	  void CmNuevoSubprograma();
	  void CmEdicionEliminar();
	  void CmEdicionEliminarSubprograma();
	  void CmEdicionPegar();
	  void CmEdicionCopiar();
	  void CmEdicionCortar();
	  void CmEdicionEditar();
	  void CmInsertarObjeto();

	  void CmAngulosEnGrados();
	  void CmAngulosEnRadianes();

	  void CeFileNew(TCommandEnabler &);
	  void CeFileOpen(TCommandEnabler &);
	  void CeFileSave(TCommandEnabler &);
	  void CeFileSaveAs(TCommandEnabler &);
	  void CeArchivoImprimir(TCommandEnabler &);

	  void CeEdicionEliminar( TCommandEnabler & );
	  void CeEdicionPegar( TCommandEnabler & );
	  void CeEdicionCopiar( TCommandEnabler & );
	  void CeEdicionCortar( TCommandEnabler & );
	  void CeEdicionEliminarSubprograma( TCommandEnabler & );
	  void CeEdicionEditar( TCommandEnabler & );
	  void CeEdicionInsertarObjeto( TCommandEnabler & );
	  void CeVerZoomMas( TCommandEnabler & );
	  void CeVerZoomMenos( TCommandEnabler & );
	  void CeVerSubprogramaMas( TCommandEnabler & );
	  void CeVerSubprogramaMenos( TCommandEnabler & );
	  void CeVerDepurador( TCommandEnabler & );

	  void CeNuevoSubprograma( TCommandEnabler & );

	  void CeObjetoSalida ( TCommandEnabler & );
	  void CeObjetoLlamada ( TCommandEnabler & );
	  void CeObjetoLectura ( TCommandEnabler & );
	  void CeObjetoDesicion ( TCommandEnabler & );
	  void CeObjetoPara ( TCommandEnabler & );
	  void CeObjetoMientras ( TCommandEnabler & );
	  void CeObjetoAsignacion( TCommandEnabler & );
	  void CeDepurarPasoSimple( TCommandEnabler & );
	  void CeDepurarEjecutar( TCommandEnabler & );
	  void CeDepurarDetener( TCommandEnabler & );
	  void CeDepurarPausar( TCommandEnabler & );
	  void CeDepurarEjecutarHasta( TCommandEnabler & );

	  void CeSeleccionarCursor ( TCommandEnabler & );
	  void CeAbout( TCommandEnabler & );
	  void CeAyudaContenido( TCommandEnabler & );
     void CeAyudaUsoAyuda(TCommandEnabler &);
     void CeAyudaLicencia(TCommandEnabler &);
     void CeAyudaRegistrarAhora(TCommandEnabler &);
	  void CeAngulosEnGrados( TCommandEnabler & );
	  void CeAngulosEnRadianes( TCommandEnabler & );


	  void CmFlechaArriba();
	  void CmFlechaAbajo();
	  void CmFlechaIzquierda();
	  void CmFlechaDerecha();
	  void CmInicio();
	  void CmFin();
	  void CmInsertar();

     DECLARE_RESPONSE_TABLE(TDrawWindow);
 } ;

 TWindow *VentanaPrincipal;
 TStatusBar *BarraEstado;

 TDC *Graficador;
 TDC *Impresor;
 TPen *LapizRojo;
 TPen *LapizAzul;
 TPen *LapizSolido;
 TPen *LapizPunteado;
 TPen *LapizPunteadoAzul;
 TPen *LapizPunteadoRojo;
 TPen *LapizBlanco;
 TBrush *BrochaBlanca;
 TBrush *Brocha;


 #include "includes.cpp"
 #include "debug.cpp"

 VentanaDepurador *Depurador=0;
 TWindow *GetDepurador(){ return Depurador; }

 ObjetoEjecucion *ClipBoard;



 DEFINE_RESPONSE_TABLE1(TDrawWindow, TWindow)
	EV_WM_LBUTTONDBLCLK,
	EV_WM_LBUTTONDOWN,
	EV_WM_MOUSEMOVE,
	EV_WM_HSCROLL,
	EV_WM_VSCROLL,
	EV_WM_SIZE,
	EV_WM_TIMER,

   EV_COMMAND(CM_QUITASELECCIONADO, Cm_QuitaSeleccionado),
	EV_COMMAND(CM_ARCHIVONUEVO, CmFileNew),
	EV_COMMAND(CM_ARCHIVOABRIR, CmFileOpen),
	EV_COMMAND(CM_ARCHIVOGUARDAR, CmFileSave),
	EV_COMMAND(CM_ARCHIVOGUARDARCOMO, CmFileSaveAs),
	EV_COMMAND(CM_ARCHIVOIMPRIMIR, CmArchivoImprimir),
	EV_COMMAND(CM_ARCHIVOSALIR, CmFileExit),

	EV_COMMAND(CM_AYUDAACERCADE, CmAbout),
	EV_COMMAND(CM_AYUDACONTENIDO, CmAyudaContenido),
   EV_COMMAND(CM_AYUDAUSOAYUDA,CmAyudaUsoAyuda),
   EV_COMMAND(CM_AYUDALICENCIA,CmAyudaLicencia),
   EV_COMMAND(CM_AYUDAREGISTRARAHORA,CmAyudaRegistrarAhora),


	EV_COMMAND(CM_VERZOOMMAS, CmVerZoomMas),
	EV_COMMAND(CM_VERZOOMMENOS, CmVerZoomMenos),

	EV_COMMAND(CM_VERSUBPROGRAMAMAS, CmVerSubprogramaMas),
	EV_COMMAND(CM_VERSUBPROGRAMAMENOS, CmVerSubprogramaMenos),
	EV_COMMAND(CM_VERDEPURADOR, CmVerDepurador),

	EV_COMMAND(CM_DEPURAREJECUTAR, CmDepurarEjecutar),
	EV_COMMAND(CM_DEPURARDETENER, CmDepurarDetener ),
	EV_COMMAND(CM_DEPURARPAUSAR, CmDepurarPausar ),
	EV_COMMAND(CM_DEPURARPASOSIMPLE, CmDepurarPasoSimple ),
	EV_COMMAND(CM_DEPURAREJECUTARHASTA, CmDepurarEjecutarHasta ),
	EV_COMMAND(CM_DEPURAREVALUAR, CmDepurarEvaluar ),

	EV_COMMAND(CM_OBJETOSALIDA, CmObjetoSalida ),
	EV_COMMAND(CM_OBJETOLLAMADA, CmObjetoLlamada ),
	EV_COMMAND(CM_OBJETOLECTURA, CmObjetoLectura ),
	EV_COMMAND(CM_OBJETODECISION, CmObjetoDesicion ),
	EV_COMMAND(CM_OBJETOPARA, CmObjetoPara ),
	EV_COMMAND(CM_OBJETOMIENTRAS, CmObjetoMientras ),
	EV_COMMAND(CM_OBJETOASIGNACION, CmObjetoAsignacion),
	EV_COMMAND(CM_OBJETOPARA, CmObjetoPara),
	EV_COMMAND(CM_OBJETONUEVOSUBPROGRAMA,CmNuevoSubprograma ),
	EV_COMMAND(CM_SELECCIONACURSOR,CmSeleccionarCursor ),

	EV_COMMAND(CM_OBJETOSALIDAMENU, CmObjetoSalida ),
	EV_COMMAND(CM_OBJETOLLAMADAMENU, CmObjetoLlamada ),
	EV_COMMAND(CM_OBJETOLECTURAMENU, CmObjetoLectura ),
	EV_COMMAND(CM_OBJETODECISIONMENU, CmObjetoDesicion ),
	EV_COMMAND(CM_OBJETOPARAMENU, CmObjetoPara ),
	EV_COMMAND(CM_OBJETOMIENTRASMENU, CmObjetoMientras ),
	EV_COMMAND(CM_OBJETOASIGNACIONMENU, CmObjetoAsignacion ),
	EV_COMMAND(CM_OBJETOPARAMENU, CmObjetoPara ),
	EV_COMMAND(CM_SELECCIONACURSORMENU,CmSeleccionarCursor ),


	EV_COMMAND(CM_EDICIONELIMINAR, CmEdicionEliminar ),
	EV_COMMAND(CM_EDICIONPEGAR, CmEdicionPegar),
	EV_COMMAND(CM_EDICIONCOPIAR, CmEdicionCopiar),
	EV_COMMAND(CM_EDICIONCORTAR, CmEdicionCortar),
	EV_COMMAND(CM_EDICIONELIMINARSUBPROGRAMA,CmEdicionEliminarSubprograma),
	EV_COMMAND(CM_EDICIONEDITAR, CmEdicionEditar),
	EV_COMMAND(CM_EDICIONINSERTAROBJETO, CmInsertar),


	EV_COMMAND(CM_ANGULOSENGRADOS, CmAngulosEnGrados ),
	EV_COMMAND(CM_ANGULOSENRADIANES, CmAngulosEnRadianes ),

	EV_COMMAND(CM_FLECHAARRIBA,CmFlechaArriba),
	EV_COMMAND(CM_FLECHAABAJO,CmFlechaAbajo),
	EV_COMMAND(CM_FLECHAIZQUIERDA,CmFlechaIzquierda),
	EV_COMMAND(CM_FLECHADERECHA,CmFlechaDerecha),
	EV_COMMAND(CM_INICIO,CmInicio),
	EV_COMMAND(CM_FIN,CmFin),
	EV_COMMAND(CM_INSERTAR,CmInsertar),
	EV_COMMAND(CM_AVANZAPAGINA, CmAvanzaPagina ),
	EV_COMMAND(CM_RETROCEDEPAGINA, CmRetrocedePagina ),


	EV_COMMAND_ENABLE(CM_ARCHIVONUEVO, CeFileNew ),
	EV_COMMAND_ENABLE(CM_ARCHIVOABRIR, CeFileOpen ),
	EV_COMMAND_ENABLE(CM_ARCHIVOGUARDAR, CeFileSave ),
	EV_COMMAND_ENABLE(CM_ARCHIVOGUARDARCOMO, CeFileSaveAs ),
	EV_COMMAND_ENABLE(CM_ARCHIVOIMPRIMIR, CeArchivoImprimir ),

	EV_COMMAND_ENABLE(CM_EDICIONELIMINAR, CeEdicionEliminar ),
	EV_COMMAND_ENABLE(CM_EDICIONPEGAR, CeEdicionPegar),
	EV_COMMAND_ENABLE(CM_EDICIONCOPIAR, CeEdicionCopiar),
	EV_COMMAND_ENABLE(CM_EDICIONCORTAR, CeEdicionCortar),
	EV_COMMAND_ENABLE(CM_EDICIONELIMINARSUBPROGRAMA,CeEdicionEliminarSubprograma),
	EV_COMMAND_ENABLE(CM_EDICIONEDITAR, CeEdicionEditar),
	EV_COMMAND_ENABLE(CM_EDICIONINSERTAROBJETO, CeEdicionInsertarObjeto),

	EV_COMMAND_ENABLE(CM_VERSUBPROGRAMAMAS, CeVerSubprogramaMas),
	EV_COMMAND_ENABLE(CM_VERSUBPROGRAMAMENOS, CeVerSubprogramaMenos),


	EV_COMMAND_ENABLE(CM_OBJETONUEVOSUBPROGRAMA,CeNuevoSubprograma ),
/*	EV_COMMAND_ENABLE(CM_OBJETOSALIDA, CeObjetoSalida ),
	EV_COMMAND_ENABLE(CM_OBJETOLLAMADA, CeObjetoLlamada ),
	EV_COMMAND_ENABLE(CM_OBJETOLECTURA, CeObjetoLectura ),
	EV_COMMAND_ENABLE(CM_OBJETODECISION, CeObjetoDesicion ),
	EV_COMMAND_ENABLE(CM_OBJETOPARA, CeObjetoPara ),
	EV_COMMAND_ENABLE(CM_OBJETOMIENTRAS, CeObjetoMientras ),
	EV_COMMAND_ENABLE(CM_OBJETOASIGNACION, CeObjetoAsignacion),
	EV_COMMAND_ENABLE(CM_OBJETOPARA, CeObjetoPara),
	EV_COMMAND_ENABLE(CM_SELECCIONACURSOR,CeSeleccionarCursor ),*/

	EV_COMMAND_ENABLE(CM_OBJETOSALIDAMENU, CeObjetoSalidaMenu ),
	EV_COMMAND_ENABLE(CM_OBJETOLLAMADAMENU, CeObjetoLlamadaMenu ),
	EV_COMMAND_ENABLE(CM_OBJETOLECTURAMENU, CeObjetoLecturaMenu ),
	EV_COMMAND_ENABLE(CM_OBJETODECISIONMENU, CeObjetoDesicionMenu ),
	EV_COMMAND_ENABLE(CM_OBJETOPARAMENU, CeObjetoParaMenu ),
	EV_COMMAND_ENABLE(CM_OBJETOMIENTRASMENU, CeObjetoMientrasMenu ),
	EV_COMMAND_ENABLE(CM_OBJETOASIGNACIONMENU, CeObjetoAsignacionMenu ),
	EV_COMMAND_ENABLE(CM_OBJETOPARAMENU, CeObjetoParaMenu ),
	EV_COMMAND_ENABLE(CM_SELECCIONACURSORMENU,CeSeleccionarCursorMenu ),


	EV_COMMAND_ENABLE(CM_AYUDAACERCADE, CeAbout),
	EV_COMMAND_ENABLE(CM_AYUDACONTENIDO, CeAyudaContenido),


	EV_COMMAND_ENABLE(CM_VERZOOMMAS, CeVerZoomMas),
	EV_COMMAND_ENABLE(CM_VERZOOMMENOS, CeVerZoomMenos),

	EV_COMMAND_ENABLE(CM_EDICIONEDITAR, CeEdicionEditar),

	EV_COMMAND_ENABLE(CM_DEPURAREJECUTAR, CeDepurarEjecutar),
	EV_COMMAND_ENABLE(CM_DEPURARDETENER, CeDepurarDetener),
	EV_COMMAND_ENABLE(CM_DEPURARPAUSAR, CeDepurarPausar),
	EV_COMMAND_ENABLE(CM_DEPURARPASOSIMPLE, CeDepurarPasoSimple),
	EV_COMMAND_ENABLE(CM_DEPURAREJECUTARHASTA, CeDepurarEjecutarHasta),
//	EV_COMMAND_ENABLE(CM_DEPURAREVALUAR, CeDepurarEvaluar),

	EV_COMMAND_ENABLE(CM_ANGULOSENGRADOS, CeAngulosEnGrados ),
	EV_COMMAND_ENABLE(CM_ANGULOSENRADIANES, CeAngulosEnRadianes),
   EV_COMMAND_ENABLE(CM_AYUDAUSOAYUDA, CeAyudaUsoAyuda),
   EV_COMMAND_ENABLE(CM_AYUDALICENCIA,CeAyudaLicencia),
   EV_COMMAND_ENABLE(CM_AYUDAREGISTRARAHORA,CeAyudaRegistrarAhora),

  END_RESPONSE_TABLE;
 //Funcion encargada de Ejecutar el siguiente Objeto.

 void EjecutarActual(){
	ObjetoEjecucion *Aux= Estado.ObjetoEnEjecucion;
	if( Estado.ObjetoHasta==Aux && Estado.Accion==EJECUCION ){
	  Estado.Accion= DEPURACION;
	  if( !EstaEnPantalla( Estado.ObjetoHasta ) )
		 ListaHojas.Centrar( Estado.ObjetoHasta );
	  else
		 ListaHojas.ArreglarBarras();
	  VentanaPrincipal->SetCursor( 0, IDC_ARROW );
	  if( Estado.DepuradorVisible )
		 Depurador->Actualizar();
	  Estado.ObjetoHasta= 0;
	  return;
	}

	Estado.ObjetoEnEjecucion= Estado.ObjetoEnEjecucion->Ejecutar();
	if( Estado.Accion==DEPURACION ){
//
	  //if( Estado.DepuradorVisible )  //producía un error lógico
	  //	 Depurador->Actualizar();
	  TClientDC Gr( *VentanaPrincipal );
	  Graficador= &Gr;
	  Graficador->SelectObject(*LapizSolido);
	  Graficador->SelectObject( *Brocha );
	  Graficador->SelectObject( TFont( Estado.DistanciaPuntos,
								  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
									FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));
	  int ActualX= ListaHojas.GetActualX();
	  int ActualY= ListaHojas.GetActualY();

	  if( Aux )
		 if( ListaHojas.EstaEnHoja( Aux ) )
			Aux->Dibujar( ActualX, ActualY );
	  if( Estado.ObjetoEnEjecucion )
		 if( !ListaHojas.EstaEnHoja( Estado.ObjetoEnEjecucion ) ){
			  ListaHojas.CambiarHoja( Estado.ObjetoEnEjecucion );
			  ListaHojas.Centrar( Estado.ObjetoEnEjecucion );
		 }
		 else{
			if( !EstaEnPantalla( Estado.ObjetoEnEjecucion ) )
			  ListaHojas.Centrar( Estado.ObjetoEnEjecucion );
			else
			  Estado.ObjetoEnEjecucion->Dibujar( ActualX, ActualY, 2 );
		 }

	  Estado.ObjetoHasta= 0;
	}
	bool HuboError= false;

	if( Buzon.GetHuboError() ){
	  ListaHojas.CambiarHoja( Aux );
	  ListaHojas.SetResaltado( Aux );
	  ListaHojas.Centrar( Aux );
	  PilaDeTablas.Vacear();
	  Buzon.Mostrar();
	  VentanaPrincipal->Invalidate();
	  HuboError= true;
	  Estado.ObjetoEnEjecucion= 0;
	}
	if( !Estado.ObjetoEnEjecucion ){
	  ListaHojas.Despreprocesar();
	  PilaDeTablas.Vacear();
	  if( Estado.DepuradorVisible )
		 Depurador->Actualizar();
	  Estado.Accion= EDICION;
	  Estado.ObjetoHasta= 0;
	  if( !HuboError ){
		 VentanaPrincipal->MessageBox("La ejecución ha terminado con normalidad."
												,NOMBREPROGRAMA, MB_ICONINFORMATION );
	  }
	  if( Estado.DepuradorVisible )
		 Depurador->Actualizar();

	}
 }

 TDrawWindow::TDrawWindow( TWindow *Parent ){

	Init( Parent, 0, 0 );
	ClipBoard= 0;
	Depurador= 0;
	Brocha= new TBrush(TColor::Black );
	BrochaBlanca= new TBrush(TColor::White );
	LapizSolido= new TPen( TColor::Black,1 );
	LapizPunteado= new TPen( TColor::Black, 1, PS_DOT);
	LapizRojo= new TPen( TColor::LtRed, 1 );
	LapizAzul= new TPen( TColor::LtBlue, 1 );
	LapizBlanco= new TPen( TColor::White, 1 );
	LapizPunteadoAzul= new TPen( TColor::LtBlue, 1, PS_DOT);
	LapizPunteadoRojo= new TPen( TColor::LtRed, 1, PS_DOT);
	Depurador= new VentanaDepurador(this);
	Estado.DepuradorVisible= false;
	Attr.Style |= WS_VSCROLL | WS_HSCROLL | WS_MAXIMIZE ;
	Archivo  = new TOpenSaveDialog::TData(OFN_OVERWRITEPROMPT|
						OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"(*.dfd)|*.dfd|", 0, "",
						"dfd");
	Printer = 0;
	Printer = new TPrinter;
   SetBkgndColor(TColor::White);  //Para que salga siempre fondo blanco
 }


 void TDrawWindow::SetupWindow(){
	void 	CambiarTitulo(char*);
	char* DirectorioActual( char* );

	char Directorio[1024];
	DirectorioActual( Directorio );
	char Titulo[1024];
	strcpy( Titulo, "Dfd - [" );
	strncat( Titulo, Directorio, 512 );
	strcat( Titulo, "\\Sin nombre.dfd]" );
	CambiarTitulo(Titulo);
	//Timer...
   if(!Estado.Registrado){ //Siempre falso
	  if(SetTimer(0, 1000, 0) == 0)
	    MessageBox("Could not Create Timer", NOMBREPROGRAMA, MB_OK);
   }
}

 bool TDrawWindow::CanClose(){
	char *DirectorioActual( char* );
	if(Estado.SeModifico){
	  char CadAux[2048];
	  if( Estado.EsNuevo ){
		 char Directorio[1024];
		 DirectorioActual( Directorio );
		 strcpy( CadAux, "¿Guardar cambios en " );
		 strncat( CadAux, Directorio, 512 );
		 strcat( CadAux, "\\Sin nombre.dfd?" );
	  }
	  else
		 sprintf( CadAux,"¿Guardar cambios en %s?",Archivo->FileName );

	  switch(MessageBox( CadAux,NOMBREPROGRAMA,
								MB_YESNOCANCEL | MB_ICONEXCLAMATION)) {
		 case IDCANCEL:
			return false;
		 case IDYES:
			FileSaveAux();
			if( Estado.SeModifico )
			  return false;
	  }

	}
	return true;

 }

 void TDrawWindow::EvSize(uint /*sizeType*/, TSize& /*size*/){
  ListaHojas.ArreglarBarras();
 }



 void TDrawWindow::EvLButtonDown(uint, TPoint &Punto ){

	TClientDC G( *VentanaPrincipal );
	Graficador= &G;
	  Graficador->SelectObject( TFont( Estado.DistanciaPuntos,
								  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
									FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));

	if( Estado.Accion==EJECUCION )
	  return;
	int ActualX= ListaHojas.GetActualX();
	int ActualY= ListaHojas.GetActualY();
	ObjetoEjecucion *ObjetoAExaminar= ListaHojas.GetActualSubprograma();

	while( ObjetoAExaminar ){
	  if( Estado.ObjetoEnCursor==NINGUNO ){
		 if( ObjetoAExaminar->EstaEnZona( Punto.x, Punto.y, ActualX, ActualY )){
			if( ListaHojas.GetSeleccionado()!=ObjetoAExaminar ){
			  if( ListaHojas.GetSeleccionado() )
				 ListaHojas.GetSeleccionado()->
					DibujarSeleccionado( ActualX, ActualY, 0 );
			  ListaHojas.SetSeleccionado( ObjetoAExaminar );
			  ListaHojas.GetSeleccionado()->
				 DibujarSeleccionado( ActualX, ActualY, 1 );

			  //Invalidate();/* OJO BORRAR **************************/
			}
			return;
		 }
		 ObjetoAExaminar= ObjetoAExaminar->GetPunteroEnHoja();
		 continue;
	  }

	  if( ObjetoAExaminar->ClickInsercion( Punto.x, Punto.y, ActualX, ActualY ))
	  {
		 Acomoda( ListaHojas.GetActualSubprograma(),
					 ListaHojas.GetActualAnchoI(), ListaHojas.GetActualAnchoD(),
					 ListaHojas.GetActualAlto() );
		 Ubica( ListaHojas.GetActualSubprograma(), 0, 5 );
		 ListaHojas.ArreglarBarras();
		 Estado.ObjetoEnCursor= NINGUNO;
		 EvMouseMove( 0, Punto );
		 Estado.SeModifico=true;
		 return;
	  }
	  ObjetoAExaminar= ObjetoAExaminar->GetPunteroEnHoja();

	}

	// Para nadie fue el click...
	if( ListaHojas.GetSeleccionado() ){
	  ListaHojas.GetSeleccionado()->
		 DibujarSeleccionado( ActualX, ActualY, 0 );
	  ListaHojas.SetSeleccionado( 0 );
	}
 }




 void TDrawWindow::EvMouseMove( uint, TPoint &Punto ){
	if(Estado.Accion==EJECUCION)
	  return;
	int ActualX= ListaHojas.GetActualX();
	int ActualY= ListaHojas.GetActualY();
	int x= Punto.x;
	int y= Punto.y;
	ObjetoEjecucion *ObjetoAExaminar= ListaHojas.GetActualSubprograma();

	if( Estado.ObjetoEnCursor==NINGUNO ){
	  while( ObjetoAExaminar ){
		 if( ObjetoAExaminar->EstaEnZona( x, y, ActualX, ActualY ) ){
			SetCursor( GetModule(), "MANO_TOCANDO" );
			return;
		 }
		 ObjetoAExaminar= ObjetoAExaminar->GetPunteroEnHoja();
	  }
	  SetCursor( 0, IDC_ARROW );
	  return;
	}

	while( ObjetoAExaminar ){
	  if( ObjetoAExaminar->EstaEnZonaInsercion( x, y, ActualX, ActualY )){
		 switch( Estado.ObjetoEnCursor ){
			case CICLOPARA:
			case CICLOMIENTRAS:
			case LLAMADA:
			  SetCursor( GetModule(), "MANO_AGARRANDO_CICLO" );
			  break;
			case DECISION:
			  SetCursor( GetModule(), "MANO_AGARRANDO_DECISION" );
			  break;
			case LECTURA:
			  SetCursor( GetModule(), "MANO_AGARRANDO_LECTURA" );
			  break;
			case IMPRESION:
			  SetCursor( GetModule(), "MANO_AGARRANDO_SALIDA" );
			  break;
			case ASIGNACION:
			  SetCursor( GetModule(), "MANO_AGARRANDO_ASIGNACION" );
		 }
		 return ;
	  }

	  ObjetoAExaminar= ObjetoAExaminar->GetPunteroEnHoja();
	}
	SetCursor( GetModule(), "PROHIBIDO" );
 }



 void TDrawWindow::EvLButtonDblClk(uint, TPoint &Punto){
	if(Estado.Accion!=EDICION)
	  return;
	int ActualX= ListaHojas.GetActualX();
	int ActualY= ListaHojas.GetActualY();
	int x= Punto.x;
	int y= Punto.y;
	ObjetoEjecucion *ObjetoAExaminar= ListaHojas.GetActualSubprograma();

	while( ObjetoAExaminar ){
	  if( ObjetoAExaminar->EstaEnZona( x, y, ActualX, ActualY ) ){
		 ObjetoAExaminar->Editar();
		 TipoObjeto Tipo= ObjetoAExaminar->GetTipo();
		 if( Tipo!=CIERREPARA && Tipo!= CIERREMIENTRAS && Tipo!= PUNTO &&
			  Tipo!=REGRESE && Tipo !=FIN && Tipo!=INICIO )
			Invalidate();
		 return;
	  }
	  ObjetoAExaminar= ObjetoAExaminar->GetPunteroEnHoja();
	}
 }


 void TDrawWindow::Paint(TDC &Gr, bool, TRect& ){

	Graficador= &Gr;
	Graficador->SelectObject(*LapizSolido);
	Graficador->SelectObject( *Brocha );
	  Graficador->SelectObject( TFont( Estado.DistanciaPuntos,
								  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
									FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));

	int ActualX= ListaHojas.GetActualX();
	int ActualY= ListaHojas.GetActualY();


	ObjetoEjecucion *ObjetoADibujar=ListaHojas.GetActualSubprograma();

	while(ObjetoADibujar){
	  ObjetoADibujar->Dibujar( ActualX, ActualY );
	  ObjetoADibujar=ObjetoADibujar->GetPunteroEnHoja();
	}

	if( Estado.Accion==DEPURACION ){
	  if( Estado.ObjetoEnEjecucion
			&& ListaHojas.EstaEnHoja(Estado.ObjetoEnEjecucion ) )
		 Estado.ObjetoEnEjecucion->Dibujar( ActualX, ActualY, 2 );
	}
	else{
	  ObjetoEjecucion *Resaltado= ListaHojas.GetResaltado();
	  if( Resaltado )
		 Resaltado->Dibujar( ActualX, ActualY, 1 );
	}
	if( Estado.Accion!=EJECUCION ){
	  ObjetoEjecucion *Seleccionado= ListaHojas.GetSeleccionado();
	  if( Seleccionado )
		 Seleccionado->DibujarSeleccionado( ActualX, ActualY );
	}
 }



 void TDrawWindow::EvHScroll(uint ScrollCode, uint ThumboPos, HWND /*hWndCtl*/){
	int MinPos, MaxPos;
	GetScrollRange( SB_HORZ, MinPos, MaxPos );
	int Posicion= GetScrollPos( SB_HORZ );

	switch( ScrollCode ){
	  case SB_PAGEDOWN:
			 if( Posicion + TAMANYOPAGINA< MaxPos ){
				SetScrollPos( SB_HORZ, Posicion+TAMANYOPAGINA );
				ListaHojas.SetActualX( ListaHojas.GetActualX()+TAMANYOPAGINA );
			 }
			 else{
				TRect Ventana;
				VentanaPrincipal->GetClientRect( Ventana );
				int  VentX= Ventana.right/Estado.DistanciaPuntos;
				SetScrollPos( SB_HORZ, MaxPos );
				ListaHojas.SetActualX(
								MaxPos-VentX-ListaHojas.GetActualAnchoI() );
			 }
			 Invalidate();
			 break;
	  case SB_PAGEUP:
			 if( Posicion - TAMANYOPAGINA>0 ){
				SetScrollPos( SB_HORZ, Posicion-TAMANYOPAGINA );
				ListaHojas.SetActualX( ListaHojas.GetActualX()-TAMANYOPAGINA );
			 }
			 else{
				TRect Ventana;
				VentanaPrincipal->GetClientRect( Ventana );
				int  VentX= Ventana.right/Estado.DistanciaPuntos;
				SetScrollPos( SB_HORZ, 0 );
				ListaHojas.SetActualX(-VentX-ListaHojas.GetActualAnchoI() );
			 }
			 Invalidate();
			 break;
	  case SB_LINEDOWN:
			 if( Posicion < MaxPos ){
				SetScrollPos( SB_HORZ, Posicion+1 );
				ListaHojas.SetActualX( ListaHojas.GetActualX()+1 );
				Invalidate();
			 }
			 break;
	  case SB_LINEUP:
			 if( Posicion > MinPos ){
				SetScrollPos( SB_HORZ, Posicion-1 );
				ListaHojas.SetActualX( ListaHojas.GetActualX()-1 );
				Invalidate();
			 }
			 break;
	  case SB_THUMBTRACK:
	  case SB_THUMBPOSITION:
			 SetScrollPos( SB_HORZ, ThumboPos );
			 TRect Ventana;
			 VentanaPrincipal->GetClientRect( Ventana );
			 int  VentX= Ventana.right/Estado.DistanciaPuntos;
			 ListaHojas.SetActualX( ThumboPos-VentX-ListaHojas.GetActualAnchoI() );
			 Invalidate();
			 break;

	}
 }


 void TDrawWindow::CmAvanzaPagina(){
	EvVScroll( SB_PAGEDOWN, 0, 0 );
 }

 void TDrawWindow::CmRetrocedePagina(){
	EvVScroll( SB_PAGEUP, 0, 0 );
 }


 void TDrawWindow::EvVScroll(uint ScrollCode, uint ThumboPos, HWND /*hWndCtl*/){
	int MinPos, MaxPos;
	int Posicion= GetScrollPos( SB_VERT );
	GetScrollRange( SB_VERT, MinPos, MaxPos );
	switch( ScrollCode ){
	  case SB_PAGEDOWN:
			 if( Posicion+TAMANYOPAGINA< MaxPos ){
				SetScrollPos( SB_VERT, Posicion+TAMANYOPAGINA );
				ListaHojas.SetActualY( ListaHojas.GetActualY()+TAMANYOPAGINA );
			 }
			 else {
				if( ListaHojas.GetActualY()==MaxPos )
              break;
				SetScrollPos( SB_VERT, MaxPos );
				ListaHojas.SetActualY( MaxPos );
			 }
			 Invalidate();
			 break;
	  case SB_PAGEUP:
			 if( Posicion-TAMANYOPAGINA>0 ){
				SetScrollPos( SB_VERT, Posicion-TAMANYOPAGINA );
				ListaHojas.SetActualY( ListaHojas.GetActualY()-TAMANYOPAGINA );
			 }
			 else{
				if( ListaHojas.GetActualY()==0 )
				  break;
				SetScrollPos( SB_VERT, 0 );
				ListaHojas.SetActualY( 0 );
			 }
			 Invalidate();
			 break;
	  case SB_LINEDOWN:
			 if( Posicion < MaxPos ){
				SetScrollPos( SB_VERT, Posicion+1 );
				ListaHojas.SetActualY( ListaHojas.GetActualY()+1 );
				Invalidate();
			 }
			 break;
	  case SB_LINEUP:
			 if( Posicion > MinPos ){
				SetScrollPos( SB_VERT, Posicion-1 );
				ListaHojas.SetActualY( ListaHojas.GetActualY()-1 );
				Invalidate();
			 }
			 break;
	  case SB_THUMBTRACK:
	  case SB_THUMBPOSITION:
			 SetScrollPos( SB_VERT, ThumboPos );
			 ListaHojas.SetActualY( ThumboPos );
			 Invalidate();
			 break;

	}
 }

 char *DirectorioActual(char *Path)
 {
	strcpy(Path, "X:\\");
	Path[0] = char('A' + getdisk());
	getcurdir(0, Path+3);
	if( Path[strlen(Path)-1]=='\\' )
	  Path[strlen(Path)-1]= 0;
	return(Path);
 }



 void TDrawWindow::CmFileNew(){
	void CambiarTitulo( char * );
	if( Estado.Accion!=EDICION )
	  return;
	if( !CanClose() )
	  return;
	ListaHojas.Vacear();
	ListaHojas.NuevaHoja();
	OE_Inicio *Inicio= new OE_Inicio;
	OE_Fin    *Fin= new OE_Fin;
	Inicio->SetSiguiente( Fin );
	ListaHojas.InsertarObjeto( Inicio );
	ListaHojas.InsertarObjeto( Fin );

	Acomoda( Inicio,ListaHojas.GetActualAnchoI(), ListaHojas.GetActualAnchoD(),
				ListaHojas.GetActualAlto() );
	Ubica( Inicio, 0, 5 );
	  VentanaPrincipal->SetScrollPos( SB_VERT, 0 );
	  VentanaPrincipal->SetScrollPos( SB_HORZ, 0 );
	ListaHojas.ArreglarBarras();
	Estado.ObjetoEnCursor= NINGUNO;
	Estado.EsNuevo= true;
	Estado.SeModifico= false;
	Estado.AngulosEnGrados= true;
	char Directorio[1024];
	DirectorioActual( Directorio );
	char Titulo[1024];
	strcpy( Titulo, "Dfd - [" );
	strncat( Titulo, Directorio, 512 );
	strcat( Titulo, "\\Sin nombre.dfd]" );
	CambiarTitulo(Titulo);

	return;

 }

 void TDrawWindow::CmFileOpen(){
	void CambiarTitulo( char * );
	if( Estado.Accion!=EDICION )
	  return;
	if ( CanClose() ){
	  if ((TFileOpenDialog(this, *Archivo)).Execute() == IDOK){
       char *GuardaCadena= new char[strlen(Archivo->FileName)+1];
       strcpy(GuardaCadena,Archivo->FileName);
		 if(!AbrirArchivo()){
         strcpy(Archivo->FileName, GuardaCadena);
         delete[] GuardaCadena;
         return ;
       }
       delete[] GuardaCadena;
		 char Titulo[1024];
		 strcpy( Titulo, "Dfd - [" );
		 strncat( Titulo, Archivo->FileName, 512 );
		 strcat( Titulo, "]" );
		 CambiarTitulo( Titulo );
	  }
	}
 }

 void TDrawWindow::CmFileSave(){
	if( Estado.Accion!=EDICION )
	  return;
	FileSaveAux();
 }

 void TDrawWindow::FileSaveAux(){
  if (Estado.EsNuevo)
	 FileSaveAsAux();
  else{
	 // MessageBox("Opción no disponible en versión shareware",NOMBREPROGRAMA);
		 GuardarArchivo();
  }
 }

 void TDrawWindow::CmFileSaveAs(){

	if( Estado.Accion!=EDICION )
	  return;
	FileSaveAsAux();
 }


 void TDrawWindow::FileSaveAsAux(){
	void CambiarTitulo( char *);
	char AntiguoFileName[1024];
	strcpy( AntiguoFileName, Archivo->FileName );
	if(Estado.EsNuevo)
	  strcpy( Archivo->FileName,  "Sin Nombre.dfd"/*Titulo*/ );


	if ((TFileSaveDialog(this, *Archivo)).Execute() == IDOK){
	  //MessageBox("Opción no disponible en versión shareware", NOMBREPROGRAMA );
	  //desde aqui
	  GuardarArchivo();
	  Estado.EsNuevo= false;

	  char Titulo[1024];
	  strcpy( Titulo, "Dfd - [" );
	  strncat( Titulo, Archivo->FileName, 512 );
	  strcat( Titulo, "]" );
	  CambiarTitulo( Titulo );
     return;
	  //hasta aqui
	}
	strcpy( Archivo->FileName, AntiguoFileName );
 }

 void TDrawWindow::CmArchivoImprimir(){
	//MessageBox("Opción no disponible en versión shareware",NOMBREPROGRAMA);

	if (Printer) {
	  char Nombre[1024];
	  if(Estado.EsNuevo)
		 strcpy( Nombre, "Sin Nombre.dfd" );
	  else
		 strcpy( Nombre, Archivo->FileName );
	  TWindowPrintout printout( Nombre, this);
	  //printout.SetBanding(TRUE);
	  Printer->Print(this, printout,TRUE);
	}

 }

 void TDrawWindow::CmFileExit(){
	this->CloseWindow();
 }

 void TDrawWindow::Cm_QuitaSeleccionado(){
   if( ListaHojas.GetSeleccionado() ){
  	  TClientDC G( *VentanaPrincipal );
	  Graficador= &G;
	  Graficador->SelectObject( TFont( Estado.DistanciaPuntos,
					  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
						FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));
  	  ListaHojas.GetSeleccionado()->
                  DibujarSeleccionado( ListaHojas.GetActualX(),
														ListaHojas.GetActualY(), 0 );
	  ListaHojas.SetSeleccionado(0);
	}

 }

 void TDrawWindow::CmAyudaContenido(){
	WinHelp("Dfd.hlp", HELP_FINDER, 0); /* ANTES HELP-INDEX */
 }

 void TDrawWindow::CmAyudaUsoAyuda(){
   WinHelp("", HELP_HELPONHELP, 0);
 }

 void TDrawWindow::CmAyudaLicencia(){
   //VentanaPrincipal->WinHelp("Dfd.hlp", HELP_CONTEXT, LICENCIA);
 }




 void TDrawWindow::CmAyudaRegistrarAhora(){
   // DialogoRegistrar().Execute();
 }

 void TDrawWindow::CmAbout(){
	TDialog Dialogo(this, IDD_ABOUT);
	//Dialogo.SetBkgndColor(TColor::LtGray);
	HayDialogo= true;
	Dialogo.Execute();
	HayDialogo= false;
 }


 void TDrawWindow::CmObjetoSalida(){
	Estado.ObjetoEnCursor= IMPRESION;
 }


 void TDrawWindow::CmObjetoLlamada(){
	Estado.ObjetoEnCursor= LLAMADA;
 }


 void TDrawWindow::CmObjetoLectura(){
	Estado.ObjetoEnCursor= LECTURA;
 }


 void TDrawWindow::CmObjetoDesicion(){
	Estado.ObjetoEnCursor= DECISION;
 }


 void TDrawWindow::CmObjetoMientras(){
	Estado.ObjetoEnCursor= CICLOMIENTRAS;
 }


 void TDrawWindow::CmObjetoPara(){
	Estado.ObjetoEnCursor= CICLOPARA;
 }

 void TDrawWindow::CmObjetoAsignacion(){
	Estado.ObjetoEnCursor= ASIGNACION ;
 }

 void TDrawWindow::CmSeleccionarCursor(){
	Estado.ObjetoEnCursor= NINGUNO ;
 }

 void TDrawWindow::CmVerZoomMas(){
	if( Estado.Accion==EJECUCION )
	  return;
	if( Estado.DistanciaPuntos>MAXZOOM )
	  return;
	Estado.DistanciaPuntos+= 2;
	ListaHojas.ArreglarBarras();
 }

 void TDrawWindow::CmVerZoomMenos(){
	if( Estado.Accion==EJECUCION )
	  return;
	int HuboCambio= Estado.DistanciaPuntos;
	Estado.DistanciaPuntos-=2;
	if(Estado.DistanciaPuntos<2)
	  Estado.DistanciaPuntos=2;
	if( HuboCambio!=Estado.DistanciaPuntos ){
	  ListaHojas.ArreglarBarras();
	}
 }

 void TDrawWindow::CmVerSubprogramaMas(){
	if( Estado.Accion!=EJECUCION )
	  ListaHojas.SubprogramaMas();
 }

 void TDrawWindow::CmVerSubprogramaMenos(){
	if( Estado.Accion!=EJECUCION )
	  ListaHojas.SubprogramaMenos();
 }

 void TDrawWindow::CmVerDepurador(){
	  static bool PrimeraVez= true;
	  if(!PrimeraVez){
		 Depurador->ShowWindow(Estado.DepuradorVisible?SW_HIDE:SW_SHOWNORMAL);
		 Estado.DepuradorVisible= !Estado.DepuradorVisible;
	  if( Estado.DepuradorVisible )
		 Depurador->Actualizar();
	  }
	  else{
		 Depurador->Create();
		 Estado.DepuradorVisible=true;
		 PrimeraVez= false;
	  }
 }





 void TDrawWindow::CmDepurarEjecutar(){
	if( Estado.Accion==EJECUCION )
	  return;
	ListaHojas.DesResaltar();
	ListaHojas.SetSeleccionado( 0 );
	Invalidate();
	Estado.ObjetoEnCursor= NINGUNO;
	SetCursor( 0, IDC_WAIT  ); /* NELSON */
	if( Estado.Accion==DEPURACION ){
	  Estado.Accion= EJECUCION;
	  return;
	}
	ListaHojas.Preprocesar();
	if( Buzon.GetHuboError() ){
	  Buzon.Mostrar();
	  Estado.Accion= EDICION;
	  Estado.ObjetoHasta= 0;
	  return;
	}
	Estado.ObjetoEnEjecucion= ListaHojas.GetInicioAlgoritmo();
	Estado.Accion= EJECUCION;
	PilaDeTablas.Apilar( new Tabla );
 }


 void TDrawWindow::CmDepurarDetener(){
	if( Estado.Accion == EDICION )
	  return;
	SetCursor( 0, IDC_ARROW  );
	ListaHojas.Despreprocesar();
	PilaDeTablas.Vacear();
	Depurador->Actualizar();
	Estado.Accion= EDICION;
	Estado.ObjetoHasta= 0;
	ListaHojas.ArreglarBarras();
	char Cad[64];
	sprintf( Cad, "Tokens en memoria: %d", ContadorToken );
	if(ContadorToken)
	  VentanaPrincipal->MessageBox( Cad, "Favor informar al fabricante" );
	sprintf( Cad, "Tablas en memoria: %d", ContadorTabla);
	if(ContadorTabla)
	  VentanaPrincipal->MessageBox( Cad, "Favor informar al fabricante" );
	sprintf( Cad, "Variables en memoria: %d", ContadorVariable);
	if( ContadorVariable )
	  VentanaPrincipal->MessageBox( Cad, "Favor informar al fabricante" );
	sprintf( Cad, "Campos variable en memoria: %d", ContadorCampoVariable );
	if(ContadorCampoVariable)
	  VentanaPrincipal->MessageBox( Cad, "Favor informar al fabricante" );
	sprintf( Cad, "Cajitas: %d", ContadorCajita);
	if(ContadorCajita)
	  VentanaPrincipal->MessageBox( Cad, "Favor informar al fabricante" );
	sprintf( Cad, "Vainas del for: %d", ContadorFor);
	if(ContadorFor)
	  VentanaPrincipal->MessageBox( Cad, "Favor informar al fabricante" );
	sprintf( Cad, "Nodo lista vectores: %d", ContadorNodoListaVectores);
	if(ContadorNodoListaVectores)
	  VentanaPrincipal->MessageBox( Cad, "Favor informar al fabricante" );
	sprintf( Cad, "Nodo lista expresiones: %d", ContadorNodoListaExpresiones);
	if(ContadorNodoListaExpresiones)
	 VentanaPrincipal->MessageBox( Cad, "Favor informar al fabricante" );

  /*
	ContadorNodoListaExpresiones=0;
	ContadorNodoListaVectores= 0;
	ContadorToken= 0;
	ContadorCampoVariable=0;
	ContadorVariable=0;
	ContadorCajita=0;
	ContadorTabla=0;
	ContadorFor=0;
	*/
 }

 void TDrawWindow::CmDepurarPausar(){
	if( Estado.Accion!= EJECUCION )
	  return;
	SetCursor( 0, IDC_ARROW  );
	Estado.Accion= DEPURACION;
	if( Estado.ObjetoEnEjecucion )
	  ListaHojas.CambiarHoja( Estado.ObjetoEnEjecucion );
	else
	  ListaHojas.CambiarHoja( ListaHojas.GetInicioAlgoritmo() );
	ListaHojas.ArreglarBarras();
	if( Estado.ObjetoEnEjecucion )
	  ListaHojas.Centrar( Estado.ObjetoEnEjecucion );
	else
	  ListaHojas.Centrar( ListaHojas.GetInicioAlgoritmo() );
   Depurador->Actualizar();
 }


 void TDrawWindow::CmDepurarPasoSimple(){
	if( Estado.Accion==EJECUCION )
	  return;
	Estado.ObjetoEnCursor= NINGUNO;
	if( Estado.Accion==DEPURACION ){
	  EjecutarActual();
	  if( Estado.DepuradorVisible )
		 Depurador->Actualizar();
	  return;
	}

	ListaHojas.DesResaltar();
	ListaHojas.SetSeleccionado( 0 );
	Invalidate();
	Estado.ObjetoEnCursor= NINGUNO;
	SetCursor( GetModule(), IDC_ARROW );
	ListaHojas.Preprocesar();
	if( Buzon.GetHuboError() ){
	  Buzon.Mostrar();
	  Estado.Accion= EDICION;
	  return;
	}
	Estado.ObjetoEnEjecucion= ListaHojas.GetInicioAlgoritmo();
	Estado.Accion= DEPURACION;
	PilaDeTablas.Apilar( new Tabla );
	ListaHojas.CambiarHoja( Estado.ObjetoEnEjecucion );

	if( !EstaEnPantalla( Estado.ObjetoEnEjecucion ) )
	  ListaHojas.Centrar( Estado.ObjetoEnEjecucion );
	else
	  ListaHojas.ArreglarBarras();

 }


 void TDrawWindow::CmDepurarEjecutarHasta(){
	if( Estado.Accion==EJECUCION )
	  return;
	ObjetoEjecucion *Seleccionado= ListaHojas.GetSeleccionado();
	if( Seleccionado ){
	  Estado.ObjetoHasta= Seleccionado;
	  CmDepurarEjecutar();
	}
 }

 void TDrawWindow::CmDepurarEvaluar(){
	DialogoEvaluador().Execute();
 }





 void TDrawWindow::CmNuevoSubprograma()
 {
	ListaHojas.NuevaHoja();
	OE_Subprograma *Inicio= new OE_Subprograma;
	OE_Regrese *Fin= new OE_Regrese;
	Inicio->SetSiguiente( Fin );
	ListaHojas.InsertarObjeto( Inicio );
	ListaHojas.InsertarObjeto( Fin );

	Acomoda( ListaHojas.GetActualSubprograma(), ListaHojas.GetActualAnchoI(), ListaHojas.GetActualAnchoD()
					,  ListaHojas.GetActualAlto() );
	Ubica( ListaHojas.GetActualSubprograma(), 0, 5 );
	ListaHojas.ArreglarBarras();
	Estado.SeModifico=true;

 }




 void TDrawWindow::CmEdicionEliminar(){
  if(Estado.Accion!=EDICION)
	 return;
  ObjetoEjecucion *ObjetoABorrar= ListaHojas.GetSeleccionado();
  if(!ObjetoABorrar)
	 return;
  TipoObjeto t= ObjetoABorrar->GetTipo();
  if( t==INICIO || t==CIERREPARA || t==CIERREMIENTRAS || t==PUNTO )
	 return;
  if( ObjetoABorrar->GetTipo()==SUBPROGRAMA ){
	 int Respuesta= MessageBox("Si elimina este objeto eliminará el subprograma completo.\n¿Eliminar?",NOMBREPROGRAMA,
					 MB_YESNO|MB_ICONQUESTION);
	 if( Respuesta==IDNO )
		  return;
	 ListaHojas.BorrarHoja();
	 ListaHojas.ArreglarBarras();
	 Estado.SeModifico=true;
	 return;
  }

  ObjetoEjecucion *Anterior= ListaHojas.GetActualSubprograma();

  while(1){
	 if(Anterior->GetTipo()==DECISION){
		if(((OE_Decision*)Anterior)->GetSi()==ObjetoABorrar||
		((OE_Decision*)Anterior)->GetNo()==ObjetoABorrar)
			break;
	 }
	 else
		if( Anterior->GetSiguienteParaAcomoda()==ObjetoABorrar)
		  break;
	 Anterior= Anterior->GetPunteroEnHoja();
  }
  ListaHojas.SetSeleccionado( Anterior );
  ObjetoEjecucion *Acompanyante;

  if( ObjetoABorrar->GetTipo()==DECISION ){
	 Acompanyante=((OE_Decision*)ObjetoABorrar)->GetPunto();
	 if(((OE_Decision*)ObjetoABorrar)->GetSi() !=
		 ((OE_Decision*)ObjetoABorrar)->GetPunto()
		 ||
		 ((OE_Decision*)ObjetoABorrar)->GetNo() !=
		 ((OE_Decision*)ObjetoABorrar)->GetPunto() )
	 {
		int Respuesta= MessageBox("Si elimina este objeto eliminará también su brazo izquierdo y su brazo derecho.\n¿Eliminar? ",NOMBREPROGRAMA,
							MB_YESNO|MB_ICONQUESTION);
		if(Respuesta==IDNO ){
		  ListaHojas.SetSeleccionado( ObjetoABorrar );
		  return;
      }
	 }
  }
  else if( ObjetoABorrar->GetTipo()==CICLOPARA ){
	 Acompanyante=((OE_CicloPara*)ObjetoABorrar)->GetCierre();
	 if( ObjetoABorrar->GetSiguienteParaAcomoda() !=
		 ((OE_CicloPara*)ObjetoABorrar)->GetCierre() )
	 {
		int Respuesta= MessageBox("Si elimina este objeto eliminará también el cuerpo del ciclo.\n¿Eliminar?",NOMBREPROGRAMA,
						  MB_YESNO|MB_ICONQUESTION);
		if(Respuesta==IDNO ){
		  ListaHojas.SetSeleccionado( ObjetoABorrar );
		  return;
		}
	 }
  }
  else if( ObjetoABorrar->GetTipo()==CICLOMIENTRAS ){
	 Acompanyante=((OE_CicloMientras*)ObjetoABorrar)->GetCierre();
	 if( ObjetoABorrar->GetSiguienteParaAcomoda() !=
		 ((OE_CicloMientras*)ObjetoABorrar)->GetCierre() )
	 {
		int Respuesta= MessageBox("Si elimina este objeto eliminará también el cuerpo del ciclo.\n¿Eliminar?",NOMBREPROGRAMA,
						  MB_YESNO|MB_ICONQUESTION);
		if(Respuesta==IDNO ){
		 ListaHojas.SetSeleccionado( ObjetoABorrar );
		 return;
		}
	 }
  }

  else
	 Acompanyante=ObjetoABorrar;

  //borra sin miedo...
	Estado.SeModifico=true;
  //Borrar del diagrama...
  if( Anterior->GetTipo()==DECISION ){
	 if( ((OE_Decision*)Anterior)->GetSi()==ObjetoABorrar )
		 ((OE_Decision*)Anterior)->
									  SetSi( Acompanyante->GetSiguienteParaAcomoda());
	 else
		 ((OE_Decision*)Anterior)->
									  SetNo( Acompanyante->GetSiguienteParaAcomoda());
  }
  else
	 Anterior->SetSiguienteParaClickInsertar
		 ( Acompanyante-> GetSiguienteParaAcomoda() );


  //Borrar de memoria y de ListaHojas...
  if( ListaHojas.GetActualY()>ObjetoABorrar->GetPosicionY() )
	 ListaHojas.SetActualY( ObjetoABorrar->GetPosicionY() );

  BorrarBloqueObjetos( ObjetoABorrar );

  Acomoda( ListaHojas.GetActualSubprograma(),
					 ListaHojas.GetActualAnchoI(), ListaHojas.GetActualAnchoD(),
					 ListaHojas.GetActualAlto() );
  Ubica( ListaHojas.GetActualSubprograma(), 0, 5 );

  if( !EstaEnPantalla( ListaHojas.GetSeleccionado() ) )
	 ListaHojas.Centrar( ListaHojas.GetSeleccionado() );
  ListaHojas.ArreglarBarras();

 }


 void TDrawWindow::CmEdicionPegar(){
  if(Estado.Accion!=EDICION)
	 return;
	if( !ClipBoard )
	  return;
	ObjetoEjecucion *PegarEn= ListaHojas.GetSeleccionado(), *Hasta;
	if( !PegarEn )
	  return;
	ObjetoEjecucion *Clip= Copia( ClipBoard, 1 );
	Hasta= HastaDonde( Clip );
	if( PegarEn->GetTipo()== DECISION ){
	  int Respuesta=PreguntaDecision();
	  if( Respuesta==1 ){
		 Hasta->SetSiguienteParaClickInsertar
				  ( ((OE_Decision*)PegarEn)->GetSi() );
		 ((OE_Decision*)PegarEn)->SetSi( Clip );
	  }

	  else if(Respuesta==2){
		 Hasta->SetSiguienteParaClickInsertar
				  ( ((OE_Decision*)PegarEn)->GetNo() );
		 ((OE_Decision*)PegarEn)->SetNo( Clip );
	  }
	  else{
		 BorrarBloqueObjetos(Clip,1,0);
		 return;
	  }
	}
	else{
	  Hasta->SetSiguienteParaClickInsertar(
			PegarEn->GetSiguienteParaAcomoda() );
	  PegarEn->SetSiguienteParaClickInsertar( Clip );
	}
	InsertarEnListaHojas( Clip );
	Acomoda( ListaHojas.GetActualSubprograma(),
				ListaHojas.GetActualAnchoI(), ListaHojas.GetActualAnchoD(),
				ListaHojas.GetActualAlto() );
	Ubica( ListaHojas.GetActualSubprograma(), 0, 5 );
	ListaHojas.SetSeleccionado( Clip );
	ListaHojas.ArreglarBarras();
	Estado.SeModifico=true;
 }


 void TDrawWindow::CmEdicionCopiar(){
	if(Estado.Accion!=EDICION)
	 return;
	ObjetoEjecucion *ObjetoACopiar= ListaHojas.GetSeleccionado();
	if( !ObjetoACopiar )
	  return;
	TipoObjeto t= ObjetoACopiar->GetTipo();
	if( t==INICIO || t==CIERREPARA || t==CIERREMIENTRAS || t==PUNTO ||
		 t==SUBPROGRAMA )
	  return;
	if( ClipBoard )
	  BorrarBloqueObjetos( ClipBoard, 1, 0 );
	ClipBoard= Copia( ObjetoACopiar, 1 );
 }



 void TDrawWindow::CmEdicionCortar(){
	if(Estado.Accion!=EDICION)
	  return;

	ObjetoEjecucion *ObjetoABorrar= ListaHojas.GetSeleccionado();
	if(!ObjetoABorrar)
	  return;

	TipoObjeto t= ObjetoABorrar->GetTipo();
	if( t==INICIO || t==CIERREPARA || t==CIERREMIENTRAS || t==PUNTO ||
		 t==SUBPROGRAMA )
	  return;

	CmEdicionCopiar();

	ObjetoEjecucion *Anterior= ListaHojas.GetActualSubprograma();

	while(1){
	  if(Anterior->GetTipo()==DECISION){
		 if(((OE_Decision*)Anterior)->GetSi()==ObjetoABorrar||
		 ((OE_Decision*)Anterior)->GetNo()==ObjetoABorrar)
			 break;
	  }
	  else
		 if( Anterior->GetSiguienteParaAcomoda()==ObjetoABorrar)
			break;
	  Anterior= Anterior->GetPunteroEnHoja();
	}

	ListaHojas.SetSeleccionado( Anterior );
	ObjetoEjecucion *Acompanyante;

	if( ObjetoABorrar->GetTipo()==DECISION )
	  Acompanyante=((OE_Decision*)ObjetoABorrar)->GetPunto();

	else if( ObjetoABorrar->GetTipo()==CICLOPARA )
	  Acompanyante=((OE_CicloPara*)ObjetoABorrar)->GetCierre();

	else if( ObjetoABorrar->GetTipo()==CICLOMIENTRAS )
	  Acompanyante=((OE_CicloMientras*)ObjetoABorrar)->GetCierre();

	else
	  Acompanyante=ObjetoABorrar;

	//borra sin miedo...
	Estado.SeModifico=true;
	//Borrar del diagrama...
	if( Anterior->GetTipo()==DECISION ){
	  if( ((OE_Decision*)Anterior)->GetSi()==ObjetoABorrar )
		  ((OE_Decision*)Anterior)->
									  SetSi( Acompanyante->GetSiguienteParaAcomoda());
	  else
		 ((OE_Decision*)Anterior)->
									  SetNo( Acompanyante->GetSiguienteParaAcomoda());
	}
	else
	  Anterior->SetSiguienteParaClickInsertar
		 ( Acompanyante-> GetSiguienteParaAcomoda() );

	//Borrar de memoria y de ListaHojas...
	if( ListaHojas.GetActualY()>ObjetoABorrar->GetPosicionY() )
	  ListaHojas.SetActualY( ObjetoABorrar->GetPosicionY() );

	BorrarBloqueObjetos( ObjetoABorrar );

	Acomoda( ListaHojas.GetActualSubprograma(),
					 ListaHojas.GetActualAnchoI(), ListaHojas.GetActualAnchoD(),
					 ListaHojas.GetActualAlto() );
	Ubica( ListaHojas.GetActualSubprograma(), 0, 5 );
	if( !EstaEnPantalla( ListaHojas.GetSeleccionado() ) )
	  ListaHojas.Centrar( ListaHojas.GetSeleccionado() );
	ListaHojas.ArreglarBarras();

				
 }

  void TDrawWindow::CmEdicionEditar(){
	if( Estado.Accion!=EDICION )
	  return;
	ObjetoEjecucion *Ob= ListaHojas.GetSeleccionado();
	if(Ob){
	  TipoObjeto Tipo= Ob->GetTipo();
	  if(Tipo!=PUNTO && Tipo!=CIERREPARA && Tipo!=CIERREMIENTRAS &&
				Tipo!=REGRESE && Tipo!=FIN ){
		 Ob->Editar();
		 Invalidate();
		 //Estado.SeModifico=true;
	  }
	}
 }


 void TDrawWindow::CmEdicionEliminarSubprograma(){

	if(ListaHojas.GetActualSubprograma()->GetTipo()==INICIO)
	  return;
	int Respuesta= MessageBox("¿Eliminar el subprograma?",NOMBREPROGRAMA,
					 MB_YESNO|MB_ICONQUESTION);
	if(Respuesta==IDNO )
	  return;

	ListaHojas.BorrarHoja();
	ListaHojas.ArreglarBarras();
	Estado.SeModifico=true;

 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void TDrawWindow::CmInsertar(){
	int Res=0;
	if( Estado.Accion!=EDICION )
	  return;
	ObjetoEjecucion *Seleccionado= ListaHojas.GetSeleccionado();
	if( !Seleccionado )
	  return;
	if( !EstaEnPantalla( Seleccionado ) )
	  ListaHojas.Centrar( Seleccionado );
	if( Estado.ObjetoEnCursor==NINGUNO )
	  return;
	if( Seleccionado->GetTipo()==DECISION ){
	  Res= PreguntaDecision();
	  if( !Res )
		 return;
	  Res= ((Res==1)?-3:3)*Estado.DistanciaPuntos;
	}

	TPoint Punto( Res + (Seleccionado->GetPosicionX()-ListaHojas.GetActualX())*
						Estado.DistanciaPuntos,
					  (Seleccionado->GetPosicionY()-ListaHojas.GetActualY())*
						Estado.DistanciaPuntos
					);
	EvLButtonDown( 0, Punto );
 }




 void TDrawWindow::CmFlechaArriba(){
	if( Estado.Accion==EJECUCION )
	  return;
	TClientDC G( *VentanaPrincipal );
	Graficador= &G;
	  Graficador->SelectObject( TFont( Estado.DistanciaPuntos,
								  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
									FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));
	ObjetoEjecucion *Seleccionado, *Anterior;
	Seleccionado= ListaHojas.GetSeleccionado();
	if( !Seleccionado ){
	  Anterior= ListaHojas.GetActualSubprograma();
	  ListaHojas.SetSeleccionado( Anterior );
	  if( !EstaEnPantalla( Anterior ) )
		 ListaHojas.Centrar( Anterior );
	  else
		 Anterior->DibujarSeleccionado( ListaHojas.GetActualX(),
														ListaHojas.GetActualY(), 1 );
	  return;
	}
	if( Seleccionado->GetTipo()== INICIO || Seleccionado->GetTipo()== SUBPROGRAMA )
	  return;
	if( Seleccionado->GetTipo()== PUNTO ){
	  Anterior= ListaHojas.GetActualSubprograma();
	  while( 1 ){
		 if( Anterior->GetTipo()== DECISION )
			if( ((OE_Decision*)Anterior)->GetPunto()== Seleccionado )
			  break;
		 Anterior= Anterior->GetPunteroEnHoja();
	  }
	  ListaHojas.SetSeleccionado( Anterior );
	  if( !EstaEnPantalla( Anterior ) )
		 ListaHojas.Centrar( Anterior );
	  else{
		 Seleccionado->DibujarSeleccionado( ListaHojas.GetActualX(),
														ListaHojas.GetActualY(), 0 );
		 Anterior->DibujarSeleccionado( ListaHojas.GetActualX(),
											  ListaHojas.GetActualY(), 1 );
	  }
	  return;
	}
	Anterior= ListaHojas.GetActualSubprograma();
	while( 1 ){

	  if( Anterior->GetTipo()== DECISION ){
		 if( ((OE_Decision*)Anterior)->GetSi()== Seleccionado
			|| ((OE_Decision*)Anterior)->GetNo()== Seleccionado )
			break;
	  }
	  else
		 if( Anterior->GetSiguienteParaAcomoda()== Seleccionado )
			break;
	  Anterior= Anterior->GetPunteroEnHoja();
	}
	ListaHojas.SetSeleccionado( Anterior );
	if( !EstaEnPantalla( Anterior ) )
	  ListaHojas.Centrar( Anterior );
	else{
	  Seleccionado->DibujarSeleccionado( ListaHojas.GetActualX(),
													ListaHojas.GetActualY(), 0 );
	  Anterior->DibujarSeleccionado( ListaHojas.GetActualX(),
													ListaHojas.GetActualY(), 1 );
	}
 }






 void TDrawWindow::CmFlechaAbajo(){
	if( Estado.Accion==EJECUCION )
	  return;
	TClientDC G( *VentanaPrincipal );
	Graficador= &G;
	  Graficador->SelectObject( TFont( Estado.DistanciaPuntos,
								  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
									FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));
	ObjetoEjecucion *Seleccionado, *Siguiente;
	Seleccionado= ListaHojas.GetSeleccionado();
	if( !Seleccionado ){
	  Siguiente= ListaHojas.GetActualSubprograma();
	  ListaHojas.SetSeleccionado( Siguiente );
	  if( !EstaEnPantalla( Siguiente ) )
		 ListaHojas.Centrar( Siguiente );
	  else
		 Siguiente->DibujarSeleccionado( ListaHojas.GetActualX(),
														ListaHojas.GetActualY(), 1 );
	  return;
	}
	if( Seleccionado->GetTipo()== DECISION )
	  Siguiente= ((OE_Decision*)Seleccionado)->GetPunto();
	else
	  Siguiente= Seleccionado->GetSiguienteParaAcomoda();
	if( Siguiente->GetTipo()== FIN || Siguiente->GetTipo()== REGRESE )
	  return;
	ListaHojas.SetSeleccionado( Siguiente );
	if( !EstaEnPantalla( Siguiente ) )
	  ListaHojas.Centrar( Siguiente );
	else{
	  Seleccionado->DibujarSeleccionado( ListaHojas.GetActualX(),
													ListaHojas.GetActualY(), 0 );
	  Siguiente->DibujarSeleccionado( ListaHojas.GetActualX(),
													ListaHojas.GetActualY(), 1 );
	}
 }



 void TDrawWindow::CmFlechaIzquierda(){
	if( Estado.Accion==EJECUCION )
	  return;
	TClientDC G( *VentanaPrincipal );
	Graficador= &G;
	  Graficador->SelectObject( TFont( Estado.DistanciaPuntos,
								  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
									FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));
	ObjetoEjecucion *Seleccionado= ListaHojas.GetSeleccionado();
	if( !Seleccionado ){
	  ObjetoEjecucion *Siguiente= ListaHojas.GetActualSubprograma();
	  ListaHojas.SetSeleccionado( Siguiente );
	  if( !EstaEnPantalla( Siguiente ) )
		 ListaHojas.Centrar( Siguiente );
	  else
		 Siguiente->DibujarSeleccionado( ListaHojas.GetActualX(),
														ListaHojas.GetActualY(), 1 );
	  return;
	}
	if( Seleccionado->GetTipo()==PUNTO ){
	  ObjetoEjecucion *Decision= ListaHojas.GetActualSubprograma();

	  while( 1 ){
		 if( Decision->GetTipo()== DECISION )
			if( ((OE_Decision*)Decision)->GetPunto()== Seleccionado )
			  break;
		 Decision= Decision->GetPunteroEnHoja();
	  }
	  ObjetoEjecucion *Anterior= ((OE_Decision*)Decision)->GetSi();

	  while( 1 ){
		 if( Anterior==Seleccionado ){
			Anterior= Decision;
			break;
		 }
		 if( Anterior->GetTipo()== DECISION )
			Anterior= ((OE_Decision*)Anterior)->GetPunto();
		 if( Anterior->GetSiguienteParaAcomoda()== Seleccionado )
			break;
		 Anterior= Anterior->GetSiguienteParaAcomoda();
	  }
	  ListaHojas.SetSeleccionado( Anterior );
	  if( !EstaEnPantalla( Anterior ) )
		 ListaHojas.Centrar( Anterior );
	  else{
		 Seleccionado->DibujarSeleccionado( ListaHojas.GetActualX(),
														ListaHojas.GetActualY(), 0 );
		 Anterior->DibujarSeleccionado( ListaHojas.GetActualX(),
											  ListaHojas.GetActualY(), 1 );
	  }
	  return;
	}
	if( Seleccionado->GetTipo()!= DECISION )
	  return;
	ObjetoEjecucion *Ob= ((OE_Decision*)Seleccionado)->GetSi();
	ListaHojas.SetSeleccionado( Ob );
	if( !EstaEnPantalla( Ob ) )
	  ListaHojas.Centrar( Ob );
	else{
	  Seleccionado->DibujarSeleccionado( ListaHojas.GetActualX(),
													ListaHojas.GetActualY(), 0 );
	  Ob->DibujarSeleccionado( ListaHojas.GetActualX(),
													ListaHojas.GetActualY(), 1 );
	}

 }

 void TDrawWindow::CmFlechaDerecha(){
	if( Estado.Accion==EJECUCION )
	  return;
	TClientDC G( *VentanaPrincipal );
	Graficador= &G;
	  Graficador->SelectObject( TFont( Estado.DistanciaPuntos,
								  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
									FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));
	ObjetoEjecucion *Seleccionado= ListaHojas.GetSeleccionado();
	if( !Seleccionado ){
	  ObjetoEjecucion *Siguiente= ListaHojas.GetActualSubprograma();
	  ListaHojas.SetSeleccionado( Siguiente );
	  if( !EstaEnPantalla( Siguiente ) )
		 ListaHojas.Centrar( Siguiente );
	  else
		 Siguiente->DibujarSeleccionado( ListaHojas.GetActualX(),
														ListaHojas.GetActualY(), 1 );
	  return;
	}
	if( Seleccionado->GetTipo()==PUNTO ){
	  ObjetoEjecucion *Decision= ListaHojas.GetActualSubprograma();

	  while( 1 ){
		 if( Decision->GetTipo()== DECISION )
			if( ((OE_Decision*)Decision)->GetPunto()== Seleccionado )
			  break;
		 Decision= Decision->GetPunteroEnHoja();
	  }
	  ObjetoEjecucion *Anterior= ((OE_Decision*)Decision)->GetNo();

	  while( 1 ){
		 if( Anterior==Seleccionado ){
			Anterior= Decision;
			break;
		 }
		 if( Anterior->GetTipo()== DECISION )
			Anterior= ((OE_Decision*)Anterior)->GetPunto();
		 if( Anterior->GetSiguienteParaAcomoda()== Seleccionado )
			break;
		 Anterior= Anterior->GetSiguienteParaAcomoda();
	  }
	  ListaHojas.SetSeleccionado( Anterior );
	  if( !EstaEnPantalla( Anterior ) )
		 ListaHojas.Centrar( Anterior );
	  else{
		 Seleccionado->DibujarSeleccionado( ListaHojas.GetActualX(),
														ListaHojas.GetActualY(), 0 );
		 Anterior->DibujarSeleccionado( ListaHojas.GetActualX(),
											  ListaHojas.GetActualY(), 1 );
	  }
	  return;
	}
	if( Seleccionado->GetTipo()!= DECISION )
	  return;
	ObjetoEjecucion *Ob= ((OE_Decision*)Seleccionado)->GetNo();
	ListaHojas.SetSeleccionado( Ob );
	if( !EstaEnPantalla( Ob ) )
	  ListaHojas.Centrar( Ob );
	else{
	  Seleccionado->DibujarSeleccionado( ListaHojas.GetActualX(),
													 ListaHojas.GetActualY(), 0 );
	  Ob->DibujarSeleccionado( ListaHojas.GetActualX(),
										ListaHojas.GetActualY(), 1 );
	}
 }



 void TDrawWindow::CmAngulosEnGrados(){
	if( !Estado.AngulosEnGrados ){
	  Estado.AngulosEnGrados=true;
	  Estado.SeModifico=true;

	}
 }


 void TDrawWindow::CmAngulosEnRadianes(){
	if( Estado.AngulosEnGrados ){
	  Estado.AngulosEnGrados=false;
	  Estado.SeModifico=true;
	}
 }



 void TDrawWindow::CmInicio(){
	if( Estado.Accion==EJECUCION )
	  return;
	TClientDC G( *VentanaPrincipal );
	Graficador= &G;
	  Graficador->SelectObject( TFont( Estado.DistanciaPuntos,
								  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
									FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));
	ObjetoEjecucion *Inicio= ListaHojas.GetActualSubprograma();
	ObjetoEjecucion *Seleccionado= ListaHojas.GetSeleccionado();
	bool EraElMismo= (Seleccionado==Inicio);
	if( Seleccionado && !EraElMismo )
	  Seleccionado->DibujarSeleccionado( ListaHojas.GetActualX(),
													 ListaHojas.GetActualY(), 0 );
	ListaHojas.SetSeleccionado( Inicio );
	if( !EstaEnPantalla( Inicio ) )
	  ListaHojas.Centrar( Inicio );
	else
	  if( !EraElMismo )
		 Inicio->DibujarSeleccionado( ListaHojas.GetActualX(),
												ListaHojas.GetActualY(), 1 );
 }



 void TDrawWindow::CmFin(){
	if( Estado.Accion==EJECUCION )
	  return;
	TClientDC G( *VentanaPrincipal );
	Graficador= &G;
	  Graficador->SelectObject( TFont( Estado.DistanciaPuntos,
								  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
									FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
									DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));
	ObjetoEjecucion *Fin= ListaHojas.GetActualSubprograma();
	while( Fin->GetTipo()!=FIN && Fin->GetTipo()!=REGRESE )
	  Fin= Fin->GetPunteroEnHoja();
	ObjetoEjecucion *AntesDelFin= ListaHojas.GetActualSubprograma();
	while( AntesDelFin->GetSiguienteParaAcomoda()!=Fin )
	  AntesDelFin= AntesDelFin->GetPunteroEnHoja() ;

	ObjetoEjecucion *Seleccionado= ListaHojas.GetSeleccionado();
	bool EraElMismo= (Seleccionado==AntesDelFin);
	if( Seleccionado && !EraElMismo )
	  Seleccionado->DibujarSeleccionado( ListaHojas.GetActualX(),
													 ListaHojas.GetActualY(), 0 );
	ListaHojas.SetSeleccionado( AntesDelFin );
	if( !EstaEnPantalla( AntesDelFin ) )
	  ListaHojas.Centrar( AntesDelFin );

	else
	  if(!EraElMismo )
		 AntesDelFin->DibujarSeleccionado( ListaHojas.GetActualX(),
													  ListaHojas.GetActualY(), 1 );
 }






;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void TDrawWindow::CeFileNew(TCommandEnabler &Ce){
	Ce.Enable( Estado.Accion==EDICION  && !( Estado.EsNuevo &&
				  !Estado.SeModifico));
 }

 void TDrawWindow::CeFileOpen(TCommandEnabler &Ce){
	Ce.Enable( Estado.Accion==EDICION );
 }

 void TDrawWindow::CeFileSave(TCommandEnabler &Ce){
	Ce.Enable( Estado.Accion==EDICION && Estado.SeModifico );
 }

 void TDrawWindow::CeFileSaveAs(TCommandEnabler &Ce){
	Ce.Enable( Estado.Accion==EDICION );
 }


 void TDrawWindow::CeArchivoImprimir(TCommandEnabler &Ce){
	Ce.Enable( Estado.Accion==EDICION );
 }

 void TDrawWindow::CeEdicionEliminar( TCommandEnabler &Ce ){
	ObjetoEjecucion *Ob=ListaHojas.GetSeleccionado();
	bool Cond;

	if(!Ob||Estado.Accion!=EDICION)
	  Cond=false;
	else{
	  TipoObjeto Tipo= Ob->GetTipo();
	  Cond= Tipo!=INICIO&&Tipo!=CIERREMIENTRAS&&Tipo!=CIERREPARA
			  &&Tipo!=PUNTO;
	}
	Ce.Enable(Cond);

 }


 void TDrawWindow::CeEdicionPegar( TCommandEnabler & Ce){
	ObjetoEjecucion *Ob=ListaHojas.GetSeleccionado();

	Ce.Enable( Ob && Estado.Accion==EDICION && ClipBoard );

 }


 void TDrawWindow::CeEdicionCopiar( TCommandEnabler & Ce ){
	ObjetoEjecucion *Ob=ListaHojas.GetSeleccionado();
	bool Cond;

	if(!Ob||Estado.Accion!=EDICION)
	  Cond=false;
	else{
	  TipoObjeto Tipo= Ob->GetTipo();
	  Cond= Tipo!=INICIO && Tipo!=CIERREMIENTRAS && Tipo!=CIERREPARA
			  && Tipo!=PUNTO && Tipo!=SUBPROGRAMA;
	}
	Ce.Enable(Cond);

 }

 void TDrawWindow::CeEdicionCortar( TCommandEnabler &Ce ){
	ObjetoEjecucion *Ob=ListaHojas.GetSeleccionado();
	bool Cond;

	if(!Ob||Estado.Accion!=EDICION)
	  Cond=false;
	else{
	  TipoObjeto Tipo= Ob->GetTipo();
	  Cond= Tipo!=INICIO && Tipo!=CIERREMIENTRAS && Tipo!=CIERREPARA
			  && Tipo!=PUNTO && Tipo!=SUBPROGRAMA;
	}
	Ce.Enable(Cond);
 }

 void TDrawWindow::CeEdicionEliminarSubprograma( TCommandEnabler &Ce ){
	Ce.Enable( ListaHojas.GetActualSubprograma()->GetTipo()==SUBPROGRAMA
				  && Estado.Accion==EDICION &&Estado.ObjetoEnCursor!=SUBPROGRAMA);
 }

 void TDrawWindow::CeEdicionInsertarObjeto( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION && Estado.ObjetoEnCursor!= NINGUNO
				  && ListaHojas.GetSeleccionado() 	);
 }





 void TDrawWindow::CeEdicionEditar( TCommandEnabler &Ce ){
	ObjetoEjecucion *Ob= ListaHojas.GetSeleccionado();
	bool Enable;
	if(!Ob||Estado.Accion!=EDICION)
	  Enable= false;
	else{
	  TipoObjeto Tipo=Ob->GetTipo();
	  Enable=Tipo!=PUNTO && Tipo!=CIERREPARA && Tipo!=CIERREMIENTRAS &&
				Tipo!=REGRESE && Tipo!=FIN ;
	}
	Ce.Enable(Enable);
 }


 void TDrawWindow::CeDepurarEjecutar(TCommandEnabler &Ce){
	Ce.Enable( Estado.Accion!=EJECUCION );
 }

 void TDrawWindow::CeDepurarDetener(TCommandEnabler &Ce){
	Ce.Enable( Estado.Accion!=EDICION );
 }
 void TDrawWindow::CeDepurarPausar(TCommandEnabler &Ce){
	Ce.Enable( Estado.Accion==EJECUCION );
 }
 void TDrawWindow::CeDepurarPasoSimple(TCommandEnabler &Ce){
	Ce.Enable( Estado.Accion!=EJECUCION );
 }
 void TDrawWindow::CeDepurarEjecutarHasta(TCommandEnabler &Ce){
	Ce.Enable( Estado.Accion!=EJECUCION && ListaHojas.GetSeleccionado() );
 }


 void TDrawWindow::CeVerSubprogramaMas( TCommandEnabler &Ce ){
	Ce.Enable(ListaHojas.FuncionaSubprogramaMas() &&Estado.Accion!=EJECUCION);
 }
 void TDrawWindow::CeVerSubprogramaMenos( TCommandEnabler &Ce ){
	Ce.Enable(ListaHojas.FuncionaSubprogramaMenos()
				 && Estado.Accion!=EJECUCION );
 }



 void TDrawWindow::CeNuevoSubprograma( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION );
 }

 void TDrawWindow::CeObjetoSalida ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION&&Estado.ObjetoEnCursor!=IMPRESION);
 }
 void TDrawWindow::CeObjetoLlamada ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION&&Estado.ObjetoEnCursor!= LLAMADA);
 }
 void TDrawWindow::CeObjetoLectura ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION&&Estado.ObjetoEnCursor!=LECTURA );
 }
 void TDrawWindow::CeObjetoDesicion ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION&&Estado.ObjetoEnCursor!=DECISION);
 }
 void TDrawWindow::CeObjetoPara ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION&&Estado.ObjetoEnCursor!=CICLOPARA);
 }
 void TDrawWindow::CeObjetoMientras ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION&&Estado.ObjetoEnCursor!=CICLOMIENTRAS);
 }
 void TDrawWindow::CeObjetoAsignacion( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION&&Estado.ObjetoEnCursor!=ASIGNACION);
 }
 void TDrawWindow::CeSeleccionarCursor ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION&&Estado.ObjetoEnCursor!=NINGUNO);
 }

 void TDrawWindow::CeAbout( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion!=EJECUCION );
 }
 void TDrawWindow::CeAyudaContenido( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion!=EJECUCION );
 }
 void TDrawWindow::CeAyudaUsoAyuda( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion!=EJECUCION );
 }
 void TDrawWindow::CeAyudaLicencia( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion!=EJECUCION );
 }
 void TDrawWindow::CeAyudaRegistrarAhora( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion!=EJECUCION );
 }

 void TDrawWindow::CeVerZoomMas( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion!=EJECUCION );
 }


 void TDrawWindow::CeVerZoomMenos( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion!=EJECUCION );
 }


 void TDrawWindow::CeVerDepurador( TCommandEnabler &Ce ){
	Ce.SetCheck((Estado.DepuradorVisible)?(TCommandEnabler::Checked)
					:(TCommandEnabler::Unchecked));

 }


 void TDrawWindow::CeAngulosEnGrados( TCommandEnabler &Ce ){
	Ce.SetCheck((Estado.AngulosEnGrados)?(TCommandEnabler::Checked)
					:(TCommandEnabler::Unchecked));
	Ce.Enable( Estado.Accion==EDICION );
 }


 void TDrawWindow::CeAngulosEnRadianes( TCommandEnabler &Ce ){
	Ce.SetCheck((!Estado.AngulosEnGrados)?(TCommandEnabler::Checked)
					:(TCommandEnabler::Unchecked));
	Ce.Enable( Estado.Accion==EDICION );
 }


 void TDrawWindow::CeObjetoSalidaMenu ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION );
	Ce.SetCheck( Estado.ObjetoEnCursor==IMPRESION );
 }
 void TDrawWindow::CeObjetoLlamadaMenu ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION );
	Ce.SetCheck( Estado.ObjetoEnCursor== LLAMADA );
 }
 void TDrawWindow::CeObjetoLecturaMenu ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION );
	Ce.SetCheck( Estado.ObjetoEnCursor==LECTURA );
 }
 void TDrawWindow::CeObjetoDesicionMenu ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION );
	Ce.SetCheck( Estado.ObjetoEnCursor==DECISION);
 }
 void TDrawWindow::CeObjetoParaMenu ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION );
	Ce.SetCheck( Estado.ObjetoEnCursor==CICLOPARA );
 }
 void TDrawWindow::CeObjetoMientrasMenu ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION );
	Ce.SetCheck( Estado.ObjetoEnCursor==CICLOMIENTRAS );
 }
 void TDrawWindow::CeObjetoAsignacionMenu ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION );
	Ce.SetCheck( Estado.ObjetoEnCursor==ASIGNACION);
 }
 void TDrawWindow::CeSeleccionarCursorMenu ( TCommandEnabler &Ce ){
	Ce.Enable( Estado.Accion==EDICION);
	Ce.SetCheck( Estado.ObjetoEnCursor==NINGUNO );
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 bool TDrawWindow::AbrirArchivo(){
	bool Retorno= ListaHojas.Abrir( Archivo->FileName );
	Invalidate();
   return Retorno;
 }

 void TDrawWindow::GuardarArchivo(){
	ListaHojas.Guardar(Archivo->FileName);
 }



 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


 class TDrawApp : public TApplication {
	public:

	  TDrawApp() : TApplication() {}
	  void InitMainWindow();
	  bool IdleAction(long );


 };



 bool TDrawApp::IdleAction(long x){
	static int UltimaAccion= EJECUCION;
	if( Estado.Accion!=UltimaAccion ){
	  switch( Estado.Accion ){
	  case EJECUCION:
		 if(Estado.ObjetoHasta)
			BarraEstado->SetText("Acción:[ Depuración/ejecutar hasta ]");
		 else
		   BarraEstado->SetText("Acción:[ Ejecución ]");
		 break;
	  case EDICION:
		 BarraEstado->SetText("Acción:[ Edición ]");
		 break;
	  case DEPURACION:
			BarraEstado->SetText("Acción:[ Depuración/paso a paso ]");
		 break;
	  };
	  UltimaAccion= Estado.Accion;
	}

	if( Estado.Accion==EJECUCION )
	  for( int i= 64; i && Estado.Accion==EJECUCION; --i ){
		 TipoObjeto Tipo= Estado.ObjetoEnEjecucion->GetTipo();
		 EjecutarActual();
		 if(Tipo==LECTURA || Tipo==IMPRESION)
			break;
	  }
	TApplication::IdleAction(x);
	return true;
 }



 void TDrawApp::InitMainWindow(){

	VentanaPrincipal= new TDrawWindow;
	VentanaPrincipal->Attr.AccelTable = "COMMANDS";
	TDecoratedFrame *Frame= new TDecoratedFrame( 0,NOMBREPROGRAMA, VentanaPrincipal, true);

   SetCmdShow(SW_SHOWMAXIMIZED); //Maximizada

	//Construccion de la barra de estado:
	TStatusBar* sb = new TStatusBar(Frame, TGadget::Plain);
	/*None, Plain, Raised, Recessed, or Embossed--*/
	BarraEstado= sb;

	// Construccion de las barras de control:
	TControlBar* cb = new TControlBar( Frame );
	TControlBar* cb2= new TControlBar( Frame );



	//cb->SetBkgndColor( TColor::LtGray);
	//cb2->SetBkgndColor(TColor::Gray);
	cb->SetHintMode(TGadgetWindow::EnterHints);
	cb2->SetHintMode(TGadgetWindow::EnterHints );


	// Botones de la barra superior:
	cb->Insert( *new TButtonGadget( CM_ARCHIVONUEVO, CM_ARCHIVONUEVO, TButtonGadget::Command));
	//XXXX->SetShadowStyle(2);
	cb->Insert( *new TButtonGadget(CM_ARCHIVOABRIR, CM_ARCHIVOABRIR, TButtonGadget::Command));
	cb->Insert( *new TButtonGadget(CM_ARCHIVOGUARDAR, CM_ARCHIVOGUARDAR, TButtonGadget::Command));
	  cb->Insert(*new TSeparatorGadget(9));
	cb->Insert( *new TButtonGadget(CM_ARCHIVOIMPRIMIR, CM_ARCHIVOIMPRIMIR, TButtonGadget::Command));
	  cb->Insert(*new TSeparatorGadget(9));
	cb->Insert( *new TButtonGadget(CM_EDICIONCORTAR,CM_EDICIONCORTAR, TButtonGadget::Command));
	cb->Insert( *new TButtonGadget(CM_EDICIONCOPIAR,CM_EDICIONCOPIAR, TButtonGadget::Command));
	cb->Insert( *new TButtonGadget(CM_EDICIONPEGAR, CM_EDICIONPEGAR, TButtonGadget::Command));
	cb->Insert( *new TButtonGadget(CM_EDICIONELIMINAR, CM_EDICIONELIMINAR, TButtonGadget::Command));
	  cb->Insert(*new TSeparatorGadget(20));
	cb->Insert( *new TButtonGadget(CM_DEPURAREJECUTAR, CM_DEPURAREJECUTAR, TButtonGadget::Command));
	cb->Insert( *new TButtonGadget(CM_DEPURARDETENER, CM_DEPURARDETENER, TButtonGadget::Command));
	cb->Insert( *new TButtonGadget(CM_DEPURARPAUSAR, CM_DEPURARPAUSAR, TButtonGadget::Command));
	  cb->Insert(*new TSeparatorGadget(9));
	cb->Insert( *new TButtonGadget(CM_DEPURARPASOSIMPLE, CM_DEPURARPASOSIMPLE, TButtonGadget::Command));
	cb->Insert( *new TButtonGadget(CM_DEPURAREJECUTARHASTA, CM_DEPURAREJECUTARHASTA, TButtonGadget::Command));
	cb->Insert( *new TButtonGadget(CM_VERDEPURADORBOTON, CM_VERDEPURADOR, TButtonGadget::Command));

	  cb->Insert(*new TSeparatorGadget(20));
	cb->Insert( *new TButtonGadget(CM_VERZOOMMENOS,CM_VERZOOMMENOS, TButtonGadget::Command));
	cb->Insert( *new TButtonGadget(CM_VERZOOMMAS,CM_VERZOOMMAS, TButtonGadget::Command));


	// Botones de la barra inferior:
	cb2->Insert( *new TButtonGadget(CM_SELECCIONACURSOR, CM_SELECCIONACURSORMENU, TButtonGadget::Command));
	cb2->Insert( *new TButtonGadget(CM_OBJETOASIGNACION, CM_OBJETOASIGNACIONMENU, TButtonGadget::Command));
	cb2->Insert( *new TButtonGadget(CM_OBJETOMIENTRAS, CM_OBJETOMIENTRASMENU, TButtonGadget::Command));
	cb2->Insert( *new TButtonGadget(CM_OBJETOPARA, CM_OBJETOPARAMENU, TButtonGadget::Command));
	cb2->Insert( *new TButtonGadget(CM_OBJETODECISION, CM_OBJETODECISIONMENU, TButtonGadget::Command));
	cb2->Insert( *new TButtonGadget(CM_OBJETOLECTURA, CM_OBJETOLECTURAMENU, TButtonGadget::Command));
	cb2->Insert( *new TButtonGadget(CM_OBJETOSALIDA, CM_OBJETOSALIDAMENU, TButtonGadget::Command));
	cb2->Insert( *new TButtonGadget(CM_OBJETOLLAMADA, CM_OBJETOLLAMADAMENU, TButtonGadget::Command));
	  cb2->Insert(*new TSeparatorGadget(9));
	cb2->Insert( *new TButtonGadget(CM_VERSUBPROGRAMAMENOS,CM_VERSUBPROGRAMAMENOS, TButtonGadget::Command));
	cb2->Insert( *new TButtonGadget(CM_VERSUBPROGRAMAMAS,CM_VERSUBPROGRAMAMAS, TButtonGadget::Command));
	cb2->Insert( *new TButtonGadget(CM_OBJETONUEVOSUBPROGRAMA, CM_OBJETONUEVOSUBPROGRAMA, TButtonGadget::Command));
	cb2->Insert( *new TButtonGadget(CM_EDICIONELIMINARSUBPROGRAMA, CM_EDICIONELIMINARSUBPROGRAMA, TButtonGadget::Command));

	// Insert the status bar and control bars into the frame
	Frame->Insert(*sb, TDecoratedFrame::Bottom);
	Frame->Insert(*cb, TDecoratedFrame::Top);
	Frame->Insert(*cb2, TDecoratedFrame::Top );

	SetMainWindow( Frame );
	GetMainWindow()->AssignMenu("COMMANDS");
	GetMainWindow()->SetIcon(this, "IDI_CREACION");
   GetMainWindow()->SetIconSm(this, "IDI_CREACION");
  	EnableBWCC();
	EnableBWCC(false);
 }



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


 TDrawApp *Aplicacion;

  void CambiarTitulo( char * Titulo ){
	Aplicacion->GetMainWindow()->SetCaption( Titulo );
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 int OwlMain(int , char*[]){
   DirectorioActual(PrimerDirectorio);
	ListaHojas.NuevaHoja();
	OE_Inicio *Inicio= new OE_Inicio;
	OE_Fin *Fin= new OE_Fin;
	Inicio->SetSiguiente( Fin );
	ListaHojas.InsertarObjeto( Inicio );
	ListaHojas.InsertarObjeto( Fin );

	Acomoda( ListaHojas.GetActualSubprograma(), ListaHojas.GetActualAnchoI(), ListaHojas.GetActualAnchoD()
					,  ListaHojas.GetActualAlto() );
	Ubica( ListaHojas.GetActualSubprograma(), 0, 5 );

	Aplicacion= new TDrawApp;
	int Retorno=1;
   Estado.Registrado= true;
   Aplicacion->Run();
	return Retorno;
}
