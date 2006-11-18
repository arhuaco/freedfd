#include <string.h>

char *
dfd_strdup (const char *example)
{
  char *new_string;

  // Se adiciona una casilla para que contenga el '\0' que va al final.
  
  new_string = new char [strlen(new_string) + 1];

  strcpy(new_string, example);

  return new_string;
}
