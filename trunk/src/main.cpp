#include <dfd.h>
#include <tokeniza.h>
#include <errores.h>

const char *program_name = "FreeDFD";
BuzonDeErrores Buzon;

int
main(int argc, char *argv[])
{

  Token * tok = GetPostfijo ("3 + 2");

  return 0;
}
