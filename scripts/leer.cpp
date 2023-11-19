#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#define MULTIPLICADOR_RADIO 1.695
#define DENSIDAD_FLUIDO 10e3
#define PRESION_RIGIDEZ 3.0
#define COLISIONES_RIGIDEZ 3e4
#define AMORTIGUAMIENTO 128.0
#define VISCOSIDAD 0.4
#define TAMANO_PARTICULA 2e-4
#define PASO_TIEMPO 1e-3

#define ACELERACION_GRAVEDAD_X 0.0
#define ACELERACION_GRAVEDAD_Y 9.8
#define ACELERACION_GRAVEDAD_Z 0.0

#define LIMITE_SUPERIOR_RECINTO_X 0.065
#define LIMITE_SUPERIOR_RECINTO_Y 0.1
#define LIMITE_SUPERIOR_RECINTO_Z 0.065
#define LIMITE_INFERIOR_RECINTO_X -0.065
#define LIMITE_INFERIOR_RECINTO_Y -0.08
#define LIMITE_INFERIOR_RECINTO_Z -0.065

struct Particle {
    float px, py, pz;
    float hvx, hvy, hvz;
    float vx, vy, vz;
};

int main(int argc, char * argv[]) {
  // Comprueba parámetros
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_entrada>" << std::endl;
    return 1;
  }

  // Inicializa variables
  std::string input_file_name = argv[1];

  // Valida archivo de entrada
  std::ifstream input_file(input_file_name, std::ios::binary);
  if (!input_file.is_open()) {
    std::cerr << "Error: no se pudo abrir el archivo de entrada. " << input_file_name << std::endl;
    return 1;
  }

  // Lee partículas por metro y número de partículas
  float ppm;
  int num_particles;
  input_file.read(reinterpret_cast<char *>(&ppm), sizeof(float));
  input_file.read(reinterpret_cast<char *>(&num_particles), sizeof(int));

  // Lee partículas
  std::vector<Particle> particles(num_particles);
  for (int i = 0; i < num_particles; ++i) {
    input_file.read(reinterpret_cast<char *>(&particles[i]), sizeof(Particle));
  }

  input_file.close();

  // Calcula parámetros
  double h             = MULTIPLICADOR_RADIO / ppm;
  double particle_mass = DENSIDAD_FLUIDO * std::pow(ppm, -3);
  int nx               = std::floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / h);
  int ny               = std::floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / h);
  int nz               = std::floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / h);
  int num_blocks       = nx * ny * nz;
  double sx            = (LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / nx;
  double sy            = (LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / ny;
  double sz            = (LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / nz;

  for (int i = 0; i < num_particles; ++i) {
    std::cout << "Particle " << i + 1 << " - Position: (" << particles[i].px << ", "
              << particles[i].py << ", " << particles[i].pz << ")";
    std::cout << "              - Half Velocity: (" << particles[i].hvx << ", " << particles[i].hvy
              << ", " << particles[i].hvz << ")";
    std::cout << "              - Velocity: (" << particles[i].vx << ", " << particles[i].vy << ", "
              << particles[i].vz << ")" << std::endl;
  }

  // Imprime la cabecera y todas las partículas
  std::cout << "Particles per meter: " << static_cast<int>(ppm) << std::endl;
  std::cout << "Number of particles: " << particles.size() << std::endl;
  std::cout << "Smoothing length: " << h << std::endl;
  std::cout << "Particle mass: " << particle_mass << std::endl;
  std::cout << "Grid size: " << nx << " x " << ny << " x " << nz << std::endl;
  std::cout << "Number of blocks: " << num_blocks << std::endl;
  std::cout << "Block size: " << sx << " x " << sy << " x " << sz << std::endl;

  return 0;
}
