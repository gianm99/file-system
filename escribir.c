#include "ficheros.h"
#define OFFSET1 0
#define OFFSET2 5120
#define OFFSET3 256000
#define OFFSET4 30720000
#define OFFSET5 71680000
#define NOFFSETS 5

int main(int argc, char const *argv[])
{
    char nombre_dispositivo[1024];
    int diferentes_inodos, escritos, ninodo, nbytes;
    struct STAT datos_inodo;
    unsigned int offsets[] = {OFFSET1, OFFSET2, OFFSET3, OFFSET4, OFFSET5};
    if (argc != 4)
    {
        fprintf(stderr, "Sintaxis: escribir <nombre_dispositivo> <\"$(cat fichero)\"> <diferentes_inodos>\n");
        fprintf(stderr, "Offsets: 0, 5120, 256000, 30720000, 71680000\n");
        fprintf(stderr, "Si diferentes_inodos=0 se reserva un solo inodo para todos los offsets\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nombre_dispositivo, argv[1]);
    if (access(nombre_dispositivo, F_OK) == -1)
    {
        fprintf(stderr, "ERROR: No existe el archivo %s\n", nombre_dispositivo);
        exit(EXIT_FAILURE);
    }
    nbytes = strlen(argv[2]); // Longitud del texto
    char buffer[nbytes];      // Buffer del texto
    strcpy(buffer, argv[2]);
    diferentes_inodos = atoi(argv[3]);
    fprintf(stderr, "longitud del texto: %d\n\n", nbytes);
    if (bmount(nombre_dispositivo) == -1)
        exit(EXIT_FAILURE);
    if (diferentes_inodos == 0)
    {
        ninodo = reservar_inodo('f', 6);
        if (ninodo == -1)
            exit(EXIT_FAILURE);
        for (int i = 0; i < NOFFSETS; i++)
        {
            fprintf(stderr, "Nº inodo reservado: %u\n", ninodo);
            fprintf(stderr, "Offset: %u\n", offsets[i]);
            escritos = mi_write_f(ninodo, buffer, offsets[i], nbytes);
            if (escritos == -1)
                exit(EXIT_FAILURE);
            fprintf(stderr, "Bytes escritos: %d\n\n", escritos);
            if (mi_stat_f(ninodo, &datos_inodo) == -1)
                exit(EXIT_FAILURE);
            fprintf(stderr, "DATOS INODO %d:\n", ninodo);
            datos_STAT(datos_inodo);
        }
    }
    else if (diferentes_inodos == 1)
    {
        for (int i = 0; i < NOFFSETS; i++)
        {
            ninodo = reservar_inodo('f', 6);
            if (ninodo == -1)
                exit(EXIT_FAILURE);
            fprintf(stderr, "Nº inodo reservado: %u\n", ninodo);
            fprintf(stderr, "Offset: %u\n", offsets[i]);
            escritos = mi_write_f(ninodo, buffer, offsets[i], nbytes);
            if (escritos == -1)
                exit(EXIT_FAILURE);
            fprintf(stderr, "Bytes escritos: %d\n\n", escritos);
            if (mi_stat_f(ninodo, &datos_inodo) == -1)
                exit(EXIT_FAILURE);
            fprintf(stderr, "DATOS INODO %d:\n", ninodo);
            datos_STAT(datos_inodo);
        }
    }
    else
    {
        fprintf(stderr, "Valor incorrecto para <diferentes_inodos>\n");
        exit(EXIT_FAILURE);
    }
    if (bumount(nombre_dispositivo) == -1)
        exit(EXIT_FAILURE);
    return 0;
}