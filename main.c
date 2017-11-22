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
#define DELIM_USUARIO_IZQUIERDA '['
#define DELIM_USUARIO_DERECHA ']'

status_t cargar_datos(usuario_s *, int, char **);
status_t abrir_archivos(int, char **, FILE ***);
status_t abrir_archivo(FILE **, char* );
status_t cerrar_archivos(int , FILE **);
status_t cargar_perfiles(FILE *, usuario_s *);
status_t cargar_archivos(FILE **,usuario_s *, int );
status_t cargar_perfil(FILE *, T_usuario *);
status_t Getline(char **, FILE *);
status_t cargar_amigos(vector_s *,char*);
status_t cargar_usuario(char*,char*);
status_t cargar_mensajes(lista_s *, char*);
void imprimir_datos(usuario_s *);/*------------------------BORRAR------------------------*/

int main(int argc, char *argv[])
{
	usuario_s *red, *conductor;
	status_t st;
	int cantidad_archivos, n;
    
	if(argc<CANTIDAD_MINIMA_ARGUMENTOS)
	{
		st=ST_ERROR_CANTIDAD_ARGUMENTOS;
		imprimir_error(st);
		return EXIT_FAILURE;
	}
    for(n=0;n<argc;n++)
    {
        printf("Argumento %d:%s\n",n,argv[n]);
    }
    printf("2\n");
	cantidad_archivos=(argc-CANTIDAD_MINIMA_ARGUMENTOS);
	if((red=(usuario_s*)malloc(sizeof(usuario_s)))==NULL)/*red apunta aun espacio de memoria tamaño nodo usuario*/
	{
		return ST_ERROR_NOMEM;
	}
    printf("3\n");
	red->sig=NULL;
	red->dato=NULL;
	conductor=red;/*posicionamos el conductor al principio*/
    printf("4\n");
	if((st=cargar_datos(conductor, cantidad_archivos, argv))!=ST_EXIT_OK)/*creamos una lista cargada de datos desde los FILE*/
	{
		imprimir_error(st);
		return EXIT_FAILURE;
	}
    printf("Finalizando\n");
    imprimir_datos(red);
	return EXIT_SUCCESS;
}

status_t cargar_datos(usuario_s *conductor, int cantidad_archivos, char *argv[])
{
	FILE **ppf=NULL;
	status_t st=ST_EXIT_OK;
	printf("5\n");
	if((st=abrir_archivos(cantidad_archivos, argv, &ppf))!=ST_EXIT_OK)
	{
		return st;
	}
	if((st=cargar_archivos(ppf, conductor, cantidad_archivos))!=ST_EXIT_OK)/*Creamos una lista y pasamos la informacion desde los arhivos*/
	{
		return st;
	}
	if((st=cerrar_archivos(cantidad_archivos, ppf))!=ST_EXIT_OK)
	{
		return st;
	}
	return ST_EXIT_OK;
}

status_t abrir_archivos(int cantidad_archivos, char *argv[], FILE ***ppf)
{
	int i;
	status_t st;
	size_t used_size, alloc_size;

	if(!argv)
	{
		return ST_ERROR_PUNTERO_NULO;
	}
	if(((*ppf)=(FILE**)malloc(sizeof(FILE*)*INIT_SIZE))==NULL)
	{
		return ST_ERROR_NOMEM;
	}
	alloc_size=INIT_SIZE;
	used_size=0;
	for(i=0; i<=(cantidad_archivos); i++)
	{
        printf("8-Archivo Nro. %d\n",i);
		if(used_size==alloc_size)
		{
			alloc_size*=CHOP_SIZE;
			if(((*ppf)=(FILE **)realloc(ppf, sizeof(FILE *)*alloc_size))==NULL)
			{
				free(ppf);
				return ST_ERROR_NOMEM;
			}
		}
		if((st=abrir_archivo(&(*ppf[i]), argv[i+CANTIDAD_MINIMA_ARGUMENTOS-1]))!=ST_EXIT_OK)
		{
			return st;
		}
	}
	return ST_EXIT_OK;
}

status_t abrir_archivo(FILE **pf, char* nombre_archivo)
{
    FILE *pf_aux;
    printf("Abriendo archivo %s\n",nombre_archivo);
	if(!nombre_archivo)
	{
		return ST_ERROR_PUNTERO_NULO;
	}
	if((pf_aux=fopen(nombre_archivo,"r"))==NULL)
	{
		return ST_ERROR_ARCHIVO_NO_ENCONTRADO;
	}
    *pf=pf_aux;
	return ST_EXIT_OK;
}

