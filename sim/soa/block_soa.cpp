#include "block_soa.hpp"

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
  for (auto & Bid : particlesID) {
    (*density)[Bid]       = 0;
    (*accelerationX)[Bid] = ACELERACION_GRAVEDAD_X;
    (*accelerationY)[Bid] = ACELERACION_GRAVEDAD_Y;
    (*accelerationZ)[Bid] = ACELERACION_GRAVEDAD_Z;
  }
}

// Funcion encargada de modificar el vector de densidades del propio bloque
void Block::densityIncreaseSingle() {
  double increm_density_pair = 0;
  for (auto const & pair : particlePairs) {
    double aux_x = std::pow((*particles).posX[pair.first] - (*particles).posX[pair.second],Two);
    double aux_y = std::pow((*particles).posY[pair.first] - (*particles).posY[pair.second],Two);
    double aux_z = std::pow((*particles).posZ[pair.first] - (*particles).posZ[pair.second],Two);

    if (aux_x + aux_y + aux_z < data.h_square) {
      increm_density_pair = std::pow(data.h_square - (aux_x + aux_y + aux_z), Three);
    } else {
      increm_density_pair = 0;
    }
    (*density)[pair.first]  += increm_density_pair;
    (*density)[pair.second] += increm_density_pair;
  }
}

// Funcion encargada de modificar el vector de densidades del propio bloque con su contiguo
void Block::densityIncrease(Block & contiguousBlock) {
  // Parte del bloque contiguo
  vector<std::pair<int, int>> aux;
  generarParejasEntreBloques(contiguousBlock, aux);
  calculate_increm_density(aux);
  // lineal_transformate_density(contiguousBlock);
}

