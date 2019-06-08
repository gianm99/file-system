#include "directorios.h"
int main(int argc, char const *argv[])
{
	char nombre_dispositivo[1024];
	char camino[1024];
	unsigned char permisos;
	if (argc != 4)
	{
		fprintf(stderr, "Sintaxis: mi_mkdir <disco> <permisos> </camino>\n");
		exit(EXIT_FAILURE);
	}
	strcpy(nombre_dispositivo, argv[1]);
	permisos = atoi(argv[2]);
	if (permisos > 7)
	{
		fprintf(stderr, "Error:  modo inválido:<<%d>>\n", permisos);
		exit(EXIT_FAILURE);
	}
	strcpy(camino, argv[3]);
	if (camino[strlen(camino) - 1] != '/')
	{
		fprintf(stderr, "Error: No es un directorio\n");
		exit(EXIT_FAILURE);
	}
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
	if (mi_creat(camino, permisos) == -1)
	{
		fprintf(stderr, "ERROR: falló mi_creat\n");
		exit(EXIT_FAILURE);
	}
	if (bumount() == -1)
	{
		fprintf(stderr, "ERROR: falló bumount\n");
		exit(EXIT_FAILURE);
	}
}