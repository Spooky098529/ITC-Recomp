// The player's profile as the game reads it (XUserReadProfileSettings, from
// the network manager), as plain hardcoded data: the 26 settings it asks
// for, in its order, with the values the SDK gave. See B5 in
// docs/rexglue-removal.md.

#pragma once

#include <cstdint>
#include <vector>

namespace user {

// The answer's size (the game asks for it first).
constexpr uint32_t kProfileBytes = 4192;

// The answer for user `user_index` into `buffer` (big-endian,
// kProfileBytes), which sits at guest address `address`: a header (count,
// the entries' address), 26 entries of 40 bytes, then the picture key's
// string (the one setting with data). The rest is left as it was.
void ProfileSettings(uint32_t user_index, uint32_t address, std::vector<uint8_t>& buffer);

}  // namespace user
