#ifndef __LISTA__CADENAS__
#define __LISTA__CADENAS__

/* Esto se podría reemplazar usando la STL */

class ListaCadenas
{

  struct Nodo
  {
    char *Dato;
    Nodo *Sig;
  } *Inicio, *Iterador, *Ultimo;

  int NItems;

public:

    ListaCadenas ()
  {
    Inicio = 0;
    NItems = 0;
  }
  int GetNItems ()
  {
    return NItems;
  }
  void Reset ()
  {
    Iterador = Inicio;
  }
  char *Itera ();
  void Insertar (char *);
  void InsertarEn (int, char *);
  void Eliminar (int);
  void Vacear ();
};

#endif
