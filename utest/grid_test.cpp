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
  gridTest.readTraze(DENSINC1_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.positionateParticle();
  grid.densityIncreaseGrid();

  int result = gridTest.compareDensity(grid);
  // Verifica que a sea igual a b
  EXPECT_EQ(result, 0);
}

// Define una prueba trivial que verifica la igualdad de dos números
TEST(LinearTransfrom, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze(DENSTRANS1_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.positionateParticle();
  grid.densityIncreaseGrid();
  grid.linealDensityTransform();

  int result = gridTest.compareDensity(grid);
  // Verifica que a sea igual a b
  EXPECT_EQ(result, 0);
}

// Define una prueba trivial que verifica la igualdad de dos números
TEST(AccelerationTransfer, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze(ACCTRANS1_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.positionateParticle();
  grid.densityIncreaseGrid();
  grid.linealDensityTransform();
  grid.aceletarionTransferGrid();

  EXPECT_EQ(gridTest.compareAccelerationX(grid), 0);
  EXPECT_EQ(gridTest.compareAccelerationY(grid), 0);
  EXPECT_EQ(gridTest.compareAccelerationZ(grid), 0);
}

// Define una prueba trivial que verifica la igualdad de dos números
TEST(CollisionsGrid, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze(PARTCOL1_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.positionateParticle();
  grid.densityIncreaseGrid();
  grid.linealDensityTransform();
  grid.aceletarionTransferGrid();
  grid.collisionsXGrid();
  grid.collisionsYGrid();
  grid.collisionsZGrid();

  EXPECT_EQ(gridTest.compareAccelerationX(grid), 0);
  EXPECT_EQ(gridTest.compareAccelerationY(grid), 0);
  EXPECT_EQ(gridTest.compareAccelerationZ(grid), 0);
}

// Define una prueba trivial que verifica la igualdad de dos números
TEST(Motion, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze(MOTION_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.positionateParticle();
  grid.densityIncreaseGrid();
  grid.linealDensityTransform();
  grid.aceletarionTransferGrid();
  grid.collisionsXGrid();
  grid.collisionsYGrid();
  grid.collisionsZGrid();
  grid.particleMotionGrid();

  EXPECT_EQ(gridTest.compareParticles(grid), 0);
}

// Define una prueba trivial que verifica la igualdad de dos números
TEST(Interactions, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze(INTERACTIONS_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.positionateParticle();
  grid.densityIncreaseGrid();
  grid.linealDensityTransform();
  grid.aceletarionTransferGrid();
  grid.collisionsXGrid();
  grid.collisionsYGrid();
  grid.collisionsZGrid();
  grid.particleMotionGrid();
  grid.interactionsXGrid();
  grid.interactionsYGrid();
  grid.interactionsZGrid();

  EXPECT_EQ(gridTest.compareParticles(grid), 0);
}

// Define una prueba trivial que verifica la igualdad de dos números
TEST(Positionate2, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze(REPOS2_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.simulation(1);
  grid.positionateParticle();

  int result = gridTest.comparePositionate(grid);
  // Verifica que a sea igual a b
  EXPECT_EQ(result, 0);
}

// Define una prueba trivial que verifica la igualdad de dos números
TEST(DensityIncrease2, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze(DENSINC2_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.simulation(1);
  grid.positionateParticle();
  grid.densityIncreaseGrid();

  EXPECT_EQ(gridTest.compareDensity(grid), 0);
}

// Define una prueba trivial que verifica la igualdad de dos números
TEST(InitAcc2, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze(INITACC2_PATH);

  Grid grid;
  grid.readFile(SMALL_PATH);
  grid.simulation(1);

  EXPECT_EQ(gridTest.compareDensity(grid), 0);
  EXPECT_EQ(gridTest.compareAccelerationX(grid), 0);
  EXPECT_EQ(gridTest.compareAccelerationY(grid), 0);
  EXPECT_EQ(gridTest.compareAccelerationZ(grid), 0);
  EXPECT_EQ(gridTest.compareParticles(grid), 0);
}
