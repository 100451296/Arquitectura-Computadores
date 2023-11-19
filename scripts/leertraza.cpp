#include <fstream>
#include <iostream>
#include <vector>

struct Particle {
    int64_t id;
    double posx, posy, posz;
    double hvx, hvy, hvz;
    double velx, vely, velz;
    double density;
    double accx, accy, accz;
};

struct Block {
    int64_t numParticles;
    std::vector<Particle> particles;
};

void printParticle(Particle const & particle) {
  std::cout << "    ID: " << particle.id << ":\n";
  std::cout << "    Posición: (" << particle.posx << ", " << particle.posy << ", " << particle.posz
            << ")\n";
  std::cout << "    Velocidad: (" << particle.velx << ", " << particle.vely << ", " << particle.velz
            << ")\n";
  std::cout << "    Vector Hv: (" << particle.hvx << ", " << particle.hvy << ", " << particle.hvz
            << ")\n";
  std::cout << "    Densidad: " << particle.density << "\n";
  std::cout << "    Aceleración: (" << particle.accx << ", " << particle.accy << ", "
            << particle.accz << ")\n\n";
}

int main(int argc, char * argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <nombre_del_archivo_binario>" << std::endl;
    return 1;
  }

  char const * fileName = argv[1];

  std::ifstream file(fileName, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Error al abrir el archivo: " << fileName << std::endl;
    return 1;
  }

  // Leer la cabecera
  int numBlocks;
  file.read(reinterpret_cast<char *>(&numBlocks), sizeof(int));
  std::cout << "Número total de bloques: " << numBlocks << std::endl;

  // Leer información de bloques
  for (int blockIndex = 0; blockIndex < numBlocks; ++blockIndex) {
    Block block;
    file.read(reinterpret_cast<char *>(&block.numParticles), sizeof(int64_t));
    std::cout << "Bloque " << blockIndex + 1 << ": Número de partículas - " << block.numParticles
              << std::endl;

    // Leer información de partículas
    for (int64_t particleIndex = 0; particleIndex < block.numParticles; ++particleIndex) {
      Particle particle;
      file.read(reinterpret_cast<char *>(&particle.id), sizeof(int64_t));
      file.read(reinterpret_cast<char *>(&particle.posx), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.posy), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.posz), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.hvx), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.hvy), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.hvz), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.velx), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.vely), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.velz), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.density), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.accx), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.accy), sizeof(double));
      file.read(reinterpret_cast<char *>(&particle.accz), sizeof(double));

      block.particles.push_back(particle);
    }

    // Procesar la información del bloque según sea necesario

    // Imprimir partículas del bloque
    std::cout << "Partículas del bloque " << blockIndex + 1 << ":\n";
    for (auto const & particle : block.particles) { printParticle(particle); }
    std::cout << "------------------------\n";
  }

  file.close();

  return 0;
}
