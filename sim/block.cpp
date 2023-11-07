#include "block.hpp"

using namespace std;

// METODOS de BLOCK

// Metodo encargado de crear las parejas de particulas de un mismo bloque
void Block::generarParejasBloque() {
  for (size_t i = 0; i < particles.size(); ++i) {
    for (size_t j = i + 1; j < particles.size(); ++j) {
      particlePairs.push_back(make_pair(particles[i], particles[j]));
    }
  }
}

// Metodo encargado de crear las parejas de particulas entre bloques contiguos
vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>>
    Block::generarParejasEntreBloques(Block & otherBlock) {
  // Generar pares entre el bloque actual y el bloque contiguo
  vector<std::pair<std::shared_ptr<Particle>, std::shared_ptr<Particle>>> aux;
  for (size_t i = 0; i < particles.size(); ++i) {
    for (size_t j = 0; j < otherBlock.particles.size(); ++j) {
      aux.push_back(make_pair(particles[i], otherBlock.particles[j]));
    }
  }
  return aux;
}

// Función para añadir una partícula al vector
void Block::addParticle(shared_ptr<Particle> particle) {
  this->particles.push_back(particle);
  this->accelerationX.push_back(0.0);  // Agrega una entrada 0 a accelerationX
  this->accelerationY.push_back(0.0);  // Agrega una entrada 0 a accelerationY
  this->accelerationZ.push_back(0.0);  // Agrega una entrada 0 a accelerationZ
  this->density.push_back(0.0);        // Agrega una entrada 0 a density
}

void Block::resetBlock() {
  particles.clear();
  particlePairs.clear();
  accelerationX.clear();
  accelerationY.clear();
  accelerationZ.clear();
  density.clear();
}

// Funcion encargada de calcular la masa y longitud de suavizado de todas las particulas de un
// bloque
void Block::calculateDataCommon() {
  data->mass           = DENSIDAD_FLUIDO / pow(data->ppm, 3);
  data->long_suavizado = MULTIPLICADOR_RADIO / data->ppm;
}

// Funcion inicializar la densidad y la aceleracion para cada particula
void Block::initDensityAcceleration() {
  for (size_t i = 0; i < accelerationX.size(); ++i) {
    density[i]       = 0;
    accelerationX[i] = ACELERACION_GRAVEDAD_X;
    accelerationY[i] = ACELERACION_GRAVEDAD_Y;
    accelerationZ[i] = ACELERACION_GRAVEDAD_Z;
  }
}

// Funcion encargada de modificar el vector de densidades del propio bloque y transformacion lineal
void Block::densityIncreaseSingle() {
  calculate_increm_density(this->particlePairs);
  lineal_transformate_density();
}

void Block::densityIncrease(Block & contiguousBlock) {
  // Parte del bloque contiguo
  vector<std::pair<shared_ptr<Particle>, shared_ptr<Particle>>> aux =
      generarParejasEntreBloques(contiguousBlock);
  calculate_increm_density(aux);
  lineal_transformate_density(contiguousBlock);
}

// Metodo auxiliar que realiza los diferentes calculos para el incremento de densidad
void Block::calculate_increm_density(
    vector<std::pair<shared_ptr<Particle>, shared_ptr<Particle>>> ParejaParticulas) {
  double aux_x, aux_y, aux_z, increm_density_pair;
  for (auto const & pair : ParejaParticulas) {
    aux_x = pow(pair.first->posX - pair.second->posX, 2);
    aux_y = pow(pair.first->posY - pair.second->posY, 2);
    aux_z = pow(pair.first->posZ - pair.second->posZ, 2);
    if (aux_x + aux_y + aux_z < pow(data->long_suavizado, 2)) {
      increm_density_pair = pow(pow(data->long_suavizado, 2) - (aux_x + aux_y + aux_z), 3);
    } else {
      increm_density_pair = 0;
    }
    density[pair.first->id]  = density[pair.first->id] + increm_density_pair;
    density[pair.second->id] = density[pair.second->id] + increm_density_pair;
  }
}

