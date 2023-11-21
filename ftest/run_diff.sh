#!/bin/bash

# Verificar si se proporciona un valor para n (iteraciones)
if [ -z "$1" ]; then
    echo "Debe proporcionar un valor para n (iteraciones)."
    exit 1
fi

# Iterar desde 1 hasta 5
for ((i=1; i<=5; i++)); do
    # Asignar valores a las variables
    iterations=$i
    input_file="../files/small.fld"
    output_file="../files/results/small-$iterations.fld"
    diff_file="../files/out/small-$iterations.fld"

    touch $output_file
    # Ejecutar el programa
    ../build/fluid/fluid $iterations $input_file $output_file > /dev/null
    # Verificar si los archivos son iguales usando diff
    if diff -q $diff_file $output_file > /dev/null; then
      echo -e "\033[0;32mTest Passed:\033[0m $iterations $input_file $output_file"
    else
      echo -e "\033[0;31mTest Failed:\033[0m $iterations $input_file $output_file"
    fi
done
