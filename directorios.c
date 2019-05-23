#include "directorios.h"

int extraer_camino(const char *camino, char *inicial, char *final, char *tipo) // PREGUNTAR
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
        strcpy(final, "\0");
    }
    else
    {
        strcpy(tipo, "DIRECTORIO");
        strncpy(inicial, camino + 1, ret - (camino + 1));
        strcpy(final, ret);
    }
    return 0;
}
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos)
{
    char inicial[70]; //preguntar
    memset(inicial, 0, sizeof(inicial));
    char final[1024];
    memset(final, 0, sizeof(final));
    char tipo[128];
    memset(tipo, 0, sizeof(tipo));
    struct inodo inodo_dir;
    int numentradas;
    int nentrada;
    struct entrada entradas[BLOCKSIZE / tamEntrada];
    if (!strcmp(camino_parcial, "/"))
    {
        *p_inodo = 0;
        *p_entrada = 0;
        return 0;
    }
    if (extraer_camino(camino_parcial, inicial, final, tipo) == -1)
        return ERROR_EXTRAER_CAMINO;
    // fprintf(stderr, "[buscar_entrada()→ inicial: %s, final: %s, reservar: %d]\n", inicial, final, reservar);
    if (leer_inodo(*p_inodo_dir, &inodo_dir) == -1)
        return EXIT_FAILURE;

    if ((inodo_dir.permisos & 4) != 4)
    {
        // fprintf(stderr, "[buscar_entrada()→ El inodo %d no tiene permisos de lectura]\n", *p_inodo_dir);
        return ERROR_PERMISO_LECTURA;
    }
    numentradas = inodo_dir.tamEnBytesLog / tamEntrada;
    nentrada = 0;
    memset(entradas, 0, BLOCKSIZE);
    if (numentradas > 0)
    {
        //leer el primer bloque de entradas y mirarlas desde el buffer hasta acabar de leer el buffer
        if (mi_read_f(*p_inodo_dir, entradas, 0, BLOCKSIZE) == -1)
            return -1;
        while ((nentrada < numentradas) && strcmp(inicial, entradas[nentrada % (BLOCKSIZE / tamEntrada)].nombre))
        {
            nentrada++;
            if ((nentrada % (BLOCKSIZE / tamEntrada)) == 0 && nentrada < numentradas) // múltiplo del número de entradas en un bloque
            {
                //leer el siguiente bloque de entradas
                if (mi_read_f(*p_inodo_dir, entradas, nentrada * tamEntrada, BLOCKSIZE) == -1)
                    return -1;
            }
        }
    }
    if (nentrada == numentradas)
    {
        switch (reservar)
        {
        case 0: //modo consulta. Como no existe retornamos error
            return ERROR_NO_EXISTE_ENTRADA_CONSULTA;
        case 1: //modo escritura
            if (inodo_dir.tipo == 'f')
            {
                // fprintf(stderr, "[buscar_entrada()→ No se puede crear entrada en un fichero]\n");
                return ERROR_NO_SE_PUEDE_CREAR_ENTRADA_EN_UN_FICHERO;
            }
            if ((inodo_dir.permisos & 2) != 2)
            {
                // fprintf(stderr, "[buscar_entrada()→ El inodo %d no tiene permisos de escritura]\n", *p_inodo_dir);
                return ERROR_PERMISO_ESCRITURA;
            }
            strcpy(entradas[nentrada % (BLOCKSIZE / tamEntrada)].nombre, inicial);
            if (!strcmp(tipo, "DIRECTORIO")) //es un directorio
            {
                if (!strcmp(final, "/"))
                {
                    entradas[nentrada % (BLOCKSIZE / tamEntrada)].ninodo = reservar_inodo('d', permisos);
                    // fprintf(stderr, "[buscar_entrada()→ entrada.nombre: %s, entrada.ninodo: %d]\n", entradas[nentrada].nombre, entradas[nentrada].ninodo);
                    // fprintf(stderr, "[buscar_entrada()→ reservado inodo %d tipo d con permisos %d]\n", entradas[nentrada].ninodo, permisos);
                }
                else
                {
                    return ERROR_NO_EXISTE_DIRECTORIO_INTERMEDIO;
                }
            }
            else //es un fichero
            {
                entradas[nentrada % (BLOCKSIZE / tamEntrada)].ninodo = reservar_inodo('f', permisos);
                // fprintf(stderr, "[buscar_entrada()→ entrada.nombre: %s, entrada.ninodo: %d]\n", entradas[nentrada].nombre, entradas[nentrada].ninodo);
                // fprintf(stderr, "[buscar_entrada()→ reservado inodo %d tipo f con permisos %d]\n", entradas[nentrada].ninodo, permisos);
            }
            // if (mi_write_f(*p_inodo_dir, entradas, (nentrada / (BLOCKSIZE / tamEntrada)) * BLOCKSIZE, BLOCKSIZE) == -1)
            if (mi_write_f(*p_inodo_dir, &entradas[nentrada % (BLOCKSIZE / tamEntrada)], nentrada * tamEntrada, tamEntrada) == -1)
            {
                if (entradas[nentrada % (BLOCKSIZE / tamEntrada)].ninodo != -1)
                {
                    liberar_inodo(entradas[nentrada % (BLOCKSIZE / tamEntrada)].ninodo);
                }
                return EXIT_FAILURE;
            }
        }
    }
    if (!strcmp(final, "") || !strcmp(final, "/"))
    {
        if ((nentrada < numentradas) && (reservar == 1))
        {
            return ERROR_ENTRADA_YA_EXISTENTE;
        }
        *p_inodo = entradas[nentrada % (BLOCKSIZE / tamEntrada)].ninodo;
        *p_entrada = nentrada;
        return EXIT_SUCCESS;
    }
    else
    {
        *p_inodo_dir = entradas[nentrada % 16].ninodo;
        return (buscar_entrada(final, p_inodo_dir, p_inodo, p_entrada, reservar, permisos));
    }
}

