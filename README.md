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
