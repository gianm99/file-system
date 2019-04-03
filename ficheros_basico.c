#include "ficheros_basico.h"

/************************************************** NIVEL 2 **************************************************/

/*	-Función: tamMB.
	-Descripción: Calcula el tamaño, en bloques, necesario para el mapa de bits.
	-Parámetros: Número de bloques del sistema de ficheros.
	-Return: El tamaño del MB como int.
*/
int tamMB(unsigned int nbloques)
{
	int MB;

	MB = (nbloques / 8);
	if (MB % BLOCKSIZE)
	{
		return (MB / BLOCKSIZE) + 1;
	}
	return (MB / BLOCKSIZE);
}
/*	-Función: tamAI.
	-Descripción: Calcula el tamaño, en bloques, necesario para el array de inodos.
	-Parámetros: Número de inodos del sistema de ficheros.
	-Return: El tamaño del AI como int.
*/
int tamAI(unsigned int ninodos)
{
	int AI;

	AI = (ninodos * INODOSIZE);
	if (AI % BLOCKSIZE)
	{
		return (AI / BLOCKSIZE) + 1;
	}
	return (AI / BLOCKSIZE);
}
/*	-Función: initSB.
	-Descripción: Inicializa los datos del superbloque y lo escribe.
	-Parámetros: Número de bloques y número de inodos en el sistema de ficheros.
	-Return: 0 si no hay ningún error y -1 en caso contrario.
*/
int initSB(unsigned int nbloques, unsigned int ninodos)
{
	struct superbloque SB;

	SB.posPrimerBloqueMB = posSB + tamSB; //posSB = 0, tamSB = 1
	SB.posUltimoBloqueMB = SB.posPrimerBloqueMB + tamMB(nbloques) - 1;
	SB.posPrimerBloqueAI = SB.posUltimoBloqueMB + 1;
	SB.posUltimoBloqueAI = SB.posPrimerBloqueAI + tamAI(ninodos) - 1;
	SB.posPrimerBloqueDatos = SB.posUltimoBloqueAI + 1;
	SB.posUltimoBloqueDatos = nbloques - 1;
	SB.posInodoRaiz = 0;
	SB.posPrimerInodoLibre = 0;
	SB.cantBloquesLibres = nbloques;
	SB.cantInodosLibres = ninodos;
	SB.totBloques = nbloques;
	SB.totInodos = ninodos;
	if (bwrite(posSB, &SB) == -1)
		return -1;
	return 0;
}
/*	-Función: initMB.
	-Descripción: Inicializa el mapa de bits y lo escribe.
	-Parámetros: 
	-Return: 0 si no hay ningún error y -1 en caso contrario.
*/
int initMB()
{
	struct superbloque SB;
	unsigned char buf[BLOCKSIZE];

	memset(buf, 0, BLOCKSIZE);
	if (bread(posSB, &SB) == -1)
		return -1;
	for (int i = SB.posPrimerBloqueMB; i <= SB.posUltimoBloqueMB; i++)
	{
		if (bwrite(i, buf) == -1)
			return -1;
	}
	for (int i = 0; i <= SB.posUltimoBloqueAI; i++)
	{
		if (escribir_bit(i, 1) == -1)
			return -1;
	}
	SB.cantBloquesLibres -= (SB.posUltimoBloqueAI + 1); //SB + MB + AI
	if (bwrite(posSB, &SB) == -1)
		return -1;
	return 0;
}
/*	-Función: initAI.
	-Descripción: Inicializa la lista de inodos libres.
	-Parámetros: 
	-Return: 0 si no hay ningún error y -1 en caso contrario.
*/
int initAI()
{
	struct superbloque SB;
	struct inodo inodos[BLOCKSIZE / INODOSIZE];
	int contInodos;

	if (bread(posSB, &SB) == -1)
		return -1;
	contInodos = SB.posPrimerInodoLibre + 1;
	for (int i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++)
	{
		for (int j = 0; j < BLOCKSIZE / INODOSIZE; j++)
		{
			inodos[j].tipo = 'l';
			if (contInodos < SB.totInodos)
			{
				inodos[j].punterosDirectos[0] = contInodos;
				contInodos++;
			}
			else
			{
				inodos[j].punterosDirectos[0] = UINT_MAX;
				break; //break out of the loop
			}
		}
		if (bwrite(i, inodos) == -1)
			return -1;
	}
	return 0;
}

/************************************************** NIVEL 3 **************************************************/

