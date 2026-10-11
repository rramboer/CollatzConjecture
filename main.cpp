// Interactive driver for the Collatz conjecture calculator.
// Prompts for an upper bound and a mode, then writes the iteration counts
// to the mode's text file. See README.md for details.

#include "Collatz.h"
#include "Input.h"

#include <cstdint>
#include <cstdlib>
#include <exception>
#include <expected>
#include <iostream>
#include <limits>
#include <print>
#include <string>
#include <string_view>

namespace {

constexpr std::uint64_t kMaxInput = std::numeric_limits<std::uint64_t>::max();

// Prints the options menu
void printMenu() {
    std::print(std::cout, "Options:\n"
                          "1. All\n"
                          "2. Odds\n"
                          "3. Evens\n"
                          "4. Powers of Two\n"
                          "5. Primes\n"
                          "6. Multiples of <X>\n");
}

// Reads one whitespace-separated token from stdin and parses it with
// parseUnsigned(). A missing token (EOF) is reported as ParseError::NotANumber.
std::expected<std::uint64_t, ParseError> readNumber() {
    std::string token;
    if (!(std::cin >> token)) {
        return std::unexpected(ParseError::NotANumber);
    }
    return parseUnsigned(token);
}

// Explains why `input` is not a valid `what` (a whole number from 1 to kMaxInput).
void reportInvalidCount(std::string_view what,
                        const std::expected<std::uint64_t, ParseError>& input) {
    if (!input && input.error() == ParseError::OutOfRange) {
        std::println(std::cerr, "Error: the {} is too large; the maximum is {}.", what, kMaxInput);
        return;
    }
    std::println(std::cerr, "Error: the {} must be a whole number from 1 to {}.", what, kMaxInput);
}

// Runs the interactive session and returns the process exit code.
int run() {
    std::print(std::cout, "Enter the upper bound (a positive integer):   ");
    const auto upperBound = readNumber();
    if (!upperBound || *upperBound < 1) {
        reportInvalidCount("upper bound", upperBound);
        return EXIT_FAILURE;
    }

    printMenu();
    std::print(std::cout, "Select a mode:\t");
    const auto choice = readNumber();
    if (!choice || *choice < 1 || *choice > 6) {
        std::println(std::cerr, "Error: the mode must be a number between 1 and 6.");
        return EXIT_FAILURE;
    }
    std::print(std::cout, "\n");

    std::uint64_t multiplesOf = 1;
    if (*choice == 6) {
        std::print(std::cout, "Which integer multiples?\t");
        const auto multiple = readNumber();
        if (!multiple || *multiple < 1) {
            reportInvalidCount("multiple", multiple);
            return EXIT_FAILURE;
        }
        multiplesOf = *multiple;
    }
    std::print(std::cout, "\n");

    Collatz c(*upperBound, static_cast<Collatz::Mode>(*choice), multiplesOf);
    c.doCalculations();

    for (const std::uint64_t start : c.overflowed()) {
        std::println(std::cerr, "Warning: the trajectory of {} exceeds 64 bits and was skipped.",
                     start);
    }

    if (!c.results().empty()) {
        std::println(std::cout, "Maximum iteration count {} reached by starting value {}.",
                     c.maxIter(), c.maxStart());
    }

    if (!c.print()) {
        std::println(std::cerr, "Error: could not write results to '{}'.", c.fileVers());
        return EXIT_FAILURE;
    }

    std::println(std::cout, "\nCalculations completed. Results are in '{}'.", c.fileVers());
    return EXIT_SUCCESS;
}

} // namespace

int main() {
    try {
        return run();
    } catch (const std::exception& e) {
        // Uses iostreams rather than std::println because, by default, they
        // report failures through stream state instead of throwing.
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
