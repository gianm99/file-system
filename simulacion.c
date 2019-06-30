#include "simulacion.h"
int acabados = 0;
int main(int argc, char const *argv[])
{
   char nombre_dispositivo[1024], buf[80], camino[1024];
   time_t now;
   struct tm ts;
   pid_t pid;
   struct REGISTRO registro;
   if (argc != 2)
   {
      fprintf(stderr, "Sintaxis:./simulacion <disco>\n");
      exit(EXIT_FAILURE);
   }
   strcpy(nombre_dispositivo, argv[1]);
   if (access(nombre_dispositivo, F_OK) == -1)
   {
      fprintf(stderr, "ERROR: No existe el archivo\n");
      exit(EXIT_FAILURE);
   }
   bmount(nombre_dispositivo);
   // Tiempo actual
   time(&now);
   // Formato de tiempo, "aaaammddhhmmss"
   ts = *localtime(&now);
   strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &ts);
   sprintf(camino, "/simul_%s/", buf);
   if (mi_creat(camino, 6) == -1)
   {
      fprintf(stderr, "ERROR: falló mi_creat\n");
      bumount();
   }
   signal(SIGCHLD, reaper);
   printf("*** Simulación de %d procesos realizando cada uno %d escrituras ***\n", NUMPROCESOS, NUMESCRITURAS);
   printf("Directorio simulación: %s\n", camino);
   for (int i = 1; i <= NUMPROCESOS; i++)
   {
      pid = fork();
      if (pid == 0) // proceso hijo
      {
         char camino_fic[1024];
         char buffer_directorio[1024];
         int pid_directorio = getpid();
         bmount(nombre_dispositivo);
         memset(buffer_directorio, 0, strlen(buffer_directorio));
         sprintf(buffer_directorio, "%sproceso_%d/", camino, pid_directorio);
         if (mi_creat(buffer_directorio, 6) == -1)
         {
            bumount();
            fprintf(stderr, "Error al crear el DIRECTORIO %s\n", buffer_directorio);
         }
         sprintf(camino_fic, "%sprueba.dat", buffer_directorio);
         if (mi_creat(camino_fic, 6) == -1)
         {
            bumount();
            fprintf(stderr, "Error al crear el fichero %s\n", camino_fic);
         }
         srand(time(NULL) + pid_directorio);
         for (int j = 0; j < NUMESCRITURAS; j++)
         {
            registro.fecha = time(NULL);
            registro.pid = pid_directorio;
            registro.nEscritura = j + 1;
            registro.nRegistro = rand() % REGMAX;
            if (mi_write(camino_fic, &registro, registro.nRegistro * sizeof(struct REGISTRO), sizeof(struct REGISTRO)) == -1)
            {
               fprintf(stderr, "Error con el mi_write al fichero\n");
            }
            // fprintf(stderr, "[simulación.c → Escritura %d en %s]\n", j, camino_fic);
            usleep(50000); //0,05 segundos
         }
         printf("Proceso %d: Completadas %d escrituras en %s\n", i, NUMESCRITURAS, camino_fic);
         bumount();
         exit(0);
      }
      usleep(200000); //0,2 segundos
   }
   while (acabados < NUMPROCESOS)
   {
      pause();
   }
   bumount();
   printf("Total de procesos terminados: %d.\n", acabados);
   exit(0);
}


/*	-Función: reaper.
	-Descripción: Entierra a los procesos hijo zombie.
	-Parámetros:
	-Return:
*/
void reaper()
{
   pid_t ended;
   signal(SIGCHLD, reaper);
   while ((ended = waitpid(-1, NULL, WNOHANG)) > 0)
   {
      acabados++;
      //Podemos testear qué procesos van acabando:
      //fprintf(stderr, "[simulación.c → Acabado proceso con PID %d, total acabados: %d]\n", ended, acabados);
   }
}