/*	-Función: escribir_bit.
	-Descripción: Escribe el valor indicado por parámetro en determinado bit del MB.
	-Parámetros: Número de bloque que tiene que acceder y bit que se tiene que escribir.
	-Return: 0 si no hay ningún error y -1 en caso contrario.
*/
int escribir_bit(unsigned int nbloque, unsigned int bit)
{
	int posbyte, posbit, nbloqueMB, nbloqueabs;
	unsigned char bufferMB[BLOCKSIZE];
	unsigned char mascara = 128; // 10000000
	struct superbloque SB;

	if (bread(posSB, &SB) == -1)
		return -1;
	posbyte = nbloque / 8;
	posbit = nbloque % 8;
	nbloqueMB = posbyte / BLOCKSIZE;
	nbloqueabs = nbloqueMB + SB.posPrimerBloqueMB;
	if (bread(nbloqueabs, bufferMB) == -1)
	{
		return -1;
	}
	posbyte = posbyte % BLOCKSIZE;
	mascara >>= posbit;			  // desplazamiento de bits a la derecha
	bufferMB[posbyte] |= mascara; // operador OR para bits
	if (bit == 1)
	{
		bufferMB[posbyte] |= mascara;
	}
	else if (bit == 0)
	{
		bufferMB[posbyte] &= ~mascara;
	}
	else
	{
		return -1;
	}
	if (bwrite(nbloqueabs, bufferMB) == -1)
		return -1;
	return 0;
}
/*	-Función: leer_bit.
	-Descripción: Lee un determinado bit del MB.
	-Parámetros: Número de bloque que tiene que acceder.
	-Return: Devuelve el bit leído o 2 en caso de error.
*/
unsigned char leer_bit(unsigned int nbloque)
{
	int posbyte, posbit, nbloqueMB, nbloqueabs;
	unsigned char bufferMB[BLOCKSIZE];
	unsigned char mascara = 128; // 10000000
	struct superbloque SB;

	if (bread(posSB, &SB) == -1)
		return 2;
	posbyte = nbloque / 8;
	posbit = nbloque % 8;
	nbloqueMB = posbyte / BLOCKSIZE;
	nbloqueabs = nbloqueMB + SB.posPrimerBloqueMB;
	if (bread(nbloqueabs, bufferMB) == -1)
		return 2;
	posbyte = posbyte % BLOCKSIZE;
	mascara >>= posbit;			  // desplazamiento de bits a la derecha
	mascara &= bufferMB[posbyte]; // operador AND para bits
	mascara >>= (7 - posbit);	 // desplazamiento de bits a la derecha
	fprintf(stderr, "[leer_bit()→ nbloque: %d, posbyte:%d, posbit:%d, nbloqueMB:%d, nbloqueabs:%d)]\n",
			nbloque, posbyte, posbit, nbloqueMB, nbloqueabs);
	return mascara;
}
/*	-Función: reservar_bloque.
	-Descripción: Encuentra el primer bloque libre y lo ocupa.
	-Parámetros: 
	-Return: Devuelve la posición del bloque libre o -1 en caso de error.
*/
int reservar_bloque()
{
	int posBloqueMB, posbyte, nbloque, posbit;
	unsigned char bufferMB[BLOCKSIZE], bufferAux[BLOCKSIZE], mascara, bufferByte;
	struct superbloque SB;

	memset(bufferAux, 255, BLOCKSIZE);
	if (bread(posSB, &SB) == -1)
		return -1;
	if (!SB.cantBloquesLibres) // Comprueba si quedan bloques libres
		return -1;
	/* Encontrar bloque */
	posBloqueMB = SB.posPrimerBloqueMB;
	if (bread(posBloqueMB, bufferMB) == -1)
		return -1;
	while (posBloqueMB <= SB.posUltimoBloqueMB && memcmp(bufferMB, bufferAux, BLOCKSIZE) == 0)
	{
		posBloqueMB++;
		if (bread(posBloqueMB, bufferMB) == -1)
			return -1;
	}
	if (memcmp(bufferMB, bufferAux, BLOCKSIZE) == 0)
		return -1;
	/* Encontrar byte */
	posbyte = 0;
	bufferByte = 255;
	while (posbyte < BLOCKSIZE && memcmp(&bufferMB[posbyte], &bufferByte, 1) == 0)
	{
		posbyte++;
	}
	if (posbyte == BLOCKSIZE)
		return -1;
	/* Encontrar bit */
	mascara = 128; // 10000000
	posbit = 0;
	while (bufferMB[posbyte] & mascara)
	{
		posbit++;
		bufferMB[posbyte] <<= 1; // desplaz. de bits a la izqda
	}

	/* Actualizar el mapa de bits */
	nbloque = ((posBloqueMB - SB.posPrimerBloqueMB) * BLOCKSIZE + posbyte) * 8 + posbit;
	if (escribir_bit(nbloque, 1) == -1)
		return -1;
	/* Actualizar el superbloque */
	SB.cantBloquesLibres--;
	if (bwrite(posSB, &SB) == -1)
		return -1;
	/* Limpiar el bloque */
	memset(bufferAux, 0, BLOCKSIZE);
	if (bwrite(nbloque, bufferAux) == -1)
		return -1;
	return nbloque;
}
/*	-Función: liberar_bloque
	-Descripción: Libera un bloque determinado poniendo a 0 el bit del MB del bloque
	que recibimos como parámetro.
	-Parámetros: Número de bloque que tiene que acceder. 
	-Return: Devuelve el número de bloque liberado o -1 en caso de error.
*/
int liberar_bloque(unsigned int nbloque)
{
	struct superbloque SB;

	if (escribir_bit(nbloque, 0) == -1)
		return -1;
	if (bread(posSB, &SB) == -1)
		return -1;
	SB.cantBloquesLibres++;
	if (bwrite(posSB, &SB) == -1)
		return -1;
	return nbloque;
}
/*	-Función: escribir_inodo
	-Descripción: Escribe el contenido de una variable inodo en un determinado 
	inodo del array de inodos.
	-Parámetros: número de inodo del array, inodo a escribir.
	-Return: -1 en caso de error, 0 en caso contrario.
*/
int escribir_inodo(unsigned int ninodo, struct inodo inodo)
{
	struct superbloque SB;
	struct inodo inodos[BLOCKSIZE / INODOSIZE];
	int nbloqueabs;

	if (bread(posSB, &SB) == -1)
		return -1;
	nbloqueabs = (ninodo * INODOSIZE) / BLOCKSIZE + SB.posPrimerBloqueAI;
	if (bread(nbloqueabs, inodos) == -1)
	{
		return -1;
	}
	inodos[ninodo % (BLOCKSIZE / INODOSIZE)] = inodo;
	if (bwrite(nbloqueabs, inodos) == -1)
		return -1;
	return 0;
}
/*	-Función: leer_inodo
	-Descripción:Lee un determinado inodo del array de inodos para volcarlo en una variable
 	de tipo struct inodo pasada por referencia. 
	-Parámetros: El número de inodo y un inodo.
	-Return: Devuelve un 0 si va bien, -1 en caso contrario.
*/
int leer_inodo(unsigned int ninodo, struct inodo *inodo)
{
	struct superbloque SB;
	struct inodo inodos[BLOCKSIZE / INODOSIZE];
	int nbloqueabs;

	if (bread(posSB, &SB) == -1)
		return -1;
	nbloqueabs = (ninodo * INODOSIZE) / BLOCKSIZE + SB.posPrimerBloqueAI;
	if (bread(nbloqueabs, inodos) == -1)
		return -1;
	*inodo = inodos[ninodo % (BLOCKSIZE / INODOSIZE)];
	return 0;
}
/*	-Función: reservar_inodo
	-Descripción: Reserva datos en la posición del primer inodo libre y actualiza la lista 
	enlazada de	inodos libres.
	-Parámetros: El tipo del inodo y sus permisos de r/w y ejecución.
	-Return: Devuelve la posición del inodo reservado o -1 en caso de error.
*/
int reservar_inodo(unsigned char tipo, unsigned char permisos)
{
	struct inodo inodoAux;
	struct superbloque SB;
	unsigned int posInodoReservado;

	if (bread(posSB, &SB) == -1)
		return -1;
	if (SB.cantInodosLibres <= 0)
		return -1;
	posInodoReservado = SB.posPrimerInodoLibre;
	if (leer_inodo(posInodoReservado, &inodoAux) == -1)
		return -1;
	SB.posPrimerInodoLibre = inodoAux.punterosDirectos[0];
	inodoAux.tipo = tipo;
	inodoAux.permisos = permisos;
	inodoAux.nlinks = 1;
	inodoAux.tamEnBytesLog = 0;
	inodoAux.atime = time(NULL);
	inodoAux.mtime = time(NULL);
	inodoAux.ctime = time(NULL);
	inodoAux.numBloquesOcupados = 0;
	memset(inodoAux.punterosDirectos, 0, 12 * sizeof(unsigned int));
	memset(inodoAux.punterosIndirectos, 0, 3 * sizeof(unsigned int));
	if (escribir_inodo(posInodoReservado, inodoAux) == -1)
		return -1;
	SB.cantInodosLibres--;
	if (bwrite(posSB, &SB) == -1)
		return -1;
	return posInodoReservado;
}

