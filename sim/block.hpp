#ifndef BLOCK
#define BLOCK

#include "common.hpp"

#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

class Block {
    // Representa un bloque en la malla. Realizará todos los cálculos entre
    // partículas

  public:
    std::vector<Particle> & particles;
    std::vector<int> particlesID;
    std::vector<std::pair<int, int>> particlePairs;
    DataCommon data;
    std::vector<double> accelerationX;
    std::vector<double> accelerationY;
    std::vector<double> accelerationZ;
    std::vector<double> density;

    Block(std::vector<Particle> & particlesRef) : particles(particlesRef) { }

    // Definir un operador de asignación personalizado
    Block & operator=(Block const & other) {
      if (this != &other) {
        // Copiar los miembros necesarios
        particles = other.particles;
        // Copiar otros miembros si es necesario
      }
      return *this;
    }

    void addParticle(int id);
    void resetBlock();
    void densityIncreaseSingle();
    void densityIncrease(Block & contiguousBlock);
    void accelerationTransferSingle();
    void accelerationTransfer(Block & contiguousBlock);
    void collisionsX(unsigned int cx);
    void collisionsY(unsigned int cy);
    void collisionsZ(unsigned int cz);
    void particleMotion();
    void interactionsX(unsigned int cx);
    void interactionsY(unsigned int cy);
    void interactionsZ(unsigned int cz);
    void generarParejasBloque();
    std::vector<std::pair<int, int>> generarParejasEntreBloques(Block & otherBlock);
    void calculateDataCommon();
    void initDensityAcceleration();
    void calculate_increm_density(std::vector<std::pair<int, int>> ParejaParticulas,
                                  Block & contiguousBlock);
    void lineal_transformate_density();
    void lineal_transformate_density(Block & contiguousBlock);
    std::vector<double> calculate_increm_aceleration(std::vector<double> position,
                                                     std::vector<double> velocity, double dist,
                                                     std::vector<unsigned int> Id);
    double calculate_dist(double posX, double posY, double posZ);

    void accelerationTransferCalculations(std::vector<std::pair<int, int>> pair_vec,
                                          Block & contiguousBlock);
};

#endif
