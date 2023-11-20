#ifndef BLOCK
#define BLOCK

#include "common.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

class Block {
    // Representa un bloque en la malla. Realizará todos los cálculos entre
    // partículas

  public:
    static constexpr double EPSILON = 1e-10;

    Block(std::vector<Particle> & particlesRef, std::vector<double> & accelerationXRef,
          std::vector<double> & accelerationYRef, std::vector<double> & accelerationZRef,
          std::vector<double> & densityRef)
      : particles(particlesRef), particlesID(), particlePairs(), data(),
        accelerationX(accelerationXRef), accelerationY(accelerationYRef),
        accelerationZ(accelerationZRef), density(densityRef) { }

    std::vector<Particle> & particles;
    std::vector<int> particlesID;
    std::vector<std::pair<int, int>> particlePairs;
    DataCommon data;
    std::vector<double> & accelerationX;
    std::vector<double> & accelerationY;
    std::vector<double> & accelerationZ;
    std::vector<double> & density;

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
    void calculate_increm_density(std::vector<std::pair<int, int>> & ParejaParticulas);
    void lineal_transformate_density();
    std::vector<double> calculate_increm_aceleration(std::vector<double> position,
                                                     std::vector<double> velocity, double dist,
                                                     std::vector<int> Id);
    double calculate_dist(double posX, double posY, double posZ);
    void accelerationTransferCalculations(std::vector<std::pair<int, int>> & pair_vec);
};

#endif