/************************************************** NIVEL 4 **************************************************/

/*	-Función: obtener_nrangoBL
	-Descripción: Asocia un nivel a cada rango de bloques lógicos.
	-Parámetros: Inodo al que pertenece el bloque lógico, número de bloque 
	lógico y puntero que se devolverá con la dirección a la que apunte el 
	puntero correspondiente del inodo.
	-Return: rango (-1 en caos de error) y puntero con misma dirección que
	el puntero del inodo correcto.
*/
int obtener_nrangoBL(struct inodo inodo, unsigned int nblogico, unsigned int *ptr)
{
	if (nblogico < DIRECTOS)
	{
		*ptr = inodo.punterosDirectos[nblogico];
		return 0;
	}
	else if (nblogico < INDIRECTOS0)
	{
		*ptr = inodo.punterosIndirectos[0];
		return 1;
	}
	else if (nblogico < INDIRECTOS1)
	{
		*ptr = inodo.punterosIndirectos[1];
		return 2;
	}
	else if (nblogico < INDIRECTOS2)
	{
		*ptr = inodo.punterosIndirectos[2];
		return 3;
	}
	else
	{
		*ptr = 0;
		fprintf(stderr, "Bloque lógico fuera de rango\n");
		return -1;
	}
}
/*	-Función: obtener_indice
	-Descripción: Obtiene el índice de los bloques de punteros.
	-Parámetros: El número del bloque logico mas el nivel de los punteros a cuyo
	indice queremos acceder.
	-Return: Devuelve el índice en el bloque de punteros o en el array de 
	punteros dependiendo del nivel o -1 en caso de error.
*/
int obtener_indice(unsigned int nblogico, unsigned int nivel_punteros)
{
	if (nblogico < DIRECTOS)
	{
		return nblogico;
	}
	else if (nblogico < INDIRECTOS0)
	{
		return nblogico - DIRECTOS;
	}
	else if (nblogico < INDIRECTOS1)
	{
		if (nivel_punteros == 2)
		{
			return (nblogico - INDIRECTOS0) / NPUNTEROS;
		}
		else if (nivel_punteros == 1)
		{
			return (nblogico - INDIRECTOS0) % NPUNTEROS;
		}
	}
	else if (nblogico < INDIRECTOS2)
	{
		if (nivel_punteros == 3)
		{
			return (nblogico - INDIRECTOS1) / (NPUNTEROS * NPUNTEROS);
		}
		else if (nivel_punteros == 2)
		{
			return ((nblogico - INDIRECTOS1) % (NPUNTEROS * NPUNTEROS)) / NPUNTEROS;
		}
		else if (nivel_punteros == 1)
		{
			return ((nblogico - INDIRECTOS1) % (NPUNTEROS * NPUNTEROS)) % NPUNTEROS;
		}
	}
	return -1;
}
/*	-Función: traducir_bloque_inodo
	-Descripción: obtener el bloque físico de un bloque lógico de un inodo.
	-Parámetros: número de inodo elegido, bloque lógico a traducir y reservar o no.
	-Return: número de bloque físico o -1 en caso de error.
*/
int traducir_bloque_inodo(unsigned int ninodo, unsigned int nblogico, char reservar)
{
	struct inodo inodo;
	int salvar_inodo, nRangoBL, nivel_punteros, indice;
	int buffer[NPUNTEROS];
	unsigned int ptr, ptr_ant;
	if (leer_inodo(ninodo, &inodo) == -1)
	{
		return -1;
	}
	ptr = 0, ptr_ant = 0, salvar_inodo = 0;
	nRangoBL = obtener_nrangoBL(inodo, nblogico, &ptr); //0:D, 1:I0, 2:I1, 3:I2
	if (nRangoBL == -1)
		return -1;
	nivel_punteros = nRangoBL; //el nivel_punteros +alto es el que cuelga del inodo
	while (nivel_punteros > 0) //iterar para cada nivel de indirectos
	{
		if (ptr == 0) //no cuelgan bloques de punteros
		{
			if (reservar == 0) //error lectura bloque inexistente
				return -1;
			salvar_inodo = 1;		 //reservar bloques punteros y crear enlaces desde inodo hasta datos
			ptr = reservar_bloque(); //de punteros
			if (ptr == -1)			 //error reservar_bloque
				return -1;
			inodo.numBloquesOcupados++;
			inodo.ctime = time(NULL);		//fecha actual
			if (nivel_punteros == nRangoBL) //el bloque cuelga directamente del inodo
			{
				inodo.punterosIndirectos[nRangoBL - 1] = ptr;
				fprintf(stderr, "[traducir_bloque_inodo()→ inodo.punterosIndirectos[%d] = %d (reservado BF %d para punteros_nivel%d)]\n",
						nRangoBL - 1, ptr, ptr, nRangoBL);
			}
			else //el bloque cuelga de otro bloque de punteros
			{
				buffer[indice] = ptr; // IMPRIMIRLO
				if (bwrite(ptr_ant, buffer) == -1)
					return -1;
				fprintf(stderr, "[traducir_bloque_inodo()→ punteros_nivel%d [%d] = %d (reservado BF %d para punteros_nivel%d)]\n",
						nivel_punteros + 1, indice, ptr, ptr, nivel_punteros);
			}
		}
		if (bread(ptr, buffer) == -1)
			return -1;
		indice = obtener_indice(nblogico, nivel_punteros);
		ptr_ant = ptr;		  //guardamos el puntero
		ptr = buffer[indice]; // y lo desplazamos al siguiente nivel
		nivel_punteros--;
	}			  //al salir de este bucle ya estamos al nivel de datos
	if (ptr == 0) //no existe bloque de datos
	{
		if (reservar == 0) //error lectura ∄ bloque
			return -1;
		salvar_inodo = 1;
		ptr = reservar_bloque(); //de datos
		if (ptr == -1)			 //error reservar_bloque
			return -1;
		inodo.numBloquesOcupados++;
		inodo.ctime = time(NULL);
		if (nRangoBL == 0)
		{
			inodo.punterosDirectos[nblogico] = ptr;
			fprintf(stderr, "[traducir_bloque_inodo()→ inodo.punterosDirectos[%d] = %d (reservado BF %d para BL %d)]\n",
					nblogico, ptr, ptr, nblogico);
		}
		else
		{
			buffer[indice] = ptr;
			if (bwrite(ptr_ant, buffer) == -1)
				return -1;
			fprintf(stderr, "[traducir_bloque_inodo()→ punteros_nivel%d [%d] = %d (reservado BF %d para BL %d)]\n",
					nivel_punteros + 1, indice, ptr, ptr, nblogico);
		}
	}
	if (salvar_inodo == 1)
	{
		if (escribir_inodo(ninodo, inodo) == -1) //sólo si lo hemos actualizado
			return -1;
	}
	return ptr; //nbfisico
}