// Metodo auxiliar que realiza los diferentes calculos para la transformacion lineal de la densidad
// en un mismo bloque
void Block::lineal_transformate_density() {
  for (size_t i = 0; i < density.size(); i++) {
    density[i] = (density[i] + pow(data->long_suavizado, 6)) * 315 * data->mass /
                 (64 * numbers::pi * pow(data->long_suavizado, 9));
  }
}

// Metodo auxiliar que realiza los diferentes calculos para la transformacion lineal de la densidad
// respecto a un bloque contiguo
void Block::lineal_transformate_density(Block & contiguousBlock) {
  for (size_t i = 0; i < contiguousBlock.density.size(); i++) {
    density[i] = (density[i] + pow(data->long_suavizado, 6)) * 315 * data->mass /
                 (64 * numbers::pi * pow(data->long_suavizado, 9));

    contiguousBlock.density[i] = (density[i] + pow(data->long_suavizado, 6)) * 315 * data->mass /
                                 (64 * numbers::pi * pow(data->long_suavizado, 9));
  }
}

// Metodo auxiliar que realiza los diferentes calculos para el incremento de aceleracion
vector<double> Block::calculate_increm_aceleration(vector<double> position, vector<double> velocity,
                                                   double dist, vector<unsigned int> Id) {
  vector<double> increm_aceleration;
  increm_aceleration = {
    ((position[0]) * (15 / (numbers::pi * pow(data->long_suavizado, 6))) *
         (3 * data->mass * PRESION_RIGIDEZ / 2) * ((pow(data->long_suavizado - dist, 2)) / dist) *
         (density[Id[0]] + density[Id[1]] - 2 * DENSIDAD_FLUIDO) +
     (velocity[0]) * (45 / (numbers::pi * pow(data->long_suavizado, 6))) * VISCOSIDAD *
         data->mass) /
        (density[Id[0]] * density[Id[1]]),
    ((position[1]) * (15 / (numbers::pi * pow(data->long_suavizado, 6))) *
         (3 * data->mass * PRESION_RIGIDEZ / 2) * ((pow(data->long_suavizado - dist, 2)) / dist) *
         (density[Id[0]] + density[Id[1]] - 2 * DENSIDAD_FLUIDO) +
     (velocity[1]) * (45 / (numbers::pi * pow(data->long_suavizado, 6))) * VISCOSIDAD *
         data->mass) /
        (density[Id[0]] * density[Id[1]]),
    ((position[2]) * (15 / (numbers::pi * pow(data->long_suavizado, 6))) *
         (3 * data->mass * PRESION_RIGIDEZ / 2) * ((pow(data->long_suavizado - dist, 2)) / dist) *
         (density[Id[0]] + density[Id[1]] - 2 * DENSIDAD_FLUIDO) +
     (velocity[2]) * (45 / (numbers::pi * pow(data->long_suavizado, 6))) * VISCOSIDAD *
         data->mass) /
        (density[Id[0]] * density[Id[1]])};
  return increm_aceleration;
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
  vector<std::pair<shared_ptr<Particle>, shared_ptr<Particle>>> aux =
      generarParejasEntreBloques(contiguousBlock);
  accelerationTransferCalculations(aux);
}

