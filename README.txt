MEJORAS REALIZADAS
Ficheros de texto externos por nombre al escribir (mi_escribir_f.c), mi_ls con info entradas, mi_ls fichero, caché de directorios > 1, buffer de entradas en buscar_entradas(), mi_touch, mi_rmdir de directorios vacíos, mi_rn, mi_cp_f, liberar_bloques_inodo optimizado (versión iterativa), secciones críticas capa ficheros.

-------------------------------------------------------------------------------
RESTRICCIONES DEL PROGRAMA
Para hacer la prueba de las mejoras hay que ejecutar EJEMPLO.sh y luego EJEMPLO1.sh. Y después hay que hacer otra vez EJEMPLO.sh y EJEMPLO2.sh.

-------------------------------------------------------------------------------
SINTAXIS ESPECÍFICA
El programa mi_escribir_f es la versión del programa mi_escribir.c en la que se usan rutas de ficheros como input para el texto a escribir. Se hace así para poder utilizar el script EJEMPLO.sh del nivel 11.
En mi_ls.c se puede usar la opción -s para mostrar la versión corta de mi_ls. Si está activada esta opción, solo se mostrarán los nombres de los ficheros o directorios. Esta opción solo tiene efecto cuando se visita un directorio, cuando se hace sobre un fichero siempre se muestra su informacióna además de su nombre. La sintaxis es: mi_ls <disco> </ruta> o mi_ls <disco> </ruta> -s para la versión corta.
En mi_cp_f la ruta de fuente debe ser de un fichero y la de destino tendrá que ser el directorio en el que se quiere copiar el fichero.

-------------------------------------------------------------------------------
NOMBRES DE LOS PARTICIPANTES
Gian Lucas Martín Chamorro.
