#ifndef __ERRORES_H
#define __ERRORES_H

// En este archivo de define la clase que se usa para guardar los errores
// que se producen en la ejecución, y en la evaluación de expresiones.

#include <stdio.h>

class BuzonDeErrores
{
  void  IntroducirError (const char *msg, TipoError UnError);

public:

    BuzonDeErrores ()
  {
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
