#include "Collatz.h"

#include <cstdint>
#include <fstream>
#include <string>

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
        // 2 is the only even prime, so only odd candidates are tested after it.
        if (num_ >= 2) {
            record(2);
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
    const auto iter = iterate(start);
    if (!iter) {
        if (iter.error() == IterateError::Overflow) {
            overflowed_.push_back(start);
        }
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

bool Collatz::print() const {
    std::ofstream fout(fileVers());
    if (!fout) {
        return false;
    }
    for (const CollatzResult& point : results_) {
        // operator<< rather than std::println: this loop can write millions of
        // lines, and std::println's per-call overhead roughly doubles the time.
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
