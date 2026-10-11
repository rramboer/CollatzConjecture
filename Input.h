#pragma once

#include <cstdint>
#include <expected>
#include <string_view>

/// Why parseUnsigned() rejected its input.
enum class ParseError : std::uint8_t {
    NotANumber, ///< Empty, or contains anything other than the digits 0-9.
    OutOfRange, ///< Only digits, but the value does not fit in 64 bits.
};

/// Parses `text` as a base-10 unsigned 64-bit integer. Only the digits 0-9
/// are accepted: no sign, whitespace, separators or exponent.
[[nodiscard]] std::expected<std::uint64_t, ParseError> parseUnsigned(std::string_view text);
