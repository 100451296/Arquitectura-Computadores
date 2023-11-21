#include "grid.hpp"

using namespace std;

// Offsets para buscar bloques contiguos
// NOLINTNEXTLINE
std::vector<std::tuple<int, int, int>> const Grid::offsets = {
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
void Grid::initGrid() {
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
  data.h_square       = pow(h, Two);
  data.h_pow6         = pow(h, Six);
  data.h_pow9         = pow(h, Nine);

  initBlocks();
  initDensityAcceleration();
}

void Grid::initDensityAcceleration() {
  // Inicializa los vectores de densidad blockY aceleración con el tamaño adecuado
  // basado en el número de partículas en el bloque

  // Establece el tamaño de los vectores de densidad blockY aceleración
  density.resize(particles.size(), 0.0);
  accelerationX.resize(particles.size(), ACELERACION_GRAVEDAD_X);
  accelerationY.resize(particles.size(), ACELERACION_GRAVEDAD_Y);
  accelerationZ.resize(particles.size(), ACELERACION_GRAVEDAD_Z);
}

void Grid::printParameters() const {
  std::cout << "Number of particles: " << num_particles << "\n";
  std::cout << "Particles per meter: " << ppm << "\n";
  std::cout << "Smoothing length: " << h << "\n";
  std::cout << "Particle mass: " << particle_mass << "\n";
  std::cout << "Grid size: " << nx << " blockX " << ny << " blockX " << nz << "\n";
  std::cout << "Number of blocks: " << num_blocks << "\n";
  std::cout << "Block size: " << sx << " blockX " << sy << " blockX " << sz << "\n";
}

void Grid::initBlocks() {
  initializeBlockVectors();
  populatePairs();
}

void Grid::initializeBlockVectors() {
  for (int blockX = 0; blockX < nx; blockX++) {
    blocks.emplace_back();  // Emplace an empty vector for the outer vector
    auto & tempVector2 = blocks.back();
    tempVector2.reserve(ny);  // Reserve capacity for the outer vector
    for (int blockY = 0; blockY < ny; blockY++) {
      tempVector2.emplace_back();  // Emplace an empty vector for the inner vector
      auto & tempVector1 = tempVector2.back();
      tempVector1.reserve(nz);  // Reserve capacity for the inner vector
      for (int blockZ = 0; blockZ < nz; blockZ++) {
        tempVector1.emplace_back(particles, density, accelerationX, accelerationY, accelerationZ);
      }
    }
  }
}

void Grid::populatePairs() {
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

int Grid::readFile(string const & input_file_name) {
  // vector<Particle> particles;  // Inicializa vector de particulas

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

bool Grid::readHeader(std::ifstream & input_file) {
  float buffer = INIT_BUFFER;
  // NOLINTNEXTLINE
  input_file.read(reinterpret_cast<char *>(&buffer), sizeof(float));
  // NOLINTNEXTLINE
  input_file.read(reinterpret_cast<char *>(&num_particles), sizeof(int));

  ppm = static_cast<double>(buffer);
  return input_file.good();
}

bool Grid::readParticles(std::ifstream & input_file) {
  particles.resize(num_particles);
  for (int i = 0; i < num_particles; i++) {
    if (!readParticle(input_file, particles[i], i)) { return false; }
  }
  return true;
}

bool Grid::readParticle(std::ifstream & input_file, Particle & particle, int index) {
  std::array<float, particleAttr> buffer = {};
  // NOLINTNEXTLINE
  if (!input_file.read(reinterpret_cast<char *>(buffer.data()), sizeof(float) * Nine)) {
    std::cerr << "Error al leer las partículas del archivo\n";
    return false;
  }

  particle.id         = index;
  particle.posX       = static_cast<double>(buffer[POS_X_INDEX]);
  particle.posY       = static_cast<double>(buffer[POS_Y_INDEX]);
  particle.posZ       = static_cast<double>(buffer[POS_Z_INDEX]);
  particle.smoothVecX = static_cast<double>(buffer[SMOOTH_VEC_X_INDEX]);
  particle.smoothVecY = static_cast<double>(buffer[SMOOTH_VEC_Y_INDEX]);
  particle.smoothVecZ = static_cast<double>(buffer[SMOOTH_VEC_Z_INDEX]);
  particle.velX       = static_cast<double>(buffer[VEL_X_INDEX]);
  particle.velY       = static_cast<double>(buffer[VEL_Y_INDEX]);
  particle.velZ       = static_cast<double>(buffer[VEL_Z_INDEX]);

  return true;
}

int Grid::writeFile(std::string const & output_file_name) {
  // NOLINTNEXTLINENEXTLINE
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

bool Grid::writeHeader(std::ofstream & output_file) {
  auto ppm_float = static_cast<float>(ppm);
  // NOLINTNEXTLINE
  output_file.write(reinterpret_cast<char const *>(&ppm_float), sizeof(float));
  // NOLINTNEXTLINE
  output_file.write(reinterpret_cast<char const *>(&num_particles), sizeof(int));
  return output_file.good();
}

bool Grid::writeParticles(std::ofstream & output_file) const {
  for (auto const & particle : particles) {
    if (!writeParticle(output_file, particle)) { return false; }
  }
  return true;
}

bool Grid::writeParticle(std::ofstream & output_file, Particle const & particle) {
  std::array<float, particleAttr> buffer = {
    static_cast<float>(particle.posX),       static_cast<float>(particle.posY),
    static_cast<float>(particle.posZ),       static_cast<float>(particle.smoothVecX),
    static_cast<float>(particle.smoothVecY), static_cast<float>(particle.smoothVecZ),
    static_cast<float>(particle.velX),       static_cast<float>(particle.velY),
    static_cast<float>(particle.velZ)};
  // NOLINTNEXTLINE
  if (!output_file.write(reinterpret_cast<char const *>(buffer.data()), sizeof(float) * Nine)) {
    std::cerr << "Error al escribir las partículas en el archivo"
              << "\n";
    return false;
  }

  return true;
}

void Grid::simulation(int iterations) {
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

void Grid::printParticles() {
  std::cout << "Partículas en la cuadrícula:"
            << "\n";
  for (auto const & particle : particles) {
    std::cout << "ID: " << particle.id << ", ";
    std::cout << "Posición (" << particle.posX << ", " << particle.posY << ", " << particle.posZ
              << "), ";
    std::cout << "Vector de suavizado (" << particle.smoothVecX << ", " << particle.smoothVecY
              << ", " << particle.smoothVecZ << "), ";
    std::cout << "Velocidad (" << particle.velX << ", " << particle.velY << ", " << particle.velZ
              << ")";
  }
}

void Grid::generateParticlePairs() {
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockY = 0; blockY < ny; blockY++) {
      for (int blockZ = 0; blockZ < nz; blockZ++) {
        blocks[blockX][blockY][blockZ].generarParejasBloque();
      }
    }
  }
}

void Grid::printFirst() {
  std::cout << "Partículas en la cuadrícula:"
            << "\n";
  for (size_t i = 0; i < 2; ++i) {
    std::cout << "ID: " << particles[0].id << ", ";
    std::cout << "Posición (" << particles[0].posX << ", " << particles[0].posY << ", "
              << particles[0].posZ << "), ";
    std::cout << "Vector de suavizado (" << particles[0].smoothVecX << ", "
              << particles[0].smoothVecY << ", " << particles[0].smoothVecZ << "), ";
    std::cout << "Velocidad (" << particles[0].velX << ", " << particles[0].velY << ", "
              << particles[0].velZ << ")"
              << "\n";
    particles[0].posX = 0.0;
  }
}

void Grid::resetBlocks() {
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockY = 0; blockY < ny; blockY++) {
      for (int blockZ = 0; blockZ < nz; blockZ++) {
        blocks[blockX][blockY][blockZ].initDensityAcceleration();
        blocks[blockX][blockY][blockZ].resetBlock();
      }
    }
  }
}

void Grid::positionateParticle() {
  for (auto & particle : particles) {
    // Obtener índices de bloque
    int const index_i =
        std::max(0, std::min(nx - 1, static_cast<int>(std::floor(
                                         (particle.posX - LIMITE_INFERIOR_RECINTO_X) / sx))));
    int const index_j =
        std::max(0, std::min(ny - 1, static_cast<int>(std::floor(
                                         (particle.posY - LIMITE_INFERIOR_RECINTO_Y) / sy))));
    int const index_k =
        std::max(0, std::min(nz - 1, static_cast<int>(std::floor(
                                         (particle.posZ - LIMITE_INFERIOR_RECINTO_Z) / sz))));
    // Insertar partícula en el bloque correspondiente
    blocks[index_i][index_j][index_k].addParticle(static_cast<int>(particle.id));
  }
  generateParticlePairs();
}

void Grid::densityIncreaseGrid() {
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

void Grid::linealDensityTransform() {
  // Aumentar densidad para cada bloque en el grid
  for (auto & block : blocks) {
    for (auto & row : block) {
      for (auto & individualBlock : row) { individualBlock.lineal_transformate_density(); }
    }
  }
}

void Grid::aceletarionTransferGrid() {
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

void Grid::collisionsXGrid() {
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

void Grid::collisionsYGrid() {
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

void Grid::collisionsZGrid() {
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

void Grid::particleMotionGrid() {
  for (int blockX = 0; blockX < nx; blockX++) {
    for (int blockY = 0; blockY < ny; blockY++) {
      for (int blockZ = 0; blockZ < nz; blockZ++) {
        blocks[blockX][blockY][blockZ].particleMotion();
      }
    }
  }
}

void Grid::interactionsXGrid() {
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

void Grid::interactionsYGrid() {
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

void Grid::interactionsZGrid() {
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
