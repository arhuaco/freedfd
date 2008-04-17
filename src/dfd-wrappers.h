#ifndef __WRAPPERS_H
#define __WRAPPERS_H

#include <token.h>

// Similar al strdup de string.h, pero esta usa "new" y no "malloc".
char *          dfd_strdup       (const char *example);

/* Funciones para manejar pilas de Tokens */
/* Deberíamos usar STL? */

void            Apila            (Token ** Inicio, Token * Nuevo);
Token *         Desapila         (Token ** Inicio);

#endif
