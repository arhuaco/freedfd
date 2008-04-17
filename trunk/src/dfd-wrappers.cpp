#include <string.h>
#include <campos.h>
#include <dfd-wrappers.h>

// Esto lo hicimos para no usar malloc, supongo.
// Creo que en todos lados usamos new[] y delete[].

char *
dfd_strdup (const char *example)
{
  char *new_string;

  new_string = new char [strlen(example) + 1];

  strcpy(new_string, example);

  return new_string;
}

// Apila,Desapila : Estas funciones se utilizan en la evaluación de
// expresiones para manejar una pila como lista ligada.

void
Apila (Token ** Inicio, Token * Nuevo)
{
  Nuevo->SetSig (*Inicio);
  *Inicio = Nuevo;
  return;
}

Token *
Desapila (Token ** Inicio)
{
  Token *Ret = *Inicio;
  (*Inicio) = (*Inicio)->GetSig ();
  return Ret;
}
