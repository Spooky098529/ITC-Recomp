// The file layer's functions (src/file_layer/, docs/functions/file-layer.md)
// against the recompiled ones. The constructors and destructors run on
// objects in the scratch area holding random bytes (a whole Path's 516, so a
// stray write would show), at aligned and odd word offsets. The originals run
// with --native_file_layer off, so their callees are the recompiled code too.

#include "harness.h"

#include "base/flags.h"
#include "file_layer/file_layer.h"
#include "guest.h"

#include <fmt/format.h>

#include <cstring>

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

void NativePathBuild(PPCContext& ctx, uint8_t* base) {
  uint32_t result;
  if (!file_layer::PathBuild(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32, &result)) {
    __imp__sub_82593CA0(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
}

void NativeSetGame(PPCContext& ctx, uint8_t* base) {
  uint32_t result;
  if (!file_layer::GamePathSetGame(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, &result)) {
    __imp__sub_820A6BF0(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
}

// A name in the scratch area at `at`: printable bytes (some '%' and '\\',
// some above 127), `length` of them.
void WriteName(Context& h, uint32_t at, uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    const uint32_t pick = h.random.Below(20);
    const uint8_t c = pick == 0 ? '%' : pick == 1 ? '\\' : pick == 2 ? uint8_t(0x80 + h.random.Below(128))
                    : uint8_t('A' + h.random.Below(26));
    guest::StoreU8(h.base, at + i, c);
  }
  guest::StoreU8(h.base, at + length, 0);
}

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

// Path::Build and GamePath::SetGame: the object at the scratch area's +1024
// (520 bytes and the 16 before compared), the name at +2048 (0 to 300
// bytes, at odd addresses too), or a null name; kinds 0 to 4 (kind 2 reads
// the game's two global strings).
void Builds(Context& h) {
  Results build("sub_82593CA0 Path::Build"), set_game("sub_820A6BF0 GamePath::SetGame");
  for (int i = 0; i < 600; i++) {
    const uint32_t kind = i < 300 ? 0 : h.random.Below(5);
    const bool null_name = h.random.Below(20) == 0;
    const uint32_t length = h.random.Below(4) == 0 ? h.random.Below(300) : h.random.Below(40);
    const uint32_t name = h.data + 2048 + h.random.Below(4);
    const uint32_t object = h.data + kObject;
    const Random start = h.random;
    for (int which = 0; which < 2; which++) {
      // (SetGame with a null name copies from address 0, which isn't
      // mapped: left out.)
      if (which == 1 && null_name) continue;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        h.random = start;
        Fill(h.base, h.data, kScratchBytes);
        WriteName(h, name, length);
        PPCContext t = CallContext(h);
        t.r3.u64 = object;
        if (which == 0) {
          t.r4.u64 = kind;
          t.r5.u64 = null_name ? 0 : name;
        } else {
          t.r4.u64 = null_name ? 0 : name;
        }
        ITC_FLAG(native_file_layer) = v == 1;
        if (which == 0) {
          (v == 0 ? __imp__sub_82593CA0 : NativePathBuild)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_820A6BF0 : NativeSetGame)(t, h.base);
        }
        ITC_FLAG(native_file_layer) = true;
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      }
      (which == 0 ? build : set_game)
          .Check(out[0], out[1], fmt::format("kind {} length {}{} case {}", kind, length,
                                             null_name ? " no name" : "", i));
    }
  }
  build.Report();
  set_game.Report();
}

// The open for read by name on real files (as the file class's harness:
// a missing file shows the dirty-disc error, so none), closed after: the
// file object (the handle as open or not), r3 and the scratch area. The
// adapter (the flag on) against the original (off).
void Opens(Context& h) {
  Results results("sub_820A6AB8 GameFile::OpenRead (by name)");
  const char* names[] = {"DATA\\DEBUG\\DEBFONT.XPR", "DATA\\BUILD\\TXR\\BUILD_CRS.TOC",
                         "DATA\\AUDIO\\CAR_SE.XWB"};
  const uint32_t file = h.data + 16, name = h.data + 2048;
  for (const char* n : names) {
    for (int i = 0; i < 4; i++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, name + i, n, uint32_t(std::strlen(n)) + 1);
        PPCContext t = CallContext(h);
        t.r3.u64 = file;
        __imp__sub_820A6A00(t, h.base);
        t = CallContext(h);
        t.r3.u64 = file;
        t.r4.u64 = name + i;
        t.r5.u64 = h.data + 8;
        ITC_FLAG(native_file_layer) = v == 1;
        (v == 0 ? __imp__sub_820A6AB8 : sub_820A6AB8)(t, h.base);
        ITC_FLAG(native_file_layer) = true;
        const uint32_t r3 = t.r3.u32, high = uint32_t(t.r3.u64 >> 32);
        const uint32_t handle = guest::LoadU32(h.base, file + 4);
        const bool open = handle != 0xFFFFFFFF;
        guest::StoreU32(h.base, file + 4, open ? 1 : 0);  // the handle's value differs
        out[v].Word(r3);
        out[v].Word(high);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        if (open) {
          guest::StoreU32(h.base, file + 4, handle);
          t = CallContext(h);
          t.r3.u64 = file;
          sub_8258C718(t, h.base);  // File::Close
        }
      }
      results.Check(out[0], out[1], fmt::format("\"{}\" at +{}", n, i));
    }
  }
  results.Report();
}

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
  Builds(h);
  Opens(h);
}

}  // namespace harness
