class Tabla
{
  // ObjetoEjecucion *Retorno;
  Variable *Inicio;
  Tabla *Sig;

public:
    Tabla ()
  {
    Inicio = 0;
    ContadorTabla++;
  }
  Variable *Crear (char *Id, Token * Untoken, Variable * Vengo,
                   unsigned *Indices, int Dim);
  Variable *Crear (char *Id, Variable * Vengo, bool FP);
  Variable *Buscar (char *Id);
  Variable *GetInicio ()
  {
    return Inicio;
  }
  Token *Leer (char *Id, unsigned *Indices, int Dim);
  void AsignarValor (char *Id, Token * UnToken, unsigned *Indices, int Dim);
  /*void SetRetorno (ObjetoEjecucion * R)
  {
    Retorno = R;
  }
  ObjetoEjecucion *GetRetorno ()
  {
    return Retorno;
  }*/
  void SetSig (Tabla * UnSig)
  {
    Sig = UnSig;
  }
  Tabla *GetSig ()
  {
    return Sig;
  }
  ~Tabla ();

};

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Variable *
Tabla::Buscar (char *Id)
{
  /* TODO: hacer que sea más rápido con hashing */
  Variable *Aux = Inicio;
  while (Aux)
    {
      if (!strcmpi (Id, Aux->GetIdentificador ()))
        break;
      Aux = Aux->GetSig ();
    }
  return Aux;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Token *
Tabla::Leer (char *Id, unsigned *Indices, int Dim)
{

  Variable *Aux = Buscar (Id);
  if (!Aux)
    {
      Buzon.SetIdentificadorAsociado (Id);
      Buzon.Error (VARIABLE_NO_EXISTE);
      return 0;
    }
  return Aux->Leer (Indices, Dim);
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Tabla::~Tabla ()
{
  ContadorTabla--;
  Variable *Aux;
  while (Inicio)
    {
      Aux = Inicio;
      Inicio = Inicio->GetSig ();
      if ((Aux->GetFU ()) && (!Aux->GetFP ()))
        delete Aux->GetCampo ();
      delete Aux;
    }
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Variable *
Tabla::Crear (char *Id, Token * UnToken, Variable * Vengo,
              unsigned *Indices, int Dim)
{
  Variable *NuevaVariable;
  NuevaVariable = new Variable (Id, UnToken, Vengo, Indices, Dim);
  if (Buzon.GetHuboError ())
    {
      delete NuevaVariable;
      return 0;
    }
  NuevaVariable->SetSig (Inicio);
  Inicio = NuevaVariable;
  return NuevaVariable;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Variable *
Tabla::Crear (char *Id, Variable * Vengo, bool FP)
{
  Variable *NuevaVariable;
  NuevaVariable = new Variable (Id, Vengo);
  NuevaVariable->SetFP (FP);
  if (Buzon.GetHuboError ())
    {
      delete NuevaVariable;
      return 0;
    }
  NuevaVariable->SetSig (Inicio);
  Inicio = NuevaVariable;
  return NuevaVariable;
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
Tabla::AsignarValor (char *Id, Token * UnToken, unsigned *Indices, int Dim)
{

  Variable *Aux = Buscar (Id);

  if (!Aux)
    Crear (Id, UnToken, 0 /*VengoDe */ , Indices, Dim);
  else
    Aux->AsignarValor (UnToken, Indices, Dim);

  return;
}


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

class PilaDeTablas
{

  Tabla *Tope;

public:
    PilaDeTablas ()
  {
    Tope = 0;
  }
  void Apilar (Tabla * UnaTabla)
  {
    UnaTabla->SetSig (Tope);
    Tope = UnaTabla;
  }
  void Desapilar ()
  {
    Tabla *Aux;
    Aux = Tope;
    Tope = Tope->GetSig ();
    delete Aux;
  }

  Variable *Crear (char *Id, Token * UnToken, Variable * Vengo,
                   unsigned *Indices, int Dim)
  {
    return Tope->Crear (Id, UnToken, Vengo, Indices, Dim);
  }
  Variable *Crear (char *Id, Variable * Vengo, bool FP)
  {
    return Tope->Crear (Id, Vengo, FP);
  }
  Variable *Buscar (char *Id);
  void AsignarValor (char *Id, Token * UnToken, unsigned *Indices, int Dim)
  {

    Tope->AsignarValor (Id, UnToken, Indices, Dim);
    return;
  }
  Tabla *GetTope ()
  {
    return Tope;
  }
  Token *Leer (char *Id, unsigned *Indices, int Dim);
  void ActualizarVariables (Variable *);
  void Vacear ();

} PilaDeTablas;

void
PilaDeTablas::Vacear ()
{
  while (Tope)
    {
      Tabla *Aux = Tope;
      Tope = Tope->GetSig ();
      delete Aux;
    }
}


Token *
PilaDeTablas::Leer (char *Id, unsigned *Indices, int Dim)
{
  if (!Tope)
    {
      Buzon.SetIdentificadorAsociado (Id);
      Buzon.Error (VARIABLE_NO_EXISTE);
      return 0;
    }
  return Tope->Leer (Id, Indices, Dim);
}


Variable *
PilaDeTablas::Buscar (char *Id)
{
  if (!Tope)
    {
      Buzon.SetIdentificadorAsociado (Id);
      Buzon.Error (VARIABLE_NO_EXISTE);
      return 0;
    }
  return Tope->Buscar (Id);
}

void
PilaDeTablas::ActualizarVariables (Variable * Muestra)
{
  Variable *Primera = Muestra->GetPrimerPadre ();
  bool HastaAqui = false;
  for (Tabla * t = Tope; t && !HastaAqui; t = t->GetSig ())
    for (Variable * v = t->GetInicio (); v; v = v->GetSig ())
      {

        if (v->GetPrimerPadre () == Primera)
          {
            v->SetFU (true);
            v->SetCampo (Muestra->GetCampo ());
            v->SetTipo (Muestra->GetCampo ()->GetTipo ());
          }
        if (v == Primera)
          {
            HastaAqui = true;
            break;
          }

      }
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void
ActualizarVariables (Variable * Muestra)
{
  PilaDeTablas.ActualizarVariables (Muestra);
}

