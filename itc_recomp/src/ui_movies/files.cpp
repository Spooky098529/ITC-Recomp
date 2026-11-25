// The UI movie library's file fix-ups (sub_825A0CF8, sub_825A0EF0 and
// their helpers) as plain C++: see ui_movies.h.

#include "ui_movies/ui_movies.h"

#include "base/log.h"

namespace ui_movies {

namespace {
bool BytesAre(uint32_t address, const char (&bytes)[8]) {
  for (uint32_t i = 0; i < 8; i++) {
    if (guest::LoadU8(address + i) != uint8_t(bytes[i])) {
      return false;
    }
  }
  return true;
}
}  // namespace

bool IsXmd(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_825A0C60 IsXmd");
  return BytesAre(schema::UiMovieXmd(file).magic_address(), "XMD\0" "001");
}

uint32_t CheckXmd(uint32_t file) {
  return (file & 0xF) == 0 && IsXmd(file) ? 1 : 0;
}

bool IsMovieFile(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_825A0E40 IsMovieFile");
  return BytesAre(schema::UiMovieFile(file).magic_address(), "X2D\0" "005");
}

uint32_t FileAnimation(uint32_t file, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A0ED8 FileAnimation");
  const schema::UiMovieFile f(file);
  return guest::LoadU32(f.table_address() + 8 * (f.first_animation() + index));
}

uint32_t FixUpScene(uint32_t scene, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_8259D688 FixUpScene");
  uint32_t entry = scene + 128;
  for (uint32_t i = 0; i < guest::LoadU32(scene + 124); i++, entry += 8) {
    const uint32_t first =
        guest::LoadU32(schema::UiMovieNode(scene).texture_left_address());
    const uint32_t value = guest::LoadU32(entry);
    if (i >= first && i < first + guest::LoadU32(scene + 120)) {
      guest::StoreU32(entry, FileAnimation(file, value));
    } else {
      guest::StoreU32(entry, value + scene);
    }
    const uint32_t child = guest::LoadU32(entry);
    if (schema::UiMovieNode(child).tag() == kTagSce) {
      FixUpScene(child, file);
    }
  }
  return 1;
}

uint32_t FixUpAnimation(uint32_t animation, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_825A6570 FixUpAnimation");
  const schema::UiMovieAnimation a(animation);
  uint32_t entry = a.entries_address();
  for (uint32_t i = 0; i < a.entry_count(); i++, entry += 8) {
    guest::StoreU32(entry, guest::LoadU32(entry) + animation);
  }
  for (uint32_t i = 0; i < a.sub_animation_count(); i++) {
    const schema::UiMovieSubAnimation sub(SubAnimation(animation, i));
    sub.set_animation(FileAnimation(file, sub.animation().address));
  }
  return 1;
}

uint32_t FixUpFile(uint32_t file) {
  if ((file & 0xF) != 0 || !IsMovieFile(file)) {
    return 0;
  }
  const schema::UiMovieFile f(file);
  uint32_t entry = f.table_address();
  for (uint32_t i = 0; i < f.table_size(); i++, entry += 8) {
    guest::StoreU32(entry, guest::LoadU32(entry) + file);
  }
  FixUpScene(FileScene(file), file);
  for (uint32_t i = 0; i < f.animation_count(); i++) {
    FixUpAnimation(FileAnimation(file, i), file);
  }
  return 1;
}

}  // namespace ui_movies
