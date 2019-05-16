#include "directorios.h"

int main(int argc, char const *argv[])
{
    char nombre_dispositivo[1024];
    char ruta[1024];
    char buffer[5000]; // PREGUNTAR
    int numentradas;
    if (argc != 3)
    {
        fprintf(stderr, "Sintaxis: mi_ls <disco> </ruta>\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nombre_dispositivo, argv[1]);
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
    strcpy(ruta, argv[2]);
    numentradas = mi_dir(ruta, buffer);
    if (ruta[strlen(ruta) - 1] == '/')
    {
        printf("Total: %d\n", numentradas);
    }
    if (numentradas == 0)
        exit(EXIT_SUCCESS);
    printf("Tipo\tPermisos\tmTime\t\tTamaño\tNombre\n");
    printf("---------------------------------------------------------\n");
    printf(buffer);
    if (bumount() == -1)
    {
        fprintf(stderr, "ERROR: falló bumount\n");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}