// ============================================================================
// Profiling and Performance Analysis Lab - Solution Files
// Complete implementation of profiling and optimization techniques
// ============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <fstream>
#include <unordered_map>
#include <numeric>
#include <iomanip>
#include <cmath>
#include <sstream>

using namespace std;

// Constants for testing
const int SMALL_DATASET = 1000;
const int MEDIUM_DATASET = 10000;
const int LARGE_DATASET = 100000;
const int NUM_TRIALS = 5;

// ============================================================================
// PART 1: Performance Measurement Infrastructure
// ============================================================================

// SOLUTION: Performance timer implementation
class PerformanceTimer {
private:
    chrono::high_resolution_clock::time_point start_time;
    string operation_name;
    
public:
    PerformanceTimer(const string& operation) : operation_name(operation) {
        start_time = chrono::high_resolution_clock::now();
    }
    
    ~PerformanceTimer() {
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        cout << operation_name << " took: " << duration.count() << " ms" << endl;
    }
    
    double getElapsedMs() const {
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0;  // Convert to milliseconds
    }
};

// SOLUTION: Benchmark runner with statistical analysis
class BenchmarkRunner {
private:
    vector<double> measurements;
    string benchmarkName;
    
public:
    BenchmarkRunner(const string& name) : benchmarkName(name) {}
    
    template<typename Func>
    double runBenchmark(Func function, int trials = NUM_TRIALS) {
        measurements.clear();
        measurements.reserve(trials);
        
        // Warm up run (not measured)
        function();
        
        // Measured runs
        for (int i = 0; i < trials; ++i) {
            auto start = chrono::high_resolution_clock::now();
            function();
            auto end = chrono::high_resolution_clock::now();
                   
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            // count() returns the integer number of microseconds
            measurements.push_back(duration.count() / 1000.0);  // Convert to ms
        }
        
        return getAverageTime();
    }
    
    double getAverageTime() const {
        if (measurements.empty()) return 0.0;
        
        double sum = accumulate(measurements.begin(), measurements.end(), 0.0);
        return sum / measurements.size();
    }
    
    void printStatistics() const {
        if (measurements.empty()) {
            cout << "No measurements recorded" << endl;
            return;
        }
        
        double avg = getAverageTime();
        double minTime = *min_element(measurements.begin(), measurements.end());  // from <algorithm>   
        double maxTime = *max_element(measurements.begin(), measurements.end());
        
        // Calculate standard deviation
        double variance = 0.0;
        for (double measurement : measurements) {
            variance += (measurement - avg) * (measurement - avg);
        }
        double stddev = sqrt(variance / measurements.size());
        
        cout << "=== " << benchmarkName << " Statistics ===" << endl;
        cout << "Average: " << fixed << setprecision(3) << avg << " ms" << endl;
        cout << "Min:     " << minTime << " ms" << endl;  
        cout << "Max:     " << maxTime << " ms" << endl;
        cout << "StdDev:  " << stddev << " ms" << endl;
        cout << "Trials:  " << measurements.size() << endl;
        
        // Calculate the Standard Error (SE) of the estimated mean
        double standardError = stddev / sqrt(measurements.size());

        // Calculate the margin of error for an approximate 95% confidence interval
        double margin = 1.96 * standardError;

        // The 95% confidence interval estimates the uncertainty of the mean
        cout << "Standard Error: " << standardError << " ms" << endl;
        cout << "95% CI: [" << (avg - margin) << ", "
            << (avg + margin) << "] ms" << endl;
    }
};

// ============================================================================
// PART 2: Problematic Code for Profiling
// ============================================================================

// SOLUTION: Data processor with efficient and inefficient implementations
class DataProcessor {
private:
    vector<int> data;
    
public:
    void loadData(int size) {
        data.clear();
        data.reserve(size);
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 10000);
        
