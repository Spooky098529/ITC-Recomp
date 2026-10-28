// The archive streaming's functions as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/archive.md and,
// for the older rows, docs/functions/files-and-saves.md): the TOC lookups,
// the request queue's list and slots, the loose files' table, a request's
// finish and Ucl::Unpack. The archive's globals are guest memory shared with
// the recompiled code (the queueing, the picker, the loader thread), so these
// work on guest memory. Adapters: src/hooks/archive.cpp. The functions that
// wait on, signal or read the archive's four semaphores stay recompiled.

#pragma once

#include <cstdint>

namespace archive {

// The first id of BUILD_CRS.DAT (its own table, id - 23011).
constexpr uint32_t kFirstCrsId = 23011;
// The TOCs' pointers: BUILD.TOC, BUILD_CRS.TOC.
constexpr uint32_t kBuildToc = 0x826A5120;
constexpr uint32_t kCrsToc = 0x826A5128;

// Archive::Toc (sub_8209D868): the TOC an asset id is in, [0x826A5120] below
// 23011, else [0x826A5128].
uint32_t Toc(uint8_t* base, uint32_t id);

// Archive::AssetSize (sub_820A0678): the asset's TOC entry (index id + 1, or
// id - 23011 + 1 in BUILD_CRS, 16 bytes each) +8, the unpacked size, or +4,
// the stored size, when that's 0.
uint32_t AssetSize(uint8_t* base, uint32_t id);

}  // namespace archive
