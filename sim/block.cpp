#include "block.hpp"

using namespace std;

// METODOS de BLOCK

// Metodo encargado de crear las parejas de particulas de un mismo bloque
void Block::generarParejasBloque() {
  for (size_t i = 0; i < particlesID.size(); i++) {
    for (size_t j = i + 1; j < particlesID.size(); j++) {
      particlePairs.emplace_back(make_pair(particlesID[i], particlesID[j]));
    }
  }
}

// Metodo encargado de crear las parejas de particulas entre bloques contiguos
void Block::generarParejasEntreBloques(Block & otherBlock, vector<std::pair<int, int>> & aux) {
  for (auto & Bid : particlesID) {
    for (auto & Cid : otherBlock.particlesID) { aux.emplace_back(make_pair(Bid, Cid)); }
  }
}

// Función para añadir una partícula al vector
void Block::addParticle(int Bid) {
  particlesID.emplace_back(Bid);
}

// Funcion para resetear el bloque
void Block::resetBlock() {
  particlesID.clear();
  particlePairs.clear();
}

// Funcion inicializar la densidad y la aceleracion para cada particula
void Block::initDensityAcceleration() {
  for (auto & id : particlesID) {
    if (id >= 0 && static_cast<size_t>(id) < (*particles).size()) {
      (*density)[id]       = 0;
      (*accelerationX)[id] = ACELERACION_GRAVEDAD_X;
      (*accelerationY)[id] = ACELERACION_GRAVEDAD_Y;
      (*accelerationZ)[id] = ACELERACION_GRAVEDAD_Z;
    } else {
      // Manejar el caso en el que el índice está fuera de rango
      std::cerr << "Error: Índice de partícula fuera de rango." << std::endl;
      // Puedes lanzar una excepción, mostrar un mensaje de error, o tomar otra acción adecuada.
    }
  }
}

// Funcion encargada de modificar el vector de densidades del propio bloque
void Block::densityIncreaseSingle() {
  for (auto const & pair : particlePairs) {
    Particle const & particle1 = (*particles)[pair.first];
    Particle const & particle2 = (*particles)[pair.second];

    double dxB = particle1.posX - particle2.posX;
    double dyB = particle1.posY - particle2.posY;
    double dzB = particle1.posZ - particle2.posZ;

    double distanceSquared = dxB * dxB + dyB * dyB + dzB * dzB;

    if (distanceSquared < data.h_square) {
      double hSquareMinusDistSquared = data.h_square - distanceSquared;
      double incremDensityPair =
          hSquareMinusDistSquared * hSquareMinusDistSquared * hSquareMinusDistSquared;

      (*density)[pair.first]  += incremDensityPair;
      (*density)[pair.second] += incremDensityPair;
    }
  }
}

// Funcion encargada de modificar el vector de densidades del propio bloque con su contiguo
void Block::densityIncrease(Block & contiguousBlock) {
  // Parte del bloque contiguo
  vector<std::pair<int, int>> aux;
  generarParejasEntreBloques(contiguousBlock, aux);
  calculate_increm_density(aux);
}

// Metodo auxiliar que realiza los diferentes calculos para el incremento de densidad
void Block::calculate_increm_density(std::vector<std::pair<int, int>> & ParejaParticulas) const {
  for (auto const & pair : ParejaParticulas) {
    Particle const & particle1 = (*particles)[pair.first];
    Particle const & particle2 = (*particles)[pair.second];

    double dxB = particle1.posX - particle2.posX;
    double dyB = particle1.posY - particle2.posY;
    double dzB = particle1.posZ - particle2.posZ;

    double distanceSquared = dxB * dxB + dyB * dyB + dzB * dzB;

    if (distanceSquared < data.h_square) {
      double hSquareMinusDistSquared = data.h_square - distanceSquared;
      double incremDensityPair =
          hSquareMinusDistSquared * hSquareMinusDistSquared * hSquareMinusDistSquared;

      (*density)[pair.first]  += incremDensityPair;
      (*density)[pair.second] += incremDensityPair;
    }
  }
}

// Metodo auxiliar que realiza los diferentes calculos para la transformacion lineal de la
// densidad en un mismo bloque
void Block::lineal_transformate_density() {
  for (auto & Bid : particlesID) {
    double coeficiente = ThreeHundFifteen / (SixtyFour * M_PI * data.h_pow9) * data.mass;

    (*density)[Bid] = ((*density)[Bid] + data.h_pow6) * coeficiente;
  }
}