void mostrar_error_buscar_entrada(int error)
{
    switch (error)
    {
    case -1:
        fprintf(stderr, "Error\n");
        break;
    case EXIT_FAILURE:
        fprintf(stderr, "Error\n");
        break;
    case ERROR_EXTRAER_CAMINO:
        fprintf(stderr, "Error: Camino incorrecto\n");
        break;
    case ERROR_PERMISO_LECTURA:
        fprintf(stderr, "Error: Permiso denegado de lectura\n");
        break;
    case ERROR_NO_EXISTE_ENTRADA_CONSULTA:
        fprintf(stderr, "Error: No existe la entrada\n");
        break;
    case ERROR_NO_SE_PUEDE_CREAR_ENTRADA_EN_UN_FICHERO:
        fprintf(stderr, "Error: No se puede crear una entrada en un fichero\n");
        break;
    case ERROR_PERMISO_ESCRITURA:
        fprintf(stderr, "Error: Permiso denegado de escritura\n");
        break;
    case ERROR_NO_EXISTE_DIRECTORIO_INTERMEDIO:
        fprintf(stderr, "Error: No existe directorio intermedio\n");
        break;
    case ERROR_ENTRADA_YA_EXISTENTE:
        fprintf(stderr, "Error: La entrada ya existe\n");
        break;
    }
}

int mi_creat(const char *camino, unsigned char permisos)
{
    int error;
    unsigned int p_inodo_dir, p_inodo, p_entrada;
    p_inodo_dir = 0;
    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 1, permisos)) != EXIT_SUCCESS)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }
    return 0;
}

