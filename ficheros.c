#include "ficheros.h"

/************************************************** NIVEL 6 **************************************************/

/*  -Función: mi_write_f.
	 -Descripción: Escribe el buffer en un fichero/directorio.
	 -Parámetros: Inodo del fichero/directorio, buffer que se escribe, offset inicial, número
	 de bytes a escribir.
	 -Return: Cantidad de bytes escritos o -1 si hay un error o la operación no está permitida.
*/
int mi_write_f(unsigned int ninodo, const void *buf_original, unsigned int offset, unsigned int nbytes)
{
   unsigned int primerBLogico, ultimoBLogico, desp1, desp2, bfisico, ultimobyte;
   char buf_bloque[BLOCKSIZE];
   struct inodo inodo;

   if (leer_inodo(ninodo, &inodo) == -1)
      return 0;
   if ((inodo.permisos & 2) != 2)
   {
      fprintf(stderr, "Error: Permiso denegado de escritura\n");
      return 0;
   }
   primerBLogico = offset / BLOCKSIZE;
   ultimoBLogico = (offset + nbytes - 1) / BLOCKSIZE;
   desp1 = offset % BLOCKSIZE;
   ultimobyte = (offset + nbytes - 1);
   if (primerBLogico == ultimoBLogico) // Un solo bloque involucrado
   {
      mi_waitSem();
      bfisico = traducir_bloque_inodo(ninodo, primerBLogico, 1);
      mi_signalSem();
      if (bread(bfisico, buf_bloque) == -1)
         return 0;
      memcpy(buf_bloque + desp1, buf_original, nbytes);
      if (bwrite(bfisico, buf_bloque) == -1)
         return 0;
   }
   else // Más de un bloque involucrado
   {
      mi_waitSem();
      bfisico = traducir_bloque_inodo(ninodo, primerBLogico, 1);
      mi_signalSem();
      if (bread(bfisico, buf_bloque) == -1)
         return 0;
      memcpy(buf_bloque + desp1, buf_original, BLOCKSIZE - desp1);
      if (bwrite(bfisico, buf_bloque) == -1)
         return 0;
      for (int i = primerBLogico + 1; i < ultimoBLogico; i++)
      {
         mi_waitSem();
         bfisico = traducir_bloque_inodo(ninodo, i, 1);
         mi_signalSem();
         if (bwrite(bfisico, buf_original + (BLOCKSIZE - desp1) + (i - primerBLogico - 1) * BLOCKSIZE) == -1)
            return 0;
      }
      mi_waitSem();
      bfisico = traducir_bloque_inodo(ninodo, ultimoBLogico, 1);
      mi_signalSem();
      if (bread(bfisico, buf_bloque) == -1)
         return 0;
      desp2 = ultimobyte % BLOCKSIZE;
      memcpy(buf_bloque, buf_original + (nbytes - desp2 - 1), desp2 + 1);
      if (bwrite(bfisico, buf_bloque) == -1)
         return 0;
   }
   mi_waitSem();
   if (leer_inodo(ninodo, &inodo) == -1)
      return 0;
   inodo.mtime = time(NULL);
   if ((offset + nbytes) >= inodo.tamEnBytesLog)
   {
      inodo.ctime = time(NULL);
      inodo.tamEnBytesLog = offset + nbytes;
   }
   if (escribir_inodo(ninodo, inodo) == -1)
      return 0;
   mi_signalSem();
   return nbytes;
}
/*  -Función: mi_read_f.
	 -Descripción: Lee información de un fichero/directorio y la almacena en un buffer de memoria.
	 -Parámetros: Inodo del fichero/directorio, buffer en el que se guarda el resultado, offset inicial, número
	 de bytes a leer.
	 -Return: Cantidad de bytes leídos o -1 si hay un error o la operación no está permitida.
*/
int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes)
{
   unsigned int primerBLogico, ultimoBLogico, desp1, desp2, leidos, ultimobyte;
   char buf_bloque[BLOCKSIZE];
   int bfisico;
   struct inodo inodo;
   if (leer_inodo(ninodo, &inodo) == -1)
      return -1;
   if ((inodo.permisos & 4) != 4)
   {
      fprintf(stderr, "Error: permiso denegado de lectura\n");
      return -1;
   }
   if (offset >= inodo.tamEnBytesLog)
   {
      leidos = 0;
      return leidos; // No podemos leer nada
   }
   if ((offset + nbytes) >= inodo.tamEnBytesLog)
   {
      nbytes = inodo.tamEnBytesLog - offset; // Leemos hasta el EOF
   }
   primerBLogico = offset / BLOCKSIZE;
   ultimoBLogico = (offset + nbytes - 1) / BLOCKSIZE;
   desp1 = offset % BLOCKSIZE;
   ultimobyte = (offset + nbytes - 1);
   if (primerBLogico == ultimoBLogico) // Un solo bloque involucrado
   {
      bfisico = traducir_bloque_inodo(ninodo, primerBLogico, 0);
      if (bfisico != -1)
      {
         if (bread(bfisico, buf_bloque) == -1)
            return -1;
         memcpy(buf_original, buf_bloque + desp1, nbytes);
      }
      leidos = nbytes;
   }
   else // Más de un bloque involucrado
   {
      desp2 = (ultimobyte % BLOCKSIZE);
      bfisico = traducir_bloque_inodo(ninodo, primerBLogico, 0);
      if (bfisico != -1)
      {
         if (bread(bfisico, buf_bloque) == -1)
            return -1;
         memcpy(buf_original, buf_bloque + desp1, BLOCKSIZE - desp1);
      }
      leidos = BLOCKSIZE - desp1;
      for (int i = primerBLogico + 1; i < ultimoBLogico; i++)
      {
         bfisico = traducir_bloque_inodo(ninodo, i, 0);
         if (bfisico != -1)
         {
            if (bread(bfisico, buf_bloque) == -1)
               return -1;
            memcpy(buf_original + leidos, buf_bloque, BLOCKSIZE);
         }
         leidos += BLOCKSIZE;
      }
      bfisico = traducir_bloque_inodo(ninodo, ultimoBLogico, 0);
      if (bfisico != -1)
      {
         if (bread(bfisico, buf_bloque) == -1)
            return -1;
         memcpy(buf_original + leidos, buf_bloque, desp2 + 1);
      }
      leidos += desp2 + 1;
   }
   if (leer_inodo(ninodo, &inodo) == -1)
      return -1;
   inodo.atime = time(NULL);
   if (escribir_inodo(ninodo, inodo) == -1)
      return -1;
   return leidos;
}

