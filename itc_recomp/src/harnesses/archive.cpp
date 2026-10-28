// The archive's functions (src/archive/, docs/functions/archive.md) against
// the recompiled ones. The TOC lookups read the game's own TOCs (only read:
// the loader thread may be using them), with ids across both archives and
// their edges. The originals run with --native_archive off, so their callees
// are the recompiled code too.

#include "harness.h"

#include "archive/archive.h"
#include "base/flags.h"

#include <fmt/format.h>

#include <string>

ITC_FLAG_DECLARE(bool, native_archive);

namespace harness {

namespace {

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeToc, ctx.r3.u64 = archive::Toc(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeAssetSize, ctx.r3.u64 = archive::AssetSize(base, ctx.r3.u32))

#undef ITC_NATIVE_STEP

// One case through both versions: `setup()` writes the inputs into the
// scratch area (filled first), `args(t)` sets the registers, the original
// (flag off) or the native step runs, and `outputs(t, out)` adds what's
// compared (r3 always, exactly).
template <typename Setup, typename Args, typename Out>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, Out outputs, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_archive) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_archive) = true;
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    outputs(t, out[v]);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

auto NoSetup = [] {};
auto NoOutputs = [](const PPCContext&, Outputs&) {};

// An id in the archives' tables: often an edge, else random in either.
uint32_t TableId(Random& r) {
  const uint32_t crs_count = 1359;
  static const uint32_t kEdges[] = {0, 1, archive::kFirstCrsId - 1, archive::kFirstCrsId,
                                    archive::kFirstCrsId + 1,
                                    archive::kFirstCrsId + crs_count - 1, 22887, 22888, 22889};
  switch (r.Below(4)) {
    case 0:
      return kEdges[r.Below(uint32_t(sizeof(kEdges) / sizeof(kEdges[0])))];
    case 1:
      return archive::kFirstCrsId + r.Below(crs_count);
    default:
      return r.Below(archive::kFirstCrsId);
  }
}

void Tocs(Context& h) {
  {
    Results results("sub_8209D868 Archive::Toc");
    for (int i = 0; i < 300; i++) {
      // Any id (only the two pointers are read), the edges and the tables'.
      uint32_t id = i < 100 ? TableId(h.random) : uint32_t(h.random.Next());
      if (i >= 280) {
        id = 0xFFFFFFFFu - uint32_t(i - 280);
      }
      Case(
          h, results, NoSetup,
          [&](PPCContext& t) {
            t.r3.u64 = (h.random.Next() << 32) | id;  // the upper half: not read
          },
          __imp__sub_8209D868, NativeToc, NoOutputs, fmt::format("case {} (id {})", i, id));
    }
    results.Report();
  }
  {
    Results results("sub_820A0678 Archive::AssetSize");
    for (int i = 0; i < 400; i++) {
      const uint32_t id = TableId(h.random);
      Case(
          h, results, NoSetup, [&](PPCContext& t) { t.r3.u64 = id; }, __imp__sub_820A0678,
          NativeAssetSize, NoOutputs, fmt::format("case {} (id {})", i, id));
    }
    results.Report();
  }
}

}  // namespace

void Archive(Context& h) {
  Tocs(h);
}

}  // namespace harness
