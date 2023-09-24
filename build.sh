#!/bin/bash

# Crear carpeta build si no existe
if [ ! -d "build" ]; then
  mkdir build
fi

# Acceder a la carpeta build
cd build

# Configurar el proyecto con CMake
cmake ..

# Compilar el proyecto
make

# Volver a la carpeta raíz del proyecto
cd ..
