// The game's image loaded by us (E2 step 3, docs/rexglue-removal.md "E2:
// the plan"; E3 brought forward): default.xex read, decrypted and laid out
// as the image the code runs over, without the SDK's loader.

#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace app {

struct Image {
  uint32_t load_address = 0;
  uint32_t stack_size = 0;     // the main thread's (the header's default stack size)
  std::vector<uint8_t> bytes;  // the image from load_address on
};

// Loads `path` (only the US release's default.xex, checked by its SHA-1:
// AES-128 encrypted, basic compression). False with `error` set otherwise.
bool LoadImage(const std::filesystem::path& path, Image& image, std::string& error);

}  // namespace app
