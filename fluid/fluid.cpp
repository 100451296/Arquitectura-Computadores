#include "../sim/grid.hpp"
#include "../sim/progargs.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <span>
#include <vector>

using namespace std;

int main(int argc, char * argv[]) {
  if (comprobacion_num_param(argc) == -1) { return -1; }

  std::vector<char const *> args_view(argv + 1, argv + argc);

  string const num_iter = args_view[0];
  string const input    = args_view[1];
  string const output   = args_view[2];
  int const resultado   = proargs_validations(input, num_iter, output);

  if (resultado != 0) { return resultado; }

  Grid grid;
  grid.readFile(input);
  grid.simulation(stoi(num_iter));
  grid.writeFile(output);

  return 0;
}