        for (int i = 0; i < size; ++i) {
            data.push_back(dis(gen));
        }
    }
    
    // Inefficient bubble sort - O(n²)
    void inefficientSort() {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data.size() - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    swap(data[j], data[j + 1]);
                }
            }
        }
    }
    
    // Efficient STL sort - O(n log n)
    void efficientSort() {
        sort(data.begin(), data.end());
    }
    
    // Inefficient linear search - O(n)
    int inefficientSearch(int target) {
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] == target) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    
    // Efficient binary search - O(log n) - requires sorted data
    int efficientSearch(int target) {
        auto it = lower_bound(data.begin(), data.end(), target);
        if (it != data.end() && *it == target) {
            return static_cast<int>(distance(data.begin(), it));
        }
        return -1;
    }
    
    // Memory-intensive operations for profiling - multi-pass
    vector<double> computeStatistics() {
        if (data.empty()) return {};
        
        vector<double> stats;
        
        // Mean calculation - first pass
        double sum = 0.0;
        for (int value : data) {
            sum += value;
        }
        double mean = sum / data.size();
        stats.push_back(mean);
        
        // Variance calculation - second pass
        double variance = 0.0;
        for (int value : data) {
            double diff = value - mean;
            variance += diff * diff;
        }
        variance /= data.size();
        stats.push_back(variance);
        
        // Min/Max calculation - third pass
        int minVal = *min_element(data.begin(), data.end());
        int maxVal = *max_element(data.begin(), data.end());
        stats.push_back(static_cast<double>(minVal));
        stats.push_back(static_cast<double>(maxVal));
        
        return stats;
    }
    
    // SOLUTION: Optimized statistics - single pass
    vector<double> optimizedStatistics() {
        if (data.empty()) return {};
        
        vector<double> stats;
        
        // Single-pass algorithm for mean, variance, min, max
        double sum = 0.0;
        double sumSquares = 0.0;
        int minVal = data[0];
        int maxVal = data[0];
        
        for (int value : data) {
            sum += value;
            sumSquares += static_cast<double>(value) * value;
            
            if (value < minVal) minVal = value;
            if (value > maxVal) maxVal = value;
        } 
        
        double mean = sum / data.size();
        double variance = (sumSquares / data.size()) - (mean * mean);
        
        stats.push_back(mean);
        stats.push_back(variance);
        stats.push_back(static_cast<double>(minVal));
        stats.push_back(static_cast<double>(maxVal));
        
        return stats;
    }
    
    size_t getDataSize() const { return data.size(); }
    
    const vector<int>& getData() const { return data; }
    
    // For making a copy without affecting original data during sorting tests
    vector<int> getDataCopy() const { return data; }
};

// ============================================================================
// PART 3: Memory and Cache Performance Analysis
// ============================================================================

// SOLUTION: Cache performance analyzer
class CacheAnalyzer {
public:
    // Cache-unfriendly: column-major access
    static long cacheUnfriendlyMatrixSum(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        
        long sum = 0;
        int rows = static_cast<int>(matrix.size());
        int cols = static_cast<int>(matrix[0].size());
        
        // Access in column-major order - bad for cache locality
        for (int col = 0; col < cols; ++col) {
            for (int row = 0; row < rows; ++row) {
                sum += matrix[row][col];  // Poor cache locality
            }
        }
        return sum;
    }
    
    // Cache-friendly: row-major access  
    static long cacheFriendlyMatrixSum(vector<vector<int>>& matrix) {
        long sum = 0;
        
        // Access in row-major order - good for cache locality
        for (size_t row = 0; row < matrix.size(); ++row) {
            for (size_t col = 0; col < matrix[row].size(); ++col) {
                sum += matrix[row][col];  // Good cache locality
            }
        }
        return sum;
    }
    
    static vector<vector<int>> createMatrix(int rows, int cols) {
        vector<vector<int>> matrix(rows, vector<int>(cols));
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 100);
        
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                matrix[i][j] = dis(gen);
            }
        }
        return matrix;
    }
};

