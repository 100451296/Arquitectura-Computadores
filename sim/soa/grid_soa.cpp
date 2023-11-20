#include "grid_soa.hpp"

using namespace std;
static constexpr double Nine = 9;
// Offsets para buscar bloques contiguos
std::vector<std::tuple<int, int, int>> const GridSoA::offsets = {
  {1,  1,  1},
  {1,  1,  0},
  {1,  1, -1},
  {1,  0,  1},
  {1,  0,  0},
  {1,  0, -1},
  {1, -1,  1},
  {1, -1,  0},
  {1, -1, -1},
  {0,  1,  1},
  {0,  1,  0},
  {0,  1, -1},
  {0,  0,  1}
};

// Método para inicializar la malla con valores predeterminados
void GridSoA::initGrid() {
  h             = MULTIPLICADOR_RADIO / ppm;
  particle_mass = DENSIDAD_FLUIDO * pow(ppm, -3);
  nx            = floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / h);
  ny            = floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / h);
  nz            = floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / h);
  num_blocks    = nx * ny * nz;
  sx            = (LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / nx;
  sy            = (LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / ny;
  sz            = (LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / nz;

  data.ppm            = static_cast<double>(ppm);
  data.mass           = particle_mass;
  data.long_suavizado = h;
  data.nx             = static_cast<unsigned int>(nx);
  data.ny             = static_cast<unsigned int>(ny);
  data.nz             = static_cast<unsigned int>(nz);
  data.h_square       = pow(h, 2);

  initBlocks();
  initDensityAcceleration();
}

void GridSoA::initDensityAcceleration() {
  // Inicializa los vectores de densidad blockY aceleración con el tamaño adecuado
  // basado en el número de partículas en el bloque

  // Establece el tamaño de los vectores de densidad blockY aceleración
  density.resize(particles.id.size(), 0.0);
  accelerationX.resize(particles.id.size(), ACELERACION_GRAVEDAD_X);
  accelerationY.resize(particles.id.size(), ACELERACION_GRAVEDAD_Y);
  accelerationZ.resize(particles.id.size(), ACELERACION_GRAVEDAD_Z);
}

void GridSoA::printParameters() {
  std::cout << "Number of particles: " << num_particles << "\n";
  std::cout << "Particles per meter: " << ppm << "\n";
  std::cout << "Smoothing length: " << h << "\n";
  std::cout << "Particle mass: " << particle_mass << "\n";
  std::cout << "Grid size: " << nx << " blockX " << ny << " blockX " << nz << "\n";
  std::cout << "Number of blocks: " << num_blocks << "\n";
  std::cout << "Block size: " << sx << " blockX " << sy << " blockX " << sz << "\n";
}

void GridSoA::initBlocks() {
  initializeBlockVectors();
  populatePairs();
}

void GridSoA::initializeBlockVectors() {
  for (int blockX = 0; blockX < nx; blockX++) {
    std::vector<std::vector<Block>> tempVector2;
    tempVector2.reserve(ny);  // Reserve capacity for the outer vector
    for (int blockY = 0; blockY < ny; blockY++) {
      std::vector<Block> tempVector1;
      tempVector1.reserve(nz);  // Reserve capacity for the inner vector
      for (int blockZ = 0; blockZ < nz; blockZ++) {
        tempVector1.emplace_back(
            Block(particles, density, accelerationX, accelerationY, accelerationZ));
      }
      tempVector2.emplace_back(
          std::move(tempVector1));  // Use std::move to avoid unnecessary copies
    }
    blocks.emplace_back(std::move(tempVector2));  // Use std::move to avoid unnecessary copies
  }
}

void GridSoA::populatePairs() {
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockY = 0; blockY < ny; blockY++) {
      for (int blockZ = 0; blockZ < nz; blockZ++) {
        blocks[blockX][blockY][blockZ].data = data;
        for (auto const & offset : offsets) {
          auto const & [x_offset, y_offset, z_offset] = offset;

          int const x_contiguo = blockX + x_offset;
          int const y_contiguo = blockY + y_offset;
          int const z_contiguo = blockZ + z_offset;

          if (x_contiguo >= 0 && x_contiguo < nx && y_contiguo >= 0 && y_contiguo < ny &&
              z_contiguo >= 0 && z_contiguo < nz) {
            parejas_unicas.emplace_back(std::piecewise_construct,
                                        std::forward_as_tuple(blockX, blockY, blockZ),
                                        std::forward_as_tuple(x_contiguo, y_contiguo, z_contiguo));
          }
        }
      }
    }
  }
}

