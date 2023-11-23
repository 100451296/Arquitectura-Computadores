#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0

perf stat -r 5 ../build/fluid/fluid 10 ../files/large.fld ../files/results/large-1000.fld