// Metodo auxiliar que realiza los diferentes calculos para el incremento de densidad
void Block::calculate_increm_density(std::vector<std::pair<int, int>> ParejaParticulas) const {
  double increm_density_pair = 0;
  for (auto const & pair : ParejaParticulas) {
    double aux_x = std::pow((*particles).posX[pair.first] - (*particles).posX[pair.second], Two);
    double aux_y = std::pow((*particles).posY[pair.first] - (*particles).posY[pair.second], Two);
    double aux_z = std::pow((*particles).posZ[pair.first] - (*particles).posZ[pair.second], Two);
    if (aux_x + aux_y + aux_z < data.h_square) {
      increm_density_pair = std::pow(data.h_square - (aux_x + aux_y + aux_z), Three);
    } else {
      increm_density_pair = 0;
    }
    (*density)[pair.first]  += increm_density_pair;
    (*density)[pair.second] += increm_density_pair;
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

vector<double> Block::calculate_increm_aceleration(vector<double> position, vector<double> velocity,
                                                   double dist, vector<int> Bid) {
  const double pi_x = position[0];
  const double pi_y = position[1];
  const double pi_z = position[2];

  const double vi_x = velocity[0];
  const double vi_y = velocity[1];
  const double vi_z = velocity[2];

  const double rho_i = (*density)[Bid[0]];
  const double rho_j = (*density)[Bid[1]];

  const double term1 = Fifteen / (M_PI * pow(data.long_suavizado, Six));
  const double term2 = Three * data.mass * PRESION_RIGIDEZ / Two;
  const double term3 = pow(data.long_suavizado - dist, Two) / dist;
  const double term4 = rho_i + rho_j - Two * DENSIDAD_FLUIDO;
  const double term5 = FortyFive / (M_PI * pow(data.long_suavizado, Six)) * VISCOSIDAD * data.mass;
  const double term6 = (*density)[Bid[0]] * (*density)[Bid[1]];

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

// Metodo auxiliar que realiza los diferentes calculos para la aceleracion
void Block::accelerationTransferCalculations(vector<std::pair<int, int>> pair_vec) {
  for (auto const & pair : pair_vec) {
    // Si las dos particulas estan cerca
    if ((pow((*particles).posX[pair.first] - (*particles).posX[pair.second], 2) +
         pow((*particles).posY[pair.first] - (*particles).posY[pair.second], 2) +
         pow((*particles).posZ[pair.first] - (*particles).posZ[pair.second], 2)) <
        pow(data.long_suavizado, Two)) {
      // Calculo de distancia
      double dist = calculate_dist((*particles).posX[pair.first] - (*particles).posX[pair.second],
                                   (*particles).posY[pair.first] - (*particles).posY[pair.second],
                                   (*particles).posZ[pair.first] - (*particles).posZ[pair.second]);
      // Calculo vector posicion
      vector<double> position = {(*particles).posX[pair.first] - (*particles).posX[pair.second],
                                 (*particles).posY[pair.first] - (*particles).posY[pair.second],
                                 (*particles).posZ[pair.first] - (*particles).posZ[pair.second]};
      // Calculo vector velocidad
      vector<double> velocity = {(*particles).velX[pair.second] - (*particles).velX[pair.first],
                                 (*particles).velY[pair.second] - (*particles).velY[pair.first],
                                 (*particles).velZ[pair.second] - (*particles).velZ[pair.first]};

      vector<int> Bid = {pair.first, pair.second};

      vector<double> increm_aceleration =
          calculate_increm_aceleration(position, velocity, dist, Bid);

      (*accelerationX)[pair.first]  = (*accelerationX)[pair.first] + increm_aceleration[0];
      (*accelerationY)[pair.first]  = (*accelerationY)[pair.first] + increm_aceleration[1];
      (*accelerationZ)[pair.first]  = (*accelerationZ)[pair.first] + increm_aceleration[2];
      (*accelerationX)[pair.second] = (*accelerationX)[pair.second] - increm_aceleration[0];
      (*accelerationY)[pair.second] = (*accelerationY)[pair.second] - increm_aceleration[1];
      (*accelerationZ)[pair.second] = (*accelerationZ)[pair.second] - increm_aceleration[2];
    }
  }
}

// Debe actualizar la componente x del vector de aceleración para cada particula
void Block::collisionsX(unsigned int cx) {
  double cord_x, increm_x = 0;
  for (auto & Bid : particlesID) {
    { cord_x = (*particles).posX[Bid] + (*particles).smoothVecX[Bid] * PASO_TIEMPO; }
    if (cx == 0) {
      increm_x = TAMANO_PARTICULA - (cord_x - LIMITE_INFERIOR_RECINTO_X);
    } else if (cx == data.nx - 1) {
      increm_x = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_X - cord_x);
    }
    if (increm_x > Epsilon) {
      if (cx == 0) {
        (*accelerationX)[Bid] += COLISIONES_RIGIDEZ * increm_x - AMORTIGUAMIENTO * (*particles).velX[Bid];
      } else if (cx == data.nx - 1) {
        (*accelerationX)[Bid] -= COLISIONES_RIGIDEZ * increm_x + AMORTIGUAMIENTO * (*particles).velX[Bid];
      }
    }
  }
}

// Debe actualizar la componente y del vector de aceleración para cada particula
void Block::collisionsY(unsigned int cy) {
  double cord_y, increm_y = 0;
  for (auto & Bid : particlesID) {
    cord_y = (*particles).posY[Bid] + (*particles).smoothVecY[Bid] * PASO_TIEMPO;
    if (cy == 0) {
      increm_y = TAMANO_PARTICULA - (cord_y - LIMITE_INFERIOR_RECINTO_Y);
    } else if (cy == data.ny - 1) {
      increm_y = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Y - cord_y);
    }
    if (increm_y > Epsilon) {
      if (cy == 0) {
        (*accelerationY)[Bid] += COLISIONES_RIGIDEZ * increm_y - AMORTIGUAMIENTO * (*particles).velY[Bid];
      } else if (cy == data.ny - 1) {
        (*accelerationY)[Bid] -= COLISIONES_RIGIDEZ * increm_y + AMORTIGUAMIENTO * (*particles).velY[Bid];
      }
    }
  }
}

// Debe actualizar la componente z del vector de aceleración para cada particula
void Block::collisionsZ(unsigned int cz) {
  double cord_z, increm_z = 0;
  for (auto & Bid : particlesID) {
    cord_z = (*particles).posZ[Bid] + (*particles).smoothVecZ[Bid] * PASO_TIEMPO;
    if (cz == 0) {
      increm_z = TAMANO_PARTICULA - (cord_z - LIMITE_INFERIOR_RECINTO_Z);
    } else if (cz == data.nz - 1) {
      increm_z = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Z - cord_z);
    }
    if (increm_z > Epsilon) {
      if (cz == 0) {
        (*accelerationZ)[Bid] += COLISIONES_RIGIDEZ * increm_z - AMORTIGUAMIENTO * (*particles).velZ[Bid];
      } else if (cz == data.nz - 1) {
        (*accelerationZ)[Bid] -= COLISIONES_RIGIDEZ * increm_z + AMORTIGUAMIENTO * (*particles).velZ[Bid];
      }
    }
  }
}

