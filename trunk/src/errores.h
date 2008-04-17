#ifndef __ERRORES_H
#define __ERRORES_H

// En este archivo de define la clase que se usa para guardar los errores
// que se producen en la ejecución, y en la evaluación de expresiones.

#include <stdio.h>

struct Mensaje
{
  char *Info;
  TipoError Tipo;
  Mensaje *Sig;
};


class BuzonDeErrores
{
    char *IdentificadorAsociado;
    Mensaje *Inicio;
    bool  HuboError;

  void  IntroducirError (const char *msg, TipoError UnError);

public:

    BuzonDeErrores ()
  {
    Inicio = NULL;
    IdentificadorAsociado = NULL;
    HuboError = false;
  }

  void Error (TipoError UnError);

  bool GetHuboError ();

  char *GetIdentificadorAsociado ();

  char *GetError ();

  void SetIdentificadorAsociado (const char *Id);

  void Mostrar ();

  void Vacear ();

  ~BuzonDeErrores ();

};


#endif
