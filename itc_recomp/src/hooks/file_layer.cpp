// The file layer natively: adapters in front of src/file_layer/file_layer.cpp
// (docs/functions/file-layer.md).

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"
#include "file_layer/file_layer.h"
#include "hooks/guest_call.h"

ITC_FLAG_BOOL(native_file_layer, true, "ITC",
              "Run the file layer's functions (docs/functions/file-layer.md: "
              "GameFile::GameFile and ~GameFile, sub_820A6A00 and sub_820A6A40, with "
              "AsyncFile::AsyncFile and ~AsyncFile, sub_82593AC0 and sub_82593B00, and "
              "File::File and ~File, sub_8258C938 and sub_8258C5F8, called from them; "
              "Path::Path and ~Path, "
              "sub_82593C28 and sub_82593C48; GamePath::GamePath, sub_820A6BE0; Path::Build, "
              "sub_82593CA0, over the native sprintf; GamePath::SetGame, sub_820A6BF0; the open "
              "for read by name, sub_820A6AB8, calling File::OpenRead's adapter) natively "
              "instead of through the translated code (same results; checked by "
              "--run_harnesses=file_layer)");

// The constructors and destructors: r3 = the object, left as it is (the
// originals return it). The volatile registers they leave (r9 to r12) are
// read by no caller. AsyncFile's and File's constructors and destructors have
// no adapters: their only callers in play are GameFile's, whose plain C++
// calls theirs (the deleting destructors, never run, call the originals).
#define ITC_FILE_LAYER_OBJECT(address, function) \
  extern "C" REX_FUNC(sub_##address) {          \
    REX_FUNC_PROLOGUE();                        \
    if (!ITC_FLAG(native_file_layer)) {         \
      __imp__sub_##address(ctx, base);          \
      return;                                   \
    }                                           \
    file_layer::function(base, ctx.r3.u32);     \
  }

ITC_FILE_LAYER_OBJECT(820A6A00, GameFileConstruct)
ITC_FILE_LAYER_OBJECT(820A6A40, GameFileDestruct)
ITC_FILE_LAYER_OBJECT(82593C28, PathConstruct)
ITC_FILE_LAYER_OBJECT(82593C48, PathDestruct)
ITC_FILE_LAYER_OBJECT(820A6BE0, GamePathConstruct)

// Path::Build: r3 = the path, r4 = the kind, r5 = the name; r3 the text (0
// for no name). Where the native sprintf declines, the original runs.
extern "C" REX_FUNC(sub_82593CA0) {
  REX_FUNC_PROLOGUE();
  uint32_t result;
  if (!ITC_FLAG(native_file_layer) ||
      !file_layer::PathBuild(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32, &result)) {
    __imp__sub_82593CA0(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
}

// GamePath::SetGame: r3 = the path buffer, r4 = the name; r3 its text.
extern "C" REX_FUNC(sub_820A6BF0) {
  REX_FUNC_PROLOGUE();
  uint32_t result;
  if (!ITC_FLAG(native_file_layer) ||
      !file_layer::GamePathSetGame(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, &result)) {
    __imp__sub_820A6BF0(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
}

// The open for read by name: r3 = the file object, r4 = the name, r5 passed
// on. Its Path natively, then File::OpenRead (its adapter, through ctx: B4's
// file class) from the original's 640-byte frame with its return address;
// r3 1 when the open did (its low byte not 0), else 0.
extern "C" REX_FUNC(sub_820A6AB8) {
  REX_FUNC_PROLOGUE();
  const uint32_t file = ctx.r3.u32;
  const uint64_t flag = ctx.r5.u64;
  uint32_t text;
  if (!ITC_FLAG(native_file_layer) ||
      !file_layer::OpenPathBuild(base, ctx.r4.u32, ctx.r1.u32, &text)) {
    __imp__sub_820A6AB8(ctx, base);
    return;
  }
  ITC_NATIVE_RUNNING("sub_820A6AB8 GameFile::OpenRead (by name)");
  guest_call::InFrame(ctx, base, file_layer::kOpenFrameBytes, 0x820A6AF8, [&](uint32_t) {
    ctx.r3.u64 = file;
    ctx.r4.u64 = text;
    ctx.r5.u64 = flag;
    sub_8258C688(ctx, base);
  });
  const bool opened = (ctx.r3.u32 & 0xFF) != 0;
  file_layer::OpenPathRelease(base, ctx.r1.u32);
  ctx.r3.u64 = opened ? 1 : 0;
}
