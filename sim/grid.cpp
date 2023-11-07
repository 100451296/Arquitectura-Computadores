#include <grid.hpp>

using namespace std;

int grid() {
  // Código de las pruebas unitarias
  return 0;
}

void Grid::simulation(int iterations) {
  for (int i = 0; i < iterations; i++) {
    std::cout << "ID: " << particles[0]->id << ", ";
    std::cout << "Posición (" << particles[0]->posX << ", " << particles[0]->posY << ", "
              << particles[0]->posZ << "), ";
    std::cout << "Vector de suavizado (" << particles[0]->smoothVecX << ", "
              << particles[0]->smoothVecY << ", " << particles[0]->smoothVecZ << "), ";
    std::cout << "Velocidad (" << particles[0]->velX << ", " << particles[0]->velY << ", "
              << particles[0]->velZ << ")" << std::endl;

    positionateParticle();
    densityIncreaseGrid();
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
  for (size_t i = 0; i < particles.size(); ++i) {
    std::cout << "ID: " << particles[i]->id << ", ";
    std::cout << "Posición (" << particles[i]->posX << ", " << particles[i]->posY << ", "
              << particles[i]->posZ << "), ";
    std::cout << "Vector de suavizado (" << particles[i]->smoothVecX << ", "
              << particles[i]->smoothVecY << ", " << particles[i]->smoothVecZ << "), ";
    std::cout << "Velocidad (" << particles[i]->velX << ", " << particles[i]->velY << ", "
              << particles[i]->velZ << ")" << std::endl;
  }
}

void Grid::printFirst() {
  std::cout << "Partículas en la cuadrícula:" << std::endl;
  for (size_t i = 0; i < 2; ++i) {
    std::cout << "ID: " << particles[0]->id << ", ";
    std::cout << "Posición (" << particles[0]->posX << ", " << particles[0]->posY << ", "
              << particles[0]->posZ << "), ";
    std::cout << "Vector de suavizado (" << particles[0]->smoothVecX << ", "
              << particles[0]->smoothVecY << ", " << particles[0]->smoothVecZ << "), ";
    std::cout << "Velocidad (" << particles[0]->velX << ", " << particles[0]->velY << ", "
              << particles[0]->velZ << ")" << std::endl;
    particles[0]->posX = 0.0;
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
                                             (particle->posX - LIMITE_INFERIOR_RECINTO_X) / sx))));
    int j = std::max(0, std::min(ny - 1, static_cast<int>(std::floor(
                                             (particle->posY - LIMITE_INFERIOR_RECINTO_Y) / sy))));
    int k = std::max(0, std::min(nz - 1, static_cast<int>(std::floor(
                                             (particle->posZ - LIMITE_INFERIOR_RECINTO_Z) / sz))));
    // Insertar partícula en el bloque correspondiente
    // cout << "Particula " << particle->id << " - Bloque " << i << "-" << j << "-" << k << endl;
    blocks[i][j][k].addParticle(particle);
  }
}

void Grid::densityIncreaseGrid() {
  for (int x = 0; x < nx; ++x) {
    for (int y = 0; y < ny; ++y) {
      for (int z = 0; z < nz; ++z) { blocks[x][y][z].densityIncreaseSingle(); }
    }
  }
  for (auto & pareja : parejas_unicas) {
    pareja.first.densityIncrease(pareja.second);  // Bloques contiguos
  }
}

void Grid::aceletarionTransferGrid() {
  for (int x = 0; x < nx; ++x) {
    for (int y = 0; y < ny; ++y) {
      for (int z = 0; z < nz; ++z) { blocks[x][y][z].accelerationTransferSingle(); }
    }
  }
  for (auto & pareja : parejas_unicas) {
    pareja.first.accelerationTransfer(pareja.second);
  }  // Bloques contiguos
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
