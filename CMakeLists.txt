cmake_minimum_required(VERSION 3.22)
project(fluid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuraciones adicionales
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)
set(CMAKE_CXX_FLAGS_RELEASE "-march=native")

# Subdirectorios
add_subdirectory(fluid)
add_subdirectory(sim)
add_subdirectory(utest)
add_subdirectory(ftest)
