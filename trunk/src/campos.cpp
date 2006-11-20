#include <dfd.h>
#include <campos.h> 
#include <errores.h>
#include <dfd-wrappers.h>

#include <string.h> // strcasecmp

 // TODO:
 // Estos contadores y datos similares deben ser únicos para
 // cada hilo de ejecución.
 // ¡¡¡Quitar der acá!!! Manejar el concepto de "entorno de ejecución".

 int ContadorToken = 0;
 int ContadorCajita = 0;
 int ContadorCampoVariable = 0;
 int ContadorTabla = 0;
 int ContadorVariable = 0;
 BuzonDeErrores Buzon;


 Token::~Token()
 {
   --ContadorToken;

   // TODO: ¿Qué es 'STRING'? ¡Eso es confuso! ¡Arreglar!
   // TODO: Ya no me parece tan confuso. Tal vez aclarar que es un tipo de dato.
     
   if (TipoD == STRING && DatoStr) delete[] DatoStr;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Token::Token ( const Token &UnToken ){

	// Depuración, para buscar tokens reservados pero no liberados.
	// Al terminar una evaluación, el número de tokens debe ser conocido.
	++ContadorToken;  

	TipoD= UnToken.TipoD;
	TipoT= UnToken.TipoT;
	Alcance= UnToken.Alcance;
	Almacenamiento= UnToken.Almacenamiento;
	Pdp= UnToken.Pdp;
	Pfp= UnToken.Pfp;

	if(TipoD == STRING)
	  DatoStr = dfd_strdup(UnToken.DatoStr);
	else
	  DatoReal= UnToken.DatoReal;
 } ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; // TODO: ¿Ubicar en otro lado?  void Token::setdato ( char *valorstr ){ if (datostr) delete[] datostr; datostr = dfd_strdup(valorstr); } ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; Token::Token ( bool ValorLogico, TipoToken UnTipoToken, TipoAlmacenamiento UnTipoAlmacenamiento ){ ++ContadorToken; //Depuraci›n DatoLogico= ValorLogico; TipoT= UnTipoToken; Almacenamiento= UnTipoAlmacenamiento; TipoD= LOGICO; }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Token::Token ( long double ValorReal, TipoToken UnTipoToken,
				 TipoAlmacenamiento UnTipoAlmacenamiento ){
	++ContadorToken; //Depuraci›n
	DatoReal= ValorReal;
	TipoT= UnTipoToken;
	Almacenamiento= UnTipoAlmacenamiento;
	TipoD= REAL;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Token::Token ( char *ValorStr, TipoToken UnTipoToken,
				 TipoAlmacenamiento UnTipoAlmacenamiento, int UnPdp, int UnPfp,
				 AlcanceOperador UnAlcance){

	++ContadorToken;   //Depuraci›n
	TipoT= UnTipoToken;
	Almacenamiento= UnTipoAlmacenamiento;
	TipoD= STRING;
	Alcance= UnAlcance;
	Pdp= UnPdp;
	Pfp= UnPfp;
	DatoStr= dfd_strdup(ValorStr);
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  class CampoVariable{
		 TipoDato Tipo;
		 int Dimension;
		 struct Cajita{
			Cajita(){
			  ContadorCajita++;
			}
			~Cajita(){
			  ContadorCajita--;
			}
			unsigned int *Indices;
			union{
			  char *DatoStr;
			  long double DatoReal;
			  bool DatoLogico;
			};
			Cajita *Sig;
		 } *Inicio;

	 public:
		 CampoVariable( Token *UnToken, unsigned *Indices,
							int Dim, char *Id );
		 Token *Leer( unsigned *Indices, int Dim, char *Id );
		 void AsignarValor( Token *UnToken, unsigned *Indices,
								  int Dim, char *Id );
		 ~CampoVariable();
		 TipoDato GetTipo(){ return Tipo; }

  };


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void CampoVariable::AsignarValor( Token *UnToken, unsigned *Indices,
											 int Dim, char *Id ){

	int ResultadoInsercion= 1;
	if(UnToken->GetTipoDato() != Tipo){
	  Buzon.SetIdentificadorAsociado( Id );
	  Buzon.Error( LOS_TIPOS_NO_COINCIDEN );
	  ResultadoInsercion= 0;
	}
	if( Dim != Dimension ){
	  if( !Dimension ){
		 Buzon.SetIdentificadorAsociado( Id );
		 Buzon.Error( ID_NO_ES_ARREGLO );
	  }
	  else{
		 Buzon.SetIdentificadorAsociado( Id );
		 Buzon.Error( DIMENSION_NO_VALIDA );
	  }
	  ResultadoInsercion= 0;
	}
	if( !ResultadoInsercion )
	  return ;

	// No se encontro errores de tipo ni de dimension

	Cajita *AsignarEn= Inicio;

	for( ; AsignarEn ; AsignarEn=AsignarEn->Sig ){
	  bool EsEste= true;
	  for( int i=0 ; i<Dimension ; ++i )
		 if( Indices[i] != AsignarEn->Indices[i] ){
			EsEste= false;
			break;
		 }
	  if( EsEste )
		 break;
	}
	//No se encontro la posicion
	if( !AsignarEn ){
	  AsignarEn= new Cajita;
	  if( Dimension ){
		 AsignarEn->Indices= new unsigned int [Dimension];
	  }
	  AsignarEn->Sig= Inicio;
	  Inicio= AsignarEn;
	  if( Tipo == STRING )
          	 AsignarEn->DatoStr = dfd_strdup(UnToken->GetDatoStr());
	  else
		 AsignarEn->DatoReal= UnToken->GetDatoReal();
	  for(int i= 0 ; i<Dimension ; ++i)
		 AsignarEn->Indices[i]= Indices[i];
	  return ;
	}
	//	Se va a asignar en una posicion ya existente
	if( Tipo == STRING ){
	  if( AsignarEn->DatoStr )
		 delete[] AsignarEn->DatoStr;
	  AsignarEn->DatoStr = dfd_strdup(UnToken->GetDatoStr());
	}
	else
	  AsignarEn->DatoReal= UnToken->GetDatoReal();

	return ;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 CampoVariable::CampoVariable( Token *UnToken, unsigned *Indices, int Dim,
										 char *Id ){
	ContadorCampoVariable++; //Depuracion
	 Inicio= 0;
	 Dimension= Dim;
	 Tipo= UnToken->GetTipoDato();
	 AsignarValor(UnToken,Indices, Dim, Id);
	 // Aqui no se necesita saber el valor de retorno de AsignarValor
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 // Esta funcion accede a un elemento del arreglo cuya posicion esta
 // especificada por el parametro Indices. Devuelve un puntero a un Token
 // con el valor del elemento.
 Token *CampoVariable::Leer(unsigned *Indices, int Dim, char *Id ){
	Cajita *LeerDe= Inicio;
	if( Dim != Dimension ){
	  Buzon.SetIdentificadorAsociado( Id );
	  if( Dimension )
		 Buzon.Error( DIMENSION_NO_VALIDA );
	  else
       Buzon.Error( ID_NO_ES_ARREGLO );
	  return 0;
	}
	if( Indices )
	  for( ; LeerDe ; LeerDe= LeerDe->Sig ){
		 bool EsEste= true;
		 for( int i= 0 ; i<Dimension ; ++i )
			if( Indices[i] != LeerDe->Indices[i] ){
			  EsEste= false;
			  break;
			}
		 if( EsEste )
			break;
	  }
	Token *Nuevo;
	if( Tipo == STRING ){
	  const char *CadenaParaToken= ( LeerDe )? LeerDe->DatoStr : "*** BASURA ***";
	  Nuevo= new Token(CadenaParaToken, OPERANDO, CONSTANTE);
	  if( Buzon.GetHuboError() == true ){
		 delete Nuevo;
		 return 0;
	  }
	  return Nuevo;
	}
	if( Tipo == REAL ){
	  long double RealParaToken= ( LeerDe )? LeerDe->DatoReal
													 : 99999.99999L ;
	  Nuevo= new Token( RealParaToken, OPERANDO, CONSTANTE );
	  if( Buzon.GetHuboError() == true ){
		 delete Nuevo;
		 return 0;
	  }
	  return Nuevo;
	}
	bool LogicoParaToken= ( LeerDe )? LeerDe->DatoLogico : false;
	Nuevo= new Token( LogicoParaToken, OPERANDO, CONSTANTE );
	if( Buzon.GetHuboError() == true ){
		 delete Nuevo;
		 return 0;
	}
	return Nuevo;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 // Destruye el vector disperso y la memoria reservada por cada elemento
 // en caso de Tipo==STRING

 CampoVariable::~CampoVariable(){
	ContadorCampoVariable--; //Depuracion

	while(Inicio){
	  Cajita *Aux= Inicio->Sig;
	  if(Tipo == STRING)
		 if( Inicio->DatoStr )
			delete[] Inicio->DatoStr;
	  if(Dimension)
		 if( Inicio->Indices )
			delete[] Inicio->Indices;
	  delete Inicio;
	  Inicio= Aux;
	}
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class Variable{

	  char *Identificador;
	  CampoVariable *Campo;
	  TipoDato Tipo;
	  bool FP;
	  bool FU;
	  Variable *VengoDe;
	  Variable *PrimerPadre;
	  Variable *Sig;

	public:
	  Variable( char *Id, Token *UnToken, Variable *Vengo, unsigned *Indices,
					int Dim );
	  Variable( char *Id, Variable *Vengo );
	  void AsignarValor( Token *UnToken, unsigned *Indices, int Dim );
	  Token *Leer( unsigned *Indices, int Dim );
	  TipoDato GetTipo(){ return Tipo; }
	  bool GetFP(){ return FP; }
	  bool GetFU(){ return FU; }
	  Variable *GetVengoDe(){ return VengoDe; }
	  Variable *GetSig(){ return Sig; }
     Variable *GetPrimerPadre(){ return PrimerPadre; }
	  CampoVariable *GetCampo(){ return Campo; }
	  char *GetIdentificador(){ return Identificador; }
	  void SetSig( Variable *UnSig ){ Sig= UnSig; }
	  void SetCampo( CampoVariable *UnCampo ){ Campo= UnCampo; }
	  void SetFP( bool UnFP ){ FP= UnFP; }
	  void SetFU( bool UnFU ){ FU= UnFU; }
	  void SetTipo( TipoDato UnTipo ){ Tipo= UnTipo; }
	  ~Variable(){ /* ojo*/
		  ContadorVariable--;
		  delete []Identificador;
	  }

 };

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Variable::Variable( char *Id, Token *UnToken, Variable *Vengo,
							unsigned *Indices, int Dim ){
	ContadorVariable++;
	Identificador = dfd_strdup(Id);
	Campo= new CampoVariable( UnToken, Indices, Dim, Id );
	if( Buzon.GetHuboError() == true ){
	  delete Campo;
	  delete[] Identificador;
	  return;
	}
	Tipo= UnToken->GetTipoDato();
	FU= true;
	FP= false;
	VengoDe= Vengo;

	if( !VengoDe )
	  PrimerPadre= this;
	else
     PrimerPadre= VengoDe->PrimerPadre;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Variable::Variable( char *Id, Variable *Vengo ){
	ContadorVariable++;
	Identificador = dfd_strdup(Id);
	FU= false;
	FP= false;
	VengoDe= Vengo;

 	if( !VengoDe )
	  PrimerPadre= this;
	else
     PrimerPadre= VengoDe->PrimerPadre;

 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void Variable::AsignarValor( Token *UnToken, unsigned *Indices, int Dim ){
   void ActualizarVariables( Variable* );
	if ( FU )
	  Campo->AsignarValor( UnToken, Indices, Dim, Identificador );
	else{
	  Campo= new CampoVariable( UnToken, Indices, Dim, Identificador );
	  /*
	  Variable *Aux= this;
	  while( Aux ){
		 Aux->SetFU( true );
		 Aux->SetCampo( Campo );
		 Aux->SetTipo( Campo->GetTipo() );
		 Aux= Aux->GetVengoDe();
	  }
	  */
	  FU= true;
	  Tipo= Campo->GetTipo();
	  if( FP )
       ActualizarVariables(this);
	}

	return;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Token *Variable::Leer( unsigned *Indices, int Dim ){
	if( !FU ){
	  Buzon.SetIdentificadorAsociado(Identificador);
	  Buzon.Error( VARIABLE_NO_INICIALIZADA );
	  return 0;
	}
	return Campo->Leer( Indices, Dim, Identificador );
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 class ObjetoEjecucion;
 class Tabla {
	  ObjetoEjecucion *Retorno;
	  Variable *Inicio;
	  Tabla *Sig;

	public:
	  Tabla(){ Inicio= 0; ContadorTabla++; }
	  Variable *Crear( char *Id, Token *Untoken, Variable *Vengo,
					  unsigned *Indices, int Dim );
	  Variable *Crear( char *Id, Variable *Vengo, bool FP );
	  Variable *Buscar( char * Id );
	  Variable *GetInicio(){ return Inicio; }
	  Token *Leer( char *Id, unsigned *Indices, int Dim );
	  void AsignarValor( char *Id, Token *UnToken, unsigned *Indices,
								int Dim );
	  void SetRetorno(ObjetoEjecucion *R){Retorno= R;}
	  ObjetoEjecucion *GetRetorno(){return Retorno;}
	  void SetSig( Tabla *UnSig ){ Sig= UnSig; }
	  Tabla *GetSig(){ return Sig; }
	  ~Tabla();

 };

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Variable * Tabla::Buscar( char *Id){
	Variable *Aux= Inicio;
	while( Aux ){
	  if( !strcasecmp( Id, Aux->GetIdentificador() ) )
		  break;
	  Aux= Aux->GetSig();
	}
	return Aux;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Token *Tabla::Leer( char *Id, unsigned *Indices, int Dim ){

	Variable *Aux= Buscar( Id );
	if( !Aux ){
	  Buzon.SetIdentificadorAsociado( Id );
	  Buzon.Error( VARIABLE_NO_EXISTE );
	  return 0;
	}
	return Aux->Leer( Indices, Dim );
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Tabla::~Tabla(){
	ContadorTabla--;
	Variable *Aux;
	while( Inicio ){
	  Aux= Inicio;
	  Inicio= Inicio->GetSig();
	  if(( Aux->GetFU() ) && ( !Aux->GetFP() ) )
		 delete Aux->GetCampo();
	  delete Aux;
	}
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Variable *Tabla::Crear( char *Id, Token *UnToken, Variable *Vengo,
						  unsigned *Indices, int Dim ){
	Variable *NuevaVariable;
	NuevaVariable= new Variable( Id, UnToken, Vengo, Indices, Dim);
	if( Buzon.GetHuboError() ){
	  delete NuevaVariable;
	  return 0;
	}
	NuevaVariable->SetSig( Inicio );
	Inicio= NuevaVariable;
	return NuevaVariable;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 Variable *Tabla::Crear( char *Id, Variable *Vengo, bool FP ){
	Variable *NuevaVariable;
	NuevaVariable= new Variable( Id, Vengo);
	NuevaVariable->SetFP( FP );
	if( Buzon.GetHuboError() ){
	  delete NuevaVariable;
	  return 0;
	}
	NuevaVariable->SetSig( Inicio );
	Inicio= NuevaVariable;
	return NuevaVariable;
 }

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void Tabla::AsignarValor( char *Id, Token *UnToken, unsigned *Indices,
									int Dim ){

	Variable *Aux= Buscar( Id );

	if( !Aux )
	  Crear( Id, UnToken, 0/*VengoDe*/, Indices, Dim );
	else
	  Aux->AsignarValor( UnToken, Indices, Dim );

	return;
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 class PilaDeTablas{

	 Tabla *Tope;

	public:
	  PilaDeTablas(){ Tope= 0; }
	  void Apilar( Tabla *UnaTabla ){
		 UnaTabla->SetSig( Tope );
		 Tope= UnaTabla;
	  }
	  void Desapilar(){
		 Tabla *Aux;
		 Aux= Tope;
		 Tope= Tope->GetSig();
		 delete Aux;
	  }

	  Variable  *Crear( char *Id, Token *UnToken, Variable *Vengo, unsigned *Indices,
					  int Dim ){
		  return Tope->Crear( Id, UnToken, Vengo, Indices, Dim );
	  }
	  Variable *Crear( char *Id, Variable *Vengo, bool FP ){
		 return Tope->Crear( Id, Vengo, FP );
	  }
	  Variable *Buscar( char *Id );
	  void AsignarValor( char *Id, Token *UnToken, unsigned *Indices,
								int Dim ){

		 Tope->AsignarValor(  Id, UnToken, Indices, Dim );
		 return;
	  }
	  Tabla *GetTope(){ return Tope; }
	  Token *Leer( char *Id, unsigned *Indices, int Dim );
	  void ActualizarVariables( Variable * );
	  void Vacear();

 }PilaDeTablas;

 void PilaDeTablas::Vacear(){
	while( Tope ){
	  Tabla *Aux= Tope;
	  Tope= Tope->GetSig();
	  delete Aux;
	}
 }


 Token *PilaDeTablas::Leer( char *Id, unsigned *Indices, int Dim ){
	if( !Tope ){
	  Buzon.SetIdentificadorAsociado( Id );
	  Buzon.Error( VARIABLE_NO_EXISTE );
	  return 0;
	}
	return Tope->Leer( Id, Indices, Dim );
 }


 Variable *PilaDeTablas::Buscar( char *Id ){
	if( !Tope ){
	  Buzon.SetIdentificadorAsociado( Id );
	  Buzon.Error( VARIABLE_NO_EXISTE );
	  return 0;
	}
	return Tope->Buscar( Id );
 }



 void PilaDeTablas::ActualizarVariables( Variable *Muestra ){
	Variable *Primera= Muestra->GetPrimerPadre();
	bool HastaAqui= false;
	for( Tabla *t= Tope; t && !HastaAqui ; t= t->GetSig() )
	  for( Variable *v= t->GetInicio(); v; v= v->GetSig() ){

		 if( v->GetPrimerPadre()==Primera ){
			v->SetFU( true );
			v->SetCampo( Muestra->GetCampo() );
			v->SetTipo( Muestra->GetCampo()->GetTipo() );
		 }
		 if( v==Primera ){
			HastaAqui= true;
			break;
		 }

	  }
 }


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 void ActualizarVariables( Variable *Muestra ){
	PilaDeTablas.ActualizarVariables( Muestra );
 }
