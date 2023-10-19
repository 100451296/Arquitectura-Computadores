#ifndef PROARGS
#define PROARGS

#include <iostream>
#include <fstream>
#include <utility>

using namespace std;

int comprobacion_num_param(int argc);
int comprobacion_primer_argumento(const string & arg1);
int comprobacion_pasos_tiempo(const string & arg1);
int comprobacion_archivo_lectura(const string & arg2);
int comprobacion_archivo_escritura(const string & arg3);
int iniciacion_simulacion(int num_particulas);
int comparacion_cantidad_particulas(int num_particles, int particles_contadas);
int leer_archivo_entrada(string input_file);
int progargs(int argc, const string & arg1, const string & arg2, const string & arg3);

#endif 
