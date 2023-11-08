#include "block.hpp"

using namespace std;

// METODOS de BLOCK

// Metodo encargado de crear las parejas de particulas de un mismo bloque
void Block::generarParejasBloque() {
  for (size_t i = 0; i < particles.size(); i++) {
    for (size_t j = i + 1; j < particles.size(); j++)
    {
      particlePairs.push_back(make_pair(i, j));
    }
  }
}

// Metodo encargado de crear las parejas de particulas entre bloques contiguos
vector<std::pair<int, int>> Block::generarParejasEntreBloques(Block & otherBlock) {
  // Generar pares entre el bloque actual y el bloque contiguo
  vector<std::pair<int, int>> aux;
  for (size_t i = 0; i < this->particles.size(); i++) {
    for (size_t j = 0; j < otherBlock.particles.size(); j++) { aux.push_back(make_pair(i, j)); }
  }
  return aux;
}

// Función para añadir una partícula al vector
void Block::addParticle(Particle & particle) {
  this->particles.push_back(std::ref(particle));
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
void Block::densityIncreaseSingle() {
  generarParejasBloque();
  //calculate_increm_density(particlePairs,);
  lineal_transformate_density();
}

void Block::densityIncrease(Block & contiguousBlock) {
  // Parte del bloque contiguo
  vector<std::pair<int, int>> aux = generarParejasEntreBloques(contiguousBlock);
  calculate_increm_density(aux, contiguousBlock);
  lineal_transformate_density(contiguousBlock);
}

// Metodo auxiliar que realiza los diferentes calculos para el incremento de densidad
void Block::calculate_increm_density(vector<std::pair<int, int>> ParejaParticulas,
                                     Block & contiguousBlock) {
  double aux_x, aux_y, aux_z, increm_density_pair;
  for (auto const & pair :
       ParejaParticulas) { /*
cout << "Index1: " << pair.first << " Size1: " << particles.size() << " Index2: " << pair.second
<< " Size2: " << contiguousBlock.particles.size() << endl;
*/
    aux_x = pow(

        particles[pair.first].get().posX - contiguousBlock.particles[pair.second].get().posX, 2);
    aux_y = pow(
        particles[pair.first].get().posY - contiguousBlock.particles[pair.second].get().posY, 2);
    aux_z = pow(
        particles[pair.first].get().posZ - contiguousBlock.particles[pair.second].get().posZ, 2);
    if (aux_x + aux_y + aux_z < pow(data.long_suavizado, 2)) {
      increm_density_pair = pow(pow(data.long_suavizado, 2) - (aux_x + aux_y + aux_z), 3);
    } else {
      increm_density_pair = 0;
    }
    density[pair.first]  = density[pair.first] + increm_density_pair;
    density[pair.second] = density[pair.second] + increm_density_pair;
  }
}

// Metodo auxiliar que realiza los diferentes calculos para la transformacion lineal de la densidad
// en un mismo bloque
void Block::lineal_transformate_density() {
  for (size_t i = 0; i < density.size(); i++) {
    density[i] = (density[i] + pow(data.long_suavizado, 6)) * 315 * data.mass /
                 (64 * numbers::pi * pow(data.long_suavizado, 9));
  }
}

// Metodo auxiliar que realiza los diferentes calculos para la transformacion lineal de la densidad
// respecto a un bloque contiguo
void Block::lineal_transformate_density(Block & contiguousBlock) {
  for (size_t i = 0; i < density.size(); i++) {
    density[i] = (density[i] + pow(data.long_suavizado, 6)) * 315 * data.mass /
                 (64 * numbers::pi * pow(data.long_suavizado, 9));
  }
  for (size_t i = 0; i < contiguousBlock.density.size(); i++) {
    contiguousBlock.density[i] = (contiguousBlock.density[i] + pow(data.long_suavizado, 6)) * 315 *
                                 data.mass / (64 * numbers::pi * pow(data.long_suavizado, 9));
  }
}

// Metodo auxiliar que realiza los diferentes calculos para el incremento de aceleracion
vector<double> Block::calculate_increm_aceleration(vector<double> position, vector<double> velocity,
                                                   double dist, vector<unsigned int> Id) {
  vector<double> increm_aceleration;
  increm_aceleration = {
    ((position[0]) * (15 / (numbers::pi * pow(data.long_suavizado, 6))) *
         (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
         (density[Id[0]] + density[Id[1]] - 2 * DENSIDAD_FLUIDO) +
     (velocity[0]) * (45 / (numbers::pi * pow(data.long_suavizado, 6))) * VISCOSIDAD * data.mass) /
        (density[Id[0]] * density[Id[1]]),
    ((position[1]) * (15 / (numbers::pi * pow(data.long_suavizado, 6))) *
         (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
         (density[Id[0]] + density[Id[1]] - 2 * DENSIDAD_FLUIDO) +
     (velocity[1]) * (45 / (numbers::pi * pow(data.long_suavizado, 6))) * VISCOSIDAD * data.mass) /
        (density[Id[0]] * density[Id[1]]),
    ((position[2]) * (15 / (numbers::pi * pow(data.long_suavizado, 6))) *
         (3 * data.mass * PRESION_RIGIDEZ / 2) * ((pow(data.long_suavizado - dist, 2)) / dist) *
         (density[Id[0]] + density[Id[1]] - 2 * DENSIDAD_FLUIDO) +
     (velocity[2]) * (45 / (numbers::pi * pow(data.long_suavizado, 6))) * VISCOSIDAD * data.mass) /
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
  accelerationTransferCalculations(particlePairs, *this);
}

// Funcion que se encarga de actualizar el vector de aceleraciones y el incremento respecto a un
// bloque contiguo
void Block::accelerationTransfer(Block & contiguousBlock) {
  vector<std::pair<int, int>> aux = generarParejasEntreBloques(contiguousBlock);
  accelerationTransferCalculations(aux, contiguousBlock);
}

// Metodo auxiliar que realiza los diferentes calculos para la aceleracion
void Block::accelerationTransferCalculations(vector<std::pair<int, int>> pair_vec,
                                             Block & contiguousBlock) {
  for (auto const & pair : pair_vec) {
    cout << "Index1: " << pair.first << " Size1: " << particles.size()
         << " Index2: " << contiguousBlock.particles.size() << endl;
    if (pow(particles[pair.first].get().posX - contiguousBlock.particles[pair.second].get().posX,
            2) +
            pow(particles[pair.first].get().posY -
                    contiguousBlock.particles[pair.second].get().posY,
                2) +
            pow(particles[pair.first].get().posZ -
                    contiguousBlock.particles[pair.second].get().posZ,
                2) <
        pow(data.long_suavizado, 2)) {
      double dist = calculate_dist(
          particles[pair.first].get().posX - contiguousBlock.particles[pair.second].get().posX,
          particles[pair.first].get().posY - contiguousBlock.particles[pair.second].get().posY,
          particles[pair.first].get().posZ - contiguousBlock.particles[pair.second].get().posZ);
      vector<double> position = {
        particles[pair.first].get().posX - contiguousBlock.particles[pair.second].get().posX,
        particles[pair.first].get().posY - contiguousBlock.particles[pair.second].get().posY,
        particles[pair.first].get().posZ - contiguousBlock.particles[pair.second].get().posZ};
      vector<double> velocity = {
        particles[pair.first].get().velX - contiguousBlock.particles[pair.second].get().velX,
        particles[pair.first].get().velY - contiguousBlock.particles[pair.second].get().velY,
        particles[pair.first].get().velZ - contiguousBlock.particles[pair.second].get().velZ};
      vector<unsigned int> Id = {particles[pair.first].get().id,
                                 contiguousBlock.particles[pair.second].get().id};
      vector<double> increm_aceleration =
          calculate_increm_aceleration(position, velocity, dist, Id);
      cout << "id: " << particles[pair.first].get().id << "size: " << accelerationX.size() << endl;
      accelerationX[particles[pair.first].get().id] =
          accelerationX[particles[pair.first].get().id] + increm_aceleration[0];
      accelerationY[particles[pair.first].get().id] =
          accelerationY[particles[pair.first].get().id] + increm_aceleration[1];
      accelerationZ[particles[pair.first].get().id] =
          accelerationZ[particles[pair.first].get().id] + increm_aceleration[2];
      accelerationX[contiguousBlock.particles[pair.second].get().id] =
          accelerationX[contiguousBlock.particles[pair.second].get().id] + increm_aceleration[0];
      accelerationY[contiguousBlock.particles[pair.second].get().id] =
          accelerationY[contiguousBlock.particles[pair.second].get().id] + increm_aceleration[1];
      accelerationZ[contiguousBlock.particles[pair.second].get().id] =
          accelerationZ[contiguousBlock.particles[pair.second].get().id] + increm_aceleration[2];
    }
  }
}

// Debe actualizar la componente x del vector de aceleración para cada particula
void Block::collisionsX(unsigned int cx) {
  double cord_x, increm_x = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    cord_x = particles[i].get().posX + particles[i].get().smoothVecX * PASO_TIEMPO;
    if (cx == 0) {
      increm_x = TAMANO_PARTICULA - (cord_x - LIMITE_INFERIOR_RECINTO_X);
    } else if (cx == data.nx - 1) {
      increm_x = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_X - cord_x);
    }
    if (increm_x > pow(10, -10)) {
      if (cx == 0) {
        accelerationX[i] = accelerationX[i] + COLISIONES_RIGIDEZ * increm_x -
                           AMORTIGUAMIENTO * particles[i].get().velX;
      } else if (cx == data.nx - 1) {
        accelerationX[i] = accelerationX[i] - COLISIONES_RIGIDEZ * increm_x +
                           AMORTIGUAMIENTO * particles[i].get().velX;
      }
    }
  }
}

// Debe actualizar la componente y del vector de aceleración para cada particula
void Block::collisionsY(unsigned int cy) {
  double cord_y, increm_y = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    cord_y = particles[i].get().posY + particles[i].get().smoothVecY * PASO_TIEMPO;
    if (cy == 0) {
      increm_y = TAMANO_PARTICULA - (cord_y - LIMITE_INFERIOR_RECINTO_Y);
    } else if (cy == data.ny - 1) {
      increm_y = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Y - cord_y);
    }
    if (increm_y > pow(10, -10)) {
      if (cy == 0) {
        accelerationY[i] = accelerationY[i] + COLISIONES_RIGIDEZ * increm_y -
                           AMORTIGUAMIENTO * particles[i].get().velY;
      } else if (cy == data.ny - 1) {
        accelerationY[i] = accelerationY[i] - COLISIONES_RIGIDEZ * increm_y +
                           AMORTIGUAMIENTO * particles[i].get().velY;
      }
    }
  }
}

