// Grid Class

#ifndef GRID
#define GRID

#include "block.hpp"
#include "common.hpp"

#include <iostream>
#include <memory>
#include <vector>

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
  public:
    std::vector<std::shared_ptr<Particle>> particles;     // AoS
    std::vector<std::vector<std::vector<Block>>> blocks;  // Matriz tridimensional de bloques
    float ppm;
    int num_particles;

    void printParticles();

    Grid(std::vector<Particle> & particles, float ppm, int num_particles)
      : ppm(ppm), num_particles(num_particles) {
      for (auto & particle : particles) {
        this->particles.push_back(std::make_shared<Particle>(particle));
      }
      // Resto de la lógica de inicialización de Grid
    }
};

#endif