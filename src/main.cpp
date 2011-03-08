#include <dfd.h>
#include <tokeniza.h>
#include <errores.h>
#include <entorno-ejecucion.h>
#include <evaluacion.h>
#include <objetos-ejecucion.h>

#include <stdio.h>
#include <stdlib.h>

/* Define HAVE_READLINE if you have GNU readline
 *   (libreadline5-dev in Debian or Ubuntu).
 * http://en.wikipedia.org/wiki/Readline
 * #define HAVE_READLINE
 */

#define BUF_SIZE 256

#ifdef HAVE_READLINE
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
int ContadorNodoListaExpresiones = 0;
int ContadorVariableVector = 0;


EstadoPrograma Estado;
class PilaDeTablas PilaDeTablas; /* entorno de ejecucion, stack, tabla de simbolos */

const char *program_name = "FreeDFD";
const char *program_version = "1.5.1";

BuzonDeErrores Buzon;


void
print_counters_if_needed()
{
  if (ContadorToken)
    fprintf(stderr, "Ops. ContadorToken == %d\n", ContadorToken);
  if (ContadorTabla != 1)
    fprintf(stderr, "Ops. ContadorTabla == %d\n", ContadorTabla);
  if (ContadorNodoListaExpresiones)
    fprintf(stderr, "Ops. ContadorNodoListaExpresiones == %d\n", ContadorTabla);
  if (ContadorVariableVector != 1)
    fprintf(stderr, "Ops. ContadorVariableVector == %d\n", ContadorVariableVector);
#if 0
  if (ContadorCajita || ContadorCampoVariable || ContadorVariable)
  {
    fprintf(stderr, "Memoria ocupada => ");
    fprintf(stderr, "cajas:%d campo_variable:%d variable:%d\n", ContadorCajita, ContadorCampoVariable, ContadorVariable);
  }
#endif
}

void
postfix_test(const char *theexp, const char *result)
{
    Token * Post = GetPostfijo (theexp);
    if (Buzon.GetHuboError())
    {
      fprintf(stderr, "Error de sintaxis: %s\n", Buzon.GetErrorInfo());
      Buzon.Vacear();
    }
    else
    {
      char tmp_buf[BUF_SIZE];

      Token *Res = EvaluaPostfijo(Post);

      if (Buzon.GetHuboError())
      {
        fprintf(stderr, "Error en ejecución: %s\n", Buzon.GetErrorInfo());
        Buzon.Vacear();
      }
      else if (Res)
      {
        const char *r = Res->AsString(tmp_buf, BUF_SIZE);
        if (strncmp(r, result, BUF_SIZE))
        {
          fprintf(stderr, "Test FAILED:");
        }
        else
          fprintf(stderr, "Test OK:");
        fprintf(stderr, "EvaluaPostfijo(%s) returned '%s' and we expected '%s'\n", theexp, r, result);
      }
      else
        fprintf(stderr, __FILE__":%d EvaluaPostfijo returned NULL\n", __LINE__);

      LiberarListaToken(Post);
      if (Res)
        delete Res;
    }
}


void
postfix_print(const char *line)
{
    Token * Post = GetPostfijo (line);

    if (Buzon.GetHuboError())
    {
      fprintf(stderr, "Error de sintaxis: %s\n", Buzon.GetErrorInfo());
      Buzon.Vacear();
    }
    else
    {
      char tmp_buf[BUF_SIZE];

      if (Post)
        printf("Postfijo => ");

      for (Token *tok = Post; tok; tok = tok->GetSig())
        printf("%s ", tok->AsString(tmp_buf, BUF_SIZE));
      if (Post)
        putchar('\n');

      fflush(stdout);

      Token *Res = EvaluaPostfijo(Post);

      if (Buzon.GetHuboError())
      {
        fprintf(stderr, "Error en ejecución: %s\n", Buzon.GetErrorInfo());
        Buzon.Vacear();
      }
      else if (Res)
        printf("Resultado => %s\n", Res->AsString(tmp_buf, BUF_SIZE));
      else
        fprintf(stderr, __FILE__":%d EvaluaPostfijo returned NULL\n", __LINE__);

      LiberarListaToken(Post);
      if (Res)
        delete Res;
    }

    print_counters_if_needed();
}

void
make_tests(void)
{
  postfix_test("1", "1");
  postfix_test("2 + 2", "4");
  postfix_test("2.3 + 2.2", "4.5");
  postfix_test(".v.", ".V.");
  postfix_test(".f.", ".F.");
  postfix_test("'hola'", "hola");
  postfix_test("'hola' + ' mundo'", "hola mundo");
  // caso 2
  postfix_test("y(1) + 2.2", "3.2");
  // caso 4
  postfix_test("(2 + 3) * 2", "10");
  postfix_test("(7 - 2) * 2", "10");
  postfix_test("7 / 2", "3.5");
  postfix_test("3 = 3", ".V.");
  postfix_test("3 != 2", ".V.");
}

int
main(int argc, char *argv[])
{
  const char *prompt = ">>> ";


  fprintf(stderr, "%s %s " __DATE__ "\n\n", program_name, program_version);
  fflush(stderr);

  /* Una asignación, esto se va a dejar de usar así como
   * se hace acá. Creo */
  PilaDeTablas.Apilar(new Tabla); /* new symbol table */
  OE_Asignacion x;
  x.NuevaAsignacion("y(1)", "1");
  x.Preprocesar();
  x.Ejecutar();
  x.Despreprocesar();

  make_tests();

#ifdef HAVE_READLINE
  while(1)
  {
    char *line;
    line = readline(prompt);
    if (!line)
      break;
    postfix_print(line);
    free(line);
  }
#else
  printf(prompt);
  while(std::cin.getline(buffer, BUF_SIZE))
  {
    postfix_print(buffer);
    printf(prompt);
  }
#endif

  return EXIT_SUCCESS;
}
