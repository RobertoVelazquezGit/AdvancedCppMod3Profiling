/*
You're optimizing a graphics rendering engine that processes large matrices.
 Analyze how memory access patterns affect performance through cache behavior profiling.
In the code below, analyze memory access pattern performance:
Compare row-major vs column-major matrix traversal performance
Test Array of Structures vs Structure of Arrays for particle processing
Use different matrix sizes to see when cache effects become significant
Measure performance differences and correlate with cache behavior
Validate that both approaches produce identical numerical results
*/

#include <vector>
#include <iostream>
#include <random>

class CachePerformanceAnalyzer {
private:
    static constexpr unsigned randomSeed = 12345;

public:
    // Matrix operations with different access patterns
    static long matrixSumRowMajor(const std::vector<std::vector<int>>& matrix) {
        long sum = 0;
        for (size_t row = 0; row < matrix.size(); ++row) {
            for (size_t col = 0; col < matrix[row].size(); ++col) {
                sum += matrix[row][col];  // Cache-friendly access
            }
        }
        return sum;
    }    
    
    static long matrixSumColumnMajor(const std::vector<std::vector<int>>& matrix) {
        // Column-major traversal is less cache-friendly because each row is a
        // separate allocation and consecutive accesses jump between rows.
        size_t columnCount = 0;
        for (const auto& row : matrix) {
            if (row.size() > columnCount) {
                columnCount = row.size();
            }
        }

        long sum = 0;
        for (size_t col = 0; col < columnCount; ++col) {
            for (size_t row = 0; row < matrix.size(); ++row) {
                if (col < matrix[row].size()) {
                    sum += matrix[row][col];
                }
            }
        }

        return sum;
    }
    
    // Structure of Arrays vs Array of Structures comparison
    struct Particle {
        double x, y, z;    // Position
        double vx, vy, vz; // Velocity
        double mass;
        int id;
    };
    
    class ParticleSystemAoS {
        std::vector<Particle> particles;
        
    public:
        void resize(size_t count) { particles.resize(count); }

        void setParticle(size_t index, const Particle& particle) {
            particles[index] = particle;
        }
        
        double calculateKineticEnergy() {
            // AoS is less cache-friendly here because each particle contains
            // position and ID fields that are not needed for this calculation.
            double totalEnergy = 0.0;
            for (const Particle& particle : particles) {
                const double velocitySquared = particle.vx * particle.vx +
                                               particle.vy * particle.vy +
                                               particle.vz * particle.vz;
                totalEnergy += 0.5 * particle.mass * velocitySquared;
            }

            return totalEnergy;
            }
    };
    
    class ParticleSystemSoA {
        std::vector<double> x, y, z;      // Position arrays
        std::vector<double> vx, vy, vz;   // Velocity arrays
        std::vector<double> mass;
        std::vector<int> id;
        
    public:
        void resize(size_t count) {
            x.resize(count);
            y.resize(count);
            z.resize(count);
            vx.resize(count);
            vy.resize(count);
            vz.resize(count);
            mass.resize(count);
            id.resize(count);
        }

        void setParticle(size_t index, const Particle& particle) {
            x[index] = particle.x;
            y[index] = particle.y;
            z[index] = particle.z;
            vx[index] = particle.vx;
            vy[index] = particle.vy;
            vz[index] = particle.vz;
            mass[index] = particle.mass;
            id[index] = particle.id;
        }
        
        double calculateKineticEnergy() {
            // SoA is cache-friendly because each attribute is stored in a
            // contiguous array, and this calculation reads only the velocity
            // and mass arrays that it actually needs.
            double totalEnergy = 0.0;
            for (size_t index = 0; index < mass.size(); ++index) {
                const double velocitySquared = vx[index] * vx[index] +
                                               vy[index] * vy[index] +
                                               vz[index] * vz[index];
                totalEnergy += 0.5 * mass[index] * velocitySquared;
            }

            return totalEnergy;
        }
    };
    
