// The game's CreateFile (sub_82295508, docs/functions/files-and-saves.md) as
// plain C++: CreateFileA over NtCreateFile, the import called as the
// original calls it (imports stay the SDK's). Not called in play since B4
// (the game's files open natively), but still reached by code (its lifted
// callers). Adapter: src/hooks/create_file.cpp.

#pragma once

#include <cstdint>

namespace game_data {

// CreateFile (sub_82295508): `path` a C string, `access`, `sharing`,
// `disposition` (1 to 5: CREATE_NEW, CREATE_ALWAYS, OPEN_EXISTING,
// OPEN_ALWAYS, TRUNCATE_EXISTING, to NT's 2, 5, 1, 3, 4; 5 needs
// GENERIC_WRITE), `flags` (the flags and attributes; r6, the security
// attributes, isn't read). From its 192-byte frame below `sp`, r13 the PCR.
// The last error set as the original sets it; returns the handle, or -1
// (all 64 bits).
uint64_t CreateFile(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t path, uint32_t access,
                    uint32_t sharing, uint32_t disposition, uint32_t flags);

}  // namespace game_data
