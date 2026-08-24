cmake_minimum_required(VERSION 3.5.1)

project(Mod3ProfilingPerformance)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE 1 CACHE STRING "Source file to build")

if(SOURCE EQUAL 1)
    set(SOURCE_FILE src/StatisticalBenchmarking.cpp)
elseif(SOURCE EQUAL 2)
    set(SOURCE_FILE src/AlgorithmPerformanceProfiling.cpp)
elseif(SOURCE EQUAL 3)
    set(SOURCE_FILE src/MemCacheAnalysis.cpp)
elseif(SOURCE EQUAL 4)
    set(SOURCE_FILE src/IOPerformanceBottleneck.cpp)
elseif(SOURCE EQUAL 5)
    set(SOURCE_FILE src/ProfilingTestApp.cpp)
else()
    message(FATAL_ERROR "Invalid SOURCE: ${SOURCE}")
endif()

add_executable(Mod3ProfilingPerformance ${SOURCE_FILE})