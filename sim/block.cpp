#include "block.hpp"

#include <cmath>
#include <iostream>
#include <numbers>

using namespace std;

// METODOS de BLOCK

// Crear parejas de un mismo bloque
void Block::generarParejasBloque() {
  // Generar pares dentro del bloque actual
  for (size_t i = 0; i < particles.size(); ++i) {
    for (size_t j = i + 1; j < particles.size(); ++j) {
      particlePairs.push_back(make_pair(particles[i], particles[j]));
    }
  }
}

// Crear parejas de dos bloques contiguos
vector<std::pair<Particle, Particle>> Block::generarParejasEntreBloques(Block & otherBlock) {
  // Generar pares entre el bloque actual y el bloque contiguo
  vector<std::pair<Particle, Particle>> aux;
  for (size_t i = 0; i < particles.size(); ++i) {
    for (size_t j = 0; j < otherBlock.particles.size(); ++j) {
      aux.push_back(make_pair(particles[i], otherBlock.particles[j]));
    }
  }
  return aux;
};

/*void Block::initVectors() {
  fill(this->accelerations.begin(), this->accelerations.end(), 0);
  fill(this->densities.begin(), this->densities.end(), 0);
}*/

/*// Función para añadir una partícula al vector
void Block::addParticle(Particle &particle) {
  this->particles.push_back(particle);
  this->accelerations.push_back(0.0);  //Agrega una entrada 0 a accelerations
  this->densities.push_back(0.0);      //Agrega una entrada 0 a densities
}*/

// Funcion encargada de calcular la masa y longitud de suavizado de todas las particulas de un
// bloque
void Block::calculateDataCommon() {
  data.mass           = DENSIDAD_FLUIDO / pow(data.ppm, 3);
  data.long_suavizado = MULTIPLICADOR_RADIO / data.ppm;
}

// Funcion inicializar la densidad y la aceleracion para cada particula
void Block::initDensityAcceleration() {
  for (size_t i = 0; i < acelerationX.size(); ++i) {
    density[i]      = 0;
    acelerationX[i] = ACELERACION_GRAVEDAD_X;
    acelerationY[i] = ACELERACION_GRAVEDAD_Y;
    acelerationZ[i] = ACELERACION_GRAVEDAD_Z;
  }
}

void Block::densityIncrease(Block & contiguousBlock) {
  // Funcion encargada de modificar el vector de densidades del propio bloque y transformacion
  // lineal Parte del bloque actual
  double aux_x, aux_y, aux_z, increm_density_pair;
  for (auto const & pair : particlePairs) {
    aux_x = pow(pair.first.posX - pair.second.posX, 2);
    aux_y = pow(pair.first.posY - pair.second.posY, 2);
    aux_z = pow(pair.first.posZ - pair.second.posZ, 2);
    if (aux_x + aux_y + aux_z < pow(data.long_suavizado, 2)) {
      increm_density_pair = pow(pow(data.long_suavizado, 2) - (aux_x + aux_y + aux_z), 3);
    } else {
      increm_density_pair = 0;
    }
    density[pair.first.id]  = density[pair.first.id] + increm_density_pair;
    density[pair.second.id] = density[pair.second.id] + increm_density_pair;
  }
  // Parte del bloque contiguo
  vector<std::pair<Particle, Particle>> aux = generarParejasEntreBloques(contiguousBlock);
  for (auto const & pair : aux) {
    aux_x = pow(pair.first.posX - pair.second.posX, 2);
    aux_y = pow(pair.first.posY - pair.second.posY, 2);
    aux_z = pow(pair.first.posZ - pair.second.posZ, 2);
    if (aux_x + aux_y + aux_z < pow(data.long_suavizado, 2)) {
      increm_density_pair = pow(pow(data.long_suavizado, 2) - (aux_x + aux_y + aux_z), 3);
    } else {
      increm_density_pair = 0;
    }
    density[pair.first.id]  = density[pair.first.id] + increm_density_pair;
    density[pair.second.id] = density[pair.second.id] + increm_density_pair;
  }
  lineal_transformate_density();
}

