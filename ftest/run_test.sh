#!/bin/bash

# Función para ejecutar el programa con un conjunto específico de parámetros
run_test() {
    local num_iter=$1
    local input=$2
    local output=$3

    # Ejecutar el programa
    ../build/fluid/fluid $num_iter $input $output

    # Verificar el código de salida
    local exit_code=$?
    if [ $exit_code -eq 0 ]; then
        echo -e "\033[0;32mTest Passed:\033[0m fluid $num_iter $input $output"
    else
        echo -e "\033[0;31mTest Failed:\033[0m fluid $num_iter $input $output"
    fi

}

# Pruebas de analisis de argumentos que deben dar fallo (TEST FAILED)
run_test                                                            #Se pasan 0 argumentos
run_test 10                                                         #Se pasa 1 argumento
run_test hola ../files/small.fld                                 #Se pasan 2 argumentos
run_test hola ../files/small.fld ../../files/results/small-3.fld #pasos de tiempo no es un numero entero (string)
run_test 10.5 ../files/small.fld ../../files/results/small-3.fld #pasos de tiempo no es un numero entero (decimal)
run_test -10 ../files/small.fld ../../files/results/small-3.fld  #pasos de tiempo indicado es negativo
run_test 10 ../files/no_existe.fld ../../files/results/small-3.fld #archivo de lectura inexistente, por lo que no se puede abrir para su lectura
run_test 10 ../files/small.fld ../../files/results/no_existe.fld #archivo de escritura inexistente, por lo que no se puede acceder para su escritura

#Pruebas de simulacion que deben ser validas y finalicen con exito (TEST PASSED) para el uso de los archivos pequenos small.fld
run_test 1 ../files/small.fld ../files/results/small-1.fld   #Simulacion para una unica iteracion
run_test 100 ../files/small.fld ../files/results/small-2.fld    #Simulacion para multiples iteraciones (100)
#run_test 1000 ../files/small.fld ../files/results/small-3.fld   #Simulacion para multiples iteraciones (1000) con uso de un archivo de mayor tamaño

#Pruebas de simulacion que deben ser validas y finalicen con exito (TEST PASSED) para el uso de los archivos grandes large.fld
run_test 1 ../files/large.fld ../files/results/large-1.fld
run_test 100 ../files/large.fld ../files/results/large-2.fld
run_test 1000 ../files/large.fld ../files/results/large-1000.fld

#Con todas ellas se puede apreciar que cada uno de los archivos de salida en los que se escriben, se ven modificados con cada ejecucion
