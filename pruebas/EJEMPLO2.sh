echo "ESTE SCRIPT PRUEBA LA VERSIÓN SIMPLE DE mi_ls.c, EL USO NOMBRES DE FICHEROS EXTERNOS COMO PARAMETRO PARA ESCRIBIR Y LA COPIA DE FICHEROS"
echo "######################################################################"
echo "./mi_ls"
./mi_ls
echo "./mi_ls disco / -s"
./mi_ls disco / -s
echo "./leer_sf disco"
./leer_sf disco
echo "Creamos un fichero y le escribimos el contenido del fichero externo texto2.txt (Lorem Ipsum)"
echo "./mi_touch disco 6 /dir1/file_prueba"
./mi_touch disco 6 /dir1/file_prueba
echo "./mi_escribir_f"
./mi_escribir_f
echo "./mi_escribir_f disco /dir1/file_prueba texto2.txt 0"
./mi_escribir_f disco /dir1/file_prueba texto2.txt 0
echo "./mi_cat disco /dir1/file_prueba"
./mi_cat disco /dir1/file_prueba
echo "######################################################################"
echo "Ahora comprobamos la copia de ese fichero"
echo "./mi_cp_f"
./mi_cp_f
echo "Comprobamos que no se puede copiar un fichero a una ruta de fichero"
echo "./mi_cp_f disco /dir1/file_prueba /dir1/file13"
./mi_cp_f disco /dir1/file_prueba /dir1/file13
echo "Comprobamos que no se puede copiar un fichero al mismo directorio en el que está ahora"
echo "./mi_cp_f disco /dir1/file_prueba /dir1/"
./mi_cp_f disco /dir1/file_prueba /dir1/
echo "./mi_cp_f disco /dir1/file_prueba /dir1/dir11/"
./mi_cp_f disco /dir1/file_prueba /dir1/dir11/
echo "./mi_ls disco /dir1/dir11/"
./mi_ls disco /dir1/dir11/
echo "./mi_cat disco /dir1/dir11/file_prueba"
./mi_cat disco /dir1/dir11/file_prueba
echo "./leer_sf disco"
./leer_sf disco

