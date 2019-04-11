#include "directorios.h"

int extraer_camino(const char *camino, char *inicial, char *final, char *tipo)
{
    const char ch = '/';
    char *ret;
    if (camino == NULL) // Está vacío
        return -1;
    if (camino[0] != ch) //No hay primer '/'
        return -1;
    ret = strchr(camino + 1, ch);
    if (!ret)
    {
        strcpy(tipo, "FICHERO");
        strcpy(inicial, camino + 1);
    }
    else
    {
        strcpy(tipo, "DIRECTORIO");
        strncpy(inicial, camino, (int)ret - (int)camino);
        strcpy(final, ret);
    }
    return 0;
}
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir,
                   unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos)
{
    char *inicial;
    char *final;
    char *tipo;
    struct inodo inodo_dir;
    {
        /* data */
    };

    if (camino_parcial == '/')
    {
        *p_inodo = 0;
        *p_entrada = 0;
        return 0;
    }
    if (extraer_camino(camino_parcial, inicial, final, &tipo) == -1)
    {
        fprintf(stderr, "ERROR_EXTRAER_CAMINO");
        return -1;
    }
    leer_inodo(p_inodo_dir, &inodo_dir);
}
