#include "directorios.h"
void datos_inodo(struct inodo inodo);
int main(int argc, char const *argv[])
{
	char nombre_dispositivo[1024];
	// struct inodo inodo_reservado;
	// int ninodo;
	struct superbloque SB;
	if (argc != 2)
	{
		fprintf(stderr, "Sintaxis: ./leer_sf <nombre_dispositivo>\n");
		exit(EXIT_FAILURE);
	}
	strcpy(nombre_dispositivo, argv[1]);
	if (access(nombre_dispositivo, F_OK) == -1)
	{
		fprintf(stderr, "ERROR: No existe el archivo %s\n", nombre_dispositivo);
		exit(EXIT_FAILURE);
	}
	if (bmount(nombre_dispositivo) == -1)
		exit(EXIT_FAILURE);
	if (bread(posSB, &SB) == -1)
		exit(EXIT_FAILURE);
	printf("DATOS DEL SUPERBLOQUE\n");
	printf("posPrimerBloqueMB = %d\n", SB.posPrimerBloqueMB);
	printf("posUltimoBloqueMB = %d\n", SB.posUltimoBloqueMB);
	printf("posPrimerBloqueAI = %d\n", SB.posPrimerBloqueAI);
	printf("posUltimoBloqueAI = %d\n", SB.posUltimoBloqueAI);
	printf("posPrimerBloqueDatos = %d\n", SB.posPrimerBloqueDatos);
	printf("posUltimoBloqueDatos = %d\n", SB.posUltimoBloqueDatos);
	printf("posInodoRaiz = %d\n", SB.posInodoRaiz);
	printf("posPrimerInodoLibre = %d\n", SB.posPrimerInodoLibre);
	printf("cantBloquesLibres = %d\n", SB.cantBloquesLibres);
	printf("cantInodosLibres = %d\n", SB.cantInodosLibres);
	printf("totBloques = %d\n", SB.totBloques);
	printf("totInodos = %d\n\n", SB.totInodos);
	if (bumount(nombre_dispositivo) == -1)
		exit(EXIT_FAILURE);
	return 0;
}