#include "block_soa.hpp"

using namespace std;

// METODOS de BLOCK

// Metodo encargado de crear las parejas de particulas de un mismo bloque
void Block::generarParejasBloque() {
  for (size_t i = 0; i < particlesID.size(); i++) {
    for (size_t j = i + 1; j < particlesID.size(); j++) {
      particlePairs.push_back(make_pair(particlesID[i], particlesID[j]));
    }
  }
}

// Metodo encargado de crear las parejas de particulas entre bloques contiguos
void Block::generarParejasEntreBloques(Block & otherBlock, vector<std::pair<int, int>> & aux) {
  for (auto & id : particlesID) {
    for (auto & Cid : otherBlock.particlesID) { aux.push_back(make_pair(id, Cid)); }
  }
}

// Función para añadir una partícula al vector
void Block::addParticle(int id) {
  particlesID.push_back(id);
}

// Funcion para resetear el bloque
void Block::resetBlock() {
  particlesID.clear();
  particlePairs.clear();
}

// Funcion inicializar la densidad y la aceleracion para cada particula
void Block::initDensityAcceleration() {
  for (auto & id : particlesID) {
    density[id]       = 0;
    accelerationX[id] = ACELERACION_GRAVEDAD_X;
    accelerationY[id] = ACELERACION_GRAVEDAD_Y;
    accelerationZ[id] = ACELERACION_GRAVEDAD_Z;
  }
}

