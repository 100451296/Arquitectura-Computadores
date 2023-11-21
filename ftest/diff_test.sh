#!/bin/sh

iterations = $1
input_file = $2
output_file = $3
diff_file = $4

# Ejecutar el programa
../build/fluid/fluid $iterations $input_file $output_file

# Verificar si los archivos son iguales usando diff
if diff -q $diff_file $output_file > /dev/null; then
    echo -e "\033[0;32mTest Passed:\033[0m $iterations $input_file $output_file"
else
    echo -e "\033[0;31mTest Failed:\033[0m $iterations $input_file $output_file"
fi