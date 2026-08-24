/*
Use professional profiling tools to perform detailed analysis of your optimized and unoptimized code versions.
Practice
In the code below, integrate external profiling tools:
Compile application with appropriate flags for gprof (-pg) and debug symbols (-g)
Run application with Valgrind callgrind to analyze function calls and cache behavior
Use gprof to identify functions consuming the most CPU time
Apply perf or other system profilers to understand hardware-level performance
Compare profiling results between optimized and unoptimized versions
*/

/*
# Compile for profiling
g++ -pg -O0 -g -o program program.cpp

# Run with gprof
./program
gprof ./program gmon.out > analysis.txt

# Run with Valgrind
valgrind --tool=callgrind ./program
callgrind_annotate callgrind.out.*
*/

#include <iostream> 
#include <vector>   
#include <random>
#include <fstream>  

// Create a complete test application for external profiling
class ProfilingTestApplication {
private:
    std::vector<int> largeDataset;
    
public:
    void initializeData(size_t size) {
        largeDataset.resize(size);

        // Fixed seed to generate the same dataset on every run
        constexpr unsigned int SEED = 42;
        std::mt19937 generator(SEED);

        std::uniform_int_distribution<int> distribution(0, 100000);

        for (int& value : largeDataset) {
            value = distribution(generator);
        }
    }
    
    void runInefficiientWorkload() {

        // Use a smaller dataset because Bubble Sort has O(n^2) complexity
        constexpr size_t SMALL_DATASET_SIZE = 10000;

        std::vector<int> smallDataset(
            largeDataset.begin(),
            largeDataset.begin() + SMALL_DATASET_SIZE
        );

        // 1. Bubble Sort
        for (size_t i = 0; i < smallDataset.size() - 1; ++i) {
            for (size_t j = 0; j < smallDataset.size() - i - 1; ++j) {
                if (smallDataset[j] > smallDataset[j + 1]) {
                    std::swap(smallDataset[j], smallDataset[j + 1]);
                }
            }
        }

        // 2. Multiple linear searches
        const std::vector<int> targets = {
            100,
            1000,
            10000,
            50000,
            90000
        };

        for (int target : targets) {
            for (int value : largeDataset) {
                if (value == target) {
                    break;
                }
            }
        }

        // 3. Column-major matrix access
        constexpr size_t ROWS = 1000;
        constexpr size_t COLS = 1000;

        std::vector<int> matrix(ROWS * COLS, 1);

        volatile long long sum = 0;

        for (size_t col = 0; col < COLS; ++col) {
            for (size_t row = 0; row < ROWS; ++row) {
                sum += matrix[row * COLS + col];
            }
        }

        // 4. Inefficient I/O: perform many small write operations
        std::ofstream file("inefficient_output.txt");

        for (int value : largeDataset) {
            file << value << '\n';
        }
    }
    
    void runOptimizedWorkload() {
        // TODO: Combine optimized versions
        // - STL sort
        // - Binary search or hash lookup
        // - Row-major matrix operations
        // - Buffered I/O operations
    }
    
    void runComprehensiveAnalysis() {
        // TODO: Execute both workloads with timing
        // Print performance comparison results
    }
};

// Profiling guide implementation
void demonstrateProfilingWorkflow() {
    std::cout << "=== External Profiling Tools Workflow ===" << std::endl;
    
    // TODO: Create sample application that can be profiled
    // Include instructions for:
    // 1. Compiling with profiling flags
    // 2. Running with different profilers
    // 3. Interpreting profiling output
    // 4. Identifying optimization targets
}

int main() {
    ProfilingTestApplication app;
    
    // Initialize data for profiling
    app.initializeData(100000); // Example size
    
    // Run inefficient workload
    app.runInefficiientWorkload();
    
    // Run optimized workload
    app.runOptimizedWorkload();
    
    // Perform comprehensive analysis
    app.runComprehensiveAnalysis();
    
    // Demonstrate profiling workflow
    demonstrateProfilingWorkflow();
    
    return 0;
}   


