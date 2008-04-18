#include <dfd.h>
#include <tokeniza.h>
#include <errores.h>
#include <entorno-ejecucion.h>
#include <evaluacion.h>

#include <stdio.h>
#include <stdlib.h>

/* set to 1 if you have GNU readline
 *   (libreadline5-dev in Debian or Ubuntu).
 * http://en.wikipedia.org/wiki/Readline
 */
#define HAVE_READLINE 0

#define BUF_SIZE 256

#if HAVE_READLINE
  #include <readline/readline.h>
#else
  #include <iostream>
  using namespace std;
  char buffer[BUF_SIZE];
#endif

/* Counters that help us check we don't have (obvious) memory leaks */
int ContadorCajita = 0;
int ContadorCampoVariable = 0;
int ContadorVariable = 0;
int ContadorToken = 0;
int ContadorTabla = 0;
EstadoPrograma Estado;
class PilaDeTablas PilaDeTablas; /* entorno de ejecucion, stack, tabla de simbolos */

const char *program_name = "FreeDFD";

BuzonDeErrores Buzon;

void
postfix_print(const char *line)
{
    Token * Post = GetPostfijo (line);

    if (Buzon.GetHuboError())
    {
      fprintf(stderr, "Error %s\n", Buzon.GetErrorInfo());
      Buzon.Vacear();
    }
    else
    {
      char tmp_buf[BUF_SIZE];
      Token *tok = Post;
      if (tok)
        printf("Postfijo => ");
      while (tok)
      {
        printf("%s ", tok->AsString(tmp_buf, BUF_SIZE));

        Token *aux = tok;
        tok = tok->GetSig();
        delete aux;
      }

      Token *Res = EvaluaPostfijo(Post);
      LiberarListaToken(Post);
      printf("%s ", Res->AsString(tmp_buf, BUF_SIZE));
      //delete Res; TODO: We have to delete this one.

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
    line = readline("DFD> ");
    if (!line)
      break;
    postfix_print(line);
    free(line);
  }
#else
  printf("DFD> ");
  while(std::cin.getline(buffer, BUF_SIZE))
  {
    postfix_print(buffer);
    printf("DFD> ");
  }
#endif

  return EXIT_SUCCESS;
}