int mi_dir(const char *camino, char *buffer)
{
    struct inodo inodo;
    unsigned int p_inodo_dir, p_inodo, p_entrada;
    int error;
    int numentradas;
    int nentrada;
    struct entrada entradas[BLOCKSIZE / tamEntrada];
    struct tm *tm;
    char tmp[100], tam[100], nom[100];
    p_inodo_dir = 0;
    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 0)) != EXIT_SUCCESS)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }
    if (leer_inodo(p_inodo, &inodo) == -1)
        return -1;
    if (inodo.tipo == 'f') // comprueba si es un fichero
    {
        strcat(buffer, "f\t");
        // permisos del inodo
        if (inodo.permisos & 4) //lectura
        {
            strcat(buffer, "r");
        }
        else
        {
            strcat(buffer, "-");
        }
        if (inodo.permisos & 2) //escritura
        {
            strcat(buffer, "w");
        }
        else
        {
            strcat(buffer, "-");
        }
        if (inodo.permisos & 1) //ejecución
        {
            strcat(buffer, "x\t");
        }
        else
        {
            strcat(buffer, "-\t");
        }
        // mtime del inodo
        tm = localtime(&inodo.mtime);
        sprintf(tmp, "%d-%02d-%02d %02d:%02d:%02d\t", tm->tm_year + 1900,
                tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
        strcat(buffer, tmp);
        // tamaño del inodo
        sprintf(tam, "%d\t", inodo.tamEnBytesLog);
        strcat(buffer, tam);
        // nombre de la entrada
        if (mi_read_f(p_inodo_dir, entradas, p_entrada * tamEntrada, tamEntrada) == -1)
            return -1;
        sprintf(nom, BOLD RED "%s\n" RESET, entradas[0].nombre);
        strcat(buffer, nom);
        return 1;
    }
    if ((inodo.permisos & 2) != 2) // comprueba el permiso de lectura
        return -1;

    numentradas = inodo.tamEnBytesLog / tamEntrada;
    nentrada = 0;
    if (numentradas > 0)
    {
        // lee el primer bloque de entradas
        if (mi_read_f(p_inodo, entradas, 0, BLOCKSIZE) == -1)
            return -1;
        while (nentrada < numentradas)
        {
            if (leer_inodo(entradas[nentrada % (BLOCKSIZE / tamEntrada)].ninodo, &inodo) == -1)
                return -1;
            // tipo de inodo
            if (inodo.tipo == 'f')
            {
                strcat(buffer, "f\t");
            }
            else
            {
                strcat(buffer, "d\t");
            }
            // permisos del inodo
            if (inodo.permisos & 4) //lectura
            {
                strcat(buffer, "r");
            }
            else
            {
                strcat(buffer, "-");
            }
            if (inodo.permisos & 2) //escritura
            {
                strcat(buffer, "w");
            }
            else
            {
                strcat(buffer, "-");
            }
            if (inodo.permisos & 1) //ejecución
            {
                strcat(buffer, "x\t");
            }
            else
            {
                strcat(buffer, "-\t");
            }
            // mtime del inodo
            tm = localtime(&inodo.mtime);
            sprintf(tmp, "%d-%02d-%02d %02d:%02d:%02d\t", tm->tm_year + 1900,
                    tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
            strcat(buffer, tmp);
            // tamaño del inodo
            sprintf(tam, "%d\t", inodo.tamEnBytesLog);
            strcat(buffer, tam);
            // nombre de la entrada
            if (inodo.tipo == 'f')
            {
                sprintf(nom, BOLD RED "%s\n" RESET, entradas[nentrada % (BLOCKSIZE / tamEntrada)].nombre);
            }
            else
            {
                sprintf(nom, BOLD BLU "%s\n" RESET, entradas[nentrada % (BLOCKSIZE / tamEntrada)].nombre);
            }
            strcat(buffer, nom);
            nentrada++;
            if (((nentrada) % (BLOCKSIZE / tamEntrada)) == 0)
            {
                // leer el siguiente bloque de entradas
                fprintf(stderr, "ESTOY EN LA ENTRADA %d Y VOY A LEER EL SIGUIENTE BLOQUE DE ENTRADAS\n", nentrada);
                if (mi_read_f(p_inodo, entradas, nentrada * tamEntrada, BLOCKSIZE) == -1)
                    return -1;
            }
        }
    }
    return numentradas;
}

int mi_chmod(const char *camino, unsigned char permisos)
{
    struct inodo inodo;
    unsigned int p_inodo_dir, p_inodo, p_entrada;
    int error;

    p_inodo_dir = 0;
    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 0)) != EXIT_SUCCESS)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }
    if (leer_inodo(p_inodo, &inodo) == -1)
        return -1;
    return mi_chmod_f(p_inodo, permisos);
}