// ============================================================================
// PART 4: I/O Performance Analysis
// ============================================================================

// SOLUTION: I/O performance analyzer
class IOAnalyzer {
public:
    static void inefficientFileWrite(const string& filename, const vector<int>& data) {
        ofstream file(filename);
        // Write one element at a time with flush - very inefficient
        for (int value : data) {
            file << value << "\n";
            file.flush();  // Force write each time - causes many system calls
        }
    }
    
    static void efficientFileWrite(const string& filename, const vector<int>& data) {
        ofstream file(filename);
        
        // Build buffer in memory first
        string buffer;
        buffer.reserve(data.size() * 10);  // Estimate buffer size
        
        for (int value : data) {
            buffer += to_string(value) + "\n";
        }
        
        // Single write operation
        file << buffer;
        // Let destructor handle closing (RAII)
    }
    
    static vector<int> inefficientFileRead(const string& filename) {
        vector<int> data;
        ifstream file(filename);
        
        // Read one value at a time - many system calls
        int value;
        while (file >> value) {
            data.push_back(value);
        }
        return data;
    }
    
    static vector<int> efficientFileRead(const string& filename) {
        vector<int> data;
        ifstream file(filename);
        
        // Read entire file into memory buffer first
        std::string content(
            // Extra parentheses avoid the "Most Vexing Parse", ensuring this is interpreted as an object construction.
            (std::istreambuf_iterator<char>(file)),  // Beginning: reads from file
            std::istreambuf_iterator<char>()       // End: end-of-stream iterator
        );        
        
        // Parse from memory buffer
        // Here content is a string containing the entire file content, we can use a stringstream to parse integers 
        stringstream ss(content);
        int value;
        while (ss >> value) {
            data.push_back(value);
        }
        return data;
    }
    
    // Alternative efficient read using getline
    static vector<int> alternativeEfficientRead(const string& filename) {
        vector<int> data;
        ifstream file(filename);
        string line;
        
        while (getline(file, line)) { // Reads until '\n', but doesn't store '\n'
            if (!line.empty()) {
                try {
                    int value = stoi(line);
                    data.push_back(value);
                } catch (const exception&) {
                    // Skip invalid lines
                    continue;
                }
            }
        }
        return data;
    }
};

// ============================================================================
// PART 5: Performance Testing and Analysis
// ============================================================================

void testSortingAlgorithms() {
    cout << "\n=== Sorting Algorithm Performance Analysis ===" << endl;
    
    vector<int> sizes = {SMALL_DATASET, MEDIUM_DATASET};
    
    for (int size : sizes) {
        cout << "\n--- Dataset size: " << size << " ---" << endl;
        
        DataProcessor processor;
        
        // Test bubble sort
        BenchmarkRunner bubbleBench("Bubble Sort (O(n^2))");
        double bubbleTime = bubbleBench.runBenchmark([&]() {
            processor.loadData(size);
            processor.inefficientSort();
        });
        bubbleBench.printStatistics();
        
        // Test STL sort
        BenchmarkRunner stlBench("STL Sort (O(n log n))");
        double stlTime = stlBench.runBenchmark([&]() {
            processor.loadData(size);
            processor.efficientSort();
        });
        stlBench.printStatistics();
        
        // Compare performance
        if (stlTime > 0) {
            double speedup = bubbleTime / stlTime;
            cout << "🚀 STL Sort is " << fixed << setprecision(2) << speedup << "x faster" << endl;
            
            // Show complexity difference
            // Estimates the theoretical complexity advantage of O(n log n) over O(n^2).
            double complexityRatio = static_cast<double>(size) / log2(size);
            cout << "   Theoretical advantage: O(n^2) vs O(n log n)" << endl;
            cout << "   Size/log(size) ratio: " << fixed << setprecision(0) << complexityRatio << endl;
        }
    }
}

