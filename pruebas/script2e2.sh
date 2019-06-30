echo "***Script para comprobar enlaces y borrado de ficheros y directorios***"
echo
rm disco
echo "$ ./mi_mkfs disco 100000"
./mi_mkfs disco 100000
echo "######################################################################"
echo "$ ./mi_mkdir disco 6 dir1/" #sintaxis incorrecta
./mi_mkdir disco 6 dir1/
echo "$ ./mi_mkdir disco 6 /dir1/"
./mi_mkdir disco 6 /dir1/
echo "$ ./mi_mkdir disco 6 /dir1/dir11/"
./mi_mkdir disco 6 /dir1/dir11/
echo "$ ./mi_mkdir disco 6 /dir1/dir12/"
./mi_mkdir disco 6 /dir1/dir12/
echo "$ ./mi_touch disco 6 /dir1/dir11/fic1"
./mi_touch disco 6 /dir1/dir11/fic1
echo "######################################################################"
echo "$ ./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "######################################################################"
echo "$ ./mi_mkdir disco 6 /dir2/"
./mi_mkdir disco 6 /dir2/
echo "$ ./mi_mkdir disco 6 /dir2/dir21/"
./mi_mkdir disco 6 /dir2/dir21/
echo "######################################################################"
echo "$ ./mi_escribir"
./mi_escribir
echo "$ ./mi_escribir disco /dir1/dir11/fic1 "hoy puede ser un gran dia" 0"
./mi_escribir disco /dir1/dir11/fic1 "hoy puede ser un gran dia" 0
echo "$ ./mi_cat disco /dir1/dir11/fic1"
./mi_cat disco /dir1/dir11/fic1
echo "######################################################################"
echo "$ ./mi_link disco /dir1/dir11/fic1 /dir2/dir21/fic2"
echo "El contenido de /dir2/dir21/fic2 ha de pasar a ser el mismo de "
echo "/dir1/dir11/fic1 y tiene que haberse incrementado el nº de enlaces"
./mi_link disco /dir1/dir11/fic1 /dir2/dir21/fic2
echo "$ ./mi_stat disco /dir1/dir11/fic1"
./mi_stat disco /dir1/dir11/fic1
echo "$ ./mi_cat disco /dir2/dir21/fic2"
./mi_cat disco /dir2/dir21/fic2
echo "######################################################################"
echo "$ ./mi_mkdir disco 6 /dir3/"
./mi_mkdir disco 6 /dir3/
echo "######################################################################"
echo "$ ./mi_ls disco /"
./mi_ls disco /
echo "######################################################################"
echo "$ ./mi_link disco /dir1/dir11/fic1 /dir3/fic4"
echo "El contenido de /dir3/fic4 ha de pasar a ser el mismo de /dir1/dir11/fic1"
echo "y tiene que haberse incrementado el nº de enlaces del inodo común"
./mi_link disco /dir1/dir11/fic1 /dir3/fic4
echo "$ ./mi_stat disco  /dir3/fic4"
./mi_stat disco  /dir3/fic4
echo "######################################################################"
echo "$ ./mi_touch disco 6 /dir3/fic3"
./mi_touch disco 6 /dir3/fic3
echo "$ ./mi_escribir disco /dir3/fic3 "!A por ello!" 5120"
./mi_escribir disco /dir3/fic3 "!A por ello!" 5120
echo "$ ./mi_ls disco /dir3/"
./mi_ls disco /dir3/
echo "######################################################################"
echo "$ ./mi_ls disco /"
./mi_ls disco /
echo "######################################################################"
echo "$ ./mi_rm disco /dir1/"
./mi_rm disco /dir1/
echo "######################################################################"
echo "$ ./mi_rm disco /dir3/fic3"
./mi_rm disco /dir3/fic3
echo "######################################################################"
echo "$ ./mi_ls disco /dir3/"
./mi_ls disco /dir3/
echo "######################################################################"
echo "$ ./mi_rm disco /dir3/fic4"
./mi_rm disco /dir3/fic4
echo "######################################################################"
echo "$ ./mi_ls disco /dir3/"
./mi_ls disco /dir3/
echo "######################################################################"
echo "$ ./mi_rm disco /dir2/dir21/"
./mi_rm disco /dir2/dir21/
echo "$ ./mi_ls disco /dir2/dir21/"
./mi_ls disco /dir2/dir21/
echo "$ ./mi_rm disco /dir2/dir21/fic2"
./mi_rm disco /dir2/dir21/fic2
echo "$ ./mi_rm disco /dir2/dir21/"
./mi_rm disco /dir2/dir21/
echo "$ ./mi_ls disco /dir2/"
./mi_ls disco /dir2/
echo "$ ./mi_rm disco /dir2/"
./mi_rm disco /dir2/
echo "$ ./mi_ls disco /"
./mi_ls disco /
echo "######################################################################"
echo "$ ./mi_rm disco /dir3/"
./mi_rm disco /dir3/
echo "$ ./mi_ls disco /"
./mi_ls disco /
echo "$ ./mi_stat disco /"
./mi_stat disco /
echo "######################################################################"
echo "$ ./mi_rm disco /dir1/dir11/fic1"
./mi_rm disco /dir1/dir11/fic1
echo "$ ./mi_ls disco /dir1/dir11/"
./mi_ls disco /dir1/dir11/
echo "$ ./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "######################################################################"
echo "$ ./mi_ls disco /"
./mi_ls disco /
echo "$ ./mi_stat disco /"
./mi_stat disco /
echo "$ ./mi_ls disco /dir1/"
./mi_ls disco /dir1/
echo "$ ./mi_stat disco /dir1/"
./mi_stat disco /dir1/
echo "$ ./mi_stat disco /dir1/dir11/"
./mi_stat disco /dir1/dir11/
echo "$ ./mi_stat disco /dir1/dir12/"
./mi_stat disco /dir1/dir12/
echo "######################################################################"
echo "$ ./leer_sf disco"
./leer_sf disco
