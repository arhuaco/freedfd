#include <dfd.h>
#include <tokeniza.h>
#include <errores.h>

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


const char *program_name = "FreeDFD";

BuzonDeErrores Buzon;

int
main(int argc, char *argv[])
{
  while(1)
  {
    char *line;

    line = readline("FreeDFD> ");
    if (!line)
      break;

    Token * tok = GetPostfijo (line);
    printf("%s\n", line);
    if (Buzon.GetHuboError())
      fprintf(stderr, "Error %s\n", Buzon.GetErrorInfo());
    free(line);
  }

  return 0;
}
