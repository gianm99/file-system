#include "directorios.h"

int main(int argc, char const *argv[])
{
   char nombre_dispositivo[1024], ruta[1024], ruta_fic_ext[1024];
   unsigned int offset, nbytes;
   int escritos, fdescriptor;
   struct stat st;
   if (argc != 5)
   {
      fprintf(stderr, "Sintaxis: mi_escribir_f <disco> </ruta_fichero> <nombre_fichero_externo> <offset>\n");
      exit(EXIT_FAILURE);
   }
   strcpy(nombre_dispositivo, argv[1]);
   strcpy(ruta, argv[2]);
   strcpy(ruta_fic_ext, argv[3]);
   offset = atoi(argv[4]);
   //comprueba que existe el disco
   if (access(nombre_dispositivo, F_OK) == -1)
   {
      fprintf(stderr, "Error: No existe el disco\n");
      exit(EXIT_FAILURE);
   }
   //comprueba que no es un directorio
   if (ruta[strlen(ruta) - 1] == '/')
   {
      fprintf(stderr, "Error: ruta de directorio");
      exit(EXIT_FAILURE);
   }
   //obtenemos el texto y su longitud
   fdescriptor = open(ruta_fic_ext, O_RDONLY);
   if (fdescriptor == -1)
   {
      fprintf(stderr, "Error: Hubo un problema con el fichero de texto\n");
      exit(EXIT_FAILURE);
   }
   if (stat(ruta_fic_ext, &st))
      exit(EXIT_FAILURE);
   nbytes = (int)st.st_size;
   fprintf(stderr, "Longitud texto: %d\n", nbytes);
   char buffer[nbytes];
   if (read(fdescriptor, buffer, nbytes) < 0)
   {
      fprintf(stderr, "Error: Hubo un problema leyendo fichero de texto\n");
      exit(EXIT_FAILURE);
   }
   if (bmount(nombre_dispositivo) == -1)
   {
      fprintf(stderr, "Error: falló bmount\n");
      exit(EXIT_FAILURE);
   }
   escritos = mi_write(ruta, buffer, offset, nbytes);
   fprintf(stderr, "Bytes escritos: %d\n", escritos);
   if (bumount(nombre_dispositivo) == -1)
      exit(EXIT_FAILURE);
}