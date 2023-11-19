#include "grid_soa.hpp"

using namespace std;

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
  data.h_square       = pow(h, 2);

  initBlocks();
  initDensityAcceleration();
}

void Grid::initDensityAcceleration() {
  // Inicializa los vectores de densidad y aceleración con el tamaño adecuado
  // basado en el número de partículas en el bloque

  // Establece el tamaño de los vectores de densidad y aceleración
  density.resize(particles.id.size(), 0.0);
  accelerationX.resize(particles.id.size(), ACELERACION_GRAVEDAD_X);
  accelerationY.resize(particles.id.size(), ACELERACION_GRAVEDAD_Y);
  accelerationZ.resize(particles.id.size(), ACELERACION_GRAVEDAD_Z);
}

void Grid::printParameters() {
  std::cout << "Number of particles: " << num_particles << std::endl;
  std::cout << "Particles per meter: " << ppm << std::endl;
  std::cout << "Smoothing length: " << h << std::endl;
  std::cout << "Particle mass: " << particle_mass << std::endl;
  std::cout << "Grid size: " << nx << " x " << ny << " x " << nz << std::endl;
  std::cout << "Number of blocks: " << num_blocks << std::endl;
  std::cout << "Block size: " << sx << " x " << sy << " x " << sz << std::endl;
}

void Grid::initBlocks() {
  initializeBlockVectors();
  populatePairs();
}

void Grid::initializeBlockVectors() {
  for (int x = 0; x < nx; x++) {
    std::vector<std::vector<Block>> tempVector2;
    for (int y = 0; y < ny; y++) {
      std::vector<Block> tempVector1;
      for (int z = 0; z < nz; z++) {
        tempVector1.push_back(
            Block(particles, accelerationX, accelerationY, accelerationZ, density));
      }
      tempVector2.push_back(tempVector1);
    }
    blocks.push_back(tempVector2);
  }
}

