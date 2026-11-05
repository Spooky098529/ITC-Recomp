// The post effects (src/post_effects/, docs/functions/post-effects.md)
// against the recompiled functions. The effects, screen objects and glows
// are built in a 16 KiB block of their own (the effects' fields are past
// the scratch area's 4 KiB), the game's globals they reach (the 15 effect
// pointers at 0x826C5D80, the scenery manager [0x826C551C] and the scene
// [0x826C2E44]) pointed at it for the case and put back after. Both
// versions run on the same bytes; the block and r3 are compared, exactly
// but for the fields the setters store as floats (and f1). The originals
// run with --native_post_effects off (so the functions they call through
// vtables are the originals too).

#include "harness.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/post_effects.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "post_effects/post_effects.h"

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <bit>
#include <cstring>
#include <initializer_list>
#include <limits>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_post_effects);

namespace harness {

namespace {

namespace pe = post_effects;

// The removed adapters' register steps (ToggleNoHistory, ToggleHidden),
// without the flag.
void NativeToggleNoHistory(PPCContext&, uint8_t* base) { pe::ToggleNoHistory(base); }
void NativeToggleHidden(PPCContext& ctx, uint8_t* base) {
  pe::ToggleScreenObjectHidden(base, ctx.r3.u32);
}

// StarDef::Init: r3 = the star definition, r4 = lines, r5 = passes, f1 =
// sample length, f2 = attenuation, f4 = inclination, r10 = rotation.
void NativeStarDefInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = pe::StarDefInit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64, ctx.f2.f64,
                               ctx.f4.f64, ctx.r10.u32);
}