int GridSoA::readFile(string const & input_file_name) {
  std::ifstream input_file(input_file_name, std::ios::binary);
  if (!input_file.is_open()) {
    std::cerr << "Error al abrir el archivo de entrada"
              << "\n";
    return -1;
  }

  if (!readHeader(input_file)) { return -1; }

  if (num_particles <= 0) {
    std::cerr << "Número de partículas inválido"
              << "\n";
    return -1;
  }

  if (!readParticles(input_file)) { return -1; }

  input_file.close();

  initGrid();

  return 0;
}

bool GridSoA::readHeader(std::ifstream & input_file) {
  float buffer = INIT_BUFFER;
  input_file.read(reinterpret_cast<char *>(&buffer), sizeof(float));
  input_file.read(reinterpret_cast<char *>(&num_particles), sizeof(int));

  ppm = static_cast<double>(buffer);
  return input_file.good();
}

bool GridSoA::readParticles(std::ifstream & input_file) {
  particles.resize(num_particles);

  // Asegúrate de que particles.id tenga el mismo tamaño que particles.posX, etc.
  particles.id.resize(num_particles);

  for (int i = 0; i < num_particles; i++) {
    if (!readParticle(input_file, particles, i)) { return false; }
  }
  return true;
}

bool GridSoA::readParticle(std::ifstream & input_file, Particles & particles, int index) {
  std::array<float, particleAttr> buffer;
  if (!input_file.read(reinterpret_cast<char *>(buffer.data()), sizeof(float) * Nine)) {
    std::cerr << "Error al leer las partículas del archivo\n";
    return false;
  }

  particles.id[index]         = index;
  particles.posX[index]       = static_cast<double>(buffer[POS_X_INDEX]);
  particles.posY[index]       = static_cast<double>(buffer[POS_Y_INDEX]);
  particles.posZ[index]       = static_cast<double>(buffer[POS_Z_INDEX]);
  particles.smoothVecX[index] = static_cast<double>(buffer[SMOOTH_VEC_X_INDEX]);
  particles.smoothVecY[index] = static_cast<double>(buffer[SMOOTH_VEC_Y_INDEX]);
  particles.smoothVecZ[index] = static_cast<double>(buffer[SMOOTH_VEC_Z_INDEX]);
  particles.velX[index]       = static_cast<double>(buffer[VEL_X_INDEX]);
  particles.velY[index]       = static_cast<double>(buffer[VEL_Y_INDEX]);
  particles.velZ[index]       = static_cast<double>(buffer[VEL_Z_INDEX]);

  return true;
}

int GridSoA::writeFile(std::string const & output_file_name) {
  std::ofstream output_file(output_file_name, std::ios::binary);
  if (!output_file.is_open()) {
    std::cerr << "Error al abrir el archivo de salida"
              << "\n";
    return -1;
  }

  if (!writeHeader(output_file)) { return -1; }

  if (!writeParticles(output_file)) { return -1; }

  output_file.close();
  return 0;
}

bool GridSoA::writeHeader(std::ofstream & output_file) {
  auto ppm_float = static_cast<float>(ppm);
  output_file.write(reinterpret_cast<char const *>(&ppm_float), sizeof(float));
  output_file.write(reinterpret_cast<char const *>(&num_particles), sizeof(int));
  return output_file.good();
}

bool GridSoA::writeParticles(std::ofstream & output_file) const {
  for (size_t i = 0; i < particles.id.size(); i++) {
    if (!writeParticle(output_file, particles, static_cast<int>(i))) { return false; }
  }
  return true;
}

bool GridSoA::writeParticle(std::ofstream & output_file, Particles const & particles, int index) {
  std::array<float, particleAttr> buffer = {static_cast<float>(particles.posX[index]),
                                            static_cast<float>(particles.posY[index]),
                                            static_cast<float>(particles.posZ[index]),
                                            static_cast<float>(particles.smoothVecX[index]),
                                            static_cast<float>(particles.smoothVecY[index]),
                                            static_cast<float>(particles.smoothVecZ[index]),
                                            static_cast<float>(particles.velX[index]),
                                            static_cast<float>(particles.velY[index]),
                                            static_cast<float>(particles.velZ[index])};

  if (!output_file.write(reinterpret_cast<char const *>(buffer.data()), sizeof(float) * Nine)) {
    std::cerr << "Error al escribir las partículas en el archivo"
              << "\n";
    return false;
  }

  return true;
}

