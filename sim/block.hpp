// Block Class

#ifndef BLOCK
#define BLOCK

#include "common.hpp"

#include <vector>
#include <memory>

using namespace std;

int block();

class Block {
    // Representa un bloque en la malla. Realizará todos los cálculos entre
    // partículas

  public:
    std::vector<std::shared_ptr<Particle>> particles;  
    vector<std::pair< std::shared_ptr<Particle>, std::shared_ptr<Particle>>> particlePairs;
    DataCommon& data;
    vector<double> accelerationX;
    vector<double> accelerationY;
    vector<double> accelerationZ;
    vector<double> density;

  void addParticle(shared_ptr<Particle> particle);
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
  vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>> generarParejasEntreBloques(Block& otherBlock);
  void calculateDataCommon();
  void initDensityAcceleration();
  void calculate_increm_density(vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>> ParejaParticulas);
  void lineal_transformate_density();
  vector<double> calculate_increm_aceleration(vector<double> position, vector <double> velocity, double dist, vector<unsigned int> Id);
  double calculate_dist(double posX, double posY, double posZ);
  double max(double n1, double n2);
  void accelerationTransferCalculations(vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>> pair_vec);
};

#endif