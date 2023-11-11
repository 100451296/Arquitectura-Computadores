#include "block.hpp"

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

void Block::resetBlock() {
  particlesID.clear();
  particlePairs.clear();
}

// Funcion encargada de calcular la masa y longitud de suavizado de todas las particulas de un
// bloque
void Block::calculateDataCommon() {
  data.mass           = DENSIDAD_FLUIDO / pow(data.ppm, 3);
  data.long_suavizado = MULTIPLICADOR_RADIO / data.ppm;
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
  calculate_increm_density_single();
  // lineal_transformate_density();
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
    aux_x = std::pow(particles[pair.first].posX - particles[pair.second].posX, 2);
    aux_y = std::pow(particles[pair.first].posY - particles[pair.second].posY, 2);
    aux_z = std::pow(particles[pair.first].posZ - particles[pair.second].posZ, 2);
    if (aux_x + aux_y + aux_z < std::pow(data.long_suavizado, 2)) {
      increm_density_pair = std::pow(std::pow(data.long_suavizado, 2) - (aux_x + aux_y + aux_z), 3);
    } else {
      increm_density_pair = 0;
    }
    density[pair.first]  += increm_density_pair;
    density[pair.second] += increm_density_pair;
  }
}

// Metodo auxiliar que realiza los diferentes calculos para el incremento de densidad
void Block::calculate_increm_density_single() {
  double aux_x, aux_y, aux_z, increm_density_pair;
  for (auto const & pair : particlePairs) {
    aux_x = std::pow(particles[pair.first].posX - particles[pair.second].posX, 2);
    aux_y = std::pow(particles[pair.first].posY - particles[pair.second].posY, 2);
    aux_z = std::pow(particles[pair.first].posZ - particles[pair.second].posZ, 2);
    if (aux_x + aux_y + aux_z < std::pow(data.long_suavizado, 2)) {
      increm_density_pair = std::pow(std::pow(data.long_suavizado, 2) - (aux_x + aux_y + aux_z), 3);
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
    if (id == 2496) {
      density[id] = density[id];
      cout << "id" << endl;
    }

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
  if (Id[0] == 2496 || Id[1] == 2496) {
    cout << "id";
    cout << endl;
  }

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
    if ((pow(particles[pair.first].posX - particles[pair.second].posX, 2) +
         pow(particles[pair.first].posY - particles[pair.second].posY, 2) +
         pow(particles[pair.first].posZ - particles[pair.second].posZ, 2)) <
        pow(data.long_suavizado, 2)) {
      // Calculo de distancia
      double dist = calculate_dist(particles[pair.first].posX - particles[pair.second].posX,
                                   particles[pair.first].posY - particles[pair.second].posY,
                                   particles[pair.first].posZ - particles[pair.second].posZ);
      // Calculo vector posicion
      vector<double> position = {particles[pair.first].posX - particles[pair.second].posX,
                                 particles[pair.first].posY - particles[pair.second].posY,
                                 particles[pair.first].posZ - particles[pair.second].posZ};
      // Calculo vector velocidad
      vector<double> velocity = {particles[pair.first].velX - particles[pair.second].velX,
                                 particles[pair.first].velY - particles[pair.second].velY,
                                 particles[pair.first].velZ - particles[pair.second].velZ};

      vector<int> Id = {pair.first, pair.second};

      vector<double> increm_aceleration =
          calculate_increm_aceleration(position, velocity, dist, Id);

      accelerationX[pair.first]  = accelerationY[pair.first] + increm_aceleration[0];
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
    cord_x = particles[id].posX + particles[id].smoothVecX * PASO_TIEMPO;
    if (cx == 0) {
      increm_x = TAMANO_PARTICULA - (cord_x - LIMITE_INFERIOR_RECINTO_X);
    } else if (cx == data.nx - 1) {
      increm_x = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_X - cord_x);
    }
    if (increm_x > pow(10, -10)) {
      if (cx == 0) {
        accelerationX[id] = accelerationX[id] + COLISIONES_RIGIDEZ * increm_x -
                            AMORTIGUAMIENTO * particles[id].velX;
      } else if (cx == data.nx - 1) {
        accelerationX[id] = accelerationX[id] - COLISIONES_RIGIDEZ * increm_x +
                            AMORTIGUAMIENTO * particles[id].velX;
      }
    }
  }
}

// Debe actualizar la componente y del vector de aceleración para cada particula
void Block::collisionsY(unsigned int cy) {
  double cord_y, increm_y = 0;
  for (auto & id : particlesID) {
    cord_y = particles[id].posY + particles[id].smoothVecY * PASO_TIEMPO;
    if (cy == 0) {
      increm_y = TAMANO_PARTICULA - (cord_y - LIMITE_INFERIOR_RECINTO_Y);
    } else if (cy == data.ny - 1) {
      increm_y = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Y - cord_y);
    }
    if (increm_y > pow(10, -10)) {
      if (cy == 0) {
        accelerationY[id] = accelerationY[id] + COLISIONES_RIGIDEZ * increm_y -
                            AMORTIGUAMIENTO * particles[id].velY;
      } else if (cy == data.ny - 1) {
        accelerationY[id] = accelerationY[id] - COLISIONES_RIGIDEZ * increm_y +
                            AMORTIGUAMIENTO * particles[id].velY;
      }
    }
  }
}

