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


#ifndef __EDITOR_CPP
#define __EDITOR_CPP

#define CLAVE_ARCHIVO_DFD "\x04 Dfd \x02\x08(c)"


 class ListaHojas{
	  struct NodoHoja{
		 int ActualX;
		 int ActualY;
		 int AnchoI;
		 int AnchoD;
		 int Alto;
		 NodoHoja *Sig;
		 ObjetoEjecucion *Seleccionado;
		 ObjetoEjecucion *Resaltado;
		 ObjetoEjecucion *Inicio, *Ultimo;
		 //datos para impresion...
		 int InicioXImp;
       int NPaginasX, NPaginasY;

	  }*Inicio, *Actual;

	  int Contador;

	  ofstream ArchivoG;
	  ifstream ArchivoA;
	  int NHojas;

	public:
	  ListaHojas(){
		 NHojas= 0;
		 Inicio= 0;
	  }
	  ObjetoEjecucion *BuscarSubprograma( char * );
	  ObjetoEjecucion *BuscarSubprograma( ObjetoEjecucion* );
     bool BuscarSubprogramaRepetido( char *Nombre );
	  ObjetoEjecucion *GetActualSubprograma(){ return Actual->Inicio; }
	  void SubprogramaMas();
	  void SubprogramaMenos();
	  int GetActualX(){ return Actual->ActualX; }
	  int GetActualY(){ return Actual->ActualY; }
	  int &GetActualAnchoI(){ return Actual->AnchoI; }
	  int &GetActualAnchoD(){ return Actual->AnchoD; }
	  int &GetActualAlto(){ return Actual->Alto; }
	  ObjetoEjecucion *GetSeleccionado(){ return Actual->Seleccionado; }
	  ObjetoEjecucion *GetResaltado(){ return Actual->Resaltado; }
	  ObjetoEjecucion *GetInicioAlgoritmo(){ return Inicio->Inicio; }
	  void Preprocesar();
	  void Despreprocesar();
	  void SetSeleccionado( ObjetoEjecucion *Ob ){ Actual->Seleccionado= Ob; }
	  void SetResaltado( ObjetoEjecucion *Ob ){ Actual->Resaltado= Ob; }
	  void DesResaltar();
	  void ArreglarBarras();
	  void NuevaHoja();
	  void BorrarHoja();
	  void Vacear();
	  void BorrarObjeto( ObjetoEjecucion *Ob );
	  void InsertarObjeto( ObjetoEjecucion * );
	  void CambiarHoja( ObjetoEjecucion* );
	  void Centrar( ObjetoEjecucion * );
	  void SetActualX( int x ){ Actual->ActualX= x; }
	  void SetActualY( int y ){ Actual->ActualY= y; }
	  void Guardar( char* );
	  bool Abrir( char * );
	  void GuardarObjeto( ofstream&, ObjetoEjecucion * );
	  ObjetoEjecucion *CargarObjeto( ifstream&, ObjetoEjecucion* );
	  bool FuncionaSubprogramaMas();
	  bool FuncionaSubprogramaMenos();
	  bool EstaEnHoja( ObjetoEjecucion * );
	  // funciones para impresion...
	  int &GetActualInicioXImp(){ return Actual->InicioXImp; }
	  int &GetActualNPaginasX(){ return Actual->NPaginasX; }
	  int &GetActualNPaginasY(){ return Actual->NPaginasY; }

 }ListaHojas;

 bool ListaHojas::FuncionaSubprogramaMas(){
	return Actual->Sig?true:false;
 }
 bool ListaHojas::FuncionaSubprogramaMenos(){
	return Inicio!=Actual;
 }

 bool ListaHojas::EstaEnHoja( ObjetoEjecucion *Ob ){
	ObjetoEjecucion *p= Actual->Inicio;
	while( p ){
	  if( p==Ob )
		 return true;
	  else
		 p= p->GetPunteroEnHoja();

	}
	return false;
 }


 void ListaHojas::CambiarHoja( ObjetoEjecucion *Objeto ){
	for( NodoHoja *Hoja= Inicio; Hoja; Hoja= Hoja->Sig )
	  for( ObjetoEjecucion *Ob= Hoja->Inicio; Ob; Ob= Ob->GetPunteroEnHoja() )
		 if( Ob==Objeto ){
			Actual= Hoja;
			return;
		 }
 }




 void ListaHojas:: NuevaHoja(){
	NodoHoja *NuevaHoja= new NodoHoja;

	NuevaHoja->Inicio= 0;
	NuevaHoja->Seleccionado= 0;
	NuevaHoja->Resaltado= 0;
	NuevaHoja->ActualX= NuevaHoja->ActualY= 0;
	if(!Inicio){
	  NuevaHoja->Sig= 0;             // Primera Inserción
	  Inicio= Actual= NuevaHoja;
	}
	else{
	  NodoHoja *Persigue= Inicio;
	  for( NodoHoja *Corre= Inicio->Sig ;  Corre ; Corre= Corre->Sig )
		 Persigue=Persigue->Sig;
	  NuevaHoja->Sig= 0;                // Inserta de último
	  Persigue->Sig= Actual= NuevaHoja;
	}
	++NHojas;
 }

 void ListaHojas::InsertarObjeto( ObjetoEjecucion *Ob ){
	Ob->SetPunteroEnHoja(0);
	if( !Actual->Inicio )
	  Actual->Inicio= Ob;
	else{
	  Ob->SetPunteroEnHoja( Actual->Inicio->GetPunteroEnHoja() );
	  Actual->Inicio->SetPunteroEnHoja( Ob );
	}
 }

 void ListaHojas::SubprogramaMas(){
	if( Actual->Sig ){
	  Actual= Actual->Sig;
	  ArreglarBarras();
	}
 }

 void ListaHojas::SubprogramaMenos(){
	for( NodoHoja *Anterior= Inicio ; Anterior->Sig ; Anterior=Anterior->Sig )
	  if( Anterior->Sig == Actual ){
		 Actual= Anterior;
		 ArreglarBarras();
		 return;
	  }
 }


 void ListaHojas::BorrarObjeto( ObjetoEjecucion *Ob ){
	ObjetoEjecucion *p= Actual->Inicio, *Ant=0;
	while( p ){
	  if( p==Ob ){
		 Actual->Resaltado= 0;
		 if( p==Actual->Inicio )
			Actual->Inicio= p->GetPunteroEnHoja();
		 else
			Ant->SetPunteroEnHoja( p->GetPunteroEnHoja() );
		 delete Ob;
		 break;
	  }
	  Ant= p;
	  p= p->GetPunteroEnHoja();
	}
 }


 void ListaHojas::BorrarHoja(){
	while( Actual->Inicio )
	  BorrarObjeto( Actual->Inicio );
   NodoHoja *i= Inicio, *Ant;
	for( ; i!=Actual; Ant= i, i= i->Sig );
	if( i==Inicio )
	  Inicio= Actual= Inicio->Sig;
	else{
	  Ant->Sig= i->Sig;
	  Actual= Ant;
	}
	delete i;
	--NHojas;
 }



 void ListaHojas::Centrar( ObjetoEjecucion *Ob ){
	TRect Ventana;
	VentanaPrincipal->GetClientRect( Ventana );
	int  MediaVentX= Ventana.right/(2*Estado.DistanciaPuntos);
	int  MediaVentY= Ventana.bottom/(2*Estado.DistanciaPuntos);
	Actual->ActualX= Ob->GetPosicionX()-MediaVentX;
	Actual->ActualY= Ob->GetPosicionY()-MediaVentY;
	if( Actual->ActualY < 0 )
	  Actual->ActualY= 0;
	ArreglarBarras();
 }


 void ListaHojas::Vacear(){
	while( Inicio )
	  BorrarHoja( );
	NHojas= 0;
 }


 ObjetoEjecucion *ListaHojas::BuscarSubprograma( char *Nombre ){
	for( NodoHoja *i= Inicio->Sig; i; i= i->Sig )
	  if( !strcmpi( Nombre, ((OE_Subprograma*)(i->Inicio))
			->GetCadenaNombre() ) )
		 return i->Inicio;
	return 0;
 }


 bool ListaHojas::BuscarSubprogramaRepetido( char *Nombre ){
   int Veces= 0;
   for( NodoHoja *i= Inicio->Sig; i; i= i->Sig )
	  if( !strcmpi( Nombre, ((OE_Subprograma*)(i->Inicio))
			->GetCadenaNombre() ) )
     {
       ++Veces;
       if(Veces==2)
         return true;
     }
	return false;
 }

 
 ObjetoEjecucion *ListaHojas::BuscarSubprograma( ObjetoEjecucion *Ob ){
	ObjetoEjecucion *ActualOb;
	for( NodoHoja *i= Inicio; i; i= i->Sig )
	  for( ActualOb= i->Inicio; ActualOb; ActualOb= ActualOb->GetPunteroEnHoja() )
		 if( Ob== ActualOb )
			return i->Inicio;

	return 0;
 }


 void ListaHojas::Preprocesar(){
	Actual->Resaltado= 0;
   NodoHoja *Hoja =Inicio;
	for( ; Hoja ; Hoja= Hoja->Sig )
	  for( ObjetoEjecucion *Ob= Hoja->Inicio ; Ob;
			 Ob= Ob->GetPunteroEnHoja() ){
		 Ob->Preprocesar();
		 if( Buzon.GetHuboError() ){
			Despreprocesar();
			Actual= Hoja;
			Actual->Resaltado= Ob;
			Centrar( Ob );
			return;
		 }
	  }


// Subprogramas repetidos ?
	for( Hoja =Inicio; Hoja ; Hoja= Hoja->Sig )
	  for( ObjetoEjecucion *Ob= Hoja->Inicio ; Ob ;Ob= Ob->GetPunteroEnHoja() )
		 if( Ob->GetTipo()== SUBPROGRAMA ){
         char *NombreSubprograma= (( OE_Subprograma*)Ob)->GetCadenaNombre();
			if( BuscarSubprogramaRepetido( NombreSubprograma ) ){
			  Buzon.SetIdentificadorAsociado( NombreSubprograma );
			  Buzon.Error( SUBPROGRAMA_REPETIDO );
			  Despreprocesar();
			  Actual= Hoja;
			  Actual->Resaltado= Ob;
			  Centrar( Ob );
			  return;
			}
       }

//sus parametros no concuerdan ?
	for( Hoja =Inicio; Hoja ; Hoja= Hoja->Sig )
	  for( ObjetoEjecucion *Ob= Hoja->Inicio ; Ob ;Ob= Ob->GetPunteroEnHoja() )
		 if( Ob->GetTipo()== LLAMADA ){
			char *NombreSubprograma= (( OE_Llamada*)Ob)->GetCadenaNombre();
			ObjetoEjecucion *Sub= BuscarSubprograma( NombreSubprograma );
			if( !Sub ){
			  Buzon.SetIdentificadorAsociado( NombreSubprograma );
			  Buzon.Error( SUBPROGRAMA_NO_EXISTE );
			  Despreprocesar();
			  Actual= Hoja;
			  Actual->Resaltado= Ob;
			  Centrar( Ob );
			  return;
			}
         int NParam= ((OE_Subprograma*)Sub)->GetNParametros();
			int NArg= ((OE_Llamada*)Ob)->GetNArgumentos();
			if( NArg!= NParam ){
			  Buzon.SetIdentificadorAsociado( NombreSubprograma );
			  Buzon.Error( (NArg>NParam)?MUCHOS_ARGUMENTOS:POCOS_ARGUMENTOS );
			  Despreprocesar();
			  Actual= Hoja;
			  Actual->Resaltado= Ob;
			  Centrar( Ob );
			  return;
			}

			((OE_Llamada*)Ob)->SetSubprograma((OE_Subprograma*)Sub );
		 }


 }


 void ListaHojas::DesResaltar(){
	for( NodoHoja *Hoja =Inicio; Hoja ; Hoja= Hoja->Sig )
	  Hoja->Resaltado= 0;
 }



 void ListaHojas::Despreprocesar(){
	for( NodoHoja *Hoja= Inicio; Hoja; Hoja= Hoja->Sig )
	  for( ObjetoEjecucion *Ob= Hoja->Inicio ; Ob ; Ob= Ob->GetPunteroEnHoja())
		  Ob->Despreprocesar();
 }



 void ListaHojas::ArreglarBarras(){

	TRect Ventana;
	VentanaPrincipal->GetClientRect( Ventana );



	if( Ventana.right<=0 || Ventana.bottom<=0 )
	  return ;
	int MinimoX, MaximoX, MaximoY;
	int  MediaVentX= Ventana.right/(2*Estado.DistanciaPuntos);

	MinimoX= -(Actual->AnchoI+2*MediaVentX);
	MaximoX= Actual->AnchoD;
	MaximoY= Actual->Alto + 2;
	int AnchoX= MaximoX-MinimoX;

	if( VentanaPrincipal->GetScrollPos( SB_HORZ ) > AnchoX )
	  VentanaPrincipal->SetScrollPos( SB_HORZ, AnchoX, false );

	VentanaPrincipal->SetScrollRange( SB_HORZ, 0, AnchoX );

	if( VentanaPrincipal->GetScrollPos( SB_VERT ) > MaximoY )
	  VentanaPrincipal->SetScrollPos( SB_VERT, MaximoY, false );
	VentanaPrincipal->SetScrollRange( SB_VERT, 0, MaximoY );



	if( Actual->AnchoI < MediaVentX && Actual->AnchoD < MediaVentX )
	  Actual->ActualX=-MediaVentX;

	VentanaPrincipal->SetScrollPos( SB_HORZ,Actual->ActualX+2*
											MediaVentX+Actual->AnchoI+1 );
	VentanaPrincipal->SetScrollPos( SB_VERT, Actual->ActualY );

	VentanaPrincipal->Invalidate();
 }


  void ListaHojas::Guardar( char *A ){
	 ofstream ArchivoG( A );
	 if( !ArchivoG ){
 		VentanaPrincipal->MessageBox(TXT_DIALOG_UNABLE_SAVE_FILE, NOMBREPROGRAMA ,
					  MB_OK | MB_ICONEXCLAMATION);
		return;
	 }
    ArchivoG << CLAVE_ARCHIVO_DFD << endl;
	 ArchivoG << 1 << endl; //Versión
	 ArchivoG << Estado.AngulosEnGrados <<endl;
	 ArchivoG << NHojas << endl;
	 for( NodoHoja *Hoja= Inicio; Hoja; Hoja= Hoja->Sig )
		GuardarObjeto( ArchivoG, Hoja->Inicio  );
	 Estado.EsNuevo= Estado.SeModifico= false;
  }




  bool ListaHojas::Abrir( char *A ){

	 void Acomoda( ObjetoEjecucion*, int&, int&, int& );
	 void Ubica( ObjetoEjecucion*, int, int );

	 ifstream ArchivoA( A );
	 if( !ArchivoA ){
		VentanaPrincipal->MessageBox(TXT_DIALOG_UNABLE_OPEN_FILE, NOMBREPROGRAMA,
					  MB_OK | MB_ICONEXCLAMATION);
		return false;
	 }

    char *CadenaClave=CLAVE_ARCHIVO_DFD;
    int Len=strlen(CLAVE_ARCHIVO_DFD);
    ArchivoA.unsetf(ios::skipws);

    for( int i=0 ; i<Len ; i++ ){
       char Caracter;
   	 ArchivoA >> Caracter;
       if(CadenaClave[i]!=Caracter){
		   VentanaPrincipal->MessageBox(TXT_DIALOG_INVALID_FILE_FORMAT, NOMBREPROGRAMA,
					  MB_OK | MB_ICONEXCLAMATION);
         return false;
       }
    }
    ArchivoA.setf(ios::skipws);

 	 Vacear(); // Libera todos los objetos del diagrama actual

	 int Version;
	 ArchivoA >> Version;
	 ArchivoA >> Estado.AngulosEnGrados;
	 int n;
	 ArchivoA >> n;
	 for( int i= 0; i<n; ++i ){
		NuevaHoja();
		CargarObjeto( ArchivoA, 0 );
		Acomoda( GetActualSubprograma(), GetActualAnchoI(), GetActualAnchoD(),
					GetActualAlto() );
		Ubica( GetActualSubprograma(), 0, 5 );

	 }
	 Estado.ObjetoEnCursor= NINGUNO;
	 NHojas= n;
	 Actual= Inicio;
	 Centrar( Actual->Inicio );
	 Estado.SeModifico= Estado.EsNuevo= false;
    return true;
  }


  bool HayCadena( char *Cadena ){
	 if( Cadena )
		if( Cadena[0] )
		  return true;
	 return false;
  }


  void ListaHojas::GuardarObjeto( ofstream &ArchivoG, ObjetoEjecucion *Ob ){
	 switch( Ob->GetTipo() ){

		case PUNTO:
		case CIERREPARA:
		case CIERREMIENTRAS:
		  return;
		case INICIO:
		{
		  OE_Inicio *Objeto= (OE_Inicio*)Ob;
		  ArchivoG << INICIO << endl;
		  if( HayCadena(Objeto->GetDescripcion()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetDescripcion()) << endl
						 << Objeto->GetDescripcion() << endl;
		  else
			 ArchivoG << 0 << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetSiguiente() );
		  return;
		}
		case FIN:
		{
		  ArchivoG << FIN << endl;
		  return;
		}
		case SUBPROGRAMA:
		{
		  OE_Subprograma *Objeto= (OE_Subprograma*)Ob;
		  ArchivoG << SUBPROGRAMA << endl;
		  if( HayCadena(Objeto->GetCadenaNombre()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadenaNombre()) << endl
						 << Objeto->GetCadenaNombre() << endl;
		  else
			 ArchivoG << 0 << endl;
		  if( HayCadena(Objeto->GetCadenaParametros()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadenaParametros())
						 << endl << Objeto->GetCadenaParametros() << endl;
		  else
			 ArchivoG << 0 << endl;
		  if( HayCadena(Objeto->GetCadenaDescripcion()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadenaDescripcion())
						 << endl << Objeto->GetCadenaDescripcion() << endl;
		  else
			 ArchivoG << 0 << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetSiguiente() );
		  return;
		}
		case REGRESE:
		{
		  ArchivoG << REGRESE << endl;
		  return;
		}
		case IMPRESION:
		{
		  OE_Impresion *Objeto= (OE_Impresion*)Ob;
		  ArchivoG << IMPRESION << endl;
		  if( HayCadena(Objeto->GetCadena()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadena()) << endl
						 << Objeto->GetCadena() << endl;
		  else
			 ArchivoG << 0 << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetSiguiente() );
		  return;
		}
		case LECTURA:
		{
		  OE_Lectura *Objeto= (OE_Lectura*)Ob;
		  ArchivoG << LECTURA << endl;
		  if( HayCadena(Objeto->GetCadena()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadena()) << endl
						 << Objeto->GetCadena() << endl;
		  else
			 ArchivoG << 0 << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetSiguiente() );
		  return;
		}
		case ASIGNACION:
		{
		  OE_Asignacion *Objeto= (OE_Asignacion*)Ob;
		  ArchivoG << ASIGNACION << endl;
		  ListaCadenas &Destinos= Objeto->GetCadenasDestino();
		  ListaCadenas &Fuentes= Objeto->GetCadenasFuente();
		  Destinos.Reset();
		  Fuentes.Reset();
		  int NItems= Destinos.GetNItems();
		  ArchivoG << NItems << endl;
		  for( int i= 0; i<NItems; ++i ){
			 char *Aux= Destinos.Itera();
			 ArchivoG << strlen(Aux) << endl << Aux << endl;
			 Aux= Fuentes.Itera();
			 ArchivoG << strlen(Aux) << endl << Aux << endl;
		  }
		  GuardarObjeto( ArchivoG, Objeto->GetSiguiente() );
		  return;
		}
		case DECISION:
		{
		  OE_Decision *Objeto=  (OE_Decision*)Ob;
		  ArchivoG << DECISION << endl;
		  ArchivoG << Objeto->GetSiIzquierda() << endl;
		  if( HayCadena(Objeto->GetCadena()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadena()) << endl
						 << Objeto->GetCadena() << endl;
		  else
			 ArchivoG << 0 << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetSi() );
		  ArchivoG << PUNTO << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetNo() );
		  ArchivoG << PUNTO << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetPunto()->GetSiguiente() );
		  return;
		}
		case CICLOPARA:
		{
		  OE_CicloPara *Objeto=  (OE_CicloPara*)Ob;
		  ArchivoG << CICLOPARA << endl;
		  if( HayCadena(Objeto->GetCadenaId()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadenaId()) << endl
						 << Objeto->GetCadenaId() << endl;
		  else
			 ArchivoG << 0 << endl;
		  if( HayCadena(Objeto->GetCadenaVInicial()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadenaVInicial())
						 << endl << Objeto->GetCadenaVInicial() << endl;
		  else
			 ArchivoG << 0 << endl;
		  if( HayCadena(Objeto->GetCadenaVFinal()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadenaVFinal())
						 << endl << Objeto->GetCadenaVFinal() << endl;
		  else
			 ArchivoG << 0 << endl;
		  if( HayCadena(Objeto->GetCadenaIncremento()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadenaIncremento())
						 << endl << Objeto->GetCadenaIncremento() << endl;
		  else
			 ArchivoG << 0 << endl;

		  GuardarObjeto( ArchivoG, Objeto->GetSiguiente() );
		  ArchivoG << CIERREPARA << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetCierre()->GetSiguiente() );
		  return;
		}
		case CICLOMIENTRAS:
		{
		  OE_CicloMientras *Objeto= (OE_CicloMientras*)Ob;
		  ArchivoG << CICLOMIENTRAS << endl;
		  if( HayCadena(Objeto->GetCadena()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadena()) << endl
						 << Objeto->GetCadena() << endl;
		  else
			 ArchivoG << 0 << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetSiguiente() );
		  ArchivoG << CIERREMIENTRAS << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetCierre()->GetSiguiente() );
		  return;
		}
		case LLAMADA:
		{
		  OE_Llamada *Objeto= (OE_Llamada*)Ob;
		  ArchivoG << LLAMADA << endl;
		  if( HayCadena(Objeto->GetCadenaNombre()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadenaNombre()) << endl
						 << Objeto->GetCadenaNombre() << endl;
		  else
			 ArchivoG << 0 << endl;
		  if( HayCadena(Objeto->GetCadenaArgumentos()) )
			 ArchivoG << 1 << endl << strlen(Objeto->GetCadenaArgumentos())
						 << endl << Objeto->GetCadenaArgumentos() << endl;
		  else
			 ArchivoG << 0 << endl;
		  GuardarObjeto( ArchivoG, Objeto->GetSiguiente() );
		  return;
		}




	 }// end switch
  }//end sub

 char *LeeCadena( char *Cadena, int Largo, ifstream &Archivo ){
	Archivo.get();
	for( int k=0;k<Largo;++k)
	  Cadena[k]=(char)(Archivo.get());
	Cadena[Largo]=0;
	return Cadena;
 }

  ObjetoEjecucion *ListaHojas::CargarObjeto( ifstream &ArchivoA,
															ObjetoEjecucion *PuntoCierre )
  {

/*
 INICIO= 0, FIN, SUBPROGRAMA, REGRESE, IMPRESION, LECTURA,
						ASIGNACION, DECISION, CICLOPARA, CICLOMIENTRAS, CIERREPARA,
						CIERREMIENTRAS, LLAMADA, PUNTO, NINGUNO };

*/

	 int Tipo;
	 ArchivoA >> Tipo;
	 switch( Tipo ){

		case CIERREMIENTRAS:
		case CIERREPARA:
		case PUNTO:
		  return PuntoCierre;

		case INICIO:
		{
		  OE_Inicio *Retorno= new OE_Inicio;
		  InsertarObjeto( Retorno );
		  bool HayDescripcion;
		  ArchivoA >> HayDescripcion;
		  if( HayDescripcion ){
			 int Largo;
			 ArchivoA >> Largo;
			 char *Cad= new char[Largo+1];
			 Retorno->SetDescripcion( LeeCadena(Cad,Largo, ArchivoA ) );
		  }
		  Retorno->SetSiguiente( CargarObjeto( ArchivoA, PuntoCierre ) );
		  return Retorno;
		}
		case FIN:
		{
		  OE_Fin *Retorno= new OE_Fin;
		  InsertarObjeto( Retorno );
		  return Retorno;
		}

		case SUBPROGRAMA:
		{
		  OE_Subprograma *Retorno= new OE_Subprograma;
		  InsertarObjeto( Retorno );
		  bool HayNombre, HayParametros, HayDescripcion;
		  int Largo;
		  char *Cad;
		  ArchivoA >> HayNombre;
		  if( HayNombre ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadenaNombre( LeeCadena(Cad,Largo, ArchivoA ) );
		  }
		  ArchivoA >> HayParametros;
		  if( HayParametros ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadenaParametros( LeeCadena(Cad,Largo, ArchivoA ) );
		  }
		  ArchivoA >> HayDescripcion;
		  if( HayDescripcion ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadenaDescripcion(LeeCadena(Cad,Largo, ArchivoA ));
		  }

		  Retorno->SetSiguiente( CargarObjeto( ArchivoA, PuntoCierre ) );
		  return Retorno;
		}
		case REGRESE:
		{
		  OE_Regrese *Retorno= new OE_Regrese;
		  InsertarObjeto( Retorno );
		  return Retorno;
		}
		case IMPRESION:
		{
		  OE_Impresion *Retorno= new OE_Impresion;
		  InsertarObjeto( Retorno );
		  bool HayCadena;
		  int Largo;
		  char *Cad;
		  ArchivoA >> HayCadena;
		  if( HayCadena ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadena( LeeCadena(Cad,Largo, ArchivoA ) );
		  }
		  Retorno->SetSiguiente( CargarObjeto( ArchivoA, PuntoCierre ) );
		  return Retorno;
		}
		case LECTURA:
		{
		  OE_Lectura *Retorno= new OE_Lectura;
		  InsertarObjeto( Retorno );
		  bool HayCadena;
		  int Largo;
		  char *Cad;
		  ArchivoA >> HayCadena;
		  if( HayCadena ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadena( LeeCadena( Cad,Largo, ArchivoA ) );
		  }
		  Retorno->SetSiguiente( CargarObjeto( ArchivoA, PuntoCierre ) );
		  return Retorno;
		}
		case ASIGNACION:
		{
		  OE_Asignacion *Retorno= new OE_Asignacion;
		  InsertarObjeto( Retorno );
		  ListaCadenas &Destinos= Retorno->GetCadenasDestino();
		  ListaCadenas &Fuentes=  Retorno->GetCadenasFuente();
		  Destinos.Reset();
		  Fuentes.Reset();
		  int NItems;
		  int Largo;
		  char *Cad;
		  ArchivoA >> NItems;
		  for( int i= 0; i<NItems; ++i ){

			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Destinos.Insertar( LeeCadena( Cad,Largo, ArchivoA ) );

			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Fuentes.Insertar( LeeCadena( Cad,Largo, ArchivoA ) );
		  }
		  Retorno->SetSiguiente( CargarObjeto( ArchivoA, PuntoCierre ) );
		  return Retorno;
		}
		case DECISION:
		{
		  OE_Decision *Retorno= new OE_Decision;
		  Retorno->SetPunto( new OE_Punto );
		  InsertarObjeto( Retorno );
		  InsertarObjeto( Retorno->GetPunto() );
		  bool SiIzquierda, HayCadena;
		  ArchivoA >> SiIzquierda;
		  Retorno->SetSiIzquierda( SiIzquierda );
		  ArchivoA >> HayCadena;
		  if( HayCadena ){
			 int Largo;
			 ArchivoA >> Largo;
			 char *Cad= new char[Largo+1];
			 Retorno->SetCadena( LeeCadena( Cad,Largo, ArchivoA ) );
		  }
		  Retorno->SetSi( CargarObjeto( ArchivoA, Retorno->GetPunto() ) );
		  Retorno->SetNo( CargarObjeto( ArchivoA, Retorno->GetPunto() ) );
		  Retorno->GetPunto()->
							SetSiguiente( CargarObjeto( ArchivoA, PuntoCierre ) );
		  return Retorno;
		}
		case CICLOPARA:
		{
		  OE_CicloPara *Retorno=  new OE_CicloPara;
		  Retorno->SetCierre( new OE_CierrePara );
		  Retorno->GetCierre()->SetCiclo( Retorno );
		  InsertarObjeto( Retorno );
		  InsertarObjeto( Retorno->GetCierre() );
		  bool HayId, HayVInicial, HayVFinal, HayIncremento;
		  int Largo;
		  char *Cad;
		  ArchivoA >> HayId;
		  if( HayId ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadenaId( LeeCadena( Cad,Largo, ArchivoA ) );
		  }
		  ArchivoA >> HayVInicial;
		  if( HayVInicial ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadenaVInicial( LeeCadena( Cad,Largo, ArchivoA ) );
		  }
		  ArchivoA >> HayVFinal;
		  if( HayVFinal ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadenaVFinal( LeeCadena( Cad,Largo, ArchivoA ) );
		  }
		  ArchivoA >> HayIncremento;
		  if( HayIncremento ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadenaIncremento( LeeCadena( Cad,Largo, ArchivoA ) );
		  }
		  Retorno->
				  SetSiguiente( CargarObjeto( ArchivoA, Retorno->GetCierre() ) );
		  Retorno->GetCierre()->
				  SetSiguiente( CargarObjeto( ArchivoA, PuntoCierre ) );
		  return Retorno;
		}
		case CICLOMIENTRAS:
		{
		  OE_CicloMientras *Retorno= new OE_CicloMientras;
		  Retorno->SetCierre( new OE_CierreMientras );
		  Retorno->GetCierre()->SetCiclo( Retorno );
		  InsertarObjeto( Retorno );
		  InsertarObjeto( Retorno->GetCierre() );
		  bool HayCadena;
		  int Largo;
		  char *Cad;
		  ArchivoA >> HayCadena;
		  if( HayCadena ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadena( LeeCadena( Cad,Largo, ArchivoA ) );
		  }
		  Retorno->SetSiguiente( CargarObjeto( ArchivoA, Retorno->GetCierre() ) );
		  Retorno->GetCierre()->SetSiguiente( CargarObjeto( ArchivoA, PuntoCierre ) );
		  return Retorno;
		}
		case LLAMADA:
		{
		  OE_Llamada *Retorno= new OE_Llamada;
		  InsertarObjeto( Retorno );
		  bool HayNombre, HayArgumentos;
		  int Largo;
		  char *Cad;
		  ArchivoA >> HayNombre;
		  if( HayNombre ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadenaNombre( LeeCadena( Cad,Largo, ArchivoA ) );
		  }
		  ArchivoA >> HayArgumentos;
		  if( HayArgumentos ){
			 ArchivoA >> Largo;
			 Cad= new char[Largo+1];
			 Retorno->SetCadenaArgumentos( LeeCadena( Cad,Largo, ArchivoA ) );
		  }

		  Retorno->SetSiguiente( CargarObjeto( ArchivoA, PuntoCierre ) );
		  return Retorno;
		}
	 }// end switch
	 return 0;
  }//end sub

  ObjetoEjecucion *GetActualSubprograma(){
	 return ListaHojas.GetActualSubprograma();
  }

  void InsertarObjetoEnListaHojas( ObjetoEjecucion * Ob ){
	 ListaHojas.InsertarObjeto( Ob );
  }


  void SetSeleccionadoEnListaHojas( ObjetoEjecucion *NuevoObjeto ){
	 ListaHojas.SetSeleccionado( NuevoObjeto );
  }

  ObjetoEjecucion* BuscarSubprograma( ObjetoEjecucion*Ob ){
	 return ListaHojas.BuscarSubprograma( Ob );
  }
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;





 void Acomoda( ObjetoEjecucion *Obj, int &AnchoI, int &AnchoD, int &Alto ){

	TipoObjeto t= Obj->GetTipo();
	AnchoI= AnchoD= Alto= 0;

	while( t!=FIN && t!=PUNTO && t!=REGRESE ){
	  if( t==DECISION ){
		 int AnchoIBrazoI, AnchoDBrazoI, AltoBrazoI;
		 int AnchoIBrazoD, AnchoDBrazoD, AltoBrazoD;


			 Acomoda( ((OE_Decision*)Obj)->GetSi(), AnchoIBrazoI, AnchoDBrazoI,
						 AltoBrazoI );
			 Acomoda( ((OE_Decision*)Obj)->GetNo(), AnchoIBrazoD, AnchoDBrazoD,
						 AltoBrazoD );

		 int Delta= AnchoDerecho[DECISION]+AnchoIzquierdo[DECISION]
						+ 2*MinimoAnchoBrazo - ( AnchoDBrazoI + AnchoIBrazoD );

		 if( Delta<2 ){
			Delta= -Delta + 2;
			((OE_Decision*)Obj)->SetBrazoI( MinimoAnchoBrazo+Delta/2 );
			((OE_Decision*)Obj)->SetBrazoD( MinimoAnchoBrazo+(Delta+1)/2 );
		 }
		 else{
			((OE_Decision*)Obj)->SetBrazoI( MinimoAnchoBrazo );
			((OE_Decision*)Obj)->SetBrazoD( MinimoAnchoBrazo );
		 }

		 int Temp= ((OE_Decision*)Obj)->GetBrazoI()+AnchoIzquierdo[DECISION]
					  + AnchoIBrazoI ;
		 if( Temp>AnchoI )
			AnchoI= Temp;

		 Temp= ((OE_Decision*)Obj)->GetBrazoD()+AnchoDerecho[DECISION]
				  + AnchoDBrazoD ;
		 if( Temp>AnchoD )
			AnchoD= Temp;

		 //Aqui va el alto
		 int AltoBrazos= (AltoBrazoI > AltoBrazoD)?  AltoBrazoI : AltoBrazoD ;
		 ((OE_Decision*)Obj)->SetAltoBrazos( AltoBrazos );
		 Alto+= ( AltoArriba[DECISION] + LargoLineaDecision
					 + AltoBrazos + LargoLinea );

		 Obj= ((OE_Decision*)Obj)->GetPunto()->GetSiguiente();
		 t= Obj->GetTipo();

		 continue;
	  }
	  // No es de tipo decision...

	  if( AnchoDerecho[t]>AnchoD )
		 AnchoD= AnchoDerecho[t];
	  if( AnchoIzquierdo[t]>AnchoI )
		 AnchoI= AnchoIzquierdo[t];
	  Alto+=( AltoArriba[t] + AltoAbajo[t] + LargoLinea );
	  Obj= Obj->GetSiguienteParaAcomoda();
	  t= Obj->GetTipo();

	}//while

	Alto+= AltoArriba[t] + AltoAbajo[t];
	return ;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


 void Ubica( ObjetoEjecucion *Obj, int PosIniX, int PosIniY ){

	Obj->SetPosicionX( PosIniX );
	Obj->SetPosicionY( PosIniY );

	TipoObjeto t= Obj->GetTipo();

	while( t!=FIN && t!=PUNTO && t!=REGRESE ){

	  if( t==DECISION ){
		 OE_Decision *Objeto= (OE_Decision*)Obj;
		 ObjetoEjecucion *Si= Objeto->GetSi();
		 ObjetoEjecucion *No= Objeto->GetNo();
		 OE_Punto *Punto= Objeto->GetPunto();
		 ObjetoEjecucion *SiguienteDelPunto= Punto->GetSiguienteParaAcomoda();

		 Ubica( Si, Objeto->GetPosicionX()- AnchoIzquierdo[DECISION]
				  -Objeto->GetBrazoI(), Objeto->GetPosicionY()+
					LargoLineaDecision + AltoArriba[ Si->GetTipo() ] );

		 Ubica( No, Objeto->GetPosicionX()+ AnchoDerecho[DECISION]
				  +Objeto->GetBrazoD(), Objeto->GetPosicionY()+
				  LargoLineaDecision + AltoArriba[ No->GetTipo() ] );

		 Punto->SetPosicionX( Objeto->GetPosicionX() );
		 Punto->SetPosicionY( Objeto->GetPosicionY() + LargoLineaDecision
									 + Objeto->GetAltoBrazos() );


		 SiguienteDelPunto->SetPosicionX( Objeto->GetPosicionX() );
		 SiguienteDelPunto->SetPosicionY( Punto->GetPosicionY() + LargoLinea
										+ AltoArriba[SiguienteDelPunto->GetTipo()] );
		 Obj= SiguienteDelPunto;
		 t= Obj->GetTipo();
		 continue;
	  }

	  //No es de tipo Decision...

	  ObjetoEjecucion *SiguienteDeObj= Obj->GetSiguienteParaAcomoda();

	  SiguienteDeObj->SetPosicionX( Obj->GetPosicionX()  );
	  SiguienteDeObj->SetPosicionY( Obj->GetPosicionY()
					  + AltoAbajo[Obj->GetTipo()] + LargoLinea
					  +AltoArriba[SiguienteDeObj->GetTipo()] );
	  Obj= SiguienteDeObj;
	  t= Obj->GetTipo();
	}//while
 }



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




 void BorrarBloqueObjetos( ObjetoEjecucion * Actual, int PrimeraVez= 1,
									int BorrarDeListaHojas= 1 ){
	TipoObjeto Tipo= Actual->GetTipo();
	if( Tipo==PUNTO || Tipo==CIERREPARA || Tipo==CIERREMIENTRAS )
	  return;
	if( Tipo==DECISION ){
	  BorrarBloqueObjetos( ((OE_Decision*)Actual)->GetSi(), 0,
								  BorrarDeListaHojas );
	  BorrarBloqueObjetos( ((OE_Decision*)Actual)->GetNo(), 0,
								  BorrarDeListaHojas );
	  if( !PrimeraVez )
		 BorrarBloqueObjetos( ((OE_Decision*)Actual)->GetPunto()
								  ->GetSiguienteParaAcomoda(),
								  0, BorrarDeListaHojas );
	  if( BorrarDeListaHojas ){
		 ListaHojas.BorrarObjeto( ((OE_Decision*)Actual)->GetPunto() );
		 ListaHojas.BorrarObjeto( Actual );
	  }
	  else{
		 delete  ((OE_Decision*)Actual)->GetPunto();
		 delete Actual;
	  }
	  return;
	}

	if( Tipo==CICLOPARA ){
	  BorrarBloqueObjetos( Actual->GetSiguienteParaAcomoda(), 0,
	                       BorrarDeListaHojas );
	  if( !PrimeraVez )
		 BorrarBloqueObjetos( ((OE_CicloPara*)Actual)->GetCierre()
								  ->GetSiguienteParaAcomoda(), 0,
								  BorrarDeListaHojas  );
	  if( BorrarDeListaHojas ){
		 ListaHojas.BorrarObjeto( ((OE_CicloPara*)Actual)->GetCierre() );
		 ListaHojas.BorrarObjeto( Actual );
	  }
	  else{
		 delete ((OE_CicloPara*)Actual)->GetCierre();
		 delete Actual;
	  }
	  return;
	}

	if( Tipo==CICLOMIENTRAS ){
	  BorrarBloqueObjetos( Actual->GetSiguienteParaAcomoda(), 0,
	                       BorrarDeListaHojas );
	  if( !PrimeraVez )
		 BorrarBloqueObjetos( ((OE_CicloMientras*)Actual)->GetCierre()
								  ->GetSiguienteParaAcomoda(), 0,
								  BorrarDeListaHojas );
	  if( BorrarDeListaHojas ){
		 ListaHojas.BorrarObjeto( ((OE_CicloMientras*)Actual)->GetCierre());
		 ListaHojas.BorrarObjeto( Actual );
	  }
	  else{
		 delete ((OE_CicloMientras*)Actual)->GetCierre();
		 delete Actual;
	  }
	  return;
	}

	//Objeto simple...

	if ( !PrimeraVez )
	  BorrarBloqueObjetos( Actual->GetSiguienteParaAcomoda(), 0,
	                       BorrarDeListaHojas );
	if( BorrarDeListaHojas )
	  ListaHojas.BorrarObjeto( Actual );
	else
	  delete Actual;
	return;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 ObjetoEjecucion *Copia( ObjetoEjecucion *Actual, int PrimeraVez,
								 ObjetoEjecucion *PuntoCierre=0 ){

	TipoObjeto Tipo= Actual->GetTipo();
	if( Tipo== PUNTO || Tipo== CIERREPARA || Tipo==CIERREMIENTRAS )
	  return PuntoCierre;

	if(Tipo==DECISION){


	  OE_Decision *Retorno= new OE_Decision((OE_Decision*)Actual);
	  Retorno->SetPunto( new OE_Punto );
	  Retorno->SetSi( Copia( ((OE_Decision*)Actual)->GetSi(),
									 0, Retorno->GetPunto() ) );
	  Retorno->SetNo( Copia( ((OE_Decision*)Actual)->GetNo(),
									 0, Retorno->GetPunto() ) );
	  if( !PrimeraVez )
		 Retorno->GetPunto()->SetSiguienteParaClickInsertar(
		 Copia( ((OE_Decision*)Actual)->GetPunto()->
				  GetSiguienteParaAcomoda(),0, PuntoCierre ) );
	  return Retorno;
	}
	if( Tipo== CICLOPARA ){
	  OE_CicloPara *Retorno= new OE_CicloPara( (OE_CicloPara*)Actual );
	  Retorno->SetCierre( new OE_CierrePara );
	  Retorno->GetCierre()->SetCiclo( Retorno );
	  Retorno->SetSiguiente( Copia( ((OE_CicloPara*)Actual)->
											  GetSiguiente(),0,
											  Retorno->GetCierre() ) );
	  if( !PrimeraVez )
		 Retorno->GetCierre()->SetSiguienteParaClickInsertar(
						 Copia( ((OE_CicloPara*)Actual)->GetCierre()->
						 GetSiguienteParaAcomoda(), 0, PuntoCierre ) );
	  return Retorno;

	}
	if( Tipo== CICLOMIENTRAS ){
	  OE_CicloMientras *Retorno=
						  new OE_CicloMientras( (OE_CicloMientras*) Actual );
	  Retorno->SetCierre( new OE_CierreMientras );
	  Retorno->GetCierre()->SetCiclo( Retorno );
	  Retorno->SetSiguiente( Copia( ((OE_CicloMientras*)Actual)->
											  GetSiguiente(),0,
											  Retorno->GetCierre() ) );
	  if( !PrimeraVez )
		 Retorno->GetCierre()->SetSiguienteParaClickInsertar(
				 Copia( ((OE_CicloMientras*)Actual)->
				 GetCierre()->GetSiguienteParaAcomoda(),0, PuntoCierre ) );
	  return Retorno;

	}

	ObjetoEjecucion *Retorno;

	switch( Tipo ){

	  case ASIGNACION:
		 Retorno= new OE_Asignacion( (OE_Asignacion*)Actual );
		 break;
	  case IMPRESION:
		 Retorno= new OE_Impresion( (OE_Impresion*)Actual );
		 break;
	  case LECTURA:
		 Retorno= new OE_Lectura( (OE_Lectura*)Actual );
		 break;
	  case LLAMADA:
		 Retorno= new OE_Llamada( (OE_Llamada*)Actual );
		 break;
	};
	if( !PrimeraVez )
	  Retorno->SetSiguienteParaClickInsertar(
	  Copia( Actual->GetSiguienteParaAcomoda(),0, PuntoCierre ) );
	return Retorno;

 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


 void InsertarEnListaHojas( ObjetoEjecucion *Actual, int PrimeraVez= 1 ){
	TipoObjeto Tipo= Actual->GetTipo();
	if( Tipo== PUNTO || Tipo== CIERREPARA || Tipo== CIERREMIENTRAS )
	  return;

	if( Tipo== DECISION ){
	  ListaHojas.InsertarObjeto( Actual );
	  ListaHojas.InsertarObjeto( ((OE_Decision*)Actual)->GetPunto() );
	  InsertarEnListaHojas( ((OE_Decision*)Actual)->GetSi(), 0 );
	  InsertarEnListaHojas( ((OE_Decision*)Actual)->GetNo(), 0 );
	  if( !PrimeraVez )
		InsertarEnListaHojas( ((OE_Decision*)Actual)->GetPunto()->
									 GetSiguienteParaAcomoda(), 0 );
	  return;
	}
	if( Tipo==CICLOPARA ){
	  ListaHojas.InsertarObjeto( Actual );
	  ListaHojas.InsertarObjeto( ((OE_CicloPara*)Actual)->GetCierre() );
	  InsertarEnListaHojas( Actual->GetSiguienteParaAcomoda(), 0 );
	  if( !PrimeraVez )
		 InsertarEnListaHojas( ((OE_CicloPara*)Actual)->GetCierre()->
									  GetSiguienteParaAcomoda(), 0 );
	  return;
	}
	if( Tipo==CICLOMIENTRAS ){
	  ListaHojas.InsertarObjeto( Actual );
	  ListaHojas.InsertarObjeto( ((OE_CicloMientras*)Actual)->GetCierre() );
	  InsertarEnListaHojas( Actual->GetSiguienteParaAcomoda(), 0 );
	  if( !PrimeraVez )
		 InsertarEnListaHojas( ((OE_CicloMientras*)Actual)->GetCierre()->
									 GetSiguienteParaAcomoda(), 0 );
	  return;
	}

	//Objeto simple...
	ListaHojas.InsertarObjeto( Actual );
	if( !PrimeraVez )
	  InsertarEnListaHojas( Actual->GetSiguienteParaAcomoda(), 0 );
	return;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


 ObjetoEjecucion *HastaDonde( ObjetoEjecucion *Ob ){
	TipoObjeto Tipo= Ob->GetTipo();
	if( Tipo== DECISION )
	  return ((OE_Decision*)Ob)->GetPunto();
	if( Tipo== CICLOPARA )
	  return ((OE_CicloPara*)Ob)->GetCierre();
	if( Tipo==CICLOMIENTRAS )
	  return ((OE_CicloMientras*)Ob)->GetCierre();
	return Ob;
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 bool EstaEnPantalla( ObjetoEjecucion * Ob ){
	TRect Region= VentanaPrincipal->GetClientRect() ;
	int x0= ListaHojas.GetActualX();
	int y0= ListaHojas.GetActualY();
	int x1= x0+Region.right/Estado.DistanciaPuntos;
	int y1= y0+Region.bottom/Estado.DistanciaPuntos;

	return( Ob->GetPosicionX()>x0 && Ob->GetPosicionX()<x1  &&
			  Ob->GetPosicionY()>y0 && Ob->GetPosicionY()<y1     );
 }


 #endif
