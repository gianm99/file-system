#include "ficheros_basico.h"
#include "semaforo_mutex_posix.h"

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
   if (descriptor > 0)
   {
      close(descriptor);
   }
   if ((descriptor = open(camino, O_RDWR | O_CREAT, 0666)) == -1)
   {
      fprintf(stderr, "Error: bloques.c → bmount() → open()\n");
   }
   if (!mutex) //mutex == 0
   {
      //el semáforo es único y sólo se ha de inicializar una vez en nuestro sistema (lo hace el padre)
      mutex = initSem(); //lo inicializa a 1
      if (mutex == SEM_FAILED)
      {
         return -1;
      }
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
   descriptor = close(descriptor);
   // hay que asignar el resultado de la operación a la variable ya que bmount() la utiliza
   if (descriptor == -1)
   {
      fprintf(stderr, "Error: bloques.c → bumount() → close(): %d: %s\n", errno, strerror(errno));
      return -1;
   }
   deleteSem(); // borramos semaforo
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

/*-Función: mi_wait_Sem.
**-Descripción: Espera al semáforo si no está ya dentro de sección crítica
**-Parámetros:
**-Return: 
*/
void mi_waitSem()
{
   if (!inside_sc)
   {
      waitSem(mutex);
   }
   inside_sc++;
}

/*-Función: mi_signalSem
**-Descripción: Sale de una sección crítica.
**-Parámetros:
**-Return: 
*/
void mi_signalSem()
{
   inside_sc--;
   if (!inside_sc)
   {
      signalSem(mutex);
   }
}