// Debe actualizar la componente z del vector de aceleración para cada particula
void Block::collisionsZ(unsigned int cz) {
  double cord_z, increm_z = 0;
  for (auto & id : particlesID) {
    cord_z = particles[id].posZ + particles[id].smoothVecZ * PASO_TIEMPO;
    if (cz == 0) {
      increm_z = TAMANO_PARTICULA - (cord_z - LIMITE_INFERIOR_RECINTO_Z);
    } else if (cz == data.nz - 1) {
      increm_z = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Z - cord_z);
    }
    if (increm_z > pow(10, -10)) {
      if (cz == 0) {
        accelerationZ[id] = accelerationZ[id] + COLISIONES_RIGIDEZ * increm_z -
                            AMORTIGUAMIENTO * particles[id].velZ;
      } else if (cz == data.nz - 1) {
        accelerationZ[id] = accelerationZ[id] - COLISIONES_RIGIDEZ * increm_z +
                            AMORTIGUAMIENTO * particles[id].velZ;
      }
    }
  }
}

// Debe actualizar el movimiento de cada particula
void Block::particleMotion() {
  for (auto & id : particlesID) {
    particles[id].posX = particles[id].posX + particles[id].smoothVecX * PASO_TIEMPO +
                         accelerationX[id] * pow(PASO_TIEMPO, 2);
    particles[id].posY = particles[id].posY + particles[id].smoothVecY * PASO_TIEMPO +
                         accelerationY[id] * pow(PASO_TIEMPO, 2);
    particles[id].posZ = particles[id].posZ + particles[id].smoothVecZ * PASO_TIEMPO +
                         accelerationZ[id] * pow(PASO_TIEMPO, 2);
    particles[id].velX       = particles[id].smoothVecX + ((accelerationX[id] * PASO_TIEMPO) / 2);
    particles[id].velY       = particles[id].smoothVecY + ((accelerationY[id] * PASO_TIEMPO) / 2);
    particles[id].velZ       = particles[id].smoothVecZ + ((accelerationZ[id] * PASO_TIEMPO) / 2);
    particles[id].smoothVecX = particles[id].smoothVecX + accelerationX[id] * PASO_TIEMPO;
    particles[id].smoothVecY = particles[id].smoothVecY + accelerationY[id] * PASO_TIEMPO;
    particles[id].smoothVecZ = particles[id].smoothVecZ + accelerationZ[id] * PASO_TIEMPO;
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje x
void Block::interactionsX(unsigned int cx) {
  double dx = 0;
  for (auto & id : particlesID) {
    if (cx == 0) {
      dx = particles[id].posX - LIMITE_INFERIOR_RECINTO_X;
    } else if (cx == data.nx - 1) {
      dx = LIMITE_SUPERIOR_RECINTO_X - particles[id].posX;
    }
    if (dx < 0) {
      if (cx == 0) {
        particles[id].posX = LIMITE_INFERIOR_RECINTO_X - dx;
      } else if (cx == data.nx - 1) {
        particles[id].posX = LIMITE_SUPERIOR_RECINTO_X + dx;
      }
      particles[id].velX       = -particles[id].velX;
      particles[id].smoothVecX = -particles[id].smoothVecX;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje y
void Block::interactionsY(unsigned int cy) {
  double dy = 0;
  for (auto & id : particlesID) {
    if (cy == 0) {
      dy = particles[id].posY - LIMITE_INFERIOR_RECINTO_Y;
    } else if (cy == data.ny - 1) {
      dy = LIMITE_SUPERIOR_RECINTO_Y - particles[id].posY;
    }
    if (dy < 0) {
      if (cy == 0) {
        particles[id].posY = LIMITE_INFERIOR_RECINTO_Y - dy;
      } else if (cy == data.ny - 1) {
        particles[id].posY = LIMITE_SUPERIOR_RECINTO_Y + dy;
      }
      particles[id].velY       = -particles[id].velY;
      particles[id].smoothVecY = -particles[id].smoothVecY;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje z
void Block::interactionsZ(unsigned int cz) {
  double dz = 0;
  for (auto & id : particlesID) {
    if (cz == 0) {
      dz = particles[id].posZ - LIMITE_INFERIOR_RECINTO_Z;
    } else if (cz == data.nz - 1) {
      dz = LIMITE_SUPERIOR_RECINTO_Z - particles[id].posZ;
    }
    if (dz < 0) {
      if (cz == 0) {
        particles[id].posZ = LIMITE_INFERIOR_RECINTO_Z - dz;
      } else if (cz == data.nz - 1) {
        particles[id].posZ = LIMITE_SUPERIOR_RECINTO_Z + dz;
      }
      particles[id].velZ       = -particles[id].velZ;
      particles[id].smoothVecZ = -particles[id].smoothVecZ;
    }
  }
}