// Debe actualizar la componente z del vector de aceleración para cada particula
void Block::collisionsZ(unsigned int cz) {
  double cord_z, increm_z = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    cord_z = particles[i].get().posZ + particles[i].get().smoothVecZ * PASO_TIEMPO;
    if (cz == 0) {
      increm_z = TAMANO_PARTICULA - (cord_z - LIMITE_INFERIOR_RECINTO_Z);
    } else if (cz == data.nz - 1) {
      increm_z = TAMANO_PARTICULA - (LIMITE_SUPERIOR_RECINTO_Z - cord_z);
    }
    if (increm_z > pow(10, -10)) {
      if (cz == 0) {
        accelerationZ[i] = accelerationZ[i] + COLISIONES_RIGIDEZ * increm_z -
                           AMORTIGUAMIENTO * particles[i].get().velZ;
      } else if (cz == data.nz - 1) {
        accelerationZ[i] = accelerationZ[i] - COLISIONES_RIGIDEZ * increm_z +
                           AMORTIGUAMIENTO * particles[i].get().velZ;
      }
    }
  }
}

// Debe actualizar el movimiento de cada particula
void Block::particleMotion() {
  for (size_t i = 0; i < particles.size(); i++) {
    particles[i].get().posX = particles[i].get().posX +
                              particles[i].get().smoothVecX * PASO_TIEMPO +
                              accelerationX[i] * pow(PASO_TIEMPO, 2);
    particles[i].get().posY = particles[i].get().posY +
                              particles[i].get().smoothVecY * PASO_TIEMPO +
                              accelerationY[i] * pow(PASO_TIEMPO, 2);
    particles[i].get().posZ = particles[i].get().posZ +
                              particles[i].get().smoothVecZ * PASO_TIEMPO +
                              accelerationZ[i] * pow(PASO_TIEMPO, 2);
    particles[i].get().velX =
        particles[i].get().smoothVecX + ((accelerationX[i] * PASO_TIEMPO) / 2);
    particles[i].get().velY =
        particles[i].get().smoothVecY + ((accelerationY[i] * PASO_TIEMPO) / 2);
    particles[i].get().velZ =
        particles[i].get().smoothVecZ + ((accelerationZ[i] * PASO_TIEMPO) / 2);
    particles[i].get().smoothVecX = particles[i].get().smoothVecX + accelerationX[i] * PASO_TIEMPO;
    particles[i].get().smoothVecY = particles[i].get().smoothVecY + accelerationY[i] * PASO_TIEMPO;
    particles[i].get().smoothVecZ = particles[i].get().smoothVecZ + accelerationZ[i] * PASO_TIEMPO;
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje x
void Block::interactionsX(unsigned int cx) {
  double dx = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    if (cx == 0) {
      dx = particles[i].get().posX - LIMITE_INFERIOR_RECINTO_X;
    } else if (cx == data.nx - 1) {
      dx = LIMITE_SUPERIOR_RECINTO_X - particles[i].get().posX;
    }
    if (dx < 0) {
      if (cx == 0) {
        particles[i].get().posX = LIMITE_INFERIOR_RECINTO_X - dx;
      } else if (cx == data.nx - 1) {
        particles[i].get().posX = LIMITE_SUPERIOR_RECINTO_X + dx;
      }
      particles[i].get().velX       = -particles[i].get().velX;
      particles[i].get().smoothVecX = -particles[i].get().smoothVecX;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje y
void Block::interactionsY(unsigned int cy) {
  double dy = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    if (cy == 0) {
      dy = particles[i].get().posY - LIMITE_INFERIOR_RECINTO_Y;
    } else if (cy == data.ny - 1) {
      dy = LIMITE_SUPERIOR_RECINTO_Y - particles[i].get().posY;
    }
    if (dy < 0) {
      if (cy == 0) {
        particles[i].get().posY = LIMITE_INFERIOR_RECINTO_Y - dy;
      } else if (cy == data.ny - 1) {
        particles[i].get().posY = LIMITE_SUPERIOR_RECINTO_Y + dy;
      }
      particles[i].get().velY       = -particles[i].get().velY;
      particles[i].get().smoothVecY = -particles[i].get().smoothVecY;
    }
  }
}

// Debe actualizar la posicion, velocidad y vector de suavizado de cada particula eje z
void Block::interactionsZ(unsigned int cz) {
  double dz = 0;
  for (size_t i = 0; i < particles.size(); ++i) {
    if (cz == 0) {
      dz = particles[i].get().posZ - LIMITE_INFERIOR_RECINTO_Z;
    } else if (cz == data.nz - 1) {
      dz = LIMITE_SUPERIOR_RECINTO_Z - particles[i].get().posZ;
    }
    if (dz < 0) {
      if (cz == 0) {
        particles[i].get().posZ = LIMITE_INFERIOR_RECINTO_Z - dz;
      } else if (cz == data.nz - 1) {
        particles[i].get().posZ = LIMITE_SUPERIOR_RECINTO_Z + dz;
      }
      particles[i].get().velZ       = -particles[i].get().velZ;
      particles[i].get().smoothVecZ = -particles[i].get().smoothVecZ;
    }
  }
}
