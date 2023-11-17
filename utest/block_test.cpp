#include "block_test.hpp"

// Se ejecuta antes de cada prueba
void TestBloque_MetodosBloque_Test::SetUp() {
  // Rellenar vectores de aceleración y densidad
  for (unsigned int i = 0; i < 10; ++i) {
    // Rellenar valores según sea necesario
    accelerationX.push_back(i * 0.1);
    accelerationY.push_back(i * 0.2);
    accelerationZ.push_back(i * 0.3);
    density.push_back(i * 0.01);
  }

  // Crear el primer bloque
  blocks.push_back(Block(particles, accelerationX, accelerationY, accelerationZ, density));

  // Crear el segundo bloque
  blocks.push_back(Block(particles, accelerationX, accelerationY, accelerationZ, density));

  // Asignar partículas a cada bloque
  for (unsigned int i = 1; i < 11; ++i) {
    Particle particle;
    particle.id = i;
    // Rellenar otros atributos según sea necesario
    particle.posX       = (1/i) * LIMITE_SUPERIOR_RECINTO_X - 0.01;
    particle.posY       = (1/i) * LIMITE_SUPERIOR_RECINTO_Y - 0.01;
    particle.posZ       = (1/i) * LIMITE_SUPERIOR_RECINTO_Z - 0.01;
    particle.smoothVecX = i * 0.5;
    particle.smoothVecY = i * 0.7;
    particle.smoothVecZ = i * 0.3;
    particle.velX       = i * 1.5;
    particle.velY       = i * 2.5;
    particle.velZ       = i * 3.5;

    particles.push_back(particle);

    // Asignar partículas a los bloques
    if (i < 6) {
      blocks[0].addParticle(i);
    } else {
      blocks[1].addParticle(i);
    }
  }
}

// Caso de prueba para el método generarParejasBloque
TEST_F(TestBloque_MetodosBloque_Test, GenerarParejasBloque) {
  // Llamamos al método para generar parejas en el bloque 1
  blocks[0].generarParejasBloque();

  // Verificamos que el número de parejas generadas sea el esperado
  EXPECT_EQ(blocks[0].particlePairs.size(), 10 * 4 / 2);

  // Verificamos que todas las parejas sean únicas
  std::set<std::pair<int, int>> uniquePairs(blocks[0].particlePairs.begin(), blocks[0].particlePairs.end());
  EXPECT_EQ(uniquePairs.size(), blocks[0].particlePairs.size());
}

// Caso de prueba para el método generarParejasEntreBloques
TEST_F(TestBloque_MetodosBloque_Test, GenerarParejasEntreBloques) {
  // Llamamos al método para generar parejas entre los bloques 1 y 2
  std::vector<std::pair<int, int>> auxPairs;
  blocks[0].generarParejasEntreBloques(blocks[1], auxPairs);

  // Verificamos que el número de parejas generadas sea el esperado
  EXPECT_EQ(auxPairs.size(), 5 * 5); // Ambos bloques tienen 5 partículas cada uno

  // Verificamos que todas las parejas sean únicas
  std::set<std::pair<int, int>> uniquePairs(auxPairs.begin(), auxPairs.end());
  EXPECT_EQ(uniquePairs.size(), auxPairs.size());
}
