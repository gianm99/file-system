#include "directorios.h"

int main(int argc, char const *argv[])
{
   char nombre_dispositivo[1024], ruta[1024];
   struct STAT stat;

   if (argc != 3)
   {
      fprintf(stderr, "Sintaxis: mi_rmdir <disco> </ruta_directorio_vacio/>\n");
      exit(EXIT_FAILURE);
   }

   strcpy(nombre_dispositivo, argv[1]);
   strcpy(ruta, argv[2]);
   if (access(nombre_dispositivo, F_OK) == -1)
   {
      fprintf(stderr, "Error: no existe el disco\n");
      exit(EXIT_FAILURE);
   }

   if (ruta[strlen(ruta) - 1] != '/')
   {
      fprintf(stderr, "Error: no es un directorio\n");
      exit(EXIT_FAILURE);
   }

   if (bmount(nombre_dispositivo) == -1)
   {
      fprintf(stderr, "Error: falló bmount\n");
      exit(EXIT_FAILURE);
   }
   mi_stat(ruta, &stat);
   if (stat.tamEnBytesLog > 0)
   {
      fprintf(stderr, "Error: el directorio no está vacío\n");
      exit(EXIT_FAILURE);
   }
   if (mi_unlink(ruta) == -1)
   {
      fprintf(stderr, "Error: falló mi_unlink\n");
      exit(EXIT_FAILURE);
   }
   if (bumount() == -1)
   {
      fprintf(stderr, "Error: falló bumount\n");
      exit(EXIT_FAILURE);
   }
}