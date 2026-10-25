// The game's data files, as plain C++: files under the data root (the
// executable's folder, or --game_data_root), named by the game's relative
// paths ("DATA\AUDIO\CAR_SE.XWB"; a "game:\" prefix in the game's strings is
// dropped: no mount points or devices). Names match whatever their case on
// Linux (the game asks for CAR_SE.XWB, the file is car_se.xwb). Read-only:
// the game writes only its save (src/saves/). See B4 in
// docs/rexglue-removal.md.

#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string_view>

namespace game_data {

// The folder the game's DATA folder is in. Set once at start-up.
void SetRoot(const std::filesystem::path& root);

// The relative path in a game path: "game:\DATA\X" and "DATA\X" give
// "DATA\X"; nothing for another device ("TXR360:\...").
std::optional<std::string_view> RelativePath(std::string_view game_path);

// The host file a relative path names, or nothing if there's none.
std::optional<std::filesystem::path> Find(std::string_view relative_path);

// Open files. Handle numbers are our own (kFirstHandle up), never -1, 0 or
// one of the SDK's (0xF8000000 up).
constexpr uint32_t kFirstHandle = 0x47440000;
constexpr uint32_t kHandleCount = 0x10000;

inline bool IsHandle(uint32_t handle) {
  return handle - kFirstHandle < kHandleCount;
}

// Opens the file a relative path names for reading, at position 0.
std::optional<uint32_t> Open(std::string_view relative_path);

// Closes a handle; false if it isn't open.
bool Close(uint32_t handle);

// The file's size, or nothing if the handle isn't open.
std::optional<uint64_t> Size(uint32_t handle);

// Reads up to `size` bytes at the handle's position into `out`, moving the
// position on; the bytes read (fewer at the end of the file), or nothing for
// an error.
std::optional<uint32_t> Read(uint32_t handle, uint8_t* out, uint32_t size);

// Reads up to `size` bytes at `offset` into `out`, leaving the position; the
// bytes read, or nothing for an error.
std::optional<uint32_t> ReadAt(uint32_t handle, uint64_t offset, uint8_t* out, uint32_t size);

// Moves the position as SetFilePointer does: from the start (0), the current
// position (1) or the end (2). The new position, or nothing for an error (a
// bad handle or method, or a position below 0).
std::optional<uint64_t> Seek(uint32_t handle, int64_t offset, uint32_t method);

}  // namespace game_data
