/*
You're optimizing a log processing system that handles gigabytes of data.
 Profile different I/O strategies to identify and eliminate performance bottlenecks.
Practice
In the code below, profile I/O performance bottlenecks:
Compare unbuffered vs buffered vs bulk I/O operations
Measure the impact of frequent flush operations on write performance
Test different file sizes to understand when I/O becomes a bottleneck
Analyze read performance with different buffering strategies
Measure file sizes and correlate with performance results
*/

#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>

class IOPerformanceAnalyzer {
public:
    // Inefficient I/O: many small operations
    // Writes to a binary file one integer at a time, flushing after each write.    
    static void writeDataUnbuffered(const std::string& filename,
                                    const std::vector<int>& data) {
        std::ofstream file(filename);

        if (!file) {
            std::cerr << "Error opening file: " << filename << '\n';
            return;
        }

        for (int value : data) {
            file << value << '\n';
            file.flush();  // Force data to be written after every value
        }
    }
        
    // Efficient I/O: buffered operations
    // Writes to a text file using a string buffer to minimize the number of write operations.  
    static void writeDataBuffered(const std::string& filename, 
                                const std::vector<int>& data) {
        std::ofstream file(filename);
        std::string buffer;
        buffer.reserve(data.size() * 10);  // Pre-allocate memory to avoid string reallocations
        
        for (int value : data) {
            buffer += std::to_string(value) + "\n";
        }
        file << buffer;  // Single write operation
    }

    // Efficient I/O: bulk operations
    // Writes to a binary file in a single operation, minimizing the number of system calls.    
    static void writeDataBulk(const std::string& filename,
                            const std::vector<int>& data) {
        std::ofstream file(filename, std::ios::binary);

        if (!file) {
            std::cerr << "Error opening file: " << filename << '\n';
            return;
        }

        // data() returns a pointer to the first int in the vector.
        // Write all N bytes to the file in a single operation.
        file.write(reinterpret_cast<const char*>(data.data()),
                data.size() * sizeof(int));
    }    

    // Reads from a binary file one integer at a time, which is inefficient for large files.
    static std::vector<int> readDataUnbuffered(const std::string& filename) {
        std::ifstream file(filename);

        if (!file) {
            std::cerr << "Error opening file: " << filename << '\n';
            return {};
        }

        std::vector<int> data;
        int value;

        // Read one integer at a time from the file.
        while (file >> value) {
            data.push_back(value);
        }

        return data;
    }    

    // Reads from a text file using a string buffer to minimize the number of read operations, which is more efficient for large files.  
    static std::vector<int> readDataBuffered(const std::string& filename) {
        std::ifstream file(filename);

        if (!file) {
            std::cerr << "Error opening file: " << filename << '\n';
            return {};
        }

        // Read the entire file into a memory buffer.
        std::stringstream buffer;
        buffer << file.rdbuf();

        std::vector<int> data;
        int value;

        // Parse integers from the memory buffer.
        while (buffer >> value) {
            data.push_back(value);
        }

        return data;
    }
    
    // File size analysis
    static size_t getFileSize(const std::string& filename) {
        // Open in binary mode to measure the exact file size in bytes.
        // This works for both text and binary files.    
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if (!file) {
            std::cerr << "Error opening file: " << filename << '\n';
            return 0;
        }

        // tellg() returns the current position, which is the file size
        // because the file was opened at the end.
        return static_cast<size_t>(file.tellg());
    }

    static std::vector<int> generateTestData(size_t count) {
        constexpr unsigned int SEED = 42;

        std::vector<int> data;
        data.reserve(count);

        std::mt19937 generator(SEED);
        std::uniform_int_distribution<int> distribution(0, 1000000);

        for (size_t i = 0; i < count; ++i) {
            data.push_back(distribution(generator));
        }

        return data;
    }        
};

int main() {
    std::cout << "I/O performance bottleneck analysis\n";

    const std::vector<size_t> testSizes = {
        10000,
        100000,
        1000000
    };

    for (size_t count : testSizes) {
        std::cout << "\nTesting with " << count << " integers\n";

        // Generate reproducible test data
        std::vector<int> data = IOPerformanceAnalyzer::generateTestData(count);

        const std::string unbufferedFile = "unbuffered_" + std::to_string(count) + ".txt";
        const std::string bufferedFile   = "buffered_"   + std::to_string(count) + ".txt";
        const std::string bulkFile       = "bulk_"       + std::to_string(count) + ".bin";

        // Write tests
        IOPerformanceAnalyzer::writeDataUnbuffered(unbufferedFile, data);
        IOPerformanceAnalyzer::writeDataBuffered(bufferedFile, data);
        IOPerformanceAnalyzer::writeDataBulk(bulkFile, data);

        // Read text files using different strategies
        auto unbufferedData =
            IOPerformanceAnalyzer::readDataUnbuffered(unbufferedFile);

        auto bufferedData =
            IOPerformanceAnalyzer::readDataBuffered(bufferedFile);

        // Show file sizes
        std::cout << "Unbuffered file size: "
                  << IOPerformanceAnalyzer::getFileSize(unbufferedFile)
                  << " bytes\n";

        std::cout << "Buffered file size:   "
                  << IOPerformanceAnalyzer::getFileSize(bufferedFile)
                  << " bytes\n";

        std::cout << "Bulk binary file size: "
                  << IOPerformanceAnalyzer::getFileSize(bulkFile)
                  << " bytes\n";

        // Verify that the reads were actually performed correctly
        std::cout << "Unbuffered values read: "
                  << unbufferedData.size() << '\n';

        std::cout << "Buffered values read:   "
                  << bufferedData.size() << '\n';
    }

    return 0;
}