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
    vector<Particle&> particles;
    vector<std::pair< const Particle&, const Particle&>> particlePairs;
    DataCommon data;
    vector<double> accelerationX;
    vector<double> accelerationY;
    vector<double> accelerationZ;
    vector<double> density;

  void addParticle(Particle& particle);
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
  vector<std::pair<const Particle&, const Particle&>> generarParejasEntreBloques(Block& otherBlock);
  void calculateDataCommon();
  void initDensityAcceleration();
  void lineal_transformate_density();
  vector<double> Block::calculate_increm_aceleration(vector<double> position, vector <double> velocity, double dist, vector<unsigned int> Id);
  double calculate_dist(double posX, double posY, double posZ);
  double max(double n1, double n2);
};

#endif