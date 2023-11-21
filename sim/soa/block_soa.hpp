#ifndef BLOCK_SOA
#define BLOCK_SOA

#include "../common.hpp"

#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

class Block {
    // Representa un bloque en la malla. Realizará todos los cálculos entre
    // partículas

  public:
    static constexpr double TenLessTwuelve   = 1e-12;
    static constexpr double Epsilon          = 1e-10;
    static constexpr double Two              = 2;
    static constexpr double Three            = 3;
    static constexpr double Six              = 6;
    static constexpr double Fifteen          = 15;
    static constexpr double FortyFive        = 45;
    static constexpr double SixtyFour        = 64;
    static constexpr double ThreeHundFifteen = 315;

    Block(Particles & particlesRef, std::vector<double> & densityRef,
          std::vector<double> & accelerationXRef, std::vector<double> & accelerationYRef,
          std::vector<double> & accelerationZRef)
      : particles(&particlesRef), particlesID(), data(), accelerationX(&accelerationXRef),
        accelerationY(&accelerationYRef), accelerationZ(&accelerationZRef), density(&densityRef) { }

    Particles * particles;
    std::vector<int> particlesID;
    std::vector<std::pair<int, int>> particlePairs;
    DataCommon data;
    std::vector<double> * accelerationX;
    std::vector<double> * accelerationY;
    std::vector<double> * accelerationZ;
    std::vector<double> * density;

    void addParticle(int id);
    void resetBlock();
    void densityIncreaseSingle();
    void densityIncrease(Block & contiguousBlock);
    void accelerationTransferSingle();
    void accelerationTransfer(Block & contiguousBlock);
    void collisionsX(unsigned int cx);
    void collisionsY(unsigned int cy);
    void collisionsZ(unsigned int cz);
    void particleMotion();
    void interactionsX(unsigned int cx);
    void interactionsY(unsigned int cy);
    void interactionsZ(unsigned int cz);
    void generarParejasBloque();
    void generarParejasEntreBloques(Block & otherBlock, std::vector<std::pair<int, int>> & aux);
    void calculateDataCommon();
    void initDensityAcceleration();
    void calculate_increm_density(std::vector<std::pair<int, int>> ParejaParticulas) const;
    void lineal_transformate_density();
    std::vector<double> calculate_increm_aceleration(std::vector<double> position,
                                                     std::vector<double> velocity, double dist,
                                                     std::vector<int> Id) const;
    static double calculate_dist(double posX, double posY, double posZ);
    void accelerationTransferCalculations(std::vector<std::pair<int, int>> & pair_vec)const;
    bool distClose(int idP1, int idP2)const;
    void updateAcceleration(int p1, int p2, std::vector<double> & increm_aceleration)const;
};

#endif
