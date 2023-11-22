#ifndef PROARGS
#define PROARGS

#include "common.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

int comprobacion_num_param(int num_params);
int comprobacion_primer_argumento(std::string const & num_iter);
int comprobacion_pasos_tiempo(std::string const & num_iter);
int comprobacion_archivo_lectura(std::string const & input);
int comprobacion_archivo_escritura(std::string const & output);
int iniciacion_simulacion(int num_particulas);
int comparacion_cantidad_particulas(int num_particles, int particles_contadas);
int leer_cabecera_archivo(std::ifstream & fichero, float & ppm, int & num_particles);
int leer_particulas_archivo(std::ifstream & fichero, int num_particles);
int leer_archivo_entrada(std::string const & num_iter_file);
int proargs_validations(std::string const & input, std::string const & num_iter,
                        std::string const & output);

#endif