// Metodo auxiliar que realiza los diferentes calculos del incremento de la aceleracion
vector<double> Block::calculate_increm_aceleration(vector<double> position, vector<double> velocity,
                                                   double dist, vector<int> Bid) {
  double const pi_x = position[0];
  double const pi_y = position[1];
  double const pi_z = position[2];

  double const vi_x = velocity[0];
  double const vi_y = velocity[1];
  double const vi_z = velocity[2];

  double const rho_i = (*density)[Bid[0]];
  double const rho_j = (*density)[Bid[1]];

  double const term1 = Fifteen / (M_PI * pow(data.long_suavizado, Six));
  double const term2 = Three * data.mass * PRESION_RIGIDEZ / Two;
  double const term3 = pow(data.long_suavizado - dist, Two) / dist;
  double const term4 = rho_i + rho_j - Two * DENSIDAD_FLUIDO;
  double const term5 = FortyFive / (M_PI * pow(data.long_suavizado, Six)) * VISCOSIDAD * data.mass;
  double const term6 = (*density)[Bid[0]] * (*density)[Bid[1]];

  vector<double> increment_acceleration = {
    // Posicion X
    (pi_x * term1 * term2 * term3 * term4 + vi_x * term5) / term6,
    // Posicion Y
    (pi_y * term1 * term2 * term3 * term4 + vi_y * term5) / term6,
    // Posicion Z
    (pi_z * term1 * term2 * term3 * term4 + vi_z * term5) / term6};
  return increment_acceleration;
}

// Metodo auxiliar que realiza los diferentes calculos para la distancia
double Block::calculate_dist(double posX, double posY, double posZ) {
  double dist = sqrt(max(pow(posX, Two) + pow(posY, Two) + pow(posZ, Two), TenLessTwuelve));
  return dist;
}

// Funcion que se encarga de actualizar el vector de aceleraciones y el incremento en un mismo
// bloque
void Block::accelerationTransferSingle() {
  accelerationTransferCalculations(particlePairs);
}

// Funcion que se encarga de actualizar el vector de aceleraciones y el incremento respecto a un
// bloque contiguo
void Block::accelerationTransfer(Block & contiguousBlock) {
  vector<std::pair<int, int>> aux;
  generarParejasEntreBloques(contiguousBlock, aux);
  accelerationTransferCalculations(aux);
}

// Método auxiliar que realiza los diferentes cálculos para saber si dos partículas están lo
// suficientemente cerca
bool Block::distClose(Particle p1, Particle p2) {
  if (pow((p1.posX - p2.posX), 2) + pow((p1.posY - p2.posY), 2) + pow((p1.posZ - p2.posZ), 2) <
      pow(data.long_suavizado, 2)) {
    return true;
  }
  return false;
}

// Metodo auxiliar que realiza los diferentes calculos para modificar la aceleracion
void Block::updateAcceleration(int p1, int p2, vector<double> & increm_aceleration) {
  (*accelerationX)[p1]  = (*accelerationX)[p1] + increm_aceleration[0];
  (*accelerationY)[p1]  = (*accelerationY)[p1] + increm_aceleration[1];
  (*accelerationZ)[p1]  = (*accelerationZ)[p1] + increm_aceleration[2];
  (*accelerationX)[p2] = (*accelerationX)[p2] - increm_aceleration[0];
  (*accelerationY)[p2] = (*accelerationY)[p2] - increm_aceleration[1];
  (*accelerationZ)[p2] = (*accelerationZ)[p2] - increm_aceleration[2];
}

