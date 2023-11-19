# Lista de archivos fuente de soa_library
set(SOA_SOURCES
  block_soa.cpp
  block_soa.hpp
  grid_soa.cpp
  grid_soa.hpp
)

# Construir la biblioteca soa_library
add_library(soa ${SOA_SOURCES})