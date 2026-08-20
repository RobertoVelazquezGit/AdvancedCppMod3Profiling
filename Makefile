SOURCE ?= 1

.PHONY: all
all: format test build

.PHONY: format
format:
	clang-format src/* include/* -i

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake -DSOURCE=$(SOURCE) .. && \
	make

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug -DSOURCE=$(SOURCE) .. && \
	make

.PHONY: clean
clean:
	rm -rf build
