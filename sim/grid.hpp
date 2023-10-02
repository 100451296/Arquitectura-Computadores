// Grid Class

#ifndef GRID
#define GRID

#include <vector>

#include "common.hpp"

using namespace std;

int grid();

// Definición de la estructura SoA
struct Particles {
  std::vector<unsigned int> id;
  std::vector<double> posX;
  std::vector<double> posY;
  std::vector<double> posZ;
  std::vector<double> smoothVecX;
  std::vector<double> smoothVecY;
  std::vector<double> smoothVecZ;
  std::vector<double> velX;
  std::vector<double> velY;
  std::vector<double> velZ;

  // Constructor para reservar espacio para un número dado de partículas
  Particles(int numParticles = 0) {
    id.resize(numParticles);
    posX.resize(numParticles);
    posY.resize(numParticles);
    posZ.resize(numParticles);
    smoothVecX.resize(numParticles);
    smoothVecY.resize(numParticles);
    smoothVecZ.resize(numParticles);
    velX.resize(numParticles);
    velY.resize(numParticles);
    velZ.resize(numParticles);
  }
};

class Grid {
  // Representa la malla. Contendrá todas las partículas y la matriz de bloques
 public:
  Particles particles;
};

#endif