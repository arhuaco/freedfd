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


//----------------------------------------------------------------------------
// TWindowPrintout

 class TWindowPrintout : public TPrintout {
	public:
	  TWindowPrintout(const char* title, TWindow* window);

	  void GetDialogInfo(int& minPage, int& maxPage,
							  int& selFromPage, int& selToPage);
	  void PrintPage(int page, TRect& rect, unsigned flags);
	  void SetBanding(BOOL b) {Banding = b;}
	  bool HasPage(int pageNumber);
	  void ArreglarPaginas();

	protected:
	  TWindow* Window;
	  BOOL     Scale;
 };




 TWindowPrintout::TWindowPrintout(const char* title, TWindow* window)
	: TPrintout(title)
 {
	Window = window;
	Scale = TRUE;
 }


 void TWindowPrintout::ArreglarPaginas(){
	int PuntosIzquierda, PuntosDerecha, PuntosVerticales;
	int PaginasIzquierda, PaginasDerecha;

	PuntosVerticales= (5+ListaHojas.GetActualAlto())*Estado.DistanciaPuntos;
	PuntosIzquierda= ListaHojas.GetActualAnchoI()*Estado.DistanciaPuntos;
	PuntosDerecha= ListaHojas.GetActualAnchoD()*Estado.DistanciaPuntos;

	PuntosIzquierda-= PageSize.cx/2;
	PuntosDerecha-= PageSize.cx/2;

	PaginasIzquierda= (PuntosIzquierda>0)?1+PuntosIzquierda/PageSize.cx : 0 ;
	PaginasDerecha= (PuntosDerecha>0)?1+PuntosDerecha/PageSize.cx : 0 ;

	ListaHojas.GetActualInicioXImp()= -int((0.5+float(PaginasIzquierda))*float(PageSize.cx));
	ListaHojas.GetActualNPaginasX()= 1+PaginasIzquierda+PaginasDerecha;
	ListaHojas.GetActualNPaginasY()= PuntosVerticales/PageSize.cy + 1;
 }






 /*
 OJO:
	 - HasPage debe funcionar correctamente ( solo imprime una pagina ).
	 - Hay que adecuar las funciones para poder imprimir el diagrama completo
 */

 void TWindowPrintout::PrintPage(int Pagina, TRect& /*rect*/, unsigned){

	Estado.DistanciaPuntos*=4;

	//DC->SetMapMode(MM_LOMETRIC);

	if( Pagina==1 )
	  ArreglarPaginas();

	//no se va a utilizar...   Window->Paint(*DC, FALSE, rect);

	Impresor= DC;
	Impresor->SelectObject(*LapizSolido);
	Impresor->SelectObject( *Brocha );
	Impresor->SelectObject( TFont( Estado.DistanciaPuntos,
			  int(float(Estado.DistanciaPuntos)*3.0/7.0), 0, 0,
			  FW_NORMAL, 0, 0, 0,ANSI_CHARSET,
			  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			  DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, NULL));

	int Fila= (Pagina-1)/ListaHojas.GetActualNPaginasX();
	int Columna= (Pagina-1)%ListaHojas.GetActualNPaginasX();

	int ActualX= ListaHojas.GetActualInicioXImp() + Columna*PageSize.cx ;
	int ActualY= Fila*PageSize.cy;

	/*
	Se llama a la funcion DibujarImp (hecha en la casa de fabian el 26-09-97)
	de todos los objetos del actual subprograma, enviando a ActualX y ActualY
	como argumentos para el punto de referencia,  para que se dibujen en la
	pagina que se esta imprimiendo .
	*/
	ObjetoEjecucion *ActualObjeto= ListaHojas.GetActualSubprograma();

	while( ActualObjeto ){
	  ActualObjeto->DibujarImp( ActualX, ActualY );
	  ActualObjeto= ActualObjeto->GetPunteroEnHoja();
	}

	Estado.DistanciaPuntos/=4;

 }

 // Do not enable page range in the print dialog since only one page is
 // available to be printed
 //
 void TWindowPrintout::GetDialogInfo(int& minPage, int& maxPage,
										 int& selFromPage, int& selToPage){
	minPage= 0;
	maxPage= 0;
	selFromPage= 0;
	selToPage= 0;
 }

 bool TWindowPrintout::HasPage( int Pagina ){
	int Filas= ListaHojas.GetActualNPaginasX();
	int Columnas= ListaHojas.GetActualNPaginasY();
	return (Pagina==1)||(Pagina <= (Filas*Columnas));

 }

