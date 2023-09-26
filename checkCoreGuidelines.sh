#!/bin/bash

# Obtener el ancho de la terminal
width=$(tput cols)

# Calcular la mitad del ancho de la terminal
half_width=$((width / 2))

# Función para mostrar el mensaje de ayuda
show_help() {
  echo -e "Uso: $0 directorio"
  echo "Este script verifica que los archivos .cpp en un directorio sigan las C++ Core Guidelines."
  echo
  echo "Argumentos:"
  echo "  directorio  El directorio que contiene los archivos .cpp que se verificarán."
  echo
  echo "Ejemplo de uso:"
  echo "  $0 mi_directorio"
  exit 1
}

# Verificar que se haya proporcionado un directorio como parámetro
if [ $# -eq 0 ]; then
  show_help
fi

# Verificar que el directorio exista
if [ ! -d "$1" ]; then
  echo -e "\e[31mError: el directorio $1 no existe\e[0m"
  show_help
fi

# Añadir saltos de línea antes de iniciar el for
echo -e "\n"

# Ejecutar cppcheck para cada archivo *.cpp en el directorio
for file in $1/*.cpp; do
  echo -e "\e[32m$(printf '%*s' $half_width | tr ' ' '#')"
  echo "Archivo: $file"
  echo -e "$(printf '%*s' $half_width | tr ' ' '#')\e[0m"
  cppcheck --rule=core_guidelines --enable=all "$file"
  echo -e "\n\e[35m$(printf '%*s' $width | tr ' ' '*')\n\e[0m"
done
