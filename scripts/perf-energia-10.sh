#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0

# Configura eventos específicos para medir energía y potencia
ENERGY_EVENTS="-e power/energy-cores/,power/energy-gpu/,power/energy-pkg/,power/energy-ram/"

# Utiliza perf stat para medir la energía
perf stat $ENERGY_EVENTS -r 5 ../build/fluid/fluid 10 ../files/large.fld ../files/results/large-1000.fld
