#include "file_layer/file_layer.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "guest.h"
#include "schema/file_layer.h"
#include "schema/files_and_saves.h"
#include "schema/std_string.h"

namespace file_layer {

uint32_t FileGetSize(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_822B80D8 File::GetSize");
  return schema::File(file).size();
}

void FileConstruct(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_8258C938 File::File");
  const schema::File f(file);
  f.set_position(0);
  f.set_vtable(kFileVtable);
  f.set_handle(0xFFFFFFFFu);
  f.set_size(0);
}

void FileDestruct(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_8258C5F8 File::~File");
  schema::File(file).set_vtable(kStreamVtable);
}

void AsyncFileConstruct(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_82593AC0 AsyncFile::AsyncFile");
  FileConstruct(file);
  schema::GameFile(file).set_vtable(kAsyncFileVtable);
}

void AsyncFileDestruct(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_82593B00 AsyncFile::~AsyncFile");
  schema::GameFile(file).set_vtable(kAsyncFileVtable);
  FileDestruct(file);
}

void GameFileConstruct(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820A6A00 GameFile::GameFile");
  AsyncFileConstruct(file);
  schema::GameFile(file).set_vtable(kGameFileVtable);
}

void GameFileDestruct(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820A6A40 GameFile::~GameFile");
  schema::GameFile(file).set_vtable(kGameFileVtable);
  AsyncFileDestruct(file);
}

void PathConstruct(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_82593C28 Path::Path");
  const schema::Path p(path);
  p.set_text(0);
  p.set_vtable(kPathVtable);
}

void PathDestruct(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_82593C48 Path::~Path");
  schema::Path(path).set_vtable(kPathVtable);
}

void GamePathConstruct(uint32_t path) {
  ITC_NATIVE_RUNNING("sub_820A6BE0 GamePath::GamePath");
  schema::GamePath(path).set_vtable(kGamePathVtable);
}

namespace {

// A std::string's text (std-string.md: in place at +4 under 16 bytes of
// capacity, +24, else the block at +4).
uint32_t StringText(uint32_t string) {
  const schema::StdString s(string);
  return s.capacity() < 16 ? s.text_address() : s.block();
}

using Globals = schema::FileLayerGlobals;
constexpr uint32_t kFormats[3] = {Globals::format_kind0_address(), Globals::format_kind1_address(),
                                  Globals::format_kind2_address()};

}  // namespace

bool PathBuild(uint32_t path, uint32_t kind, uint32_t name, uint32_t stack,
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
      args[count++] = StringText(Globals::prefix_string_address());
      args[count++] = StringText(Globals::project_string_address());
    }
    args[count++] = name;
    for (uint32_t i = 0; i < count; i++) {
      guest::StoreU32(frame + 32 + 8 * i, 0);
      guest::StoreU32(frame + 36 + 8 * i, args[i]);
    }
    int32_t written;
    if (!c_runtime::Sprintf(schema::Path(path).text_address(), kFormats[kind], frame + 32, frame - 144 - 1328,
                            &written)) {
      return false;
    }
  }
  ITC_NATIVE_RUNNING("sub_82593CA0 Path::Build");
  *result = schema::Path(path).text_address();
  return true;
}

bool GamePathSetGame(uint32_t game_path, uint32_t name, uint32_t stack,
                     uint32_t* result) {
  const uint32_t frame = stack - 624;
  const uint32_t path = frame + 80;
  PathConstruct(path);
  uint32_t text;
  if (!PathBuild(path, 0, name, frame, &text)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820A6BF0 GamePath::SetGame");
  uint32_t out = schema::GamePath(game_path).text_address();
  for (;;) {
    const uint8_t c = guest::LoadU8(text++);
    guest::StoreU8(out++, c);
    if (c == 0) break;
  }
  PathDestruct(path);
  *result = schema::GamePath(game_path).text_address();
  return true;
}

bool OpenPathBuild(uint32_t name, uint32_t stack, uint32_t* text) {
  const uint32_t frame = stack - kOpenFrameBytes;
  PathConstruct(frame + 80);
  return PathBuild(frame + 80, 0, name, frame, text);
}

void OpenPathRelease(uint32_t stack) {
  PathDestruct(stack - kOpenFrameBytes + 80);
}

}  // namespace file_layer