// RenderTexture::Release: r3 = the entry; the original when the frees
// aren't the layer's (as its adapter was).
void NativeRenderTextureRelease(PPCContext& ctx, uint8_t* base) {
  const auto r3 = pe::RenderTextureRelease(base, ctx.r3.u32);
  if (!r3) {
    __imp__sub_82133070(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

// The removed adapters' steps, without the flag: RenderTextures::Release
// and PostEffect::ReleaseSurface (r3 = the field's address), the
// destructors (r3 = the effect; from a frame like the original's).
void NativeRenderTexturesRelease(PPCContext& ctx, uint8_t* base) {
  hooks::PostEffectsReleaseTexture(ctx, base);
}
void NativeReleaseSurface(PPCContext& ctx, uint8_t* base) {
  hooks::PostEffectsReleaseSurface(ctx, base);
}
template <uint32_t (*Destroy)(uint8_t*, uint32_t, const pe::DestroyCalls&)>
void NativeDestroy(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  const uint32_t effect = ctx.r3.u32;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, base, 112, 0, [&](uint32_t) {
    r3 = Destroy(base, effect, hooks::PostEffectsDestroyCalls(ctx, base));
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

// The removed adapters' steps, without the flag: GlareDef::GlareDef (r3 =
// the glare definition), GlareDef::Init (r4 = the glare type) and
// TargetFilter::TargetFilter (r3 = the object, r4 = the table entry; the
// surface's parameters in a 112-byte frame like the original's, the
// original where the copy isn't native).
void NativeGlareDefConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  pe::GlareDefConstruct(base, ctx.r3.u32);
}
void NativeGlareDefInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = pe::GlareDefInit(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeTargetFilterConstruct(PPCContext& ctx, uint8_t* base) {
  std::optional<uint32_t> r3;
  const uint32_t filter = ctx.r3.u32;
  const uint32_t entry = ctx.r4.u32;
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 112, 0x82135AD4, [&](uint32_t frame) {
    r3 = pe::TargetFilterConstruct(base, frame, filter, entry);
  });
  ctx.lr = lr;
  if (!r3) {
    __imp__sub_82135AA8(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

constexpr uint32_t kBlockBytes = 0x4000;
constexpr uint32_t kGlowVtable = 0x820088B0;  // the game's (slots 2, 6, 8 the setters)

// Runs one version: the recompiled (v == 0, the flag off) or the adapter
// (the flag on).
void Run(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t, uint8_t* base) {
  const bool saved = ITC_FLAG(native_post_effects);
  ITC_FLAG(native_post_effects) = v != 0;
  (v == 0 ? recompiled : native)(t, base);
  ITC_FLAG(native_post_effects) = saved;
}

// A double for f1: ordinary values, then and now NaNs (quiet, signaling,
// negative), infinities, zeros, denormals (as singles and as doubles) and
// values past a single's range.
double RandomDouble(Random& r, double range) {
  static const uint64_t kOdd[] = {
      0x7FF8000000000000ull, 0xFFF8000000000000ull, 0x7FF0000000000001ull,
      0x7FF4000000000000ull, 0x7FF0000000000000ull, 0xFFF0000000000000ull,
      0x0000000000000000ull, 0x8000000000000000ull, 0x0000000000000001ull,
      0x3690000000000000ull, 0x36A0000000000000ull, 0xB690000000000000ull,
      0x47F0000000000000ull, 0xC7F0000000000000ull, 0x47EFFFFFF0000000ull,
      0x3FF0000000000001ull, 0x380FFFFFFFFFFFFFull};
  switch (r.Below(6)) {
    case 0:
      return std::bit_cast<double>(kOdd[r.Below(std::size(kOdd))]);
    case 1:
      return std::bit_cast<double>(r.Next());
    default:
      return (double(r.Next() >> 11) / double(1ull << 53) * 2.0 - 1.0) * range;
  }
}

// Saves guest words and puts them back.
class Saved {
 public:
  Saved(uint8_t* base, uint32_t address, uint32_t words) : base_(base), address_(address) {
    for (uint32_t i = 0; i < words; i++) {
      words_.push_back(guest::LoadU32(base, address + 4 * i));
    }
  }
  ~Saved() {
    for (uint32_t i = 0; i < words_.size(); i++) {
      guest::StoreU32(base_, address_ + 4 * i, words_[i]);
    }
  }

 private:
  uint8_t* base_;
  uint32_t address_;
  std::vector<uint32_t> words_;
};

void RandomBytes(Context& h, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(h.base, address + i, uint32_t(h.random.Next()));
  }
}

// The effects' cases: Get, MarkUpdated, ToggleNoHistory and DestroyRace
// over 15 random effects (each present or not, at a random place in the
// block, with random +32, +44 and +48), DestroyRace's slot 1 being
// ScreenObject::ToggleHidden (which toggles +25 and leaves r3 the object).
void EffectsCases(Context& h, uint32_t block) {
  Saved effects(h.base, pe::kEffects, pe::kCount);
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Function functions[] = {
      {"sub_82133AE8 PostEffects::Get", __imp__sub_82133AE8, sub_82133AE8},
      {"sub_82133A08 PostEffects::MarkUpdated", __imp__sub_82133A08, sub_82133A08},
      {"sub_82133A38 PostEffects::ToggleNoHistory", __imp__sub_82133A38, NativeToggleNoHistory},
      {"sub_82133A70 PostEffects::DestroyRace", __imp__sub_82133A70, sub_82133A70},
  };
  constexpr uint32_t kVtable = 0x3F00;  // in the block: slot 1 ToggleHidden
  for (const Function& f : functions) {
    Results results(f.name);
    for (int c = 0; c < 60; c++) {
      uint32_t pointers[pe::kCount];
      for (uint32_t i = 0; i < pe::kCount; i++) {
        const bool present = c % 10 == 0 ? false : h.random.Below(5) != 0;
        pointers[i] = present ? block + 64 * h.random.Below(200) : 0;
      }
      // Random contents with the flags the game uses, now and then any.
      Random& r = h.random;
      std::vector<uint32_t> words(kBlockBytes / 4);
      for (uint32_t& w : words) {
        w = uint32_t(r.Next());
      }
      for (uint32_t i = 0; i < pe::kCount; i++) {
        if (pointers[i] != 0) {
          const uint32_t at = (pointers[i] - block) / 4;
          words[at] = block + kVtable;
          words[at + 8] = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(8);
          words[at + 11] = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2);
          words[at + 12] = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2);
        }
      }
      words[kVtable / 4 + 1] = 0x8212EDE0;
      const uint32_t index = c < 30 ? r.Below(pe::kCount) : r.Below(64);
      const uint32_t r3 = uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, block + 4 * i, words[i]);
        }
        for (uint32_t i = 0; i < pe::kCount; i++) {
          guest::StoreU32(h.base, pe::kEffects + 4 * i, pointers[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = f.recompiled == __imp__sub_82133AE8 ? index : r3;
        Run(v, f.recompiled, f.native, t, h.base);
        out[v].Bytes(Read(h.base, block, kBlockBytes));
        out[v].Bytes(Read(h.base, pe::kEffects, 4 * pe::kCount));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {} index {}", c, index));
    }
    results.Report();
  }
}

// ScreenObject::ToggleHidden and Race::ToggleEffects: the race's three
// screen objects (each present or not), the effects, the scenery manager
// and the scene in the block, with random bytes.
void ToggleCases(Context& h, uint32_t block) {
  Saved effects(h.base, pe::kEffects, pe::kCount);
  Saved scenery(h.base, 0x826C551C, 1);
  Saved scene(h.base, 0x826C2E44, 1);
  for (bool race : {false, true}) {
    Results results(race ? "sub_8215B690 Race::ToggleEffects" : "sub_8212EDE0 ScreenObject::ToggleHidden");
    for (int c = 0; c < 60; c++) {
      Random& r = h.random;
      std::vector<uint32_t> words(kBlockBytes / 4);
      for (uint32_t& w : words) {
        w = uint32_t(r.Next());
      }
      // Bytes +25 and words +44, +5048's 0 or 1 mostly.
      for (uint32_t at = 0; at < kBlockBytes / 4; at += 16) {
        if (r.Below(2) == 0) {
          words[at + 6] &= 0xFFFF00FFu;  // byte +25 of each 64-byte object
        }
      }
      // The race at block + 0x2000 - 31152: its three pointers at +0x2000.
      const uint32_t race_object = block + 0x2000 - 31152;
      for (uint32_t i = 0; i < 3; i++) {
        words[0x2000 / 4 + i] = r.Below(4) == 0 ? 0 : block + 64 * r.Below(100);
      }
      uint32_t pointers[pe::kCount];
      for (uint32_t i = 0; i < pe::kCount; i++) {
        pointers[i] = r.Below(4) == 0 ? 0 : block + 64 * r.Below(100);
        if (pointers[i] != 0 && r.Below(2) == 0) {
          words[(pointers[i] - block) / 4 + 11] = r.Below(2);
        }
      }
      const uint32_t manager = block + 0x2100;
      const uint32_t scene_object = block + 0x2400 - 5048 + 4 * r.Below(16);
      if (r.Below(2) == 0) {
        words[(scene_object + 5048 - block) / 4] = r.Below(2);
      }
      const uint32_t object = block + 64 * r.Below(100) + r.Below(8);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, block + 4 * i, words[i]);
        }
        for (uint32_t i = 0; i < pe::kCount; i++) {
          guest::StoreU32(h.base, pe::kEffects + 4 * i, pointers[i]);
        }
        guest::StoreU32(h.base, 0x826C551C, manager);
        guest::StoreU32(h.base, 0x826C2E44, scene_object);
        PPCContext t = CallContext(h);
        t.r3.u64 = race ? race_object : object;
        if (race) {
          Run(v, __imp__sub_8215B690, sub_8215B690, t, h.base);
        } else {
          Run(v, __imp__sub_8212EDE0, NativeToggleHidden, t, h.base);
        }
        out[v].Bytes(Read(h.base, block, kBlockBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// The setters: the object placed so its fields land in the block, random
// bytes there, f1 (to f3) random doubles. The fields they store are floats.
void SetterCases(Context& h, uint32_t block) {
  struct Setter {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t offset;
    uint32_t count;  // floats stored from f1 on (SetColor0: 3, and +8092)
  };
  const Setter setters[] = {
      {"sub_82136EF8 Glow::Set8436", __imp__sub_82136EF8, sub_82136EF8, 8436, 1},
      {"sub_82136F00 Glow::Set8440", __imp__sub_82136F00, sub_82136F00, 8440, 1},
      {"sub_82136F08 Glow::Set8444", __imp__sub_82136F08, sub_82136F08, 8444, 1},
      {"sub_82136F10 Glow::Set8448", __imp__sub_82136F10, sub_82136F10, 8448, 1},
      {"sub_82136F18 Glow::SetStrength", __imp__sub_82136F18, sub_82136F18, 8452, 1},
      {"sub_82136F20 Glow::Set8456", __imp__sub_82136F20, sub_82136F20, 8456, 1},
      {"sub_82136F28 Glow::SetHistoryWeight", __imp__sub_82136F28, sub_82136F28, 8460, 1},
      {"sub_82136F30 Glow::Set8464", __imp__sub_82136F30, sub_82136F30, 8464, 1},
      {"sub_82136F38 Glow::Set8468", __imp__sub_82136F38, sub_82136F38, 8468, 1},
      {"sub_82136F40 Glow::Set8472", __imp__sub_82136F40, sub_82136F40, 8472, 1},
      {"sub_82136F48 Glow::Set8476", __imp__sub_82136F48, sub_82136F48, 8476, 1},
      {"sub_821350F8 ColorFilter::SetColor0", __imp__sub_821350F8, sub_821350F8, 8080, 4},
      {"sub_82135138 ColorFilter::Set8112", __imp__sub_82135138, sub_82135138, 8112, 1},
      {"sub_82135140 ColorFilter::Set8116", __imp__sub_82135140, sub_82135140, 8116, 1},
      {"sub_82135148 ColorFilter::Set8120", __imp__sub_82135148, sub_82135148, 8120, 1},
  };
  constexpr uint32_t kField = 0x100;  // where the first field lands
  for (const Setter& s : setters) {
    Results results(s.name);
    for (int c = 0; c < 60; c++) {
      std::vector<uint32_t> words(64);
      for (uint32_t& w : words) {
        w = uint32_t(h.random.Next());
      }
      const double f[3] = {RandomDouble(h.random, 1000.0), RandomDouble(h.random, 1000.0),
                           RandomDouble(h.random, 1000.0)};
      const uint32_t object = block + kField - s.offset;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, block + kField - 64 + 4 * i, words[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = object;
        t.f1.f64 = f[0];
        t.f2.f64 = f[1];
        t.f3.f64 = f[2];
        Run(v, s.recompiled, s.native, t, h.base);
        out[v].Bytes(Read(h.base, block + kField - 64, 64));
        out[v].Floats(h.base, block + kField, s.count);
        out[v].Bytes(Read(h.base, block + kField + 4 * s.count, 256 - 64 - 4 * s.count));
        out[v].Word(t.r3.u32);
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("case {} f1 {:016X}", c, std::bit_cast<uint64_t>(f[0])));
    }
    results.Report();
  }
}

// Glow::SetFromSpeed on a glow with the game's vtable (slots 2, 6 and 8:
// Set8436, SetStrength and SetHistoryWeight): speeds over the game's range
// (0 to 400 km/h) and odd doubles. Compared: the fields from +8432 to
// +8480 (the three set ones as floats), r3 and f1.
void SetFromSpeedCases(Context& h, uint32_t block) {
  Results results("sub_82133878 Glow::SetFromSpeed");
  const uint32_t glow = block + 0x2000 - 8432;
  // The glow's vtable pointer at +0: in the block too (block + 0x2000 - 8432
  // is below it, so the pointer is written there and put back).
  for (int c = 0; c < 80; c++) {
    std::vector<uint32_t> words(12);
    for (uint32_t& w : words) {
      w = uint32_t(h.random.Next());
    }
    const double speed = c < 40 ? double(h.random.Below(4000)) / 10.0 : RandomDouble(h.random, 500.0);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Saved vtable(h.base, glow, 1);
      guest::StoreU32(h.base, glow, kGlowVtable);
      for (uint32_t i = 0; i < words.size(); i++) {
        guest::StoreU32(h.base, block + 0x2000 + 4 * i, words[i]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = glow;
      t.f1.f64 = speed;
      Run(v, __imp__sub_82133878, sub_82133878, t, h.base);
      out[v].Bytes(Read(h.base, block + 0x2000, 4));          // +8432
      out[v].Floats(h.base, block + 0x2000 + 4, 1);           // +8436
      out[v].Bytes(Read(h.base, block + 0x2000 + 8, 12));     // +8440 to +8448
      out[v].Floats(h.base, block + 0x2000 + 20, 1);          // +8452
      out[v].Bytes(Read(h.base, block + 0x2000 + 24, 4));     // +8456
      out[v].Floats(h.base, block + 0x2000 + 28, 1);          // +8460
      out[v].Bytes(Read(h.base, block + 0x2000 + 32, 16));    // +8464 to +8476
      out[v].Word(t.r3.u32);
      out[v].Double(t.f1.u64);
    }
    results.Check(out[0], out[1], fmt::format("case {} speed {:016X}", c, std::bit_cast<uint64_t>(speed)));
  }
  results.Report();
}

// --- The recording helpers ---

// A command list in the block (at kList): the slots used (+12) random, the
// capacity (+8) with room for the helper's slots and a little more (a full
// list makes the original print and write through address 0, so it isn't
// tried), the write pointer (+44) somewhere in the first 8 KiB (filled with
// random words, so the words a helper leaves alone are compared too).
constexpr uint32_t kList = 0x3800;

void RandomList(Context& h, std::vector<uint32_t>& words, uint32_t block, uint32_t slots) {
  Random& r = h.random;
  const uint32_t used = r.Below(4) == 0 ? uint32_t(r.Next()) & 0x7FFFFFFFu : r.Below(5000);
  const uint32_t room = r.Below(3) == 0 ? 1 : 1 + r.Below(100);
  words[kList / 4 + 2] = used + slots + room;  // the capacity: used + slots below it
  words[kList / 4 + 3] = used;
  words[kList / 4 + 11] = block + 16 * r.Below(400) + (r.Below(8) == 0 ? 4 * r.Below(4) : 0);
}

struct RecordCase {
  uint32_t r3, r4, r5, r6, r7, r8;
  double f1, f2;
};

// Runs one helper on the block both ways and compares the block (the
// ranges in `floats` as floats: the vertices AddQuad writes), r3 and f1.
void RecordCheck(Context& h, Results& results, uint32_t block, const std::vector<uint32_t>& words,
                 const RecordCase& c, GuestFunction recompiled, GuestFunction native,
                 uint32_t float_start, uint32_t float_count, const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    for (uint32_t i = 0; i < words.size(); i++) {
      guest::StoreU32(h.base, block + 4 * i, words[i]);
    }
    PPCContext t = CallContext(h);
    t.r3.u64 = c.r3;
    t.r4.u64 = c.r4;
    t.r5.u64 = c.r5;
    t.r6.u64 = c.r6;
    t.r7.u64 = c.r7;
    t.r8.u64 = c.r8;
    t.f1.f64 = c.f1;
    t.f2.f64 = c.f2;
    Run(v, recompiled, native, t, h.base);
    if (float_count != 0) {
      out[v].Bytes(Read(h.base, block, float_start - block));
      out[v].Floats(h.base, float_start, float_count);
      const uint32_t end = float_start + 4 * float_count;
      out[v].Bytes(Read(h.base, end, block + kBlockBytes - end));
    } else {
      out[v].Bytes(Read(h.base, block, kBlockBytes));
    }
    out[v].Word(t.r3.u32);
  }
  results.Check(out[0], out[1], what);
}

uint32_t RandomRegister(Random& r) {
  return r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(256);
}

void RecordCases(Context& h, uint32_t block) {
  struct Helper {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t slots;
  };
  const Helper helpers[] = {
      {"sub_82133340 PostEffect::RecordBeginStates", __imp__sub_82133340, sub_82133340, 24},
      {"sub_82133608 PostEffect::RecordEndStates", __imp__sub_82133608, sub_82133608, 13},
      {"sub_82133790 PostEffect::RecordTargets", __imp__sub_82133790, sub_82133790, 7},
      {"sub_82133D00 PostEffect::RecordPixelConstants", __imp__sub_82133D00, sub_82133D00, 2},
      {"sub_82133D70 PostEffect::RecordPixelBool", __imp__sub_82133D70, sub_82133D70, 2},
      {"sub_82133DD8 PostEffect::RecordTexelSize", __imp__sub_82133DD8, sub_82133DD8, 2},
      {"sub_82133F58 PostEffect::RecordResolve", __imp__sub_82133F58, sub_82133F58, 6},
  };
  Random& r = h.random;
  for (const Helper& helper : helpers) {
    Results results(helper.name);
    for (int c = 0; c < 100; c++) {
      std::vector<uint32_t> words(kBlockBytes / 4);
      for (uint32_t& w : words) {
        w = uint32_t(r.Next());
      }
      RandomList(h, words, block, helper.slots);
      // The effect at 0x3000 (its +16 to +28: the view's size and position),
      // the 16 bytes at 0x2800.
      RecordCase k{};
      k.r3 = block + 0x3000;
      k.r4 = block + kList;
      k.r5 = uint32_t(r.Next());
      k.r6 = uint32_t(r.Next());
      k.r7 = uint32_t(r.Next());
      k.r8 = r.Below(2) == 0 ? 0 : uint32_t(r.Next());
      if (helper.recompiled == __imp__sub_82133D00) {
        k.r5 = block + 0x2800 + 4 * r.Below(16);
        k.r6 = RandomRegister(r);
        k.r7 = r.Below(4) == 0 ? uint32_t(r.Next()) : 1 + r.Below(8);
      } else if (helper.recompiled == __imp__sub_82133D70) {
        k.r5 = r.Below(2) == 0 ? r.Below(2) : uint32_t(r.Next());
        k.r6 = RandomRegister(r);
      } else if (helper.recompiled == __imp__sub_82133F58) {
        k.r6 = r.Below(2) == 0 ? r.Below(1024) : uint32_t(r.Next());
        k.r7 = r.Below(2) == 0 ? 0 : (r.Below(2) == 0 ? uint32_t(-3 - int(r.Below(4))) : uint32_t(r.Next()));
        if (r.Below(8) == 0) {
          k.r7 = 0;
          k.r8 = 0x10000000u << r.Below(4);
        }
      }
      k.f1 = c < 50 ? double(1 + r.Below(1280)) : RandomDouble(r, 4096.0);
      k.f2 = c < 50 ? double(1 + r.Below(720)) : RandomDouble(r, 4096.0);
      RecordCheck(h, results, block, words, k, helper.recompiled, helper.native, 0, 0,
                  fmt::format("case {}", c));
    }
    results.Report();
  }
}

// AddQuad on an effect at the block's start: +8060 (the buffer, 0 or 1),
// +8056 (the vertices used, mostly below 100), +24 and +28 (the view's
// position) and the width and height (the game's, or any 32 bits: the
// original converts them unsigned), f1 random, r7 0 or not (its low word).
void AddQuadCases(Context& h, uint32_t block) {
  Results results("sub_82133E30 PostEffect::AddQuad");
  Random& r = h.random;
  for (int c = 0; c < 200; c++) {
    std::vector<uint32_t> words(kBlockBytes / 4);
    for (uint32_t& w : words) {
      w = uint32_t(r.Next());
    }
    const uint32_t buffer = r.Below(2);
    const uint32_t used = r.Below(5) == 0 ? r.Below(150) : 3 * r.Below(33);
    words[8060 / 4] = buffer;
    words[8056 / 4] = used;
    auto size = [&](uint32_t typical) {
      switch (r.Below(4)) {
        case 0: return uint32_t(r.Next());
        case 1: return r.Below(1 << 24);
        default: return typical >> r.Below(4);
      }
    };
    if (r.Below(2) == 0) {
      words[24 / 4] = r.Below(2) == 0 ? 0 : 288;
      words[28 / 4] = r.Below(2) == 0 ? 0 : 288;
    }
    RecordCase k{};
    k.r3 = block;
    k.r4 = size(1280);
    k.r5 = size(720);
    k.f1 = c < 100 ? double(r.Below(1000)) / 1000.0 : RandomDouble(r, 10.0);
    // r7: 0, 1, or only its upper word set (read as 0); r4's upper word
    // set with it (the original takes the low word).
    const uint64_t r7 = r.Below(5) == 0 ? 0x100000000ull : r.Below(2);
    const uint32_t vertices = block + 56 + 40 * (100 * buffer + used);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t i = 0; i < words.size(); i++) {
        guest::StoreU32(h.base, block + 4 * i, words[i]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = k.r3;
      t.r4.u64 = k.r4 | (uint64_t(r7 >> 32) << 40);
      t.r5.u64 = k.r5;
      t.r7.u64 = r7;
      t.f1.f64 = k.f1;
      Run(v, __imp__sub_82133E30, sub_82133E30, t, h.base);
      out[v].Bytes(Read(h.base, block, vertices - block));
      out[v].Floats(h.base, vertices, 30);
      out[v].Bytes(Read(h.base, vertices + 120, block + kBlockBytes - vertices - 120));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {} buffer {} used {} size {} {} r7 {:X}", c,
                                              buffer, used, k.r4, k.r5, r7));
  }
  results.Report();
}

// --- The glare's definition ---

// GlareDef::GlareDef, GlareDef::Init (the table's eight glare types) and
// StarDef::Init (lines 0 to 13: from 7 on, the lines' words run over the
// inclination and rotation and the colors, which the original writes
// after) on a definition in the block with random bytes. Compared: its
// 288 bytes and the ones around it, exactly (constants and conversions of
// the inputs), r3.
void GlareCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t glare = block + 0x100;
  for (int f = 0; f < 3; f++) {
    Results results(f == 0   ? "sub_8212ED78 GlareDef::GlareDef"
                    : f == 1 ? "sub_8212EC90 GlareDef::Init"
                             : "sub_8212EA98 StarDef::Init");
    for (int c = 0; c < 80; c++) {
      std::vector<uint32_t> words(0x200 / 4);
      for (uint32_t& w : words) {
        w = uint32_t(r.Next());
      }
      RecordCase k{};
      k.r3 = f == 2 ? glare + 48 : glare;
      k.r4 = f == 1 ? r.Below(8) : r.Below(14);
      k.r5 = r.Below(2) == 0 ? 1 + r.Below(3) : uint32_t(r.Next());
      k.f1 = RandomDouble(r, 2.0);
      k.f2 = RandomDouble(r, 2.0);
      const double f4 = RandomDouble(r, 3.2);
      const uint64_t r10 = r.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, block + 4 * i, words[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = k.r3;
        t.r4.u64 = k.r4;
        t.r5.u64 = k.r5;
        t.f1.f64 = k.f1;
        t.f2.f64 = k.f2;
        t.f4.f64 = f4;
        t.r10.u64 = r10;
        if (f == 0) {
          Run(v, __imp__sub_8212ED78, NativeGlareDefConstruct, t, h.base);
        } else if (f == 1) {
          Run(v, __imp__sub_8212EC90, NativeGlareDefInit, t, h.base);
        } else {
          Run(v, __imp__sub_8212EA98, NativeStarDefInit, t, h.base);
        }
        out[v].Bytes(Read(h.base, block, 0x200));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {} r4 {}", c, k.r4));
    }
    results.Report();
  }
}

// --- The render textures and surfaces ---

// Maps the addresses a case made for one version to small numbers, so the
// two versions' outputs compare.
uint32_t Mapped(uint32_t value, std::initializer_list<uint32_t> made) {
  uint32_t i = 1;
  for (uint32_t address : made) {
    if (address != 0 && value == address) {
      return 0xADD00000u + i;
    }
    i++;
  }
  return value;
}

uint32_t MappedIn(uint32_t value, const std::vector<uint32_t>& made) {
  for (uint32_t i = 0; i < made.size(); i++) {
    if (made[i] != 0 && value == made[i]) {
      return 0xADD00000u + i + 1;
    }
  }
  return value;
}

// RenderTexture::Release and RenderTextures::Release: an entry with random
// users (0 to 3, or any) and its header (operator new, 52 bytes) and memory
// (XPhysicalAlloc's, 4 KiB) made for each version; for RenderTextures, the
// entry the pointer at kAt holds is a pool entry in the block (any id but
// 20) or one of its own (operator new, 36 bytes, id 20). Compared: the
// block, the own entry while it's there, r3 and what was freed, the made
// addresses mapped; what's left is freed after.
void RenderTextureCases(Context& h, uint32_t block) {
  Random& r = h.random;
  constexpr uint32_t kAt = 0x80, kPool = 0x100;
  for (bool pool : {false, true}) {
    Results results(pool ? "sub_821332F0 RenderTextures::Release" : "sub_82133070 RenderTexture::Release");
    for (int c = 0; c < 60; c++) {
      std::vector<uint32_t> words(0x200 / 4);
      for (uint32_t& w : words) {
        w = uint32_t(r.Next());
      }
      const uint32_t users = r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(4);
      const bool own = pool && r.Below(2) == 0;
      uint32_t id = r.Below(21);
      if (id == 20 && !own) {
        id = r.Below(20);
      }
      if (own) {
        id = 20;
      }
      std::vector<uint32_t> fields(9);
      for (uint32_t& w : fields) {
        w = uint32_t(r.Next());
      }
      fields[5] = users;
      fields[8] = id;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, block + 4 * i, words[i]);
        }
        const uint32_t header = *memory::crt::OperatorNew(h.base, 52);
        const uint32_t memory = memory::AllocPhysical(4096, 0, 0xFFFFFFFFu, 4096, 0x404);
        const uint32_t entry = own ? *memory::crt::OperatorNew(h.base, 36) : block + kPool;
        fields[6] = header;
        fields[7] = memory;
        for (uint32_t i = 0; i < 9; i++) {
          guest::StoreU32(h.base, entry + 4 * i, fields[i]);
        }
        guest::StoreU32(h.base, block + kAt, entry);
        PPCContext t = CallContext(h);
        t.r3.u64 = pool ? block + kAt : entry;
        if (pool) {
          Run(v, __imp__sub_821332F0, NativeRenderTexturesRelease, t, h.base);
        } else {
          Run(v, __imp__sub_82133070, NativeRenderTextureRelease, t, h.base);
        }
        const std::initializer_list<uint32_t> made = {header, memory, entry};
        for (uint32_t i = 0; i < 0x200; i += 4) {
          out[v].Word(Mapped(guest::LoadU32(h.base, block + i), made));
        }
        const bool own_left = own && memory::IsHeapBlock(entry);
        out[v].Word(own_left);
        if (own_left) {
          for (uint32_t i = 0; i < 9; i++) {
            out[v].Word(Mapped(guest::LoadU32(h.base, entry + 4 * i), made));
          }
        }
        const bool header_left = memory::IsHeapBlock(header);
        out[v].Word(header_left);
        out[v].Word(Mapped(t.r3.u32, made));
        if (header_left) {
          memory::crt::OperatorDelete(header);
          memory::FreePhysical(memory);
        }
        if (own_left) {
          memory::crt::OperatorDelete(entry);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {} users {} id {}", c, users, id));
    }
    results.Report();
  }
}

// PostEffect::ReleaseSurface on a stand-in surface in the block (type 0, a
// count of 2 to 5, so Resource::Release only takes one), with the game's
// video output and device lock. Compared: the pointer, the count, r3.
void ReleaseSurfaceCases(Context& h, uint32_t block) {
  Results results("sub_82133C60 PostEffect::ReleaseSurface");
  constexpr uint32_t kAt = 0x80, kSurface = 0x100;
  for (int c = 0; c < 20; c++) {
    const uint32_t count = 2 + h.random.Below(4);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32(h.base, block + kSurface + 0, 0);
      guest::StoreU32(h.base, block + kSurface + 4, count);
      guest::StoreU32(h.base, block + kAt, block + kSurface);
      PPCContext t = CallContext(h);
      t.r3.u64 = block + kAt;
      Run(v, __imp__sub_82133C60, NativeReleaseSurface, t, h.base);
      out[v].Word(guest::LoadU32(h.base, block + kAt));
      out[v].Word(guest::LoadU32(h.base, block + kSurface + 4));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {} count {}", c, count));
  }
  results.Report();
}

// TargetFilter::TargetFilter on an object at the block's start (random
// bytes), from the game's table entry 14 or a random one (its size, +16
// and +20, up to 2,048), the hi-z taken or not. Compared: the object but
// +8064, the surface's 48-byte header (freed after), the hi-z word, r3.
void TargetFilterCases(Context& h, uint32_t block) {
  Results results("sub_82135AA8 TargetFilter::TargetFilter");
  Random& r = h.random;
  constexpr uint32_t kEntry = 0x3F80;
  constexpr uint32_t kHiZTaken = 0x829537C4;  // the device's hi-z taken (graphics_resources)
  Saved hi_z(h.base, kHiZTaken, 1);
  for (int c = 0; c < 40; c++) {
    std::vector<uint32_t> words(kBlockBytes / 4);
    for (uint32_t& w : words) {
      w = uint32_t(r.Next());
    }
    const bool game = c < 10;
    words[kEntry / 4 + 3] = 1 + r.Below(2048);
    words[kEntry / 4 + 4] = 1 + r.Below(2048);
    const uint32_t entry = game ? 0x826174A8u + 36 * 14 : block + kEntry;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t i = 0; i < words.size(); i++) {
        guest::StoreU32(h.base, block + 4 * i, words[i]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = block;
      t.r4.u64 = entry;
      guest::StoreU32(h.base, kHiZTaken, uint32_t(c & 1));
      Run(v, __imp__sub_82135AA8, NativeTargetFilterConstruct, t, h.base);
      const uint32_t surface = guest::LoadU32(h.base, block + 8064);
      out[v].Bytes(Read(h.base, block, 8064));
      out[v].Bytes(Read(h.base, block + 8068, kBlockBytes - 8068));
      out[v].Word(surface != 0);
      if (surface != 0) {
        out[v].Bytes(Read(h.base, surface, 48));
        memory::crt::XMemFree(surface, 0x24800000);
      }
      out[v].Word(guest::LoadU32(h.base, kHiZTaken));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {} entry {:08X}", c, entry));
  }
  results.Report();
}

// PostEffects::InitEdramBase: the EDRAM base word (random before, put back
// after) and r3.
void InitEdramBaseCases(Context& h) {
  Results results("sub_825D99D0 PostEffects::InitEdramBase");
  Saved saved(h.base, pe::kEdramBase, 1);
  for (int c = 0; c < 4; c++) {
    const uint32_t before = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32(h.base, pe::kEdramBase, before);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.random.Next();
      Run(v, __imp__sub_825D99D0, sub_825D99D0, t, h.base);
      out[v].Word(guest::LoadU32(h.base, pe::kEdramBase));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// --- Making the effects ---

// The removed adapters' register steps, without the flag (r1 = the stack):
// RenderTexture::Create, RenderTextures::Acquire and the constructors.
void NativeTextureCreate(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = pe::RenderTextureCreate(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                       ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32);
}
void NativeAcquire(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = pe::RenderTexturesAcquire(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                         ctx.r6.u32, ctx.r7.u32, ctx.r8.u32);
}
template <uint32_t (*Construct)(uint8_t*, uint32_t, uint32_t, uint32_t)>
void NativeConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = Construct(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
}

constexpr uint32_t kPool = 0x826C5DC8;  // the render textures: 20 of 36 bytes
constexpr uint32_t kPoolWords = 20 * 9;

// The stack below a call's r1 (and the 32 bytes above it, the caller's
// parameter words) filled with the same bytes for both versions.
void FillStack(uint8_t* base, uint32_t sp, const std::vector<uint32_t>& words) {
  const uint32_t from = sp - 4 * uint32_t(words.size() - 8);
  for (uint32_t i = 0; i < words.size(); i++) {
    guest::StoreU32(base, from + 4 * i, words[i]);
  }
}

std::vector<uint32_t> StackWords(Random& r) {
  std::vector<uint32_t> words(0x800 / 4 + 8);
  for (uint32_t& w : words) {
    w = uint32_t(r.Next());
  }
  return words;
}

// A texture header the case made (52 bytes, relocated to `memory`): its
// words, the two address words relative to `memory`'s page.
void HeaderOutputs(Outputs& out, uint8_t* base, uint32_t header, uint32_t memory) {
  for (uint32_t i = 0; i < 52; i += 4) {
    uint32_t w = guest::LoadU32(base, header + i);
    if ((i == 32 || i == 48) && (w & 0xFFFFF000u) != 0) {
      w = (w & 0xFFFu) | ((w & 0xFFFFF000u) - (memory & 0xFFFFF000u));
    }
    out.Word(w);
  }
}

// What a pool entry made in a case holds (users 0 before, 1 after): its
// header (+24) and memory (+28), freed after.
struct Made {
  std::vector<uint32_t> addresses;  // mapped in the outputs
  std::vector<std::pair<uint32_t, uint32_t>> textures;  // header, memory
  std::vector<uint32_t> entries;  // own (id 20) entries
  void Texture(uint8_t* base, uint32_t entry, bool own) {
    const uint32_t header = guest::LoadU32(base, entry + 24);
    const uint32_t memory = guest::LoadU32(base, entry + 28);
    if (own) {
      entries.push_back(entry);
      addresses.push_back(entry);
    }
    textures.push_back({header, memory});
    addresses.push_back(header);
    addresses.push_back(memory);
  }
  void Free() {
    for (const auto& [header, memory] : textures) {
      if (memory::IsHeapBlock(header)) {
        memory::crt::OperatorDelete(header);
      }
      if (memory != 0) {
        memory::FreePhysical(memory);
      }
    }
    for (uint32_t entry : entries) {
      if (memory::IsHeapBlock(entry)) {
        memory::crt::OperatorDelete(entry);
      }
    }
  }
};

// The pool's states for a case: each entry unused (users 0, the rest
// random), or with `used_ok` in use (users 2 to 4) with the key `key(id)`
// gives (shared) or, with `other_ok`, now and then another (then an own
// entry is made).
std::vector<uint32_t> PoolWords(Random& r,
                                const std::function<std::array<uint32_t, 5>(uint32_t)>& key,
                                bool used_ok, bool other_ok) {
  std::vector<uint32_t> words(kPoolWords);
  for (uint32_t& w : words) {
    w = uint32_t(r.Next());
  }
  for (uint32_t id = 0; id < 20; id++) {
    uint32_t* e = &words[9 * id];
    e[8] = id;
    if (!used_ok || r.Below(3) != 0) {
      e[5] = 0;
      continue;
    }
    e[5] = 2 + r.Below(3);
    const auto k = key(id);
    for (int i = 0; i < 5; i++) {
      e[i] = k[i];
    }
    if (other_ok && r.Below(4) == 0) {
      e[1 + r.Below(4)] ^= 1u << r.Below(32);
    }
  }
  return words;
}

// RenderTexture::Create on an entry in the block (random bytes) with random
// sizes and formats (the effects' and others); RenderTextures::Acquire on
// the pool (unused, shared, or in use with another key: an own entry) for
// ids 0 to 20, the stack's word at the original's frame + 80 (read on no
// path) pointing at a stand-in entry. Compared: the entry or the pool, the
// stand-in entry, the own entry, the made header's words (its addresses
// relative), r3, made addresses mapped.
void RenderTextureMakeCases(Context& h, uint32_t block) {
  Random& r = h.random;
  static constexpr uint32_t kFormats[] = {0x1A20015A, 0x2DA2ABA4, 0x2D200199, 0x1A2201BF,
                                          0x18280186};
  constexpr uint32_t kEntry = 0x100, kStray = 0x200;
  {
    Results results("sub_821330D0 RenderTexture::Create");
    for (int c = 0; c < 40; c++) {
      std::vector<uint32_t> words(0x400 / 4);
      for (uint32_t& w : words) {
        w = uint32_t(r.Next());
      }
      const std::vector<uint32_t> stack = StackWords(r);
      const uint32_t format = kFormats[r.Below(std::size(kFormats))];
      const uint32_t width = c < 4 ? 1024u >> c : 1 + r.Below(1024);
      const uint32_t height = c < 4 ? 576u >> c : 1 + r.Below(1024);
      const uint32_t id = r.Below(21), key0 = uint32_t(r.Next()), key1 = uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, block + 4 * i, words[i]);
        }
        PPCContext t = CallContext(h);
        FillStack(h.base, t.r1.u32, stack);
        t.r3.u64 = block + kEntry;
        t.r4.u64 = format;
        t.r5.u64 = width;
        t.r6.u64 = height;
        t.r7.u64 = id;
        t.r8.u64 = key0;
        t.r9.u64 = key1;
        Run(v, __imp__sub_821330D0, NativeTextureCreate, t, h.base);
        Made made;
        made.Texture(h.base, block + kEntry, false);
        for (uint32_t i = 0; i < 36; i += 4) {
          out[v].Word(MappedIn(guest::LoadU32(h.base, block + kEntry + i), made.addresses));
        }
        HeaderOutputs(out[v], h.base, made.textures[0].first, made.textures[0].second);
        out[v].Word(t.r3.u32);
        made.Free();
      }
      results.Check(out[0], out[1], fmt::format("case {} {}x{} format {:08X}", c, width, height,
                                                format));
    }
    results.Report();
  }
  Results results("sub_82133190 RenderTextures::Acquire");
  Saved pool(h.base, kPool, kPoolWords);
  for (int c = 0; c < 80; c++) {
    std::vector<uint32_t> words(0x400 / 4);
    for (uint32_t& w : words) {
      w = uint32_t(r.Next());
    }
    words[(kStray + 20) / 4] = 2;
    words[(kStray + 32) / 4] = 0;
    std::vector<uint32_t> stack = StackWords(r);
    const uint32_t id = c < 21 ? uint32_t(c) : r.Below(21);
    const uint32_t format = kFormats[r.Below(std::size(kFormats))];
    const uint32_t width = 1 + r.Below(1024), height = 1 + r.Below(1024);
    const uint32_t key0 = r.Below(2) == 0 ? 0 : r.Below(1024);
    const uint32_t key1 = r.Below(2) == 0 ? 0 : r.Below(1024);
    const std::vector<uint32_t> pool_words =
        PoolWords(r, [&](uint32_t) { return std::array<uint32_t, 5>{format, width, height, key0, key1}; },
                  true, true);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t i = 0; i < words.size(); i++) {
        guest::StoreU32(h.base, block + 4 * i, words[i]);
      }
      guest::StoreU32s(h.base, kPool, pool_words.data(), kPoolWords);
      PPCContext t = CallContext(h);
      // Acquire's frame's +80: the stand-in entry.
      stack[stack.size() - 8 - 160 / 4 + 80 / 4] = block + kStray;
      FillStack(h.base, t.r1.u32, stack);
      t.r3.u64 = id;
      t.r4.u64 = format;
      t.r5.u64 = width;
      t.r6.u64 = height;
      t.r7.u64 = key0;
      t.r8.u64 = key1;
      Run(v, __imp__sub_82133190, NativeAcquire, t, h.base);
      Made made;
      const bool own = t.r3.u32 < kPool || t.r3.u32 >= kPool + 20 * 36;
      if (own) {
        made.Texture(h.base, t.r3.u32, true);
      } else if (pool_words[9 * id + 5] == 0) {
        made.Texture(h.base, kPool + 36 * id, false);
      }
      for (uint32_t i = 0; i < kPoolWords; i++) {
        out[v].Word(MappedIn(guest::LoadU32(h.base, kPool + 4 * i), made.addresses));
      }
      out[v].Bytes(Read(h.base, block + kStray, 36));
      if (own) {
        for (uint32_t i = 0; i < 36; i += 4) {
          out[v].Word(MappedIn(guest::LoadU32(h.base, t.r3.u32 + i), made.addresses));
        }
      }
      if (!made.textures.empty()) {
        HeaderOutputs(out[v], h.base, made.textures[0].first, made.textures[0].second);
      }
      out[v].Word(MappedIn(t.r3.u32, made.addresses));
      made.Free();
    }
    results.Check(out[0], out[1], fmt::format("case {} id {} users {}", c, id,
                                              id < 20 ? pool_words[9 * id + 5] : 0));
  }
  results.Report();
}

// The pool's entries a made effect holds that the case made (users 0
// before), and its own entries (id 20), in the effect's field order.
void EffectTextures(Made& made, uint8_t* base, uint32_t effect, const std::vector<uint32_t>& fields,
                    const std::vector<uint32_t>& pool_before) {
  for (uint32_t at : fields) {
    const uint32_t entry = guest::LoadU32(base, effect + at);
    if (entry >= kPool && entry < kPool + 20 * 36) {
      const uint32_t id = (entry - kPool) / 36;
      if (pool_before[9 * id + 5] == 0 &&
          std::find(made.addresses.begin(), made.addresses.end(),
                    guest::LoadU32(base, entry + 24)) == made.addresses.end()) {
        made.Texture(base, entry, false);
      }
    } else if (memory::IsHeapBlock(entry) && guest::LoadU32(base, entry + 32) == 20) {
      made.Texture(base, entry, true);
    }
  }
}

struct EffectKind {
  const char* name;
  GuestFunction recompiled, native;
  uint32_t size;
  std::vector<uint32_t> textures, surfaces;
  uint32_t index;  // the game's table entry
};

const std::vector<EffectKind>& EffectKinds() {
  static const std::vector<EffectKind> kinds = [] {
    std::vector<uint32_t> glow_textures;
    for (uint32_t at = 8352; at <= 8408; at += 4) {
      glow_textures.push_back(at);
    }
    return std::vector<EffectKind>{
        {"sub_82138610 Glow::Glow", __imp__sub_82138610, NativeConstruct<pe::GlowConstruct>,
         8480, glow_textures, {8412, 8416, 8420, 8424, 8428, 8432}, 1},
        {"sub_82135008 ColorFilter::ColorFilter", __imp__sub_82135008,
         NativeConstruct<pe::ColorFilterConstruct>, 8128, {8064}, {}, 2},
        {"sub_82136130 MotionBlur::MotionBlur", __imp__sub_82136130,
         NativeConstruct<pe::MotionBlurConstruct>, 8224, {8064, 8068}, {8072, 8076}, 3},
    };
  }();
  return kinds;
}

// The key a constructor asks the pool for an id with (to share it).
std::array<uint32_t, 5> ConstructorKey(uint8_t* base, uint32_t id, uint32_t entry) {
  const uint32_t w = guest::LoadU32(base, entry + 12), hgt = guest::LoadU32(base, entry + 16);
  switch (id) {
    case 1: return {0x1A20015A, w, hgt, 0, 0};
    case 6: return {0x1A20015A, w >> 3, hgt >> 3, 0, 0};
    case 14: return {0x2DA2ABA4, 64, 64, 0, 0};
    case 15: return {0x2DA2ABA4, 16, 16, 0, 0};
    case 16: return {0x2DA2ABA4, 4, 4, 0, 0};
    case 17:
    case 18: return {0x2DA2ABA4, 1, 1, 0, 0};
    default: return {0x1A20015A, w >> 2, hgt >> 2, 0, 0};
  }
}

// Outputs of a made effect: its bytes and the pool's, made addresses
// mapped, the made headers, its surfaces' 48-byte headers (made with
// parameters: no EDRAM). Frees what the case made (surfaces with XMemFree).
void EffectOutputs(Outputs& out, uint8_t* base, const EffectKind& k, uint32_t effect,
                   const std::vector<uint32_t>& pool_before, uint32_t r3, uint32_t stray) {
  Made made;
  made.addresses.push_back(effect);
  std::vector<uint32_t> surfaces;
  for (uint32_t at : k.surfaces) {
    surfaces.push_back(guest::LoadU32(base, effect + at));
    made.addresses.push_back(surfaces.back());
  }
  EffectTextures(made, base, effect, k.textures, pool_before);
  for (uint32_t i = 0; i < k.size; i += 4) {
    const bool single = i >= 8436 && i <= 8476 && k.size == 8480;
    const uint32_t w = guest::LoadU32(base, effect + i);
    if (single) {
      out.Float(w);
    } else {
      out.Word(MappedIn(w, made.addresses));
    }
  }
  for (uint32_t i = 0; i < kPoolWords; i++) {
    out.Word(MappedIn(guest::LoadU32(base, kPool + 4 * i), made.addresses));
  }
  out.Bytes(Read(base, stray, 36));
  for (const auto& [header, memory] : made.textures) {
    HeaderOutputs(out, base, header, memory);
  }
  for (uint32_t surface : surfaces) {
    out.Word(surface != 0);
    if (surface != 0) {
      out.Bytes(Read(base, surface, 48));
    }
  }
  out.Word(MappedIn(r3, made.addresses));
  for (uint32_t surface : surfaces) {
    if (surface != 0) {
      memory::crt::XMemFree(surface, 0x24800000);
    }
  }
  made.Free();
}

// The constructors on an object from operator new (random bytes), from the
// game's table entry (its sizes) or a random one in the block (its sizes,
// position and camera pass), the pool unused or with entries in use with
// the key asked (shared) or another (an own entry made).
void ConstructorCases(Context& h, uint32_t block) {
  Random& r = h.random;
  constexpr uint32_t kEntry = 0x100, kStray = 0x200;
  Saved pool(h.base, kPool, kPoolWords);
  for (const EffectKind& k : EffectKinds()) {
    Results results(k.name);
    for (int c = 0; c < 20; c++) {
      std::vector<uint32_t> words(0x400 / 4);
      for (uint32_t& w : words) {
        w = uint32_t(r.Next());
      }
      words[(kStray + 20) / 4] = 2;
      words[(kStray + 32) / 4] = 0;
      const bool game = c % 2 == 0;
      words[kEntry / 4 + 3] = 1 + r.Below(1280);
      words[kEntry / 4 + 4] = 1 + r.Below(720);
      words[kEntry / 4 + 8] = r.Below(2) == 0 ? 0 : uint32_t(r.Next());
      for (uint32_t i : {5u, 6u}) {
        words[kEntry / 4 + i] = r.Below(2) == 0 ? 0 : r.Below(600);
      }
      const uint32_t entry = game ? 0x826174A8u + 36 * k.index : block + kEntry;
      std::vector<uint32_t> object(k.size / 4);
      for (uint32_t& w : object) {
        w = uint32_t(r.Next());
      }
      const std::vector<uint32_t> stack = StackWords(r);
      const uint64_t r5 = r.Next();
      Outputs out[2];
      std::vector<uint32_t> pool_words;
      for (int v = 0; v < 2; v++) {
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, block + 4 * i, words[i]);
        }
        if (v == 0) {
          pool_words = PoolWords(
              r, [&](uint32_t id) { return ConstructorKey(h.base, id, entry); }, c >= 4, true);
        }
        guest::StoreU32s(h.base, kPool, pool_words.data(), kPoolWords);
        const uint32_t effect = *memory::crt::OperatorNew(h.base, k.size);
        guest::StoreU32s(h.base, effect, object.data(), uint32_t(object.size()));
        PPCContext t = CallContext(h);
        FillStack(h.base, t.r1.u32, stack);
        t.r3.u64 = effect;
        t.r4.u64 = entry;
        t.r5.u64 = r5;
        Run(v, k.recompiled, k.native, t, h.base);
        EffectOutputs(out[v], h.base, k, effect, pool_words, t.r3.u32, block + kStray);
        memory::crt::OperatorDelete(effect);
      }
      results.Check(out[0], out[1], fmt::format("case {} entry {:08X}", c, entry));
    }
    results.Report();
  }
}

// PostEffects::Create for each of the game's 15 entries but kinds 1 and 4
// (0, 4, 8 and 12: their recompiled constructors make what this harness
// can't free), the effect's pointer empty or not, the device given or 0
// (the device lock taken, with the game's video output), the pool
// unused. Compared: the 15 pointers, the made effect (as the constructors'
// cases), the device word, r3. Kinds 1 and 4's skip path (pointer set) is
// covered too.
void CreateCases(Context& h, uint32_t block) {
  Random& r = h.random;
  Results results("sub_82138A18 PostEffects::Create");
  Saved effects(h.base, pe::kEffects, pe::kCount);
  Saved pool(h.base, kPool, kPoolWords);
  constexpr uint32_t kStray = 0x200;
  for (int c = 0; c < 45; c++) {
    const uint32_t index = uint32_t(c % 15);
    const uint32_t kind = guest::LoadU32(h.base, 0x826174A8u + 36 * index);
    const bool present = kind == 1 || kind == 4 || r.Below(4) == 0;
    const uint32_t device = r.Below(2) == 0 ? 0 : uint32_t(r.Next()) | 1;
    std::vector<uint32_t> pointers(pe::kCount);
    for (uint32_t& p : pointers) {
      p = r.Below(2) == 0 ? 0 : block + 0x300;
    }
    pointers[index] = present ? block + 0x300 : 0;
    const std::vector<uint32_t> pool_words = PoolWords(r, nullptr, false, false);
    std::vector<uint32_t> stack = StackWords(r);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, block, 0x400, 0x5A);
      guest::StoreU32(h.base, block + kStray + 20, 2);
      guest::StoreU32(h.base, block + kStray + 32, 0);
      guest::StoreU32s(h.base, pe::kEffects, pointers.data(), pe::kCount);
      guest::StoreU32s(h.base, kPool, pool_words.data(), kPoolWords);
      PPCContext t = CallContext(h);
      FillStack(h.base, t.r1.u32, stack);
      t.r3.u64 = device;
      t.r4.u64 = index;
      Run(v, __imp__sub_82138A18, sub_82138A18, t, h.base);
      const uint32_t made = present ? 0 : guest::LoadU32(h.base, pe::kEffects + 4 * index);
      for (uint32_t i = 0; i < pe::kCount; i++) {
        out[v].Word(i == index && made != 0 ? 1 : guest::LoadU32(h.base, pe::kEffects + 4 * i));
      }
      out[v].Word(guest::LoadU32(h.base, t.r1.u32 + 20));
      uint32_t r3 = t.r3.u32;
      if (made != 0 && r3 == made) {
        r3 = 1;
      }
      out[v].Word(r3);
      if (made != 0) {
        const auto& kinds = EffectKinds();
        if (kind == 5) {
          out[v].Bytes(Read(h.base, made, 8064));
          const uint32_t surface = guest::LoadU32(h.base, made + 8064);
          out[v].Word(surface != 0);
          if (surface != 0) {
            out[v].Bytes(Read(h.base, surface, 48));
            memory::crt::XMemFree(surface, 0x24800000);
          }
        } else {
          const EffectKind& k = kinds[kind == 0 ? 0 : kind == 2 ? 1 : 2];
          EffectOutputs(out[v], h.base, k, made, pool_words, 0, block + kStray);
        }
        memory::crt::OperatorDelete(made);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {} index {} device {:08X} present {}", c,
                                              index, device, present));
  }
  results.Report();
}

