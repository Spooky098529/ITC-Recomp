// The file layer's functions as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/file-layer.md): the file objects' class
// chain (GameFile over AsyncFile over File) and the path objects (Path, the
// game's GamePath). The objects live on recompiled callers' stacks, so these
// work on guest memory. Adapters: src/hooks/file_layer.cpp.

#pragma once

#include <cstdint>

namespace file_layer {

// The vtables (the classes, from the game's down; file-layer.md's intro).
constexpr uint32_t kGameFileVtable = 0x82002270;
constexpr uint32_t kAsyncFileVtable = 0x8206CC0C;
constexpr uint32_t kFileVtable = 0x8206C05C;
constexpr uint32_t kStreamVtable = 0x8206BFF8;
constexpr uint32_t kPathVtable = 0x8206CC3C;
constexpr uint32_t kGamePathVtable = 0x820022A0;

// File::File (sub_8258C938): +0 the vtable, +4 = -1 (closed), +8 = 0 (the
// position), +12 = 0 (the size).
void FileConstruct(uint8_t* base, uint32_t file);
// File::~File (sub_8258C5F8): the base stream's vtable written, nothing else.
void FileDestruct(uint8_t* base, uint32_t file);
// AsyncFile::AsyncFile (sub_82593AC0): File::File, then its vtable.
void AsyncFileConstruct(uint8_t* base, uint32_t file);
// AsyncFile::~AsyncFile (sub_82593B00): its vtable, then File::~File.
void AsyncFileDestruct(uint8_t* base, uint32_t file);
// GameFile::GameFile (sub_820A6A00): AsyncFile::AsyncFile, then its vtable.
void GameFileConstruct(uint8_t* base, uint32_t file);
// GameFile::~GameFile (sub_820A6A40): its vtable, then AsyncFile::~AsyncFile.
// The handle isn't closed.
void GameFileDestruct(uint8_t* base, uint32_t file);

// Path::Path (sub_82593C28): the vtable, the text's first byte (+4) = 0.
void PathConstruct(uint8_t* base, uint32_t path);
// Path::~Path (sub_82593C48): the vtable written again.
void PathDestruct(uint8_t* base, uint32_t path);
// GamePath::GamePath (sub_820A6BE0, also its destructor): the vtable.
void GamePathConstruct(uint8_t* base, uint32_t path);

// Path::Build (sub_82593CA0): `name`'s full path written into the path's
// text (+4) by sprintf with the kind's format: 0 0x8206CC48 ("game:\%s")
// and the name, 1 0x8206CC54 and the name, 2 0x8206CC60 with the two global
// strings' texts (0x8265EE7C, 0x8265EE98) and the name; 3 and up nothing.
// `*result` the text, or 0 for a null name. `stack` is the original's r1 at
// its entry: sprintf's arguments and frame go where the original's would.
// False, with nothing written but sprintf's arguments in the frame, where
// c_runtime::Sprintf declines (the original must run).
bool PathBuild(uint8_t* base, uint32_t path, uint32_t kind, uint32_t name, uint32_t stack,
               uint32_t* result);

// GamePath::SetGame (sub_820A6BF0): a Path on the stack (r1 + 80 in the
// original's 624-byte frame below `stack`) built with kind 0, its text copied
// into `game_path` + 4 (up to its terminator), the Path released.
// `*result` game_path + 4. False, with only the Path's stack bytes written,
// where Path::Build's is.
bool GamePathSetGame(uint8_t* base, uint32_t game_path, uint32_t name, uint32_t stack,
                     uint32_t* result);

// The open for read by name (sub_820A6AB8)'s Path: made at r1 + 80 in the
// original's 640-byte frame below `stack` and built with kind 0; `*text`
// its text. False where Path::Build's is.
constexpr uint32_t kOpenFrameBytes = 640;
bool OpenPathBuild(uint8_t* base, uint32_t name, uint32_t stack, uint32_t* text);
// Its end: the Path released.
void OpenPathRelease(uint8_t* base, uint32_t stack);

}  // namespace file_layer
