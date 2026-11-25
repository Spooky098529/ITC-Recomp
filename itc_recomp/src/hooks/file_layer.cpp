// The file layer natively: adapters in front of src/file_layer/file_layer.cpp
// (docs/functions/file-layer.md).

#include "guest.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "base/log.h"
#include "file_layer/file_layer.h"
#include "hooks/guest_call.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_file_layer, true, "ITC",
              "Run the file layer's functions (docs/functions/file-layer.md: "
              "GameFile::GameFile and ~GameFile, sub_820A6A00 and sub_820A6A40, with "
              "AsyncFile::AsyncFile and ~AsyncFile, sub_82593AC0 and sub_82593B00, and "
              "File::File and ~File, sub_8258C938 and sub_8258C5F8, called from them; "
              "GamePath::GamePath, sub_820A6BE0; GamePath::SetGame, sub_820A6BF0, and the open "
              "for read by name, sub_820A6AB8 (calling File::OpenRead's adapter), with "
              "Path::Path and ~Path, sub_82593C28 and sub_82593C48, and Path::Build, "
              "sub_82593CA0, over the native sprintf, called from them; File::GetSize "
              "sub_822B80D8) natively "
              "instead of through the translated code (same results; checked by "
              "--run_harnesses=file_layer)");

// The constructors and destructors: r3 = the object, left as it is (the
// originals return it). The volatile registers they leave (r9 to r12) are
// read by no caller. AsyncFile's and File's constructors and destructors have
// no adapters: their only callers in play are GameFile's, whose plain C++
// calls theirs (the deleting destructors, never run, call the originals).
// Path's constructor, destructor and Path::Build have none either: their
// callers in play are the open by name and GamePath::SetGame, whose native
// code calls theirs.
#define ITC_FILE_LAYER_OBJECT(address, function) \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_file_layer)) { \
    file_layer::function(ctx.r3.u32);     \
  }

// File::GetSize: r3 = the file object; its size (+12). From the debug
// font's loader (recompiled) and, as the XMD archive's count, from
// UiMovie::OnLoaded.
ITC_NATIVE_ENTRY(822B80D8, ITC_FLAG(native_file_layer)) {
  ctx.r3.u64 = file_layer::FileGetSize(ctx.r3.u32);
}

ITC_FILE_LAYER_OBJECT(820A6A00, GameFileConstruct)
ITC_FILE_LAYER_OBJECT(820A6A40, GameFileDestruct)
ITC_FILE_LAYER_OBJECT(820A6BE0, GamePathConstruct)

// GamePath::SetGame: r3 = the path buffer, r4 = the name; r3 its text.
ITC_NATIVE_ENTRY_ALWAYS(820A6BF0) {
  
  uint32_t result;
  if (!ITC_FLAG(native_file_layer) ||
      !file_layer::GamePathSetGame(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, &result)) {
    lifted::original::c820A6BF0(ctx);
    return;
  }
  ctx.r3.u64 = result;
}

// The open for read by name: r3 = the file object, r4 = the name, r5 passed
// on. Its Path natively, then File::OpenRead (its adapter, through ctx: B4's
// file class) from the original's 640-byte frame with its return address;
// r3 1 when the open did (its low byte not 0), else 0.
ITC_NATIVE_ENTRY_ALWAYS(820A6AB8) {
  
  const uint32_t file = ctx.r3.u32;
  const uint64_t flag = ctx.r5.u64;
  uint32_t text;
  if (!ITC_FLAG(native_file_layer) ||
      !file_layer::OpenPathBuild(ctx.r4.u32, ctx.r1.u32, &text)) {
    lifted::original::c820A6AB8(ctx);
    return;
  }
  ITC_NATIVE_RUNNING("sub_820A6AB8 GameFile::OpenRead (by name)");
  guest_call::InFrame(ctx, file_layer::kOpenFrameBytes, 0x820A6AF8, [&](uint32_t) {
    ctx.r3.u64 = file;
    ctx.r4.u64 = text;
    ctx.r5.u64 = flag;
    lifted::Enter(ctx, game::File::OpenRead);
  });
  const bool opened = (ctx.r3.u32 & 0xFF) != 0;
  file_layer::OpenPathRelease(ctx.r1.u32);
  ctx.r3.u64 = opened ? 1 : 0;
}
