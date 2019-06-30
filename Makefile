CC=gcc
CFLAGS=-c -g -Wall -std=gnu99
LDFLAGS=-pthread

SOURCES=bloques.c ficheros_basico.c ficheros.c directorios.c mi_mkfs.c leer_sf.c mi_cat.c mi_chmod.c mi_escribir.c mi_escribir_f.c mi_ls.c mi_mkdir.c mi_stat.c mi_touch.c mi_link.c mi_rm.c mi_rmdir.c mi_rn.c mi_cp_f.c semaforo_mutex_posix.c simulacion.c verificacion.c
LIBRARIES=bloques.o ficheros_basico.o ficheros.o directorios.o semaforo_mutex_posix.o
INCLUDES=bloques.h ficheros_basico.h ficheros.h directorios.h semaforo_mutex_posix.h simulacion.h verificacion.h
PROGRAMS=mi_mkfs leer_sf mi_cat mi_chmod mi_escribir mi_escribir_f mi_ls mi_mkdir mi_stat mi_touch mi_link mi_rm mi_rmdir mi_rn mi_cp_f simulacion verificacion
OBJS=$(SOURCES:.c=.o)

all: $(OBJS) $(PROGRAMS)

$(PROGRAMS): $(LIBRARIES) $(INCLUDES)
	$(CC) $(LDFLAGS) $(LIBRARIES) $@.o -o $@

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf *.o *~ $(PROGRAMS)