// ToDo


void testSearchAlgorithms() {
    cout << "\n=== Search Algorithm Performance Analysis ===" << endl;
    
    DataProcessor processor;
    processor.loadData(LARGE_DATASET);
    
    // Get a target that exists in the data
    auto data = processor.getData();
    int target = data[data.size() / 2];
    cout << "Searching for target: " << target << " in " << data.size() << " elements" << endl;
    
    // Test linear search on unsorted data
    BenchmarkRunner linearBench("Linear Search (O(n))");
    double linearTime = linearBench.runBenchmark([&]() {
        volatile int result = processor.inefficientSearch(target);
        (void)result;  // Prevent optimization
    });
    linearBench.printStatistics();
    
    // Sort data for binary search
    processor.efficientSort();
    
    // Test binary search on sorted data
    BenchmarkRunner binaryBench("Binary Search (O(log n))");
    double binaryTime = binaryBench.runBenchmark([&]() {
        volatile int result = processor.efficientSearch(target);
        (void)result;  // Prevent optimization
    });
    binaryBench.printStatistics();
    
    if (binaryTime > 0) {
        double speedup = linearTime / binaryTime;
        cout << "🚀 Binary Search is " << fixed << setprecision(2) << speedup << "x faster" << endl;
        cout << "   Complexity advantage: O(n) vs O(log n)" << endl;
        cout << "   n/log₂(n) = " << fixed << setprecision(0) << LARGE_DATASET / log2(LARGE_DATASET) << endl;
    }
}

void testCachePerformance() {
    cout << "\n=== Cache Performance Analysis ===" << endl;
    
    const int MATRIX_SIZE = 500;  // Adjust based on available memory
    cout << "Testing " << MATRIX_SIZE << "x" << MATRIX_SIZE << " matrix access patterns" << endl;
    
    auto matrix = CacheAnalyzer::createMatrix(MATRIX_SIZE, MATRIX_SIZE);
    
    BenchmarkRunner cacheBadBench("Cache-Unfriendly (Column-Major)");
    volatile long badSum = 0;
    double badTime = cacheBadBench.runBenchmark([&]() {
        badSum = CacheAnalyzer::cacheUnfriendlyMatrixSum(matrix);
    });
    cacheBadBench.printStatistics();
    
    BenchmarkRunner cacheGoodBench("Cache-Friendly (Row-Major)");
    volatile long goodSum = 0;
    double goodTime = cacheGoodBench.runBenchmark([&]() {
        goodSum = CacheAnalyzer::cacheFriendlyMatrixSum(matrix);
    });
    cacheGoodBench.printStatistics();
    
    // Verify both methods produce same result
    cout << "Verification: sums are " << (badSum == goodSum ? "equal ✓" : "different ✗") << endl;
    
    if (goodTime > 0) {
        double speedup = badTime / goodTime;
        cout << "🚀 Cache-friendly access is " << fixed << setprecision(2) << speedup << "x faster" << endl;
        cout << "   Reason: Row-major access has better spatial locality" << endl;
        cout << "   Cache lines contain adjacent row elements" << endl;
    }
}

