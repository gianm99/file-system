#include "directorios.h"

int main(int argc, char **argv)
{
	unsigned char buf[BLOCKSIZE];
	char nombre_dispositivo[1024];
	int nbloques;
	int ninodos;
	if (argc != 3)
	{
		fprintf(stderr, "Sintaxis: ./mi_mkfs nombre_dispositivo n_bloques\n");
		exit(EXIT_FAILURE);
	}
	strcpy(nombre_dispositivo, argv[1]);
	nbloques = atoi(argv[2]);
	if (nbloques <= 0)
	{
		fprintf(stderr, "ERROR: Número de bloques incorrecto\n");
		exit(EXIT_FAILURE);
	}
	umask(0);
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
	memset(buf, 0, BLOCKSIZE);
	for (int i = 0; i < nbloques; i++)
	{
		if (bwrite(i, buf) == -1)
		{
			fprintf(stderr, "ERROR: ocurrió un error vaciando %s\n", nombre_dispositivo);
			exit(EXIT_FAILURE);
		}
	}
	ninodos = nbloques / 4;
	if (initSB(nbloques, ninodos) == -1)
	{
		fprintf(stderr, "ERROR: falló initSB\n");
		exit(EXIT_FAILURE);
	}
	if (initMB() == -1)
	{
		fprintf(stderr, "ERROR: falló initMB\n");
		exit(EXIT_FAILURE);
	}
	if (initAI() == -1)
	{
		fprintf(stderr, "ERROR: falló initAI\n");
		exit(EXIT_FAILURE);
	}
	if (reservar_inodo('d', 7) == -1)
	{
		fprintf(stderr, "ERROR: falló reservar_inodo\n");
		exit(EXIT_FAILURE);
	}
	if (bumount() == -1)
	{
		fprintf(stderr, "ERROR: falló bumount\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}
