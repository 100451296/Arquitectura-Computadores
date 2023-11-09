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
    std::vector<double> & accelerationX;
    std::vector<double> & accelerationY;
    std::vector<double> & accelerationZ;
    std::vector<double> & density;

    Block(std::vector<Particle> & particlesRef, std::vector<double> & accelerationXRef,
          std::vector<double> & accelerationYRef, std::vector<double> & accelerationZRef,
          std::vector<double> & densityRef)
      : particles(particlesRef), accelerationX(accelerationXRef), accelerationY(accelerationYRef),
        accelerationZ(accelerationZRef), density(densityRef) {
      generarParejasBloque();
    }

    Block & operator=(Block const & other) {
      if (this != &other) {
        // Copiar los miembros necesarios
        particles.clear();
        for (auto const & particle : other.particles) { particles.push_back(particle); }
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
    void generarParejasEntreBloques(Block & otherBlock, std::vector<std::pair<int, int>> & aux);
    void calculateDataCommon();
    void initDensityAcceleration();
    void calculate_increm_density(std::vector<std::pair<int, int>> ParejaParticulas);
    void calculate_increm_density();
    void lineal_transformate_density();
    void lineal_transformate_density(Block & contiguousBlock);
    std::vector<double> calculate_increm_aceleration(std::vector<double> position,
                                                     std::vector<double> velocity, double dist,
                                                     std::vector<unsigned int> Id);
    double calculate_dist(double posX, double posY, double posZ);

    void accelerationTransferCalculations(std::vector<std::pair<int, int>> pair_vec);
};

#endif
