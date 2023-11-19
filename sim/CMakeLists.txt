# Add to this list all files related to sim library
add_library(sim
progargs.hpp
progargs.cpp
grid.cpp
grid.hpp
block.cpp
block.hpp
)

# Asegúrate de que soa_library esté construida antes de agregarla a sim
add_dependencies(sim soa)
