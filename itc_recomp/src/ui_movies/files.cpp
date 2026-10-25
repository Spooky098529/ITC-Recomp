// The UI movie library's file fix-ups (sub_825A0CF8, sub_825A0EF0 and
// their helpers) as plain C++: see ui_movies.h.

#include "ui_movies/ui_movies.h"

#include "base/log.h"

namespace ui_movies {

namespace {
bool BytesAre(uint8_t* base, uint32_t address, const char (&bytes)[8]) {
  for (uint32_t i = 0; i < 8; i++) {
    if (guest::LoadU8(base, address + i) != uint8_t(bytes[i])) {
      return false;
    }
  }
  return true;
}
}  // namespace

bool IsXmd(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_825A0C60 IsXmd");
  return BytesAre(base, file, "XMD\0" "001");
}

uint32_t CheckXmd(uint8_t* base, uint32_t file) {
  return (file & 0xF) == 0 && IsXmd(base, file) ? 1 : 0;
}

bool IsMovieFile(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_825A0E40 IsMovieFile");
  return BytesAre(base, file + 8, "X2D\0" "005");
}

uint32_t FileAnimation(uint8_t* base, uint32_t file, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A0ED8 FileAnimation");
  return guest::LoadU32(base, file + 8 * (guest::LoadU32(base, file + 16) + index + 4));
}

uint32_t FixUpScene(uint8_t* base, uint32_t scene, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_8259D688 FixUpScene");
  uint32_t entry = scene + 128;
  for (uint32_t i = 0; i < guest::LoadU32(base, scene + 124); i++, entry += 8) {
    const uint32_t first = guest::LoadU32(base, scene + 116);
    const uint32_t value = guest::LoadU32(base, entry);
    if (i >= first && i < first + guest::LoadU32(base, scene + 120)) {
      guest::StoreU32(base, entry, FileAnimation(base, file, value));
    } else {
      guest::StoreU32(base, entry, value + scene);
    }
    const uint32_t child = guest::LoadU32(base, entry);
    if (guest::LoadU32(base, child) == kTagSce) {
      FixUpScene(base, child, file);
    }
  }
  return 1;
}

uint32_t FixUpAnimation(uint8_t* base, uint32_t animation, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_825A6570 FixUpAnimation");
  uint32_t entry = animation + 64;
  for (uint32_t i = 0; i < guest::LoadU32(base, animation + 56); i++, entry += 8) {
    guest::StoreU32(base, entry, guest::LoadU32(base, entry) + animation);
  }
  for (uint32_t i = 0; i < guest::LoadU32(base, animation + 52); i++) {
    const uint32_t sub = SubAnimation(base, animation, i);
    guest::StoreU32(base, sub + 16, FileAnimation(base, file, guest::LoadU32(base, sub + 16)));
  }
  return 1;
}

uint32_t FixUpFile(uint8_t* base, uint32_t file) {
  if ((file & 0xF) != 0 || !IsMovieFile(base, file)) {
    return 0;
  }
  uint32_t entry = file + 32;
  for (uint32_t i = 0; i < guest::LoadU32(base, file + 24); i++, entry += 8) {
    guest::StoreU32(base, entry, guest::LoadU32(base, entry) + file);
  }
  FixUpScene(base, FileScene(base, file), file);
  for (uint32_t i = 0; i < guest::LoadU32(base, file + 20); i++) {
    FixUpAnimation(base, FileAnimation(base, file, i), file);
  }
  return 1;
}

}  // namespace ui_movies
