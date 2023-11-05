#ifndef COMMON
#define COMMON

#define MULTIPLICADOR_RADIO 1.695
#define DENSIDAD_FLUIDO 10e3
#define PRESION_RIGIDEZ 3.0
#define COLISIONES_RIGIDEZ 3e4
#define AMORTIGUAMIENTO 128.0
#define VISCOSIDAD 0.4
#define TAMANO_PARTICULA 2e-4
#define PASO_TIEMPO 1e-3

#define ACELERACION_GRAVEDAD_X 0.0
#define ACELERACION_GRAVEDAD_Y 9.8
#define ACELERACION_GRAVEDAD_Z 0.0

#define LIMITE_SUPERIOR_RECINTO_X 0.065
#define LIMITE_SUPERIOR_RECINTO_Y 0.1
#define LIMITE_SUPERIOR_RECINTO_Z 0.065
#define LIMITE_INFERIOR_RECINTO_X -0.065
#define LIMITE_INFERIOR_RECINTO_Y -0.08
#define LIMITE_INFERIOR_RECINTO_Z -0.065

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
} typedef Particle;

struct DataCommon {
    double ppm;
    double mass;
    double long_suavizado;
    unsigned int nx;
    unsigned int ny;
    unsigned int nz;
} typedef DataCommon;

#endif