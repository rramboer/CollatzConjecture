#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

/// Parses `text` as a base-10 unsigned 64-bit integer. Returns std::nullopt
/// unless `text` is non-empty, consists only of the digits 0-9, and its value
/// fits in 64 bits.
[[nodiscard]] std::optional<std::uint64_t> parseUnsigned(std::string_view text);
