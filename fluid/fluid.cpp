#include "../sim/progargs.hpp"
#include "../sim/soa/grid_soa.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char * argv[]) {
  if (comprobacion_num_param(argc) == -1) { return -1; }  // Comprobacion del numero de parametros

  string num_iter = argv[1];
  string input    = argv[2];
  string output   = argv[3];

  int resultado = proargs_validations(num_iter, input, output);
  if (resultado != 0) { return resultado; }
  string input_file_name  = argv[2];
  string output_file_name = argv[3];

  Grid grid;
  grid.readFile(input_file_name);
  grid.simulation(stoi(argv[1]));
  grid.writeFile(output_file_name);

  // writeFile(output_file_name, ppm, num_particles, particles);

  return 0;
}
