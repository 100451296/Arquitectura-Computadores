// Grid Class

#ifndef GRID
#define GRID

#include <vector>

#include "common.hpp"
#include "block.hpp"

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
  Particle particles; // AoS

  vector<vector<vector<Block>>> blocks; // Matriz tridimensional de bloques

  // Constructor para inicializar la matriz de bloques
  Grid(int x, int y, int z) : blocks(x, vector<vector<Block>>(y, vector<Block>(z))) {
    // Código para inicializar la matriz de bloques
    for (int i = 0; i < x; i++) {
      for (int j = 0; j < y; j++) {
        for (int k = 0; k < z; k++) {
          // Código para inicializar cada bloque
          blocks[i][j][k] = Block();
        }
      }
    }
  }
};

#endif