#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0

# Ejecutar el programa con Valgrind y cachegrind
valgrind --tool=cachegrind --cachegrind-out-file=cachegrind.out ../build/fluid/fluid 10 ../files/large.fld ../files/results/large-1000.fld

# Analizar el resultado de cachegrind para obtener la tasa de fallos de caché
cg_annotate cachegrind.out
