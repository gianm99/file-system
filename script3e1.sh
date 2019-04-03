echo "################################################################################"
echo "$ rm disco"
rm disco
echo "$ ./mi_mkfs disco 100000"
./mi_mkfs disco 100000
echo "################################################################################"
echo "$ ./escribir disco "$(cat texto2.txt)" 0"
echo "#escribimos el texto contenido en text2.txt en los offsets  0, 5120, 256000, "
echo "#30720000 y 71680000, del mismo inodo"
./escribir disco "$(cat texto2.txt)" 0
echo "################################################################################"
echo "$ ./leer disco 1 > ext4.txt"
echo "#leemos el contenido del inodo 1 (escrito en el offset 0) y lo direccionamos"
echo "#al fichero externo ext4.txt"
./leer disco 1 > ext4.txt
echo "################################################################################"
echo "$ ls -l ext4.txt"
echo "#comprobamos cuánto ocupa el fichero externo ext4.txt"
echo "#(ha de coincidir con el tamaño en bytes lógico del inodo 1 y con total_leidos)"
ls -l ext4.txt
echo "################################################################################"
echo "$ ./leer_sf disco"
echo "#mostramos los campos del superbloque"
./leer_sf disco
echo "################################################################################"
echo "$ ./truncar"
echo "#mostramos la sintaxis de truncar"
./truncar
echo "################################################################################"
echo "$ ./truncar disco 1 100"
echo "#truncamos el inodo 1 (escrito en el offset 0) en el byte 100"
./truncar disco 1 100
echo "################################################################################"
echo "$ ./leer_sf disco"
echo "#mostramos de nuevo el SB para comprobar cantidad bloques libres"
./leer_sf disco
echo "################################################################################"
echo "$ ./leer disco 1"
echo "#leemos el inodo 1 truncado"
./leer disco 1
echo "################################################################################"
echo "$ ./truncar disco 1 200"
echo "#intentamos truncar más allá de EOF"
./truncar disco 1 200
echo "################################################################################"
echo "$ ./permitir"
echo "#mostramos sintaxis de permitir"
./permitir
echo "################################################################################"
echo "$ ./permitir disco 1 0"
echo "#cambiamos permisos del inodo 1 a 0"
./permitir disco 1 0
echo "################################################################################"
echo "$ ./leer disco 1"
echo "#intentamos leer inodo 1 con permisos=0"
./leer disco 1
echo "################################################################################"
echo "$ ./truncar disco 1 50"
echo "#intentamos truncar inodo 1 con permisos=0"
./truncar disco 1 50
