#add_library(sim block.cpp block.hpp grid.cpp grid.hpp progargs.cpp progargs.hpp)

# Crear biblioteca dinámica
add_library(sim SHARED block.cpp grid.cpp progargs.cpp)
target_include_directories(sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