// Funcion encargada de modificar el vector de densidades del propio bloque
void Block::densityIncreaseSingle() {
  double aux_x, aux_y, aux_z, increm_density_pair;
  for (auto const & pair : particlePairs) {


    aux_x = std::pow(particles.posX[pair.first] - particles.posX[pair.second], 2);
    aux_y = std::pow(particles.posY[pair.first] - particles.posY[pair.second], 2);
    aux_z = std::pow(particles.posZ[pair.first] - particles.posZ[pair.second], 2);

    if (aux_x + aux_y + aux_z < data.h_square) {
      increm_density_pair = std::pow(data.h_square - (aux_x + aux_y + aux_z), 3);
    } else {
      increm_density_pair = 0;
    }
    density[pair.first]  += increm_density_pair;
    density[pair.second] += increm_density_pair;
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
void Block::calculate_increm_density(std::vector<std::pair<int, int>> ParejaParticulas) {
  double aux_x, aux_y, aux_z, increm_density_pair;
  for (auto const & pair : ParejaParticulas) {
    if (pair.first == 2496 || pair.second == 2496) { aux_x = 0; }
    aux_x = std::pow(particles.posX[pair.first] - particles.posX[pair.second], 2);
    aux_y = std::pow(particles.posY[pair.first] - particles.posY[pair.second], 2);
    aux_z = std::pow(particles.posZ[pair.first] - particles.posZ[pair.second], 2);
    if (aux_x + aux_y + aux_z < data.h_square) {
      increm_density_pair = std::pow(data.h_square - (aux_x + aux_y + aux_z), 3);
    } else {
      increm_density_pair = 0;
    }
    density[pair.first]  += increm_density_pair;
    density[pair.second] += increm_density_pair;
  }
}

// Metodo auxiliar que realiza los diferentes calculos para la transformacion lineal de la
// densidad en un mismo bloque
void Block::lineal_transformate_density() {
  for (auto & id : particlesID) {
    if (id == 2496) { density[id] = density[id]; }

    double h_pow6      = pow(data.long_suavizado, 6);
    double h_pow9      = pow(data.long_suavizado, 9);
    double m           = data.mass;
    double coeficiente = 315 / (64 * M_PI * h_pow9) * m;

    density[id] = (density[id] + h_pow6) * coeficiente;

    // density[id] *= 0.1;
  }
}

vector<double> Block::calculate_increm_aceleration(vector<double> position, vector<double> velocity,
                                                   double dist, vector<int> Id) {
  double pi_x = position[0];
  double pi_y = position[1];
  double pi_z = position[2];

  double vi_x = velocity[0];
  double vi_y = velocity[1];
  double vi_z = velocity[2];

  double rho_i = density[Id[0]];
  double rho_j = density[Id[1]];

  double term1 = 15 / (M_PI * pow(data.long_suavizado, 6));
  double term2 = 3 * data.mass * PRESION_RIGIDEZ / 2;
  double term3 = pow(data.long_suavizado - dist, 2) / dist;
  double term4 = rho_i + rho_j - 2 * DENSIDAD_FLUIDO;
  double term5 = 45 / (M_PI * pow(data.long_suavizado, 6)) * VISCOSIDAD * data.mass;
  double term6 = density[Id[0]] * density[Id[1]];

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
  double dist = sqrt(max(pow(posX, 2) + pow(posY, 2) + pow(posZ, 2), pow(10, -12)));
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
    if ((pow(particles.posX[pair.first] - particles.posX[pair.second], 2) +
         pow(particles.posY[pair.first] - particles.posY[pair.second], 2) +
         pow(particles.posZ[pair.first] - particles.posZ[pair.second], 2)) <
        pow(data.long_suavizado, 2)) {
      // Calculo de distancia
      double dist = calculate_dist(particles.posX[pair.first] - particles.posX[pair.second],
                                   particles.posY[pair.first] - particles.posY[pair.second],
                                   particles.posZ[pair.first] - particles.posZ[pair.second]);
      // Calculo vector posicion
      vector<double> position = {particles.posX[pair.first] - particles.posX[pair.second],
                                 particles.posY[pair.first] - particles.posY[pair.second],
                                 particles.posZ[pair.first] - particles.posZ[pair.second]};
      // Calculo vector velocidad
      vector<double> velocity = {particles.velX[pair.second] - particles.velX[pair.first],
                                 particles.velY[pair.second] - particles.velY[pair.first],
                                 particles.velZ[pair.second] - particles.velZ[pair.first]};

      vector<int> Id = {pair.first, pair.second};

      vector<double> increm_aceleration =
          calculate_increm_aceleration(position, velocity, dist, Id);

      accelerationX[pair.first]  = accelerationX[pair.first] + increm_aceleration[0];
      accelerationY[pair.first]  = accelerationY[pair.first] + increm_aceleration[1];
      accelerationZ[pair.first]  = accelerationZ[pair.first] + increm_aceleration[2];
      accelerationX[pair.second] = accelerationX[pair.second] - increm_aceleration[0];
      accelerationY[pair.second] = accelerationY[pair.second] - increm_aceleration[1];
      accelerationZ[pair.second] = accelerationZ[pair.second] - increm_aceleration[2];
    }
  }
}

// Debe actualizar la componente x del vector de aceleración para cada particula
void Block::collisionsX(unsigned int cx) {
  double cord_x, increm_x = 0;
  for (auto & id : particlesID) {
    { cord_x = particles.posX[id] + particles.smoothVecX[id] * PASO_TIEMPO; }
    if (cx == 0) {
      increm_x = TAMANO_PARTICULA - (cord_x - LIMITE_INFERIOR_RECINTO_X);
    } else if (cx == data.nx - 1) {
      increm_x = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_X - cord_x);
    }
    if (increm_x > pow(10, -10)) {
      if (cx == 0) {
        accelerationX[id] += COLISIONES_RIGIDEZ * increm_x - AMORTIGUAMIENTO * particles.velX[id];
      } else if (cx == data.nx - 1) {
        accelerationX[id] -= COLISIONES_RIGIDEZ * increm_x + AMORTIGUAMIENTO * particles.velX[id];
      }
    }
  }
}

// Debe actualizar la componente y del vector de aceleración para cada particula
void Block::collisionsY(unsigned int cy) {
  double cord_y, increm_y = 0;
  for (auto & id : particlesID) {
    cord_y = particles.posY[id] + particles.smoothVecY[id] * PASO_TIEMPO;
    if (cy == 0) {
      increm_y = TAMANO_PARTICULA - (cord_y - LIMITE_INFERIOR_RECINTO_Y);
    } else if (cy == data.ny - 1) {
      increm_y = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Y - cord_y);
    }
    if (increm_y > pow(10, -10)) {
      if (cy == 0) {
        accelerationY[id] += COLISIONES_RIGIDEZ * increm_y - AMORTIGUAMIENTO * particles.velY[id];
      } else if (cy == data.ny - 1) {
        accelerationY[id] -= COLISIONES_RIGIDEZ * increm_y + AMORTIGUAMIENTO * particles.velY[id];
      }
    }
  }
}

