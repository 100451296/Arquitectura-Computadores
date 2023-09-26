// Block Class

#ifndef BLOCK
#define BLOCK

#include <vector>

#define MULTIPLICADOR_RADIO 1.695
#define DENSIDAD_FLUIDO 103
#define PRESION_RIGIDEZ 3.0
#define COLISIONES_RIGIDEZ 3e4
#define AMORTIGUAMIENTO 128.0
#define VISCOSIDAD 0.4
#define TAMANO_PARTICULA 2e-4
#define PASO_TIEMPO 1e-3

#define ACELERACION_GRAVEDAD_X 0.0
#define ACELERACION_GRAVEDAD_Y 9.8
#define ACELERACION_GRAVEDAD_Z 0.0

#define LIMITE_INFERIOR_RECINTO_X 0.065
#define LIMITE_INFERIOR_RECINTO_Y 0.1
#define LIMITE_INFERIOR_RECINTO_Z 0.065
#define LIMITE_SUPERIOR_RECINTO_X -0.065
#define LIMITE_SUPERIOR_RECINTO_Y -0.08
#define LIMITE_SUPERIOR_RECINTO_Z -0.065

using namespace std;

int block();

struct Particle {
  unsigned int id;
  double posX;
  double posY;
  double posZ;
  double smoothVecX;
  double smoothVecY;
  double smoothVecZ;
  double velX;
  double velY;
  double velZ;
} typedef Particle;

class Block {
  // Representa una clase en la malla. Realizará todos los cálculos entre
  // partículas
 public:
  vector<Particle*> particlesId;
  vector<double> accelerations;
  vector<double> densities;

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