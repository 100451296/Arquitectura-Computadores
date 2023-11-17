#ifndef BLOCK_TEST_HPP
#define BLOCK_TEST_HPP

#include "../sim/block.hpp"

#include <gtest/gtest.h>

class TestBloque_MetodosBloque_Test : public testing::Test {
  protected:
    // Variables miembro
    std::vector<Block> blocks;
    std::vector<Particle> particles;
    std::vector<double> accelerationX, accelerationY, accelerationZ, density;
    // Se ejecuta antes de cada prueba
    void SetUp() override;
};

#endif  // BLOCK_TEST_HPP