#include "../sim/block.hpp"
#include "../sim/grid.hpp"

#define NX 22
#define NY 31
#define NZ 22
#define NUM_PARTICLES 15138

#define ROUND_MOTION 10000000

#define SMALL_PATH "../../files/large.fld"
#define REPOS1_PATH "../../files/trz/large/repos-base-1.trz"
#define DENSINC1_PATH "../../files/trz/large/densinc-base-1.trz"
#define DENSTRANS1_PATH "../../files/trz/large/denstransf-base-1.trz"
#define ACCTRANS1_PATH "../../files/trz/large/acctransf-base-1.trz"
#define PARTCOL1_PATH "../../files/trz/large/partcol-base-1.trz"
#define MOTION_PATH "../../files/trz/large/motion-base-1.trz"
#define INTERACTIONS_PATH "../../files/trz/large/boundint-base-1.trz"

#define REPOS1_PATH "../../files/trz/large/repos-base-1.trz"
#define DENSINC1_PATH "../../files/trz/large/densinc-base-1.trz"
#define DENSTRANS1_PATH "../../files/trz/large/denstransf-base-1.trz"
#define ACCTRANS1_PATH "../../files/trz/large/acctransf-base-1.trz"
#define PARTCOL1_PATH "../../files/trz/large/partcol-base-1.trz"
#define MOTION_PATH "../../files/trz/large/motion-base-1.trz"
#define INTERACTIONS_PATH "../../files/trz/large/boundint-base-1.trz"

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
    int comparePositionate(Grid & grid);
    int compareDensity(Grid & grid);
    int compareAccelerationX(Grid & grid);
    int compareAccelerationY(Grid & grid);
    int compareAccelerationZ(Grid & grid);
    int compareParticles(Grid & grid);

  private:
    int compareAccelerationComponent(std::vector<double> & vec1, std::vector<double> & vec2);
};
