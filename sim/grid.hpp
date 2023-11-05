// Grid Class

#ifndef GRID
#define GRID

#include "block.hpp"
#include "common.hpp"

#include <cmath>
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
    double h;
    double particle_mass;
    int nx;
    int ny;
    int nz;
    int num_blocks;
    double sx;
    double sy;
    double sz;

    void printParticles();
    void positionateParticle();

    Grid(std::vector<Particle> & particles, float ppm, int num_particles)
      : particles(), ppm(ppm), num_particles(num_particles), h(MULTIPLICADOR_RADIO / ppm),
        particle_mass(DENSIDAD_FLUIDO * std::pow(ppm, -3)),
        nx(std::floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / h)),
        ny(std::floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / h)),
        nz(std::floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / h)),
        num_blocks(nx * ny * nz), sx((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / nx),
        sy((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / ny),
        sz((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / nz) {
      for (auto & particle : particles) {
        this->particles.push_back(std::make_shared<Particle>(particle));
      }
      blocks.resize(nx, std::vector<std::vector<Block>>(ny, std::vector<Block>(nz)));
    }
};

#endif