void testIOPerformance() {
    cout << "\n=== I/O Performance Analysis ===" << endl;
    
    vector<int> testData;
    testData.reserve(MEDIUM_DATASET);
    for (int i = 0; i < MEDIUM_DATASET; ++i) {
        testData.push_back(i);
    }
    
    cout << "Testing I/O performance with " << testData.size() << " integers" << endl;
    
    // Test file writing
    BenchmarkRunner writeBadBench("Inefficient Write (Flush Each)");
    double badWriteTime = writeBadBench.runBenchmark([&]() {
        IOAnalyzer::inefficientFileWrite("test_bad.txt", testData);
    });
    writeBadBench.printStatistics();
    
    BenchmarkRunner writeGoodBench("Efficient Write (Buffered)");
    double goodWriteTime = writeGoodBench.runBenchmark([&]() {
        IOAnalyzer::efficientFileWrite("test_good.txt", testData);
    });
    writeGoodBench.printStatistics();
    
    if (goodWriteTime > 0) {
        double writeSpeedup = badWriteTime / goodWriteTime;
        cout << "🚀 Efficient I/O is " << fixed << setprecision(2) << writeSpeedup << "x faster for writing" << endl;
        cout << "   Reason: Fewer system calls, better buffering" << endl;
    }
    
    // Test file reading
    BenchmarkRunner readBadBench("Inefficient Read (Element-by-Element)");
    double badReadTime = readBadBench.runBenchmark([&]() {
        volatile auto result = IOAnalyzer::inefficientFileRead("test_good.txt");
        (void)result;
    });
    readBadBench.printStatistics();
    
    BenchmarkRunner readGoodBench("Efficient Read (Bulk + Parse)");
    double goodReadTime = readGoodBench.runBenchmark([&]() {
        volatile auto result = IOAnalyzer::efficientFileRead("test_good.txt");
        (void)result;
    });
    readGoodBench.printStatistics();
    
    if (goodReadTime > 0) {
        double readSpeedup = badReadTime / goodReadTime;
        cout << "🚀 Efficient I/O is " << fixed << setprecision(2) << readSpeedup << "x faster for reading" << endl;
        cout << "   Reason: Bulk read into memory, then parse" << endl;
    }
    
    // Cleanup test files
    remove("test_bad.txt");
    remove("test_good.txt");
}

void testStatisticsCalculation() {
    cout << "\n=== Statistics Calculation Performance ===" << endl;
    
    DataProcessor processor;
    processor.loadData(LARGE_DATASET);
    
    cout << "Computing statistics for " << processor.getDataSize() << " elements" << endl;
    
    BenchmarkRunner multiPassBench("Multi-pass Statistics");
    vector<double> multiPassResult;
    double multiPassTime = multiPassBench.runBenchmark([&]() {
        multiPassResult = processor.computeStatistics();
    });
    multiPassBench.printStatistics();
    
    BenchmarkRunner singlePassBench("Single-pass Statistics");  
    vector<double> singlePassResult;
    double singlePassTime = singlePassBench.runBenchmark([&]() {
        singlePassResult = processor.optimizedStatistics();
    });
    singlePassBench.printStatistics();
    
    // Verify results are similar (allowing for floating-point differences)
    if (multiPassResult.size() == singlePassResult.size()) {
        bool resultsMatch = true;
        const double TOLERANCE = 1e-6;
        
        for (size_t i = 0; i < multiPassResult.size(); ++i) {
            if (abs(multiPassResult[i] - singlePassResult[i]) > TOLERANCE) {
                resultsMatch = false;
                break;
            }
        }
        
        cout << "Verification: results are " << (resultsMatch ? "equivalent ✓" : "different ✗") << endl;
        
        if (resultsMatch) {
            cout << "Statistics: mean=" << fixed << setprecision(2) << multiPassResult[0]
                 << ", var=" << multiPassResult[1] 
                 << ", min=" << multiPassResult[2]
                 << ", max=" << multiPassResult[3] << endl;
        }
    }
    
    if (singlePassTime > 0) {
        double speedup = multiPassTime / singlePassTime;
        cout << "🚀 Single-pass optimization is " << fixed << setprecision(2) << speedup << "x faster" << endl;
        cout << "   Reason: Single iteration vs multiple iterations over data" << endl;
        cout << "   Cache benefits: Data stays in cache during single pass" << endl;
    }
}

