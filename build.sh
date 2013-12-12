#!/bin/bash

if [ "$CXX" = "" ]; then
    CXX="g++"
fi

if [ "$CXXFLAGS" = "" ]; then
    CXXFLAGS="-g -std=c++11 -march=native -O3 -Werror -Wall -Wextra -Wunused -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wdouble-promotion -Wconversion -Wsign-conversion -Wmissing-declarations -Wredundant-decls -Wno-unknown-pragmas"
fi

if [ "$OMPFLAGS" = "" ]; then
    OMPFLAGS="-fopenmp"
fi

mkdir -p bin || exit 1

$CXX $CXXFLAGS           -DTEST_SIMPLE -o bin/test-simple$BINSUFFIX test.cc || exit 1
$CXX $CXXFLAGS $OMPFLAGS -DTEST_OPENMP -o bin/test-openmp$BINSUFFIX test.cc || exit 1
