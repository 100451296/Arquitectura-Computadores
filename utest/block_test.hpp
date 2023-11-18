#ifndef BLOCK_TEST_HPP
#define BLOCK_TEST_HPP

#include "../sim/block.hpp"

#include <gtest/gtest.h>

class TestBloque_MetodosBloque_Test : public testing::Test {
  protected:
    // Variables miembro
    std::vector<Block> & blocks;
    std::vector<Particle> & particles;
    std::vector<int> particlesID;
    std::vector<std::pair<int, int>> particlePairs;
    std::vector<double> accelerationX, accelerationY, accelerationZ, density;
    DataCommon data;
    double ppm;
    int num_particles;
    double h;
    double particle_mass;
    int nx;
    int ny;
    int nz;
    int num_blocks;
    double sx;
    double sy;
    double sz;
    // Se ejecuta antes de cada prueba
    void SetUp() override;
};

#endif  // BLOCK_TEST_HPP