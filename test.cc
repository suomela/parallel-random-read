#include <chrono>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

//// Settings

constexpr unsigned TESTS {10};
constexpr unsigned BITS_FROM {20};
constexpr unsigned BITS_TO {32};
constexpr double TIME_SCALE {1E12};  // picoseconds per array element
constexpr unsigned MAGIC {123};

static_assert(BITS_TO <= 32, "BITS_TO");
static_assert(std::numeric_limits<unsigned>::max() == 0xFFFFFFFF, "32-bit unsigned");

using data_t = unsigned char;
using Clock = std::chrono::high_resolution_clock;
using Rng = std::mt19937;

//// What to test

#if defined(TEST_SIMPLE)

static void loop(unsigned n, const unsigned *p, const data_t *x, data_t *y) {
    for (unsigned i = 0; i < n; ++i) {
        y[i] = x[p[i]];
    }
}

#elif defined(TEST_OPENMP)

static void loop(unsigned n, const unsigned *p, const data_t *x, data_t *y) {
    #pragma omp parallel for
    for (unsigned i = 0; i < n; ++i) {
        y[i] = x[p[i]];
    }
}

#else
#error "no test defined"
#endif

//// Tester

inline double seconds_since(Clock::time_point start)
{
    using Sec = std::chrono::duration<double>;
    return {std::chrono::duration_cast<Sec>(Clock::now() - start).count()};
}

static void tester(unsigned n, const unsigned *p, data_t *x, data_t *y) {
    double min {std::numeric_limits<double>::infinity()};
    double max {0.0};
    const Clock::time_point start0{Clock::now()};
    for (unsigned j = 0; j < TESTS; ++j) {
        const Clock::time_point start{Clock::now()};
        loop(n, p, x, y);
        double t {seconds_since(start)};
        min = min <= t ? min : t;
        max = max >= t ? max : t;
        std::swap(x, y);
    }
    double total {seconds_since(start0)};
    double avg {total / TESTS};
    double scale {TIME_SCALE / n};
    std::cout.precision(1);
    std::cout << std::fixed << scale * min << "\t";
    std::cout << std::fixed << scale * avg << "\t";
    std::cout << std::fixed << scale * max << "\t";
    std::cout << n << std::endl;
}

static void create(unsigned n, unsigned *p, data_t *x, data_t *y) {
    Rng rng {1};
    unsigned mask {n - 1};
    static_assert(rng.max() == std::numeric_limits<unsigned>::max(), "rng.max");
    for (unsigned i = 0; i < n; ++i) {
        unsigned a {static_cast<unsigned>(rng()) & mask};
        p[i] = a;
        x[i] = MAGIC;
        y[i] = 0;
    }
}

static bool sanity(unsigned n, const data_t *x, const data_t *y) {
    for (unsigned i = 0; i < n; ++i) {
        if (x[i] != MAGIC || y[i] != MAGIC) {
            return false;
        }
    }
    return true;
}

static bool driver(unsigned n, unsigned *p, data_t *x, data_t *y) {
    create(n, p, x, y);
    tester(n, p, x, y);
    return sanity(n, x, y);
}

int main() {
    for (unsigned bits = BITS_FROM; bits < BITS_TO; ++bits) {
        unsigned n {1U << bits};
        std::unique_ptr<data_t[]> x { new data_t[n] };
        std::unique_ptr<data_t[]> y { new data_t[n] };
        std::unique_ptr<unsigned[]> p { new unsigned[n] };
        if (!driver(n, p.get(), x.get(), y.get())) {
            std::cerr << "error" << std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
