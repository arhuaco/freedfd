#include <string.h>

// Esto lo hicimos para no usar malloc, supongo.
// Creo que en todos lados usamos new[] y delete[].

char *
dfd_strdup (const char *example)
{
  char *new_string;

  new_string = new char [strlen(new_string) + 1];

  strcpy(new_string, example);

  return new_string;
}