// Metodo auxiliar que realiza los diferentes calculos para la aceleracion
void Block::accelerationTransferCalculations(
    vector<std::pair<shared_ptr<Particle>, shared_ptr<Particle>>> pair_vec) {
  for (auto const & pair : pair_vec) {
    if (pow(pair.first->posX - pair.second->posX, 2) +
            pow(pair.first->posY - pair.second->posY, 2) +
            pow(pair.first->posZ - pair.second->posZ, 2) <
        pow(data->long_suavizado, 2)) {
      double dist =
          calculate_dist(pair.first->posX - pair.second->posX, pair.first->posY - pair.second->posY,
                         pair.first->posZ - pair.second->posZ);
      vector<double> position = {pair.first->posX - pair.second->posX,
                                 pair.first->posY - pair.second->posY,
                                 pair.first->posZ - pair.second->posZ};
      vector<double> velocity = {pair.first->velX - pair.second->velX,
                                 pair.first->velY - pair.second->velY,
                                 pair.first->velZ - pair.second->velZ};
      vector<unsigned int> Id = {pair.first->id, pair.second->id};
      vector<double> increm_aceleration =
          calculate_increm_aceleration(position, velocity, dist, Id);
      accelerationX[pair.first->id] = accelerationX[pair.first->id]+ increm_aceleration[0];
      accelerationY[pair.first->id] = accelerationY[pair.first->id] + increm_aceleration[1];
      accelerationZ[pair.first->id] = accelerationZ[pair.first->id] + increm_aceleration[2];
      accelerationX[pair.second->id] = accelerationX[pair.second->id] + increm_aceleration[0];
      accelerationY[pair.second->id] = accelerationY[pair.second->id] + increm_aceleration[1];
      accelerationZ[pair.second->id] = accelerationZ[pair.second->id] + increm_aceleration[2];
    }
  }
}

// Debe actualizar la componente x del vector de aceleración para cada particula
void Block::collisionsX(unsigned int cx) {
  double cord_x, increm_x = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    cord_x = particles[i]->posX + particles[i]->smoothVecX * PASO_TIEMPO;
    if (cx == 0) {
      increm_x = TAMANO_PARTICULA - (cord_x - LIMITE_INFERIOR_RECINTO_X);
    } else if (cx == data->nx - 1) {
      increm_x = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_X - cord_x);
    }
    if (increm_x > pow(10, -10)) {
      if (cx == 0) {
        accelerationX[i] =
            accelerationX[i] + COLISIONES_RIGIDEZ * increm_x - AMORTIGUAMIENTO * particles[i]->velX;
      } else if (cx == data->nx - 1) {
        accelerationX[i] =
            accelerationX[i] - COLISIONES_RIGIDEZ * increm_x + AMORTIGUAMIENTO * particles[i]->velX;
      }
    }
  }
}

// Debe actualizar la componente y del vector de aceleración para cada particula
void Block::collisionsY(unsigned int cy) {
  double cord_y, increm_y = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    cord_y = particles[i]->posY + particles[i]->smoothVecY * PASO_TIEMPO;
    if (cy == 0) {
      increm_y = TAMANO_PARTICULA - (cord_y - LIMITE_INFERIOR_RECINTO_Y);
    } else if (cy == data->ny - 1) {
      increm_y = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Y - cord_y);
    }
    if (increm_y > pow(10, -10)) {
      if (cy == 0) {
        accelerationY[i] =
            accelerationY[i] + COLISIONES_RIGIDEZ * increm_y - AMORTIGUAMIENTO * particles[i]->velY;
      } else if (cy == data->ny - 1) {
        accelerationY[i] =
            accelerationY[i] - COLISIONES_RIGIDEZ * increm_y + AMORTIGUAMIENTO * particles[i]->velY;
      }
    }
  }
}

// Debe actualizar la componente z del vector de aceleración para cada particula
void Block::collisionsZ(unsigned int cz) {
  double cord_z, increm_z = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    cord_z = particles[i]->posZ + particles[i]->smoothVecZ * PASO_TIEMPO;
    if (cz == 0) {
      increm_z = TAMANO_PARTICULA - (cord_z - LIMITE_INFERIOR_RECINTO_Z);
    } else if (cz == data->nz - 1) {
      increm_z = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Z - cord_z);
    }
    if (increm_z > pow(10, -10)) {
      if (cz == 0) {
        accelerationZ[i] =
            accelerationZ[i] + COLISIONES_RIGIDEZ * increm_z - AMORTIGUAMIENTO * particles[i]->velZ;
      } else if (cz == data->nz - 1) {
        accelerationZ[i] =
            accelerationZ[i] - COLISIONES_RIGIDEZ * increm_z + AMORTIGUAMIENTO * particles[i]->velZ;
      }
    }
  }
}

