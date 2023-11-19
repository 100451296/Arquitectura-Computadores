#include "gridTestSoA.hpp"

std::tuple<int, int, int> GridSoATest::obtenerIndicesDesdeId(int identificador) const {
  int x = identificador % nx;
  int y = (identificador / nx) % ny;
  int z = identificador / (nx * ny);

  return std::make_tuple(x, y, z);
}

void GridSoATest::readTraze(std::string const & input_file_name) {
  std::ifstream file(input_file_name);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file: " << input_file_name << std::endl;
    return;
  }

  int numBlocks = readHeader(file);

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
  int numBlocks;
  file.read(reinterpret_cast<char *>(&numBlocks), sizeof(int));
  return numBlocks;
}

void GridSoATest::readParticlesInfo(std::ifstream & file, int numBlocks) {
  for (int blockIndex = 0; blockIndex < numBlocks; ++blockIndex) {
    readBlockInfo(file, blockIndex);
  }
}

void GridSoATest::readBlockInfo(std::ifstream & file, int blockIndex) {
  int64_t numParticles;
  file.read(reinterpret_cast<char *>(&numParticles), sizeof(int64_t));

  for (int64_t particleIndex = 0; particleIndex < numParticles; particleIndex++) {
    readParticleInfo(file, blockIndex);
  }
}

void GridSoATest::readParticleInfo(std::ifstream & file, int blockIndex) {
  int64_t id;
  file.read(reinterpret_cast<char *>(&id), sizeof(int64_t));

  // Asignar el ID a la posición correspondiente en el array de IDs
  particles.id.push_back(static_cast<int>(id));

  // Leer las propiedades de la partícula y añadirlas a los arrays correspondientes
  readParticleProperties(file, id);

  // Añadir la partícula al bloque
  addParticleToBlock(blockIndex, static_cast<int>(id));
}

void GridSoATest::addParticleToBlock(int blockIndex, int64_t id) {
  std::tuple<int, int, int> index = obtenerIndicesDesdeId(blockIndex);
  int x                           = std::get<0>(index);
  int y                           = std::get<1>(index);
  int z                           = std::get<2>(index);

  blocks[x][y][z].addParticle(id);
}

void GridSoATest::readParticleProperties(std::ifstream & file, int64_t id) {
  // Usar un solo búfer para almacenar los datos leídos del archivo
  double buffer[13];

  // Leer los datos del archivo en el búfer
  file.read(reinterpret_cast<char *>(buffer), 13 * sizeof(double));

  // Asignar los datos del búfer a las propiedades de las partículas en los arreglos SoA
  particles.posX[id]       = buffer[0];
  particles.posY[id]       = buffer[1];
  particles.posZ[id]       = buffer[2];
  particles.smoothVecX[id] = buffer[3];
  particles.smoothVecY[id] = buffer[4];
  particles.smoothVecZ[id] = buffer[5];
  particles.velX[id]       = buffer[6];
  particles.velY[id]       = buffer[7];
  particles.velZ[id]       = buffer[8];
  density[id]              = buffer[9];
  accelerationX[id]        = buffer[10];
  accelerationY[id]        = buffer[11];
  accelerationZ[id]        = buffer[12];
}

void GridSoATest::initializeGridAndBlocks() {
  nx = NX;
  ny = NY;
  nz = NZ;
  initializeBlockVectors();
}

int GridSoATest::comparePositionate(GridSoA & grid) {
  // Obtener la longitud de cada dimensión
  int nx1 = grid.blocks.size();  // Longitud de la primera dimensión
  int ny1 =
      (grid.blocks.size() > 0) ? grid.blocks[0].size() : 0;  // Longitud de la segunda dimensión
  int nz1 = (grid.blocks.size() > 0 && grid.blocks[0].size() > 0)
                ? grid.blocks[0][0].size()
                : 0;  // Longitud de la tercera dimensión

  // Las mallas no tienen la misma longitud
  if (nx != nx1 || ny != ny1 || nz != nz1) { return -1; }

  // Comprueba bloques
  for (int x = 0; x < nx; ++x) {
    for (int y = 0; y < ny; ++y) {
      for (int z = 0; z < nz; ++z) {
        if (!std::is_permutation(blocks[x][y][z].particlesID.begin(),
                                 blocks[x][y][z].particlesID.end(),
                                 grid.blocks[x][y][z].particlesID.begin())) {
          return -1;
        }
      }
    }
  }
  return 0;
}
