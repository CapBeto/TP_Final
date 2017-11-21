#ifndef ERRORES_H
#define ERRORES_H
#include <stdlib.h>

#define TXT_ERROR "Error"
#define MSG_ST_ERROR_ARCHIVO_NO_ENCONTRADO "El archivo no se pudo encotrar"
#define MSG_ST_ERROR_ARCHIVO_CORRUPTO "El archivo tiene datos faltantes o esta corrupto"
#define MSG_ST_ERROR_NOMEM "No existe memoria para la operacion"
#define MSG_ST_ERROR_PUNTERO_NULO "El puntero esta vacio"
#define MSG_ST_ERROR_NO_ABIERTO "El archivo no se abrio correctamente"
#define MSG_ST_ERROR_FICHERO_NO_CERRADO "El fichero no se cerro correctamente"
#define MSG_ST_ERROR_ARCHIVO_VACIO "El archivo esta vacio"
#define MSG_ST_ERROR_EXTRAER_DATOS "No se pudieron extraer los datos"
#define MSG_ST_ERROR_CREAR_ARCHIVO "No se pudo crear el archivo binario"
#define MSG_ST_ERROR_ESCRIBIR_ARCHIVO "No se pudo editar el archivo binario"
#define MSG_ST_ERROR_CANTIDAD_ARGUMENTOS "Cantidad de argumentos incorrecta\n"
#define MSG_ST_ERROR_FSEEK "Error en la funcion fseek(para eliminar)"
#define MSG_ST_ERROR_ID_EXISTENTE "El id ya existe dentro de la base de datos"
#define MSG_ST_ERROR_RENOMBRAR_ARCHIVO "El archivo no ha sido renombrado"
#define MSG_ST_ERROR_ELIMINAR_ARCHIVO "No se pudo eliminar el archivo"
#define MSG_EXIT_OK "Error en la validacion EXIT_OK"


typedef enum{
ST_ERROR_ARCHIVO_NO_ENCONTRADO,
ST_ERROR_ARCHIVO_CORRUPTO,
ST_ERROR_NOMEM,
ST_ERROR_PUNTERO_NULO,
ST_ERROR_NO_ABIERTO,
ST_ERROR_FICHERO_NO_CERRADO,
ST_ERROR_ARCHIVO_VACIO,
ST_ERROR_EXTRAER_DATOS,
ST_ERROR_CREAR_ARCHIVO,
ST_ERROR_ESCRIBIR_ARCHIVO,
ST_ERROR_CANTIDAD_ARGUMENTOS,
ST_ERROR_FSEEK,
ST_ERROR_ID_EXISTENTE,
ST_ERROR_RENOMBRAR_ARCHIVO,
ST_ERROR_ELIMINAR_ARCHIVO,
ST_EXIT_OK
}status_t;

static char *msg_errores[]={
MSG_ST_ERROR_ARCHIVO_NO_ENCONTRADO,
MSG_ST_ERROR_ARCHIVO_CORRUPTO,
MSG_ST_ERROR_NOMEM,
MSG_ST_ERROR_PUNTERO_NULO,
MSG_ST_ERROR_NO_ABIERTO,
MSG_ST_ERROR_FICHERO_NO_CERRADO,
MSG_ST_ERROR_ARCHIVO_VACIO,
MSG_ST_ERROR_EXTRAER_DATOS,
MSG_ST_ERROR_CREAR_ARCHIVO,
MSG_ST_ERROR_ESCRIBIR_ARCHIVO,
MSG_ST_ERROR_CANTIDAD_ARGUMENTOS,
MSG_ST_ERROR_FSEEK,
MSG_ST_ERROR_ID_EXISTENTE,
MSG_ST_ERROR_RENOMBRAR_ARCHIVO,
MSG_ST_ERROR_ELIMINAR_ARCHIVO,
MSG_EXIT_OK,
};

void imprimir_error(status_t st)
{
	printf("%s<%d>: %s\n",TXT_ERROR,st,msg_errores[st]);
}


#endif