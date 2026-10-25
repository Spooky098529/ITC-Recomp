// The game's container files as plain C++ (docs/functions/files-and-saves.md,
// DataFile::; the layout in docs/known-data.md, "container"): a count and one
// offset an entry, little-endian, the entries back to back. The containers
// are guest data the recompiled code reads too, so this reads guest memory.
// Adapter: src/hooks/data_file.cpp.

#pragma once

#include <cstdint>

namespace game_data {

// DataFile::GetEntry (sub_82294730): `container` plus the `index`-th
// little-endian offset at +4, or 0 when `index` isn't below the
// little-endian count at +0 (unsigned); with `skip_empty`, also 0 for an
// entry whose first word is 0. No check for a null container.
uint32_t GetEntry(uint8_t* base, uint32_t container, uint32_t index, bool skip_empty);

}  // namespace game_data
