#include <stdlib.h>

typedef struct vector_s{
	size_t real;
	size_t alloc;
	int* dato;
}vector_s;

typedef struct lista_s{
	char *dato;
	struct lista_s *sig;
}lista_s;

typedef struct T_usuario{
	int id;
	char *nombre;
	char *usuario;
	vector_s amigos;
	lista_s mensajes;
}T_usuario;

typedef struct usuario_s{
	struct usuario_s *sig;
	struct T_usuario *dato;
}usuario_s;