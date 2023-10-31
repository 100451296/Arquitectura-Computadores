// Block Class

#ifndef BLOCK
#define BLOCK

#include <vector>

#include "common.hpp"

using namespace std;

int block();

class Block {
  // Representa un bloque en la malla. Realizará todos los cálculos entre
  // partículas
 public:
  vector<Particle*> particlesId;
  vector<double> accelerations;
  vector<double> densities;

  void addParticle(Particle* particle);
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
};

#endif