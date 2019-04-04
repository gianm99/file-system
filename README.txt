MEJORAS REALIZADAS
Hemos realizado dos mejoras en el algoritmo de la función liberar_bloques_inodo(). 
La primera consiste en comprobar el valor de los punteros indirectos del inodo y si vale 0 se salta al siguiente rango. Si es el último rango, sale del bucle.
La segunda consiste en que cuando se encuentra un puntero con valor 0 al recorrer el árbol del inodo se comprueba si se trata de un puntero que debería apuntar a un bloque de punteros y, si es el caso, se los salta.

RESTRICCIONES DEL PROGRAMA
Ninguna.

SINTAXIS ESPECÍFICA
Ninguna.

NOMBRES DE LOS PARTICIPANTES
Gian Lucas Martín Chamorro, Juanjo Puya Milián y Alejandro Cortés Fernández.
