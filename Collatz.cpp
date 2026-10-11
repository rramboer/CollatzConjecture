#include "Collatz.h"

#include <cstdint>
#include <fstream>
#include <limits>
#include <optional>
#include <string>

namespace {
constexpr std::uint64_t kMax = std::numeric_limits<std::uint64_t>::max();
} // namespace

Collatz::Collatz(std::uint64_t upperBound, Mode mode, std::uint64_t multiplesOf)
    : mode_(mode), num_(upperBound), multiplesOf_(multiplesOf) {}

void Collatz::doCalculations() {
    switch (mode_) {
    case Mode::All:
        recordRange(1, 1);
        return;
    case Mode::Odds:
        recordRange(1, 2);
        return;
    case Mode::Evens:
        recordRange(2, 2);
        return;
    case Mode::PowersOfTwo:
        for (std::uint64_t i = 1; i <= num_;) {
            record(i);
            if (i > kMax / 2) {
                break; // doubling again would wrap around
            }
            i *= 2;
        }
        return;
    case Mode::Primes:
        // Reproduces the historical output of this mode byte-for-byte: the
        // original program tested only odd starting values, and its primality
        // test accepted 1, so the first line is "1 0" and 2 never appears.
        // isPrime() itself is mathematically correct; it agrees with the
        // original test for every odd value >= 3 checked by the loop below.
        if (num_ >= 1) {
            record(1);
        }
        for (std::uint64_t i = 3; i <= num_; i += 2) {
            if (isPrime(i)) {
                record(i);
            }
            if (i > kMax - 2) {
                break;
            }
        }
        return;
    case Mode::MultiplesOf:
        recordRange(multiplesOf_, multiplesOf_);
        return;
    }
}

void Collatz::record(std::uint64_t start) {
    const std::optional<std::uint64_t> iter = iterate(start);
    if (!iter) {
        overflowed_.push_back(start);
        return;
    }
    if (results_.empty() || *iter > maxIter_) {
        maxStart_ = start;
        maxIter_ = *iter;
    }
    results_.push_back(CollatzResult{.start = start, .iterations = *iter});
}

void Collatz::recordRange(std::uint64_t first, std::uint64_t step) {
    if (first == 0 || step == 0) {
        return; // defensive: iterate(0) has no answer and step 0 never ends
    }
    for (std::uint64_t i = first; i <= num_;) {
        record(i);
        if (i > kMax - step) {
            break; // the next step would wrap around
        }
        i += step;
    }
}

bool Collatz::isPrime(std::uint64_t testNum) {
    if (testNum < 2) {
        return false;
    }
    if (testNum % 2 == 0) {
        return testNum == 2;
    }
    for (std::uint64_t i = 3; i <= testNum / i; i += 2) {
        if (testNum % i == 0) {
            return false;
        }
    }
    return true;
}

std::optional<std::uint64_t> Collatz::iterate(std::uint64_t start) {
    if (start == 0) {
        return std::nullopt; // 0 -> 0 -> ... never reaches 1
    }
    std::uint64_t curr = start;
    std::uint64_t numIter = 0;
    while (curr != 1) {
        ++numIter;
        if (curr % 2 == 0) { // even
            curr = curr / 2;
        } else { // odd
            if (curr > (kMax - 1) / 3) {
                return std::nullopt; // 3n + 1 would overflow 64 bits
            }
            curr = (3 * curr) + 1;
        }
    }
    return numIter;
}

bool Collatz::print() const {
    std::ofstream fout(fileVers());
    if (!fout) {
        return false;
    }
    for (const CollatzResult& point : results_) {
        fout << point.start << ' ' << point.iterations << '\n';
    }
    fout.flush();
    return fout.good();
}

std::string Collatz::fileVers() const {
    switch (mode_) {
    case Mode::All:
        return "output.txt";
    case Mode::Odds:
        return "odds.txt";
    case Mode::Evens:
        return "evens.txt";
    case Mode::PowersOfTwo:
        return "pow_two.txt";
    case Mode::Primes:
        return "primes.txt";
    case Mode::MultiplesOf:
        return "multiples_of_" + std::to_string(multiplesOf_) + ".txt";
    }
    return "output.txt";
}
