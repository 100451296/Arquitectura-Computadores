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
    std::vector<std::shared_ptr<Particle>> particles;
    std::vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>> particlePairs;
    std::shared_ptr<DataCommon> data;
    std::vector<double> accelerationX;
    std::vector<double> accelerationY;
    std::vector<double> accelerationZ;
    std::vector<double> density;

    void addParticle(std::shared_ptr<Particle> particle);
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
    std::vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>>
        generarParejasEntreBloques(Block & otherBlock);
    void calculateDataCommon();
    void initDensityAcceleration();
    void calculate_increm_density(
        std::vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>>
            ParejaParticulas);
    void lineal_transformate_density();
    void lineal_transformate_density(Block & contiguousBlock);
    std::vector<double> calculate_increm_aceleration(std::vector<double> position,
                                                     std::vector<double> velocity, double dist,
                                                     std::vector<unsigned int> Id);
    void increm_aceleration_sum(std::vector<std::shared_ptr<double>> Acceleration,
                                std::vector<double> increm_aceleration);
    double calculate_dist(double posX, double posY, double posZ);
    
    void accelerationTransferCalculations(
        std::vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>> pair_vec);
};

#endif
