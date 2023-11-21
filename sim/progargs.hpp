#ifndef PROARGS
#define PROARGS

#include "common.hpp"

#include <filesystem>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

int comprobacion_num_param(int argc);
int comprobacion_primer_argumento(std::string const & arg1);
int comprobacion_pasos_tiempo(std::string const & arg1);
int comprobacion_archivo_lectura(std::string const & arg2);
int comprobacion_archivo_escritura(std::string const & arg3);
int iniciacion_simulacion(int num_particulas);
int comparacion_cantidad_particulas(int num_particles, int particles_contadas);
int leer_cabecera_archivo(std::ifstream& fichero, float& ppm, int& num_particles);
int leer_particulas_archivo(std::ifstream& fichero, int num_particles);
int leer_archivo_entrada(const std::string& num_iter_file);
int proargs_validations(std::string const & arg1, std::string const & arg2,
                        std::string const & arg3);
void printParameters(int ppm, int num_particles);

#endif
