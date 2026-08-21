/*
You're developing a high-frequency trading system where microsecond-level performance differences
 directly impact profitability. Create a robust timing and benchmarking system that provides statistically meaningful results.
Practice
In the code below, build comprehensive performance measurement tools:
Implement precise timing using std::chrono with microsecond accuracy
Create statistical analysis that accounts for measurement variance
Add warm-up runs to eliminate cold cache effects
Calculate meaningful statistics including confidence intervals
Test the framework with simple operations to validate accuracy
*/

#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cmath>
#include <functional>
#include <string>

class PrecisionTimer {
private:
    std::chrono::high_resolution_clock::time_point startTime;

public:
    PrecisionTimer() {
        // Initialize timing system
        startTime = std::chrono::high_resolution_clock::now();
    }

    void startTiming() {
        // Record start time
        startTime = std::chrono::high_resolution_clock::now();
    }

    double getElapsedMicroseconds() {
        // Calculate elapsed time in microseconds
        auto endTime = std::chrono::high_resolution_clock::now();

        return std::chrono::duration<double, std::micro>(
            endTime - startTime
        ).count();
    }

    double getElapsedMilliseconds() {
        // Calculate elapsed time in milliseconds
        auto endTime = std::chrono::high_resolution_clock::now();

        return std::chrono::duration<double, std::milli>(
            endTime - startTime
        ).count();
    }
};

class StatisticalBenchmark {
private:
    std::vector<double> measurements;
    std::string benchmarkName;
    
public:
    StatisticalBenchmark(const std::string& name) : benchmarkName(name) {}
    
    template<typename Func>
    void runBenchmark(Func function, int trials = 10) {
        measurements.clear();
        
        // Warm-up run, eliminate cold cache effects    
        function();
        
        for (int i = 0; i < trials; ++i) {
            PrecisionTimer timer;
            timer.startTiming();
            function();
            measurements.push_back(timer.getElapsedMicroseconds());
        }
    }
    
    void printStatistics() {
        if (measurements.empty()) return;
        
        double mean = std::accumulate(measurements.begin(), measurements.end(), 0.0) / measurements.size();
        double variance = 0.0;
        for (double m : measurements) variance += (m - mean) * (m - mean);
        double stddev = std::sqrt(variance / measurements.size());
        
        std::cout << "Mean: " << mean << " μs, StdDev: " << stddev << " μs" << std::endl;
    }
    
    double getAverageTime() const {
        if (measurements.empty()) return 0.0;

        return std::accumulate(measurements.begin(), measurements.end(), 0.0) /
               measurements.size();
    }
    
    double getStandardDeviation() const {
        if (measurements.empty()) return 0.0;

        const double average = getAverageTime();
        double variance = 0.0;
        for (double measurement : measurements) {
            const double difference = measurement - average;
            variance += difference * difference;
        }

        return std::sqrt(variance / measurements.size());
    }
};

int main() {
    std::vector<int> values(1000);
    std::iota(values.begin(), values.end(), 1);  // fill with values 1 to 1000  

    volatile long long result = 0;
    StatisticalBenchmark benchmark("Sum 1000 integers");
    benchmark.runBenchmark([&values, &result]() {
        result = std::accumulate(values.begin(), values.end(), 0LL);
/*
        for (int iteration = 0; iteration < 10000; ++iteration) {
            result = std::accumulate(values.begin(), values.end(), 0LL);
        }
*/
    }, 100);    

    std::cout << "Benchmark: Sum 1000 integers" << std::endl;
    std::cout << std::fixed << std::setprecision(3)
                        << "Average: " << benchmark.getAverageTime() << " us" << std::endl
                        << "Standard deviation: " << benchmark.getStandardDeviation()
                        << " us" << std::endl
                        << "Result: " << result << std::endl;                        
  return 0;
}


