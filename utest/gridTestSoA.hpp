#ifndef GRIDTEST_SOA
#define GRIDTEST_SOA

#include "../sim/soa/block_soa.hpp"
#include "../sim/soa/grid_soa.hpp"

#include <cstdint>

enum : std::uint16_t { NX = 15, NY = 21, NZ = 15, NUM_PARTICLES = 4800 };

constexpr int ROUND_MOTION = 10000000;

constexpr char const * SMALL_PATH         = "../../files/small.fld";
constexpr char const * REPOS1_PATH        = "../../files/trz/small/repos-base-1.trz";
constexpr char const * DENSINC1_PATH      = "../../files/trz/small/densinc-base-1.trz";
constexpr char const * DENSTRANS1_PATH    = "../../files/trz/small/denstransf-base-1.trz";
constexpr char const * ACCTRANS1_PATH     = "../../files/trz/small/acctransf-base-1.trz";
constexpr char const * PARTCOL1_PATH      = "../../files/trz/small/partcol-base-1.trz";
constexpr char const * MOTION_PATH1       = "../../files/trz/small/motion-base-1.trz";
constexpr char const * INTERACTIONS_PATH1 = "../../files/trz/small/boundint-base-1.trz";

constexpr char const * REPOS2_PATH        = "../../files/trz/small/repos-base-2.trz";
constexpr char const * DENSINC2_PATH      = "../../files/trz/small/densinc-base-2.trz";
constexpr char const * DENSTRANS2_PATH    = "../../files/trz/small/denstransf-base-2.trz";
constexpr char const * ACCTRANS2_PATH     = "../../files/trz/small/acctransf-base-2.trz";
constexpr char const * PARTCOL2_PATH      = "../../files/trz/small/partcol-base-2.trz";
constexpr char const * MOTION_PATH2       = "../../files/trz/small/motion-base-2.trz";
constexpr char const * INTERACTIONS_PATH2 = "../../files/trz/small/boundint-base-2.trz";

class GridSoATest : public GridSoA {
  public:
    void readTraze(std::string const & input_file_name);
    static int readHeader(std::ifstream & file);
    void initializeGridAndBlocks();
    void readParticlesInfo(std::ifstream & file, int numBlocks);
    void readBlockInfo(std::ifstream & file, int blockIndex);
    void readParticleInfo(std::ifstream & file, int blockIndex);
    void readParticleProperties(std::ifstream & file, int64_t id);
    void addParticleToBlock(int blockIndex, int64_t id);
    [[nodiscard]] std::tuple<int, int, int> obtenerIndicesDesdeId(int identificador) const;
    int compareAccelerationComponent(std::vector<double> & vec1, std::vector<double> & vec2);
    int comparePositionate(GridSoA & GridSoA);
    int compareDensity(GridSoA & GridSoA);
    int compareAccelerationX(GridSoA & GridSoA);
    int compareAccelerationY(GridSoA & GridSoA);
    int compareAccelerationZ(GridSoA & GridSoA);
    int compareParticles(GridSoA & GridSoA);
};

#endif