// PostEffects::InitBoot: the device given or 0 (the lock taken), effect 14
// there or not. Compared: the vertex declaration (its 56-byte head and
// elements; freed after), the 15 pointers (the made one as 1), the made
// target filter (its bytes, its surface's header; freed after), the device
// word, r3 (the made effect as 1).
void InitBootCases(Context& h, uint32_t block) {
  Random& r = h.random;
  Results results("sub_82138C38 PostEffects::InitBoot");
  Saved effects(h.base, pe::kEffects, pe::kCount);
  Saved declaration(h.base, pe::kVertexDeclaration, 1);
  uint32_t elements = 0;
  while (guest::LoadU16(h.base, 0x826176C4 + 12 * elements) != 255) {
    elements++;
  }
  for (int c = 0; c < 12; c++) {
    const uint32_t device = c % 2 == 0 ? 0 : uint32_t(r.Next()) | 1;
    const bool present = c % 4 >= 2;
    std::vector<uint32_t> pointers(pe::kCount);
    for (uint32_t& p : pointers) {
      p = r.Below(2) == 0 ? 0 : block + 0x300;
    }
    pointers[14] = present ? block + 0x300 : 0;
    const std::vector<uint32_t> stack = StackWords(r);
    const uint32_t before = uint32_t(r.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32s(h.base, pe::kEffects, pointers.data(), pe::kCount);
      guest::StoreU32(h.base, pe::kVertexDeclaration, before);
      PPCContext t = CallContext(h);
      FillStack(h.base, t.r1.u32, stack);
      t.r3.u64 = device;
      Run(v, __imp__sub_82138C38, sub_82138C38, t, h.base);
      const uint32_t decl = guest::LoadU32(h.base, pe::kVertexDeclaration);
      out[v].Word(decl != 0 && decl != before);
      if (decl != 0 && decl != before) {
        out[v].Bytes(Read(h.base, decl, 56 + 12 * (elements + 1)));
        memory::crt::XMemFree(decl, 0x24800000);
      }
      const uint32_t made = present ? 0 : guest::LoadU32(h.base, pe::kEffects + 4 * 14);
      for (uint32_t i = 0; i < pe::kCount; i++) {
        out[v].Word(i == 14 && made != 0 ? 1 : guest::LoadU32(h.base, pe::kEffects + 4 * i));
      }
      out[v].Word(guest::LoadU32(h.base, t.r1.u32 + 20));
      out[v].Word(made != 0 && t.r3.u32 == made ? 1 : t.r3.u32);
      if (made != 0) {
        out[v].Bytes(Read(h.base, made, 8064));
        const uint32_t surface = guest::LoadU32(h.base, made + 8064);
        out[v].Word(surface != 0);
        if (surface != 0) {
          out[v].Bytes(Read(h.base, surface, 48));
          memory::crt::XMemFree(surface, 0x24800000);
        }
        memory::crt::OperatorDelete(made);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {} device {:08X} present {}", c, device,
                                              present));
  }
  results.Report();
}

