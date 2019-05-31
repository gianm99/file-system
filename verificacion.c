#include "verificacion.h"

int main(int argc, char const *argv[])
{
	char nombre_dispositivo[1024], dir_simul[1024], camino_fic[1024], camino_prueba[1024], *ret;
	struct entrada buffer_entradas[NUMPROCESOS];
	struct STAT stat;
	struct INFORMACION info;
	int numentradas, offset, validadas;
	int cant_registros_buffer_escrituras = 256;
	struct REGISTRO buffer_escrituras[cant_registros_buffer_escrituras];

	if (argc != 3)
	{
		fprintf(stderr, "Sintaxis:./verificacion <disco> <directorio_simulacion>\n");
		exit(EXIT_FAILURE);
	}
	strcpy(nombre_dispositivo, argv[1]);
	if (access(nombre_dispositivo, F_OK) == -1)
	{
		fprintf(stderr, "ERROR: No existe el archivo\n");
		exit(EXIT_FAILURE);
	}

	bmount(nombre_dispositivo);
	strcpy(dir_simul, argv[2]);
	mi_stat(dir_simul, &stat);
	if (dir_simul[strlen(dir_simul) - 1] != '/')
	{
		fprintf(stderr, "Error: No es un directorio\n");
		exit(EXIT_FAILURE);
	}
	numentradas = stat.tamEnBytesLog / tamEntrada;
	if (numentradas != NUMPROCESOS)
		return -1;

	sprintf(camino_fic, "%sinforme.txt", dir_simul);
	mi_creat(camino_fic, 6);
	mi_read(dir_simul, buffer_entradas, 0, NUMPROCESOS * sizeof(struct entrada)); //lee las entradas
	for (int i = 0; i < NUMPROCESOS; i++)
	{
		validadas = 0;
		ret = strchr(buffer_entradas[i].nombre, '_');
		if (!ret) //ret = null
		{
			fprintf(stderr, "ERROR: Formato de nombre de directorio incorrecto\n");
			exit(EXIT_FAILURE);
		}
		info.pid = atoi(ret + 1);

		//Recorrido secuencial del fichero prueba.dat
		offset = 0;
		memset(camino_prueba, 0, sizeof(camino_prueba));
		sprintf(camino_prueba, "%s%s/prueba.dat", dir_simul, buffer_entradas[i].nombre);
		memset(buffer_escrituras, 0, NUMPROCESOS * sizeof(struct REGISTRO));
		while (mi_read(camino_prueba, buffer_escrituras, offset, sizeof(buffer_escrituras)) > 0)
		{
			for (int j = 0; j < cant_registros_buffer_escrituras; j++)
			{
				if (buffer_escrituras[j].pid == info.pid) //la escritura es válida
				{
					if (validadas == 0) ///es la primera escritura validada
					{
						info.PrimeraEscritura = buffer_escrituras[j];
						info.UltimaEscritura = buffer_escrituras[j];
						info.MenorPosicion = buffer_escrituras[j];
						info.MayorPosicion = buffer_escrituras[j];
					}
					else
					{
						if (difftime(buffer_escrituras[j].fecha, info.PrimeraEscritura.fecha) < 0 ||
							 buffer_escrituras[j].nEscritura < info.PrimeraEscritura.nEscritura) //es anterior
						{
							info.PrimeraEscritura = buffer_escrituras[j];
						}
						else if (difftime(buffer_escrituras[j].fecha, info.UltimaEscritura.fecha) > 0 ||
									buffer_escrituras[j].nEscritura > info.UltimaEscritura.nEscritura) //es posterior
						{
							info.UltimaEscritura = buffer_escrituras[j];
						}
					}
				}
			}
			offset += sizeof(buffer_escrituras);
		}
	}
	bumount();
}
