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

// StarDef::Init: r3 = the star definition, r4 = lines, r5 = passes, f1 =
// sample length, f2 = attenuation, f4 = inclination, r10 = rotation.
void NativeStarDefInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = pe::StarDefInit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64, ctx.f2.f64,
                               ctx.f4.f64, ctx.r10.u32);
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
          Run(v, __imp__sub_8212ED78, sub_8212ED78, t, h.base);
        } else if (f == 1) {
          Run(v, __imp__sub_8212EC90, sub_8212EC90, t, h.base);
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
  memory::Free(block);
}

}  // namespace harness
