#include "../sim/grid.hpp"
#include "gridTest.hpp"

#include <gtest/gtest.h>

// Define una prueba trivial que verifica la igualdad de dos números
TEST(Positionate, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze("/home/edu/Escritorio/Uni/4Cuarto/1Cuatri/Arquitectura-Computadores/proyecto/"
                     "Arquitectura-Computadores/files/trz/small/repos-base-1.trz");

  Grid grid;
  grid.readFile("/home/edu/Escritorio/Uni/4Cuarto/1Cuatri/Arquitectura-Computadores/proyecto/"
                "Arquitectura-Computadores/files/small.fld");
  grid.positionateParticle();

  int result = gridTest.comparePositionate(grid);
  // Verifica que a sea igual a b
  EXPECT_EQ(result, 0);
}

// Define una prueba trivial que verifica la igualdad de dos números
TEST(DensityIncrease, TestGrid) {
  GridTest gridTest;
  gridTest.readTraze("/home/edu/Escritorio/Uni/4Cuarto/1Cuatri/Arquitectura-Computadores/proyecto/"
                     "Arquitectura-Computadores/files/trz/small/repos-base-1.trz");

  Grid grid;
  grid.readFile("/home/edu/Escritorio/Uni/4Cuarto/1Cuatri/Arquitectura-Computadores/proyecto/"
                "Arquitectura-Computadores/files/small.fld");
  grid.positionateParticle();

  int result = gridTest.comparePositionate(grid);
  // Verifica que a sea igual a b
  EXPECT_EQ(result, 0);
}