// --- The records ---

// A made-up scene for the records in the block from +0x2100 (past the
// motion blur's 8,224 bytes), as
// mode_tick's harness makes it: the scene's vtable slot 8 and the
// manager's slots 1 and 2 are getters of the game's (sub_8248CA78 returns
// [r3 + 284], sub_82379320 [r3 + 124]), and the queue's slot 4 is
// BillboardMaterial::SetLights (sub_8209F5C8: +32 = r4, +36 = r5 unless r4
// is 0), which keeps the chunk and key it's handed. `kind`: 0 room, 1 a
// chunk already open on the list, 2 another open (nested); a full list
// isn't made (the original then writes at guest address 0).
constexpr uint32_t kRecWorld = 0x2100, kRecScene = kRecWorld, kRecSceneVtable = kRecWorld + 0x140,
                   kRecManager = kRecWorld + 0x180, kRecManagerVtable = kRecWorld + 0x2C0,
                   kRecQueue = kRecWorld + 0x300, kRecQueueVtable = kRecWorld + 0x340,
                   kRecList = kRecWorld + 0x380, kRecSlots = kRecWorld + 0x400, kRecWorldEnd = kRecWorld + 0xA00;

void SetRecordScene(uint8_t* base, uint32_t block, uint32_t kind, uint32_t used) {
  const uint32_t d = block;
  guest::StoreU32(base, d + kRecScene, d + kRecSceneVtable);
  guest::StoreU32(base, d + kRecSceneVtable + 32, 0x8248CA78);
  guest::StoreU32(base, d + kRecScene + 284, d + kRecManager);
  guest::StoreU32(base, d + kRecManager, d + kRecManagerVtable);
  guest::StoreU32(base, d + kRecManagerVtable + 4, 0x8248CA78);
  guest::StoreU32(base, d + kRecManagerVtable + 8, 0x82379320);
  guest::StoreU32(base, d + kRecManager + 284, d + kRecList);
  guest::StoreU32(base, d + kRecManager + 124, d + kRecQueue);
  guest::StoreU32(base, d + kRecQueue, d + kRecQueueVtable);
  guest::StoreU32(base, d + kRecQueueVtable + 16, 0x8209F5C8);
  const uint32_t list = d + kRecList;
  guest::StoreU32(base, list + 8, used + 1000);
  guest::StoreU32(base, list + 12, used);
  guest::StoreU32(base, list + 32, d + kRecSlots);
  guest::StoreU32(base, list + 44, d + kRecSlots);
  guest::StoreU8(base, list + 52, kind == 1 ? 1 : 0);
  if (kind == 1) {
    guest::StoreU32(base, list + 44, d + kRecSlots + 16);
  }
  guest::StoreU32(base, 0x8296BD28, kind == 2 ? 1 : 0);  // the open chunks
  guest::StoreU32(base, 0x826BABA0, d + kRecScene);
}

