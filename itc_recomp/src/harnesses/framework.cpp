// The app framework (src/framework/, docs/functions/framework.md) against
// the recompiled functions. The getters and thunks read objects the cases
// make in the scratch area (random words); the debug font's position
// (0x8296BCE0, the game's) is kept and put back around each case.

#include "harness.h"

#include "framework/framework.h"

#include <fmt/format.h>

#include <string>
#include <vector>

namespace harness {

namespace {

namespace fw = framework;

// The register steps: the plain C++ through these, not the adapters.
#define ITC_NATIVE_WORD(name, function)       \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.r3.u64 = fw::function(base, ctx.r3.u32); \
  }

ITC_NATIVE_WORD(NativeFlag133, Flag133)
ITC_NATIVE_WORD(NativeFlag134, Flag134)
ITC_NATIVE_WORD(NativeWord60, Word60)
ITC_NATIVE_WORD(NativeWord64, Word64)
ITC_NATIVE_WORD(NativeWord88, Word88)
ITC_NATIVE_WORD(NativeWord68, Word68)
ITC_NATIVE_WORD(NativeWord44, Word44)
ITC_NATIVE_WORD(NativeInputManagerFrom36, InputManagerFrom36)
ITC_NATIVE_WORD(NativeRendererFrom52, RendererFrom52)
ITC_NATIVE_WORD(NativeWorkerPoolFrom52, WorkerPoolFrom52)
ITC_NATIVE_WORD(NativeRendererOrTextFrom24, RendererOrTextFrom24)
ITC_NATIVE_WORD(NativeSceneFrom52, SceneFrom52)
ITC_NATIVE_WORD(NativeRendererFrom36, RendererFrom36)
ITC_NATIVE_WORD(NativeWorkerPoolFrom36, WorkerPoolFrom36)
ITC_NATIVE_WORD(NativeSceneFrom36, SceneFrom36)

void NativeDebugFontSetPosition(PPCContext& ctx, uint8_t* base) {
  fw::DebugFontSetPosition(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r11.u64 = fw::kDebugFontPosition;
}

void NativeDebugTextSetPosition(PPCContext& ctx, uint8_t* base) {
  const uint32_t x = guest::LoadU32(base, ctx.r4.u32);
  const uint32_t y = guest::LoadU32(base, ctx.r5.u32);
  fw::DebugTextSetPosition(base, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = x;
  ctx.r4.u64 = y;
  ctx.r11.u64 = fw::kDebugFontPosition;
}

struct Getter {
  const char* name;
  GuestFunction recompiled;
  GuestFunction native;
  uint32_t adjust;  // the interface's offset in the object (0 for a getter)
};

// Random words in the scratch area's first 512 bytes, then r3 at an object
// (or an interface `adjust` bytes into it) placed at a random word.
void Getters(Context& h) {
  const Getter getters[] = {
      {"Flag133 (sub_82090760)", __imp__sub_82090760, NativeFlag133, 0},
      {"Flag134 (sub_82090768)", __imp__sub_82090768, NativeFlag134, 0},
      {"Word60 (sub_82090770)", __imp__sub_82090770, NativeWord60, 0},
      {"Word64 (sub_82090778)", __imp__sub_82090778, NativeWord64, 0},
      {"Word88 (sub_82090788)", __imp__sub_82090788, NativeWord88, 0},
      {"Word68 (sub_8248F030)", __imp__sub_8248F030, NativeWord68, 0},
      {"Word44 (sub_824892F8)", __imp__sub_824892F8, NativeWord44, 0},
      {"InputManagerFrom36 (sub_82090830)", __imp__sub_82090830, NativeInputManagerFrom36, 12},
      {"RendererFrom52 (sub_82090828)", __imp__sub_82090828, NativeRendererFrom52, 44},
      {"WorkerPoolFrom52 (sub_82090838)", __imp__sub_82090838, NativeWorkerPoolFrom52, 28},
      {"RendererOrTextFrom24 (sub_82090860)", __imp__sub_82090860, NativeRendererOrTextFrom24, 16},
      {"SceneFrom52 (sub_82090868)", __imp__sub_82090868, NativeSceneFrom52, 32},
      {"RendererFrom36 (sub_820908E0)", __imp__sub_820908E0, NativeRendererFrom36, 28},
      {"WorkerPoolFrom36 (sub_820908E8)", __imp__sub_820908E8, NativeWorkerPoolFrom36, 12},
      {"SceneFrom36 (sub_82090908)", __imp__sub_82090908, NativeSceneFrom36, 16},
  };
  for (const Getter& g : getters) {
    Results results(g.name);
    for (int c = 0; c < 16; c++) {
      std::vector<uint32_t> words(128);
      for (auto& w : words) {
        w = uint32_t(h.random.Next());
      }
      const uint32_t object = h.data + 4 * h.random.Below(64);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, h.data + 4 * i, words[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = object + g.adjust;
        (v == 0 ? g.recompiled : g.native)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("{} case {}", g.name, c));
    }
    results.Report();
  }
}

void Positions(Context& h) {
  Results font("DebugFont::SetPosition (sub_8258BE08)");
  Results text("DebugText::SetPosition (sub_82091050)");
  const std::vector<uint8_t> kept = Read(h.base, fw::kDebugFontPosition, 16);
  const uint32_t edge[][2] = {{64, 64}, {0, 0}, {0xFFFFFFFF, 0x80000000}};
  for (int c = 0; c < 19; c++) {
    const uint32_t x = c < 3 ? edge[c][0] : uint32_t(h.random.Next());
    const uint32_t y = c < 3 ? edge[c][1] : uint32_t(h.random.Next());
    for (int which = 0; which < 2; which++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Fill(h.base, fw::kDebugFontPosition, 16);
        PPCContext t = CallContext(h);
        if (which == 0) {
          t.r3.u64 = x;
          t.r4.u64 = y;
          (v == 0 ? __imp__sub_8258BE08 : NativeDebugFontSetPosition)(t, h.base);
        } else {
          guest::StoreU32(h.base, h.data + 8, x);
          guest::StoreU32(h.base, h.data + 20, y);
          t.r3.u64 = h.data + 100;
          t.r4.u64 = h.data + 8;
          t.r5.u64 = h.data + 20;
          (v == 0 ? __imp__sub_82091050 : NativeDebugTextSetPosition)(t, h.base);
        }
        out[v].Bytes(Read(h.base, fw::kDebugFontPosition, 16));
        out[v].Bytes(Read(h.base, h.data, 64));
        out[v].Word(t.r3.u32);
        out[v].Word(t.r4.u32);
        out[v].Word(t.r11.u32);
      }
      (which == 0 ? font : text).Check(out[0], out[1], fmt::format("case {}", c));
    }
  }
  Write(h.base, fw::kDebugFontPosition, kept.data(), 16);
  font.Report();
  text.Report();
}

}  // namespace

void Framework(Context& h) {
  Getters(h);
  Positions(h);
}

}  // namespace harness
