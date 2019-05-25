CC=gcc
CFLAGS=-c -g -Wall -std=gnu99
LDFLAGS=-pthread

SOURCES=bloques.c ficheros_basico.c ficheros.c directorios.c mi_mkfs.c leer_sf.c mi_cat.c mi_chmod.c mi_escribir.c mi_ls.c mi_mkdir.c mi_stat.c mi_touch.c mi_link.c mi_rm.c semaforo__mutex_posix.c#todos los .c
LIBRARIES=bloques.o ficheros_basico.o ficheros.o directorios.o semaforo__mutex_posix.o #todos los .o de la biblioteca del SF
INCLUDES=bloques.h ficheros_basico.h ficheros.h directorios.h semaforo__mutex_posix.h #todos los .h
PROGRAMS=mi_mkfs leer_sf mi_cat mi_chmod mi_escribir mi_ls mi_mkdir mi_stat mi_touch mi_link mi_rm
OBJS=$(SOURCES:.c=.o)

all: $(OBJS) $(PROGRAMS)

$(PROGRAMS): $(LIBRARIES) $(INCLUDES)
	$(CC) $(LDFLAGS) $(LIBRARIES) $@.o -o $@

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm -rf *.o *~ $(PROGRAMS)