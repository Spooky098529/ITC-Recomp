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
#include "hooks/heap.h"
#include "memory/heap.h"
#include "post_effects/post_effects.h"

#include <fmt/format.h>

#include <bit>
#include <cstring>
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
  memory::Free(block);
}

}  // namespace harness
