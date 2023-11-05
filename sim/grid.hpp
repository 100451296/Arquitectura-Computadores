// Grid Class

#ifndef GRID
#define GRID

#include "block.hpp"
#include "common.hpp"

#include <cmath>
#include <iostream>
#include <memory>
#include <set>
#include <tuple>
#include <vector>

// Offsets para buscar bloques contiguos
std::vector<std::tuple<int, int, int>> const offsets = {
  {1,  1,  1},
  {1,  1,  0},
  {1,  1, -1},
  {1,  0,  1},
  {1,  0,  0},
  {1,  0, -1},
  {1, -1,  1},
  {1, -1,  0},
  {1, -1, -1},
  {0,  1,  1},
  {0,  1,  0},
  {0,  1, -1},
  {0,  0,  1}
};

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
    std::vector<std::pair<Block, Block>> parejas_unicas;  // Bloques contiguos
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

    DataCommon data{static_cast<double>(ppm),
                    static_cast<double>(particle_mass),
                    h,
                    static_cast<unsigned int>(nx),
                    static_cast<unsigned int>(ny),
                    static_cast<unsigned int>(nz)};

    void printParticles();
    void printPairs();
    void simulation(int interations);

    void positionateParticle();
    void densityIncreaseGrid();
    void aceletarionTransferGrid();
    void densityTransform();
    void collisionsXGrid();
    void collisionsYGrid();
    void collisionsZGrid();

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
      for (int x = 0; x < nx; ++x) {
        for (int y = 0; y < ny; ++y) {
          for (int z = 0; z < nz; ++z) {
            blocks[x][y][z].data = std::make_shared<DataCommon>(data);
            for (auto const & offset : offsets) {
              int x_offset, y_offset, z_offset;
              std::tie(x_offset, y_offset, z_offset) = offset;

              int x_contiguo = x + x_offset;
              int y_contiguo = y + y_offset;
              int z_contiguo = z + z_offset;

              if (x_contiguo >= 0 && x_contiguo < nx && y_contiguo >= 0 && y_contiguo < ny &&
                  z_contiguo >= 0 && z_contiguo < nz) {
                parejas_unicas.push_back(
                    std::make_pair(blocks[x][y][z], blocks[x_contiguo][y_contiguo][z_contiguo]));
              }
            }
          }
        }
      }
    }
};

#endif