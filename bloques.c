#include "ficheros_basico.h"

/************************************************** NIVEL 1 **************************************************/
static sem_t *mutex;
static unsigned int inside_sc = 0;
static int descriptor = 0;
/*-Función: Bmount.
**-Descripción: Monta el fichero que se utilizará como dispositivo virtual.
**-Parámetros: Tiene como parámetro un puntero a la dirección donde se crea el
** fichero.
**-Return: Devuelve el fichero en sí en la variable "descriptor", en caso
** contrario -1.
*/
int bmount(const char *camino)
{
   mutex = initSem();
   descriptor = open(camino, O_RDWR | O_CREAT, 0666);
   if (descriptor < 0)
   {
      fprintf(stderr, "ERROR %d: %s\n", errno, strerror(errno));
      return -1;
   }
   return descriptor;
}
/*-Función: Bumount.
**-Descripción: Desmonta el fichero creado con anterioridad.
**-Parámetros: No recibe parámetros.
**-Return: Devuelve 0 si el fichero se ha cerrado correctamente, en caso
** contrario -1.
*/
int bumount()
{
   deleteSem();
   if (close(descriptor) < 0)
   {
      fprintf(stderr, "ERROR %d: %s\n", errno, strerror(errno));
      return -1;
   }
   return 0;
}
/*-Función: bwrite
**-Descripción: Escribe en uno de los bloques apuntados a la dirección que
** indica *buf.
**-Parámetros: Tiene como parámetros el número de bloque al que acceder y un
** puntero a la dirección del buffer.
**-Return: Devuelve el números de bytes en los que se ha escrito, en caso
** contrario -1.
*/
int bwrite(unsigned int nbloque, const void *buf)
{
   int nbytes;
   if (lseek(descriptor, BLOCKSIZE * nbloque, SEEK_SET) < 0)
   {
      fprintf(stderr, "ERROR %d: %s\n", errno, strerror(errno));
      return -1;
   }
   if ((nbytes = write(descriptor, buf, BLOCKSIZE)) < 0)
   {
      fprintf(stderr, "ERROR %d: %s\n", errno, strerror(errno));
      return -1;
   }
   return nbytes;
}
/*-Función: Bread.
**-Descripción: Lee el bloque especificado y copia su contenido en el buffer
** que indica *buff.
**-Parámetros: Tiene como parámetros el número de bloque que tiene que leer y
** un puntero a la dirección del buffer.
**-Return: Devuelve el número de bytes leidos o -1 en caso de error
*/
int bread(unsigned int nbloque, void *buf)
{
   int nbytes;
   if (lseek(descriptor, BLOCKSIZE * nbloque, SEEK_SET) < 0)
   {
      fprintf(stderr, "ERROR %d: %s\n", errno, strerror(errno));
      return -1;
   }
   if ((nbytes = read(descriptor, buf, BLOCKSIZE)) < 0)
   {
      fprintf(stderr, "ERROR %d: %s\n", errno, strerror(errno));
      return -1;
   }
   return nbytes;
}

void mi_waitSem()
{
   if (!inside_sc)
   {
      waitSem(mutex);
   }
   inside_sc++;
}

void mi_signalSem()
{
   inside_sc--;
   if (!inside_sc)
   {
      signalSem(mutex);
   }
}