void Block::lineal_transformate_density() {
  for (size_t i = 0; i < density.size(); i++) {
    density[i] = (density[i] + pow(data.long_suavizado, 6)) * 315 * data.mass /
                 (64 * numbers::pi * pow(data.long_suavizado, 9));
  }
}

void Block::accelerationTransfer(Block & contiguousBlock) {
  // Funcion que se encarga de actualizar el vector de aceleraciones del propio bloque y debe
  // calcular la distancia y el incremento Parte del bloque actual
  for (auto const & pair : particlePairs) {
    if (pow(pair.first.posX - pair.second.posX, 2) + pow(pair.first.posY - pair.second.posY, 2) +
            pow(pair.first.posZ - pair.second.posZ, 2) <
        pow(data.long_suavizado, 2)) {
      double dist                       = sqrt(max(pow(pair.first.posX - pair.second.posX, 2) +
                                                       pow(pair.first.posY - pair.second.posY, 2) +
                                                       pow(pair.first.posZ - pair.second.posZ, 2),
                                                   pow(10, -12)));
      vector<double> increm_aceleration = {
        ((pair.first.posX - pair.second.posX) * (15 / numbers::pi * pow(data.long_suavizado, 6)) *
             (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
             (density[pair.first.id] + density[pair.second.id] - 2 * DENSIDAD_FLUIDO) +
         (pair.first.velX - pair.second.velX) * (45 / numbers::pi * pow(data.long_suavizado, 6)) *
             VISCOSIDAD * data.mass) /
            (density[pair.first.id] * density[pair.second.id]),
        ((pair.first.posY - pair.second.posY) * (15 / numbers::pi * pow(data.long_suavizado, 6)) *
             (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
             (density[pair.first.id] + density[pair.second.id] - 2 * DENSIDAD_FLUIDO) +
         (pair.first.velY - pair.second.velY) * (45 / numbers::pi * pow(data.long_suavizado, 6)) *
             VISCOSIDAD * data.mass) /
            (density[pair.first.id] * density[pair.second.id]),
        ((pair.first.posZ - pair.second.posZ) * (15 / numbers::pi * pow(data.long_suavizado, 6)) *
             (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
             (density[pair.first.id] + density[pair.second.id] - 2 * DENSIDAD_FLUIDO) +
         (pair.first.velZ - pair.second.velZ) * (45 / numbers::pi * pow(data.long_suavizado, 6)) *
             VISCOSIDAD * data.mass) /
            (density[pair.first.id] * density[pair.second.id])};
      acelerationX[pair.first.id]  = acelerationX[pair.first.id] + increm_aceleration[0];
      acelerationY[pair.first.id]  = acelerationY[pair.first.id] + increm_aceleration[1];
      acelerationZ[pair.first.id]  = acelerationZ[pair.first.id] + increm_aceleration[2];
      acelerationX[pair.second.id] = acelerationX[pair.second.id] + increm_aceleration[0];
      acelerationY[pair.second.id] = acelerationY[pair.second.id] + increm_aceleration[1];
      acelerationZ[pair.second.id] = acelerationZ[pair.second.id] + increm_aceleration[2];
    }
  }
  // Parte del bloque contiguo
  vector<std::pair<Particle, Particle>> aux = generarParejasEntreBloques(contiguousBlock);
  for (auto const & pair : aux) {
    if (pow(pair.first.posX - pair.second.posX, 2) + pow(pair.first.posY - pair.second.posY, 2) +
            pow(pair.first.posZ - pair.second.posZ, 2) <
        pow(data.long_suavizado, 2)) {
      double dist                       = sqrt(max(pow(pair.first.posX - pair.second.posX, 2) +
                                                       pow(pair.first.posY - pair.second.posY, 2) +
                                                       pow(pair.first.posZ - pair.second.posZ, 2),
                                                   pow(10, -12)));
      vector<double> increm_aceleration = {
        ((pair.first.posX - pair.second.posX) * (15 / numbers::pi * pow(data.long_suavizado, 6)) *
             (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
             (density[pair.first.id] + density[pair.second.id] - 2 * DENSIDAD_FLUIDO) +
         (pair.first.velX - pair.second.velX) * (45 / numbers::pi * pow(data.long_suavizado, 6)) *
             VISCOSIDAD * data.mass) /
            (density[pair.first.id] * density[pair.second.id]),
        ((pair.first.posY - pair.second.posY) * (15 / numbers::pi * pow(data.long_suavizado, 6)) *
             (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
             (density[pair.first.id] + density[pair.second.id] - 2 * DENSIDAD_FLUIDO) +
         (pair.first.velY - pair.second.velY) * (45 / numbers::pi * pow(data.long_suavizado, 6)) *
             VISCOSIDAD * data.mass) /
            (density[pair.first.id] * density[pair.second.id]),
        ((pair.first.posZ - pair.second.posZ) * (15 / numbers::pi * pow(data.long_suavizado, 6)) *
             (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
             (density[pair.first.id] + density[pair.second.id] - 2 * DENSIDAD_FLUIDO) +
         (pair.first.velZ - pair.second.velZ) * (45 / numbers::pi * pow(data.long_suavizado, 6)) *
             VISCOSIDAD * data.mass) /
            (density[pair.first.id] * density[pair.second.id])};
      acelerationX[pair.first.id]  = acelerationX[pair.first.id] + increm_aceleration[0];
      acelerationY[pair.first.id]  = acelerationY[pair.first.id] + increm_aceleration[1];
      acelerationZ[pair.first.id]  = acelerationZ[pair.first.id] + increm_aceleration[2];
      acelerationX[pair.second.id] = acelerationX[pair.second.id] + increm_aceleration[0];
      acelerationY[pair.second.id] = acelerationY[pair.second.id] + increm_aceleration[1];
      acelerationZ[pair.second.id] = acelerationZ[pair.second.id] + increm_aceleration[2];
    }
  }
}

void Block::collisionsX(unsigned int cx) {
  // Debe actualizar la componente x del vector de aceleración para cada
  // particula del bloque

  cout << "Colisiones en la dirección X con valor de cx: " << cx << endl;
  // Tu código aquí
}

void Block::collisionsY(unsigned int cy) {
  // Debe actualizar la componente y del vector de aceleración para cada
  // particula del bloque

  cout << "Colisiones en la dirección Y con valor de cy: " << cy << endl;
  // Tu código aquí
}

void Block::collisionsZ(unsigned int cx) {
  // Debe actualizar la componente z del vector de aceleración para cada
  // particula del bloque

  cout << "Colisiones en la dirección Z con valor de cx: " << cx << endl;
  // Tu código aquí
}

void Block::particleMotion() {
  // Debe actualizar la posicion, velocidad y vector de suavizado de cada
  // particula

  cout << "Movimiento de partículas" << endl;
  // Tu código aquí
}

void Block::interactionsX(unsigned int cx) {
  // Debe actualizar la posicion, velocidad y vector de suavizado de cada
  // particula, cuando dx sea negativo

  cout << "Interacciones en la dirección X con valor de cx: " << cx << endl;
  // Tu código aquí
}

void Block::interactionsY(unsigned int cy) {
  // Debe actualizar la posicion, velocidad y vector de suavizado de cada
  // particula, cuando dy sea negativo

  cout << "Interacciones en la dirección Y con valor de cy: " << cy << endl;
  // Tu código aquí
}

void Block::interactionsZ(unsigned int cz) {
  // Debe actualizar la posicion, velocidad y vector de suavizado de cada
  // particula, cuando dz sea negativo

  cout << "Interacciones en la dirección Z con valor de cz: " << cz << endl;
  // Tu código aquí
}

double Block::max(double n1, double n2) {
  if (n1 > n2) {
    return n1;
  } else {
    return n2;
  }
}
