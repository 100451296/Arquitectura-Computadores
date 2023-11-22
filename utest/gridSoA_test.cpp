#include "gridTestSoA.hpp"

#include <gtest/gtest.h>

#define SMALL_PATH "../../files/small.fld"

// Define una prueba trivial que verifica la igualdad de dos números
TEST(Prueba, GridTestSoA) {
  GridSoATest gridTest;
  gridTest.readTraze(REPOS1_PATH);

  GridSoA grid;
  grid.readFile(SMALL_PATH);
  grid.positionateParticle();

  int const result = gridTest.comparePositionate(grid);
  // Verifica que a sea igual a b
  EXPECT_EQ(result, 0);
}
