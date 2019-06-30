#include "directorios.h"
#define BUFFERSIZE 8000
int main(int argc, char const *argv[])
{
   char nombre_dispositivo[1024], ruta_fichero[1024], ruta_destino[1024], *buffer;
   char buffer_texto[BUFFERSIZE];
   int offset, leidos;
   struct STAT stat;
   if (argc != 4)
   {
      fprintf(stderr, "Sintaxis: mi_cp_f <disco> </ruta_fichero> </ruta_destino/>\n");
      return -1;
   }
   strcpy(nombre_dispositivo, argv[1]);
   if (access(nombre_dispositivo, F_OK) == -1)
   {
      fprintf(stderr, "Error: No existe el disco\n");
      exit(EXIT_FAILURE);
   }
   strcpy(ruta_fichero, argv[2]);
   strcpy(ruta_destino, argv[3]);
   if (ruta_fichero[strlen(ruta_fichero) - 1] == '/')
   {
      fprintf(stderr, "Error: No se puede copiar un directorio\n");
      exit(EXIT_FAILURE);
   }
   else if (ruta_destino[strlen(ruta_destino) - 1] != '/')
   {
      fprintf(stderr, "Error: La ruta destino tiene que ser un directorio\n");
      exit(EXIT_FAILURE);
   }
   if (bmount(nombre_dispositivo) == -1)
      exit(EXIT_FAILURE);
   //crear el fichero en el directorio destino
   if (mi_stat(ruta_fichero, &stat) == -1)
      exit(EXIT_FAILURE);
   buffer = strrchr(ruta_fichero, '/') + 1;
   strcat(ruta_destino, buffer);
   if (mi_creat(ruta_destino, stat.permisos) == -1)
      exit(EXIT_FAILURE);
   //copia el contenido al nuevo fichero
   offset = 0;
   leidos = 0;
   memset(buffer_texto, 0, BUFFERSIZE);
   while ((leidos = mi_read(ruta_fichero, buffer_texto, offset, BUFFERSIZE)) > 0)
   {
      if (mi_write(ruta_destino, buffer_texto, offset, leidos) == -1)
         exit(EXIT_FAILURE);
      memset(buffer_texto, 0, BUFFERSIZE);
      offset += BUFFERSIZE;
   }

   if (bumount() == -1)
      exit(EXIT_FAILURE);
}
