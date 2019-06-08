#include "directorios.h"

int main(int argc, char const *argv[])
{
	char nombre_dispositivo[1024];
	char ruta[1024];
	struct STAT stat;
	struct tm *tm;
	if (argc != 3)
	{
		fprintf(stderr, "Sintaxis: mi_stat <disco> </ruta>\n");
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
	strcpy(ruta, argv[2]);
	if (mi_stat(ruta, &stat) == -1)
	{
		fprintf(stderr, "ERROR: falló mi_stat\n");
		exit(EXIT_FAILURE);
	}
	// tipo de inodo
	printf("tipo: %c\n", stat.tipo);
	// permisos del inodo
	printf("permisos: %d\n", stat.permisos);
	// atime del inodo
	tm = localtime(&stat.atime);
	printf("atime: %d-%02d-%02d %02d:%02d:%02d\n", tm->tm_year + 1900,
			 tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	// ctime del inodo
	tm = localtime(&stat.ctime);
	printf("ctime: %d-%02d-%02d %02d:%02d:%02d\n", tm->tm_year + 1900,
			 tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	// mtime del inodo
	tm = localtime(&stat.mtime);
	printf("mtime: %d-%02d-%02d %02d:%02d:%02d\n", tm->tm_year + 1900,
			 tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	// nlinks del inodo
	printf("nlinks: %u\n", stat.nlinks);
	// tamEnBytesLog del inodo
	printf("tamEnBytesLog: %d\n", stat.tamEnBytesLog);
	// numBloquesOcupados del inodo
	printf("numBloquesOcupados: %d\n", stat.numBloquesOcupados);
	if (bumount() == -1)
	{
		fprintf(stderr, "ERROR: falló bumount\n");
		exit(EXIT_FAILURE);
	}
}