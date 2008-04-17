#ifndef __FUNCION__H__
#define __FUNCION__H__

 // Generado con:
 // cat funcion.cpp | perl -nwe 'if (/\w+\s*\(\s*Token\s*\*/){chomp; print "void $_;\n"}' > funcion.h
 // y algo de edición manual.

void MenosUnitario (Token ** Pila);
void Seno (Token ** Pila);
void Coseno (Token ** Pila);
void Tangente (Token ** Pila);
void SenoHiperbolico (Token ** Pila);
void CosenoHiperbolico (Token ** Pila);
void TangenteHiperbolica (Token ** Pila);
void ArcoSeno (Token ** Pila);
void ArcoCoseno (Token ** Pila);
void ArcoTangente (Token ** Pila);
void Exp (Token ** Pila);
void Log (Token ** Pila);
void Ln (Token ** Pila);
void Trunc (Token ** Pila);
void Round (Token ** Pila);
void Abs (Token ** Pila);
void Sqrt (Token ** Pila);
void Random (Token ** Pila);
void Not (Token ** Pila);
void Len (Token ** Pila);
void Relacion (Token ** Pila, int Caso);
void Menor (Token ** Pila);
void Mayor (Token ** Pila);
void MenorIgual (Token ** Pila);
void MayorIgual (Token ** Pila);
void Igual (Token ** Pila);
void NoIgual (Token ** Pila);
void LogicosBinarios (Token ** Pila, int Caso);
void And (Token ** Pila);
void Nand (Token ** Pila);
void Or (Token ** Pila);
void Nor (Token ** Pila);
void Xor (Token ** Pila);
void Xnor (Token ** Pila);
void Sumar (Token ** Pila);
void NumericosBinarios (Token ** Pila, int Caso);
void Restar (Token ** Pila);
void Multiplicar (Token ** Pila);
void Dividir (Token ** Pila);
void Modulo (Token ** Pila);
void Elevar (Token ** Pila);
void Substring (Token ** Pila, int NArgumentos);

#endif
