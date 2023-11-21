#!/bin/bash

# Función para modificar el número de partículas en el archivo .fld
modify_particles() {
    local input_file=$1
    local new_num_particles=$2

    # Convertir el número de partículas a formato hexadecimal
    local hex_value
    if [ "$new_num_particles" -eq 0 ]; then
        hex_value="00000000"
    elif [ "$new_num_particles" -lt 0 ]; then
        # Manejar el caso de un número negativo (cambiar signo y establecer el bit de signo)
        hex_value=$(printf "%08x" $(( -new_num_particles | 0x80000000 )))
    else
        # Número positivo aleatorio mayor que cero
        hex_value=$(printf "%08x" $(( (RANDOM % 0x7FFFFFFF) + 1 )))
    fi

    # Modificar el número de partículas en el archivo .fld
    echo -ne "$(echo $hex_value | tac -rs..)" | xxd -r -p | dd of="$input_file" bs=1 seek=4 count=4 conv=notrunc
}

# Función para ejecutar el programa con un conjunto específico de parámetros y verificar el resultado
run_and_verify() {
    local num_iter=$1
    local input=$2
    local output=$3

    # Ejecutar el programa
    ../build/fluid/fluid $num_iter "$input" "$output"
    # Verificar el código de salida
    local exit_code=$?
    if [ $exit_code -eq 0 ]; then
        echo -e "\033[0;32mTest Passed:\033[0m fluid $num_iter $input $output"
    else
        echo -e "\033[0;31mTest Failed:\033[0m fluid $num_iter $input $output"
    fi
}

run_tests_parameters() {
    # Pruebas de análisis de argumentos que deben dar fallo (TEST FAILED)
    run_and_verify && ((successful_tests++)) # Se pasan 0 argumentos
    run_and_verify 10 && ((successful_tests++)) # Se pasa 1 argumento
    run_and_verify hola ../files/small.fld && ((successful_tests++)) # Se pasan 2 argumentos
    run_and_verify hola ../files/small.fld ../../files/results/small-3.fld && ((successful_tests++)) # pasos de tiempo no es un número entero (string)
    run_and_verify 10.5 ../files/small.fld ../../files/results/small-3.fld && ((successful_tests++)) # pasos de tiempo no es un número entero (decimal)
    run_and_verify -10 ../files/small.fld ../../files/results/small-3.fld && ((successful_tests++)) # pasos de tiempo indicado es negativo
    run_and_verify 10 ../files/no_existe.fld ../../files/results/small-3.fld && ((successful_tests++)) # archivo de lectura inexistente, no se puede abrir para su lectura
    run_and_verify 10 ../files/small.fld ../../files/results/no_existe.fld && ((successful_tests++)) # archivo de escritura inexistente, no se puede acceder para su escritura
}

run_tests_num_particles() {
    # Pruebas de comprobación del valor de num_particles especificado en la cabecera del archivo. El valor se modifica en un archivo llamado small_malo.fld
    modify_particles ./files/archivos_pruebas/small_malo.fld 0 # Comprobación modificando el valor del num_particles a 0
    run_and_verify 1 ./files/archivos_pruebas/small_malo.fld ../files/results/small-1.fld && ((successful_tests++)) # Simulación para una única iteración
    modify_particles ../files/archivos_pruebas/small_malo.fld -123 # Comprobación modificando el valor del num_particles a un número negativo.
    run_and_verify 1 ../files/archivos_pruebas/small_malo.fld ../files/results/small-1.fld && ((successful_tests++)) # Simulación para una única iteración
    modify_particles ../files/archivos_pruebas/pepe.fld  # Comprobación modificando el valor del num_particles a un número positivo mayor que 0 completamente aleatorio
    run_and_verify 1 ../files/archivos_pruebas/pepe.fld ../files/results/small-1.fld && ((successful_tests++)) # Simulación para una única iteración
}

run_tests_simulacion() {
    # Pruebas de simulación que deben ser válidas y finalizar con éxito (TEST PASSED) para el uso de los archivos pequeños small.fld
    run_and_verify 1 ../files/small.fld ../files/results/small-1.fld && ((successful_tests++))
    #run_and_verify 100 ../files/small.fld ../files/results/small-2.fld && ((successful_tests++)) # Simulación para múltiples iteraciones (100)
    #run_and_verify 1000 ../files/small.fld ../files/results/small-3.fld && ((successful_tests++)) # Simulación para múltiples iteraciones (1000) con uso de un archivo de mayor tamaño

    # Pruebas de simulación que deben ser válidas y finalizar con éxito (TEST PASSED) para el uso de los archivos grandes large.fld
    run_and_verify 1 ../files/large.fld ../files/results/large-1.fld && ((successful_tests++))
    #run_and_verify 100 ../files/large.fld ../files/results/large-2.fld && ((successful_tests++))
    #run_and_verify 1000 ../files/large.fld ../files/results/large-1000.fld && ((successful_tests++))
}

# Función para ejecutar todas las pruebas y contar los tests exitosos
run_all_tests() {
    local successful_tests=0
    run_tests_parameters
    run_tests_num_particles
    run_tests_simulacion

    echo -e "\nTotal de tests exitosos: $successful_tests"
}

# Ejecutar todas las pruebas al ejecutar el script
run_all_tests
