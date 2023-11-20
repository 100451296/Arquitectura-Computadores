#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0
time ../build/fluid/fluid 5 ../files/large.fld ../files/results/large-5.fld