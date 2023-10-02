#include <gtest/gtest.h>

// Define una prueba trivial que verifica la igualdad de dos números
TEST(TestPrueba, EqualityTest) {
  int a = 42;
  int b = 42;

  // Verifica que a sea igual a b
  EXPECT_EQ(a, b);
}