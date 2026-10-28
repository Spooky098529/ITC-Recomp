#include "file_layer/file_layer.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "guest.h"

namespace file_layer {

void FileConstruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_8258C938 File::File");
  guest::StoreU32(base, file + 8, 0);
  guest::StoreU32(base, file + 0, kFileVtable);
  guest::StoreU32(base, file + 4, 0xFFFFFFFFu);
  guest::StoreU32(base, file + 12, 0);
}

void FileDestruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_8258C5F8 File::~File");
  guest::StoreU32(base, file, kStreamVtable);
}

void AsyncFileConstruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_82593AC0 AsyncFile::AsyncFile");
  FileConstruct(base, file);
  guest::StoreU32(base, file, kAsyncFileVtable);
}

void AsyncFileDestruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_82593B00 AsyncFile::~AsyncFile");
  guest::StoreU32(base, file, kAsyncFileVtable);
  FileDestruct(base, file);
}

void GameFileConstruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820A6A00 GameFile::GameFile");
  AsyncFileConstruct(base, file);
  guest::StoreU32(base, file, kGameFileVtable);
}

void GameFileDestruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820A6A40 GameFile::~GameFile");
  guest::StoreU32(base, file, kGameFileVtable);
  AsyncFileDestruct(base, file);
}

void PathConstruct(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_82593C28 Path::Path");
  guest::StoreU8(base, path + 4, 0);
  guest::StoreU32(base, path, kPathVtable);
}

void PathDestruct(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_82593C48 Path::~Path");
  guest::StoreU32(base, path, kPathVtable);
}

void GamePathConstruct(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_820A6BE0 GamePath::GamePath");
  guest::StoreU32(base, path, kGamePathVtable);
}

namespace {

// A std::string's text (std-string.md: in place at +4 under 16 bytes of
// capacity, +24, else the block at +4).
uint32_t StringText(uint8_t* base, uint32_t string) {
  return guest::LoadU32(base, string + 24) < 16 ? string + 4 : guest::LoadU32(base, string + 4);
}

constexpr uint32_t kFormats[3] = {0x8206CC48, 0x8206CC54, 0x8206CC60};
constexpr uint32_t kPrefixString = 0x8265EE7C, kProjectString = 0x8265EE98;

}  // namespace

bool PathBuild(uint8_t* base, uint32_t path, uint32_t kind, uint32_t name, uint32_t stack,
               uint32_t* result) {
  if (name == 0) {
    ITC_NATIVE_RUNNING("sub_82593CA0 Path::Build");
    *result = 0;
    return true;
  }
  if (kind < 3) {
    // sprintf's call from the original's 96-byte frame: its arguments in
    // the frame's parameter area (as sprintf spills them), its own frames
    // below.
    const uint32_t frame = stack - 96;
    uint32_t args[3];
    uint32_t count = 0;
    if (kind == 2) {
      args[count++] = StringText(base, kPrefixString);
      args[count++] = StringText(base, kProjectString);
    }
    args[count++] = name;
    for (uint32_t i = 0; i < count; i++) {
      guest::StoreU32(base, frame + 32 + 8 * i, 0);
      guest::StoreU32(base, frame + 36 + 8 * i, args[i]);
    }
    int32_t written;
    if (!c_runtime::Sprintf(base, path + 4, kFormats[kind], frame + 32, frame - 144 - 1328,
                            &written)) {
      return false;
    }
  }
  ITC_NATIVE_RUNNING("sub_82593CA0 Path::Build");
  *result = path + 4;
  return true;
}

bool GamePathSetGame(uint8_t* base, uint32_t game_path, uint32_t name, uint32_t stack,
                     uint32_t* result) {
  const uint32_t frame = stack - 624;
  const uint32_t path = frame + 80;
  PathConstruct(base, path);
  uint32_t text;
  if (!PathBuild(base, path, 0, name, frame, &text)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820A6BF0 GamePath::SetGame");
  uint32_t out = game_path + 4;
  for (;;) {
    const uint8_t c = guest::LoadU8(base, text++);
    guest::StoreU8(base, out++, c);
    if (c == 0) break;
  }
  PathDestruct(base, path);
  *result = game_path + 4;
  return true;
}

bool OpenPathBuild(uint8_t* base, uint32_t name, uint32_t stack, uint32_t* text) {
  const uint32_t frame = stack - kOpenFrameBytes;
  PathConstruct(base, frame + 80);
  return PathBuild(base, frame + 80, 0, name, frame, text);
}

void OpenPathRelease(uint8_t* base, uint32_t stack) {
  PathDestruct(base, stack - kOpenFrameBytes + 80);
}

}  // namespace file_layer
