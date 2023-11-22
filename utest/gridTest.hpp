#ifndef GRIDTEST
#define GRIDTEST

#include "../sim/block.hpp"
#include "../sim/grid.hpp"

#include <cstdint>
#include <string>

enum : std::uint16_t { NX = 15, NY = 21, NZ = 15, NUM_PARTICLES = 4800 };

constexpr int ROUND_MOTION       = 10000000;
constexpr int ROUND_DENSITY      = 100;
constexpr int ROUND_ACCELERATION = 100;

constexpr char const * SMALL_PATH        = "../../files/small.fld";
constexpr char const * REPOS1_PATH       = "../../files/trz/small/repos-base-1.trz";
constexpr char const * DENSINC1_PATH     = "../../files/trz/small/densinc-base-1.trz";
constexpr char const * DENSTRANS1_PATH   = "../../files/trz/small/denstransf-base-1.trz";
constexpr char const * ACCTRANS1_PATH    = "../../files/trz/small/acctransf-base-1.trz";
constexpr char const * PARTCOL1_PATH     = "../../files/trz/small/partcol-base-1.trz";
constexpr char const * MOTION_PATH       = "../../files/trz/small/motion-base-1.trz";
constexpr char const * INTERACTIONS_PATH = "../../files/trz/small/boundint-base-1.trz";

constexpr char const * REPOS2_PATH        = "../../files/trz/small/repos-base-2.trz";
constexpr char const * DENSINC2_PATH      = "../../files/trz/small/densinc-base-2.trz";
constexpr char const * DENSTRANS2_PATH    = "../../files/trz/small/denstransf-base-2.trz";
constexpr char const * ACCTRANS2_PATH     = "../../files/trz/small/acctransf-base-2.trz";
constexpr char const * PARTCOL2_PATH      = "../../files/trz/small/partcol-base-2.trz";
constexpr char const * MOTION2_PATH       = "../../files/trz/small/motion-base-2.trz";
constexpr char const * INTERACTIONS2_PATH = "../../files/trz/small/boundint-base-2.trz";
constexpr char const * INITACC2_PATH      = "../../files/trz/small/initacc-base-2.trz";

class GridTest : public Grid {
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
    static int compareAccelerationComponent(std::vector<double> & vec1, std::vector<double> & vec2);
    int comparePositionate(Grid & grid);
    int compareDensity(Grid & grid);
    int compareAccelerationX(Grid & grid);
    int compareAccelerationY(Grid & grid);
    int compareAccelerationZ(Grid & grid);
    int compareParticles(Grid & grid);
    static bool areEqualWithTolerance(double value1, double value2);
    static void printDifferentAttributes(double value1, std::string const & attributeName,
                                         double value2);
    [[nodiscard]] static bool areParticlesDifferent(Particle const & particle1,
                                             Particle const & particle2);
    static void printDifferentParticlesDetails(Particle const & particle1,
                                        Particle const & particle2);
};

#endif