// ColorFilter::Record, TargetFilter::Record and MotionBlur::Record on an
// effect at the block's start (random bytes; its vertex buffer's place
// small, its texture entries in the block, its size up to 1,280 x 720, its
// floats random with NaNs and infinities, its flags any of the four; the
// motion blur's camera pass 0 to 12, +44 and +48), the made-up scene (now
// and then a chunk open), camera passes 0 to 12 active or not and render
// targets 3 and 4 set or 0. Compared: the block (the effect, the scene, the list and
// its slots), the open chunks' count, r3.
void EffectRecordCases(Context& h, uint32_t block) {
  Random& r = h.random;
  Saved scene(h.base, 0x826BABA0, 1);
  Saved open_chunks(h.base, 0x8296BD28, 1);
  std::vector<Saved> actives;
  actives.reserve(13);
  for (uint32_t pass = 0; pass < 13; pass++) {
    actives.emplace_back(h.base, 0x826BAC00 + pass * 592 + 588, 1);
  }
  Saved target3(h.base, 0x826BABC8, 1);
  Saved target4(h.base, 0x826BABD4, 1);
  constexpr uint32_t kTextureEntry = 0x3000;
  struct Record {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t size;
  };
  const Record records[] = {
      {"sub_821351F0 ColorFilter::Record", __imp__sub_821351F0, sub_821351F0, 8128},
      {"sub_82135B50 TargetFilter::Record", __imp__sub_82135B50, sub_82135B50, 8068},
      {"sub_821363F0 MotionBlur::Record", __imp__sub_821363F0, sub_821363F0, 8224},
  };
  for (const Record& k : records) {
    Results results(k.name);
    for (int c = 0; c < 100; c++) {
      std::vector<uint32_t> words(kBlockBytes / 4);
      for (uint32_t& w : words) {
        w = uint32_t(r.Next());
      }
      words[8056 / 4] = r.Below(98);
      words[8060 / 4] = r.Below(2);
      words[16 / 4] = 1 + r.Below(1280);
      words[20 / 4] = 1 + r.Below(720);
      words[24 / 4] = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(640);
      words[28 / 4] = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(360);
      words[32 / 4] = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(4);
      if (k.size == 8128) {
        words[8064 / 4] = block + kTextureEntry;
        for (uint32_t at = 8080; at <= 8120; at += 4) {
          words[at / 4] = std::bit_cast<uint32_t>(float(RandomDouble(r, 2.0)));
        }
        words[8068 / 4] = r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2);
        words[8072 / 4] = r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2);
      }
      if (k.size == 8224) {
        // The motion blur: its two textures' entries in the block, its
        // camera pass 0 to 12, +44 and +48 0 or 1 (or any), its floats
        // random, its matrices random words.
        words[8064 / 4] = block + kTextureEntry;
        words[8068 / 4] = block + kTextureEntry + 0x40;
        words[36 / 4] = r.Below(13);
        words[44 / 4] = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2);
        words[48 / 4] = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2);
        for (uint32_t at = 8208; at <= 8220; at += 4) {
          words[at / 4] = std::bit_cast<uint32_t>(float(RandomDouble(r, 2.0)));
        }
      }
      const uint32_t kind = r.Below(8) == 0 ? 1 + r.Below(2) : 0;
      const uint32_t used = r.Below(100);
      uint32_t passes[13];
      for (uint32_t& p : passes) {
        p = r.Below(3) == 0 ? 0u : 1u + r.Below(2);
      }
      const uint32_t targets[2] = {r.Below(4) == 0 ? 0u : block + 0x3100 + 16 * r.Below(8),
                                   r.Below(4) == 0 ? 0u : block + 0x3100 + 16 * r.Below(8)};
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32s(h.base, block, words.data(), uint32_t(words.size()));
        SetRecordScene(h.base, block, kind, used);
        for (uint32_t pass = 0; pass < 13; pass++) {
          guest::StoreU32(h.base, 0x826BAC00 + pass * 592 + 588, passes[pass]);
        }
        guest::StoreU32(h.base, 0x826BABC8, targets[0]);
        guest::StoreU32(h.base, 0x826BABD4, targets[1]);
        PPCContext t = CallContext(h);
        t.r3.u64 = block;
        Run(v, k.recompiled, k.native, t, h.base);
        out[v].Bytes(Read(h.base, block, kBlockBytes));
        out[v].Word(guest::LoadU32(h.base, 0x8296BD28));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {} kind {} used {} flags {:X}", c, kind,
                                                used, words[32 / 4]));
    }
    results.Report();
  }
}

