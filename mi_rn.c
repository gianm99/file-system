#include "directorios.h"
int main(int argc, char const *argv[])
{
   char nombre_dispositivo[1024], fuente[1024], destino[1024], *buffer1, *buffer2;
   char buffer_nombre[60];
   unsigned int p_inodo_dir, p_inodo, p_entrada;
   int error;

   if (argc != 4)
   {
      fprintf(stderr, "Sintaxis: mi_rn <disco> <fuente> <destino>\n");
      exit(EXIT_FAILURE);
   }
   strcpy(nombre_dispositivo, argv[1]);
   if (access(nombre_dispositivo, F_OK) == -1)
   {
      fprintf(stderr, "Error: No existe el disco\n");
      exit(EXIT_FAILURE);
   }
   strcpy(fuente, argv[2]);
   strcpy(destino, argv[3]);
   //comprobar que son del mismo tipo (fichero o directorio)
   if ((fuente[strlen(fuente) - 1] == '/' && destino[strlen(destino) - 1] != '/') ||
       (fuente[strlen(fuente) - 1] != '/' && destino[strlen(destino) - 1] == '/'))
   {
      fprintf(stderr, "Error: fuente y destino deben ser del mismo tipo (fichero o directorio)\n");
      exit(EXIT_FAILURE);
   }
   if (bmount(nombre_dispositivo) == -1)
   {
      fprintf(stderr, "Error: falló bmount\n");
      exit(EXIT_FAILURE);
   }
   //comprobar que tienen el mismo directorio padre
   if (fuente[strlen(fuente) - 1] == '/') //los dos son directorios
   {
      fuente[strlen(fuente) - 1] = 0;
      destino[strlen(destino) - 1] = 0;
   }
   buffer1 = strrchr(fuente, '/');
   buffer2 = strrchr(destino, '/');
   if (!buffer1 || !buffer2 || strlen(buffer2) > 60)
   {
      fprintf(stderr, "Error: caminos incorrectos\n");
      exit(EXIT_FAILURE);
   }
   if (strncmp(fuente, destino, strlen(fuente) - strlen(buffer1)))
   {
      fprintf(stderr, "Error: los caminos no pertenecen al mismo directorio\n");
      exit(EXIT_FAILURE);
   }
   //CAMBIAR ESTO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   p_inodo_dir = 0;
   if ((error = buscar_entrada(destino, &p_inodo_dir, &p_inodo, &p_entrada, 0, 0)) == EXIT_SUCCESS)
   {
      fprintf(stderr, "Error: ya existe el camino destino\n");
      exit(EXIT_FAILURE);
   }
   p_inodo_dir = 0;
   if ((error = buscar_entrada(fuente, &p_inodo_dir, &p_inodo, &p_entrada, 0, 0)) != EXIT_SUCCESS)
   {
      mostrar_error_buscar_entrada(error);
      exit(EXIT_FAILURE);
   }
   //cambiar el nombre de la entrada en el directorio padre
   memset(buffer_nombre, 0, sizeof(buffer_nombre));
   strcpy(buffer_nombre, buffer2 + 1);
   mi_write_f(p_inodo_dir, buffer_nombre, p_entrada * sizeof(struct entrada), sizeof(buffer_nombre));
   if (bumount() == -1)
   {
      fprintf(stderr, "Error: falló bumount\n");
      exit(EXIT_FAILURE);
   }
}