# Collatz Conjecture

[![CI](https://github.com/rramboer/CollatzConjecture/actions/workflows/ci.yml/badge.svg)](https://github.com/rramboer/CollatzConjecture/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](CMakeLists.txt)

The Collatz conjecture is a conjecture in mathematics that concerns sequences defined as follows:
start with any positive integer n. Then each term is obtained from the previous term as follows:
if the previous term is even, the next term is one half of the previous term.
If the previous term is odd, the next term is 3 times the previous term plus 1.
The conjecture is that no matter what value of n, the sequence will always reach 1.

[WIKIPEDIA LINK](https://en.wikipedia.org/wiki/Collatz_conjecture)

This program computes, for a family of starting numbers up to an upper bound of your choice,
how many iterations each number takes to reach 1, writes the results to a text file, and reports
the starting value with the most iterations.

## Requirements

- A C++23 compiler (CI tests GCC 15 and Clang 22)
- CMake 3.25 or newer

## Building

```sh
cmake --preset release
cmake --build --preset release
```

The executable is written to `build/release/collatz`. Use the `debug` preset for a
warnings-as-errors development build, or `asan` for an Address/UB-sanitizer build.

## Usage

The program is interactive. It prompts for an upper bound, then a mode (and, for mode 6,
which multiples to use):

```text
$ ./build/release/collatz
Enter the upper bound (a positive integer):   100
Options:
1. All
2. Odds
3. Evens
4. Powers of Two
5. Primes
6. Multiples of <X>
Select a mode:  1

Maximum iteration count 118 reached by starting value 97.

Calculations completed. Results are in 'output.txt'.
```

| Mode | Starting numbers tested        | Output file          |
| ---- | ------------------------------ | -------------------- |
| 1    | Every integer from 1 to N      | `output.txt`         |
| 2    | Odd numbers                    | `odds.txt`           |
| 3    | Even numbers                   | `evens.txt`          |
| 4    | Powers of two                  | `pow_two.txt`        |
| 5    | Primes                         | `primes.txt`         |
| 6    | Multiples of X                 | `multiples_of_X.txt` |

Output files are written to the directory the program is run from. Each line is a
space-separated pair `STARTING_NUMBER ITERATIONS`, for example:

```text
7 16
8 3
9 19
```

Notes:

- Every number you enter must be plain decimal digits (no commas, signs, decimals or
  exponents) from 1 to 18,446,744,073,709,551,615 (2^64 − 1). Anything else is rejected
  with an error.
- Starting values whose trajectory would exceed 64 bits are skipped with a warning
  instead of silently overflowing.
- All results are buffered in memory before writing, so very large bounds
  (hundreds of millions and up) are slow and memory-hungry. Bounds up to a few
  million complete in well under a second.

## Regenerating result files

The generated `.txt` outputs are not tracked in git. To regenerate one, run `./collatz`,
enter the upper bound, and pick the matching mode — e.g. `primes.txt` for the first
100,000 integers is bound `100000`, mode `5`. Each run overwrites the mode's output file
in the current directory.

## Testing

Unit tests use [Catch2 v3](https://github.com/catchorg/Catch2) (fetched automatically
by CMake) and run through CTest:

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

## Development

- Format: `clang-format -i *.cpp *.h tests/*.cpp` (style in [.clang-format](.clang-format))
- Lint: `clang-tidy -p build/debug *.cpp` (checks in [.clang-tidy](.clang-tidy))
- CI runs the build/test matrix (GCC and Clang), an ASan/UBSan job, clang-format,
  and clang-tidy on every push and pull request.

## Project structure

| File          | Role                                                            |
| ------------- | --------------------------------------------------------------- |
| `main.cpp`    | Interactive driver: prompts, input validation, result summary    |
| `Collatz.h`   | `Collatz` class, `CollatzResult`, constexpr `iterate`/`isPrime`  |
| `Collatz.cpp` | Mode selection, result recording, file output                   |
| `Input.h`     | `parseUnsigned`, the strict number parser used for all prompts   |
| `Input.cpp`   | `parseUnsigned` implementation                                   |
| `tests/`      | Catch2 unit tests for the computation logic and input parsing    |

## License

Licensed under the [MIT License](LICENSE).