/*  -Función: mi_stat_f.
	 -Descripción: Devuelve la metainformación de un fichero/directorio.
	 -Parámetros: Inodo del fichero/directorio, tipo estructurado denominado STAT similar a INODO pero sin punteros.
	 -Return: 0 si funciona correctamente o -1 si hay un error o la operación no está permitida.
*/
int mi_stat_f(unsigned int ninodo, struct STAT *p_stat)
{
   struct inodo inodo;
   if (leer_inodo(ninodo, &inodo) == -1)
   {
      return -1;
   }
   p_stat->tipo = inodo.tipo;
   p_stat->permisos = inodo.permisos;
   p_stat->atime = inodo.atime;
   p_stat->mtime = inodo.mtime;
   p_stat->ctime = inodo.ctime;
   p_stat->nlinks = inodo.nlinks;
   p_stat->tamEnBytesLog = inodo.tamEnBytesLog;
   p_stat->numBloquesOcupados = inodo.numBloquesOcupados;
   return 0;
}

/*  -Función: mi_chmod_f.
	 -Descripción: Cambia los permisos de un fichero/directorio.
	 -Parámetros: Inodo del fichero/directorio, permisos a los que se quiere actualizar.
	 -Return: 0 si funciona correctamente o -1 si hay un error o la operación no está permitida.
*/
int mi_chmod_f(unsigned int ninodo, unsigned char permisos)
{
   struct inodo inodo;
   if (leer_inodo(ninodo, &inodo) == -1)
      return -1;
   inodo.permisos = permisos;
   inodo.ctime = time(NULL);
   if (escribir_inodo(ninodo, inodo) == -1)
      return -1;
   return 0;
}

