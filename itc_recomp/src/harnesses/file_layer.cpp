// The file layer's functions (src/file_layer/, docs/functions/file-layer.md)
// against the recompiled ones. The constructors and destructors run on
// objects in the scratch area holding random bytes (a whole Path's 516, so a
// stray write would show), at aligned and odd word offsets. The originals run
// with --native_file_layer off, so their callees are the recompiled code too.

#include "harness.h"

#include "base/flags.h"
#include "file_layer/file_layer.h"

#include <string>

ITC_FLAG_DECLARE(bool, native_file_layer);

namespace harness {

namespace {

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_STEP(name, function)       \
  void name(PPCContext& ctx, uint8_t* base) { \
    file_layer::function(base, ctx.r3.u32);   \
  }

ITC_NATIVE_STEP(NativeGameFileConstruct, GameFileConstruct)
ITC_NATIVE_STEP(NativeGameFileDestruct, GameFileDestruct)
ITC_NATIVE_STEP(NativeAsyncFileConstruct, AsyncFileConstruct)
ITC_NATIVE_STEP(NativeAsyncFileDestruct, AsyncFileDestruct)
ITC_NATIVE_STEP(NativeFileConstruct, FileConstruct)
ITC_NATIVE_STEP(NativeFileDestruct, FileDestruct)
ITC_NATIVE_STEP(NativePathConstruct, PathConstruct)
ITC_NATIVE_STEP(NativePathDestruct, PathDestruct)
ITC_NATIVE_STEP(NativeGamePathConstruct, GamePathConstruct)

#undef ITC_NATIVE_STEP

// The object and the bytes around it compared: 16 before it and 520 from it.
constexpr uint32_t kObject = 1024;
constexpr uint32_t kCompared = 16 + 520;

// One case through both versions: random bytes around the object, the
// original (flag off) or the native step on it; r3 and the bytes compared.
void Case(Context& h, Results& results, GuestFunction recompiled, GuestFunction native,
          uint32_t offset, const std::string& what) {
  std::vector<uint8_t> bytes(kCompared);
  for (auto& b : bytes) {
    b = uint8_t(h.random.Next());
  }
  const uint32_t object = h.data + kObject + offset;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, h.data, kScratchBytes);
    Write(h.base, object - 16, bytes.data(), kCompared);
    PPCContext t = CallContext(h);
    t.r3.u64 = object;
    ITC_FLAG(native_file_layer) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_file_layer) = true;
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    out[v].Bytes(Read(h.base, object - 16, kCompared));
  }
  results.Check(out[0], out[1], what);
}

struct Function {
  const char* name;
  GuestFunction recompiled;
  GuestFunction native;
};

}  // namespace

void FileLayer(Context& h) {
  const Function functions[] = {
      {"GameFile::GameFile", __imp__sub_820A6A00, NativeGameFileConstruct},
      {"GameFile::~GameFile", __imp__sub_820A6A40, NativeGameFileDestruct},
      {"AsyncFile::AsyncFile", __imp__sub_82593AC0, NativeAsyncFileConstruct},
      {"AsyncFile::~AsyncFile", __imp__sub_82593B00, NativeAsyncFileDestruct},
      {"File::File", __imp__sub_8258C938, NativeFileConstruct},
      {"File::~File", __imp__sub_8258C5F8, NativeFileDestruct},
      {"Path::Path", __imp__sub_82593C28, NativePathConstruct},
      {"Path::~Path", __imp__sub_82593C48, NativePathDestruct},
      {"GamePath::GamePath", __imp__sub_820A6BE0, NativeGamePathConstruct},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    // Word-aligned offsets (the game's objects are on 8-aligned stacks), and
    // a few unaligned ones.
    static const uint32_t kOffsets[] = {0, 4, 8, 12, 1, 2, 3};
    for (uint32_t offset : kOffsets) {
      for (int i = 0; i < 4; i++) {
        Case(h, results, f.recompiled, f.native, offset,
             std::string(f.name) + " at +" + std::to_string(offset) + " #" +
                 std::to_string(i));
      }
    }
    results.Report();
  }
}

}  // namespace harness
