#include "../sim/block.hpp"
#include "../sim/grid.hpp"
#include "../sim/progargs.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char * argv[]) {
  // Comprueba parametros
  if (argc != 4) {
    cout << "Error: Invalid number of arguments:" << argc << endl;
    return -1;
  }
  if (int resultado = proargs_validations(argc, argv[1], argv[2], argv[3]) != 0) {
    return resultado;
  }

  // Inicializa variables
  // int num_iterations = std::stoi(argv[1]);
  std::string input_file_name  = argv[2];
  std::string output_file_name = argv[3];
  float ppm;
  int num_particles;
  vector<Particle> particles;

  readFile(input_file_name, ppm, num_particles, particles);
  printParameters(ppm, num_particles);

  return 0;
}
