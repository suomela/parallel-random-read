Parallel Random Reads
=====================

Testing how well the following loop parallelises:

    for i = 1 to n:
        y[i] = x[p[i]]

See: http://cs.stackexchange.com/q/18834/232


Files
-----

Source code: test.cc

Compile everything: ./build.sh

Compiled files: bin/test-*


Results
-------

Output files:

- result-X: experiments on a machine with X physical cores.

- result-X/test-openmp-Y: parallelisation with OpenMP, Y threads

- result-X/test-simple: baseline, no parallelism.

In the result files, the columns are

1. minimum time
2. average time
3. maximum time
4. n

Time is in picoseconds per iteration.

There is a simple Python script "print-speedups.py" that analyses
the results and prints out the following information:

1. minimum speedup
2. average speedup
3. maximum speedup
4. n

Examples:

    $ cat result-12/test-openmp-12
    ...
    4780.6  4793.0  4799.8  4294967296

    $ cat result-12/test-simple
    ...
    42800.0 42807.9 42815.0 4294967296

    $ ./print-speedups.py
    ...
    result-12/test-openmp-12
    ...
    8.917   8.931   8.956   4294967296

Interpretation:

- 12 cores, input = 4 gigabytes
- sequential version: approx. 42.8 ns per iteration
- parallel version: approx. 4.8 ns per iteration
- speedup: approx. 8.9-fold speedup
