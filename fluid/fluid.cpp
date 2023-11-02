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
  if (argc != 4) { return 1; }
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

  // Calcula parametros
  double h             = MULTIPLICADOR_RADIO / ppm;
  double particle_mass = DENSIDAD_FLUIDO * std::pow(ppm, -3);
  int nx               = std::floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / h);
  int ny               = std::floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / h);
  int nz               = std::floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / h);
  int num_blocks       = nx * ny * nz;
  double sx            = (LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / nx;
  double sy            = (LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / ny;
  double sz            = (LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / nz;

  // Imprime parametros
  std::cout << "Number of particles: " << particles.size() << std::endl;
  std::cout << "Particles per meter: " << static_cast<int>(ppm) << std::endl;
  std::cout << "Smoothing length: " << h << std::endl;
  std::cout << "Particle mass: " << particle_mass << std::endl;
  std::cout << "Grid size: " << nx << " x " << ny << " x " << nz << std::endl;
  std::cout << "Number of blocks: " << num_blocks << std::endl;
  std::cout << "Block size: " << sx << " x " << sy << " x " << sz << std::endl;

  Grid grid(nx, ny, nz);

  // Valida archivo de salida
  std::ofstream output_file(output_file_name, std::ios::binary);
  if (!output_file.is_open()) {
    std::cerr << "Error: no se pudo abrir el archivo de salida." << std::endl;
    return 1;
  }

  // Escribe la cabecera (ppm y numero de particulas)
  output_file.write(reinterpret_cast<char *>(&ppm), sizeof(float));
  output_file.write(reinterpret_cast<char *>(&num_particles), sizeof(int));

  // Escribe particulas
  for (int i = 0; i < num_particles; ++i) {
    output_file.write(reinterpret_cast<char *>(&particles[i]), sizeof(Particle));
  }

  output_file.close();

  return 0;
}
<<<<<<< HEAD

/*
int main(){
        using namespace std;
        block();
        cout << "eeeee" << endl;
        return 0;
}
*/
=======
>>>>>>> ac2d651 (formateo de codigo)