// Debe actualizar el movimiento de cada particula
void Block::particleMotion() {
  for (auto & Bid : particlesID) {
    (*particles).posX[Bid] = (*particles).posX[Bid] + (*particles).smoothVecX[Bid] * PASO_TIEMPO +
                          (*accelerationX)[Bid] * pow(PASO_TIEMPO, Two);
    (*particles).posY[Bid] = (*particles).posY[Bid] + (*particles).smoothVecY[Bid] * PASO_TIEMPO +
                          (*accelerationY)[Bid] * pow(PASO_TIEMPO, Two);
    (*particles).posZ[Bid] = (*particles).posZ[Bid] + (*particles).smoothVecZ[Bid] * PASO_TIEMPO +
                          (*accelerationZ)[Bid] * pow(PASO_TIEMPO, Two);
    (*particles).velX[Bid] = (*particles).smoothVecX[Bid] + (((*accelerationX)[Bid] * PASO_TIEMPO) / Two);
    (*particles).velY[Bid] = (*particles).smoothVecY[Bid] + (((*accelerationY)[Bid] * PASO_TIEMPO) / Two);
    (*particles).velZ[Bid] = (*particles).smoothVecZ[Bid] + (((*accelerationZ)[Bid] * PASO_TIEMPO) / Two);
    (*particles).smoothVecX[Bid] = (*particles).smoothVecX[Bid] + (*accelerationX)[Bid] * PASO_TIEMPO;
    (*particles).smoothVecY[Bid] = (*particles).smoothVecY[Bid] + (*accelerationY)[Bid] * PASO_TIEMPO;
    (*particles).smoothVecZ[Bid] = (*particles).smoothVecZ[Bid] + (*accelerationZ)[Bid] * PASO_TIEMPO;
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje x
void Block::interactionsX(unsigned int cx) {
  double dxB = 0;
  for (auto & Bid : particlesID) {
    if (cx == 0) {
      dxB = (*particles).posX[Bid] - LIMITE_INFERIOR_RECINTO_X;
    } else if (cx == data.nx - 1) {
      dxB = LIMITE_SUPERIOR_RECINTO_X - (*particles).posX[Bid];
    }
    if (dxB < 0) {
      if (cx == 0) {
        (*particles).posX[Bid] = LIMITE_INFERIOR_RECINTO_X - dxB;
      } else if (cx == data.nx - 1) {
        (*particles).posX[Bid] = LIMITE_SUPERIOR_RECINTO_X + dxB;
      }
      (*particles).velX[Bid]       = -(*particles).velX[Bid];
      (*particles).smoothVecX[Bid] = -(*particles).smoothVecX[Bid];
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje y
void Block::interactionsY(unsigned int cy) {
  double dyB = 0;
  for (auto & Bid : particlesID) {
    if (cy == 0) {
      dyB = (*particles).posY[Bid] - LIMITE_INFERIOR_RECINTO_Y;
    } else if (cy == data.ny - 1) {
      dyB = LIMITE_SUPERIOR_RECINTO_Y - (*particles).posY[Bid];
    }
    if (dyB < 0) {
      if (cy == 0) {
        (*particles).posY[Bid] = LIMITE_INFERIOR_RECINTO_Y - dyB;
      } else if (cy == data.ny - 1) {
        (*particles).posY[Bid] = LIMITE_SUPERIOR_RECINTO_Y + dyB;
      }
      (*particles).velY[Bid]       = -(*particles).velY[Bid];
      (*particles).smoothVecY[Bid] = -(*particles).smoothVecY[Bid];
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje z
void Block::interactionsZ(unsigned int cz) {
  double dzB = 0;
  for (auto & Bid : particlesID) {
    if (cz == 0) {
      dzB = (*particles).posZ[Bid] - LIMITE_INFERIOR_RECINTO_Z;
    } else if (cz == data.nz - 1) {
      dzB = LIMITE_SUPERIOR_RECINTO_Z - (*particles).posZ[Bid];
    }
    if (dzB < 0) {
      if (cz == 0) {
        (*particles).posZ[Bid] = LIMITE_INFERIOR_RECINTO_Z - dzB;
      } else if (cz == data.nz - 1) {
        (*particles).posZ[Bid] = LIMITE_SUPERIOR_RECINTO_Z + dzB;
      }
      (*particles).velZ[Bid]       = -(*particles).velZ[Bid];
      (*particles).smoothVecZ[Bid] = -(*particles).smoothVecZ[Bid];
    }
  }
}
