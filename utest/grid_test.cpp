#include "../sim/grid.hpp"
#include "gridTest.hpp"

#include <gtest/gtest.h>

// Define una prueba trivial que verifica la igualdad de dos números
TEST(Positionate, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze(REPOS1_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.positionateParticle();

  int result = gridTest.comparePositionate(grid);
  // Verifica que a sea igual a b
  EXPECT_EQ(result, 0);
}

// Define una prueba trivial que verifica la igualdad de dos números
TEST(DensityIncrease, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze(DENSITY1_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.positionateParticle();
  grid.densityIncreaseGrid();
  grid.linealDensityTransform();

  int result = gridTest.compareDensity(grid);
  // Verifica que a sea igual a b
  EXPECT_EQ(result, 0);
}