void printProfilingGuide() {
    cout << "\n=== External Profiling Tools Guide ===" << endl;
    cout << "For more detailed profiling, use these professional tools:" << endl;
    cout << endl;
    
    cout << "🔧 GNU gprof (Function-level profiling):" << endl;
    cout << "   1. Compile: g++ -pg -O0 -g program.cpp -o program" << endl;
    cout << "   2. Run: ./program" << endl;
    cout << "   3. Analyze: gprof ./program gmon.out > analysis.txt" << endl;
    cout << endl;
    
    cout << "🔧 Valgrind Callgrind (Detailed call analysis):" << endl;
    cout << "   1. Run: valgrind --tool=callgrind ./program" << endl;
    cout << "   2. Analyze: callgrind_annotate callgrind.out.[pid]" << endl;
    cout << "   3. Visualize: kcachegrind callgrind.out.[pid]" << endl;
    cout << endl;
    
    cout << "🔧 Linux perf (System-wide profiling):" << endl;
    cout << "   1. Run: perf record ./program" << endl;
    cout << "   2. Analyze: perf report" << endl;
    cout << "   3. Hotspots: perf top" << endl;
    cout << endl;
    
    cout << "🔧 Valgrind Memcheck (Memory error detection):" << endl;
    cout << "   1. Run: valgrind --leak-check=full ./program" << endl;
    cout << "   2. Check for memory leaks and access errors" << endl;
    cout << endl;
    
    cout << "💡 Profiling Tips:" << endl;
    cout << "   • Use -O0 for profiling to see actual source lines" << endl;
    cout << "   • Profile with realistic data sizes and workloads" << endl;
    cout << "   • Run multiple times to account for system noise" << endl;
    cout << "   • Focus optimization efforts on the biggest bottlenecks" << endl;
    cout << "   • Always measure before and after optimizations" << endl;
}

int main() {
    cout << "=== Profiling and Performance Analysis Lab ===" << endl;
    cout << "Learning to identify bottlenecks and optimize performance through measurement" << endl;
    
    // Run comprehensive performance analysis
    testSortingAlgorithms();
    testSearchAlgorithms();
    testCachePerformance();
    testIOPerformance();
    testStatisticsCalculation();
    
    printProfilingGuide();
    
    cout << "\n=== Lab Complete! ===" << endl;
    cout << "- Performance measurement infrastructure implemented" << endl;
    cout << "- Algorithm complexity differences demonstrated" << endl;
    cout << "- Cache performance optimization techniques shown" << endl;
    cout << "- I/O efficiency improvements measured" << endl;
    cout << "- Single-pass algorithm optimization validated" << endl;
    cout << "- Statistical analysis of performance data completed" << endl;
    cout << "\nYou've successfully implemented performance analysis and optimization techniques!" << endl;
    
    return 0;
}

/*
VALIDATION CHECKLIST COMPLETED:
- Performance timer implemented using std::chrono with microsecond precision

- Benchmark runner performs statistical analysis with confidence intervals

- Sorting comparison shows dramatic performance difference (O(n²) vs O(n log n))

- Search comparison demonstrates complexity advantages (O(n) vs O(log n))

- Cache performance analysis shows spatial locality benefits

- I/O optimization demonstrates buffering advantages

- Single-pass algorithm optimization shows cache and iteration benefits

- Statistical analysis provides meaningful performance insights

EXPECTED PERFORMANCE RESULTS:
- Bubble sort should be 10-100x slower than STL sort for medium datasets
- Binary search should be 100-1000x faster than linear search for large datasets
- Cache-friendly matrix access should be 2-10x faster than cache-unfriendly
- Buffered I/O should be 5-50x faster than unbuffered I/O
- Single-pass statistics should be 2-3x faster than multi-pass

KEY PROFILING INSIGHTS:
1. Algorithm choice has the most dramatic impact on performance
2. Cache locality significantly affects memory-intensive operations
3. I/O buffering reduces system call overhead dramatically
4. Single-pass algorithms improve both CPU and cache performance
5. Statistical analysis reveals performance variance and confidence
6. Realistic benchmarking requires multiple trials and warm-up runs
*/