/************************************************** NIVEL 5 **************************************************/

int liberar_inodo(unsigned int ninodo)
{
	struct inodo inodo;
	struct superbloque SB;
	int numBloquesLiberados;

	clock_t t;
	t = clock();
	numBloquesLiberados = liberar_bloques_inodo(ninodo, 0);
	t = clock() - t;
	double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

	printf("liberar_bloques_inodo() took %f seconds to execute \n", time_taken);

	if (numBloquesLiberados == -1)
		return -1;
	leer_inodo(ninodo, &inodo);
	inodo.numBloquesOcupados -= numBloquesLiberados;
	inodo.tipo = 'l';
	if (bread(posSB, &SB) == -1)
	{
		return -1;
	}
	inodo.punterosDirectos[0] = SB.posPrimerInodoLibre;
	SB.posPrimerInodoLibre = ninodo;
	SB.cantInodosLibres++;
	if (escribir_inodo(ninodo, inodo) == -1)
		return -1;
	if (bwrite(posSB, &SB) == -1)
		return -1;
	return ninodo;
}
/*	-Función: liberar_bloques_inodo
	-Descripción: libera los bloques lógicos del inodo a partir de nblogico hasta el último.
	-Parámetros: número de inodo elegido, bloque lógico a partir del cual se empieza a liberar.
	-Return: número de bloques liberados o -1 en caso de error.
*/
int liberar_bloques_inodo(unsigned int ninodo, unsigned int nblogico)
{
	struct inodo inodo;
	unsigned int nRangoBL, nivel_punteros, indice, ptr, ultimoBL;
	unsigned char buffer_aux[BLOCKSIZE];
	int bloques_punteros[3][NPUNTEROS]; //array de bloques de punteros
	int ptr_nivel[3];					//punteros a bloques de punteros de cada nivel
	int indices[3];						//indices de cada nivel
	int liberados;						//nº de bloques liberados
	int salvar_inodo;

	liberados = 0;
	if (leer_inodo(ninodo, &inodo) == -1)
		return -1;
	if (inodo.tamEnBytesLog == 0) //fichero vacío
		return liberados;
	// obtenemos el último bloque lógico del inodo
	if (inodo.tamEnBytesLog % BLOCKSIZE == 0)
	{
		ultimoBL = inodo.tamEnBytesLog / BLOCKSIZE - 1;
	}
	else
	{
		ultimoBL = inodo.tamEnBytesLog / BLOCKSIZE;
	}
	memset(buffer_aux, 0, BLOCKSIZE);
	ptr = 0;
	fprintf(stderr, "primer BL: %u, último BL: %u\n", nblogico, ultimoBL);
	for (int nblog = nblogico; nblog <= ultimoBL; nblog++)
	{
		nRangoBL = obtener_nrangoBL(inodo, nblog, &ptr);
		if (nRangoBL == -1)
			return -1;
		/* MEJORA DEL ALGORITMO 1 de 2.
		Si uno de los punteros del inodo apunta a 0 se salta al siguiente rango.*/
		if (!ptr)
		{
			if (nRangoBL == 1)
			{
				nblog = INDIRECTOS0 - 1;
				continue;
			}
			else if (nRangoBL == 2)
			{
				nblog = INDIRECTOS1 - 1;
				continue;
			}
			else if (nRangoBL == 3)
			{
				break;
			}
		}
		nivel_punteros = nRangoBL; //el nivel_punteros más alto cuelga del inodo
		while (ptr > 0 && nivel_punteros > 0)
		{
			bread(ptr, bloques_punteros[nivel_punteros - 1]);
			indice = obtener_indice(nblog, nivel_punteros);
			ptr_nivel[nivel_punteros - 1] = ptr;
			indices[nivel_punteros - 1] = indice;
			ptr = bloques_punteros[nivel_punteros - 1][indice];
			nivel_punteros--;
		}
		if (ptr > 0)
		{
			liberar_bloque(ptr);
			fprintf(stderr, "[liberar_bloques_inodo()→ liberado BF %d de datos correspondiente al BL %d ]\n", ptr, nblog);
			liberados++;
			if (nRangoBL == 0)
			{
				inodo.punterosDirectos[nblog] = 0;
				salvar_inodo = 1;
			}
			else
			{
				while (nivel_punteros < nRangoBL)
				{
					indice = indices[nivel_punteros];
					bloques_punteros[nivel_punteros][indice] = 0;
					ptr = ptr_nivel[nivel_punteros];
					if (!memcmp(bloques_punteros[nivel_punteros], buffer_aux, BLOCKSIZE))
					{
						//No cuelgan bloques ocupados, hay que liberar el bloque de punteros
						liberar_bloque(ptr);
						fprintf(stderr, "[liberar_bloques_inodo()→ liberado BF %d de punteros de nivel %u correspondiente al BL %d ]\n",
								ptr, nivel_punteros + 1, nblog);
						liberados++;
						nivel_punteros++;
						if (nivel_punteros == nRangoBL)
						{
							inodo.punterosIndirectos[nRangoBL - 1] = 0;
							salvar_inodo = 1;
						}
					}
					else
					{ //escribimos en el dispositivo el bloque de punteros modificado
						bwrite(ptr, bloques_punteros[nivel_punteros]);
						nivel_punteros = nRangoBL; // para salir del bucle
					}
				}
			}
		}
		else if (ptr == 0)
		{
			/* MEJORA DEL ALGORITMO 2 de 2.
			Se comprueba a qué nivel pertenece el puntero y se saltan los
			bloques lógicos que no hacen falta comprobar.*/
			if (nivel_punteros == 2)
			{
				nblog += NPUNTEROS - 1;
			}
			else if (nivel_punteros == 3)
			{
				nblog += (NPUNTEROS * NPUNTEROS) - 1;
			}
		}
	}
	if (salvar_inodo)
	{
		if (escribir_inodo(ninodo, inodo) == -1)
			return -1;
	}
	fprintf(stderr, "[liberar_bloques_inodo()→ total bloques liberados: %d ]\n\n", liberados);
	return liberados;
}