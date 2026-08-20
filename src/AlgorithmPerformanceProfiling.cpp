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

class AlgorithmProfiler {
private:
    std::vector<int> testData;
    
public:
    void generateTestData(size_t size) {
        // TODO: Generate realistic test dataset
        // Use random data with controlled distribution
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
        // TODO: Use std::sort for comparison
    }
    
    // Linear search O(n)
    int linearSearch(const std::vector<int>& data, int target) {
        // TODO: Implement simple linear search
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
        // TODO: Build hash map for O(1) lookups
        return {};
    }
    
    int hashLookup(const std::unordered_map<int, int>& hashMap, int target) {
        // TODO: Perform hash-based lookup
        return -1;
    }
    
    // Memory-intensive operation for cache analysis
    void processDataSequential(const std::vector<int>& data) {
        // TODO: Sequential memory access pattern (cache-friendly)
        volatile long sum = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            sum += data[i];
        }
    }
    
    void processDataRandom(const std::vector<int>& data) {
        // TODO: Random memory access pattern (cache-unfriendly)
        std::vector<size_t> indices(data.size());
        std::iota(indices.begin(), indices.end(), 0);
       std::default_random_engine rng(std::random_device{}()); 
        std::shuffle(indices.begin(), indices.end(), rng);

        
        volatile long sum = 0;
        for (size_t idx : indices) {
            sum += data[idx];
        }
    }
};

int main() {
  std::cout << "Hello world source2" << std::endl;

  return 0;
}
