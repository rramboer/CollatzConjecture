// Interactive driver for the Collatz conjecture calculator.
// Prompts for an upper bound and a mode, then writes the iteration counts
// to the mode's text file. See README.md for details.

#include "Collatz.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace {

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

// Reads one integer from stdin; returns false on non-numeric input or EOF.
bool readNumber(long long& value) { return static_cast<bool>(std::cin >> value); }

} // namespace

int main() {
    long long initNum = 0;
    std::cout << "Enter the upper bound (a positive integer):   ";
    if (!readNumber(initNum) || initNum < 1) {
        std::cerr << "Error: the upper bound must be a positive integer.\n";
        return EXIT_FAILURE;
    }

    printMenu();
    long long choice = 0;
    std::cout << "Select a mode:\t";
    if (!readNumber(choice) || choice < 1 || choice > 6) {
        std::cerr << "Error: the mode must be a number between 1 and 6.\n";
        return EXIT_FAILURE;
    }
    std::cout << '\n';

    long long multiplesOf = 1;
    if (choice == 6) {
        std::cout << "Which integer multiples?\t";
        if (!readNumber(multiplesOf) || multiplesOf < 1) {
            std::cerr << "Error: the multiple must be a positive integer.\n";
            return EXIT_FAILURE;
        }
    }
    std::cout << "\n";

    Collatz c(static_cast<std::uint64_t>(initNum), static_cast<Collatz::Mode>(choice),
              static_cast<std::uint64_t>(multiplesOf));
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
