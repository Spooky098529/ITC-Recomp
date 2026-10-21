// The player's save, as plain C++: files in a saves/ folder next to the
// executable, laid out our own way (the user's call; the SDK's content folders
// aren't read, and old saves there aren't migrated). The game keeps one save,
// a single file (TXR_DATA_1, 129,400 bytes), read and written whole. See B4
// in docs/rexglue-removal.md.

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace saves {

// The file name in a path the game gives ("TXR360:\TXR_DATA_1" gives
// "TXR_DATA_1"), or nothing when it isn't a plain name (letters, digits,
// '_', '-', '.', not starting with '.').
std::optional<std::string> FileName(std::string_view path);

// Whether the save file `name` exists.
bool Exists(const std::string& name);

// The save file `name`'s bytes, at most `max_bytes`; nothing if it can't be
// read.
std::optional<std::vector<uint8_t>> Read(const std::string& name, uint32_t max_bytes);

// Writes `bytes` as the save file `name`: to a temporary file, then renamed
// over the old one, so a failed write leaves the old save. False on failure.
bool Write(const std::string& name, const uint8_t* bytes, uint32_t size);

// Deletes the save file `name`; true when it's gone.
bool Delete(const std::string& name);

// When the save file `name` was last written, in UTC (the game's own clock
// has no time zone), as "YYYY/MM/DD HH:MM"; nothing if it doesn't exist.
std::optional<std::string> WrittenAt(const std::string& name);

}  // namespace saves
