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
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>

// Create a complete test application for external profiling
class ProfilingTestApplication {
private:
    static constexpr size_t SMALL_DATASET_SIZE = 10000;
    std::vector<int> largeDataset;
    std::vector<int> smallDataset;

    void resetSmallDataset() {
    // Restore the original unsorted data
    smallDataset.assign(
        largeDataset.begin(),
        largeDataset.begin() + SMALL_DATASET_SIZE
    );
}
    
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

        // Initialize smallDataset with the first elements of largeDataset
        resetSmallDataset();
    }    

    void runInefficiientWorkload() {

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
            for (int value : smallDataset) {
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

        // 1. Optimized sorting using STL
        std::sort(smallDataset.begin(), smallDataset.end());

        // 2. Binary search on the sorted dataset
        const std::vector<int> targets = {
            100,
            1000,
            10000,
            50000,
            90000
        };

        for (int target : targets) {
            std::binary_search(
                smallDataset.begin(),
                smallDataset.end(),
                target
            );
        }

        // 3. Row-major matrix access
        constexpr size_t ROWS = 1000;
        constexpr size_t COLS = 1000;

        std::vector<int> matrix(ROWS * COLS, 1);

        volatile long long sum = 0;

        for (size_t row = 0; row < ROWS; ++row) {
            for (size_t col = 0; col < COLS; ++col) {
                sum += matrix[row * COLS + col];
            }
        }

        // 4. Buffered I/O
        std::ostringstream buffer;

        for (int value : largeDataset) {
            buffer << value << '\n';
        }

        std::ofstream file("optimized_output.txt");
        file << buffer.str();
    }
    
    void runComprehensiveAnalysis() {

        std::cout << "=== Comprehensive Performance Analysis ===\n";

        // Measure inefficient workload
        auto start = std::chrono::high_resolution_clock::now();

        runInefficiientWorkload();

        auto end = std::chrono::high_resolution_clock::now();

        auto inefficientTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        // Restore the original unsorted dataset
        resetSmallDataset();

        // Measure optimized workload
        start = std::chrono::high_resolution_clock::now();

        runOptimizedWorkload();

        end = std::chrono::high_resolution_clock::now();

        auto optimizedTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        // Print results
        std::cout << "Inefficient workload: " << inefficientTime << " ms\n";
        std::cout << "Optimized workload:   " << optimizedTime << " ms\n";

        if (optimizedTime > 0) {
            double speedup =
                static_cast<double>(inefficientTime) / optimizedTime;

            std::cout << "Speedup:              " << speedup << "x\n";
        }
    }
};

// Profiling guide implementation
void demonstrateProfilingWorkflow() {
    std::cout << "=== External Profiling Tools Workflow ===\n";

    ProfilingTestApplication app;

    // Initialize data for profiling
    app.initializeData(100000);

    // Run complete performance analysis
    app.runComprehensiveAnalysis();

    std::cout << "\nProfiling commands:\n";
    std::cout << "gprof ./program gmon.out > analysis.txt\n";
    std::cout << "valgrind --tool=callgrind ./program\n";
    std::cout << "callgrind_annotate callgrind.out.*\n";
}

int main() {
    // Demonstrate profiling workflow
    demonstrateProfilingWorkflow();
    
    return 0;
}   


