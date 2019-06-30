rm disco
echo "ESTE SCRIPT CREA VARIOS DIRECTORIOS Y FICHEROS, CON INFORMACIÓN EN VARIOS OFFSETS CON EL PROPÓSITO DE HACER PRUEBAS"
echo "######################################################################"
echo "$ ./mi_mkfs disco 100000"
./mi_mkfs disco 100000
echo "######################################################################"
echo "Creamos un set de directorios y ficheros para hacer pruebas"
echo "./mi_mkdir disco 6 /dir1/"
./mi_mkdir disco 6 /dir1/
echo "./mi_mkdir disco 6 /dir1/dir11/"
./mi_mkdir disco 6 /dir1/dir11/
echo "./mi_mkdir disco 6 /dir1/dir12/"
./mi_mkdir disco 6 /dir1/dir12/
echo "./mi_touch disco 6 /dir1/dir12/file11/"
./mi_touch disco 6 /dir1/dir12/file11
echo "./mi_touch disco 6 /dir1/dir12/file12/"
./mi_touch disco 6 /dir1/dir12/file12
echo "./mi_mkdir disco 6 /dir1/dir13/"
./mi_mkdir disco 6 /dir1/dir13/
echo "./mi_touch disco 6 /dir1/file13/"
./mi_touch disco 6 /dir1/file13
echo "./mi_mkdir disco 6 /dir2/"
./mi_mkdir disco 6 /dir2/
echo "./mi_mkdir disco 6 /dir2/dir21/"
./mi_mkdir disco 6 /dir2/dir21/
echo "./mi_touch disco 6 /dir2/dir21/file21"
./mi_touch disco 6 /dir2/dir21/file21
echo "./mi_mkdir disco 6 /dir2/dir22/"
./mi_mkdir disco 6 /dir2/dir22/
echo "./mi_touch disco 6 /dir2/dir22/file22"
./mi_touch disco 6 /dir2/dir22/file22
echo "./mi_mkdir disco 6 /dir2/dir23/"
./mi_mkdir disco 6 /dir2/dir23/
echo "./mi_mkdir disco 6 /dir3/"
./mi_mkdir disco 6 /dir3/
echo "./mi_touch disco 6 /dir3/file31"
./mi_touch disco 6 /dir3/file31
echo "./mi_mkdir disco 6 /dir3/dir31/"
./mi_mkdir disco 6 /dir3/dir31/
echo "./mi_mkdir disco 6 /dir3/dir32/"
./mi_mkdir disco 6 /dir3/dir32/
echo "./mi_mkdir disco 6 /dir3/dir33/"
./mi_mkdir disco 6 /dir3/dir33/
echo "######################################################################"
echo "Metemos información en diferentes offsets de los ficheros"
echo "./mi_escribir disco /dir1/dir12/file11 hola que tal esta - 0"
./mi_escribir disco /dir1/dir12/file11 "hola que tal esta - " 0
echo "./mi_escribir disco /dir1/dir12/file11 Hoy hace un buen dia para comer dulces bajo de la luz de la luna llena - 5120 "
./mi_escribir disco /dir1/dir12/file11 "Hoy hace un buen dia para comer dulces bajo de la luz de la luna llena - " 5120
echo "./mi_escribir disco /dir1/dir12/file11 A mi madre le encanta cocinar para mi - 48000 "
./mi_escribir disco /dir1/dir12/file11 "A mi madre le encanta cocinar para mi - " 48000
echo "./mi_escribir disco /dir1/dir12/file12 uno y uno son dos - 23"
./mi_escribir disco /dir1/dir12/file12 "uno y uno son dos - " 23
echo "./mi_escribir disco /dir1/dir12/file12 Hoy hace un buen dia para comer dulces bajo de la luz de la luna llena - 90000"
./mi_escribir disco /dir1/dir12/file12 "Hoy hace un buen dia para comer dulces bajo de la luz de la luna llena - " 90000
echo "./mi_escribir disco /dir1/dir12/file12 A mi madre le encanta cocinar para mi - 15230"
./mi_escribir disco /dir1/dir12/file12 "A mi madre le encanta cocinar para mi - " 15230
echo "./mi_escribir disco /dir1/file13 uno y uno son dos - 666"
./mi_escribir disco /dir1/file13 "uno y uno son dos - " 666
echo "./mi_escribir disco /dir1/file13 Hoy hace un buen dia para comer dulces bajo de la luz de la luna llena- 6666"
./mi_escribir disco /dir1/file13 "Hoy hace un buen dia para comer dulces bajo de la luz de la luna llena - " 6666
echo "./mi_escribir disco /dir1/file13 A mi madre le encanta cocinar para mi - 66666"
./mi_escribir disco /dir1/file13 "A mi madre le encanta cocinar para mi - " 66666
echo "./mi_escribir disco /dir2/dir21/file21 el patio de mi casa es particular - 88"
./mi_escribir disco /dir2/dir21/file21 "el patio de mi casa es particular - " 88
echo "./mi_escribir disco /dir2/dir22/file22 las cortinas de su casa no son particulares - 3333"
./mi_escribir disco /dir2/dir22/file22 "las cortinas de su casa no son particulares - " 3333
echo "./mi_escribir disco /dir3/file31 que la suerte te acompañe - 777"
./mi_escribir disco /dir3/file31 "que la suerte te acompañe - " 777
echo "######################################################################"
echo "Comprobamos el contenido de algunos directorios"
echo "$ ./mi_ls disco /"
./mi_ls disco /
echo "./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "./mi_ls disco /dir1/dir11/"
./mi_ls disco /dir1/dir11/
echo "./mi_ls disco /dir1/dir12/file11"
./mi_ls disco /dir1/dir12/file11
echo "./mi_ls disco /dir2/dir21/"
./mi_ls disco /dir2/dir21/
echo "./mi_ls disco /dir2/dir21/file21"
./mi_ls disco /dir2/dir21/file21
echo "./mi_ls disco /dir2/dir22/file22"
./mi_ls disco /dir2/dir22/file22
echo "./mi_ls disco /dir2/dir23/"
./mi_ls disco /dir2/dir23/
echo "./mi_ls disco /dir3/"
./mi_ls disco /dir3/