// Metodo auxiliar que realiza los diferentes calculos para la aceleracion
void Block::accelerationTransferCalculations(vector<std::pair<int, int>> & pair_vec) {
  for (auto const & pair : pair_vec) {
    // Si las dos particulas estan cerca
    if (distClose((*particles)[pair.first], (*particles)[pair.second])) {
      // Calculo de distancia
      double dist = calculate_dist((*particles)[pair.first].posX - (*particles)[pair.second].posX,
                                   (*particles)[pair.first].posY - (*particles)[pair.second].posY,
                                   (*particles)[pair.first].posZ - (*particles)[pair.second].posZ);
      // Calculo vector posicion
      vector<double> position = {(*particles)[pair.first].posX - (*particles)[pair.second].posX,
                                 (*particles)[pair.first].posY - (*particles)[pair.second].posY,
                                 (*particles)[pair.first].posZ - (*particles)[pair.second].posZ};
      // Calculo vector velocidad
      vector<double> velocity = {(*particles)[pair.second].velX - (*particles)[pair.first].velX,
                                 (*particles)[pair.second].velY - (*particles)[pair.first].velY,
                                 (*particles)[pair.second].velZ - (*particles)[pair.first].velZ};

      vector<int> Bid = {pair.first, pair.second};

      vector<double> increm_aceleration =
          calculate_increm_aceleration(position, velocity, dist, Bid);

      updateAcceleration(pair.first, pair.second, increm_aceleration);
    }
  }
}

// Debe actualizar la componente x del vector de aceleración para cada particula
void Block::collisionsX(unsigned int cx) {
  double increm_x = 0;
  for (auto & Bid : particlesID) {
    double cord_x = (*particles)[Bid].posX + (*particles)[Bid].smoothVecX * PASO_TIEMPO;
    if (cx == 0) {
      increm_x = TAMANO_PARTICULA - (cord_x - LIMITE_INFERIOR_RECINTO_X);
    } else if (cx == data.nx - 1) {
      increm_x = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_X - cord_x);
    }
    if (increm_x > Epsilon) {
      if (cx == 0) {
        (*accelerationX)[Bid] +=
            COLISIONES_RIGIDEZ * increm_x - AMORTIGUAMIENTO * (*particles)[Bid].velX;
      } else if (cx == data.nx - 1) {
        (*accelerationX)[Bid] -=
            COLISIONES_RIGIDEZ * increm_x + AMORTIGUAMIENTO * (*particles)[Bid].velX;
      }
    }
  }
}

// Debe actualizar la componente y del vector de aceleración para cada particula
void Block::collisionsY(unsigned int cy) {
  double increm_y = 0;
  for (auto & Bid : particlesID) {
    double cord_y = (*particles)[Bid].posY + (*particles)[Bid].smoothVecY * PASO_TIEMPO;
    if (cy == 0) {
      increm_y = TAMANO_PARTICULA - (cord_y - LIMITE_INFERIOR_RECINTO_Y);
    } else if (cy == data.ny - 1) {
      increm_y = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Y - cord_y);
    }
    if (increm_y > Epsilon) {
      if (cy == 0) {
        (*accelerationY)[Bid] +=
            COLISIONES_RIGIDEZ * increm_y - AMORTIGUAMIENTO * (*particles)[Bid].velY;
      } else if (cy == data.ny - 1) {
        (*accelerationY)[Bid] -=
            COLISIONES_RIGIDEZ * increm_y + AMORTIGUAMIENTO * (*particles)[Bid].velY;
      }
    }
  }
}

// Debe actualizar la componente z del vector de aceleración para cada particula
void Block::collisionsZ(unsigned int cz) {
  double increm_z = 0;
  for (auto & Bid : particlesID) {
    double cord_z = (*particles)[Bid].posZ + (*particles)[Bid].smoothVecZ * PASO_TIEMPO;
    if (cz == 0) {
      increm_z = TAMANO_PARTICULA - (cord_z - LIMITE_INFERIOR_RECINTO_Z);
    } else if (cz == data.nz - 1) {
      increm_z = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Z - cord_z);
    }
    if (increm_z > Epsilon) {
      if (cz == 0) {
        (*accelerationZ)[Bid] +=
            COLISIONES_RIGIDEZ * increm_z - AMORTIGUAMIENTO * (*particles)[Bid].velZ;
      } else if (cz == data.nz - 1) {
        (*accelerationZ)[Bid] -=
            COLISIONES_RIGIDEZ * increm_z + AMORTIGUAMIENTO * (*particles)[Bid].velZ;
      }
    }
  }
}

