#include <grid.hpp>

using namespace std;

// Método para inicializar la malla con valores predeterminados
void Grid::initGrid() {
  h             = MULTIPLICADOR_RADIO / ppm;
  particle_mass = DENSIDAD_FLUIDO * std::pow(ppm, -3);
  nx            = std::floor((LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / h);
  ny            = std::floor((LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / h);
  nz            = std::floor((LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / h);
  num_blocks    = nx * ny * nz;
  sx            = (LIMITE_SUPERIOR_RECINTO_X - LIMITE_INFERIOR_RECINTO_X) / nx;
  sy            = (LIMITE_SUPERIOR_RECINTO_Y - LIMITE_INFERIOR_RECINTO_Y) / ny;
  sz            = (LIMITE_SUPERIOR_RECINTO_Z - LIMITE_INFERIOR_RECINTO_Z) / nz;

  data.ppm            = static_cast<double>(ppm);
  data.mass           = static_cast<double>(particle_mass);
  data.long_suavizado = h;
  data.nx             = static_cast<unsigned int>(nx);
  data.ny             = static_cast<unsigned int>(ny);
  data.nz             = static_cast<unsigned int>(nz);

  printParameters();
  initBlocks();
  initDensityAcceleration();
}

void Grid::initDensityAcceleration() {
  // Inicializa los vectores de densidad y aceleración con el tamaño adecuado
  // basado en el número de partículas en el bloque

  // Establece el tamaño de los vectores de densidad y aceleración
  density.resize(particles.size(), 0.0);
  accelerationX.resize(particles.size(), ACELERACION_GRAVEDAD_X);
  accelerationY.resize(particles.size(), ACELERACION_GRAVEDAD_Y);
  accelerationZ.resize(particles.size(), ACELERACION_GRAVEDAD_Z);
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
  for (int x = 0; x < nx; ++x) {
    std::vector<std::vector<Block>> tempVector2;
    for (int y = 0; y < ny; ++y) {
      std::vector<Block> tempVector1;
      for (int z = 0; z < nz; ++z) {
        tempVector1.push_back(
            Block(particles, accelerationX, accelerationY, accelerationZ, density));
      }
      tempVector2.push_back(tempVector1);
    }
    blocks.push_back(tempVector2);
  }
}

void Grid::populatePairs() {
  for (int x = 0; x < nx; ++x) {
    for (int y = 0; y < ny; ++y) {
      for (int z = 0; z < nz; ++z) {
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
  input_file.read(reinterpret_cast<char *>(&ppm), sizeof(float));
  input_file.read(reinterpret_cast<char *>(&num_particles), sizeof(int));
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
  float buffer[9];
  if (!input_file.read(reinterpret_cast<char *>(buffer), sizeof(float) * 9)) {
    std::cerr << "Error al leer las partículas del archivo" << std::endl;
    return false;
  }

  particle.id         = index;
  particle.posX       = static_cast<double>(buffer[0]);
  particle.posY       = static_cast<double>(buffer[1]);
  particle.posZ       = static_cast<double>(buffer[2]);
  particle.smoothVecX = static_cast<double>(buffer[3]);
  particle.smoothVecY = static_cast<double>(buffer[4]);
  particle.smoothVecZ = static_cast<double>(buffer[5]);
  particle.velX       = static_cast<double>(buffer[6]);
  particle.velY       = static_cast<double>(buffer[7]);
  particle.velZ       = static_cast<double>(buffer[8]);

  return true;
}

void Grid::simulation(int iterations) {
  printParticles();
  for (int i = 0; i < iterations; i++) {
    std::cout << "Iteración: " << i << std::endl;  // Línea añadida para imprimir el valor de i

    std::cout << "Llamada a positionateParticle()" << std::endl;
    positionateParticle();
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Llamada a densityIncreaseGrid()" << std::endl;
    densityIncreaseGrid();
    std::cout << "--------------------------------" << std::endl;
    // for (auto & de : density) { cout << de << endl; }
    std::cout << "Llamada a aceletarionTransferGrid()" << std::endl;
    aceletarionTransferGrid();
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Llamada a collisionsXGrid()" << std::endl;
    collisionsXGrid();
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Llamada a collisionsYGrid()" << std::endl;
    collisionsYGrid();
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Llamada a collisionsZGrid()" << std::endl;
    collisionsZGrid();
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Llamada a particleMotionGrid()" << std::endl;
    particleMotionGrid();
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Llamada a interactionsXGrid()" << std::endl;
    interactionsXGrid();
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Llamada a interactionsYGrid()" << std::endl;
    interactionsYGrid();
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Llamada a interactionsZGrid()" << std::endl;
    interactionsZGrid();
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Llamada a printParticles()" << std::endl;
    printParticles();
    std::cout << "--------------------------------" << std::endl;

    std::cout << "Llamada a resetBlocks()" << std::endl;
    resetBlocks();
    std::cout << "--------------------------------" << std::endl;
  }
}

void Grid::printParticles() {
  std::cout << "Partículas en la cuadrícula:" << std::endl;
  for (size_t i = 0; i < particles.size(); ++i) {
    std::cout << "ID: " << particles[i].id << ", ";
    std::cout << "Posición (" << particles[i].posX << ", " << particles[i].posY << ", "
              << particles[i].posZ << "), ";
    std::cout << "Vector de suavizado (" << particles[i].smoothVecX << ", "
              << particles[i].smoothVecY << ", " << particles[i].smoothVecZ << "), ";
    std::cout << "Velocidad (" << particles[i].velX << ", " << particles[i].velY << ", "
              << particles[i].velZ << ")" << std::endl;
  }
}

void Grid::printFirst() {
  std::cout << "Partículas en la cuadrícula:" << std::endl;
  for (size_t i = 0; i < 2; ++i) {
    std::cout << "ID: " << particles[0].id << ", ";
    std::cout << "Posición (" << particles[0].posX << ", " << particles[0].posY << ", "
              << particles[0].posZ << "), ";
    std::cout << "Vector de suavizado (" << particles[0].smoothVecX << ", "
              << particles[0].smoothVecY << ", " << particles[0].smoothVecZ << "), ";
    std::cout << "Velocidad (" << particles[0].velX << ", " << particles[0].velY << ", "
              << particles[0].velZ << ")" << std::endl;
    particles[0].posX = 0.0;
  }
}

void Grid::resetBlocks() {
  for (int x = 0; x < nx; ++x) {
    for (int y = 0; y < ny; ++y) {
      for (int z = 0; z < nz; ++z) { blocks[x][y][z].resetBlock(); }
    }
  }
}

void Grid::positionateParticle() {
  for (auto & particle : particles) {
    // Obtener índices de bloque
    int i = std::max(0, std::min(nx - 1, static_cast<int>(std::floor(
                                             (particle.posX - LIMITE_INFERIOR_RECINTO_X) / sx))));
    int j = std::max(0, std::min(ny - 1, static_cast<int>(std::floor(
                                             (particle.posY - LIMITE_INFERIOR_RECINTO_Y) / sy))));
    int k = std::max(0, std::min(nz - 1, static_cast<int>(std::floor(
                                             (particle.posZ - LIMITE_INFERIOR_RECINTO_Z) / sz))));
    // Insertar partícula en el bloque correspondiente
    if (particle.id > particles.size()) { cout << "Error" << endl; }
    blocks[i][j][k].addParticle(particle.id);
  }
}

void Grid::densityIncreaseGrid() {
  for (int x = 0; x < nx; ++x) {
    for (int y = 0; y < ny; ++y) {
      for (int z = 0; z < nz; ++z) { blocks[x][y][z].densityIncreaseSingle(); }
    }
  }

  // Imprimir todas las parejas
  for (auto const & pareja : parejas_unicas) {
    int x1, y1, z1, x2, y2, z2;
    std::tie(x1, y1, z1) = pareja.first;
    std::tie(x2, y2, z2) = pareja.second;
    blocks[x1][y1][z1].densityIncrease(blocks[x2][y2][z2]);
  }
}

void Grid::aceletarionTransferGrid() {
  for (int x = 0; x < nx; ++x) {
    for (int y = 0; y < ny; ++y) {
      for (int z = 0; z < nz; ++z) { blocks[x][y][z].accelerationTransferSingle(); }
    }
  }
  for (auto const & pareja : parejas_unicas) {
    int x1, y1, z1, x2, y2, z2;
    std::tie(x1, y1, z1) = pareja.first;
    std::tie(x2, y2, z2) = pareja.second;
    blocks[x1][y1][z1].accelerationTransfer(blocks[x2][y2][z2]);
  }
}

void Grid::collisionsXGrid() {
  int cx = 0;
  for (int y = 0; y < ny; ++y) {
    for (int z = 0; z < nz; ++z) { blocks[cx][y][z].collisionsX(cx); }
  }

  cx = nx - 1;
  for (int y = 0; y < ny; ++y) {
    for (int z = 0; z < nz; ++z) { blocks[cx][y][z].collisionsX(cx); }
  }
}

void Grid::collisionsYGrid() {
  int cy = 0;
  for (int x = 0; x < nx; ++x) {
    for (int z = 0; z < nz; ++z) { blocks[x][cy][z].collisionsY(cy); }
  }

  cy = ny - 1;
  for (int x = 0; x < nx; ++x) {
    for (int z = 0; z < nz; ++z) { blocks[x][cy][z].collisionsY(cy); }
  }
}

void Grid::collisionsZGrid() {
  int cz = 0;
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) { blocks[x][y][cz].collisionsZ(cz); }
  }

  cz = nz - 1;
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) { blocks[x][y][cz].collisionsZ(cz); }
  }
}

void Grid::particleMotionGrid() {
  for (int x = 0; x < nx; ++x) {
    for (int y = 0; y < ny; ++y) {
      for (int z = 0; z < nz; ++z) { blocks[x][y][z].particleMotion(); }
    }
  }
}

void Grid::interactionsXGrid() {
  int cx = 0;
  for (int y = 0; y < ny; ++y) {
    for (int z = 0; z < nz; ++z) { blocks[cx][y][z].interactionsX(cx); }
  }

  cx = nx - 1;
  for (int y = 0; y < ny; ++y) {
    for (int z = 0; z < nz; ++z) { blocks[cx][y][z].interactionsX(cx); }
  }
}

void Grid::interactionsYGrid() {
  int cy = 0;
  for (int x = 0; x < nx; ++x) {
    for (int z = 0; z < nz; ++z) { blocks[x][cy][z].interactionsY(cy); }
  }

  cy = ny - 1;
  for (int x = 0; x < nx; ++x) {
    for (int z = 0; z < nz; ++z) { blocks[x][cy][z].interactionsY(cy); }
  }
}

void Grid::interactionsZGrid() {
  int cz = 0;
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) { blocks[x][y][cz].interactionsZ(cz); }
  }

  cz = nz - 1;
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) { blocks[x][y][cz].interactionsZ(cz); }
  }
}
