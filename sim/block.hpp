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
    vector<ParticleRef> particles;
    vector<std::pair< const ParticleRef, const ParticleRef>> particlePairs;
    DataCommon data;
    vector<double> accelerationX;
    vector<double> accelerationY;
    vector<double> accelerationZ;
    vector<double> density;

  void addParticle(ParticleRef particle);
  void initVectors();
  void densityIncrease(Block& contiguousBlock);
  void accelerationTransfer(Block& contiguousBlock);
  void collisionsX(unsigned int cx);
  void collisionsY(unsigned int cy);
  void collisionsZ(unsigned int cz);
  void particleMotion();
  void interactionsX(unsigned int cx);
  void interactionsY(unsigned int cy);
  void interactionsZ(unsigned int cz);
  void generarParejasBloque();
  vector<std::pair<const ParticleRef, const ParticleRef>> generarParejasEntreBloques(Block& otherBlock);
  void calculateDataCommon();
  void initDensityAcceleration();
  void calculate_increm_density(vector<std::pair<ParticleRef const, ParticleRef const>> ParejaParticulas);
  void lineal_transformate_density();
  vector<double> calculate_increm_aceleration(vector<double> position, vector <double> velocity, double dist, vector<unsigned int> Id);
  double calculate_dist(double posX, double posY, double posZ);
  double max(double n1, double n2);
  void accelerationTransferCalculations(vector<std::pair<const ParticleRef, const ParticleRef>> pair_vec);
};

#endif