// Debe actualizar el movimiento de cada particula
void Block::particleMotion() {
  for (auto & Bid : particlesID) {
    (*particles)[Bid].posX = (*particles)[Bid].posX + (*particles)[Bid].smoothVecX * PASO_TIEMPO +
                             (*accelerationX)[Bid] * pow(PASO_TIEMPO, Two);
    (*particles)[Bid].posY = (*particles)[Bid].posY + (*particles)[Bid].smoothVecY * PASO_TIEMPO +
                             (*accelerationY)[Bid] * pow(PASO_TIEMPO, Two);
    (*particles)[Bid].posZ = (*particles)[Bid].posZ + (*particles)[Bid].smoothVecZ * PASO_TIEMPO +
                             (*accelerationZ)[Bid] * pow(PASO_TIEMPO, Two);
    (*particles)[Bid].velX =
        (*particles)[Bid].smoothVecX + (((*accelerationX)[Bid] * PASO_TIEMPO) / Two);
    (*particles)[Bid].velY =
        (*particles)[Bid].smoothVecY + (((*accelerationY)[Bid] * PASO_TIEMPO) / Two);
    (*particles)[Bid].velZ =
        (*particles)[Bid].smoothVecZ + (((*accelerationZ)[Bid] * PASO_TIEMPO) / Two);
    (*particles)[Bid].smoothVecX =
        (*particles)[Bid].smoothVecX + (*accelerationX)[Bid] * PASO_TIEMPO;
    (*particles)[Bid].smoothVecY =
        (*particles)[Bid].smoothVecY + (*accelerationY)[Bid] * PASO_TIEMPO;
    (*particles)[Bid].smoothVecZ =
        (*particles)[Bid].smoothVecZ + (*accelerationZ)[Bid] * PASO_TIEMPO;
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje x
void Block::interactionsX(unsigned int cx) {
  double dxB = 0;
  for (auto & Bid : particlesID) {
    if (cx == 0) {
      dxB = (*particles)[Bid].posX - LIMITE_INFERIOR_RECINTO_X;
    } else if (cx == data.nx - 1) {
      dxB = LIMITE_SUPERIOR_RECINTO_X - (*particles)[Bid].posX;
    }
    if (dxB < 0) {
      if (cx == 0) {
        (*particles)[Bid].posX = LIMITE_INFERIOR_RECINTO_X - dxB;
      } else if (cx == data.nx - 1) {
        (*particles)[Bid].posX = LIMITE_SUPERIOR_RECINTO_X + dxB;
      }
      (*particles)[Bid].velX       = -(*particles)[Bid].velX;
      (*particles)[Bid].smoothVecX = -(*particles)[Bid].smoothVecX;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje y
void Block::interactionsY(unsigned int cy) {
  double dyB = 0;
  for (auto & Bid : particlesID) {
    if (cy == 0) {
      dyB = (*particles)[Bid].posY - LIMITE_INFERIOR_RECINTO_Y;
    } else if (cy == data.ny - 1) {
      dyB = LIMITE_SUPERIOR_RECINTO_Y - (*particles)[Bid].posY;
    }
    if (dyB < 0) {
      if (cy == 0) {
        (*particles)[Bid].posY = LIMITE_INFERIOR_RECINTO_Y - dyB;
      } else if (cy == data.ny - 1) {
        (*particles)[Bid].posY = LIMITE_SUPERIOR_RECINTO_Y + dyB;
      }
      (*particles)[Bid].velY       = -(*particles)[Bid].velY;
      (*particles)[Bid].smoothVecY = -(*particles)[Bid].smoothVecY;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje z
void Block::interactionsZ(unsigned int cz) {
  double dzB = 0;
  for (auto & Bid : particlesID) {
    if (cz == 0) {
      dzB = (*particles)[Bid].posZ - LIMITE_INFERIOR_RECINTO_Z;
    } else if (cz == data.nz - 1) {
      dzB = LIMITE_SUPERIOR_RECINTO_Z - (*particles)[Bid].posZ;
    }
    if (dzB < 0) {
      if (cz == 0) {
        (*particles)[Bid].posZ = LIMITE_INFERIOR_RECINTO_Z - dzB;
      } else if (cz == data.nz - 1) {
        (*particles)[Bid].posZ = LIMITE_SUPERIOR_RECINTO_Z + dzB;
      }
      (*particles)[Bid].velZ       = -(*particles)[Bid].velZ;
      (*particles)[Bid].smoothVecZ = -(*particles)[Bid].smoothVecZ;
    }
  }
}