// --- The destructors ---

// The destructors and the deleting ones (r4 0 or 1, or any) on an effect of
// each kind made for each version (operator new, its size; random bytes),
// its surfaces stand-ins in the block (type 0, a count of 2 to 5), its
// render textures each a pool entry in the block (users 0, 2 or 3, any id
// but 20), an own entry (operator new, id 20, users 2), or one whose last
// user goes (users 1, with a header from operator new and 4 KiB from
// XPhysicalAlloc). Compared: the block, the effect and the own entries
// while they're there, what was freed, r3, the made addresses mapped;
// what's left is freed after.
void DestroyCases(Context& h, uint32_t block) {
  Random& r = h.random;
  struct Kind {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t size;
    std::vector<uint32_t> surfaces, textures;
    bool deleting;
  };
  std::vector<uint32_t> glow_textures;
  for (uint32_t at = 8408; at >= 8352; at -= 4) {
    glow_textures.push_back(at);
  }
  const Kind kinds[] = {
      {"sub_82136E30 Glow::~Glow", __imp__sub_82136E30, NativeDestroy<pe::GlowDestroy>, 8480,
       {8432, 8428, 8424, 8420, 8416, 8412}, glow_textures, false},
      {"sub_821351A8 ColorFilter::~ColorFilter", __imp__sub_821351A8, NativeDestroy<pe::ColorFilterDestroy>, 8128, {}, {8064},
       false},
      {"sub_82136390 MotionBlur::~MotionBlur", __imp__sub_82136390, NativeDestroy<pe::MotionBlurDestroy>, 8224,
       {8076, 8072}, {8068, 8064}, false},
      {"sub_821389C8 Glow::~Glow (deleting)", __imp__sub_821389C8, sub_821389C8, 8480,
       {8432, 8428, 8424, 8420, 8416, 8412}, glow_textures, true},
      {"sub_82138570 ColorFilter::~ColorFilter (deleting)", __imp__sub_82138570, sub_82138570, 8128,
       {}, {8064}, true},
      {"sub_821385C0 MotionBlur::~MotionBlur (deleting)", __imp__sub_821385C0, sub_821385C0, 8224,
       {8076, 8072}, {8068, 8064}, true},
  };
  constexpr uint32_t kSurfaces = 0x100, kPool = 0x400;  // 16 and 36 bytes each
  for (const Kind& k : kinds) {
    Results results(k.name);
    for (int c = 0; c < 20; c++) {
      std::vector<uint32_t> words(0x1000 / 4);
      for (uint32_t& w : words) {
        w = uint32_t(r.Next());
      }
      std::vector<uint32_t> object(k.size / 4);
      for (uint32_t& w : object) {
        w = uint32_t(r.Next());
      }
      // Each texture's mode: 0 a pool entry, 1 an own entry (users 2), 2
      // one whose last user goes (own or pool).
      std::vector<uint32_t> modes, users;
      for (size_t i = 0; i < k.textures.size(); i++) {
        modes.push_back(r.Below(3));
        const uint32_t u = r.Below(3);
        users.push_back(u == 1 ? 3 : u);
      }
      for (size_t i = 0; i < k.surfaces.size(); i++) {
        words[(kSurfaces + 16 * i) / 4] = 0;
        words[(kSurfaces + 16 * i) / 4 + 1] = 2 + r.Below(4);
      }
      const uint32_t r4 = k.deleting ? (r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2)) : 0;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (uint32_t i = 0; i < words.size(); i++) {
          guest::StoreU32(h.base, block + 4 * i, words[i]);
        }
        std::vector<uint32_t> made;
        const uint32_t effect = *memory::crt::OperatorNew(h.base, k.size);
        made.push_back(effect);
        guest::StoreU32s(h.base, effect, object.data(), uint32_t(object.size()));
        for (size_t i = 0; i < k.surfaces.size(); i++) {
          guest::StoreU32(h.base, effect + k.surfaces[i], block + kSurfaces + 16 * uint32_t(i));
        }
        std::vector<uint32_t> owns;
        std::vector<std::pair<uint32_t, uint32_t>> textures;
        for (size_t i = 0; i < k.textures.size(); i++) {
          const bool own = modes[i] == 1 || (modes[i] == 2 && (i & 1) == 0);
          const uint32_t entry = own ? *memory::crt::OperatorNew(h.base, 36)
                                     : block + kPool + 36 * uint32_t(i);
          if (own) {
            owns.push_back(entry);
            made.push_back(entry);
          }
          uint32_t fields[9];
          for (uint32_t j = 0; j < 9; j++) {
            fields[j] = guest::LoadU32(h.base, block + kPool + 36 * uint32_t(i) + 4 * j);
          }
          fields[8] = own ? 20 : (fields[8] % 20);
          fields[5] = modes[i] == 2 ? 1 : (own ? 2 : users[i]);
          if (modes[i] == 2) {
            fields[6] = *memory::crt::OperatorNew(h.base, 52);
            fields[7] = memory::AllocPhysical(4096, 0, 0xFFFFFFFFu, 4096, 0x404);
            made.push_back(fields[6]);
            made.push_back(fields[7]);
            textures.push_back({fields[6], fields[7]});
          }
          guest::StoreU32s(h.base, entry, fields, 9);
          guest::StoreU32(h.base, effect + k.textures[i], entry);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = effect;
        t.r4.u64 = r4;
        Run(v, k.recompiled, k.native, t, h.base);
        for (uint32_t i = 0; i < 0x1000; i += 4) {
          out[v].Word(MappedIn(guest::LoadU32(h.base, block + i), made));
        }
        const bool effect_left = memory::IsHeapBlock(effect);
        out[v].Word(effect_left);
        if (effect_left) {
          for (uint32_t i = 0; i < k.size; i += 4) {
            out[v].Word(MappedIn(guest::LoadU32(h.base, effect + i), made));
          }
        }
        for (size_t i = 1; i < made.size(); i++) {
          out[v].Word(memory::IsHeapBlock(made[i]));  // what's freed (physical: always 0)
        }
        out[v].Word(MappedIn(t.r3.u32, made));
        for (const auto& [header, physical] : textures) {
          if (memory::IsHeapBlock(header)) {
            memory::crt::OperatorDelete(header);
            memory::FreePhysical(physical);
          }
        }
        for (uint32_t own : owns) {
          if (memory::IsHeapBlock(own)) {
            memory::crt::OperatorDelete(own);
          }
        }
        if (effect_left) {
          memory::crt::OperatorDelete(effect);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {} r4 {:X}", c, r4));
    }
    results.Report();
  }
}

// --- The glow's star streaks ---

// Glow::RecordStarPass's steps, as an adapter would take them (it has none:
// only Glow::RecordStar's plain C++ calls it): r3 = the glow, r4 = the
// texture, r5 = samples, f1-f4, r8 = the colors, r9 = the pass, the
// destination at r1 + 92.
void NativeStarPass(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t r3 = pe::GlowRecordStarPass(
      base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64,
      ctx.f4.f64, ctx.r8.u32, ctx.r9.u32, guest::LoadU32(base, ctx.r1.u32 + 92),
      hooks::PostEffectsRecordCalls(ctx, base));
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

// A float's bits: ordinary values in +-range, now and then a NaN, an
// infinity, a zero or a denormal.
uint32_t RandomFloatBits(Random& r, double range) {
  return std::bit_cast<uint32_t>(float(RandomDouble(r, range)));
}

// The star cases' own 64 KiB block: the glow at +0 (8,480 bytes), the
// made-up scene of the records (SetRecordScene) from +0x2300 (its list's
// slots from +0x2600, room for 1,000), the render texture entries at
// +0x7000 (32 bytes each, +24 the texture), the colors of the star pass's
// cases at +0x7800.
constexpr uint32_t kStarBlockBytes = 0x10000;
constexpr uint32_t kStarScene = 0x200;  // SetRecordScene's block: the scene at +0x2300
constexpr uint32_t kStarEntries = 0x7000;
constexpr uint32_t kStarColors = 0x7800;

// The glow's words for a star case: random bytes; a quarter of the cases
// the game's glare (GlareDef::Init type 4: four lines of two passes) and
// the constructor's values; its size up to 1,280 x 720, the vertex
// buffer's place small, its render texture entries (+8360 to +8384, +8408)
// in the block, its camera pass 0 to 12, the star's lines 0 to 6 (mostly
// 4) of 0 to 2 passes (now and then 3: the original's path), its floats
// random with NaNs and infinities.
std::vector<uint32_t> StarGlowWords(Context& h, uint32_t star_block, bool game) {
  Random& r = h.random;
  std::vector<uint32_t> words(kStarBlockBytes / 4);
  for (uint32_t& w : words) {
    w = uint32_t(r.Next());
  }
  if (game) {
    guest::StoreU32s(h.base, star_block, words.data(), uint32_t(words.size()));
    pe::GlareDefConstruct(h.base, star_block + 8064);
    pe::GlareDefInit(h.base, star_block + 8064, 4);
    for (uint32_t at = 8064; at < 8352; at += 4) {
      words[at / 4] = guest::LoadU32(h.base, star_block + at);
    }
    words[8468 / 4] = std::bit_cast<uint32_t>(1.0f);
    words[8472 / 4] = std::bit_cast<uint32_t>(0.2f);
  } else {
    for (const uint32_t at : {8084u, 8088u, 8468u, 8472u, 8112u + 100}) {
      words[at / 4] = RandomFloatBits(r, 4.0);
    }
    // Up to 6 lines: from 7 on, the lines' entries run into the colors,
    // whose words as pass counts would have the original record for ever.
    const uint32_t lines = r.Below(3) == 0 ? r.Below(7) : 4;
    words[8112 / 4] = lines;
    for (uint32_t d = 0; d < 6; d++) {
      const uint32_t entry = 8112 + 4 + 16 * d;
      words[entry / 4] = r.Below(12) == 0 ? 3 : r.Below(3) == 0 ? r.Below(3) : 2;
      words[entry / 4 + 3] = RandomFloatBits(r, 7.0);  // the angle
    }
    for (uint32_t at = 8112 + 112; at < 8112 + 240; at += 4) {
      words[at / 4] = r.Below(4) == 0 ? RandomFloatBits(r, 2.0)
                                      : std::bit_cast<uint32_t>(float(r.Below(1000)) / 1000.0f);
    }
  }
  words[8056 / 4] = r.Below(40);
  words[8060 / 4] = r.Below(2);
  words[16 / 4] = r.Below(8) == 0 ? r.Below(4) : 1 + r.Below(1280);
  words[20 / 4] = r.Below(8) == 0 ? r.Below(4) : 1 + r.Below(720);
  words[36 / 4] = r.Below(13);
  for (uint32_t i = 0; i < 7; i++) {
    words[(8360 + 4 * i) / 4] = star_block + kStarEntries + 32 * i;
  }
  words[8408 / 4] = star_block + kStarEntries + 32 * 7;
  return words;
}

// The star block's words: the list's slots (from the made-up scene's
// +0x2500 to its write pointer) as the stream's words, command 7's payloads
// (shader constants: floats) through Floats and every other word exactly
// (a command the walk doesn't know ends it: the rest exactly), the rest of
// the block exactly.
void StarBlockOutputs(Outputs& out, uint8_t* base, uint32_t star_block) {
  const uint32_t from = star_block + kStarScene + kRecSlots;
  const uint32_t to = guest::LoadU32(base, star_block + kStarScene + kRecList + 44);
  out.Bytes(Read(base, star_block, from - star_block));
  uint32_t at = from;
  while (at + 16 <= to) {
    const uint32_t command = guest::LoadU32(base, at);
    const uint32_t count = guest::LoadU32(base, at + 4);
    uint64_t payload;
    switch (command) {
      case 1: case 2: case 3: case 4: case 7:
        payload = count;
        break;
      case 8: case 9: case 10: case 19:
        payload = 1;
        break;
      case 25:
        payload = 5;  // the resolve's 68-byte item
        break;
      case 26:
        payload = 18;  // the 280 bytes after it (allocated without a header)
        break;
      default:
        payload = ~0ull;
    }
    if (payload == ~0ull || at + 16 + 16 * payload > to) {
      break;
    }
    out.Bytes(Read(base, at, 16));
    at += 16;
    if (command == 7) {
      out.Floats(base, at, uint32_t(payload) * 4);
    } else {
      out.Bytes(Read(base, at, uint32_t(payload) * 16));
    }
    at += uint32_t(payload) * 16;
  }
  const uint32_t end = star_block + kStarBlockBytes;
  if (at < end) {
    out.Bytes(Read(base, at, end - at));
  }
}

// Glow::RecordStarPass on the glow (StarGlowWords), samples 0 to 16 (mostly
// the game's 8), the colors random floats in the block, f1 to f4 random
// (the game's attenuation 0.2 now and then), the pass 0 to 2 (or any), the
// texture and the destination random words, the stack below r1 the same
// random words for both versions. Compared: the block (the glow's
// vertices, the list and its slots: the two constant blocks hold the
// frame's words the original leaves unwritten; StarBlockOutputs), the
// frame's +80 to +624 (floats from +88; not its register save area), r3.
void StarPassCases(Context& h, uint32_t star_block) {
  Random& r = h.random;
  Results results("sub_82134060 Glow::RecordStarPass");
  Saved scene(h.base, 0x826BABA0, 1);
  Saved open_chunks(h.base, 0x8296BD28, 1);
  for (int c = 0; c < 60; c++) {
    std::vector<uint32_t> words = StarGlowWords(h, star_block, r.Below(4) == 0);
    for (uint32_t i = 0; i < 64; i++) {
      words[(kStarColors + 4 * i) / 4] =
          r.Below(4) == 0 ? RandomFloatBits(r, 3.0)
                          : std::bit_cast<uint32_t>(float(r.Below(1000)) / 1000.0f);
    }
    const uint32_t used = r.Below(100);
    const uint32_t samples = r.Below(4) == 0 ? r.Below(17) : 8;
    const double f[4] = {r.Below(3) == 0 ? 0.2 : RandomDouble(r, 2.0),
                         double(float(RandomDouble(r, 1.0))), double(float(RandomDouble(r, 0.1))),
                         double(float(RandomDouble(r, 0.1)))};
    const uint32_t pass = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(3);
    const uint32_t texture = uint32_t(r.Next());
    const uint32_t dest = uint32_t(r.Next());
    std::vector<uint32_t> stack(0x1000 / 4 + 8);
    for (uint32_t& w : stack) {
      w = uint32_t(r.Next());
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32s(h.base, star_block, words.data(), uint32_t(words.size()));
      SetRecordScene(h.base, star_block + kStarScene, 0, used);
      PPCContext t = CallContext(h);
      FillStack(h.base, t.r1.u32, stack);
      guest::StoreU32(h.base, t.r1.u32 + 92, dest);
      t.r3.u64 = star_block;
      t.r4.u64 = texture;
      t.r5.u64 = samples;
      t.r8.u64 = star_block + kStarColors;
      t.r9.u64 = pass;
      t.f1.f64 = f[0];
      t.f2.f64 = f[1];
      t.f3.f64 = f[2];
      t.f4.f64 = f[3];
      const uint32_t frame = t.r1.u32 - 768;
      Run(v, __imp__sub_82134060, NativeStarPass, t, h.base);
      StarBlockOutputs(out[v], h.base, star_block);
      out[v].Bytes(Read(h.base, frame + 80, 8));  // the last index, as a doubleword
      out[v].Floats(h.base, frame + 88, (624 - 88) / 4);
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {} samples {} pass {:X} used {}", c, samples,
                                              pass, used));
  }
  results.Report();
}

// Glow::RecordStar on the glow (StarGlowWords), its camera pass active or
// not (its view matrix random floats; now and then all zeros, or the
// view's z axis along y, for the normalize's zero), the made-up scene's
// list with room. Compared: the block (StarBlockOutputs: the star passes'
// frames reach it through their constant blocks), the frame's +80 to +464
// (not its register save area; the star pass's frame below it is dead
// stack by then, written over by the merge's helpers' frames in the
// original), the camera pass's matrix untouched, the open chunks, r3.
void StarCases(Context& h, uint32_t star_block) {
  Random& r = h.random;
  Results results("sub_82136FA8 Glow::RecordStar");
  Saved scene(h.base, 0x826BABA0, 1);
  Saved open_chunks(h.base, 0x8296BD28, 1);
  std::vector<Saved> passes;
  passes.reserve(13);
  for (uint32_t pass = 0; pass < 13; pass++) {
    passes.emplace_back(h.base, 0x826BAC00 + pass * 592, 148);
  }
  for (int c = 0; c < 80; c++) {
    const std::vector<uint32_t> words = StarGlowWords(h, star_block, r.Below(3) == 0);
    const uint32_t used = r.Below(100);
    const uint32_t camera = words[36 / 4];
    const uint32_t active = r.Below(4) == 0 ? 0u : r.Below(8) == 0 ? uint32_t(r.Next()) | 1 : 1u;
    uint32_t matrix[16];
    const uint32_t shape = r.Below(10);
    for (uint32_t i = 0; i < 16; i++) {
      matrix[i] = shape == 0 ? 0u
                  : shape == 1 ? std::bit_cast<uint32_t>(i % 5 == 0 ? 1.0f : 0.0f)
                  : r.Below(6) == 0 ? RandomFloatBits(r, 100.0)
                                    : std::bit_cast<uint32_t>(float(RandomDouble(r, 1.0)));
    }
    if (shape == 2) {
      // A turn about x by 90 degrees: the inverse's z axis along y.
      const float m[16] = {1, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 5, 6, 7, 1};
      for (uint32_t i = 0; i < 16; i++) {
        matrix[i] = std::bit_cast<uint32_t>(m[i]);
      }
    }
    std::vector<uint32_t> stack(0x1000 / 4 + 8);
    for (uint32_t& w : stack) {
      w = uint32_t(r.Next());
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32s(h.base, star_block, words.data(), uint32_t(words.size()));
      SetRecordScene(h.base, star_block + kStarScene, 0, used);
      guest::StoreU32(h.base, 0x8296BD28, 1);  // the glow's chunk open
      guest::StoreU8(h.base, star_block + kStarScene + kRecList + 52, 1);
      const uint32_t pass = 0x826BAC00 + camera * 592;
      guest::StoreU32s(h.base, pass + 16, matrix, 16);
      guest::StoreU32(h.base, pass + 588, active);
      PPCContext t = CallContext(h);
      FillStack(h.base, t.r1.u32, stack);
      t.r3.u64 = star_block;
      const uint32_t frame = t.r1.u32 - 672;
      Run(v, __imp__sub_82136FA8, sub_82136FA8, t, h.base);
      StarBlockOutputs(out[v], h.base, star_block);
      // The frame: +92 the last destination, +96 and +112 the quarter
      // size's doublewords, the rest floats (the lines' words, the turn's
      // vectors and matrix, the colors).
      out[v].Bytes(Read(h.base, frame + 80, 24));
      out[v].Floats(h.base, frame + 104, 2);
      out[v].Bytes(Read(h.base, frame + 112, 8));
      out[v].Floats(h.base, frame + 120, (464 - 120) / 4);
      out[v].Bytes(Read(h.base, pass + 16, 64));
      out[v].Word(guest::LoadU32(h.base, 0x8296BD28));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1],
                  fmt::format("case {} lines {} camera {} active {:X} shape {} used {}", c,
                              words[8112 / 4], camera, active, shape, used));
  }
  results.Report();
}

}  // namespace

void PostEffects(Context& h) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const uint32_t block = memory::Alloc(kBlockBytes, memory::kHeapZeroMemory);
  if (block == 0) {
    return;
  }
  EffectsCases(h, block);
  ToggleCases(h, block);
  SetterCases(h, block);
  SetFromSpeedCases(h, block);
  RecordCases(h, block);
  AddQuadCases(h, block);
  GlareCases(h, block);
  RenderTextureCases(h, block);
  ReleaseSurfaceCases(h, block);
  TargetFilterCases(h, block);
  InitEdramBaseCases(h);
  RenderTextureMakeCases(h, block);
  ConstructorCases(h, block);
  CreateCases(h, block);
  InitBootCases(h, block);
  EffectRecordCases(h, block);
  DestroyCases(h, block);
  memory::Free(block);
  const uint32_t star_block = memory::Alloc(kStarBlockBytes, memory::kHeapZeroMemory);
  if (star_block != 0) {
    StarPassCases(h, star_block);
    StarCases(h, star_block);
    memory::Free(star_block);
  }
}

}  // namespace harness
