#include "directorios.h"

int main(int argc, char const *argv[])
{
	char nombre_dispositivo[1024];
	char ruta[1024];
	unsigned char permisos;
	int resultado;
	if (argc != 4)
	{
		fprintf(stderr, "Sintaxis: mi_chmod <nombre_dispositivo> <permisos> </ruta>\n");
		exit(EXIT_FAILURE);
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

	permisos = atoi(argv[2]);
	if (permisos > '7')
	{
		fprintf(stderr, "Error:  modo inválido:<<%d>>\n", permisos);
		exit(EXIT_FAILURE);
	}
	strcpy(ruta, argv[3]);
	resultado = mi_chmod(ruta, permisos);
	if (resultado == -1)
	{
		fprintf(stderr, "Error común\n");
		exit(EXIT_FAILURE);
	}
	if (bumount() == -1)
	{
		fprintf(stderr, "ERROR: falló bumount\n");
		exit(EXIT_FAILURE);
	}
}