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
    particle.posX       = (1 / i) * LIMITE_SUPERIOR_RECINTO_X - 0.01;
    particle.posY       = (1 / i) * LIMITE_SUPERIOR_RECINTO_Y - 0.01;
    particle.posZ       = (1 / i) * LIMITE_SUPERIOR_RECINTO_Z - 0.01;
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

  // Rellenar los datos comunes
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
}

// Caso de prueba para el método generarParejasBloque
TEST_F(TestBloque_MetodosBloque_Test, GenerarParejasBloque) {
  // Llamamos al método para generar parejas en el bloque 1
  blocks[0].generarParejasBloque();
  int N = blocks[0].particlesID.size();

  // Verificamos que el número de parejas generadas sea el esperado
  EXPECT_EQ(blocks[0].particlePairs.size(), (N * (N - 1)) / 2);

  // Verificamos que todas las parejas sean únicas
  std::set<std::pair<int, int>> uniquePairs(blocks[0].particlePairs.begin(),
                                            blocks[0].particlePairs.end());
  EXPECT_EQ(uniquePairs.size(), blocks[0].particlePairs.size());
}

// Caso de prueba para el método generarParejasEntreBloques
TEST_F(TestBloque_MetodosBloque_Test, GenerarParejasEntreBloques) {
  // Llamamos al método para generar parejas entre los bloques 1 y 2
  std::vector<std::pair<int, int>> auxPairs;
  blocks[0].generarParejasEntreBloques(blocks[1], auxPairs);
  int N = blocks[0].particlesID.size();
  int M = blocks[1].particlesID.size();

  // Verificamos que el número de parejas generadas sea el esperado
  EXPECT_EQ(auxPairs.size(), N * M);  // Ambos bloques tienen 5 partículas cada uno

  // Verificamos que todas las parejas sean únicas
  std::set<std::pair<int, int>> uniquePairs(auxPairs.begin(), auxPairs.end());
  EXPECT_EQ(uniquePairs.size(), auxPairs.size());
}

// Caso de prueba para el método addParticle
TEST_F(TestBloque_MetodosBloque_Test, AddParticle) {
  // Guardamos el size antes de añadir la particula
  int tam = blocks[0].particlesID.size();

  // Guardamos el ID de la particula a insertar
  int id = 11;

  // Llamamos al método para agregar una partícula al bloque 1
  blocks[0].addParticle(id);  // Puedes ajustar el ID de la partícula según sea necesario

  // Verificamos que la partícula se haya agregado correctamente
  EXPECT_EQ(blocks[0].particlesID.size(),
            tam + 1);  // El bloque 1 debería tener 6 partículas después de agregar una más
  EXPECT_EQ(blocks[0].particlesID.back(),
            id);  // El ID de la última partícula en el bloque debería ser 11
}

// Caso de prueba para la función resetBlock
TEST_F(TestBloque_MetodosBloque_Test, ResetBlock) {
  // Generamos algunas parejas en el bloque
  blocks[0].generarParejasBloque();

  // Verificamos que el bloque tiene partículas y parejas antes del reset
  EXPECT_FALSE(blocks[0].particlesID.empty());
  EXPECT_FALSE(blocks[0].particlePairs.empty());

  // Reseteamos el bloque
  blocks[0].resetBlock();

  // Verificamos que el bloque está vacío después del reset
  EXPECT_TRUE(blocks[0].particlesID.empty());
  EXPECT_TRUE(blocks[0].particlePairs.empty());
}

// Caso de prueba para la función initDensityAcceleration
TEST_F(TestBloque_MetodosBloque_Test, InitDensityAcceleration) {
  // Inicializamos la densidad y la aceleración en el bloque
  blocks[0].initDensityAcceleration();

  // Verificamos que la densidad y la aceleración se han inicializado correctamente
  for (auto & id : blocks[0].particlesID) {
    EXPECT_EQ(blocks[0].density[id], 0.0);
    EXPECT_EQ(blocks[0].accelerationX[id], ACELERACION_GRAVEDAD_X);
    EXPECT_EQ(blocks[0].accelerationY[id], ACELERACION_GRAVEDAD_Y);
    EXPECT_EQ(blocks[0].accelerationZ[id], ACELERACION_GRAVEDAD_Z);
  }
}

// Caso de prueba para la función densityIncreaseSingle
TEST_F(TestBloque_MetodosBloque_Test, DensityIncreaseSingle) {
  // Generamos algunas parejas en el bloque
  blocks[0].generarParejasBloque();

  // Inicializamos la densidad y la aceleración en el bloque
  blocks[0].initDensityAcceleration();

  // Llamamos al método densityIncreaseSingle
  blocks[0].densityIncreaseSingle();

  // Verificamos que la densidad se ha incrementado correctamente
  for (auto & pair : blocks[0].particlePairs) {
    double aux_x = std::pow(particles[pair.first].posX - particles[pair.second].posX, 2);
    double aux_y = std::pow(particles[pair.first].posY - particles[pair.second].posY, 2);
    double aux_z = std::pow(particles[pair.first].posZ - particles[pair.second].posZ, 2);

    if (aux_x + aux_y + aux_z < std::pow(data.long_suavizado, 2)) {
      EXPECT_GT(blocks[0].density[pair.first], 0.0);
      EXPECT_GT(blocks[0].density[pair.second], 0.0);
    } else {
      EXPECT_EQ(blocks[0].density[pair.first], 0.0);
      EXPECT_EQ(blocks[0].density[pair.second], 0.0);
    }
  }
}
