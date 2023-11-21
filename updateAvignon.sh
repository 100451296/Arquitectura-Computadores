#!/bin/sh

# Archivos y carpetas a copiar
archivos_a_copiar="files fluid ftest scripts sim utest .clang-tidy"

# Ruta del destino en el servidor remoto
destino_remoto="avignon:~/proyecto/"

# Copiar archivos y carpetas usando scp
scp -r $archivos_a_copiar $destino_remoto
