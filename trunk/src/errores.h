#ifndef __ERRORES_H
#define __ERRORES_H

// En este archivo de define la clase que se usa para guardar los errores
// que se producen en la ejecución, y en la evaluación de expresiones.

#include <stdio.h>

class BuzonDeErrores
{

  struct Mensaje
  {
    char *Info;
    TipoError Tipo;
    Mensaje *Sig;
  } *Inicio;

  char *IdentificadorAsociado;
  bool  HuboError;
  void  IntroducirError (char *Id, TipoError UnError, int Asociado = 1);

public:

    BuzonDeErrores ()
  {
    Inicio = NULL;
    IdentificadorAsociado = NULL;
    HuboError = false;
  }

  void Error (TipoError UnError);

  bool GetHuboError ()
  {
    return HuboError;
  }

  char *GetIdentificadorAsociado ()
  {
    return IdentificadorAsociado;
  }

  char *GetError ();

  void SetIdentificadorAsociado (char *Id);

  void Mostrar ();

  void Vacear ();

  ~BuzonDeErrores ();

};

#endif
