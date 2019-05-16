#include "directorios.h"
int main(int argc, char const *argv[])
{
    char nombre_dispositivo[1024];
    char ruta_original[1024];
    char ruta_enlace[1024];

    if (argc != 4)
    {
        fprintf(stderr, "Sintaxis: mi_link  disco  /ruta_fichero_original  /ruta_enlace\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nombre_dispositivo, argv[1]);
    strcpy(ruta_original, argv[2]);
    if (ruta_original[strlen(ruta_original) - 1] == '/')
    {
        fprintf(stderr, "Error: No es un fichero\n");
        exit(EXIT_FAILURE);
    }
    strcpy(ruta_enlace, argv[3]);
    if (ruta_enlace[strlen(ruta_enlace) - 1] == '/')
    {
        fprintf(stderr, "Error: No es un fichero\n");
        exit(EXIT_FAILURE);
    }

    if (access(nombre_dispositivo, F_OK) == -1)
    {
        fprintf(stderr, "ERROR: No existe el disco\n");
        exit(EXIT_FAILURE);
    }

    if (bmount(nombre_dispositivo) == -1)
    {
        fprintf(stderr, "ERROR: falló bmount\n");
        exit(EXIT_FAILURE);
    }
    mi_link(ruta_original, ruta_enlace);
    if (bumount() == -1)
    {
        fprintf(stderr, "ERROR: falló bumount\n");
        exit(EXIT_FAILURE);
    }
}