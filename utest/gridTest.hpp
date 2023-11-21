#ifndef GRIDTEST
#define GRIDTEST

#include "../sim/block.hpp"
#include "../sim/grid.hpp"

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
#define INITACC2_PATH "../../files/trz/small/initacc-base-2.trz"

class GridTest : public Grid {
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
    int comparePositionate(Grid & grid);
    int compareDensity(Grid & grid);
    int compareAccelerationX(Grid & grid);
    int compareAccelerationY(Grid & grid);
    int compareAccelerationZ(Grid & grid);
    int compareParticles(Grid & grid);
};

#endif