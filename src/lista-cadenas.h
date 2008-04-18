#ifndef __LISTA__CADENAS__
#define __LISTA__CADENAS__

/* Creo que esta clase va a desaparecer */

class ListaCadenas
{

  struct Nodo
  {
    const char *Dato;
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
  const char *Itera ();
  void Insertar (const char *);
  void InsertarEn (int, char *);
  void Eliminar (int);
  void Vacear ();
};

#endif
