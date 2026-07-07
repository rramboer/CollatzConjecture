#include "Collatz.h"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

namespace {
constexpr std::uint64_t kMax = std::numeric_limits<std::uint64_t>::max();

std::vector<std::uint64_t> starts(const Collatz& c) {
    std::vector<std::uint64_t> out;
    out.reserve(c.results().size());
    for (const CollatzResult& point : c.results()) {
        out.push_back(point.start);
    }
    return out;
}
} // namespace

TEST_CASE("iterate counts steps to reach 1", "[iterate]") {
    CHECK(Collatz::iterate(1) == 0);
    CHECK(Collatz::iterate(2) == 1);
    CHECK(Collatz::iterate(3) == 7);
    CHECK(Collatz::iterate(6) == 8);
    CHECK(Collatz::iterate(7) == 16);
    CHECK(Collatz::iterate(27) == 111);
    CHECK(Collatz::iterate(97) == 118);
}

TEST_CASE("iterate on powers of two returns the exponent", "[iterate]") {
    for (std::uint64_t exponent = 0; exponent < 64; ++exponent) {
        CHECK(Collatz::iterate(std::uint64_t{1} << exponent) == exponent);
    }
}

TEST_CASE("iterate rejects 0 (its trajectory never reaches 1)", "[iterate]") {
    CHECK_FALSE(Collatz::iterate(0).has_value());
}

TEST_CASE("iterate reports overflow instead of wrapping", "[iterate][overflow]") {
    // kMax is odd, so the first step is 3n + 1, which cannot fit in 64 bits.
    CHECK_FALSE(Collatz::iterate(kMax).has_value());
    // kMax - 1 halves once to an odd value that still overflows 3n + 1.
    CHECK_FALSE(Collatz::iterate(kMax - 1).has_value());
    // The smallest odd value whose very first step 3n + 1 overflows.
    const std::uint64_t threshold = (kMax - 1) / 3; // even, so +1 is odd
    REQUIRE((threshold + 1) % 2 == 1);
    CHECK_FALSE(Collatz::iterate(threshold + 1).has_value());
    // Just below the guard, a power of two halves down without overflowing.
    CHECK(Collatz::iterate(std::uint64_t{1} << 62).has_value());
}

TEST_CASE("isPrime classifies small numbers", "[isPrime]") {
    CHECK_FALSE(Collatz::isPrime(0));
    CHECK_FALSE(Collatz::isPrime(1));
    CHECK(Collatz::isPrime(2));
    CHECK(Collatz::isPrime(3));
    CHECK_FALSE(Collatz::isPrime(4));
    CHECK(Collatz::isPrime(5));
    CHECK_FALSE(Collatz::isPrime(9));
    CHECK_FALSE(Collatz::isPrime(15));
    CHECK(Collatz::isPrime(97));
    CHECK_FALSE(Collatz::isPrime(1000000));
    CHECK(Collatz::isPrime(1000003));
}

TEST_CASE("mode All records every value and tracks the maximum", "[modes]") {
    Collatz c(10, Collatz::Mode::All);
    c.doCalculations();
    CHECK(starts(c) == std::vector<std::uint64_t>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    CHECK(c.results()[6].iterations == 16); // iterate(7)
    CHECK(c.maxStart() == 9);
    CHECK(c.maxIter() == 19);
    CHECK(c.overflowed().empty());
}

TEST_CASE("mode Odds and mode Evens select alternating values", "[modes]") {
    Collatz odds(10, Collatz::Mode::Odds);
    odds.doCalculations();
    CHECK(starts(odds) == std::vector<std::uint64_t>{1, 3, 5, 7, 9});

    Collatz evens(10, Collatz::Mode::Evens);
    evens.doCalculations();
    CHECK(starts(evens) == std::vector<std::uint64_t>{2, 4, 6, 8, 10});
}

TEST_CASE("mode PowersOfTwo records each power once and terminates at the "
          "64-bit boundary",
          "[modes][overflow]") {
    Collatz small(1024, Collatz::Mode::PowersOfTwo);
    small.doCalculations();
    CHECK(starts(small) == std::vector<std::uint64_t>{1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024});
    CHECK(small.maxStart() == 1024);
    CHECK(small.maxIter() == 10);

    // The doubling loop must stop rather than wrap past 2^63.
    Collatz full(kMax, Collatz::Mode::PowersOfTwo);
    full.doCalculations();
    REQUIRE(full.results().size() == 64); // 2^0 .. 2^63
    CHECK(full.results().back().start == std::uint64_t{1} << 63);
    CHECK(full.results().back().iterations == 63);
}

TEST_CASE("mode Primes preserves the original program's output: 1 is listed, "
          "2 is not",
          "[modes]") {
    Collatz c(30, Collatz::Mode::Primes);
    c.doCalculations();
    // The original program tested only odd starting values with a primality
    // test that accepted 1; this quirk is preserved so regenerated primes.txt
    // files match previously published results byte-for-byte.
    CHECK(starts(c) == std::vector<std::uint64_t>{1, 3, 5, 7, 11, 13, 17, 19, 23, 29});
    CHECK(c.results().front().iterations == 0); // iterate(1)
}

TEST_CASE("mode MultiplesOf steps by the chosen multiple", "[modes]") {
    Collatz c(10, Collatz::Mode::MultiplesOf, 3);
    c.doCalculations();
    CHECK(starts(c) == std::vector<std::uint64_t>{3, 6, 9});
}

TEST_CASE("overflowing starting values are skipped and reported", "[modes][overflow]") {
    Collatz c(kMax, Collatz::Mode::MultiplesOf, kMax);
    c.doCalculations();
    CHECK(c.results().empty());
    CHECK(c.overflowed() == std::vector<std::uint64_t>{kMax});
}

TEST_CASE("fileVers names the output file for each mode", "[output]") {
    CHECK(Collatz(10, Collatz::Mode::All).fileVers() == "output.txt");
    CHECK(Collatz(10, Collatz::Mode::Odds).fileVers() == "odds.txt");
    CHECK(Collatz(10, Collatz::Mode::Evens).fileVers() == "evens.txt");
    CHECK(Collatz(10, Collatz::Mode::PowersOfTwo).fileVers() == "pow_two.txt");
    CHECK(Collatz(10, Collatz::Mode::Primes).fileVers() == "primes.txt");
    CHECK(Collatz(10, Collatz::Mode::MultiplesOf, 7).fileVers() == "multiples_of_7.txt");
}

TEST_CASE("print writes one 'START ITERATIONS' pair per line", "[output]") {
    Collatz c(5, Collatz::Mode::All);
    c.doCalculations();
    REQUIRE(c.print());

    std::ifstream fin(c.fileVers());
    REQUIRE(fin.is_open());
    std::vector<std::string> lines;
    for (std::string line; std::getline(fin, line);) {
        lines.push_back(line);
    }
    fin.close();
    CHECK(lines == std::vector<std::string>{"1 0", "2 1", "3 7", "4 2", "5 5"});
    CHECK(std::remove(c.fileVers().c_str()) == 0);
}
