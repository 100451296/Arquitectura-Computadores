// Block Class

#ifndef BLOCK
#define BLOCK

#include "common.hpp"

#include <vector>

using namespace std;

int block();

class Block {
    // Representa un bloque en la malla. Realizará todos los cálculos entre
    // partículas

  public:
    vector<Particle> particles;
    vector<std::pair<Particle, Particle>> particlePairs;
    DataCommon data;
    vector<double> acelerationX;
    vector<double> acelerationY;
    vector<double> acelerationZ;
    vector<double> density;

    void addParticle(Particle & particle);
    void initVectors();
    void densityIncrease(Block & contiguousBlock);
    void accelerationTransfer(Block & contiguousBlock);
    void collisionsX(unsigned int cx);
    void collisionsY(unsigned int cy);
    void collisionsZ(unsigned int cz);
    void particleMotion();
    void interactionsX(unsigned int cx);
    void interactionsY(unsigned int cy);
    void interactionsZ(unsigned int cz);
    void generarParejasBloque();
    vector<std::pair<Particle, Particle>> generarParejasEntreBloques(Block & otherBlock);
    void calculateDataCommon();
    void initDensityAcceleration();
    void lineal_transformate_density();
    double pow(double base, int exponent);
};

#endif