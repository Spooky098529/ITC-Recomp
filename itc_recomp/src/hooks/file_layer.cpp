// The file layer natively: adapters in front of src/file_layer/file_layer.cpp
// (docs/functions/file-layer.md).

#include "guest.h"

#include "base/flags.h"
#include "file_layer/file_layer.h"

ITC_FLAG_BOOL(native_file_layer, true, "ITC",
              "Run the file layer's functions (docs/functions/file-layer.md: "
              "GameFile::GameFile and ~GameFile, sub_820A6A00 and sub_820A6A40, with "
              "AsyncFile::AsyncFile and ~AsyncFile, sub_82593AC0 and sub_82593B00, and "
              "File::File and ~File, sub_8258C938 and sub_8258C5F8, called from them; "
              "Path::Path and ~Path, "
              "sub_82593C28 and sub_82593C48; GamePath::GamePath, sub_820A6BE0) natively "
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
