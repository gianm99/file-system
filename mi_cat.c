#include "directorios.h"
#define readbuffer 1500

int main(int argc, char const *argv[])
{
    char nombre_dispositivo[1024];
    char string[128];
    char ruta[1024];
    char buffer_texto[readbuffer];
    int offset, leidos, total_leidos, resultado;
    struct STAT datos_inodo;
    unsigned int p_inodo_dir, p_inodo, p_entrada;

    if (argc != 3)
    {
        fprintf(stderr, "Sintaxis: ./mi_cat <disco> </ruta_fichero>\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nombre_dispositivo, argv[1]);
    strcpy(ruta, argv[2]);
    //comprueba que existe el disco
    if (access(nombre_dispositivo, F_OK) == -1)
    {
        fprintf(stderr, "ERROR: No existe el disco\n");
        exit(EXIT_FAILURE);
    }
    //comprueba que no es un directorio
    if (ruta[strlen(ruta) - 1] == '/')
    {
        fprintf(stderr, "ERROR: ruta de directorio");
        exit(EXIT_FAILURE);
    }
    if (bmount(nombre_dispositivo) == -1)
        exit(EXIT_FAILURE);
    offset = 0;
    leidos = 0;
    total_leidos = 0;
    memset(buffer_texto, 0, readbuffer);
    while ((leidos = mi_read(ruta, buffer_texto, offset, readbuffer)) > 0)
    {
        write(1, buffer_texto, leidos);
        memset(buffer_texto, 0, readbuffer);
        total_leidos += leidos;
        offset += readbuffer;
    }
    sprintf(string, "\ntotal_leidos %d\n", total_leidos);
    write(2, string, strlen(string));
    resultado = buscar_entrada(ruta, &p_inodo_dir, &p_inodo, &p_entrada, 0, 0);
    // if(resultado != EXIT_SUCCESS)
    //     return -1;
    switch (resultado)
    {
    case ERROR_EXTRAER_CAMINO:
        fprintf(stderr, "Error: Camino incorrecto\n");
        exit(EXIT_FAILURE);
        break;
    case ERROR_PERMISO_LECTURA:
        fprintf(stderr, "Error: Permiso denegado de lectura\n");
        exit(EXIT_FAILURE);
        break;
    }

    if (mi_stat_f(p_inodo, &datos_inodo) == -1)
        exit(EXIT_FAILURE);
    sprintf(string, "tamEnBytesLog %d\n", datos_inodo.tamEnBytesLog);
    write(2, string, strlen(string));
    if (bumount(nombre_dispositivo) == -1)
        exit(EXIT_FAILURE);

    return 0;
}
