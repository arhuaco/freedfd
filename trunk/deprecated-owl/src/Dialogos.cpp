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

 #ifndef __DIALOGOS_CPP
 #define __DIALOGOS_CPP



 #include <owl/radiobut.h>
// #include <owl/groupbox.h>


  class DialogoLectura:public TDialog{

		OE_Lectura *Objeto;
	 public:

		DialogoLectura( OE_Lectura *Ob ):
				 TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOLECTURA ),
				 Objeto(Ob){
			 //SetBkgndColor(TColor::LtGray);

				 }
   	bool  CanClose();
		void  SetupWindow();

  };

  void DialogoLectura::SetupWindow(){
	 if( Objeto->GetCadena() )
		SetDlgItemText( IDC_EDIT1, Objeto->GetCadena() );
  }


  bool DialogoLectura::CanClose(){
	 char CadenaAuxiliar[2048];
	 GetDlgItemText( IDC_EDIT1, CadenaAuxiliar, 2047 );
	 char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadena( NuevaCadena );
	 Estado.SeModifico= true;
	 return true;
  }





  void EditarLectura( OE_Lectura *Ob ){
	 DialogoLectura( Ob ).Execute();
  }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




  class DialogoSalida:public TDialog{

		OE_Impresion *Objeto;
	 public:

		DialogoSalida( OE_Impresion *Ob ):
				 TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOSALIDA ),
				 Objeto(Ob){
          //SetBkgndColor(TColor::LtGray);

				 }

		bool  CanClose();
		void  SetupWindow();


  };


  void DialogoSalida::SetupWindow(){
	 if( Objeto->GetCadena() )
		SetDlgItemText( IDC_EDIT1, Objeto->GetCadena() );
  }


  bool DialogoSalida::CanClose(){
	 char CadenaAuxiliar[2048];
	 GetDlgItemText( IDC_EDIT1, CadenaAuxiliar, 2047 );
	 char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadena( NuevaCadena );
	 Estado.SeModifico= true;
	 return true;
  }





  void EditarImpresion( OE_Impresion *Ob ){
	 DialogoSalida( Ob ).Execute();
  }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


  class DialogoCicloMientras:public TDialog{

		OE_CicloMientras *Objeto;
	 public:

		DialogoCicloMientras( OE_CicloMientras *Ob ):
				 TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOCICLOMIENTRAS ),
				 Objeto(Ob){
         //SetBkgndColor(TColor::LtGray);

				 }

		bool  CanClose();
		void  SetupWindow();


  };


  void DialogoCicloMientras::SetupWindow(){
	 if( Objeto->GetCadena() )
		SetDlgItemText( IDC_EDIT1, Objeto->GetCadena() );
  }


  bool DialogoCicloMientras::CanClose(){
	 char CadenaAuxiliar[2048];
	 GetDlgItemText( IDC_EDIT1, CadenaAuxiliar, 2047 );
	 char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadena( NuevaCadena );
	 Estado.SeModifico= true;
	 return true;
  }





  void EditarCicloMientras( OE_CicloMientras *Ob ){
	 DialogoCicloMientras( Ob ).Execute();
  }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




  class DialogoInicio:public TDialog{

		OE_Inicio *Objeto;
	 public:
		DialogoInicio( OE_Inicio *Ob );

		bool  CanClose();
		void  SetupWindow();


  };

  DialogoInicio::DialogoInicio( OE_Inicio *Ob ): TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOINICIO ),Objeto(Ob){
	 //SetBkgndColor(TColor::LtGray);

  }

  void DialogoInicio::SetupWindow(){
	 if( Objeto->GetDescripcion() )
		SetDlgItemText( IDC_EDIT1, Objeto->GetDescripcion() );
  }


  bool DialogoInicio::CanClose(){
	 char CadenaAuxiliar[2048];
	 GetDlgItemText( IDC_EDIT1, CadenaAuxiliar, 2047 );
	 char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetDescripcion( NuevaCadena );
	 Estado.SeModifico= true;
	 return true;
  }





  void EditarInicio( OE_Inicio *Ob ){
	 DialogoInicio( Ob ).Execute();
  }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  class DialogoDecision:public TDialog{

		OE_Decision *Objeto;
		TRadioButton *Derecha;
		TRadioButton *Izquierda;

	 public:

		DialogoDecision( OE_Decision *Ob );
		~DialogoDecision(){
		  delete Derecha;
		  delete Izquierda;
		}
		bool  CanClose();
		void  SetupWindow();


  };

  DialogoDecision::DialogoDecision( OE_Decision *Ob ):
				 TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGODECISION ),
				 Objeto(Ob){

	 //SetBkgndColor(TColor::LtGray);
	 Derecha= new TRadioButton( this, IDC_DERECHA );
	 Izquierda= new TRadioButton( this, IDC_IZQUIERDA );
  }

  void DialogoDecision::SetupWindow(){
	 if( Objeto->GetSiIzquierda() )
		CheckRadioButton( IDC_DERECHA, IDC_IZQUIERDA, IDC_IZQUIERDA  );
	 else
		CheckRadioButton( IDC_DERECHA, IDC_IZQUIERDA , IDC_DERECHA );
	 if( Objeto->GetCadena() )
		SetDlgItemText( IDC_EDIT1, Objeto->GetCadena());

  }


  bool DialogoDecision::CanClose(){
	 char CadenaAuxiliar[2048];
	 GetDlgItemText( IDC_EDIT1, CadenaAuxiliar, 2047 );
	 char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadena( NuevaCadena );
	 if( IsDlgButtonChecked( IDC_IZQUIERDA )==1 )
		Objeto->SetSiIzquierda( 1 );
	 else
		Objeto->SetSiIzquierda( 0 );
	 Estado.SeModifico= true;
	 return true;
  }



  void EditarDecision( OE_Decision *Ob ){
	 DialogoDecision( Ob ).Execute();
  }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




  class DialogoLlamada:public TDialog{

		OE_Llamada *Objeto;
	 public:

		DialogoLlamada( OE_Llamada *Ob ):
				 TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOLLAMADA ),
				 Objeto(Ob){
        	 //SetBkgndColor(TColor::LtGray);

				 }

		bool  CanClose();
		void  SetupWindow();


  };


  void DialogoLlamada::SetupWindow(){

	 if( Objeto->GetCadenaNombre() )
		SetDlgItemText( IDC_EDIT1, Objeto->GetCadenaNombre() );

	 if( Objeto->GetCadenaArgumentos() )
		SetDlgItemText( IDC_EDIT2, Objeto->GetCadenaArgumentos() );

  }


  bool DialogoLlamada::CanClose(){
	 char CadenaAuxiliar[2048];

	 GetDlgItemText( IDC_EDIT1, CadenaAuxiliar, 2047 );
	 char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadenaNombre( NuevaCadena );

	 GetDlgItemText( IDC_EDIT2, CadenaAuxiliar, 2047 );
	 NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadenaArgumentos( NuevaCadena );
	 Estado.SeModifico= true;
	 return true;
  }


  void EditarLlamada( OE_Llamada *Ob ){
	 DialogoLlamada( Ob ).Execute();
  }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




  class DialogoSubprograma:public TDialog{

		OE_Subprograma *Objeto;
	 public:

		DialogoSubprograma( OE_Subprograma *Ob ):
				 TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOSUBPROGRAMA ),
				 Objeto(Ob){
		  //SetBkgndColor(TColor::LtGray);

				 }

		bool  CanClose();
		void  SetupWindow();


  };


  void DialogoSubprograma::SetupWindow(){

	 if( Objeto->GetCadenaNombre() )
		SetDlgItemText( IDC_EDIT1, Objeto->GetCadenaNombre() );

	 if( Objeto->GetCadenaParametros() )
		SetDlgItemText( IDC_EDIT2, Objeto->GetCadenaParametros() );

	 if( Objeto->GetCadenaDescripcion() )
		SetDlgItemText( IDC_EDIT3, Objeto->GetCadenaDescripcion() );


  }


  bool DialogoSubprograma::CanClose(){
	 char CadenaAuxiliar[2048];

	 GetDlgItemText( IDC_EDIT1, CadenaAuxiliar, 2047 );
	 char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadenaNombre( NuevaCadena );

	 GetDlgItemText( IDC_EDIT2, CadenaAuxiliar, 2047 );
	 NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadenaParametros( NuevaCadena );

	 GetDlgItemText( IDC_EDIT3, CadenaAuxiliar, 2047 );
	 NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadenaDescripcion( NuevaCadena );

	 Estado.SeModifico= true;
	 return true;

  }


  void EditarSubprograma( OE_Subprograma *Ob ){
	 DialogoSubprograma( Ob ).Execute();
  }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




  class DialogoCicloPara:public TDialog{

		OE_CicloPara *Objeto;
	 public:

		DialogoCicloPara( OE_CicloPara *Ob ):
				 TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOCICLOPARA ),
				 Objeto(Ob){
        //SetBkgndColor(TColor::LtGray);
				 }

		bool  CanClose();
		void  SetupWindow();


  };


  void DialogoCicloPara::SetupWindow(){

	 if( Objeto->GetCadenaId() )
		SetDlgItemText( IDC_EDIT1, Objeto->GetCadenaId() );

	 if( Objeto->GetCadenaVInicial() )
		SetDlgItemText( IDC_EDIT2, Objeto->GetCadenaVInicial() );

	 if( Objeto->GetCadenaVFinal() )
		SetDlgItemText( IDC_EDIT3, Objeto->GetCadenaVFinal() );

	 if( Objeto->GetCadenaIncremento() )
		SetDlgItemText( IDC_EDIT4, Objeto->GetCadenaIncremento() );
	 else
		SetDlgItemText( IDC_EDIT4, "1" );
  }


  bool DialogoCicloPara::CanClose(){
	 char CadenaAuxiliar[2048];

	 GetDlgItemText( IDC_EDIT1, CadenaAuxiliar, 2047 );
	 char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadenaId( NuevaCadena );

	 GetDlgItemText( IDC_EDIT2, CadenaAuxiliar, 2047 );
	 NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadenaVInicial( NuevaCadena );

	 GetDlgItemText( IDC_EDIT3, CadenaAuxiliar, 2047 );
	 NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadenaVFinal( NuevaCadena );

	 GetDlgItemText( IDC_EDIT4, CadenaAuxiliar, 2047 );
	 NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 Objeto->SetCadenaIncremento( NuevaCadena );

	 Estado.SeModifico= true;
	 return true;

  }


  void EditarCicloPara( OE_CicloPara *Ob ){
	 DialogoCicloPara( Ob ).Execute();
  }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




  class DialogoAsignacion:public TDialog{

		OE_Asignacion *Objeto;
	 public:

		DialogoAsignacion( OE_Asignacion *Ob ):
				 TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOASIGNACION ),
				 Objeto(Ob){
        //SetBkgndColor(TColor::LtGray);
				 }

		bool  CanClose();
		void  SetupWindow();


  };


  void DialogoAsignacion::SetupWindow(){

	 ListaCadenas &Destinos= Objeto->GetCadenasDestino();
	 ListaCadenas &Fuentes = Objeto->GetCadenasFuente();
	 Destinos.Reset();
	 Fuentes.Reset();

	 int NItems= Fuentes.GetNItems();

	 if( NItems>0 ){
		SetDlgItemText( IDC_EDIT1, Destinos.Itera() );
		SetDlgItemText( IDC_EDIT2, Fuentes.Itera() );
	 }
	 if( NItems>1 ){
		SetDlgItemText( IDC_EDIT3, Destinos.Itera() );
		SetDlgItemText( IDC_EDIT4, Fuentes.Itera() );
	 }
	 if( NItems>2 ){
		SetDlgItemText( IDC_EDIT5, Destinos.Itera() );
		SetDlgItemText( IDC_EDIT6, Fuentes.Itera() );
	 }
  }



  bool DialogoAsignacion::CanClose(){
	 char CadenaAuxiliar[2048];
	 char CadenaAuxiliar2[2048];

	 ListaCadenas &Destinos= Objeto->GetCadenasDestino();
	 ListaCadenas &Fuentes = Objeto->GetCadenasFuente();

	 Destinos.Vacear();
	 Fuentes.Vacear();

	 GetDlgItemText( IDC_EDIT1, CadenaAuxiliar, 2047 );
	 GetDlgItemText( IDC_EDIT2, CadenaAuxiliar2, 2047 );
	 if( CadenaAuxiliar[0] || CadenaAuxiliar2[0] ){
		char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
		strcpy( NuevaCadena, CadenaAuxiliar );
		Destinos.Insertar( NuevaCadena );

		NuevaCadena= new char[strlen(CadenaAuxiliar2)+1];
		strcpy( NuevaCadena, CadenaAuxiliar2 );
		Fuentes.Insertar( NuevaCadena );
	 }

	 GetDlgItemText( IDC_EDIT3, CadenaAuxiliar, 2047 );
	 GetDlgItemText( IDC_EDIT4, CadenaAuxiliar2, 2047 );
	 if( CadenaAuxiliar[0] || CadenaAuxiliar2[0] ){
		char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
		strcpy( NuevaCadena, CadenaAuxiliar );
		Destinos.Insertar( NuevaCadena );

		NuevaCadena= new char[strlen(CadenaAuxiliar2)+1];
		strcpy( NuevaCadena, CadenaAuxiliar2 );
		Fuentes.Insertar( NuevaCadena );
	 }

	 GetDlgItemText( IDC_EDIT5, CadenaAuxiliar, 2047 );
	 GetDlgItemText( IDC_EDIT6, CadenaAuxiliar2, 2047 );
	 if( CadenaAuxiliar[0] || CadenaAuxiliar2[0] ){
		char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
		strcpy( NuevaCadena, CadenaAuxiliar );
		Destinos.Insertar( NuevaCadena );

		NuevaCadena= new char[strlen(CadenaAuxiliar2)+1];
		strcpy( NuevaCadena, CadenaAuxiliar2 );
		Fuentes.Insertar( NuevaCadena );
	 }

	 Estado.SeModifico= true;
	 return true;
  }



  void EditarAsignacion( OE_Asignacion *Ob ){
	 DialogoAsignacion( Ob ).Execute();
  }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  // No dejar la cadena cuando se hace pause
  class DialogoEntradaTeclado:public TDialog{
		char *Cadena;
		bool Detenido;
	 public:
		DialogoEntradaTeclado():
				 TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOENTRADATECLADO ){
					Cadena=0;
					Detenido= false;
               //SetBkgndColor(TColor::LtGray);
				 }

		bool  CanClose();
		void  SetupWindow();
		char *GetCadena(){ return Cadena; }
		bool GetDetenido(){ return Detenido; }
		void Detener();
		DECLARE_RESPONSE_TABLE(DialogoEntradaTeclado);
  };

  DEFINE_RESPONSE_TABLE1(DialogoEntradaTeclado, TDialog)
	 EV_COMMAND( IDDETENER, Detener ),
  END_RESPONSE_TABLE;

  void DialogoEntradaTeclado::Detener(){
	 Detenido= true;
	 CmCancel();
  }

  void DialogoEntradaTeclado::SetupWindow(){
		if(Cadena)
		  SetDlgItemText( IDC_EDIT1, Cadena );
		else
		  SetDlgItemText( IDC_EDIT1, "" );
  }


  bool DialogoEntradaTeclado::CanClose(){
	 char CadenaAuxiliar[2048];
	 GetDlgItemText( IDC_EDIT1, CadenaAuxiliar, 2047 );
	 char *NuevaCadena= new char[strlen(CadenaAuxiliar)+1];
	 strcpy( NuevaCadena, CadenaAuxiliar );
	 if(Cadena)
		delete []Cadena;
	 Cadena =NuevaCadena ;
	 return true;
  }


 Token *EntradaTeclado(){
	DialogoEntradaTeclado Dialogo;
	char *Entrada;
	while(1){
	  Dialogo.Execute();
	  Entrada= Dialogo.GetCadena();
	  if( Dialogo.GetDetenido() ){
		 delete[] Entrada;
		 return 0;
	  }


	  if( !Entrada ){
		 VentanaPrincipal->MessageBox(
		 "Debe ingresar un valor constante.", NOMBREPROGRAMA,MB_ICONEXCLAMATION|MB_OK );
		 continue;
	  }

	  Token *t= GetPostfijo( Entrada );

	  if( !t || Buzon.GetHuboError() ){
		 VentanaPrincipal->MessageBox(
		 "Debe ingresar un valor constante.",NOMBREPROGRAMA,MB_ICONEXCLAMATION|MB_OK);
		 Buzon.Vacear();
		 continue;
	  }

	  if( t->GetTipoAlmacenamiento()!=CONSTANTE ){
		 //Solo se leen expresiones constantes de teclado.
		 //leer nuevamente
		 delete t;
		 VentanaPrincipal->MessageBox("Debe ingresar un valor constante.",NOMBREPROGRAMA,MB_ICONEXCLAMATION|MB_OK);
		 continue;
	  }

	  Token *Aux= t->GetSig() ;
	  if( Aux ){
		 if( (Aux->GetTipoToken()==OPERANDO) ||
			  ( (Aux->GetDatoStr()[0]!='-')  && (Aux->GetDatoStr()[0]!='+') ) )
		 {
			VentanaPrincipal->MessageBox("Debe ingresar un valor constante.",NOMBREPROGRAMA,MB_ICONEXCLAMATION|MB_OK);
			LiberarListaToken( t );
			continue;
		 }
		 Aux= Aux->GetSig();
		 if( Aux ){
			VentanaPrincipal->MessageBox("Debe ingresar un valor constante.",NOMBREPROGRAMA,MB_ICONEXCLAMATION|MB_OK);
			LiberarListaToken( t );
			continue;
		 }
	  }

	  // Por si acaso...
	  Token *Aux2= t;
	  t= EvaluaPostfijo( t );
	  LiberarListaToken( Aux2 );
	  if( Buzon.GetHuboError() ){
		 VentanaPrincipal->MessageBox("Error de evaluación",NOMBREPROGRAMA);
		 Buzon.Vacear();
		 continue;
	  }


	  delete[] Entrada;
	  return t;

	}
 }
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


  class DialogoSalidaPorPantalla:public TDialog{

	 char *Cadena;
	 bool Detenido;

	 public:
		DialogoSalidaPorPantalla( char *Cad ):
				 TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOSALIDAPORPANTALLA){

		  Detenido= false;
		  Cadena= new char[strlen(Cad)+1];
		  strcpy( Cadena, Cad );
        //SetBkgndColor(TColor::LtGray);
		}
		bool GetDetenido(){ return Detenido; }
		void Detener();
		DECLARE_RESPONSE_TABLE(DialogoSalidaPorPantalla);

	  //	bool  CanClose();
		void  SetupWindow();


  };

  DEFINE_RESPONSE_TABLE1(DialogoSalidaPorPantalla, TDialog)
	 EV_COMMAND( IDDETENER, Detener ),
  END_RESPONSE_TABLE;


  void DialogoSalidaPorPantalla::SetupWindow(){

	 SetDlgItemText( IDC_EDIT0, Cadena );

  }

  void DialogoSalidaPorPantalla::Detener(){
	 Detenido= true;
	 CmCancel();
  }

  bool Imprime( char *Cadena ){
	 DialogoSalidaPorPantalla Dialogo( Cadena );
	 Dialogo.Execute();
	 return Dialogo.GetDetenido();
  }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


  class DialogoPreguntaDecision:public TDialog{
	 public:
		int Estado;
		DialogoPreguntaDecision():TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOPREGUNTADECISION){
		  Estado=0;
        //SetBkgndColor(TColor::LtGray);
		}
		void CmIzquierda(){
		  Estado=1;
		  CmCancel();
		}
		void CmDerecha(){
		  Estado=2;
		  CmCancel();
		}
		DECLARE_RESPONSE_TABLE(DialogoPreguntaDecision);
  };

  DEFINE_RESPONSE_TABLE1(DialogoPreguntaDecision, TDialog)
  EV_COMMAND( IDC_DERECHA,CmDerecha ),
  EV_COMMAND( IDC_IZQUIERDA,CmIzquierda),
  END_RESPONSE_TABLE;

 int PreguntaDecision(){
  DialogoPreguntaDecision Dialogo;
  Dialogo.Execute();
  return Dialogo.Estado;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  class DialogoEvaluador:public TDialog{
	 public:
		DialogoEvaluador():TWindow( VentanaPrincipal ),
				 TDialog( VentanaPrincipal, DIALOGOEVALUADOR ){
		  //SetBkgndColor(TColor::LtGray);
		}
		void Evaluar();

		DECLARE_RESPONSE_TABLE(DialogoEvaluador);
  } ;

  DEFINE_RESPONSE_TABLE1(DialogoEvaluador, TDialog)
	 EV_COMMAND( IDEVALUAR, Evaluar ),
  END_RESPONSE_TABLE;
  //DialogoDepurador *Depurador;

  void DialogoEvaluador::Evaluar(){
	 char CadenaExpresion[2048];
	 GetDlgItemText( IDC_EDIT0, CadenaExpresion, 2047 );
	 Token *Pos= GetPostfijo( CadenaExpresion );
	 if( Buzon.GetHuboError() ){
		char *Error= Buzon.GetError();
		SetDlgItemText( IDC_EDIT1, Error );
		delete[] Error;
		return;
	 }
	 if( !Pos ){
		SetDlgItemText( IDC_EDIT1, "" );
		return;
	 }
	 Token *Res= EvaluaPostfijo( Pos );
	 LiberarListaToken( Pos );
	 if( Buzon.GetHuboError() ){
		char *Error= Buzon.GetError();
		SetDlgItemText( IDC_EDIT1, Error );
		delete[] Error;
		return;
	 }
	 char *CadenaRespuesta= new char[
						20+strlen(RetornaValorDeTokenEnCadena( Res ))];
	 if( Res->GetTipoDato()==LOGICO )
		strcpy( CadenaRespuesta, "(Lógico): " );
	 else if( Res->GetTipoDato()==REAL )
		strcpy( CadenaRespuesta, "(Real): " );
	 else
		strcpy( CadenaRespuesta, "(Cadena): '" );

	 strcat( CadenaRespuesta, RetornaValorDeTokenEnCadena( Res ) );
	 if( Res->GetTipoDato()==STRING )
		strcat( CadenaRespuesta, "'" );
	 delete Res;

	 SetDlgItemText( IDC_EDIT1, CadenaRespuesta );
	 delete[] CadenaRespuesta;


  }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


  TWindow *GetDepurador();

  class DialogoExpresion:public TDialog{

		char *IniCad;
	 public:

		char *Cadena;


		DialogoExpresion( char *IniC ):TWindow( GetDepurador() ),
				 TDialog( GetDepurador(), DIALOGOEXPRESION ){
		  Cadena= 0;
		  IniCad= IniC;
        //SetBkgndColor(TColor::LtGray);
		}
		void SetupWindow();
		bool CanClose();

  } ;


  void DialogoExpresion::SetupWindow(){
	 if( IniCad )
		SetDlgItemText( IDC_EDIT0, IniCad );
  }


  bool DialogoExpresion::CanClose(){
	  char Temp[2048];
	  GetDlgItemText( IDC_EDIT0, Temp, 2047 );
	  if( Cadena ){
		 delete[] Cadena;
		 Cadena= 0;
	  }
	  Cadena= new char[strlen(Temp)+1];
	  strcpy( Cadena, Temp );
	  return true;
  }


  char *LeerExpresion( char *IniCad= 0 ){
	 DialogoExpresion Dialogo( IniCad );
	 Dialogo.Execute();
	 return Dialogo.Cadena;
  }


#endif