void GridSoA::simulation(int iterations) {
  printParameters();
  for (int i = 0; i < iterations; i++) {
    positionateParticle();
    densityIncreaseGrid();
    linealDensityTransform();
    aceletarionTransferGrid();
    collisionsXGrid();
    collisionsYGrid();
    collisionsZGrid();
    particleMotionGrid();
    interactionsXGrid();
    interactionsYGrid();
    interactionsZGrid();
    resetBlocks();
  }
}

void GridSoA::printParticles() {
  std::cout << "Partículas en la cuadrícula:"
            << "\n";
  for (size_t i = 0; i < particles.id.size(); ++i) {
    std::cout << "ID: " << particles.id[i] << ", ";
    std::cout << "Posición (" << particles.posX[i] << ", " << particles.posY[i] << ", "
              << particles.posZ[i] << "), ";
    std::cout << "Vector de suavizado (" << particles.smoothVecX[i] << ", "
              << particles.smoothVecY[i] << ", " << particles.smoothVecZ[i] << "), ";
    std::cout << "Velocidad (" << particles.velX[i] << ", " << particles.velY[i] << ", "
              << particles.velZ[i] << ")"
              << "\n";
  }
}

void GridSoA::generateParticlePairs() {
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockY = 0; blockY < ny; blockY++) {
      for (int blockZ = 0; blockZ < nz; blockZ++) {
        blocks[blockX][blockY][blockZ].generarParejasBloque();
      }
    }
  }
}

void GridSoA::printFirst() {
  std::cout << "Partículas en la cuadrícula:"
            << "\n";
  for (size_t i = 0; i < 2; ++i) {
    std::cout << "ID: " << particles.id[0] << ", ";
    std::cout << "Posición (" << particles.posX[0] << ", " << particles.posY[0] << ", "
              << particles.posZ[0] << "), ";
    std::cout << "Vector de suavizado (" << particles.smoothVecX[0] << ", "
              << particles.smoothVecY[0] << ", " << particles.smoothVecZ[0] << "), ";
    std::cout << "Velocidad (" << particles.velX[0] << ", " << particles.velY[0] << ", "
              << particles.velZ[0] << ")"
              << "\n";
    particles.posX[0] = 0.0;
  }
}

void GridSoA::resetBlocks() {
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockY = 0; blockY < ny; blockY++) {
      for (int blockZ = 0; blockZ < nz; blockZ++) {
        blocks[blockX][blockY][blockZ].initDensityAcceleration();
        blocks[blockX][blockY][blockZ].resetBlock();
      }
    }
  }
}

void GridSoA::positionateParticle() {
  for (size_t index = 0; index < particles.id.size(); ++index) {
    // Obtener índices de bloque
    int index_i = std::max(
        0, std::min(nx - 1, static_cast<int>(std::floor(
                                (particles.posX[index] - LIMITE_INFERIOR_RECINTO_X) / sx))));
    int index_j = std::max(
        0, std::min(ny - 1, static_cast<int>(std::floor(
                                (particles.posY[index] - LIMITE_INFERIOR_RECINTO_Y) / sy))));
    int index_k = std::max(
        0, std::min(nz - 1, static_cast<int>(std::floor(
                                (particles.posZ[index] - LIMITE_INFERIOR_RECINTO_Z) / sz))));
    // Insertar partícula en el bloque correspondiente
    blocks[index_i][index_j][index_k].addParticle(static_cast<int>(particles.id[index]));
  }
  generateParticlePairs();
}

void GridSoA::densityIncreaseGrid() {
  // Aumentar densidad para pares únicos
  for (auto const & pareja : parejas_unicas) {
    auto const & [x1, y1, z1] = pareja.first;
    auto const & [x2, y2, z2] = pareja.second;
    blocks[x1][y1][z1].densityIncrease(blocks[x2][y2][z2]);
  }

  // Aumentar densidad para cada bloque en el grid
  for (auto & block : blocks) {
    for (auto & row : block) {
      for (auto & individualBlock : row) { individualBlock.densityIncreaseSingle(); }
    }
  }
}