/*  -Función: mi_truncar_f.
	 -Descripción: Trunca un fichero/directorio a los bytes indicados.
	 -Parámetros: Inodo del fichero/directorio, número de bytes a los que se quiere truncar.
	 -Return: Devuelve la cantidad de bloques liberados o -1 si hay un error o la operación no está permitida.
*/
int mi_truncar_f(unsigned int ninodo, unsigned int nbytes)
{
   int nblogico, liberados;
   struct inodo inodo;
   if (leer_inodo(ninodo, &inodo) == -1)
      return -1;
   if (nbytes > inodo.tamEnBytesLog)
   {
      fprintf(stderr, "No se puede truncar más allá del EOF: %d\n", inodo.tamEnBytesLog);
      return -1;
   }
   if ((inodo.permisos & 2) != 2)
   {
      fprintf(stderr, "Error: permiso denegado de escritura\n");
      return -1;
   }
   if ((nbytes % BLOCKSIZE) == 0)
   {
      nblogico = nbytes / BLOCKSIZE;
   }
   else
   {
      nblogico = nbytes / BLOCKSIZE + 1;
   }
   liberados = liberar_bloques_inodo(ninodo, nblogico);
   if (liberados == -1)
      return -1;
   inodo.mtime = time(NULL);
   inodo.ctime = time(NULL);
   inodo.tamEnBytesLog = nbytes;
   inodo.numBloquesOcupados -= liberados;
   if (escribir_inodo(ninodo, inodo) == -1)
      return -1;
   return liberados;
}

/****************************************** FUNCIONES AUXILIARES **************************************************/

/*	-Función: datos_inodo.
	-Descripción: Imprime los valores de un inodo.
	-Parámetros: Inodo que se va a mostrar
	-Return:
*/
void datos_inodo(struct inodo inodo)
{
   struct tm *ts;
   char atime[80];
   char mtime[80];
   char ctime[80];
   ts = localtime(&inodo.atime);
   strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
   ts = localtime(&inodo.mtime);
   strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
   ts = localtime(&inodo.ctime);
   strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
   printf("tipo: %c\n", inodo.tipo);
   printf("permisos: %d\n", inodo.permisos);
   printf("atime: %s\n", atime);
   printf("ctime: %s\n", ctime);
   printf("mtime: %s\n", mtime);
   printf("nlinks: %u\n", inodo.nlinks);
   printf("tamEnBytesLog: %u\n", inodo.tamEnBytesLog);
   printf("numBloquesOcupados: %u\n", inodo.numBloquesOcupados);
}

/*	-Función: datos_STAT.
	-Descripción: Imprime los valores de un inodo.
	-Parámetros: STAT del inodo que se va a mostrar.
	-Return:
*/
void datos_STAT(struct STAT STAT)
{
   struct tm *ts;
   char atime[80];
   char mtime[80];
   char ctime[80];
   ts = localtime(&STAT.atime);
   strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
   ts = localtime(&STAT.mtime);
   strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
   ts = localtime(&STAT.ctime);
   strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
   fprintf(stderr, "tipo=%c\n", STAT.tipo);
   fprintf(stderr, "permisos=%d\n", STAT.permisos);
   fprintf(stderr, "atime: %s\n", atime);
   fprintf(stderr, "ctime: %s\n", ctime);
   fprintf(stderr, "mtime: %s\n", mtime);
   fprintf(stderr, "nlinks=%u\n", STAT.nlinks);
   fprintf(stderr, "tamEnBytesLog=%u\n", STAT.tamEnBytesLog);
   fprintf(stderr, "numBloquesOcupados=%u\n\n", STAT.numBloquesOcupados);
}
