#include "directorios.h"
int main(int argc, char const *argv[])
{
    char nombre_dispositivo[1024];
    char ruta[1024];
    int resultado;

    if (argc != 3)
    {
        fprintf(stderr, "Sintaxis: mi_rm disco /ruta\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nombre_dispositivo, argv[1]);
    strcpy(ruta, argv[2]);
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
    resultado = mi_unlink(ruta);
    if (resultado == EXIT_FAILURE)
    {
        fprintf(stderr, "ERROR: falló mi_unlink\n");
    }
    if (bumount() == -1)
    {
        fprintf(stderr, "ERROR: falló bumount\n");
        exit(EXIT_FAILURE);
    }
}