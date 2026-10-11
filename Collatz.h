#pragma once

#include <cstdint>
#include <optional>
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

    Collatz(std::uint64_t upperBound, Mode mode, std::uint64_t multiplesOf = 1);

    /// Computes the iteration count for every starting number selected by
    /// the mode, in ascending order.
    void doCalculations();

    /// Writes the results to the file named by fileVers(), one
    /// "START ITERATIONS" pair per line. Returns false on I/O failure.
    [[nodiscard]] bool print() const;

    /// Returns the output file name for the selected mode.
    [[nodiscard]] std::string fileVers() const;

    /// Returns the number of iterations needed to reach 1 from `start`,
    /// or std::nullopt if `start` is 0 or the trajectory would overflow
    /// 64 bits.
    [[nodiscard]] static std::optional<std::uint64_t> iterate(std::uint64_t start);

    /// Determines whether `testNum` is prime.
    [[nodiscard]] static bool isPrime(std::uint64_t testNum);

    [[nodiscard]] const std::vector<CollatzResult>& results() const { return results_; }

    /// Starting numbers whose trajectories overflowed and were skipped.
    [[nodiscard]] const std::vector<std::uint64_t>& overflowed() const { return overflowed_; }

    /// The starting number with the most iterations, and that count.
    /// Meaningful only when results() is non-empty.
    [[nodiscard]] std::uint64_t maxStart() const { return maxStart_; }
    [[nodiscard]] std::uint64_t maxIter() const { return maxIter_; }

  private:
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
