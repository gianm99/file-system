#include "ficheros.h"
#define tambuffer BLOCKSIZE

int main(int argc, char const *argv[])
{
    char nombre_dispositivo[1024];
    char string[128];
    char buffer_texto[tambuffer];
    int ninodo, offset, leidos, total_leidos;
    struct STAT datos_inodo;

    if (argc != 3)
    {
        fprintf(stderr, "Sintaxis: escribir <nombre_dispositivo> <ninodo>\n");
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
    offset = 0;
    leidos = 0;
    total_leidos = 0;
    memset(buffer_texto, 0, tambuffer);
    while ((leidos = mi_read_f(ninodo, buffer_texto, offset, tambuffer)) > 0)
    {
        total_leidos += leidos;
        write(1, buffer_texto, leidos);
        offset += tambuffer;
        memset(buffer_texto, 0, tambuffer);
    }
    sprintf(string, "\ntotal_leidos %d\n", total_leidos);
    write(2, string, strlen(string));
    if (mi_stat_f(ninodo, &datos_inodo) == -1)
        exit(EXIT_FAILURE);
    sprintf(string, "tamEnBytesLog %d\n", datos_inodo.tamEnBytesLog);
    write(2, string, strlen(string));
    if (bumount(nombre_dispositivo) == -1)
        exit(EXIT_FAILURE);

    return 0;
}