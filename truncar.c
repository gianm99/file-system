#include "ficheros.h"

int main(int argc, char const *argv[])
{
    char nombre_dispositivo[1024];
    int ninodo, nbytes;
    struct STAT datos;

    if (argc != 4)
    {
        fprintf(stderr, "Sintaxis: truncar <nombre_dispositivo> <ninodo> <nbytes>\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nombre_dispositivo, argv[1]);
    if (access(nombre_dispositivo, F_OK) == -1)
    {
        fprintf(stderr, "ERROR: No existe el archivo\n");
        exit(EXIT_FAILURE);
    }
    if (bmount(nombre_dispositivo) == -1)
        exit(EXIT_FAILURE);
    ninodo = atoi(argv[2]);
    nbytes = atoi(argv[3]);

    if (bmount(nombre_dispositivo) == -1)
        exit(EXIT_FAILURE);
    mi_truncar_f(ninodo, nbytes);
    if (mi_stat_f(ninodo, &datos) == -1)
        exit(EXIT_FAILURE);
    fprintf(stderr, "DATOS INODO %d\n", ninodo);
    datos_STAT(datos);
    if (bumount(nombre_dispositivo) == -1)
        exit(EXIT_FAILURE);
    return 0;
}