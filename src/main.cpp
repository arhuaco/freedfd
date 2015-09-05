#include <dfd.h>
#include <tokeniza.h>
#include <errores.h>
#include <evaluacion.h>

/* Counters that help us check we don't have (obvious) memory leaks */
int ContadorCajita = 0;
int ContadorCampoVariable = 0;
int ContadorVariable = 0;
int ContadorToken = 0;
int ContadorTabla = 0;
int ContadorNodoListaExpresiones = 0;
int ContadorVariableVector = 0;

const char *program_name = "FreeDFD";
const char *program_version = "1.5.1";

BuzonDeErrores Buzon;

/*
    Token * Post = GetPostfijo (theexp);
    if (Buzon.GetHuboError())
    {
      fprintf(stderr, "Error de sintaxis: %s\n", Buzon.GetErrorInfo());
      Buzon.Vacear();
    }
    else
    {
      Token *Res = EvaluaPostfijo(Post);
      if (Buzon.GetHuboError())
      {
        fprintf(stderr, "Error en ejecución: %s\n", Buzon.GetErrorInfo());
        Buzon.Vacear();
      }
      else if (Res)
      {
      }
      else
        fprintf(stderr, __FILE__":%d EvaluaPostfijo returned NULL\n", __LINE__);

      LiberarListaToken(Post);
      if (Res)
        delete Res;
    }
}
*/

int
main(int argc, char *argv[])
{
  /* Una asignación, esto se va a dejar de usar así como
   * se hace acá. Creo */
  //PilaDeTablas.Apilar(new Tabla); /* new symbol table */
  //OE_Asignacion x;
  //x.NuevaAsignacion("y(1)", "1");
  //x.Preprocesar();
  //x.Ejecutar();
  //x.Despreprocesar();
  return EXIT_SUCCESS;
}
