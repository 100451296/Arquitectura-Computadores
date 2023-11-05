#include <grid.hpp>

int grid() {
  // Código de las pruebas unitarias
  return 0;
}

void Grid::printParticles() {
  std::cout << "Printing all particles:" << std::endl;
  for (auto const & particle : particles) {
    std::cout << "Particle ID: " << particle->id << std::endl;
    std::cout << "Position: (" << particle->posX << ", " << particle->posY << ", " << particle->posZ
              << ")" << std::endl;
    std::cout << "Smooth vector: (" << particle->smoothVecX << ", " << particle->smoothVecY << ", "
              << particle->smoothVecZ << ")" << std::endl;
    std::cout << "Velocity: (" << particle->velX << ", " << particle->velY << ", " << particle->velZ
              << ")" << std::endl;
  }
}
