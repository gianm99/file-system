#include "ficheros_basico.h"
// Nivel 6
struct STAT
{
	char tipo;								// Tipo ('l':libre, 'd':directorio o 'f':fichero)
	char permisos;							// Permisos (lectura y/o escritura y/o ejecución)
	time_t atime;							// Fecha y hora del último acceso a datos: atime
	time_t mtime;							// Fecha y hora de la última modificación de datos: mtime
	time_t ctime;							// Fecha y hora de la última modificación del inodo: ctime
	unsigned int nlinks;					// Cantidad de enlaces de entradas en directorio
	unsigned int tamEnBytesLog;		// Tamaño en bytes lógicos
	unsigned int numBloquesOcupados; // Cantidad de bloques ocupados zona de datos
};
int mi_write_f(unsigned int ninodo, const void *buf_original, unsigned int offset, unsigned int nbytes);
int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes);
int mi_stat_f(unsigned int ninodo, struct STAT *p_stat);
int mi_chmod_f(unsigned int ninodo, unsigned char permisos);
int mi_truncar_f(unsigned int ninodo, unsigned int nbytes);
// Funciones auxiliares
void datos_inodo(struct inodo inodo);
void datos_STAT(struct STAT STAT);
