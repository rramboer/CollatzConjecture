#pragma once

#include <cstdint>
#include <expected>
#include <limits>
#include <string>
#include <vector>

/// A single data point: a starting number and the number of Collatz
/// iterations it takes to reach 1.
struct CollatzResult {
    std::uint64_t start = 0;
    std::uint64_t iterations = 0;
};

/// Computes Collatz iteration counts for a family of starting numbers
/// (all / odds / evens / powers of two / primes / multiples of X) up to an
/// upper bound, tracks the maximum, and writes the results to a text file.
class Collatz {
  public:
    /// Mode selected from the interactive menu.
    enum class Mode : std::uint8_t {
        All = 1,
        Odds = 2,
        Evens = 3,
        PowersOfTwo = 4,
        Primes = 5,
        MultiplesOf = 6,
    };

    /// Why iterate() could not produce an iteration count.
    enum class IterateError : std::uint8_t {
        ZeroStart, ///< 0 maps to itself and never reaches 1.
        Overflow,  ///< A 3n + 1 step would exceed 64 bits.
    };

    Collatz(std::uint64_t upperBound, Mode mode, std::uint64_t multiplesOf = 1);

    /// Computes the iteration count for every starting number selected by
    /// the mode, in ascending order.
    void doCalculations();

    /// Writes the results to the file named by fileVers(), one
    /// "START ITERATIONS" pair per line. Returns false on I/O failure.
    [[nodiscard]] bool print() const;

    /// Returns the output file name for the selected mode.
    [[nodiscard]] std::string fileVers() const;

    /// Returns the number of iterations needed to reach 1 from `start`.
    [[nodiscard]] static constexpr std::expected<std::uint64_t, IterateError>
    iterate(std::uint64_t start) {
        if (start == 0) {
            return std::unexpected(IterateError::ZeroStart);
        }
        std::uint64_t curr = start;
        std::uint64_t numIter = 0;
        while (curr != 1) {
            ++numIter;
            if (curr % 2 == 0) { // even
                curr = curr / 2;
            } else { // odd
                if (curr > (kMax - 1) / 3) {
                    return std::unexpected(IterateError::Overflow);
                }
                curr = (3 * curr) + 1;
            }
        }
        return numIter;
    }

    /// Determines whether `testNum` is prime.
    [[nodiscard]] static constexpr bool isPrime(std::uint64_t testNum) {
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

    [[nodiscard]] const std::vector<CollatzResult>& results() const { return results_; }

    /// Starting numbers whose trajectories overflowed and were skipped.
    [[nodiscard]] const std::vector<std::uint64_t>& overflowed() const { return overflowed_; }

    /// The starting number with the most iterations, and that count.
    /// Meaningful only when results() is non-empty.
    [[nodiscard]] std::uint64_t maxStart() const { return maxStart_; }
    [[nodiscard]] std::uint64_t maxIter() const { return maxIter_; }

  private:
    static constexpr std::uint64_t kMax = std::numeric_limits<std::uint64_t>::max();

    /// Computes and stores one data point, updating the running maximum.
    void record(std::uint64_t start);

    /// Records every value first, first + step, ... that is <= the bound.
    void recordRange(std::uint64_t first, std::uint64_t step);

    Mode mode_;
    std::uint64_t num_;
    std::uint64_t multiplesOf_;
    std::uint64_t maxStart_ = 0;
    std::uint64_t maxIter_ = 0;
    std::vector<CollatzResult> results_;
    std::vector<std::uint64_t> overflowed_;
};