void Grid::populatePairs() {
  for (int x = 0; x < nx; x++) {
    for (int y = 0; y < ny; y++) {
      for (int z = 0; z < nz; z++) {
        blocks[x][y][z].data = data;
        for (auto const & offset : offsets) {
          int x_offset, y_offset, z_offset;
          std::tie(x_offset, y_offset, z_offset) = offset;

          int x_contiguo = x + x_offset;
          int y_contiguo = y + y_offset;
          int z_contiguo = z + z_offset;

          if (x_contiguo >= 0 && x_contiguo < nx && y_contiguo >= 0 && y_contiguo < ny &&
              z_contiguo >= 0 && z_contiguo < nz) {
            parejas_unicas.push_back(std::make_pair(
                std::make_tuple(x, y, z), std::make_tuple(x_contiguo, y_contiguo, z_contiguo)));
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
    std::cerr << "Error al abrir el archivo de entrada" << std::endl;
    return -1;
  }

  if (!readHeader(input_file)) { return -1; }

  if (num_particles <= 0) {
    std::cerr << "Número de partículas inválido" << std::endl;
    return -1;
  }

  if (!readParticles(input_file)) { return -1; }

  input_file.close();

  initGrid();
  return 0;
}

bool Grid::readHeader(std::ifstream & input_file) {
  float buffer;
  input_file.read(reinterpret_cast<char *>(&buffer), sizeof(float));
  input_file.read(reinterpret_cast<char *>(&num_particles), sizeof(int));

  ppm = static_cast<double>(buffer);
  return input_file.good();
}

bool Grid::readParticles(std::ifstream & input_file) {
  particles.resize(num_particles);

  // Asegúrate de que particles.id tenga el mismo tamaño que particles.posX, etc.
  particles.id.resize(num_particles);

  for (int i = 0; i < num_particles; i++) {
    if (!readParticle(input_file, particles, i)) { return false; }
  }
  return true;
}

bool Grid::readParticle(std::ifstream & input_file, Particles & particles, int index) {
  float buffer[9];
  if (!input_file.read(reinterpret_cast<char *>(buffer), sizeof(float) * 9)) {
    std::cerr << "Error al leer las partículas del archivo" << std::endl;
    return false;
  }

  // Asegúrate de que particles.id tenga el mismo tamaño que particles.posX, etc.
  particles.id[index]         = index;
  particles.posX[index]       = static_cast<double>(buffer[0]);
  particles.posY[index]       = static_cast<double>(buffer[1]);
  particles.posZ[index]       = static_cast<double>(buffer[2]);
  particles.smoothVecX[index] = static_cast<double>(buffer[3]);
  particles.smoothVecY[index] = static_cast<double>(buffer[4]);
  particles.smoothVecZ[index] = static_cast<double>(buffer[5]);
  particles.velX[index]       = static_cast<double>(buffer[6]);
  particles.velY[index]       = static_cast<double>(buffer[7]);
  particles.velZ[index]       = static_cast<double>(buffer[8]);

  return true;
}

int Grid::writeFile(std::string const & output_file_name) {
  std::ofstream output_file(output_file_name, std::ios::binary);
  if (!output_file.is_open()) {
    std::cerr << "Error al abrir el archivo de salida" << std::endl;
    return -1;
  }

  if (!writeHeader(output_file)) { return -1; }

  if (!writeParticles(output_file)) { return -1; }

  output_file.close();
  return 0;
}

bool Grid::writeHeader(std::ofstream & output_file) {
  float ppm_float = static_cast<float>(ppm);
  output_file.write(reinterpret_cast<char const *>(&ppm_float), sizeof(float));
  output_file.write(reinterpret_cast<char const *>(&num_particles), sizeof(int));
  return output_file.good();
}

bool Grid::writeParticles(std::ofstream & output_file) {
  for (size_t i = 0; i < particles.id.size(); i++) {
    if (!writeParticle(output_file, particles, i)) { return false; }
  }
  return true;
}

bool Grid::writeParticle(std::ofstream & output_file, Particles const & particles, int index) {
  float buffer[9] = {static_cast<float>(particles.posX[index]),
                     static_cast<float>(particles.posY[index]),
                     static_cast<float>(particles.posZ[index]),
                     static_cast<float>(particles.smoothVecX[index]),
                     static_cast<float>(particles.smoothVecY[index]),
                     static_cast<float>(particles.smoothVecZ[index]),
                     static_cast<float>(particles.velX[index]),
                     static_cast<float>(particles.velY[index]),
                     static_cast<float>(particles.velZ[index])};

  if (!output_file.write(reinterpret_cast<char const *>(buffer), sizeof(float) * 9)) {
    std::cerr << "Error al escribir las partículas en el archivo" << std::endl;
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
  std::cout << "Partículas en la cuadrícula:" << std::endl;
  for (size_t i = 0; i < particles.id.size(); ++i) {
    std::cout << "ID: " << particles.id[i] << ", ";
    std::cout << "Posición (" << particles.posX[i] << ", " << particles.posY[i] << ", "
              << particles.posZ[i] << "), ";
    std::cout << "Vector de suavizado (" << particles.smoothVecX[i] << ", "
              << particles.smoothVecY[i] << ", " << particles.smoothVecZ[i] << "), ";
    std::cout << "Velocidad (" << particles.velX[i] << ", " << particles.velY[i] << ", "
              << particles.velZ[i] << ")" << std::endl;
  }
}

void Grid::generateParticlePairs() {
  for (int x = 0; x < nx; x++) {
    for (int y = 0; y < ny; y++) {
      for (int z = 0; z < nz; z++) { blocks[x][y][z].generarParejasBloque(); }
    }
  }
}

void Grid::printFirst() {
  std::cout << "Partículas en la cuadrícula:" << std::endl;
  for (size_t i = 0; i < 2; ++i) {
    std::cout << "ID: " << particles.id[0] << ", ";
    std::cout << "Posición (" << particles.posX[0] << ", " << particles.posY[0] << ", "
              << particles.posZ[0] << "), ";
    std::cout << "Vector de suavizado (" << particles.smoothVecX[0] << ", "
              << particles.smoothVecY[0] << ", " << particles.smoothVecZ[0] << "), ";
    std::cout << "Velocidad (" << particles.velX[0] << ", " << particles.velY[0] << ", "
              << particles.velZ[0] << ")" << std::endl;
    particles.posX[0] = 0.0;
  }
}

void Grid::resetBlocks() {
  for (int x = 0; x < nx; x++) {
    for (int y = 0; y < ny; y++) {
      for (int z = 0; z < nz; z++) {
        blocks[x][y][z].initDensityAcceleration();
        blocks[x][y][z].resetBlock();
      }
    }
  }
}

void Grid::positionateParticle() {
  for (size_t index = 0; index < particles.id.size(); ++index) {
    // Obtener índices de bloque
    int i = std::max(
        0, std::min(nx - 1, static_cast<int>(std::floor(
                                (particles.posX[index] - LIMITE_INFERIOR_RECINTO_X) / sx))));
    int j = std::max(
        0, std::min(ny - 1, static_cast<int>(std::floor(
                                (particles.posY[index] - LIMITE_INFERIOR_RECINTO_Y) / sy))));
    int k = std::max(
        0, std::min(nz - 1, static_cast<int>(std::floor(
                                (particles.posZ[index] - LIMITE_INFERIOR_RECINTO_Z) / sz))));
    // Insertar partícula en el bloque correspondiente
    blocks[i][j][k].addParticle(particles.id[index]);
  }
  generateParticlePairs();
}

void Grid::densityIncreaseGrid() {
  for (auto const & pareja : parejas_unicas) {
    int x1, y1, z1, x2, y2, z2;
    std::tie(x1, y1, z1) = pareja.first;
    std::tie(x2, y2, z2) = pareja.second;
    blocks[x1][y1][z1].densityIncrease(blocks[x2][y2][z2]);
  }
  for (int x = 0; x < nx; x++) {
    for (int y = 0; y < ny; y++) {
      for (int z = 0; z < nz; z++) { blocks[x][y][z].densityIncreaseSingle(); }
    }
  }
}

void Grid::linealDensityTransform() {
  for (int z = 0; z < nz; z++) {
    for (int y = 0; y < ny; y++) {
      for (int x = 0; x < nx; x++) { blocks[x][y][z].lineal_transformate_density(); }
    }
  }
}

void Grid::aceletarionTransferGrid() {
  for (auto const & pareja : parejas_unicas) {
    int x1, y1, z1, x2, y2, z2;
    std::tie(x1, y1, z1) = pareja.first;
    std::tie(x2, y2, z2) = pareja.second;
    blocks[x1][y1][z1].accelerationTransfer(blocks[x2][y2][z2]);
  }
  for (int x = 0; x < nx; x++) {
    for (int y = 0; y < ny; y++) {
      for (int z = 0; z < nz; z++) { blocks[x][y][z].accelerationTransferSingle(); }
    }
  }
}

void Grid::collisionsXGrid() {
  int cx = 0;
  for (int y = 0; y < ny; y++) {
    for (int z = 0; z < nz; z++) { blocks[cx][y][z].collisionsX(cx); }
  }

  cx = nx - 1;
  for (int y = 0; y < ny; y++) {
    for (int z = 0; z < nz; z++) { blocks[cx][y][z].collisionsX(cx); }
  }
}

void Grid::collisionsYGrid() {
  int cy = 0;
  for (int x = 0; x < nx; x++) {
    for (int z = 0; z < nz; z++) { blocks[x][cy][z].collisionsY(cy); }
  }

  cy = ny - 1;
  for (int x = 0; x < nx; x++) {
    for (int z = 0; z < nz; z++) { blocks[x][cy][z].collisionsY(cy); }
  }
}

void Grid::collisionsZGrid() {
  int cz = 0;
  for (int y = 0; y < ny; y++) {
    for (int x = 0; x < nx; x++) { blocks[x][y][cz].collisionsZ(cz); }
  }

  cz = nz - 1;
  for (int y = 0; y < ny; y++) {
    for (int x = 0; x < nx; x++) { blocks[x][y][cz].collisionsZ(cz); }
  }
}

void Grid::particleMotionGrid() {
  for (int x = 0; x < nx; x++) {
    for (int y = 0; y < ny; y++) {
      for (int z = 0; z < nz; z++) { blocks[x][y][z].particleMotion(); }
    }
  }
}

void Grid::interactionsXGrid() {
  int cx = 0;
  for (int y = 0; y < ny; y++) {
    for (int z = 0; z < nz; z++) { blocks[cx][y][z].interactionsX(cx); }
  }

  cx = nx - 1;
  for (int y = 0; y < ny; y++) {
    for (int z = 0; z < nz; z++) { blocks[cx][y][z].interactionsX(cx); }
  }
}

void Grid::interactionsYGrid() {
  int cy = 0;
  for (int x = 0; x < nx; x++) {
    for (int z = 0; z < nz; z++) { blocks[x][cy][z].interactionsY(cy); }
  }

  cy = ny - 1;
  for (int x = 0; x < nx; x++) {
    for (int z = 0; z < nz; z++) { blocks[x][cy][z].interactionsY(cy); }
  }
}

void Grid::interactionsZGrid() {
  int cz = 0;
  for (int y = 0; y < ny; y++) {
    for (int x = 0; x < nx; x++) { blocks[x][y][cz].interactionsZ(cz); }
  }

  cz = nz - 1;
  for (int y = 0; y < ny; y++) {
    for (int x = 0; x < nx; x++) { blocks[x][y][cz].interactionsZ(cz); }
  }
}
