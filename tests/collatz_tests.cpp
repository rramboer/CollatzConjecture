#include "Collatz.h"
#include "Input.h"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <cstdio>
#include <expected>
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

using IterateError = Collatz::IterateError;

// iterate() and isPrime() are constexpr, so these checks run at compile time:
// a wrong result fails the build rather than the test run.
TEST_CASE("iterate counts steps to reach 1", "[iterate][constexpr]") {
    STATIC_CHECK(Collatz::iterate(1) == 0U);
    STATIC_CHECK(Collatz::iterate(2) == 1U);
    STATIC_CHECK(Collatz::iterate(3) == 7U);
    STATIC_CHECK(Collatz::iterate(6) == 8U);
    STATIC_CHECK(Collatz::iterate(7) == 16U);
    STATIC_CHECK(Collatz::iterate(27) == 111U);
    STATIC_CHECK(Collatz::iterate(97) == 118U);
}

TEST_CASE("iterate on powers of two returns the exponent", "[iterate]") {
    for (std::uint64_t exponent = 0; exponent < 64; ++exponent) {
        CHECK(Collatz::iterate(std::uint64_t{1} << exponent) == exponent);
    }
}

TEST_CASE("iterate rejects 0 (its trajectory never reaches 1)", "[iterate][constexpr]") {
    STATIC_CHECK(Collatz::iterate(0) == std::unexpected(IterateError::ZeroStart));
}

TEST_CASE("iterate reports overflow instead of wrapping", "[iterate][overflow][constexpr]") {
    // kMax is odd, so the first step is 3n + 1, which cannot fit in 64 bits.
    STATIC_CHECK(Collatz::iterate(kMax) == std::unexpected(IterateError::Overflow));
    // kMax - 1 halves once to an odd value that still overflows 3n + 1.
    STATIC_CHECK(Collatz::iterate(kMax - 1) == std::unexpected(IterateError::Overflow));
    // The smallest odd value whose very first step 3n + 1 overflows.
    constexpr std::uint64_t threshold = (kMax - 1) / 3; // even, so +1 is odd
    STATIC_REQUIRE((threshold + 1) % 2 == 1);
    STATIC_CHECK(Collatz::iterate(threshold + 1) == std::unexpected(IterateError::Overflow));
    // Just below the guard, a power of two halves down without overflowing.
    STATIC_CHECK(Collatz::iterate(std::uint64_t{1} << 62) == 62U);
}

TEST_CASE("isPrime classifies small numbers", "[isPrime][constexpr]") {
    STATIC_CHECK_FALSE(Collatz::isPrime(0));
    STATIC_CHECK_FALSE(Collatz::isPrime(1));
    STATIC_CHECK(Collatz::isPrime(2));
    STATIC_CHECK(Collatz::isPrime(3));
    STATIC_CHECK_FALSE(Collatz::isPrime(4));
    STATIC_CHECK(Collatz::isPrime(5));
    STATIC_CHECK_FALSE(Collatz::isPrime(9));
    STATIC_CHECK_FALSE(Collatz::isPrime(15));
    STATIC_CHECK(Collatz::isPrime(97));
    STATIC_CHECK_FALSE(Collatz::isPrime(1000000));
    STATIC_CHECK(Collatz::isPrime(1000003));
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

TEST_CASE("mode Primes records exactly the primes up to the bound", "[modes]") {
    Collatz c(30, Collatz::Mode::Primes);
    c.doCalculations();
    CHECK(starts(c) == std::vector<std::uint64_t>{2, 3, 5, 7, 11, 13, 17, 19, 23, 29});
    CHECK(c.results().front().iterations == 1); // iterate(2)
}

TEST_CASE("mode Primes handles bounds below 3", "[modes]") {
    Collatz one(1, Collatz::Mode::Primes);
    one.doCalculations();
    CHECK(one.results().empty());

    Collatz two(2, Collatz::Mode::Primes);
    two.doCalculations();
    CHECK(starts(two) == std::vector<std::uint64_t>{2});
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

TEST_CASE("parseUnsigned accepts plain decimal digits across the 64-bit range", "[input]") {
    CHECK(parseUnsigned("0") == 0U);
    CHECK(parseUnsigned("42") == 42U);
    CHECK(parseUnsigned("007") == 7U);
    CHECK(parseUnsigned("9223372036854775808") == std::uint64_t{1} << 63);
    CHECK(parseUnsigned("18446744073709551615") == kMax);
}

TEST_CASE("parseUnsigned rejects text that is not only digits", "[input]") {
    const auto notANumber = std::unexpected(ParseError::NotANumber);
    CHECK(parseUnsigned("") == notANumber);
    CHECK(parseUnsigned("-5") == notANumber);
    CHECK(parseUnsigned("+5") == notANumber);
    CHECK(parseUnsigned("1,000,000") == notANumber);
    CHECK(parseUnsigned("1e6") == notANumber);
    CHECK(parseUnsigned("1.5") == notANumber);
    CHECK(parseUnsigned("12abc") == notANumber);
    CHECK(parseUnsigned("0x10") == notANumber);
    CHECK(parseUnsigned(" 5") == notANumber);
    CHECK(parseUnsigned("99999999999999999999abc") == notANumber);
}

TEST_CASE("parseUnsigned reports values above 2^64 - 1 as out of range", "[input]") {
    const auto outOfRange = std::unexpected(ParseError::OutOfRange);
    CHECK(parseUnsigned("18446744073709551616") == outOfRange); // 2^64
    CHECK(parseUnsigned("99999999999999999999") == outOfRange);
}
