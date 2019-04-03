#include "ficheros_basico.h"
void datos_inodo(struct inodo inodo);
int main(int argc, char const *argv[])
{
    char nombre_dispositivo[1024];
    // struct inodo inodo_reservado;
    // int ninodo;
    struct superbloque SB;
    if (argc != 2)
    {
        fprintf(stderr, "Sintaxis: ./leer_sf <nombre_dispositivo>\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nombre_dispositivo, argv[1]);
    if (access(nombre_dispositivo, F_OK) == -1)
    {
        fprintf(stderr, "ERROR: No existe el archivo %s\n",nombre_dispositivo);
        exit(EXIT_FAILURE);
    }
    if (bmount(nombre_dispositivo) == -1)
        exit(EXIT_FAILURE);
    if (bread(posSB, &SB) == -1)
        exit(EXIT_FAILURE);
    printf("DATOS DEL SUPERBLOQUE\n");
    printf("posPrimerBloqueMB = %d\n", SB.posPrimerBloqueMB);
    printf("posUltimoBloqueMB = %d\n", SB.posUltimoBloqueMB);
    printf("posPrimerBloqueAI = %d\n", SB.posPrimerBloqueAI);
    printf("posUltimoBloqueAI = %d\n", SB.posUltimoBloqueAI);
    printf("posPrimerBloqueDatos = %d\n", SB.posPrimerBloqueDatos);
    printf("posUltimoBloqueDatos = %d\n", SB.posUltimoBloqueDatos);
    printf("posInodoRaiz = %d\n", SB.posInodoRaiz);
    printf("posPrimerInodoLibre = %d\n", SB.posPrimerInodoLibre);
    printf("cantBloquesLibres = %d\n", SB.cantBloquesLibres);
    printf("cantInodosLibres = %d\n", SB.cantInodosLibres);
    printf("totBloques = %d\n", SB.totBloques);
    printf("totInodos = %d\n\n", SB.totInodos);

    // /* RESERVA DE INODO Y TRADUCCION DE BLOQUES LOGICOS */
    // ninodo = reservar_inodo('f', 6);
    // if (ninodo == -1)
    //     exit(EXIT_FAILURE);
    // printf("INODO %d. TRADUCCION DE LOS BLOQUES LOGICOS 8, 204, 30.004, 400.004 y 16.843.019 \n\n", ninodo);
    // if (traducir_bloque_inodo(ninodo, (unsigned int)8, 1) == -1)
    //     exit(EXIT_FAILURE);
    // if (traducir_bloque_inodo(ninodo, (unsigned int)204, 1) == -1)
    //     exit(EXIT_FAILURE);
    // if (traducir_bloque_inodo(ninodo, (unsigned int)30004, 1) == -1)
    //     exit(EXIT_FAILURE);
    // if (traducir_bloque_inodo(ninodo, (unsigned int)400004, 1) == -1)
    //     exit(EXIT_FAILURE);
    // if (traducir_bloque_inodo(ninodo, (unsigned int)16843019, 1) == -1)
    //     exit(EXIT_FAILURE);
    // puts("\n");
    // printf("DATOS DEL INODO RESERVADO %d\n", ninodo);
    // if (leer_inodo(ninodo, &inodo_reservado) == -1)
    // {
    //     fprintf(stderr, "ERROR: falló leer_inodo\n");
    //     exit(EXIT_FAILURE);
    // }
    // datos_inodo(inodo_reservado);
    // if (bread(posSB, &SB) == -1)
    //     return -1;
    // puts("\n");
    // printf("SB.posPrimerInodoLibre = %d\n\n", SB.posPrimerInodoLibre);

    // printf("LIBERAMOS EL INODO RESERVADO EN EL NIVEL ANTERIOR Y TODOS SUS BLOQUES\n");
    // if (liberar_inodo(ninodo) == -1)
    //     exit(EXIT_FAILURE);

    // if (bread(posSB, &SB) == -1)
    //     return -1;
    // printf("SB.posPrimerInodoLibre = %d\n", SB.posPrimerInodoLibre);
    if (bumount(nombre_dispositivo) == -1)
        exit(EXIT_FAILURE);
    return 0;
}