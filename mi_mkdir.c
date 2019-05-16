#include "directorios.h"
int main(int argc, char const *argv[])
{
    char nombre_dispositivo[1024];
    char ruta[1024];
    unsigned char permisos;
    int resultado;
    if (argc != 4)
    {
        fprintf(stderr, "Sintaxis: mi_mkdir <disco> <permisos> </ruta>\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nombre_dispositivo, argv[1]);
    permisos = atoi(argv[2]);
    if (permisos > 7)
    {
        fprintf(stderr, "Error:  modo inválido:<<%d>>\n", permisos);
        exit(EXIT_FAILURE);
    }
    strcpy(ruta, argv[3]);
    if (ruta[strlen(ruta) - 1] != '/')
    {
        fprintf(stderr, "Error: No es un directorio\n");
        exit(EXIT_FAILURE);
    }
    if (access(nombre_dispositivo, F_OK) == -1)
    {
        fprintf(stderr, "ERROR: No existe el archivo\n");
        exit(EXIT_FAILURE);
    }
    if (bmount(nombre_dispositivo) == -1)
    {
        fprintf(stderr, "ERROR: falló bmount\n");
        exit(EXIT_FAILURE);
    }
    resultado = mi_creat(ruta, permisos);
    switch (resultado)
    {
    case ERROR_ENTRADA_YA_EXISTENTE:
        fprintf(stderr, "Error: Entrada ya existente\n");
        exit(EXIT_FAILURE);
        break;
    case ERROR_EXTRAER_CAMINO:
        fprintf(stderr, "Error: Camino incorrecto\n");
        exit(EXIT_FAILURE);
        break;
    case ERROR_NO_EXISTE_DIRECTORIO_INTERMEDIO:
        fprintf(stderr, "Error: No existe directorio intermedio\n");
        exit(EXIT_FAILURE);
        break;
    case ERROR_NO_SE_PUEDE_CREAR_ENTRADA_EN_UN_FICHERO:
        fprintf(stderr, "Error: No se puede crear una entrada en un fichero\n");
        exit(EXIT_FAILURE);
        break;
    case ERROR_PERMISO_ESCRITURA:
        fprintf(stderr, "Error: Permiso denegado de escritura\n");
        exit(EXIT_FAILURE);
        break;
    case ERROR_PERMISO_LECTURA:
        fprintf(stderr, "Error: Permiso denegado de lectura\n");
        exit(EXIT_FAILURE);
        break;
    }
    if (bumount() == -1)
    {
        fprintf(stderr, "ERROR: falló bumount\n");
        exit(EXIT_FAILURE);
    }
}