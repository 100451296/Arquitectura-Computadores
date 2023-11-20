#ifndef COMMON
#define COMMON

constexpr double MULTIPLICADOR_RADIO = 1.695;
constexpr double DENSIDAD_FLUIDO = 1e3;
constexpr double PRESION_RIGIDEZ = 3.0;
constexpr double COLISIONES_RIGIDEZ = 3e4;
constexpr double AMORTIGUAMIENTO = 128.0;
constexpr double VISCOSIDAD = 0.4;
constexpr double TAMANO_PARTICULA = 2e-4;
constexpr double PASO_TIEMPO = 1e-3;

constexpr double ACELERACION_GRAVEDAD_X = 0.0;
constexpr double ACELERACION_GRAVEDAD_Y = -9.8;
constexpr double ACELERACION_GRAVEDAD_Z = 0.0;

constexpr double LIMITE_SUPERIOR_RECINTO_X = 0.065;
constexpr double LIMITE_SUPERIOR_RECINTO_Y = 0.1;
constexpr double LIMITE_SUPERIOR_RECINTO_Z = 0.065;
constexpr double LIMITE_INFERIOR_RECINTO_X = -0.065;
constexpr double LIMITE_INFERIOR_RECINTO_Y = -0.08;
constexpr double LIMITE_INFERIOR_RECINTO_Z = -0.065;

#include <cmath>
#include <vector>

struct Particle {
    unsigned int id;
    double posX;
    double posY;
    double posZ;
    double smoothVecX;
    double smoothVecY;
    double smoothVecZ;
    double velX;
    double velY;
    double velZ;
};

// Definición de la estructura SoA
struct Particles {
    std::vector<unsigned int> id;
    std::vector<double> posX;
    std::vector<double> posY;
    std::vector<double> posZ;
    std::vector<double> smoothVecX;
    std::vector<double> smoothVecY;
    std::vector<double> smoothVecZ;
    std::vector<double> velX;
    std::vector<double> velY;
    std::vector<double> velZ;

    // Método para cambiar el tamaño de todos los vectores
    void resize(int newSize) {
      id.resize(newSize);
      posX.resize(newSize);
      posY.resize(newSize);
      posZ.resize(newSize);
      smoothVecX.resize(newSize);
      smoothVecY.resize(newSize);
      smoothVecZ.resize(newSize);
      velX.resize(newSize);
      velY.resize(newSize);
      velZ.resize(newSize);
    }
};

struct DataCommon {
    double ppm;
    double mass;
    double long_suavizado;
    double h_square;
    double h_pow6;
    double h_pow9;
    unsigned int nx;
    unsigned int ny;
    unsigned int nz;
};

#endif