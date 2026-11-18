#include "file_layer/file_layer.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "guest.h"
#include "schema/file_layer.h"

namespace file_layer {

uint32_t FileGetSize(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_822B80D8 File::GetSize");
  return guest::LoadU32(base, file + 12);
}

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
  schema::GameFile(base, file).set_vtable(kAsyncFileVtable);
}

void AsyncFileDestruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_82593B00 AsyncFile::~AsyncFile");
  schema::GameFile(base, file).set_vtable(kAsyncFileVtable);
  FileDestruct(base, file);
}

void GameFileConstruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820A6A00 GameFile::GameFile");
  AsyncFileConstruct(base, file);
  schema::GameFile(base, file).set_vtable(kGameFileVtable);
}

void GameFileDestruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820A6A40 GameFile::~GameFile");
  schema::GameFile(base, file).set_vtable(kGameFileVtable);
  AsyncFileDestruct(base, file);
}

void PathConstruct(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_82593C28 Path::Path");
  const schema::Path p(base, path);
  p.set_text(0);
  p.set_vtable(kPathVtable);
}

void PathDestruct(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_82593C48 Path::~Path");
  schema::Path(base, path).set_vtable(kPathVtable);
}

void GamePathConstruct(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_820A6BE0 GamePath::GamePath");
  schema::GamePath(base, path).set_vtable(kGamePathVtable);
}

namespace {

// A std::string's text (std-string.md: in place at +4 under 16 bytes of
// capacity, +24, else the block at +4).
uint32_t StringText(uint8_t* base, uint32_t string) {
  return guest::LoadU32(base, string + 24) < 16 ? string + 4 : guest::LoadU32(base, string + 4);
}

using Globals = schema::FileLayerGlobals;
constexpr uint32_t kFormats[3] = {Globals::format_kind0_address(), Globals::format_kind1_address(),
                                  Globals::format_kind2_address()};

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
      args[count++] = StringText(base, Globals::prefix_string_address());
      args[count++] = StringText(base, Globals::project_string_address());
    }
    args[count++] = name;
    for (uint32_t i = 0; i < count; i++) {
      guest::StoreU32(base, frame + 32 + 8 * i, 0);
      guest::StoreU32(base, frame + 36 + 8 * i, args[i]);
    }
    int32_t written;
    if (!c_runtime::Sprintf(base, schema::Path(base, path).text_address(), kFormats[kind], frame + 32, frame - 144 - 1328,
                            &written)) {
      return false;
    }
  }
  ITC_NATIVE_RUNNING("sub_82593CA0 Path::Build");
  *result = schema::Path(base, path).text_address();
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
  uint32_t out = schema::GamePath(base, game_path).text_address();
  for (;;) {
    const uint8_t c = guest::LoadU8(base, text++);
    guest::StoreU8(base, out++, c);
    if (c == 0) break;
  }
  PathDestruct(base, path);
  *result = schema::GamePath(base, game_path).text_address();
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
