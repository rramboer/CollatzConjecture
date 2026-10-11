#include "Input.h"

#include <charconv>
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <system_error>

std::optional<std::uint64_t> parseUnsigned(std::string_view text) {
    std::uint64_t value = 0;
    const char* const first = std::to_address(text.begin());
    const char* const last = std::to_address(text.end());
    const auto [ptr, ec] = std::from_chars(first, last, value);
    if (ec != std::errc{} || ptr != last) {
        return std::nullopt;
    }
    return value;
}
