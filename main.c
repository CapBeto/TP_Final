#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "elementos.c"
#include "errores.h"

#define CANTIDAD_MINIMA_ARGUMENTOS 6
#define INIT_SIZE 1
#define CHOP_SIZE 2
#define MAX_SIZE_FILA 2000
#define MAX_SIZE_DIGITOS_AMIGOS 6
#define DELIMITADOR "="
#define DELIMITADOR_AMIGOS ","
#define IDENTIFICADOR_ID "id"
#define IDENTIFICADOR_NOMBRE "nombre"
#define IDENTIFICADOR_AMIGOS "amigos"
#define IDENTIFICADOR_MENSAJE "mensaje"

status_t cargar_datos(usuario_s *, int, char **);
status_t abrir_archivos(int, char **, FILE **);
status_t abrir_archivo(FILE *, char* );
status_t cerrar_archivos(int , FILE **);
status_t cargar_usuarios(FILE *, usuario_s *);
status_t cargar_archivos(FILE **,usuario_s *, int );
status_t cargar_usuario(FILE *, T_usuario *);
status_t Getline(char *, FILE *);
status_t cargar_amigos(vector_s,char*);

int main(int argc, char *argv[])
{
	usuario_s *red, *conductor;
	status_t st;
	int cantidad_archivos;
	
	if(argc<CANTIDAD_MINIMA_ARGUMENTOS)
	{
		st=ST_ERROR_CANTIDAD_ARGUMENTOS;
		imprimir_error(st);
		return EXIT_FAILURE;
	}
	cantidad_archivos=(argc-CANTIDAD_MINIMA_ARGUMENTOS);
	
	if((red=(usuario_s*)malloc(sizeof(usuario_s)))==NULL)
	{
		return ST_ERROR_NOMEM;
	}
	red->sig=NULL;
	red->dato=NULL;
	conductor=red;
	
	if((st=cargar_datos(conductor, cantidad_archivos, argv))!=ST_EXIT_OK)
	{
		imprimir_error(st);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

status_t cargar_datos(usuario_s *conductor, int cantidad_archivos, char *argv[])
{
	FILE **ppf=NULL;
	status_t st;
	
	if((st=abrir_archivos(cantidad_archivos, argv, ppf))!=ST_EXIT_OK)
	{
		return st;
	}
	if((st=cargar_archivos(ppf, conductor, cantidad_archivos))!=ST_EXIT_OK)
	{
		return st;
	}
	if((st=cerrar_archivos(cantidad_archivos, ppf))!=ST_EXIT_OK)
	{
		return st;
	}
	return ST_EXIT_OK;
}

status_t abrir_archivos(int cantidad_archivos, char *argv[], FILE **ppf)
{
	int i;
	status_t st;
	FILE **aux;
	size_t used_size, alloc_size;
	
	if(!argv)
	{
		return ST_ERROR_PUNTERO_NULO;
	}
	if((ppf=(FILE**)malloc(sizeof(FILE*)*INIT_SIZE))==NULL)
	{
		return ST_ERROR_NOMEM;
	}
	alloc_size=INIT_SIZE;
	used_size=0;
	for(i=0; i<=(cantidad_archivos); i++)
	{
		if(used_size==alloc_size)
		{
			alloc_size*=CHOP_SIZE;
			if((aux=(FILE **)realloc(ppf, sizeof(FILE *)*alloc_size))==NULL)
			{
				free(ppf);
				return ST_ERROR_NOMEM;
			}
			ppf=aux;
		}
		if((st=abrir_archivo(ppf[i], argv[i+CANTIDAD_MINIMA_ARGUMENTOS]))!=ST_EXIT_OK)
		{
			return st;
		}
	}
	return ST_EXIT_OK;
}

status_t abrir_archivo(FILE *pf, char* nombre_archivo)
{
	if(!nombre_archivo)
	{
		return ST_ERROR_PUNTERO_NULO;
	}
	if((pf=fopen(nombre_archivo,"r"))==NULL)
	{
		return ST_ERROR_ARCHIVO_NO_ENCONTRADO;
	}
	return ST_EXIT_OK;
}

status_t cerrar_archivos(int cantidad_archivos, FILE **ppf)
{
	int i;
	
	for(i=0; i<=(cantidad_archivos); i++)
	{
		if((fclose(ppf[i]))!=0)
		{
			return ST_ERROR_FICHERO_NO_CERRADO;
		}
	}
	return ST_EXIT_OK;
}

status_t cargar_archivos(FILE **ppf,usuario_s *conductor, int cantidad_archivos)
{
	int i;
	status_t st=ST_EXIT_OK;
	
	for(i=0; i<cantidad_archivos; i++)
	{
		if((cargar_usuarios(ppf[i], conductor))!=ST_EXIT_OK)
		{
			return st;
		}
	}
	return ST_EXIT_OK;
}

status_t cargar_usuario(FILE *pf, T_usuario *pusuario)
{
	char fila[MAX_SIZE_FILA], *aux;
	status_t st;
	
	if((st=Getline(fila,pf))!=ST_EXIT_OK)
	{
		return st;
	}
	if((aux=strtok(fila,DELIMITADOR))==NULL)
	{
		return ST_ERROR_EXTRAER_DATOS;/*caso usuario*/
	}
	if(strncmp(fila,IDENTIFICADOR_ID,strlen(fila))==1)
	{
		pusuario->id=atoi(aux);
	}
	if(strncmp(fila,IDENTIFICADOR_NOMBRE,strlen(fila))==1)
	{
		pusuario->nombre=aux;
	}
	if(strncmp(fila,IDENTIFICADOR_AMIGOS,strlen(fila))==1)
	{
		if((st=cargar_amigos(pusuario->amigos,aux))!=ST_EXIT_OK)
		{
			return st;
		}
	}
	if(strncmp(fila,IDENTIFICADOR_MENSAJE,strlen(fila))==1)
	{
		/*pusuario->mensaje=aux;*/
	}
	return ST_EXIT_OK;
}

status_t cargar_usuarios(FILE *pf, usuario_s *conductor)
{
	status_t st;

	while(!feof(pf))
	{
		if((conductor->sig=(usuario_s*)malloc(sizeof(usuario_s)))==NULL)
		{
			return ST_ERROR_NOMEM;
		}
		conductor=conductor->sig;
		
		if((conductor->dato=(T_usuario*)malloc(sizeof(T_usuario)))==NULL)
		{
			return ST_ERROR_NOMEM;
		}
		if((st=cargar_usuario(pf, conductor->dato))!=ST_EXIT_OK)
		{
			return st;
		}
		conductor->sig=NULL;
	}
	return ST_EXIT_OK;
}

status_t Getline(char *s, FILE *pf)
{
    int c, i;
	
	if(!pf)
	{
		return ST_ERROR_PUNTERO_NULO;
	}
    for (i=0;(c=fgetc(pf))!=EOF && c!='\n'; i++)
	{
		s[i]=c;
	}
    s[i]='\0';
	
    return ST_EXIT_OK;
}

status_t cargar_amigos(vector_s vector,char* linea)
{
	size_t used_size, alloc_size;
	char *auxiliar;
	int *aux;
	
	
	if((vector.dato=(int*)malloc(sizeof(int)*INIT_SIZE))==NULL)
	{
		return ST_ERROR_NOMEM;
	}
	alloc_size=INIT_SIZE;
	used_size=0;
	for(used_size=0; auxiliar==NULL;used_size++)
	{
		if(used_size==alloc_size)
		{
			alloc_size*=CHOP_SIZE;
			if((aux=(int*)realloc(vector.dato,sizeof(int)*alloc_size))==NULL)
			{
				return ST_ERROR_NOMEM;
			}
		}
		auxiliar=strtok(linea,DELIMITADOR_AMIGOS);
		vector.dato[used_size]=atoi(auxiliar);
		linea=NULL;
	}
	return ST_EXIT_OK;
}