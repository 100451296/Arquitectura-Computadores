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
  string input_file_name  = argv[2];
  string output_file_name = argv[3];
  /*
  float ppm;
  int num_particles;
  vector<shared_ptr<Particle>> particles;

  readFile(input_file_name, ppm, num_particles, particles);
  printParameters(ppm, num_particles);

  Grid grid(particles, ppm, num_particles);
  grid.simulation(stoi(argv[1]));
  cout << "Main " << particles[0]->posX << endl;
  */
  Grid grid;
  grid.readFile(input_file_name);
  grid.simulation(stoi(argv[1]));
  grid.writeFile(output_file_name);

  // writeFile(output_file_name, ppm, num_particles, particles);

  return 0;
}
