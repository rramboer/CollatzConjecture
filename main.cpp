// Interactive driver for the Collatz conjecture calculator.
// Prompts for an upper bound and a mode, then writes the iteration counts
// to the mode's text file. See README.md for details.

#include "Collatz.h"
#include "Input.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <optional>
#include <string>

namespace {

constexpr std::uint64_t kMaxInput = std::numeric_limits<std::uint64_t>::max();

// Prints the options menu
void printMenu() {
    std::cout << "Options:\n"
              << "1. All\n"
              << "2. Odds\n"
              << "3. Evens\n"
              << "4. Powers of Two\n"
              << "5. Primes\n"
              << "6. Multiples of <X>\n";
}

// Reads one whitespace-separated token from stdin and parses it with
// parseUnsigned(). Returns false on EOF or if the token is not a valid number.
bool readNumber(std::uint64_t& value) {
    std::string token;
    if (!(std::cin >> token)) {
        return false;
    }
    const std::optional<std::uint64_t> parsed = parseUnsigned(token);
    if (!parsed) {
        return false;
    }
    value = *parsed;
    return true;
}

} // namespace

int main() {
    std::uint64_t initNum = 0;
    std::cout << "Enter the upper bound (a positive integer):   ";
    if (!readNumber(initNum) || initNum < 1) {
        std::cerr << "Error: the upper bound must be a whole number from 1 to " << kMaxInput
                  << ".\n";
        return EXIT_FAILURE;
    }

    printMenu();
    std::uint64_t choice = 0;
    std::cout << "Select a mode:\t";
    if (!readNumber(choice) || choice < 1 || choice > 6) {
        std::cerr << "Error: the mode must be a number between 1 and 6.\n";
        return EXIT_FAILURE;
    }
    std::cout << '\n';

    std::uint64_t multiplesOf = 1;
    if (choice == 6) {
        std::cout << "Which integer multiples?\t";
        if (!readNumber(multiplesOf) || multiplesOf < 1) {
            std::cerr << "Error: the multiple must be a whole number from 1 to " << kMaxInput
                      << ".\n";
            return EXIT_FAILURE;
        }
    }
    std::cout << "\n";

    Collatz c(initNum, static_cast<Collatz::Mode>(choice), multiplesOf);
    c.doCalculations();

    for (const std::uint64_t start : c.overflowed()) {
        std::cerr << "Warning: the trajectory of " << start
                  << " exceeds 64 bits and was skipped.\n";
    }

    if (!c.results().empty()) {
        std::cout << "Maximum iteration count " << c.maxIter() << " reached by starting value "
                  << c.maxStart() << ".\n";
    }

    if (!c.print()) {
        std::cerr << "Error: could not write results to '" << c.fileVers() << "'.\n";
        return EXIT_FAILURE;
    }

    std::cout << "\nCalculations completed. Results are in '" << c.fileVers() << "'.\n";
    return EXIT_SUCCESS;
}