// Debe actualizar la componente z del vector de aceleración para cada particula
void Block::collisionsZ(unsigned int cz) {
  double cord_z, increm_z = 0;
  for (auto & id : particlesID) {
    cord_z = particles.posZ[id] + particles.smoothVecZ[id] * PASO_TIEMPO;
    if (cz == 0) {
      increm_z = TAMANO_PARTICULA - (cord_z - LIMITE_INFERIOR_RECINTO_Z);
    } else if (cz == data.nz - 1) {
      increm_z = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Z - cord_z);
    }
    if (increm_z > pow(10, -10)) {
      if (cz == 0) {
        accelerationZ[id] += COLISIONES_RIGIDEZ * increm_z - AMORTIGUAMIENTO * particles.velZ[id];
      } else if (cz == data.nz - 1) {
        accelerationZ[id] -= COLISIONES_RIGIDEZ * increm_z + AMORTIGUAMIENTO * particles.velZ[id];
      }
    }
  }
}

// Debe actualizar el movimiento de cada particula
void Block::particleMotion() {
  for (auto & id : particlesID) {
    particles.posX[id] = particles.posX[id] + particles.smoothVecX[id] * PASO_TIEMPO +
                         accelerationX[id] * pow(PASO_TIEMPO, 2);
    particles.posY[id] = particles.posY[id] + particles.smoothVecY[id]  * PASO_TIEMPO +
                         accelerationY[id] * pow(PASO_TIEMPO, 2);
    particles.posZ[id] = particles.posZ[id] + particles.smoothVecZ[id]  * PASO_TIEMPO +
                         accelerationZ[id] * pow(PASO_TIEMPO, 2);
    particles.velX[id]       = particles.smoothVecX[id] + ((accelerationX[id] * PASO_TIEMPO) / 2);
    particles.velY[id]       = particles.smoothVecY[id]  + ((accelerationY[id] * PASO_TIEMPO) / 2);
    particles.velZ[id]       = particles.smoothVecZ[id]  + ((accelerationZ[id] * PASO_TIEMPO) / 2);
    particles.smoothVecX[id] = particles.smoothVecX[id] + accelerationX[id] * PASO_TIEMPO;
    particles.smoothVecY[id]  = particles.smoothVecY[id]  + accelerationY[id] * PASO_TIEMPO;
    particles.smoothVecZ[id]  = particles.smoothVecZ[id]  + accelerationZ[id] * PASO_TIEMPO;
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje x
void Block::interactionsX(unsigned int cx) {
  double dx = 0;
  for (auto & id : particlesID) {
    if (cx == 0) {
      dx = particles.posX[id] - LIMITE_INFERIOR_RECINTO_X;
    } else if (cx == data.nx - 1) {
      dx = LIMITE_SUPERIOR_RECINTO_X - particles.posX[id];
    }
    if (dx < 0) {
      if (cx == 0) {
        particles.posX[id] = LIMITE_INFERIOR_RECINTO_X - dx;
      } else if (cx == data.nx - 1) {
        particles.posX[id] = LIMITE_SUPERIOR_RECINTO_X + dx;
      }
      particles.velX[id]       = -particles.velX[id];
      particles.smoothVecX[id] = -particles.smoothVecX[id];
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje y
void Block::interactionsY(unsigned int cy) {
  double dy = 0;
  for (auto & id : particlesID) {
    if (cy == 0) {
      dy = particles.posY[id] - LIMITE_INFERIOR_RECINTO_Y;
    } else if (cy == data.ny - 1) {
      dy = LIMITE_SUPERIOR_RECINTO_Y - particles.posY[id];
    }
    if (dy < 0) {
      if (cy == 0) {
        particles.posY[id] = LIMITE_INFERIOR_RECINTO_Y - dy;
      } else if (cy == data.ny - 1) {
        particles.posY[id] = LIMITE_SUPERIOR_RECINTO_Y + dy;
      }
      particles.velY[id]       = -particles.velY[id];
      particles.smoothVecY[id]  = -particles.smoothVecY[id] ;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje z
void Block::interactionsZ(unsigned int cz) {
  double dz = 0;
  for (auto & id : particlesID) {
    if (cz == 0) {
      dz = particles.posZ[id] - LIMITE_INFERIOR_RECINTO_Z;
    } else if (cz == data.nz - 1) {
      dz = LIMITE_SUPERIOR_RECINTO_Z - particles.posZ[id];
    }
    if (dz < 0) {
      if (cz == 0) {
        particles.posZ[id] = LIMITE_INFERIOR_RECINTO_Z - dz;
      } else if (cz == data.nz - 1) {
        particles.posZ[id] = LIMITE_SUPERIOR_RECINTO_Z + dz;
      }
      particles.velZ[id]       = -particles.velZ[id];
      particles.smoothVecZ[id]  = -particles.smoothVecZ[id] ;
    }
  }
}
