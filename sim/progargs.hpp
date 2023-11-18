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
int leer_archivo_entrada(const std::string& num_iter_file);
int proargs_validations(std::string const & arg1, std::string const & arg2,
                        std::string const & arg3);
int readFile(std::string const & input_file_name, float & ppm, int & num_particles,
             std::vector<std::shared_ptr<Particle>> & refParticles);
bool readHeader(std::ifstream & input_file, float & ppm, int & num_particles);
bool readParticles(std::ifstream & input_file, std::vector<Particle> & particles,
                   int num_particles);
bool readParticle(std::ifstream & input_file, Particle & particle, int index);
void printParameters(int ppm, int num_particles);

int writeFile(std::string const & output_file_name, float ppm, int num_particles,
              std::vector<std::shared_ptr<Particle>> const & particles);
bool writeHeader(std::ofstream & output_file, float ppm, int num_particles);
bool writeParticles(std::ofstream & output_file,
                    std::vector<std::shared_ptr<Particle>> const & particles);
bool writeParticle(std::ofstream & output_file, std::shared_ptr<Particle> const & particle);

#endif