int mi_stat(const char *camino, struct STAT *stat)
{

    struct inodo inodo;
    unsigned int p_inodo_dir, p_inodo, p_entrada;
    int error;

    p_inodo_dir = 0;

    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 0)) != EXIT_SUCCESS)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }
    if (leer_inodo(p_inodo, &inodo) == -1)
        return -1;
    // if ((inodo.permisos & 4) != 4)
    //     return -1;
    printf("Nº de inodo: %d\n", p_inodo);
    return mi_stat_f(p_inodo, stat);
}
int mi_read(const char *camino, void *buf, unsigned int offset, unsigned int nbytes)
{
    unsigned int p_inodo_dir, p_inodo, p_entrada;
    int error;
    p_inodo_dir = 0;
    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 0)) != EXIT_SUCCESS)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }
    return mi_read_f(p_inodo, buf, offset, nbytes);
}
int mi_write(const char *camino, const void *buf, unsigned int offset, unsigned int nbytes)
{
    unsigned int p_inodo_dir, p_inodo, p_entrada;
    int error;
    p_inodo_dir = 0;
    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 0)) != EXIT_SUCCESS)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }
    return mi_write_f(p_inodo, buf, offset, nbytes);
}
//Nivel 11
int mi_link(const char *camino1, const char *camino2)
{
    unsigned int p_inodo_dir1, p_inodo_dir2, p_inodo1, p_inodo2, p_entrada1, p_entrada2;
    struct inodo inodo1;
    int error;
    struct entrada entrada;
    p_inodo_dir1 = 0;
    p_inodo_dir2 = 0;
    if ((error = buscar_entrada(camino1, &p_inodo_dir1, &p_inodo1, &p_entrada1, 0, 0)) != EXIT_SUCCESS)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }
    if (leer_inodo(p_inodo1, &inodo1) == -1)
        return -1;
    if ((inodo1.permisos & 4) != 4)
    {
        fprintf(stderr, "Error: El inodo %d, de camino 1, no tiene permiso de lectura", p_inodo1);
        return -1;
    }
    if ((error = buscar_entrada(camino2, &p_inodo_dir2, &p_inodo2, &p_entrada2, 1, 6)) != EXIT_SUCCESS)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }
    //leer la entrada del camino2
    if (mi_read_f(p_inodo_dir2, &entrada, p_entrada2 * tamEntrada, tamEntrada) == -1)
        return -1;
    //crear el enlace
    entrada.ninodo = p_inodo1;
    if (mi_write_f(p_inodo_dir2, &entrada, p_entrada2 * tamEntrada, tamEntrada) == -1)
        return -1;
    if (liberar_inodo(p_inodo2) == -1)
        return -1;
    inodo1.nlinks++;
    inodo1.ctime = time(NULL);
    if (escribir_inodo(p_inodo1, inodo1) == -1)
        return -1;
    return EXIT_SUCCESS;
}

int mi_unlink(const char *camino)
{
    unsigned int p_inodo_dir, p_inodo, p_entrada;
    int error, numentradas;
    struct inodo inodo;
    struct entrada entrada;
    p_inodo_dir = 0;
    if ((error = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 0)) != EXIT_SUCCESS)
    {
        mostrar_error_buscar_entrada(error);
        return -1;
    }
    if (leer_inodo(p_inodo, &inodo) == -1)
        return -1;
    if (inodo.tipo == 'd' && inodo.tamEnBytesLog > 0)
    {
        fprintf(stderr, "Error: El directorio %s no está vacío\n", camino);
        exit(EXIT_FAILURE); //PREGUNTAR
    }
    if (leer_inodo(p_inodo_dir, &inodo) == -1)
        return -1;
    //Eliminar la entrada en el directorio
    numentradas = inodo.tamEnBytesLog / tamEntrada;
    if (p_entrada != (numentradas - 1))
    {
        if (mi_read_f(p_inodo_dir, &entrada, (numentradas - 1) * tamEntrada, tamEntrada) == -1)
            return -1;
        if (mi_write_f(p_inodo_dir, &entrada, p_entrada * tamEntrada, tamEntrada) == -1)
        {
            return -1;
        }
    }
    if (mi_truncar_f(p_inodo_dir, inodo.tamEnBytesLog - tamEntrada) == -1)
    {
        return -1;
    }
    if (leer_inodo(p_inodo, &inodo) == -1)
        return -1;
    inodo.nlinks--;
    if (inodo.nlinks == 0)
    {
        //Eliminar el inodo
        if (liberar_inodo(p_inodo) == -1)
        {
            return -1;
        }
    }
    else
    {
        inodo.ctime = time(NULL);
        if (escribir_inodo(p_inodo, inodo) == -1)
        {
            return -1;
        }
    }
    return EXIT_SUCCESS;
}