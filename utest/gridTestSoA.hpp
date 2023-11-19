#ifndef GRIDTEST_SOA
#define GRIDTEST_SOA

#include "../sim/soa/block_soa.hpp"
#include "../sim/soa/grid_soa.hpp"

#define NX 15
#define NY 21
#define NZ 15
#define NUM_PARTICLES 4800

#define ROUND_MOTION 10000000

#define SMALL_PATH "../../files/small.fld"
#define REPOS1_PATH "../../files/trz/small/repos-base-1.trz"
#define DENSINC1_PATH "../../files/trz/small/densinc-base-1.trz"
#define DENSTRANS1_PATH "../../files/trz/small/denstransf-base-1.trz"
#define ACCTRANS1_PATH "../../files/trz/small/acctransf-base-1.trz"
#define PARTCOL1_PATH "../../files/trz/small/partcol-base-1.trz"
#define MOTION_PATH "../../files/trz/small/motion-base-1.trz"
#define INTERACTIONS_PATH "../../files/trz/small/boundint-base-1.trz"

#define REPOS2_PATH "../../files/trz/small/repos-base-2.trz"
#define DENSINC2_PATH "../../files/trz/small/densinc-base-2.trz"
#define DENSTRANS1_PATH "../../files/trz/small/denstransf-base-1.trz"
#define ACCTRANS1_PATH "../../files/trz/small/acctransf-base-1.trz"
#define PARTCOL1_PATH "../../files/trz/small/partcol-base-1.trz"
#define MOTION_PATH "../../files/trz/small/motion-base-1.trz"
#define INTERACTIONS_PATH "../../files/trz/small/boundint-base-1.trz"

class GridSoATest : public GridSoA {
  public:
    void readTraze(std::string const & input_file_name);
    int readHeader(std::ifstream & file);
    void initializeGridAndBlocks();
    void readParticlesInfo(std::ifstream & file, int numBlocks);
    void readBlockInfo(std::ifstream & file, int blockIndex);
    void readParticleInfo(std::ifstream & file, int blockIndex);
    void readParticleProperties(std::ifstream & file, int64_t id);
    void addParticleToBlock(int blockIndex, int64_t id);
    std::tuple<int, int, int> obtenerIndicesDesdeId(int identificador) const;
    int compareAccelerationComponent(std::vector<double> & vec1, std::vector<double> & vec2);
    int comparePositionate(GridSoA & GridSoA);
    int compareDensity(GridSoA & GridSoA);
    int compareAccelerationX(GridSoA & GridSoA);
    int compareAccelerationY(GridSoA & GridSoA);
    int compareAccelerationZ(GridSoA & GridSoA);
    int compareParticles(GridSoA & GridSoA);
};

#endif