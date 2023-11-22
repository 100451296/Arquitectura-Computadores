#include "gridTest.hpp"

std::tuple<int, int, int> GridTest::obtenerIndicesDesdeId(int identificador) const {
  int const x_index = identificador % nx;
  int const y_index = (identificador / nx) % ny;
  int const z_index = identificador / (nx * ny);

  return std::make_tuple(x_index, y_index, z_index);
}

void GridTest::readTraze(std::string const & input_file_name) {
  std::ifstream file(input_file_name);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file: " << input_file_name << "\n";
    return;
  }

  int const numBlocks = readHeader(file);

  particles.resize(NUM_PARTICLES);
  density.resize(NUM_PARTICLES, 0.0);
  accelerationX.resize(NUM_PARTICLES, ACELERACION_GRAVEDAD_X);
  accelerationY.resize(NUM_PARTICLES, ACELERACION_GRAVEDAD_Y);
  accelerationZ.resize(NUM_PARTICLES, ACELERACION_GRAVEDAD_Z);

  initializeGridAndBlocks();
  readParticlesInfo(file, numBlocks);

  file.close();
}

int GridTest::readHeader(std::ifstream & file) {
  int numBlocks = 0;
  // NOLINTNEXTLINE
  file.read(reinterpret_cast<char *>(&numBlocks), sizeof(int));
  return numBlocks;
}

void GridTest::initializeGridAndBlocks() {
  nx = NX;
  ny = NY;
  nz = NZ;
  initializeBlockVectors();
}

void GridTest::readParticlesInfo(std::ifstream & file, int numBlocks) {
  for (int blockIndex = 0; blockIndex < numBlocks; ++blockIndex) {
    readBlockInfo(file, blockIndex);
  }
}

void GridTest::readBlockInfo(std::ifstream & file, int blockIndex) {
  int64_t numParticles = 0;
  // NOLINTNEXTLINE
  file.read(reinterpret_cast<char *>(&numParticles), sizeof(int64_t));

  for (int64_t particleIndex = 0; particleIndex < numParticles; particleIndex++) {
    readParticleInfo(file, blockIndex);
  }
}

void GridTest::readParticleInfo(std::ifstream & file, int blockIndex) {
  int64_t particleId = 0;
  // NOLINTNEXTLINE
  file.read(reinterpret_cast<char *>(&particleId), sizeof(int64_t));
  particles[particleId].id = static_cast<int>(particleId);

  readParticleProperties(file, particleId);
  addParticleToBlock(blockIndex, particleId);
}

void GridTest::readParticleProperties(std::ifstream & file, int64_t id) {
  std::array<double, ElementsTraze> buffer = {};

  // Leer los datos del archivo en el búfer
  // NOLINTNEXTLINE
  file.read(reinterpret_cast<char *>(buffer.data()), 13 * sizeof(double));

  // Asignar los datos del búfer a las propiedades de la partícula
  particles[id].posX       = buffer[POS_X_INDEX];
  particles[id].posY       = buffer[POS_Y_INDEX];
  particles[id].posZ       = buffer[POS_Z_INDEX];
  particles[id].smoothVecX = buffer[SMOOTH_VEC_X_INDEX];
  particles[id].smoothVecY = buffer[SMOOTH_VEC_Y_INDEX];
  particles[id].smoothVecZ = buffer[SMOOTH_VEC_Z_INDEX];
  particles[id].velX       = buffer[VEL_X_INDEX];
  particles[id].velY       = buffer[VEL_Y_INDEX];
  particles[id].velZ       = buffer[VEL_Z_INDEX];
  density[id]              = buffer[DENSITY_INDEX];
  accelerationX[id]        = buffer[ACCELERATION_X_INDEX];
  accelerationY[id]        = buffer[ACCELERATION_Y_INDEX];
  accelerationZ[id]        = buffer[ACCELERATION_Z_INDEX];
}

void GridTest::addParticleToBlock(int blockIndex, int64_t id) {
  std::tuple<int, int, int> index = obtenerIndicesDesdeId(blockIndex);
  int const x_index               = std::get<0>(index);
  int const y_index               = std::get<1>(index);
  int const z_index               = std::get<2>(index);

  blocks[x_index][y_index][z_index].addParticle(static_cast<int>(id));
}

int GridTest::comparePositionate(Grid & grid) {
  std::size_t const nx1 = grid.blocks.size();
  std::size_t const ny1 =
      (!grid.blocks.empty() && !grid.blocks[0].empty()) ? grid.blocks[0].size() : 0;
  std::size_t const nz1 =
      (!grid.blocks.empty() && !grid.blocks[0].empty()) ? grid.blocks[0][0].size() : 0;
  if (nx != static_cast<int>(nx1) || ny != static_cast<int>(ny1) || nz != static_cast<int>(nz1)) {
    return -1;
  }
  for (std::size_t x_index = 0; x_index < nx1; ++x_index) {
    for (std::size_t y_index = 0; y_index < ny1; ++y_index) {
      for (std::size_t z_index = 0; z_index < nz1; ++z_index) {
        if (!std::is_permutation(blocks[x_index][y_index][z_index].particlesID.begin(),
                                 blocks[x_index][y_index][z_index].particlesID.end(),
                                 grid.blocks[x_index][y_index][z_index].particlesID.begin())) {
          return -1;
        }
      }
    }
  }

  return 0;
}

