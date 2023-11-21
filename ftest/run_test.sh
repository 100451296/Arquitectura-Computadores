#!/bin/bash

# Variables para almacenar los resultados esperados
expected_result_params=(-1 -1 -1 -1 -1 -2 -3 -4)
expected_result_num_particles=(-5 -5 -5)
expected_result_simulation=(0 0 0)

# Contadores de tests exitosos y totales
successful_tests=0
total_tests=0

# Función para modificar el número de partículas en el archivo .fld
modify_particles() {
    local input_file=$1
    local new_num_particles=$2

    # Convertir el número de partículas a formato hexadecimal
    local hex_value
    if [ "$new_num_particles" -eq 0 ]; then
        hex_value="00000000"
    elif [ "$new_num_particles" -lt 0 ]; then
        hex_value=$(printf "%08x" $(( -new_num_particles | 0x80000000 )))
    else
        hex_value=$(printf "%08x" $(( (RANDOM % 0x7FFFFFFF) + 1 )))
    fi

    # Modificar el número de partículas en el archivo .fld
    echo -ne "$(echo $hex_value | tac -rs..)" | xxd -r -p | dd of="$input_file" bs=1 seek=4 count=4 conv=notrunc
}

# Función para ejecutar el programa con un conjunto específico de parámetros y verificar el resultado
run_and_verify() {
    local expected_result=$1
    local num_iter=$2
    local input=$3
    local output=$4
    

    # Ejecutar el programa
    ../build/fluid/fluid $num_iter "$input" "$output"
    # Verificar el código de salida
    local exit_code=$?
    if [ $exit_code -eq $expected_result ]; then
        echo -e "\033[0;32mTest Passed:\033[0m fluid $num_iter $input $output"
        ((successful_tests++))
    else
        echo -e "\033[0;31mTest Failed:\033[0m fluid $num_iter $input $output"
    fi
    ((total_tests++))
}

run_tests_parameters() {
    # Pruebas de análisis de argumentos que deben dar fallo (TEST FAILED)
    run_and_verify  ${expected_result_params[1]}
    run_and_verify 10 ${expected_result_params[1]}
    run_and_verify hola ../files/small.fld ${expected_result_params[2]}
    run_and_verify hola ../files/small.fld ../../files/results/small-3.fld ${expected_result_params[3]}
    run_and_verify 10.5 ../files/small.fld ../../files/results/small-3.fld ${expected_result_params[4]}
    run_and_verify -10 ../files/small.fld ../../files/results/small-3.fld ${expected_result_params[5]}
    run_and_verify 10 ../files/no_existe.fld ../../files/results/small-3.fld ${expected_result_params[6]}
    run_and_verify 10 ../files/small.fld ../../files/results/no_existe.fld ${expected_result_params[7]}
}

run_tests_num_particles() {
    # Pruebas de comprobación del valor de num_particles especificado en la cabecera del archivo. El valor se modifica en un archivo llamado small_malo.fld
    modify_particles ./files/archivos_pruebas/small_malo.fld 0
    run_and_verify 1 ./files/archivos_pruebas/small_malo.fld ../files/results/small-1.fld ${expected_result_num_particles[0]}
    modify_particles ../files/archivos_pruebas/small_malo.fld -123
    run_and_verify 1 ../files/archivos_pruebas/small_malo.fld ../files/results/small-1.fld ${expected_result_num_particles[1]}
    modify_particles ../files/archivos_pruebas/pepe.fld
    run_and_verify 1 ../files/archivos_pruebas/pepe.fld ../files/results/small-1.fld ${expected_result_num_particles[2]}
}

run_tests_simulacion() {
    # Pruebas de simulación que deben ser válidas y finalizar con éxito (TEST PASSED) para el uso de los archivos pequeños small.fld
    run_and_verify 1 ../files/small.fld ../files/results/small-1.fld ${expected_result_simulation[0]}
    #run_and_verify 100 ../files/small.fld ../files/results/small-2.fld ${expected_result_simulation[1]}
    #run_and_verify 1000 ../files/small.fld ../files/results/small-3.fld ${expected_result_simulation[2]}

    # Pruebas de simulación que deben ser válidas y finalizar con éxito (TEST PASSED) para el uso de los archivos grandes large.fld
    run_and_verify 1 ../files/large.fld ../files/results/large-1.fld ${expected_result_simulation[0]}
    #run_and_verify 100 ../files/large.fld ../files/results/large-2.fld ${expected_result_simulation[1]}
    #run_and_verify 1000 ../files/large.fld ../files/results/large-1000.fld ${expected_result_simulation[2]}
}

# Función para ejecutar todas las pruebas y mostrar el resultado final
run_all_tests() {
    run_tests_parameters
    run_tests_num_particles
    run_tests_simulacion

    echo -e "\nTotal de tests exitosos: $successful_tests de $total_tests"
}

# Ejecutar todas las pruebas al ejecutar el script
run_all_tests

