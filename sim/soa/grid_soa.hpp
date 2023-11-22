// Grid Class

#ifndef GRID_SOA
#define GRID_SOA

#include "../common.hpp"
#include "block_soa.hpp"

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <tuple>
#include <vector>

class GridSoA {
  public:
    Particles particles;                                  // SoA
    std::vector<std::vector<std::vector<Block>>> blocks;  // Matriz tridimensional de bloques
    std::vector<std::pair<std::tuple<int, int, int>, std::tuple<int, int, int>>> parejas_unicas;
    std::vector<double> accelerationX;
    std::vector<double> accelerationY;
    std::vector<double> accelerationZ;
    std::vector<double> density;
    static std::vector<std::tuple<int, int, int>> const offsets;
    double ppm           = 0.0;
    int num_particles    = 0;
    double h             = 0.0;
    double particle_mass = 0.0;
    int nx               = 0;
    int ny               = 0;
    int nz               = 0;
    int num_blocks       = 0;
    double sx            = 0.0;
    double sy            = 0.0;
    double sz            = 0.0;
    DataCommon data;

    GridSoA() : data() { }

    void printParticles();
    void printFirst();
    void simulation(int iterations);

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
    static bool readParticle(std::ifstream & input_file, Particles & particles, int index);

    bool writeHeader(std::ofstream & output_file);
    bool writeParticles(std::ofstream & output_file) const;
    static bool writeParticle(std::ofstream & output_file, Particles const & particles, int index);

    void initGrid();
    void initBlocks();

    void populatePairs();
    void printParameters() const;
};

#endif