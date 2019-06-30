#include "directorios.h"

int main(int argc, char const *argv[])
{
	char nombre_dispositivo[1024];
	char ruta[1024];
	unsigned int offset, nbytes;
	int escritos;
	if (argc != 5)
	{
		fprintf(stderr, "Sintaxis: mi_escribir <disco> </ruta_fichero> <texto> <offset>\n");
		exit(EXIT_FAILURE);
	}
	strcpy(nombre_dispositivo, argv[1]);
	strcpy(ruta, argv[2]);
	nbytes = strlen(argv[3]); //Longitud del texto
	fprintf(stderr, "Longitud texto: %d\n", nbytes);
	char buffer[nbytes]; //Buffer del texto
	strcpy(buffer, argv[3]);
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