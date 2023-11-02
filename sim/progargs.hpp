#ifndef PROARGS
#define PROARGS

#include "common.hpp"

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int comprobacion_num_param(int argc);
int comprobacion_primer_argumento(string const & arg1);
int comprobacion_pasos_tiempo(string const & arg1);
int comprobacion_archivo_lectura(string const & arg2);
int comprobacion_archivo_escritura(string const & arg3);
int iniciacion_simulacion(int num_particulas);
int comparacion_cantidad_particulas(int num_particles, int particles_contadas);
int leer_archivo_entrada(string input_file);
int proargs_validations(int argc, string const & arg1, string const & arg2, string const & arg3);
int readFile(string const input_file_name, float & ppm, int & num_particles,
             vector<Particle> & particles);
#endif