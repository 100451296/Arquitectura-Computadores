// Grid Class

#ifndef GRID
#define GRID

#include "../common.hpp"
#include "block_soa.hpp"

#include <cmath>
#include <fstream>
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
    std::vector<Particle> particles;                      // AoS
    std::vector<std::vector<std::vector<Block>>> blocks;  // Matriz tridimensional de bloques
    std::vector<std::pair<std::tuple<int, int, int>, std::tuple<int, int, int>>> parejas_unicas;
    std::vector<double> accelerationX;
    std::vector<double> accelerationY;
    std::vector<double> accelerationZ;
    std::vector<double> density;
    double ppm;
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

    DataCommon data;

    void printParticles();
    void printFirst();
    void printPairs();
    void simulation(int interations);
    void prueba();

    int readFile(std::string const & input_file_name);
    int writeFile(std::string const & output_file_name);

    void generateParticlePairs();
    void positionateParticle();
    void resetBlocks();
    void densityIncreaseGrid();
    void aceletarionTransferGrid();
    void linealDensityTransform();
    void collisionsXGrid();
    void collisionsYGrid();
    void collisionsZGrid();
    void particleMotionGrid();
    void interactionsXGrid();
    void interactionsYGrid();
    void interactionsZGrid();
    void initializeBlockVectors();
    void initDensityAcceleration();

  private:
    bool readHeader(std::ifstream & input_file);
    bool readParticles(std::ifstream & input_file);
    bool readParticle(std::ifstream & input_file, Particle & particle, int index);

    bool writeHeader(std::ofstream & output_file);
    bool writeParticles(std::ofstream & output_file);
    bool writeParticle(std::ofstream & output_file, Particle const & particle);

    void initGrid();
    void initBlocks();

    void populatePairs();
    void printParameters();
};

#endif