status_t cerrar_archivos(int cantidad_archivos, FILE **ppf)
{
	int i;
	
	for(i=0; i<(cantidad_archivos); i++)
	{
        printf("Cerrando archivo %d de %d",i+1,cantidad_archivos);
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
    
    if(!ppf)
    {
        return ST_ERROR_PUNTERO_NULO;
    }
    printf("10\n");
	for(i=0;i<=cantidad_archivos;i++)/*Iteracion de los archivos, cargando la misma lista*/
	{
        printf("Cargando archivo Nro %d\n",i);
        if(!ppf[i])
        {
            return ST_ERROR_PUNTERO_NULO;
        }
		if((st=cargar_perfiles(ppf[i], conductor))!=ST_EXIT_OK)
		{
			return st;
		}
	}
	return ST_EXIT_OK;
}

status_t cargar_perfiles(FILE *pf, usuario_s *conductor)
{
	status_t st=ST_EXIT_OK;
    
    printf("cargar_perfiles\n");
    if(!pf)
    {
        return ST_ERROR_PUNTERO_NULO;
    }
	while(!feof(pf))/*Iteracion de la funcion cargar_perfil() con memoria para un nodo usuario y usuario_t*/
	{
        printf("11\n");
		if((conductor->sig=(usuario_s*)malloc(sizeof(usuario_s)))==NULL)
		{
			return ST_ERROR_NOMEM;
		}
		conductor=conductor->sig;
		if((conductor->dato=(T_usuario*)malloc(sizeof(T_usuario)))==NULL)
		{
			return ST_ERROR_NOMEM;
		}
        printf("12\n");
		if((st=cargar_perfil(pf, conductor->dato))!=ST_EXIT_OK)
		{
			return st;
		}
		conductor->sig=NULL;/*Apuntamos el ultimo elemeto de la lista a NULL por si la lista termino*/
	}
    printf("Termino de cargar perfiles...");
	return ST_EXIT_OK;
}

status_t cargar_perfil(FILE *pf, T_usuario *pusuario)
{
	char *fila=NULL, *aux;
	status_t st=ST_EXIT_OK;

    while(!feof(pf))
    {
        if((st=Getline(&fila,pf))!=ST_EXIT_OK)
        {
            return st;
        }
        printf("Linea cargada : %s\n",fila);
        if((aux=strtok(fila,DELIMITADOR)))
        {
            if(aux[0]=='\n')/*El caso de una linea vacia separa entre el texto para usuarios*/
            {
                return ST_EXIT_OK;
            }
            else 
                if((aux[0]==DELIM_USUARIO_IZQUIERDA)&&(aux[strlen(aux)-1]==DELIM_USUARIO_DERECHA))/*Si el primer y ultimo caracter son los delimitadores*/
                {
                    if((st=cargar_usuario(pusuario->usuario,aux))!=ST_EXIT_OK)/*Cargar usuario*/
                    {
                        return st;
                    }
                }
            else/*Si no es ninguno de los dos casos, la fila esta corrupta o mal escrita.*/
            {
                return ST_ERROR_EXTRAER_DATOS;
            }
        }
        printf("15\n");
        if(strncmp(fila,IDENTIFICADOR_ID,strlen(fila))==1)/*Cargar ID*/
        {
            pusuario->id=atoi(aux);
        }
        printf("16\n");
        if(strncmp(fila,IDENTIFICADOR_NOMBRE,strlen(fila))==1)/*Cargar nombre*/
        {
            pusuario->nombre=aux;
        }
        printf("17\n");
        if(strncmp(fila,IDENTIFICADOR_AMIGOS,strlen(fila))==1)/*Cargar amigos*/
        {
            if((st=cargar_amigos(&pusuario->amigos,aux))!=ST_EXIT_OK)
            {
                return st;
            }
        }
        printf("18\n");
        if(strncmp(fila,IDENTIFICADOR_MENSAJE,strlen(fila))==1)/*Cargar mensajes*/
        {
            if((st=cargar_mensajes(&pusuario->mensajes,fila))!=ST_EXIT_OK)
            {
                return st;
            }
        }
    }
	return ST_EXIT_OK;
}

status_t cargar_amigos(vector_s *vector,char* linea)
{
	size_t used_size, alloc_size;
	char *auxiliar;
	int *aux;
	
	if((vector->dato=(int*)malloc(sizeof(int)*INIT_SIZE))==NULL)/*Se otorga espacio al vector de enteros*/
	{
		return ST_ERROR_NOMEM;
	}
	alloc_size=INIT_SIZE;
	used_size=0;
	for(used_size=0;auxiliar==NULL;used_size++)/*Hasta que stortok() apunte a auxiliar a NULL*/
	{
		if(used_size==alloc_size)/*Si se queda sin espacio, se otorga mas, exponencialmente*/
		{
			alloc_size*=CHOP_SIZE;
			if((aux=(int*)realloc(vector->dato,sizeof(int)*alloc_size))==NULL)
			{
				return ST_ERROR_NOMEM;
			}
		}
		auxiliar=strtok(linea,DELIMITADOR_AMIGOS);/*A Auxiliar se lo apunta al siguiente delimitador despues de linea. Se podria poner un break en vez de la condicion de corte*/
		vector->dato[used_size]=atoi(auxiliar);
		linea=NULL;
        if(used_size>50)/*-----------------------------------------BORRAR-------------------------------------*/
        {
            printf("Fallamos en condicion de corte - pasar los amigos\n");
            return ST_ERROR_ESCRIBIR_ARCHIVO;
        }
	}
    vector->alloc=alloc_size;
    vector->real=used_size;
	return ST_EXIT_OK;
}

status_t Getline(char **s, FILE *pf)
{
    char c;
    char* aux;
    size_t alloc_size, used_size=0;
	
	if(!pf)
	{
		return ST_ERROR_PUNTERO_NULO;
	}
    if((aux=(char*)calloc(0,sizeof(char)*INIT_SIZE))==NULL)
    {
        return ST_ERROR_NOMEM;
    }
    used_size=INIT_SIZE;
    for(used_size=0; (c=fgetc(pf))!=EOF && c!='\n'; used_size++)
	{
        if(alloc_size==used_size)
        {
            alloc_size*=CHOP_SIZE;
            if((aux=(char*)realloc(aux,sizeof(char*)*alloc_size))==NULL)
            {
                return ST_ERROR_NOMEM;
            }
        }
		aux[used_size]=c;
        printf("%c",c);
	}
    aux[used_size]='\0';
    printf("%s",aux);
    *s=aux;
    return ST_EXIT_OK;
}

status_t cargar_usuario(char *usuario,char *linea)
{
    int len = strlen(linea); 
    
    printf("cargar_usuario\n");
    if(len > 0)
    {
        linea++;/*Adelantar el puntero un caracter*/
    }
    else
    {
        return ST_ERROR_EXTRAER_DATOS;
    }
    if(len > 1)
    {
        linea[len - 2] = '\0';/*Reemplazar el ultimo caracter con un fin de linea*/ 
    }
    else
    {
        return ST_ERROR_EXTRAER_DATOS;
    }
    if((usuario=(char*)malloc(sizeof(char)*(len+1)))==NULL)/*Memoria para el largo de la cadena +1*/
    {
        return ST_ERROR_NOMEM;
    }
    strcpy(usuario,linea);
    return ST_EXIT_OK;
}

status_t cargar_mensajes(lista_s *lista, char *fila)
{
    lista_s *conductor=lista;
    
    conductor->dato=NULL;
    while(conductor->sig!=NULL)/*Recorre la lista hasta el ultimo elemento*/
    {
        conductor=conductor->sig;
    }
    if((conductor->sig=(lista_s*)malloc(sizeof(lista_s)))==NULL)/*Damos espacio para un siguiente nodo de mensaje*/
    {
        return ST_ERROR_NOMEM;
    }
    conductor=conductor->sig;/*Pasamos al siguiente nodo*/
    if((conductor->dato=(char*)malloc(sizeof(char)*strlen(fila))))/*Damos espacio para los caracteres y apuntamos a dato ahi*/
    {
        return ST_ERROR_NOMEM;
    }
    strcpy(conductor->dato,fila);/*Copiamos el dato ahi VALIDAR*/
    return ST_EXIT_OK;
}

void imprimir_datos(usuario_s *red)/*-------------------------BORRAR---------------------------*/
{
    usuario_s *conductor=red;
    lista_s *conductor_mensaje;
    int i;
    
    conductor->dato=NULL;
    while(conductor->sig!=NULL)
    {
        printf("%s\n",conductor->dato->usuario);
        printf("%s\n",conductor->dato->nombre);
        printf("%d\n",conductor->dato->id);
        for(i=0;conductor->dato->amigos.real>=i;i++)
        {
            printf("%n,",&((conductor->dato->amigos).dato[i]));
        }
        printf("\n");
        conductor_mensaje=(conductor->dato->mensajes).sig;
        while(conductor_mensaje->sig!=NULL)
        {
            printf("%s",conductor_mensaje->dato);
        }
    }
}