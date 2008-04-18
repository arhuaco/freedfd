#include <dfd.h>
#include <tokeniza.h>
#include <errores.h>

#include <stdio.h>
#include <stdlib.h>

/* set to 1 if you have GNU readline
 *   (libreadline5-dev in Debian or Ubuntu).
 * http://en.wikipedia.org/wiki/Readline
 */
#define HAVE_READLINE 0

#if HAVE_READLINE
  #include <readline/readline.h>
#else
  #include <iostream>
  using namespace std;
  #define BUF_SIZE 256
  char buffer[BUF_SIZE];
#endif

/* Counters that help us check we don't have (obvious) memory leaks */
int ContadorCajita = 0;
int ContadorCampoVariable = 0;
int ContadorVariable = 0;

const char *program_name = "FreeDFD";

BuzonDeErrores Buzon;


void
postfix_print(const char *line)
{
    Token * tok = GetPostfijo (line);

    if (Buzon.GetHuboError())
    {
      fprintf(stderr, "Error %s\n", Buzon.GetErrorInfo());
      Buzon.Vacear();
    }
    else
    {
      printf("Postfijo => ");
      while (tok)
      {
        char tmp_buf[256];
        printf("%s ", tok->AsString(tmp_buf, 256));

        Token *aux = tok;
        tok = tok->GetSig();
        delete aux;
      }
      putchar('\n');
    }
}

int
main(int argc, char *argv[])
{
#if HAVE_READLINE
  while(1)
  {
    char *line;
    line = readline("FreeDFD> ");
    if (!line)
      break;
    postfix_print(line);
    free(line);
  }
#else
  while(std::cin.getline(buffer, BUF_SIZE))
    postfix_print(buffer);
#endif

  return EXIT_SUCCESS;
}
