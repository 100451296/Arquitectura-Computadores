#include "gridTestSoA.hpp"

std::tuple<int, int, int> GridSoATest::obtenerIndicesDesdeId(int identificador) const {
  int const x_index = identificador % nx;
  int const y_index = (identificador / nx) % ny;
  int const z_index = identificador / (nx * ny);

  return std::make_tuple(x_index, y_index, z_index);
}

void GridSoATest::readTraze(std::string const & input_file_name) {
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

int GridSoATest::readHeader(std::ifstream & file) {
  int numBlocks = 0;
  // NOLINTNEXTLINE
  file.read(reinterpret_cast<char *>(&numBlocks), sizeof(int));
  return numBlocks;
}

void GridSoATest::readParticlesInfo(std::ifstream & file, int numBlocks) {
  for (int blockIndex = 0; blockIndex < numBlocks; ++blockIndex) {
    readBlockInfo(file, blockIndex);
  }
}

void GridSoATest::readBlockInfo(std::ifstream & file, int blockIndex) {
  int64_t numParticles = 0;
  // NOLINTNEXTLINE
  file.read(reinterpret_cast<char *>(&numParticles), sizeof(int64_t));

  for (int64_t particleIndex = 0; particleIndex < numParticles; particleIndex++) {
    readParticleInfo(file, blockIndex);
  }
}

void GridSoATest::readParticleInfo(std::ifstream & file, int blockIndex) {
  int64_t particleId = 0;
  // NOLINTNEXTLINE
  file.read(reinterpret_cast<char *>(&particleId), sizeof(int64_t));

  // Asignar el ID a la posición correspondiente en el array de IDs
  particles.id.push_back(static_cast<int>(particleId));

  // Leer las propiedades de la partícula y añadirlas a los arrays correspondientes
  readParticleProperties(file, particleId);

  // Añadir la partícula al bloque
  addParticleToBlock(blockIndex, static_cast<int>(particleId));
}

void GridSoATest::addParticleToBlock(int blockIndex, int64_t id) {
  std::tuple<int, int, int> index = obtenerIndicesDesdeId(blockIndex);
  int const x_index               = std::get<0>(index);
  int const y_index               = std::get<1>(index);
  int const z_index               = std::get<2>(index);

  blocks[x_index][y_index][z_index].addParticle(static_cast<int>(id));
}

void GridSoATest::readParticleProperties(std::ifstream & file, int64_t id) {
  std::array<double, ElementsTraze> buffer = {};

  // Leer los datos del archivo en el búfer
  // NOLINTNEXTLINE
  file.read(reinterpret_cast<char *>(buffer.data()), 13 * sizeof(double));

  // Asignar los datos del búfer a las propiedades de las partículas en los arreglos SoA
  particles.posX[id]       = buffer[POS_X_INDEX];
  particles.posY[id]       = buffer[POS_Y_INDEX];
  particles.posZ[id]       = buffer[POS_Z_INDEX];
  particles.smoothVecX[id] = buffer[SMOOTH_VEC_X_INDEX];
  particles.smoothVecY[id] = buffer[SMOOTH_VEC_Y_INDEX];
  particles.smoothVecZ[id] = buffer[SMOOTH_VEC_Z_INDEX];
  particles.velX[id]       = buffer[VEL_X_INDEX];
  particles.velY[id]       = buffer[VEL_Y_INDEX];
  particles.velZ[id]       = buffer[VEL_Z_INDEX];
  density[id]              = buffer[DENSITY_INDEX];
  accelerationX[id]        = buffer[ACCELERATION_X_INDEX];
  accelerationY[id]        = buffer[ACCELERATION_Y_INDEX];
  accelerationZ[id]        = buffer[ACCELERATION_Z_INDEX];
}

void GridSoATest::initializeGridAndBlocks() {
  nx = NX;
  ny = NY;
  nz = NZ;
  initializeBlockVectors();
}

int GridSoATest::comparePositionate(GridSoA & grid) {
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
