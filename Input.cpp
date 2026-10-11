#include "Input.h"

#include <charconv>
#include <cstdint>
#include <expected>
#include <memory>
#include <string_view>
#include <system_error>

std::expected<std::uint64_t, ParseError> parseUnsigned(std::string_view text) {
    std::uint64_t value = 0;
    const char* const first = std::to_address(text.begin());
    const char* const last = std::to_address(text.end());
    const auto [ptr, ec] = std::from_chars(first, last, value);
    if (ec == std::errc::invalid_argument || ptr != last) {
        return std::unexpected(ParseError::NotANumber);
    }
    if (ec == std::errc::result_out_of_range) {
        return std::unexpected(ParseError::OutOfRange);
    }
    return value;
}
