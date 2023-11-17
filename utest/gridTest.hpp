#include "../sim/block.hpp"
#include "../sim/grid.hpp"

#define NX 15
#define NY 21
#define NZ 15
#define NUM_PARTICLES 4800

#define SMALL_PATH "../../files/small.fld"
#define REPOS1_PATH "../../files/trz/small/repos-base-1.trz"
#define DENSITY1_PATH "../../files/trz/small/denstransf-base-1.trz"

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
};
