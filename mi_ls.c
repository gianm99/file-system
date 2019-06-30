#include "directorios.h"

int main(int argc, char const *argv[])
{
   char nombre_dispositivo[1024], ruta[1024], buffer[5000];
   int numentradas, long_version = 1;
   //Número de argumentos incorrecto
   if (argc != 3 && argc != 4)
   {
      fprintf(stderr, "Sintaxis: mi_ls <disco> </ruta> || mi_ls <disco> </ruta> -s\n");
      exit(EXIT_FAILURE);
   }
   //Comprobar opción de visualización
   if (argc == 4)
   {
      if (strcmp(argv[3], "-s"))
      {
         fprintf(stderr, "Error: No existe esa opción\n");
         fprintf(stderr, "Sintaxis: mi_ls <disco> </ruta> || mi_ls <disco> </ruta> -s\n");
         exit(EXIT_FAILURE);
      }
      else
      {
         long_version = 0;
      }
   }
   strcpy(nombre_dispositivo, argv[1]);
   if (access(nombre_dispositivo, F_OK) == -1)
   {
      fprintf(stderr, "ERROR: No existe el archivo\n");
      exit(EXIT_FAILURE);
   }
   if (bmount(nombre_dispositivo) == -1)
   {
      fprintf(stderr, "ERROR: falló bmount\n");
      exit(EXIT_FAILURE);
   }
   strcpy(ruta, argv[2]);
   numentradas = mi_dir(ruta, buffer, long_version);
   if (ruta[strlen(ruta) - 1] == '/' && long_version == 1)
   {
      printf("Total entradas: %d\n", numentradas);
   }
   if (numentradas == 0)
   {
      printf("\n");
      exit(EXIT_SUCCESS);
   }
   if (long_version || ruta[strlen(ruta) - 1] != '/')
   {
      printf("\nTipo\tPermisos\tmTime\t\tTamaño\tNombre\n");
      printf("---------------------------------------------------------\n");
   }
   printf("%s\n", buffer);
   if (bumount() == -1)
   {
      fprintf(stderr, "ERROR: falló bumount\n");
      exit(EXIT_FAILURE);
   }
   return EXIT_SUCCESS;
}