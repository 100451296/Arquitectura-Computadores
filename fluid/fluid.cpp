#include "../sim/grid.hpp"
#include "../sim/progargs.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char * argv[]) {
  if (comprobacion_num_param(argc) == -1) { return -1; }  // Comprobacion del numero de parametros

  string const num_iter = argv[1];
  string const input    = argv[2];
  string const output   = argv[3];

  int resultado = proargs_validations(
      input, num_iter, output);  // Se toma este orden para evitar parametros facilmente
                                 // intercambiables (propuesto por clang tidy)
  if (resultado != 0) { return resultado; }

  Grid grid;
  grid.readFile(input);
  grid.simulation(stoi(num_iter));
  grid.writeFile(output);

  return 0;
}
