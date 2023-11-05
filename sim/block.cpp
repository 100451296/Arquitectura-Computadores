#include "block.hpp"

#include <cmath>
#include <iostream>
#include <numbers>

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
vector<std::pair<ParticleRef const, ParticleRef const>>
    Block::generarParejasEntreBloques(Block & otherBlock) {
  // Generar pares entre el bloque actual y el bloque contiguo
  vector<std::pair<ParticleRef const, ParticleRef const>> aux;
  for (size_t i = 0; i < particles.size(); ++i) {
    for (size_t j = 0; j < otherBlock.particles.size(); ++j) {
      aux.push_back(make_pair(particles[i], otherBlock.particles[j]));
    }
  }
  return aux;
}

// Función para añadir una partícula al vector
void Block::addParticle(ParticleRef particle) {
  this->particles.push_back(particle);
  this->accelerationX.push_back(0.0);  // Agrega una entrada 0 a accelerationX
  this->accelerationY.push_back(0.0);  // Agrega una entrada 0 a accelerationY
  this->accelerationZ.push_back(0.0);  // Agrega una entrada 0 a accelerationZ
  this->density.push_back(0.0);        // Agrega una entrada 0 a density
}

// Funcion encargada de calcular la masa y longitud de suavizado de todas las particulas de un
// bloque
void Block::calculateDataCommon() {
  data.mass           = DENSIDAD_FLUIDO / pow(data.ppm, 3);
  data.long_suavizado = MULTIPLICADOR_RADIO / data.ppm;
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
void Block::densityIncrease(Block & contiguousBlock) {
  // Parte del bloque actual
  calculate_increm_density(this->particlePairs);
  // Parte del bloque contiguo
  vector<std::pair<ParticleRef const, ParticleRef const>> aux =
      generarParejasEntreBloques(contiguousBlock);
  calculate_increm_density(aux);
  lineal_transformate_density();
}

// Metodo auxiliar que realiza los diferentes calculos para el incremento de densidad
void Block::calculate_increm_density(
    vector<std::pair<ParticleRef const, ParticleRef const>> ParejaParticulas) {
  double aux_x, aux_y, aux_z, increm_density_pair;
  for (auto const & pair : ParejaParticulas) {
    aux_x = pow(pair.first.p1.posX - pair.second.p1.posX, 2);
    aux_y = pow(pair.first.p1.posY - pair.second.p1.posY, 2);
    aux_z = pow(pair.first.p1.posZ - pair.second.p1.posZ, 2);
    if (aux_x + aux_y + aux_z < pow(data.long_suavizado, 2)) {
      increm_density_pair = pow(pow(data.long_suavizado, 2) - (aux_x + aux_y + aux_z), 3);
    } else {
      increm_density_pair = 0;
    }
    density[pair.first.p1.id]  = density[pair.first.p1.id] + increm_density_pair;
    density[pair.second.p1.id] = density[pair.second.p1.id] + increm_density_pair;
  }
}

// Metodo auxiliar que realiza los diferentes calculos para la transformacion lineal de la densidad
void Block::lineal_transformate_density() {
  for (size_t i = 0; i < density.size(); i++) {
    if (density[i] != 0) {
      density[i] = (density[i] + pow(data.long_suavizado, 6)) * 315 * data.mass /
                   (64 * numbers::pi * pow(data.long_suavizado, 9));
    }
  }
}

// Metodo auxiliar que realiza los diferentes calculos para el incremento de aceleracion
vector<double> Block::calculate_increm_aceleration(vector<double> position, vector<double> velocity,
                                                   double dist, vector<unsigned int> Id) {
  vector<double> increm_aceleration;
  increm_aceleration = {
    ((position[0]) * (15 / numbers::pi * pow(data.long_suavizado, 6)) *
         (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
         (density[Id[0]] + density[Id[1]] - 2 * DENSIDAD_FLUIDO) +
     (velocity[0]) * (45 / numbers::pi * pow(data.long_suavizado, 6)) * VISCOSIDAD * data.mass) /
        (density[Id[0]] * density[Id[1]]),
    ((position[1]) * (15 / numbers::pi * pow(data.long_suavizado, 6)) *
         (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
         (density[Id[0]] + density[Id[1]] - 2 * DENSIDAD_FLUIDO) +
     (velocity[1]) * (45 / numbers::pi * pow(data.long_suavizado, 6)) * VISCOSIDAD * data.mass) /
        (density[Id[0]] * density[Id[1]]),
    ((position[2]) * (15 / numbers::pi * pow(data.long_suavizado, 6)) *
         (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
         (density[Id[0]] + density[Id[1]] - 2 * DENSIDAD_FLUIDO) +
     (velocity[2]) * (45 / numbers::pi * pow(data.long_suavizado, 6)) * VISCOSIDAD * data.mass) /
        (density[Id[0]] * density[Id[1]])};
  return increm_aceleration;
}

// Metodo auxiliar que realiza los diferentes calculos para la distancia
double Block::calculate_dist(double posX, double posY, double posZ) {
  double dist = sqrt(max(pow(posX, 2) + pow(posY, 2) + pow(posZ, 2), pow(10, -12)));
  return dist;
}

// Metodo auxiliar que realiza los diferentes calculos para la aceleracion
void Block::accelerationTransferCalculations(
    vector<std::pair<ParticleRef const, ParticleRef const>> pair_vec) {
  for (auto const & pair : pair_vec) {
    if (pow(pair.first.p1.posX - pair.second.p1.posX, 2) +
            pow(pair.first.p1.posY - pair.second.p1.posY, 2) +
            pow(pair.first.p1.posZ - pair.second.p1.posZ, 2) <
        pow(data.long_suavizado, 2)) {
      double dist             = calculate_dist(pair.first.p1.posX - pair.second.p1.posX,
                                               pair.first.p1.posY - pair.second.p1.posY,
                                               pair.first.p1.posZ - pair.second.p1.posZ);
      vector<double> position = {pair.first.p1.posX - pair.second.p1.posX,
                                 pair.first.p1.posY - pair.second.p1.posY,
                                 pair.first.p1.posZ - pair.second.p1.posZ};
      vector<double> velocity = {pair.first.p1.velX - pair.second.p1.velX,
                                 pair.first.p1.velY - pair.second.p1.velY,
                                 pair.first.p1.velZ - pair.second.p1.velZ};
      vector<unsigned int> Id = {pair.first.p1.id, pair.second.p1.id};
      vector<double> increm_aceleration =
          calculate_increm_aceleration(position, velocity, dist, Id);
      accelerationX[pair.first.p1.id]  = accelerationX[pair.first.p1.id] + increm_aceleration[0];
      accelerationY[pair.first.p1.id]  = accelerationY[pair.first.p1.id] + increm_aceleration[1];
      accelerationZ[pair.first.p1.id]  = accelerationZ[pair.first.p1.id] + increm_aceleration[2];
      accelerationX[pair.second.p1.id] = accelerationX[pair.second.p1.id] + increm_aceleration[0];
      accelerationY[pair.second.p1.id] = accelerationY[pair.second.p1.id] + increm_aceleration[1];
      accelerationZ[pair.second.p1.id] = accelerationZ[pair.second.p1.id] + increm_aceleration[2];
    }
  }
}

// Funcion que se encarga de actualizar el vector de aceleraciones y el incremento
void Block::accelerationTransfer(Block & contiguousBlock) {
  // Parte del bloque actual
  accelerationTransferCalculations(particlePairs);
  // Parte del bloque contiguo
  vector<std::pair<ParticleRef const, ParticleRef const>> aux =
      generarParejasEntreBloques(contiguousBlock);
  accelerationTransferCalculations(aux);
}

// Debe actualizar la componente x del vector de aceleración para cada particula
void Block::collisionsX(unsigned int cx) {
  double cord_x, increm_x = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    cord_x = particles[i].p1.posX + particles[i].p1.smoothVecX * PASO_TIEMPO;
    if (cx == 0) {
      increm_x = TAMANO_PARTICULA - (cord_x - LIMITE_INFERIOR_RECINTO_X);
    } else if (cx == data.nx - 1) {
      increm_x = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_X - cord_x);
    }
    if (increm_x > pow(10, -10)) {
      if (cx == 0) {
        accelerationX[i] = accelerationX[i] + COLISIONES_RIGIDEZ * increm_x -
                           AMORTIGUAMIENTO * particles[i].p1.velX;
      } else if (cx == data.nx - 1) {
        accelerationX[i] = accelerationX[i] - COLISIONES_RIGIDEZ * increm_x +
                           AMORTIGUAMIENTO * particles[i].p1.velX;
      }
    }
  }
}

// Debe actualizar la componente y del vector de aceleración para cada particula
void Block::collisionsY(unsigned int cy) {
  double cord_y, increm_y = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    cord_y = particles[i].p1.posY + particles[i].p1.smoothVecY * PASO_TIEMPO;
    if (cy == 0) {
      increm_y = TAMANO_PARTICULA - (cord_y - LIMITE_INFERIOR_RECINTO_Y);
    } else if (cy == data.ny - 1) {
      increm_y = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Y - cord_y);
    }
    if (increm_y > pow(10, -10)) {
      if (cy == 0) {
        accelerationY[i] = accelerationY[i] + COLISIONES_RIGIDEZ * increm_y -
                           AMORTIGUAMIENTO * particles[i].p1.velY;
      } else if (cy == data.ny - 1) {
        accelerationY[i] = accelerationY[i] - COLISIONES_RIGIDEZ * increm_y +
                           AMORTIGUAMIENTO * particles[i].p1.velY;
      }
    }
  }
}

