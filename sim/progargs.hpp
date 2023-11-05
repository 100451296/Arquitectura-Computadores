#ifndef PROARGS
#define PROARGS

#include "common.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

int comprobacion_num_param(int argc);
int comprobacion_primer_argumento(std::string const & arg1);
int comprobacion_pasos_tiempo(std::string const & arg1);
int comprobacion_archivo_lectura(std::string const & arg2);
int comprobacion_archivo_escritura(std::string const & arg3);
int iniciacion_simulacion(int num_particulas);
int comparacion_cantidad_particulas(int num_particles, int particles_contadas);
int leer_archivo_entrada(std::string input_file);
int proargs_validations(int argc, std::string const & arg1, std::string const & arg2,
                        std::string const & arg3);
int readFile(std::string const & input_file_name, float & ppm, int & num_particles,
             std::vector<Particle> & particles);
bool readHeader(std::ifstream & input_file, float & ppm, int & num_particles);
bool readParticles(std::ifstream & input_file, std::vector<Particle> & particles,
                   int num_particles);
bool readParticle(std::ifstream & input_file, Particle & particle, int index);
void printParameters(int ppm, int num_particles);

#endif