// Debe actualizar el movimiento de cada particula
void Block::particleMotion() {
  for (size_t i = 0; i < particles.size(); i++) {
    particles[i]->posX = particles[i]->posX + particles[i]->smoothVecX * PASO_TIEMPO +
                         accelerationX[i] * pow(PASO_TIEMPO, 2);
    particles[i]->posY = particles[i]->posY + particles[i]->smoothVecY * PASO_TIEMPO +
                         accelerationY[i] * pow(PASO_TIEMPO, 2);
    particles[i]->posZ = particles[i]->posZ + particles[i]->smoothVecZ * PASO_TIEMPO +
                         accelerationZ[i] * pow(PASO_TIEMPO, 2);
    particles[i]->velX       = particles[i]->smoothVecX + ((accelerationX[i] * PASO_TIEMPO) / 2);
    particles[i]->velY       = particles[i]->smoothVecY + ((accelerationY[i] * PASO_TIEMPO) / 2);
    particles[i]->velZ       = particles[i]->smoothVecZ + ((accelerationZ[i] * PASO_TIEMPO) / 2);
    particles[i]->smoothVecX = particles[i]->smoothVecX + accelerationX[i] * PASO_TIEMPO;
    particles[i]->smoothVecY = particles[i]->smoothVecY + accelerationY[i] * PASO_TIEMPO;
    particles[i]->smoothVecZ = particles[i]->smoothVecZ + accelerationZ[i] * PASO_TIEMPO;
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje x
void Block::interactionsX(unsigned int cx) {
  double dx = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    if (cx == 0) {
      dx = particles[i]->posX - LIMITE_INFERIOR_RECINTO_X;
    } else if (cx == data->nx - 1) {
      dx = LIMITE_SUPERIOR_RECINTO_X - particles[i]->posX;
    }
    if (dx < 0) {
      if (cx == 0) {
        particles[i]->posX = LIMITE_INFERIOR_RECINTO_X - dx;
      } else if (cx == data->nx - 1) {
        particles[i]->posX = LIMITE_SUPERIOR_RECINTO_X + dx;
      }
      particles[i]->velX       = -particles[i]->velX;
      particles[i]->smoothVecX = -particles[i]->smoothVecX;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje y
void Block::interactionsY(unsigned int cy) {
  double dy = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    if (cy == 0) {
      dy = particles[i]->posY - LIMITE_INFERIOR_RECINTO_Y;
    } else if (cy == data->ny - 1) {
      dy = LIMITE_SUPERIOR_RECINTO_Y - particles[i]->posY;
    }
    if (dy < 0) {
      if (cy == 0) {
        particles[i]->posY = LIMITE_INFERIOR_RECINTO_Y - dy;
      } else if (cy == data->ny - 1) {
        particles[i]->posY = LIMITE_SUPERIOR_RECINTO_Y + dy;
      }
      particles[i]->velY       = -particles[i]->velY;
      particles[i]->smoothVecY = -particles[i]->smoothVecY;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje z
void Block::interactionsZ(unsigned int cz) {
  double dz = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    if (cz == 0) {
      dz = particles[i]->posZ - LIMITE_INFERIOR_RECINTO_Z;
    } else if (cz == data->nz - 1) {
      dz = LIMITE_SUPERIOR_RECINTO_Z - particles[i]->posZ;
    }
    if (dz < 0) {
      if (cz == 0) {
        particles[i]->posZ = LIMITE_INFERIOR_RECINTO_Z - dz;
      } else if (cz == data->nz - 1) {
        particles[i]->posZ = LIMITE_SUPERIOR_RECINTO_Z + dz;
      }
      particles[i]->velZ       = -particles[i]->velZ;
      particles[i]->smoothVecZ = -particles[i]->smoothVecZ;
    }
  }
}
