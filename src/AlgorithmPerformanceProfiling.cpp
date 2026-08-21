/*
You're analyzing a data processing application that handles millions of records.
 Use systematic profiling to compare different algorithmic approaches and identify performance bottlenecks.
Practice
In the code below, implement comprehensive algorithm profiling:
Create test data with different sizes (1K, 10K, 100K elements)
Profile sorting algorithms to demonstrate O(n²) vs O(n log n) complexity differences
Compare search algorithms showing O(n) vs O(log n) vs O(1) performance
Test memory access patterns to understand cache behavior impact
Measure how performance scales with data size to validate complexity analysis
*/

#include <vector>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <iostream>
#include <numeric>

class AlgorithmProfiler {
private:
    std::vector<int> testData;
    
public:
    void generateTestData(size_t size) {
        testData.resize(size);

        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(1, size * 10);

        for (int& value : testData) {
            value = distribution(generator);
        }
    }

    const std::vector<int>& getTestData() const {
        return testData;
    }
    
    // Inefficient O(n²) sorting algorithm for comparison
    void bubbleSort(std::vector<int>& data) {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data.size() - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
    }
    
    
    // Efficient O(n log n) sorting using STL
    void optimizedSort(std::vector<int>& data) {
        std::sort(data.begin(), data.end());
    }
    
    // Linear search O(n)
    int linearSearch(const std::vector<int>& data, int target) {
        for (size_t index = 0; index < data.size(); ++index) {
            if (data[index] == target) {
                return static_cast<int>(index);
            }
        }

        return -1;
    }
    
    // Binary search O(log n) - requires sorted data
    int binarySearch(const std::vector<int>& data, int target) {
        int left = 0, right = data.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (data[mid] == target) return mid;
            else if (data[mid] < target) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }
    
    // Hash-based lookup O(1) average case
    std::unordered_map<int, int> buildHashIndex(const std::vector<int>& data) {
        std::unordered_map<int, int> hashMap;
        hashMap.reserve(data.size());

        for (size_t index = 0; index < data.size(); ++index) {
            hashMap[data[index]] = static_cast<int>(index);
        }

        return hashMap;
    }
    
    int hashLookup(const std::unordered_map<int, int>& hashMap, int target) {
        auto iterator = hashMap.find(target);
        return iterator != hashMap.end() ? iterator->second : -1;
    }
    
    // Memory-intensive operation for cache analysis
    void processDataSequential(const std::vector<int>& data) {
        volatile long long sum = 0;
        for (int value : data) {
            sum += value;
        }
    }
    
    void processDataRandom(const std::vector<int>& data) {
        std::vector<size_t> indices(data.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::mt19937 generator(std::random_device{}());
        std::shuffle(indices.begin(), indices.end(), generator);

        volatile long long sum = 0;
        for (size_t idx : indices) {
            sum += data[idx];
        }
    }
};

int main() {
    AlgorithmProfiler profiler;
    const std::vector<size_t> dataSizes = {1000, 10000, 100000};

    std::cout << "Algorithm profiling benchmark" << std::endl;

    for (size_t size : dataSizes) {
        profiler.generateTestData(size);
        const auto& data = profiler.getTestData();
        const int target = data.back();

        std::cout << "\nData size: " << size << std::endl;

        if (size <= 10000) {
            std::vector<int> bubbleData = data;
            profiler.bubbleSort(bubbleData);
        }

        std::vector<int> optimizedData = data;
        profiler.optimizedSort(optimizedData);

        for (int iteration = 0; iteration < 1000; ++iteration) {
            profiler.linearSearch(data, target);
            profiler.binarySearch(optimizedData, target);
        }

        const auto hashMap = profiler.buildHashIndex(data);
        for (int iteration = 0; iteration < 1000; ++iteration) {
            profiler.hashLookup(hashMap, target);
        }

        for (int iteration = 0; iteration < 100; ++iteration) {
            profiler.processDataSequential(data);
            profiler.processDataRandom(data);
        }
    }

    return 0;
}
