#include <gtest/gtest.h>

int main(int argc, char** argv) {
  // Inicializa Google Test
  ::testing::InitGoogleTest(&argc, argv);

  // Ejecuta todas las pruebas
  return RUN_ALL_TESTS();
}
