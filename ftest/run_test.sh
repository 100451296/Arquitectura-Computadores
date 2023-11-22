#!/bin/bash

successful_tests=0
total_tests=0

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

run_and_verify() {
    local expected_error=$1
    shift  # Eliminar el primer argumento (mensaje de error esperado)
    
    # Ejecutar el programa
    local error_message
    error_message=$("../build/fluid/fluid" "$@" 2>&1 >/dev/null)

    # Verificar si el mensaje de error contiene el texto esperado
    if  [ $exit_code -eq 0 ] || [[ "$error_message" == *"$expected_error"* ]]; then
        echo -e "\033[0;32mTest Passed:\033[0m fluid $*"
        echo "Expected error message: $expected_error"
        echo "Actual error message: $error_message"
        ((successful_tests++))
    else
        echo -e "\033[0;31mTest Failed:\033[0m fluid $*"
        echo "Expected error message: $expected_error"
        echo "Actual error message: $error_message"
    fi

    ((total_tests++))
}

run_tests_parameters() {
    run_and_verify "Error: Invalid number of arguments: 0"  
    run_and_verify "Error: Invalid number of arguments: 1" 10 
    run_and_verify "Error: Invalid number of arguments: 2" 10 ../files/small.fld 
    run_and_verify "Error: Invalid number of arguments: 4" 10 ../files/small.fld ../../files/results/small-3.fld 40
    #run_and_verify "Error: time steps must be numeric." 10 ../files/small.fld ../../files/results/small-3.fld 40 #Se usaba como prueba para ver si detecta un test fallido
    run_and_verify "Error: time steps must be numeric." hola ../files/small.fld ../../files/results/small-3.fld 
    run_and_verify "Error: time steps must be numeric." 10.5 ../files/small.fld ../../files/results/small-3.fld 
    run_and_verify "Error: Invalid number of time steps." -10 ../files/small.fld ../../files/results/small-3.fld 
    run_and_verify "Error: Cannot open ../files/no_existe.fld for reading" 10 ../files/no_existe.fld ../../files/results/small-3.fld 
    run_and_verify "Error: Cannot open ../../files/results/no_existe.fld for writing" 10 ../files/small.fld ../../files/results/no_existe.fld 
}

run_tests_num_particles() {
    modify_particles ../files/archivos_pruebas/small_malo.fld 0
    run_and_verify "Error: Invalid number of particles: 0" 1 ../files/archivos_pruebas/small_malo.fld ../files/results/small-1.fld 
    modify_particles ../files/archivos_pruebas/small_malo.fld -123
    run_and_verify "Error: Invalid number of particles: " 1 ../files/archivos_pruebas/small_malo.fld ../files/results/small-1.fld 
    modify_particles ../files/archivos_pruebas/pepe.fld
    run_and_verify "Error: Number of particles mismatch." 1 ../files/archivos_pruebas/pepe.fld ../files/results/small-1.fld 
}

run_tests_simulacion() {
    run_and_verify "" 1 ../files/small.fld ../files/results/small-1.fld 
    run_and_verify "" 10 ../files/small.fld ../files/results/small-1.fld 
    run_and_verify "" 1 ../files/small.fld ../files/results/small-1.fld 
    run_and_verify "" 1 ../files/large.fld ../files/results/large-1.fld 
    run_and_verify "" 10 ../files/large.fld ../files/results/large-1.fld 
    run_and_verify "" 1 ../files/large.fld ../files/results/large-1.fld 
}

# Función para ejecutar todas las pruebas y mostrar el resultado final
run_all_tests() {
    run_tests_parameters
    run_tests_num_particles
    run_tests_simulacion
    echo "Total tests: $total_tests"
    echo "Successful tests: $successful_tests"
}

# Ejecutar todas las pruebas al ejecutar el script
run_all_tests
