#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0
time ../build/fluid/fluid 100 ../files/large.fld ../files/results/large-1000.fld