// Debe actualizar la componente z del vector de aceleración para cada particula
void Block::collisionsZ(unsigned int cz) {
  double cord_z, increm_z = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    cord_z = particles[i].p1.posZ + particles[i].p1.smoothVecZ * PASO_TIEMPO;
    if (cz == 0) {
      increm_z = TAMANO_PARTICULA - (cord_z - LIMITE_INFERIOR_RECINTO_Z);
    } else if (cz == data.nz - 1) {
      increm_z = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Z - cord_z);
    }
    if (increm_z > pow(10, -10)) {
      if (cz == 0) {
        accelerationZ[i] = accelerationZ[i] + COLISIONES_RIGIDEZ * increm_z -
                           AMORTIGUAMIENTO * particles[i].p1.velZ;
      } else if (cz == data.nz - 1) {
        accelerationZ[i] = accelerationZ[i] - COLISIONES_RIGIDEZ * increm_z +
                           AMORTIGUAMIENTO * particles[i].p1.velZ;
      }
    }
  }
}

// Debe actualizar el movimiento de cada particula
void Block::particleMotion() {
  for (size_t i = 0; i < particles.size(); i++) {
    particles[i].p1.posX = particles[i].p1.posX + particles[i].p1.smoothVecX * PASO_TIEMPO +
                           accelerationX[i] * pow(PASO_TIEMPO, 2);
    particles[i].p1.posY = particles[i].p1.posY + particles[i].p1.smoothVecY * PASO_TIEMPO +
                           accelerationY[i] * pow(PASO_TIEMPO, 2);
    particles[i].p1.posZ = particles[i].p1.posZ + particles[i].p1.smoothVecZ * PASO_TIEMPO +
                           accelerationZ[i] * pow(PASO_TIEMPO, 2);
    particles[i].p1.velX = particles[i].p1.smoothVecX + ((accelerationX[i] * PASO_TIEMPO) / 2);
    particles[i].p1.velY = particles[i].p1.smoothVecY + ((accelerationY[i] * PASO_TIEMPO) / 2);
    particles[i].p1.velZ = particles[i].p1.smoothVecZ + ((accelerationZ[i] * PASO_TIEMPO) / 2);
    particles[i].p1.smoothVecX = particles[i].p1.smoothVecX + accelerationX[i] * PASO_TIEMPO;
    particles[i].p1.smoothVecY = particles[i].p1.smoothVecY + accelerationY[i] * PASO_TIEMPO;
    particles[i].p1.smoothVecZ = particles[i].p1.smoothVecZ + accelerationZ[i] * PASO_TIEMPO;
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje x
void Block::interactionsX(unsigned int cx) {
  double dx = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    if (cx == 0) {
      dx = particles[i].p1.posX - LIMITE_INFERIOR_RECINTO_X;
    } else if (cx == data.nx - 1) {
      dx = LIMITE_SUPERIOR_RECINTO_X - particles[i].p1.posX;
    }
    if (dx < 0) {
      if (cx == 0) {
        particles[i].p1.posX = LIMITE_INFERIOR_RECINTO_X - dx;
      } else if (cx == data.nx - 1) {
        particles[i].p1.posX = LIMITE_SUPERIOR_RECINTO_X + dx;
      }
      particles[i].p1.velX       = particles[i].p1.velX * -1;
      particles[i].p1.smoothVecX = particles[i].p1.smoothVecX * -1;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje y
void Block::interactionsY(unsigned int cy) {
  double dy = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    if (cy == 0) {
      dy = particles[i].p1.posY - LIMITE_INFERIOR_RECINTO_Y;
    } else if (cy == data.ny - 1) {
      dy = LIMITE_SUPERIOR_RECINTO_Y - particles[i].p1.posY;
    }
    if (dy < 0) {
      if (cy == 0) {
        particles[i].p1.posY = LIMITE_INFERIOR_RECINTO_Y - dy;
      } else if (cy == data.ny - 1) {
        particles[i].p1.posY = LIMITE_SUPERIOR_RECINTO_Y + dy;
      }
      particles[i].p1.velY       = particles[i].p1.velY * -1;
      particles[i].p1.smoothVecY = particles[i].p1.smoothVecY * -1;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje z
void Block::interactionsZ(unsigned int cz) {
  double dz = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    if (cz == 0) {
      dz = particles[i].p1.posZ - LIMITE_INFERIOR_RECINTO_Z;
    } else if (cz == data.nz - 1) {
      dz = LIMITE_SUPERIOR_RECINTO_Z - particles[i].p1.posZ;
    }
    if (dz < 0) {
      if (cz == 0) {
        particles[i].p1.posZ = LIMITE_INFERIOR_RECINTO_Z - dz;
      } else if (cz == data.nz - 1) {
        particles[i].p1.posZ = LIMITE_SUPERIOR_RECINTO_Z + dz;
      }
      particles[i].p1.velZ       = particles[i].p1.velZ * -1;
      particles[i].p1.smoothVecZ = particles[i].p1.smoothVecZ * -1;
    }
  }
}

// Metodo que devuelve el valor maximo
double Block::max(double n1, double n2) {
  if (n1 > n2) {
    return n1;
  } else {
    return n2;
  }
}
