#include "ficheros.h"
#define ERROR_EXTRAER_CAMINO 2
#define ERROR_PERMISO_LECTURA 3
#define ERROR_NO_EXISTE_ENTRADA_CONSULTA 4
#define ERROR_NO_SE_PUEDE_CREAR_ENTRADA_EN_UN_FICHERO 5
#define ERROR_PERMISO_ESCRITURA 6
#define ERROR_NO_EXISTE_DIRECTORIO_INTERMEDIO 7
#define ERROR_ENTRADA_YA_EXISTENTE 8
struct entrada
{
    char nombre[60]; //En el SF ext2 la longitud del nombre es 255
    unsigned int ninodo;
};
#define tamEntrada sizeof(struct entrada)
int extraer_camino(const char *camino, char *inicial, char *final, char *tipo);
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir,
                   unsigned int *p_inodo, unsigned int *p_entrada,
                   char reservar, unsigned char permisos);
int mi_creat(const char *camino, unsigned char permisos);
int mi_dir(const char *camino, char *buffer);
int mi_chmod(const char *camino, unsigned char permisos);
int mi_stat(const char *camino, struct STAT *p_stat);
int mi_read(const char *camino, void *buf, unsigned int offset, unsigned int nbytes);
int mi_write(const char *camino, const void *buf, unsigned int offset, unsigned int nbytes);
//Colores y estilos
#define RED "\x1B[31m"
#define BLU "\x1B[34m"
#define RESET "\x1B[0m"
#define BOLD "\e[1m"
//Nivel 11
int mi_link(const char *camino1, const char *camino2);
int mi_unlink(const char *camino);
