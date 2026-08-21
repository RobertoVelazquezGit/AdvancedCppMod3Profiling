Advanced Cpp Module 3 Profiling

## Build and Debug Configuration

This project contains several source files, each with its own `main()` function. Only one source file is compiled at a time. The source file is selected using the `SOURCE` variable.

The available sources are:

* `SOURCE=1` → `src/StatisticalBenchmarking.cpp`
* `SOURCE=2` → `src/source2.cpp`
* `SOURCE=3` → `src/source3.cpp`

### Build

To build the project, use:

```bash
make build SOURCE=1
```

This builds `StatisticalBenchmarking.cpp`.

For example, to build `source2.cpp`:

```bash
make build SOURCE=2
```

To build `source3.cpp`:

```bash
make build SOURCE=3
```

If `SOURCE` is not specified, the Makefile uses `SOURCE=1` by default:

```bash
make build
```

### Debug Build

The same `SOURCE` selection can be used for a Debug build:

```bash
make debug SOURCE=1
```

For example:

```bash
make debug SOURCE=2
```

builds `source2.cpp` in **Debug** configuration.

This allows each source file to be built and debugged independently, even though they all generate the same executable:

```text
build/Mod3ProfilingPerformance
```

## Profiling with gprof

Run these commands from the project root. The `build` target already enables the `-pg` compiler option required by `gprof`:

```bash
# Remove profiling output from a previous run
rm -f gmon.out StatisticalBenchmarking.txt

# Build the source to profile (SOURCE=1 is StatisticalBenchmarking.cpp)
make build SOURCE=1

# Run the instrumented executable and generate gmon.out
./build/Mod3ProfilingPerformance

# Generate the profiling report
gprof ./build/Mod3ProfilingPerformance gmon.out > StatisticalBenchmarking.txt

# Read the report
less StatisticalBenchmarking.txt
```

To profile another source file, replace `SOURCE=1` with `SOURCE=2` or `SOURCE=3` in the build command. Execute the program before running `gprof`, otherwise `gmon.out` will not contain a new profile.
