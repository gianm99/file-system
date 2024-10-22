echo "################################################################################"
echo "$ rm disco"
rm disco
echo "$ ./mi_mkfs disco 100000"
echo "#inicializamos el sistema de ficheros con 100.000 bloques"
./mi_mkfs disco 100000
echo "################################################################################"
echo "$ ./leer_sf disco"
echo "#mostramos solo el SB"
./leer_sf disco
echo "################################################################################"
echo "$ ./escribir"
echo "#consultamos sintaxis comando"
./escribir
echo "################################################################################"
echo "$ ./escribir disco "123456789" 0"
echo "#escribimos el texto “123456789” en los offsets  0, 5120, 256000, 30720000 y "
echo "#71680000 de un mismo inodo"
./escribir disco "123456789" 0
echo "################################################################################"
echo "$ ./leer disco 1 > ext1.txt"
echo "#leemos el contenido del inodo 1 y lo direccionamos al fichero externo ext1.txt"
./leer disco 1 > ext1.txt
echo "################################################################################"
echo "$ ls -l ext1.txt"
echo "#comprobamos cuánto ocupa el fichero externo"
echo "#(ha de coincidir con el tamaño en bytes lógico del inodo y con los bytes leídos)"
ls -l ext1.txt
echo "################################################################################"
echo "$ ./escribir disco "123456789" 1"
echo "#escribimos el texto “123456789” en los offsets  0, 5120, 256000, 30720000 y "
echo "#71680000, de inodos diferentes"
./escribir disco "123456789" 1
echo "################################################################################"
echo "$ ./leer disco 3 > ext2.txt" 
echo "#leemos el contenido del inodo 3 (escrito en el offset 5120) y lo direccionamos"
echo "#al fichero externo ext2.txt"
./leer disco 3 > ext2.txt
echo "################################################################################"
echo "$ ls -l ext2.txt"
echo "#comprobamos cuánto ocupa el fichero externo ext2.txt"
echo "#(ha de coincidir con el tamaño en bytes lógico del inodo 3 y con total_leidos)"
ls -l ext2.txt
echo "################################################################################"
echo "$ cat ext2.txt"
echo "#usamos el comando cat del sistema para leer el contenido del fichero externo"
cat ext2.txt
echo
echo "################################################################################"
echo "$ ./leer disco 3"
echo "#leemos el contenido de nuestro inodo 3"
echo "#(ha de contener lo mismo que el fichero externo ext2.txt)"
./leer disco 3
echo "################################################################################"
echo "$ ./leer disco 6 > ext3.txt"
echo "#leemos todo el contenido del inodo 6 (escrito en el offset 71680000) y lo "
echo "#direccionamos al fichero externo ext3.txt"
./leer disco 6 > ext3.txt
echo "################################################################################"
echo "$ ls -l ext3.txt"
echo "#comprobamos cuánto ocupa el fichero externo ext3.txt"
echo "#(ha de coincidir con el tamaño en bytes lógico del inodo 6 y con total_leidos)"
ls -l ext3.txt
echo "################################################################################"
echo "$ cat ext3.txt"
echo "#usamos el comando cat del sistema para leer el contenido del fichero externo"
cat ext3.txt
echo
echo "################################################################################"
echo "$ ./leer disco 6"
echo "#leemos el contenido de nuestro inodo 6"
echo "#(ha de contener lo mismo que el fichero externo ext3.txt)"
./leer disco 6
