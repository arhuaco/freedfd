#ifndef __DFD__H__
#define __DFD__H__

#define ASSERT

#define __DFD__SPANISH__ 1
#define __DFD__PORTUGUESE__ 0

extern const char *program_name;


enum Accion { EDICION = 0, EJECUCION, DEPURACION };

enum TipoDato { REAL, STRING, LOGICO };

enum TipoToken { OPERADOR, OPERANDO };

enum TipoAlmacenamiento { CONSTANTE, VARIABLE };

enum TipoError {
    LOS_TIPOS_NO_COINCIDEN, ID_NO_ES_ARREGLO, DIMENSION_NO_VALIDA,
    CONSTANTE_NUMERICA_ILEGAL, NOMBRE_VARIABLE_INCORRECTO,
    FUNCION_REQUIERE_PARAMETRO,
    FALTA_OPERADOR, CERRADO_PARENTESIS_NO_ABIERTO,
    CADENA_INDETERMINADA,       /*OPERADOR_ILEGAL_DE_CADENA, */
    CONSTANTE_LOGICA_ILEGAL, OPERADOR_LOGICO_ILEGAL,
    FALTA_OPERANDO, OPERADOR_ILEGAL,
    CARACTER_ILEGAL, ABIERTO_PARENTESIS_NO_CERRADO,
    LLAMADO_DE_FUNCION_NO_VALIDO,

    /*COMA_ILEGAL, */ ERROR_INDEFINIDO,
    DESBORDAMIENTO, DESBORDAMIENTO_NEGATIVO,
    VARIABLE_NO_EXISTE, VARIABLE_NO_INICIALIZADA,
    DIVISION_POR_CERO, /*SUBINDICE_NO_VALIDO, */ INDICES,
    ILEGAL_COMA_O_CADENA_VACIA, SOLO_VARIABLES, NO_INDICES,
    TIPO_DEBE_SER_LOGICO, VINICIAL_CONTADOR,
    VFINAL_CONTADOR, INCREMENTO_CONSTANTE, CONTADOR_REAL,
    LIMITE_REAL, PARAMETRO_ILEGAL, PARAMETRO_REPETIDO,
    ILEGAL_NOMBRE_SUBPROGRAMA, SUBPROGRAMA_NO_EXISTE,
    SUBPROGRAMA_REPETIDO, MUCHOS_ARGUMENTOS,
    POCOS_ARGUMENTOS, PARAMETRO_NO_VARIABLE
};

/* shared symbols */
class BuzonDeErrores;
extern BuzonDeErrores Buzon;
extern int ContadorCajita;
extern int ContadorCampoVariable;
extern int ContadorVariable;
extern int ContadorToken;
extern int ContadorTabla;
extern int ContadorNodoListaExpresiones;
extern int ContadorVariableVector;

class EstadoPrograma {
public:
  EstadoPrograma(){
    AngulosEnGrados = true;
  }
  bool AngulosEnGrados;
};
extern EstadoPrograma Estado;

class PilaDeTablas;
extern PilaDeTablas PilaDeTablas;

#endif
