#!/bin/bash

CXX=icpc CXXFLAGS="-Wall -g -O3 -msse4.2 -restrict -std=c++11" ./build.sh || exit 1