void GridSoA::linealDensityTransform() {
  // Aumentar densidad para cada bloque en el grid
  for (auto & block : blocks) {
    for (auto & row : block) {
      for (auto & individualBlock : row) { individualBlock.lineal_transformate_density(); }
    }
  }
}

void GridSoA::aceletarionTransferGrid() {
  // Aumentar densidad para pares únicos
  for (auto const & pareja : parejas_unicas) {
    auto const & [x1, y1, z1] = pareja.first;
    auto const & [x2, y2, z2] = pareja.second;
    blocks[x1][y1][z1].accelerationTransfer(blocks[x2][y2][z2]);
  }
  for (auto & block : blocks) {
    for (auto & row : block) {
      for (auto & individualBlock : row) { individualBlock.accelerationTransferSingle(); }
    }
  }
}

void GridSoA::collisionsXGrid() {
  int borderX = 0;
  for (int blockY = 0; blockY < ny; blockY++) {
    for (int blockZ = 0; blockZ < nz; blockZ++) {
      blocks[borderX][blockY][blockZ].collisionsX(borderX);
    }
  }

  borderX = nx - 1;
  for (int blockY = 0; blockY < ny; blockY++) {
    for (int blockZ = 0; blockZ < nz; blockZ++) {
      blocks[borderX][blockY][blockZ].collisionsX(borderX);
    }
  }
}

void GridSoA::collisionsYGrid() {
  int borderY = 0;
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockZ = 0; blockZ < nz; blockZ++) {
      blocks[blockX][borderY][blockZ].collisionsY(borderY);
    }
  }

  borderY = ny - 1;
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockZ = 0; blockZ < nz; blockZ++) {
      blocks[blockX][borderY][blockZ].collisionsY(borderY);
    }
  }
}

void GridSoA::collisionsZGrid() {
  int borderZ = 0;
  for (int blockY = 0; blockY < ny; blockY++) {
    for (int blockX = 0; blockX < nx; blockX++) {
      blocks[blockX][blockY][borderZ].collisionsZ(borderZ);
    }
  }

  borderZ = nz - 1;
  for (int blockY = 0; blockY < ny; blockY++) {
    for (int blockX = 0; blockX < nx; blockX++) {
      blocks[blockX][blockY][borderZ].collisionsZ(borderZ);
    }
  }
}

void GridSoA::particleMotionGrid() {
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockY = 0; blockY < ny; blockY++) {
      for (int blockZ = 0; blockZ < nz; blockZ++) {
        blocks[blockX][blockY][blockZ].particleMotion();
      }
    }
  }
}

void GridSoA::interactionsXGrid() {
  int borderX = 0;
  for (int blockY = 0; blockY < ny; blockY++) {
    for (int blockZ = 0; blockZ < nz; blockZ++) {
      blocks[borderX][blockY][blockZ].interactionsX(borderX);
    }
  }

  borderX = nx - 1;
  for (int blockY = 0; blockY < ny; blockY++) {
    for (int blockZ = 0; blockZ < nz; blockZ++) {
      blocks[borderX][blockY][blockZ].interactionsX(borderX);
    }
  }
}

void GridSoA::interactionsYGrid() {
  int borderY = 0;
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockZ = 0; blockZ < nz; blockZ++) {
      blocks[blockX][borderY][blockZ].interactionsY(borderY);
    }
  }

  borderY = ny - 1;
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockZ = 0; blockZ < nz; blockZ++) {
      blocks[blockX][borderY][blockZ].interactionsY(borderY);
    }
  }
}

void GridSoA::interactionsZGrid() {
  int borderZ = 0;
  for (int blockY = 0; blockY < ny; blockY++) {
    for (int blockX = 0; blockX < nx; blockX++) {
      blocks[blockX][blockY][borderZ].interactionsZ(borderZ);
    }
  }

  borderZ = nz - 1;
  for (int blockY = 0; blockY < ny; blockY++) {
    for (int blockX = 0; blockX < nx; blockX++) {
      blocks[blockX][blockY][borderZ].interactionsZ(borderZ);
    }
  }
}