    static std::vector<std::vector<int>> createMatrix(int rows, int cols) {
        if (rows <= 0 || cols <= 0) {
            return {};
        }

        /*
        Conceptually is the same as
std::vector<std::vector<int>> matrix;
std::vector<int> temporaryRow(cols);
for (int row = 0; row < rows; ++row) {
    matrix.push_back(temporaryRow);
}        
        */
        std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                matrix[row][col] = row * cols + col;
            }
        }

        return matrix;
    }
    
    static void initializeParticles(ParticleSystemAoS& system, size_t count) {
        system.resize(count);

        // Use a fixed seed so benchmark data is reproducible across runs.
        std::mt19937 generator(randomSeed);
        std::uniform_real_distribution<double> positionDistribution(-100.0, 100.0);
        std::uniform_real_distribution<double> velocityDistribution(-10.0, 10.0);
        std::uniform_real_distribution<double> massDistribution(0.1, 100.0);

        for (size_t index = 0; index < count; ++index) {
            Particle particle{
                positionDistribution(generator),
                positionDistribution(generator),
                positionDistribution(generator),
                velocityDistribution(generator),
                velocityDistribution(generator),
                velocityDistribution(generator),
                massDistribution(generator),
                static_cast<int>(index)
            };
            system.setParticle(index, particle);
        }
    }
    
    static void initializeParticles(ParticleSystemSoA& system, size_t count) {
        system.resize(count);

        // Use the same seed and distributions as the AoS initializer.
        std::mt19937 generator(randomSeed);
        std::uniform_real_distribution<double> positionDistribution(-100.0, 100.0);
        std::uniform_real_distribution<double> velocityDistribution(-10.0, 10.0);
        std::uniform_real_distribution<double> massDistribution(0.1, 100.0);

        for (size_t index = 0; index < count; ++index) {
            Particle particle{
                positionDistribution(generator),
                positionDistribution(generator),
                positionDistribution(generator),
                velocityDistribution(generator),
                velocityDistribution(generator),
                velocityDistribution(generator),
                massDistribution(generator),
                static_cast<int>(index)
            };
            system.setParticle(index, particle);
        }
    }
};

int main() {
    std::cout << "Memory access pattern and cache behavior analysis" << std::endl;

    volatile long long matrixResult = 0;
    const std::vector<int> matrixSizes = {128, 256, 512};

    for (int size : matrixSizes) {
        const auto matrix = CachePerformanceAnalyzer::createMatrix(size, size);
        const long rowMajorResult =
            CachePerformanceAnalyzer::matrixSumRowMajor(matrix);
        const long columnMajorResult =
            CachePerformanceAnalyzer::matrixSumColumnMajor(matrix);

        if (rowMajorResult != columnMajorResult) {
            std::cerr << "Matrix sum validation failed for size " << size
                      << std::endl;
            return 1;
        }

        for (int iteration = 0; iteration < 100; ++iteration) {
            matrixResult += CachePerformanceAnalyzer::matrixSumRowMajor(matrix);
            matrixResult += CachePerformanceAnalyzer::matrixSumColumnMajor(matrix);
        }

        std::cout << "Matrix " << size << "x" << size
                  << " validated: sum = " << rowMajorResult << std::endl;
    }

    volatile double energyResult = 0.0;
    const std::vector<size_t> particleCounts = {10000, 50000, 100000};

    for (size_t count : particleCounts) {
        CachePerformanceAnalyzer::ParticleSystemAoS aos;
        CachePerformanceAnalyzer::ParticleSystemSoA soa;
        CachePerformanceAnalyzer::initializeParticles(aos, count);
        CachePerformanceAnalyzer::initializeParticles(soa, count);

        const double aosEnergy = aos.calculateKineticEnergy();
        const double soaEnergy = soa.calculateKineticEnergy();

        if (aosEnergy != soaEnergy) {
            std::cerr << "Particle energy validation failed for count " << count
                      << std::endl;
            return 1;
        }

        for (int iteration = 0; iteration < 100; ++iteration) {
            energyResult += aos.calculateKineticEnergy();
            energyResult += soa.calculateKineticEnergy();
        }

        std::cout << "Particles " << count
                  << " validated: kinetic energy = " << aosEnergy << std::endl;
    }

    // Keep benchmark results observable so profiling work is not optimized away.
    std::cout << "Benchmark checksum: " << matrixResult + energyResult << std::endl;
    return 0;
}

