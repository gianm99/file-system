echo "ESTE SCRIPT PRUEBA EL BORRADO DE DIRECTORIOS VACÍOS Y EL CAMBIO DE NOMBRES"
echo "######################################################################"
echo "./leer_sf disco"
./leer_sf disco
echo "Borramos el directorio /dir1/dir11/ y comprobamos cómo queda el disco después"
echo "./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "./mi_ls disco /dir1/dir11/"
./mi_ls disco /dir1/dir11/
echo "./mi_rmdir"
./mi_rmdir
echo "Borramos el directorio /dir1/dir11/, que está vacío"
echo "./mi_rmdir disco /dir1/dir11/"
./mi_rmdir disco /dir1/dir11/
echo "./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "./leer_sf disco"
./leer_sf disco
echo "######################################################################"
echo "También comprobamos como no se pueden borrar directorios que no estén vacíos y tampoco ficheros"
echo "./mi_rmdir disco /dir1/"
./mi_rmdir disco /dir1/
echo "./mi_ls disco /"
./mi_ls disco /
echo "./mi_rmdir disco /dir1/file13"
./mi_rmdir disco /dir1/file13
echo "./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "######################################################################"
echo "Le cambiamos el nombre al directorio /dir1/dir12/ por /dir1/dir12_nuevo/"
echo "./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "./mi_rn"
./mi_rn
echo "./mi_rn disco /dir1/dir12/ /dir1/dir12_nuevo/"
./mi_rn disco /dir1/dir12/ /dir1/dir12_nuevo/
echo "./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "######################################################################"
echo "Y cambiamos el nombre al fichero /dir1/file13 por /dir1/cambiado_de_nombre"
echo "./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "./mi_rn disco /dir1/file13 /dir1/cambiado_de_nombre"
./mi_rn disco /dir1/file13 /dir1/cambiado_de_nombre
echo "./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "######################################################################"
echo "Comprobamos como no se puede usar un nombre que ya esté en el directorio"
echo "./mi_ls disco /dir2/"
./mi_ls disco /dir2/
echo "./mi_rn disco /dir2/dir21/ /dir2/dir22/"
./mi_rn disco /dir2/dir21/ /dir2/dir22/
echo "./mi_ls disco /dir2/"
./mi_ls disco /dir2/
echo "Y que no se puede pasar de directorio a fichero o viceversa"
echo "./mi_rn disco /dir2/dir21/ /dir2/file"
./mi_rn disco /dir2/dir21/ /dir2/file
echo "./mi_ls disco /dir2/"
./mi_ls disco /dir2/
echo "./mi_rn disco /dir3/file31 /dir3/directory/"
./mi_rn disco /dir3/file31 /dir3/directory/
echo "./mi_ls disco /dir3/"
./mi_ls disco /dir3/
echo "######################################################################"
echo "También se comprueba que los dos caminos tienen el mismo directorio padre"
echo "./mi_rn disco /dir2/dir21/ /dir3/dir22/"
./mi_rn disco /dir2/dir21/ /dir3/dir22/
echo "./mi_ls disco /dir2/"
./mi_ls disco /dir2/