int GridTest::compareDensity(Grid & grid) {
  if (density.size() != grid.density.size()) { return -1; }

  // Función lambda para redondear vectores
  auto roundVector = [](std::vector<double> & vec) {
    for (auto & value : vec) {
      value = std::round(value * ROUND_DENSITY) / ROUND_DENSITY;  // Redondear a dos decimales
    }
  };

  roundVector(density);
  roundVector(grid.density);

  if (!std::is_permutation(density.begin(), density.end(), grid.density.begin())) { return -1; }

  return 0;
}

int GridTest::compareAccelerationX(Grid & grid) {
  return compareAccelerationComponent(accelerationX, grid.accelerationX);
}

int GridTest::compareAccelerationY(Grid & grid) {
  return compareAccelerationComponent(accelerationY, grid.accelerationY);
}

int GridTest::compareAccelerationZ(Grid & grid) {
  return compareAccelerationComponent(accelerationZ, grid.accelerationZ);
}

int GridTest::compareAccelerationComponent(std::vector<double> & vec1, std::vector<double> & vec2) {
  if (vec1.size() != vec2.size()) { return -1; }
  auto roundVector = [](std::vector<double> & vec) {
    for (auto & value : vec) {
      value =
          std::round(value * ROUND_ACCELERATION) / ROUND_ACCELERATION;  // Redondear a dos decimales
    }
  };
  roundVector(vec1);
  roundVector(vec2);
  if (!std::is_permutation(vec1.begin(), vec1.end(), vec2.begin())) {
    std::cout << "Valores diferentes encontrados:"
              << "\n";
    for (size_t i = 0; i < vec1.size(); ++i) {
      if (vec1[i] != vec2[i]) {
        std::cout << "vec1[" << i << "]: " << vec1[i] << " != vec2[" << i << "]: " << vec2[i]
                  << "\n";
      }
    }
    return -1;
  }
  return 0;
}

int GridTest::compareParticles(Grid & grid) {
  if (particles.size() != grid.particles.size()) {
    std::cout << particles.size() << "||" << grid.particles.size() << "\n";
    return -1;
  }

  for (size_t i = 0; i < particles.size(); ++i) {
    if (areParticlesDifferent(particles[i], grid.particles[i])) {
      printDifferentParticlesDetails(particles[i], grid.particles[i]);
      return -1;
    }
  }
  return 0;
}

bool GridTest::areParticlesDifferent(Particle const & particle1, Particle const & particle2) {
  return particle1.id != particle2.id || !areEqualWithTolerance(particle1.posX, particle2.posX) ||
         !areEqualWithTolerance(particle1.posY, particle2.posY) ||
         !areEqualWithTolerance(particle1.posZ, particle2.posZ) ||
         !areEqualWithTolerance(particle1.smoothVecX, particle2.smoothVecX) ||
         !areEqualWithTolerance(particle1.smoothVecY, particle2.smoothVecY) ||
         !areEqualWithTolerance(particle1.smoothVecZ, particle2.smoothVecZ) ||
         !areEqualWithTolerance(particle1.velX, particle2.velX) ||
         !areEqualWithTolerance(particle1.velY, particle2.velY) ||
         !areEqualWithTolerance(particle1.velZ, particle2.velZ);
}

void GridTest::printDifferentParticlesDetails(Particle const & particle1,
                                              Particle const & particle2) {
  std::cout << "Partícula diferente encontrada: ID " << particle1.id
            << " || posX: " << particle1.posX << " || grid.posX: " << particle2.posX << "\n";

  // Mostrar atributos diferentes junto con sus valores
  printDifferentAttributes(particle1.id, "id", particle2.id);
  printDifferentAttributes(particle1.posX, "posX", particle2.posX);
  printDifferentAttributes(particle1.posY, "posY", particle2.posY);
  printDifferentAttributes(particle1.posZ, "posZ", particle2.posZ);
  printDifferentAttributes(particle1.smoothVecX, "smoothVecX", particle2.smoothVecX);
  printDifferentAttributes(particle1.smoothVecY, "smoothVecY", particle2.smoothVecY);
  printDifferentAttributes(particle1.smoothVecZ, "smoothVecZ", particle2.smoothVecZ);
  printDifferentAttributes(particle1.velX, "velX", particle2.velX);
  printDifferentAttributes(particle1.velY, "velY", particle2.velY);
  printDifferentAttributes(particle1.velZ, "velZ", particle2.velZ);

  std::cout << "***********************************************************************\n";
}

// Función de utilidad para comparar dos valores con tolerancia
bool GridTest::areEqualWithTolerance(double value1, double value2) {
  return std::round(value1 * ROUND_MOTION) / ROUND_MOTION ==
         std::round(value2 * ROUND_MOTION) / ROUND_MOTION;
}

// Función de utilidad para imprimir detalles de atributos diferentes
void GridTest::printDifferentAttributes(double value1, std::string const & attributeName,
                                        double value2) {
  if (value1 != value2) {
    std::cout << "   - " << attributeName << ": " << value1 << " || grid." << attributeName << ": "
              << value2 << "\n";
  }
}
