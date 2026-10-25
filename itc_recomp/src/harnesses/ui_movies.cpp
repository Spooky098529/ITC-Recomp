// The UI movie library's functions (src/hooks/ui_movies.cpp) against the
// recompiled ones. Movies are live guest objects: every case builds its own
// in the scratch area, so nothing the game holds is read or written.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "command_list/command_list.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "ui_movies/ui_movies.h"

#include <algorithm>
#include <bit>
#include <cmath>
#include <iterator>
#include <limits>

ITC_FLAG_DECLARE(bool, native_ui_movies);

namespace harness {

namespace {

// The register steps of the file fix-ups whose adapters went once their
// callers (CheckXmd, UiMovie::FixUpFile) were native (docs/loops/b7-loop.md,
// "Removing an adapter"): the cases run the plain C++ through these, against
// the recompiled versions.
void NativeIsXmd(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ui_movies::IsXmd(base, ctx.r3.u32) ? 1 : 0;
}

void NativeIsMovieFile(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ui_movies::IsMovieFile(base, ctx.r3.u32) ? 1 : 0;
}

void NativeFileAnimation(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ui_movies::FileAnimation(base, ctx.r3.u32, ctx.r4.u32);
}

void NativeFixUpScene(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ui_movies::FixUpScene(base, ctx.r3.u32, ctx.r4.u32);
}

void NativeFixUpAnimation(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ui_movies::FixUpAnimation(base, ctx.r3.u32, ctx.r4.u32);
}

// The draw walk's register steps (as their adapters', without the flag).
void NativeCopyDrawContext(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ui_movies::CopyDrawContext(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeCopyQueuedText(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ui_movies::CopyQueuedText(base, ctx.r3.u32, ctx.r4.u32);
}
int g_draw_fallbacks = 0;
void NativeAppendText(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!ui_movies::AppendText(base, ctx.r3.u32, ctx.r4.u32, &r3)) {
    g_draw_fallbacks++;
    __imp__sub_8259F700(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
void NativeQueueText(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!ui_movies::QueueText(base, ctx.r3.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32 - 272, &r3)) {
    g_draw_fallbacks++;
    __imp__sub_8259F770(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}

// UiMovie::ApplyTransform's register steps (as its adapter's, without the
// flag).
void NativeApplyTransform(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ui_movies::ApplyTransform(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 352);
}

constexpr uint32_t ui_movies_kTrm = 0x54524D00;       // "TRM\0"
constexpr uint32_t ui_movies_kSce = 0x53434500;       // "SCE\0"
constexpr uint32_t ui_movies_kRenderer = 0x82970C18;  // then the command list at +4

struct Function {
  const char* name;
  GuestFunction recompiled, native;
};

// The accessors that only add: r3 and r4 random, and their edges.
void Arithmetic(Context& h, const Function& f) {
  Results results(f.name);
  std::vector<std::pair<uint32_t, uint32_t>> inputs = {
      {0, 0}, {0xFFFFFFFF, 0xFFFFFFFF}, {0x80000000, 0x7FFFFFFF}, {0x40000000, 0x10000000},
      {0x82970000, 3}};
  for (int i = 0; i < 500; i++) {
    const uint32_t r3 = uint32_t(h.random.Next());
    const uint32_t r4 = h.random.Below(4) == 0 ? uint32_t(h.random.Next()) : h.random.Below(64);
    inputs.push_back({r3, r4});
  }
  for (auto [r3, r4] : inputs) {
    Both(h, results, [] {},
         [&](PPCContext& t) {
           t.r3.u64 = r3;
           t.r4.u64 = r4;
         },
         f.recompiled, f.native, h.data, 16, fmt::format("r3 {:08X} r4 {:X}", r3, r4));
  }
  results.Report();
}

// The accessors that read a pointer from an object's entries: the object in
// the scratch area, its index field (`index_field`) and i chosen so the entry
// read is in the scratch area too (wrapping included), the entry random.
void Indexed(Context& h, const Function& f, uint32_t first_entry, uint32_t index_field) {
  Results results(f.name);
  const uint32_t object = h.data + 64;
  const uint32_t entries = (kScratchBytes - 64 - first_entry - 8) / 8;
  std::vector<std::pair<uint32_t, uint32_t>> inputs = {
      {0, 0}, {0xFFFFFFFF, 1}, {0x20, 0xFFFFFFF0}, {1, uint32_t(entries - 2)}};
  for (int i = 0; i < 300; i++) {
    const uint32_t index = h.random.Below(entries);
    const uint32_t step = h.random.Below(entries - index);
    // Half the cases split the index the other way round, through a wrap.
    if (h.random.Below(2) == 0) {
      inputs.push_back({index, step});
    } else {
      const uint32_t bias = uint32_t(h.random.Next()) | 0x80000000u;
      inputs.push_back({index + step + bias, uint32_t(0) - bias});
    }
  }
  for (auto [index, i] : inputs) {
    const uint32_t entry = uint32_t(h.random.Next());
    Both(h, results,
         [&] {
           guest::StoreU32(h.base, object + index_field, index);
           guest::StoreU32(h.base, object + first_entry + 8 * (index + i), entry);
         },
         [&](PPCContext& t) {
           t.r3.u64 = object;
           t.r4.u64 = i;
         },
         f.recompiled, f.native, h.data, kScratchBytes,
         fmt::format("index {:X} i {:X} entry {:08X}", index, i, entry));
  }
  results.Report();
}

// sub_8259D628: a primitive's vertex, from its texture count at +112.
void PrimitiveVertex(Context& h) {
  Results results("PrimitiveVertex (sub_8259D628)");
  const uint32_t primitive = h.data + 64;
  for (int k = 0; k < 500; k++) {
    const uint32_t textures = k < 8 ? uint32_t(k) : uint32_t(h.random.Next());
    const uint32_t i = h.random.Below(4) == 0 ? uint32_t(h.random.Next()) : h.random.Below(32);
    Both(h, results, [&] { guest::StoreU32(h.base, primitive + 112, textures); },
         [&](PPCContext& t) {
           t.r3.u64 = primitive;
           t.r4.u64 = i;
         },
         __imp__sub_8259D628, sub_8259D628, h.data,
         kScratchBytes, fmt::format("textures {:X} i {:X}", textures, i));
  }
  results.Report();
}

// Appends a word's bytes to a result.
void AppendWord(std::vector<uint8_t>& out, uint32_t word) {
  for (int b = 0; b < 4; b++) {
    out.push_back(uint8_t(word >> (24 - 8 * b)));
  }
}

// The odd float words the cases mix in: NaNs of both kinds and signs,
// infinities, denormals, signed zeros, the largest float.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001, 0x7F800000,
                                   0xFF800000, 0x00000001, 0x807FFFFF, 0x00000000, 0x80000000,
                                   0x7F7FFFFF, 0x3F800000};

uint32_t RandomFloat(Random& r, float low, float high) {
  if (r.Below(10) == 0) {
    return kOddFloats[r.Below(std::size(kOddFloats))];
  }
  const float f = low + (high - low) * float(r.Below(1u << 24)) / float(1u << 24);
  return std::bit_cast<uint32_t>(f);
}

// sub_8259D5C8: chains of chunks in the scratch area (offsets 8 to 64, tags
// SCE, GRP, PIC, random words or the one asked for), ending with TRM; asked
// for: a tag in the chain, one not in it, and TRM.
void FindChunkCases(Context& h) {
  Results results("FindChunk (sub_8259D5C8)");
  constexpr uint32_t kTags[] = {0x53434500, 0x47525000, 0x50494300, 0x54584D00};
  Random& r = h.random;
  for (int i = 0; i < 600; i++) {
    std::vector<std::pair<uint32_t, uint32_t>> chain;  // offset, tag
    uint32_t at = 0;
    const int count = 1 + int(r.Below(40));
    for (int k = 0; k < count; k++) {
      const uint32_t step = 8 + 4 * r.Below(15);
      const uint32_t tag = r.Below(5) == 0 ? uint32_t(r.Next()) : kTags[r.Below(4)];
      chain.push_back({at, tag});
      at += step;
    }
    chain.push_back({at, ui_movies_kTrm});
    uint32_t wanted;
    switch (r.Below(3)) {
      case 0: wanted = chain[r.Below(uint32_t(chain.size()))].second; break;
      case 1: wanted = uint32_t(r.Next()); break;
      default: wanted = ui_movies_kTrm; break;
    }
    Both(h, results,
         [&] {
           for (size_t k = 0; k < chain.size(); k++) {
             guest::StoreU32(h.base, h.data + chain[k].first, chain[k].second);
             if (k + 1 < chain.size()) {
               guest::StoreU32(h.base, h.data + chain[k].first + 4,
                               chain[k + 1].first - chain[k].first);
             }
           }
         },
         [&](PPCContext& t) {
           t.r3.u64 = h.data;
           t.r4.u64 = wanted;
         },
         __imp__sub_8259D5C8, sub_8259D5C8, h.data, 16,
         fmt::format("case {}: {} chunks, tag {:08X}", i, chain.size(), wanted));
  }
  results.Report();
}

// sub_825A0EB0: a movie file's scene, tagged SCE or not, at offsets in the
// scratch area.
void FileSceneCases(Context& h) {
  Results results("FileScene (sub_825A0EB0)");
  Random& r = h.random;
  for (int i = 0; i < 300; i++) {
    const uint32_t offset = 8 + 4 * r.Below(1000);
    const uint32_t tag = r.Below(2) ? ui_movies_kSce : (r.Below(2) ? uint32_t(r.Next()) : 0x53434501);
    Both(h, results,
         [&] {
           guest::StoreU32(h.base, h.data + 4, offset);
           guest::StoreU32(h.base, h.data + offset, tag);
         },
         [&](PPCContext& t) { t.r3.u64 = h.data; }, __imp__sub_825A0EB0, sub_825A0EB0, h.data, 16,
         fmt::format("offset {:X} tag {:08X}", offset, tag));
  }
  results.Report();
}

// sub_825A0CD0: an XMD file's entries (count at +12, offsets from +16): i
// below the count, at it, above it, and huge.
void XmdEntryCases(Context& h) {
  Results results("XmdEntry (sub_825A0CD0)");
  Random& r = h.random;
  for (int i = 0; i < 400; i++) {
    const uint32_t count = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(900);
    uint32_t index;
    switch (r.Below(4)) {
      case 0: index = count; break;
      case 1: index = count + 1 + r.Below(100); break;
      case 2: index = std::max(count, uint32_t(r.Next()) | 0x80000000u); break;
      default: index = r.Below(std::min<uint32_t>(std::max<uint32_t>(count, 1), 900)); break;
    }
    const uint32_t entry = uint32_t(r.Next());
    Both(h, results,
         [&] {
           guest::StoreU32(h.base, h.data + 12, count);
           if (index < 900) {
             guest::StoreU32(h.base, h.data + 16 + 4 * index, entry);
           }
         },
         [&](PPCContext& t) {
           t.r3.u64 = h.data;
           t.r4.u64 = index;
         },
         __imp__sub_825A0CD0, sub_825A0CD0, h.data, 16,
         fmt::format("count {:X} i {:X}", count, index));
  }
  results.Report();
}

// UiMovie::SetRenderer: the game's two globals, put back after.
void SetRendererCases(Context& h) {
  Results results("UiMovie::SetRenderer (sub_8259D798)");
  const uint32_t saved_renderer = guest::LoadU32(h.base, ui_movies_kRenderer);
  const uint32_t saved_list = guest::LoadU32(h.base, ui_movies_kRenderer + 4);
  for (int i = 0; i < 50; i++) {
    const uint32_t renderer = uint32_t(h.random.Next()), list = uint32_t(h.random.Next());
    Both(h, results,
         [&] {
           guest::StoreU32(h.base, ui_movies_kRenderer, 0xCDCDCDCD);
           guest::StoreU32(h.base, ui_movies_kRenderer + 4, 0xCDCDCDCD);
         },
         [&](PPCContext& t) {
           t.r3.u64 = renderer;
           t.r4.u64 = list;
         },
         __imp__sub_8259D798, sub_8259D798, ui_movies_kRenderer, 8,
         fmt::format("{:08X} {:08X}", renderer, list));
  }
  guest::StoreU32(h.base, ui_movies_kRenderer, saved_renderer);
  guest::StoreU32(h.base, ui_movies_kRenderer + 4, saved_list);
  results.Report();
}

// sub_8259D930: draw contexts at aligned and unaligned addresses, over
// different fills.
void ResetDrawContextCases(Context& h) {
  Results results("ResetDrawContext (sub_8259D930)");
  for (uint32_t offset = 0; offset < 32; offset++) {
    for (uint8_t fill : {uint8_t(0xCD), uint8_t(0x00), uint8_t(0xFF)}) {
      Both(h, results, [&] { Fill(h.base, h.data, 256, fill); },
           [&](PPCContext& t) { t.r3.u64 = h.data + 16 + offset; }, __imp__sub_8259D930,
           sub_8259D930, h.data, 256, fmt::format("offset {} fill {:02X}", offset, fill));
    }
  }
  results.Report();
}

// The keys: two 32-byte keys in the scratch area (times, values, tangent
// angles random, the odd floats mixed in; integer values random words or
// small), t mostly in [0, 1] (the caller clamps it) with odd doubles. The
// integer ones compare r3; CubicFloat compares f1 (its r3 and r4 are tan's
// leftovers, which the caller doesn't read).
void KeyCases(Context& h) {
  struct KeyFunction {
    const char* name;
    GuestFunction recompiled, native;
    bool integer_values, result_in_f1;
  };
  const KeyFunction functions[] = {
      {"EaseInteger (sub_8259FA20)", __imp__sub_8259FA20, sub_8259FA20, true, false},
      {"CubicFloat (sub_8259FB30)", __imp__sub_8259FB30, sub_8259FB30, false, true},
      {"CubicInteger (sub_8259FC18)", __imp__sub_8259FC18, sub_8259FC18, true, false},
  };
  const double odd_t[] = {0.0, 1.0, -0.0, 0.5, 1e-40, -1.0, 2.0, 1.0000001, std::nan(""),
                          std::numeric_limits<double>::infinity(), 0.1, 1.0 / 3.0};
  for (const KeyFunction& f : functions) {
    Results results(f.name);
    Random& r = h.random;
    for (int i = 0; i < 3000; i++) {
      uint32_t key[2][5];  // +0..+16
      for (int k = 0; k < 2; k++) {
        key[k][0] = uint32_t(r.Next());
        key[k][1] = RandomFloat(r, 0, 10);
        if (f.integer_values) {
          key[k][2] = r.Below(3) == 0 ? uint32_t(r.Next()) : uint32_t(int32_t(r.Below(2000)) - 1000);
        } else {
          key[k][2] = RandomFloat(r, -1000, 1000);
        }
        key[k][3] = RandomFloat(r, -1.5f, 1.5f);
        key[k][4] = RandomFloat(r, -1.5f, 1.5f);
      }
      if (r.Below(20) == 0) {  // tan's error paths: huge angles
        key[1][3 + r.Below(2)] = std::bit_cast<uint32_t>(4e8f);
      }
      double t;
      if (r.Below(8) == 0) {
        t = odd_t[r.Below(std::size(odd_t))];
      } else if (r.Below(2) == 0) {
        t = double(float(r.Below(1u << 24)) / float(1u << 24));
      } else {
        t = double(r.Below(1u << 30)) / double(1u << 30);  // not a float's value
      }
      std::vector<uint8_t> result[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, 64);
        for (int k = 0; k < 2; k++) {
          for (int w = 0; w < 5; w++) {
            guest::StoreU32(h.base, h.data + 32 * k + 4 * w, key[k][w]);
          }
        }
        PPCContext c = CallContext(h);
        c.r3.u64 = h.data;
        c.r4.u64 = h.data + 32;
        c.f1.f64 = t;
        (v == 0 ? f.recompiled : f.native)(c, h.base);
        result[v] = Read(h.base, h.data, 64);
        if (f.result_in_f1) {
          AppendWord(result[v], uint32_t(c.f1.u64 >> 32));
          AppendWord(result[v], uint32_t(c.f1.u64));
        } else {
          AppendWord(result[v], c.r3.u32);
        }
      }
      results.Check(result[0], result[1],
                    fmt::format("t {} k0 {:08X} {:08X} {:08X} {:08X} k1 {:08X} {:08X} {:08X} {:08X}",
                                t, key[0][1], key[0][2], key[0][3], key[0][4], key[1][1], key[1][2],
                                key[1][3], key[1][4]));
    }
    results.Report();
  }
}

// The recorders: a made-up command list, renderer and quad in the scratch
// area, the game's globals for the renderer and the list (and the open
// chunk count) pointed at them during the cases and put back after. The
// renderer's vtable slot 4 is BillboardMaterial::SetLights (sub_8209F5C8),
// which stores what it's given (r4, the chunk; r5, the key) at its +32 and
// +36 unless r4 is 0. Cases: every blend mode and odd ones, clipped or not,
// the texture stage flag, 0 to 8 vertices with odd floats, the chunk already
// open, another chunk open (the original runs), and a list with just room.
// Compared: the whole scratch area, the open chunk count, r3 and f1.
void RecorderCases(Context& h) {
  struct Recorder {
    const char* name;
    GuestFunction recompiled, native;
    int kind;  // 0 textured quad, 1 untextured, 2 the resets, 3 either by +84, 4 the resets by r4
  };
  const Recorder recorders[] = {
      {"RecordQuad, textured (sub_8259DAD0)", __imp__sub_8259DAD0, sub_8259DAD0, 0},
      {"RecordQuad, untextured (sub_8259E098)", __imp__sub_8259E098, sub_8259E098, 1},
      {"the clip and state resets (sub_8259D7B0)", __imp__sub_8259D7B0, sub_8259D7B0, 2},
      {"RecordQuad and the resets when clipped (sub_8259EBE8)", __imp__sub_8259EBE8, sub_8259EBE8, 3},
      {"the resets after a movie (sub_8259DAC0)", __imp__sub_8259DAC0, sub_8259DAC0, 4},
  };
  constexpr uint32_t kList = 0, kRendererObject = 64, kVtable = 128, kQuad = 256,
                     kPositions = 512, kColors = 640, kUvs = 768, kSlots = 1024;
  const uint32_t saved_renderer = guest::LoadU32(h.base, ui_movies_kRenderer);
  const uint32_t saved_list = guest::LoadU32(h.base, ui_movies_kRenderer + 4);
  const uint32_t saved_open = guest::LoadU32(h.base, command_list::kOpenChunks);
  for (const Recorder& f : recorders) {
    Results results(f.name);
    Random& r = h.random;
    for (int i = 0; i < 600; i++) {
      const uint32_t mode = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(7);
      const uint32_t vertices = r.Below(9);
      const uint32_t clipped = r.Below(2) ? 0 : (r.Below(2) ? 1 : uint32_t(r.Next()));
      const uint32_t stage = r.Below(2) ? 0 : (r.Below(2) ? 1 : uint32_t(r.Next()));
      const uint32_t key = r.Below(2) ? 678 : uint32_t(r.Next());
      const int state = i < 520 ? 0 : 1 + (i % 3);  // 1 already open, 2 nested, 3 just room
      const uint32_t used = r.Below(2) ? 0 : r.Below(100000);
      const uint32_t texture = f.kind == 3 && r.Below(2) ? 0 : uint32_t(r.Next()) | 1;
      const bool textured = f.kind == 0 || (f.kind == 3 && texture != 0);
      // The slots it needs, for "just room".
      uint32_t needed = 8;
      if (f.kind != 2 && f.kind != 4) {
        const uint32_t blend[] = {5, 5, 5, 6, 4};
        needed = (textured ? 5 : 3) + (mode < 5 ? blend[mode] : 0) + 6 +
                 ((vertices * 24 + 15) >> 4) + 3;
        if (f.kind == 3 && clipped != 0) {
          needed += 8;
        }
      }
      const uint32_t unread = uint32_t(r.Next());
      std::vector<uint32_t> words(4 * 3 * 8 + 64);
      for (uint32_t& w : words) {
        w = r.Below(3) ? RandomFloat(r, -2000, 2000) : uint32_t(r.Next());
      }
      std::vector<uint8_t> result[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        const uint32_t list = h.data + kList;
        guest::StoreU32(h.base, list + 8, state == 3 ? used + needed + 1 : used + 1000);
        guest::StoreU32(h.base, list + 12, used);
        guest::StoreU32(h.base, list + 32, h.data + kSlots);
        guest::StoreU8(h.base, list + 52, state == 1 ? 1 : 0);
        if (state == 1) {
          guest::StoreU32(h.base, list + 44, h.data + kSlots + 16);
        }
        guest::StoreU32(h.base, command_list::kOpenChunks, state == 2 ? 1 : 0);
        guest::StoreU32(h.base, h.data + kRendererObject, h.data + kVtable);
        guest::StoreU32(h.base, h.data + kVtable + 16, 0x8209F5C8);
        const uint32_t quad = h.data + kQuad;
        guest::StoreU32(h.base, quad + 68, mode);
        guest::StoreU32(h.base, quad + 80, vertices);
        guest::StoreU32(h.base, quad + 84, texture);
        guest::StoreU32(h.base, quad + 88, h.data + kPositions);
        guest::StoreU32(h.base, quad + 92, h.data + kColors);
        guest::StoreU32(h.base, quad + 96, h.data + kUvs);
        guest::StoreU32(h.base, quad + 100, clipped);
        guest::StoreU32(h.base, quad + 104, words[1]);
        guest::StoreU32(h.base, quad + 108, words[2]);
        guest::StoreU32(h.base, quad + 112, stage);
        for (uint32_t k = 0; k < 32; k++) {
          guest::StoreU32(h.base, h.data + kPositions + 4 * k, words[8 + k]);
          guest::StoreU32(h.base, h.data + kColors + 4 * k, words[40 + k]);
          guest::StoreU32(h.base, h.data + kUvs + 4 * k, words[72 + k]);
        }
        guest::StoreU32(h.base, ui_movies_kRenderer, h.data + kRendererObject);
        guest::StoreU32(h.base, ui_movies_kRenderer + 4, list);
        PPCContext t = CallContext(h);
        if (f.kind == 2) {
          t.r3.u64 = key;
        } else if (f.kind == 4) {
          t.r3.u64 = unread;
          t.r4.u64 = key;
        } else {
          t.r3.u64 = quad;
          t.r4.u64 = key;
        }
        t.f1.f64 = 123.0;  // not read
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        result[v] = Read(h.base, h.data, kScratchBytes);
        AppendWord(result[v], guest::LoadU32(h.base, command_list::kOpenChunks));
        AppendWord(result[v], t.r3.u32);
        AppendWord(result[v], uint32_t(t.f1.u64 >> 32));
        AppendWord(result[v], uint32_t(t.f1.u64));
      }
      size_t at = 0;
      while (at < result[0].size() && result[0][at] == result[1][at]) {
        at++;
      }
      const size_t from = at < result[0].size() ? at : 0;
      const size_t to = std::min(result[0].size(), from + 32);
      results.Check({result[0].begin() + from, result[0].begin() + to},
                    {result[1].begin() + from, result[1].begin() + to},
                    fmt::format("case {}: state {} mode {:X} vertices {} clipped {:X} stage {:X}, "
                                "from byte {}",
                                i, state, mode, vertices, clipped, stage, from));
    }
    results.Report();
  }
  guest::StoreU32(h.base, ui_movies_kRenderer, saved_renderer);
  guest::StoreU32(h.base, ui_movies_kRenderer + 4, saved_list);
  guest::StoreU32(h.base, command_list::kOpenChunks, saved_open);
}

// The headers: sub_825A0C60 (XMD 001 at +0), sub_825A0E40 (X2D 005 at +8)
// and sub_825A0CF8 (aligned, then XMD's check; the words after it random,
// so a swap would show): the header exact, a byte off, byte-swapped, and
// random, at aligned and unaligned addresses.
void HeaderCases(Context& h) {
  struct Header {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t at;
    uint8_t bytes[8];
  };
  const Header headers[] = {
      {"IsXmd (sub_825A0C60)", __imp__sub_825A0C60, NativeIsXmd, 0, {'X', 'M', 'D', 0, '0', '0', '1', 0}},
      {"CheckXmd (sub_825A0CF8)", __imp__sub_825A0CF8, sub_825A0CF8, 0, {'X', 'M', 'D', 0, '0', '0', '1', 0}},
      {"IsMovieFile (sub_825A0E40)", __imp__sub_825A0E40, NativeIsMovieFile, 8, {'X', '2', 'D', 0, '0', '0', '5', 0}},
  };
  for (const Header& f : headers) {
    Results results(f.name);
    Random& r = h.random;
    for (int i = 0; i < 400; i++) {
      uint8_t bytes[8];
      std::copy(std::begin(f.bytes), std::end(f.bytes), bytes);
      const int kind = r.Below(4);
      if (kind == 1) {
        bytes[r.Below(8)] ^= uint8_t(1 + r.Below(255));
      } else if (kind == 2) {
        std::reverse(bytes, bytes + 4);
        std::reverse(bytes + 4, bytes + 8);
      } else if (kind == 3) {
        for (uint8_t& b : bytes) {
          b = uint8_t(r.Next());
        }
      }
      const uint32_t file = ((h.data + 64) & ~0xFu) + (r.Below(3) == 0 ? r.Below(16) : 0);
      std::vector<uint32_t> words(64);
      for (uint32_t& w : words) {
        w = r.Below(2) ? r.Below(40) : uint32_t(r.Next());
      }
      Both(h, results,
           [&] {
             for (uint32_t k = 0; k < 64; k++) {
               guest::StoreU32(h.base, file + 4 * k, words[k]);
             }
             Write(h.base, file + f.at, bytes, 8);
           },
           [&](PPCContext& t) { t.r3.u64 = file; }, f.recompiled, f.native, h.data, 512,
           fmt::format("case {}: kind {} at +{:X}", i, kind, file & 0xF));
    }
    results.Report();
  }
}

// sub_825A6570: an animation (at +256) whose [+56] entries (offsets) point
// at sub-animation records (from +1024, 32 bytes each, each used once), the
// [+52] sub-animations from index [+48] among them, each sub-animation's +16
// an index into the file's table (at +0: [+16] its start, entries from +32).
// The file table's entries random.
void FixUpAnimationCases(Context& h) {
  Results results("FixUpAnimation (sub_825A6570)");
  Random& r = h.random;
  for (int i = 0; i < 300; i++) {
    const uint32_t file = h.data, animation = h.data + 256;
    const uint32_t start = r.Below(4);
    const uint32_t entries = r.Below(17);
    const uint32_t first = entries == 0 ? r.Below(3) : r.Below(entries);
    const uint32_t subs = entries == 0 ? 0 : r.Below(entries - first + 1);
    std::vector<uint32_t> table(16), indices(16);
    for (uint32_t k = 0; k < 16; k++) {
      table[k] = uint32_t(r.Next());
      indices[k] = r.Below(16 - start);
    }
    Both(h, results,
         [&] {
           guest::StoreU32(h.base, file + 16, start);
           for (uint32_t k = 0; k < 16; k++) {
             guest::StoreU32(h.base, file + 32 + 8 * k, table[k]);
             guest::StoreU32(h.base, animation + 64 + 8 * k, 1024 + 32 * k - 256);
             guest::StoreU32(h.base, h.data + 1024 + 32 * k + 16, indices[k]);
           }
           guest::StoreU32(h.base, animation + 48, first);
           guest::StoreU32(h.base, animation + 52, subs);
           guest::StoreU32(h.base, animation + 56, entries);
         },
         [&](PPCContext& t) {
           t.r3.u64 = animation;
           t.r4.u64 = file;
         },
         __imp__sub_825A6570, NativeFixUpAnimation, h.data, kScratchBytes,
         fmt::format("case {}: start {} entries {} first {} subs {}", i, start, entries, first, subs));
  }
  results.Report();
}

// A scene in the scratch area for the fix-ups: at `scene`, `children` child
// entries from +128, [+116] and [+120] the animation range; entries outside
// it are offsets to plain nodes (from +3584, 16 bytes each, tagged at random
// but never SCE) or, once, to a sub-scene; entries in it index the file's
// table (at `file`: [+16] its start, entries from +32, pointing at plain
// nodes).
struct SceneSpec {
  uint32_t first, count, children;
  std::vector<uint32_t> entries;
};
SceneSpec RandomScene(Random& r, uint32_t scene, uint32_t data, uint32_t start,
                      uint32_t sub_scene, uint32_t table_entries) {
  SceneSpec spec;
  spec.children = r.Below(15);
  spec.first = r.Below(spec.children + 2);
  spec.count = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(spec.children + 1);
  bool sub_used = false;
  for (uint32_t k = 0; k < spec.children; k++) {
    const bool in_range = k >= spec.first && k < spec.first + spec.count;
    if (in_range) {
      spec.entries.push_back(r.Below(table_entries - start));
    } else if (sub_scene != 0 && !sub_used && r.Below(3) == 0) {
      spec.entries.push_back(sub_scene - scene);
      sub_used = true;
    } else {
      spec.entries.push_back(data + 3584 + 16 * r.Below(30) - scene);
    }
  }
  return spec;
}

void WriteScene(uint8_t* base, uint32_t scene, const SceneSpec& spec) {
  guest::StoreU32(base, scene, ui_movies_kSce);
  guest::StoreU32(base, scene + 116, spec.first);
  guest::StoreU32(base, scene + 120, spec.count);
  guest::StoreU32(base, scene + 124, spec.children);
  for (uint32_t k = 0; k < spec.children; k++) {
    guest::StoreU32(base, scene + 128 + 8 * k, spec.entries[k]);
  }
}

// The plain nodes' tags.
void WritePlainNodes(Context& h, const std::vector<uint32_t>& tags) {
  for (uint32_t k = 0; k < 30; k++) {
    guest::StoreU32(h.base, h.data + 3584 + 16 * k, tags[k]);
  }
}

std::vector<uint32_t> RandomTags(Random& r) {
  constexpr uint32_t kTags[] = {0x47525000, 0x50494300, 0x50524D00, 0x54585400, 0x53434501};
  std::vector<uint32_t> tags(30);
  for (uint32_t& tag : tags) {
    tag = r.Below(3) == 0 ? (uint32_t(r.Next()) | 1) : kTags[r.Below(5)];
  }
  return tags;
}

// sub_8259D688: a scene with a sub-scene (itself with plain children).
// sub_825A0ED8 alone: indices into the table.
void FixUpSceneCases(Context& h) {
  Results results("FixUpScene (sub_8259D688)");
  Random& r = h.random;
  for (int i = 0; i < 400; i++) {
    const uint32_t file = h.data, scene = h.data + 512, sub = h.data + 1536;
    const uint32_t start = r.Below(4);
    const SceneSpec top = RandomScene(r, scene, h.data, start, sub, 16);
    const SceneSpec inner = RandomScene(r, sub, h.data, start, 0, 16);
    const std::vector<uint32_t> tags = RandomTags(r);
    std::vector<uint32_t> table(16);
    for (uint32_t& entry : table) {
      entry = h.data + 3584 + 16 * r.Below(30);
    }
    Both(h, results,
         [&] {
           guest::StoreU32(h.base, file + 16, start);
           for (uint32_t k = 0; k < 16; k++) {
             guest::StoreU32(h.base, file + 32 + 8 * k, table[k]);
           }
           WriteScene(h.base, scene, top);
           WriteScene(h.base, sub, inner);
           WritePlainNodes(h, tags);
         },
         [&](PPCContext& t) {
           t.r3.u64 = scene;
           t.r4.u64 = file;
         },
         __imp__sub_8259D688, NativeFixUpScene, h.data, kScratchBytes,
         fmt::format("case {}: start {} top {} children ({} from {}) inner {} ({} from {})", i,
                     start, top.children, top.count, top.first, inner.children, inner.count,
                     inner.first));
  }
  results.Report();

  Results animation_results("FileAnimation (sub_825A0ED8)");
  for (int i = 0; i < 300; i++) {
    const uint32_t start = r.Below(2) ? r.Below(200) : uint32_t(r.Next());
    const uint32_t index = (r.Below(300) - start) & 0xFFFFFFFFu;
    const uint32_t entry = uint32_t(r.Next());
    Both(h, animation_results,
         [&] {
           guest::StoreU32(h.base, h.data + 16, start);
           guest::StoreU32(h.base, h.data + 32 + 8 * (start + index), entry);
         },
         [&](PPCContext& t) {
           t.r3.u64 = h.data;
           t.r4.u64 = index;
         },
         __imp__sub_825A0ED8, NativeFileAnimation, h.data, kScratchBytes,
         fmt::format("start {:X} index {:X}", start, index));
  }
  animation_results.Report();
}

// UiMovie::FixUpFile: a whole movie file (16-byte aligned, the header at
// +8): its table ([+24] entries of 16, offsets to plain nodes or, from its
// start [+16], to its [+20] animations), the scene (at +512, with a
// sub-scene at +1536), the animations (at +2304 and +2560, their entries
// pointing at sub-animation records from +2816 and +3200). Also refused
// files: unaligned, a header byte off.
void FixUpFileCases(Context& h) {
  Results results("UiMovie::FixUpFile (sub_825A0EF0)");
  Random& r = h.random;
  const uint32_t file = (h.data + 15) & ~0xFu;
  const uint32_t scene = file + 512, sub = file + 1536;
  const uint32_t animations[2] = {file + 2304, file + 2560};
  const uint32_t records[2] = {file + 2816, file + 3200};
  for (int i = 0; i < 400; i++) {
    const int kind = i < 360 ? 0 : 1 + i % 2;  // 1 unaligned, 2 header off
    const uint32_t start = r.Below(4);
    const uint32_t anims = r.Below(3);
    const uint32_t table_entries = r.Below(4) == 0 ? r.Below(16) : 16;
    std::vector<uint32_t> table(16);
    for (uint32_t k = 0; k < 16; k++) {
      table[k] = (h.data + 3584 + 16 * r.Below(30)) - file;
    }
    for (uint32_t j = 0; j < anims; j++) {
      table[start + j] = animations[j] - file;
    }
    SceneSpec top = RandomScene(r, scene, h.data, start, sub, 16);
    SceneSpec inner = RandomScene(r, sub, h.data, start, 0, 16);
    const std::vector<uint32_t> tags = RandomTags(r);
    struct AnimationSpec {
      uint32_t entries, first, subs;
      std::vector<uint32_t> indices;
    } specs[2];
    for (AnimationSpec& a : specs) {
      a.entries = r.Below(9);
      a.first = a.entries == 0 ? 0 : r.Below(a.entries);
      a.subs = a.entries == 0 ? 0 : r.Below(a.entries - a.first + 1);
      for (int k = 0; k < 8; k++) {
        a.indices.push_back(r.Below(16 - start));
      }
    }
    // With fewer table entries than its animations need, the file has none.
    const bool fits = table_entries >= start + anims;
    const uint32_t header_byte = r.Below(8);
    const uint32_t misalign = 4 + 4 * r.Below(3);
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      const uint8_t header[8] = {'X', '2', 'D', 0, '0', '0', '5', 0};
      Write(h.base, file + 8, header, 8);
      if (kind == 2) {
        guest::StoreU8(h.base, file + 8 + header_byte, 0x5A);
      }
      guest::StoreU32(h.base, file + 4, scene - file);
      guest::StoreU32(h.base, file + 16, start);
      guest::StoreU32(h.base, file + 20, fits ? anims : 0);
      guest::StoreU32(h.base, file + 24, table_entries);
      for (uint32_t k = 0; k < 16; k++) {
        guest::StoreU32(h.base, file + 32 + 8 * k, table[k]);
      }
      // Entries the loop won't fix are already pointers (the scene's
      // children may index them).
      for (uint32_t k = table_entries; k < 16; k++) {
        guest::StoreU32(h.base, file + 32 + 8 * k, table[k] + file);
      }
      WriteScene(h.base, scene, top);
      WriteScene(h.base, sub, inner);
      WritePlainNodes(h, tags);
      for (int j = 0; j < 2; j++) {
        const AnimationSpec& a = specs[j];
        guest::StoreU32(h.base, animations[j] + 48, a.first);
        guest::StoreU32(h.base, animations[j] + 52, a.subs);
        guest::StoreU32(h.base, animations[j] + 56, a.entries);
        for (uint32_t k = 0; k < 8; k++) {
          guest::StoreU32(h.base, animations[j] + 64 + 8 * k, records[j] + 32 * k - animations[j]);
          guest::StoreU32(h.base, records[j] + 32 * k + 16, a.indices[k]);
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = file + (kind == 1 ? misalign : 0);
      (v == 0 ? __imp__sub_825A0EF0 : sub_825A0EF0)(t, h.base);
      result[v] = Read(h.base, h.data, kScratchBytes);
      AppendWord(result[v], t.r3.u32);
    }
    results.Check(result[0], result[1],
                  fmt::format("case {}: kind {} start {} animations {} table {}", i, kind, start,
                              anims, table_entries));
  }
  results.Report();
}

// UiMovie::RestorePose: two scenes (live and rest) in an area of guest stack
// below the scratch area (as the sprites harness's lists: the call's own
// stack stays above it), their children random nodes: GRP (with a range of
// leaves at indices 8 to 11), PIC, PRM (textures and vertices), SCE (a
// sub-scene of leaves), TFM, TXT or other tags; everything else random
// words with odd floats mixed in. Both scenes' entries from base index
// [+108]; the range walked from the live scene's +100 and +104, or a
// group's. Compared: the whole area and r3.
void RestorePoseCases(Context& h) {
  Results results("UiMovie::RestorePose (sub_82119B80)");
  constexpr uint32_t kAreaBelow = 0x10000, kAreaBytes = 0x2000;
  const uint32_t area = (h.data - kAreaBelow) & ~0xFu;
  const uint32_t live_scene = area, rest_scene = area + 256, live_sub = area + 512,
                 rest_sub = area + 768;
  auto live_node = [&](uint32_t k) { return area + 1024 + 320 * k; };
  auto rest_node = [&](uint32_t k) { return area + 4352 + 320 * k; };
  constexpr uint32_t kLeafTags[] = {0x50494300, 0x50524D00, 0x54464D00, 0x54585400, 0x12345678};
  Random& r = h.random;
  for (int i = 0; i < 500; i++) {
    std::vector<uint32_t> words(kAreaBytes / 4);
    for (uint32_t& w : words) {
      w = r.Below(4) == 0 ? kOddFloats[r.Below(std::size(kOddFloats))] : uint32_t(r.Next());
    }
    // The children: top-level indices 0..7 (the range within them), group
    // leaves 8..11, the sub-scene's 0..3. Nodes 0-9 (live and rest).
    const uint32_t first = r.Below(3), count = r.Below(7);
    const uint32_t base_live = r.Below(3), base_rest = r.Below(3);
    std::vector<std::pair<uint32_t, uint32_t>> scene_entries(12), sub_entries(4);
    std::vector<uint32_t> node_tags(10);
    std::vector<uint32_t> node_vertices(10), node_textures_live(10), node_textures_rest(10);
    for (uint32_t k = 0; k < 10; k++) {
      node_tags[k] = kLeafTags[r.Below(5)];
      node_vertices[k] = r.Below(5);
      node_textures_live[k] = r.Below(3);
      node_textures_rest[k] = r.Below(3);
    }
    // Nodes 8 and 9 may be a group and a sub-scene.
    node_tags[8] = r.Below(2) ? 0x47525000 : kLeafTags[r.Below(5)];
    node_tags[9] = r.Below(2) ? 0x53434500 : kLeafTags[r.Below(5)];
    for (uint32_t k = 0; k < 12; k++) {
      const uint32_t node = k < 8 ? r.Below(10) : r.Below(8);
      scene_entries[k] = {live_node(node), rest_node(node)};
    }
    for (uint32_t k = 0; k < 4; k++) {
      const uint32_t node = r.Below(8);
      sub_entries[k] = {live_node(node), rest_node(node)};
    }
    const uint32_t group_count = r.Below(5), sub_count = r.Below(5);
    const bool use_group = r.Below(4) == 0;
    const bool no_rest_group = r.Below(4) == 0;  // r7 0: the live scene's range
    const uint32_t movie = uint32_t(r.Next());
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t k = 0; k < words.size(); k++) {
        guest::StoreU32(h.base, area + 4 * k, words[k]);
      }
      auto scene = [&](uint32_t at, uint32_t entry_base, uint32_t range_first, uint32_t range_count,
                       const std::vector<std::pair<uint32_t, uint32_t>>& entries, bool live) {
        guest::StoreU32(h.base, at + 100, range_first);
        guest::StoreU32(h.base, at + 104, range_count);
        guest::StoreU32(h.base, at + 108, entry_base);
        for (uint32_t k = 0; k < entries.size(); k++) {
          guest::StoreU32(h.base, at + 128 + 8 * (entry_base + k),
                          live ? entries[k].first : entries[k].second);
        }
      };
      scene(live_scene, base_live, first, count, scene_entries, true);
      scene(rest_scene, base_rest, first, count, scene_entries, false);
      scene(live_sub, 0, 0, sub_count, sub_entries, true);
      scene(rest_sub, 0, 0, sub_count, sub_entries, false);
      guest::StoreU32(h.base, live_sub, 0x53434500);
      for (uint32_t k = 0; k < 10; k++) {
        guest::StoreU32(h.base, live_node(k), node_tags[k]);
        guest::StoreU32(h.base, live_node(k) + 112, node_textures_live[k]);
        guest::StoreU32(h.base, rest_node(k) + 112, node_textures_rest[k]);
        guest::StoreU32(h.base, rest_node(k) + 116, node_vertices[k]);
      }
      // The group's leaves (8 to 11 + its base); the sub-scene node points
      // the walk into the sub-scenes.
      guest::StoreU32(h.base, live_node(8) + 100, 8);
      guest::StoreU32(h.base, live_node(8) + 104, group_count);
      if (node_tags[9] == 0x53434500) {
        // A scene node: its children are the sub-scene's (the live node is
        // the sub-scene itself).
        for (uint32_t k = 0; k < 12; k++) {
          for (int side = 0; side < 2; side++) {
            const uint32_t entry = (side == 0 ? live_scene + 128 + 8 * (base_live + k)
                                              : rest_scene + 128 + 8 * (base_rest + k));
            if (guest::LoadU32(h.base, entry) == (side == 0 ? live_node(9) : rest_node(9))) {
              guest::StoreU32(h.base, entry, side == 0 ? live_sub : rest_sub);
            }
          }
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = movie;
      t.r4.u64 = live_scene;
      t.r5.u64 = rest_scene;
      t.r6.u64 = use_group ? live_node(8) : 0;
      t.r7.u64 = use_group && !no_rest_group ? rest_node(8) : 0;
      (v == 0 ? __imp__sub_82119B80 : sub_82119B80)(t, h.base);
      result[v] = Read(h.base, area, kAreaBytes);
      AppendWord(result[v], t.r3.u32);
    }
    size_t at = 0;
    while (at < result[0].size() && result[0][at] == result[1][at]) {
      at++;
    }
    const size_t from = at < result[0].size() ? at : 0;
    const size_t to = std::min(result[0].size(), from + 32);
    results.Check({result[0].begin() + from, result[0].begin() + to},
                  {result[1].begin() + from, result[1].begin() + to},
                  fmt::format("case {}: range {} {} group {} ({}) sub {}, from byte {}", i, first,
                              count, node_tags[8] == 0x47525000, use_group, sub_count, from));
  }
  results.Report();
}

// UiMovie::ApplyTransform (sub_8259E6C8): a draw context (random matrix,
// clip zero or random, color) and a node (position, pivot, scale, rotation,
// rectangle, color) in the scratch area, each field often at its "nothing to
// do" value (0, 1.0, near the threshold) and with odd floats (NaN, -0.0,
// infinities, huge). The context's matrix and color compared as floats, the
// rest (clip, +96..+108, the node) exactly, and r3.
void ApplyTransformCases(Context& h) {
  Results results("UiMovie::ApplyTransform (sub_8259E6C8)");
  Random r;  // its own, so the harnesses after it keep their inputs
  const uint32_t threshold = guest::LoadU32(h.base, 0x82002A44);
  const float threshold_f = std::bit_cast<float>(threshold);
  const uint32_t odd[] = {0x7FC00000u, 0xFFC00000u, 0x80000000u, 0x7F800000u, 0xFF800000u,
                          0x4F000000u /* 2^31 */, 0xCF000001u, 0x5F000000u, 0x00000001u,
                          threshold, threshold ^ 0x80000000u, threshold + 1,
                          (threshold + 1) ^ 0x80000000u, threshold - 1, 0x3F800000u,
                          0x7F800001u /* signaling */, 0xFFA00000u};
  const auto odd_or = [&](uint32_t usual, uint32_t otherwise) {
    const uint32_t pick = r.Below(10);
    if (pick == 0) {
      return odd[r.Below(std::size(odd))];
    }
    return pick < 5 ? usual : otherwise;
  };
  const auto small_int = [&] {
    const uint32_t pick = r.Below(12);
    if (pick == 0) return 0x80000000u;
    if (pick == 1) return 0x7FFFFFFFu;
    if (pick == 2) return uint32_t(r.Next());
    return uint32_t(int32_t(r.Below(1400)) - 700);
  };
  for (int c = 0; c < 3000; c++) {
    const uint32_t context = h.data + 16 * r.Below(2);  // aligned, and not (a frame's +16)
    const uint32_t node = h.data + 192 + 4 * r.Below(4);
    uint32_t cw[28];
    for (int i = 0; i < 16; i++) {
      cw[i] = r.Below(12) == 0 ? odd[r.Below(std::size(odd))] : RandomFloat(r, -800, 800);
    }
    const bool clip_set = r.Below(2) == 0;
    for (int i = 16; i < 20; i++) {
      cw[i] = clip_set ? small_int() : 0;
    }
    for (int i = 20; i < 24; i++) {
      cw[i] = odd_or(0x3F800000u, RandomFloat(r, 0, 1));
    }
    for (int i = 24; i < 28; i++) {
      cw[i] = r.Below(2) == 0 ? 0 : uint32_t(r.Next());
    }
    uint32_t nw[18];
    for (int i = 0; i < 3; i++) {
      nw[i] = uint32_t(r.Next());
    }
    nw[3] = odd_or(0, RandomFloat(r, -640, 640));  // +12, +16: the position
    nw[4] = odd_or(0, RandomFloat(r, -360, 360));
    nw[5] = r.Below(2) == 0 ? 0 : small_int();  // +20, +24: the pivot
    nw[6] = r.Below(2) == 0 ? 0 : small_int();
    nw[7] = odd_or(0x3F800000u, RandomFloat(r, -3, 3));  // +28, +32: the scale
    nw[8] = odd_or(0x3F800000u, r.Below(3) == 0 ? RandomFloat(r, -threshold_f * 2,
                                                              threshold_f * 2)
                                                : RandomFloat(r, -3, 3));
    nw[9] = odd_or(0, RandomFloat(r, -720, 720));  // +36: the rotation
    const bool rect = r.Below(2) == 0;
    for (int i = 10; i < 14; i++) {  // +40..+52
      nw[i] = rect && r.Below(4) != 0 ? small_int() : 0;
    }
    for (int i = 14; i < 17; i++) {  // +56..+64
      nw[i] = r.Below(3) == 0 ? small_int() : r.Below(256);
    }
    nw[17] = odd_or(0x3F800000u, RandomFloat(r, 0, 1));  // +68
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (int i = 0; i < 28; i++) {
        guest::StoreU32(h.base, context + 4 * i, cw[i]);
      }
      for (int i = 0; i < 18; i++) {
        guest::StoreU32(h.base, node + 4 * i, nw[i]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = context;
      t.r4.u64 = node;
      (v == 0 ? __imp__sub_8259E6C8 : NativeApplyTransform)(t, h.base);
      out[v].Floats(h.base, context, 16);
      out[v].Bytes(Read(h.base, context + 64, 16));
      out[v].Floats(h.base, context + 80, 4);
      out[v].Bytes(Read(h.base, context + 96, 16));
      out[v].Bytes(Read(h.base, node, 72));
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: clip {} rect {} pos {:08X} {:08X} pivot {:08X} {:08X} "
                              "scale {:08X} {:08X} turn {:08X}",
                              c, clip_set, rect, nw[3], nw[4], nw[5], nw[6], nw[7], nw[8],
                              nw[9]));
  }
  results.Report();
}

// The draw walk's copies and the texts' queue: sub_8259D9F8 (a context from
// its parent's, apart or overlapping), sub_8259E620 (a text item, apart or
// overlapping), sub_8259F700 (a list with room: a full one grows, the
// original's) and UiMovie::QueueText (a context with no list or one with
// room, a node as ApplyTransform's cases make them, a random layer with
// junk in r6's upper half). Floats only where the code loads and stores
// them as floats (the matrix's and color's arithmetic in QueueText's
// transform, the items' +80); the rest exactly.
// UiMovie::DrawPicture (sub_8259ED40) and UiMovie::DrawPrimitive
// (sub_8259F348), the whole path: the quad built natively and handed to
// sub_8259EBE8's adapter (ui_movies_hooks::RecordPicture and
// RecordPrimitive) against the originals run with --native_ui_movies off
// (so their callees are the translated ones too). A made-up command list,
// renderer (slot 4 BillboardMaterial::SetLights, storing r4 and r5 at its
// +32 and +36, as RecorderCases) and texture table in the scratch area, the
// game's globals pointed at them during the cases. A random draw context
// (matrix, clip set or not, color, +108) and node (the transform's fields,
// a texture or none, a zero-sized texture, the flips; a primitive's 0 to 16
// vertices, its color flag), with odd floats (NaN, infinities) in some.
// Compared: the whole scratch area (the recorded chunks, the context, the
// node), the open chunk count and r3.
void QuadDrawCases(Context& h) {
  Random r;  // its own, so the harnesses after it keep their inputs
  constexpr uint32_t kList = 0, kRendererObject = 64, kVtable = 128, kContext = 256,
                     kNode = 512, kTableRef = 1536, kTable = 1600, kTexture = 1664,
                     kSlots = 2048;
  const uint32_t saved_renderer = guest::LoadU32(h.base, ui_movies_kRenderer);
  const uint32_t saved_list = guest::LoadU32(h.base, ui_movies_kRenderer + 4);
  const uint32_t saved_open = guest::LoadU32(h.base, command_list::kOpenChunks);
  const uint32_t odd[] = {0x7FC00000u, 0xFFC00000u, 0x7F800000u, 0xFF800000u, 0x80000000u,
                          0x7F800001u, 0x00000001u, 0x5F800000u};
  const auto value = [&](float lo, float hi) {
    return r.Below(40) == 0 ? odd[r.Below(std::size(odd))] : RandomFloat(r, lo, hi);
  };
  for (int kind = 0; kind < 2; kind++) {
    const bool picture = kind == 0;
    Results results(picture ? "UiMovie::DrawPicture (sub_8259ED40)"
                            : "UiMovie::DrawPrimitive (sub_8259F348)");
    for (int c = 0; c < 600; c++) {
      uint32_t cw[28];
      for (int i = 0; i < 16; i++) {
        cw[i] = value(-2, 2);
      }
      const bool clip = r.Below(2) == 0;
      for (int i = 16; i < 20; i++) {
        cw[i] = clip ? (r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(1400)) : 0;
      }
      if (clip && r.Below(4) == 0) {  // only +72 set: no clip, as read
        cw[16] = cw[17] = cw[19] = 0;
      }
      for (int i = 20; i < 24; i++) {
        cw[i] = value(0, 1);
      }
      cw[24] = 0;  // the texture table, set below
      cw[25] = uint32_t(r.Next());
      cw[26] = uint32_t(r.Next());
      cw[27] = r.Below(2) ? 0 : 1;
      // The node: the transform's fields mostly at rest.
      uint32_t nw[256];
      for (uint32_t& w : nw) {
        w = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(64);
      }
      nw[3] = r.Below(2) ? 0 : value(-640, 640);
      nw[4] = r.Below(2) ? 0 : value(-360, 360);
      nw[5] = r.Below(2) ? 0 : r.Below(200);
      nw[6] = r.Below(2) ? 0 : r.Below(200);
      nw[7] = r.Below(2) ? 0x3F800000u : value(-3, 3);
      nw[8] = r.Below(2) ? 0x3F800000u : value(-3, 3);
      nw[9] = r.Below(2) ? 0 : value(-360, 360);
      for (int i = 10; i < 14; i++) {
        nw[i] = r.Below(3) == 0 ? r.Below(1280) : 0;
      }
      for (int i = 14; i < 17; i++) {
        nw[i] = r.Below(256);
      }
      nw[17] = value(0, 1);
      nw[18] = r.Below(1000);  // +72: the key
      const uint32_t texture_index = r.Below(4);
      const bool textured = r.Below(4) != 0;
      uint32_t vertices = 0;
      if (picture) {
        nw[25] = value(0, 512);  // +100, +104: the size
        nw[26] = value(0, 512);
        nw[27] = r.Below(3);  // +108: the texture entry
        for (int i = 29; i < 33; i++) {  // +116..+128: the pixel rectangle
          nw[i] = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(512);
        }
        nw[33] = r.Below(2) ? 0 : 1 + r.Below(3);  // +132, +136: the flips
        nw[34] = r.Below(2) ? 0 : 1;
        nw[35] = 1 + r.Below(2);  // +140: a texture (see null_texture)
        for (int i = 0; i < 3; i++) {  // the entries from +144: +0 the index
          nw[36 + 4 * i] = texture_index;
        }
      } else {
        nw[25] = r.Below(2) ? 0 : 1 + r.Below(3);  // +100: the color flag (and blend mode)
        const uint32_t entries = textured ? 1 + r.Below(2) : 0;
        nw[26] = 0;  // +104: the texture entry
        nw[28] = entries;  // +112
        vertices = r.Below(8) == 0 ? 16 : r.Below(9);
        nw[29] = vertices;  // +116
        for (uint32_t i = 0; i < entries; i++) {
          nw[32 + 4 * i] = texture_index;
        }
        for (uint32_t i = 0; i < vertices; i++) {
          const uint32_t at = 32 + 4 * entries + 8 * i;
          nw[at + 0] = value(-300, 300);
          nw[at + 1] = value(-300, 300);
          nw[at + 2] = r.Below(256);
          nw[at + 3] = r.Below(256);
          nw[at + 4] = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(256);
          nw[at + 5] = value(0, 1);
          nw[at + 6] = r.Below(512);
          nw[at + 7] = r.Below(512);
        }
      }
      const uint32_t size_case = r.Below(6);  // 0: a zero width, 1: a zero height
      const uint32_t width = size_case == 0 ? 0 : 1 + r.Below(1024);
      const uint32_t height = size_case == 1 ? 0 : 1 + r.Below(1024);
      // A picture without a texture faults in the original (its size read at
      // guest address 4): the adapter leaves those to it, so no cases.
      const bool null_texture = !picture && r.Below(6) == 0;
      const uint32_t texture_word = uint32_t(r.Next());
      const uint32_t layer = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(100);
      std::vector<uint8_t> result[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        const uint32_t list = h.data + kList;
        guest::StoreU32(h.base, list + 8, 1000);
        guest::StoreU32(h.base, list + 12, 0);
        guest::StoreU32(h.base, list + 32, h.data + kSlots);
        guest::StoreU8(h.base, list + 52, 0);
        guest::StoreU32(h.base, command_list::kOpenChunks, 0);
        guest::StoreU32(h.base, h.data + kRendererObject, h.data + kVtable);
        guest::StoreU32(h.base, h.data + kVtable + 16, 0x8209F5C8);
        const uint32_t context = h.data + kContext, node = h.data + kNode;
        for (int i = 0; i < 28; i++) {
          guest::StoreU32(h.base, context + 4 * i, cw[i]);
        }
        guest::StoreU32(h.base, context + 96, h.data + kTableRef);
        guest::StoreU32(h.base, h.data + kTableRef, h.data + kTable);
        for (uint32_t i = 0; i < 4; i++) {
          guest::StoreU32(h.base, h.data + kTable + 4 * i,
                          null_texture && i == texture_index ? 0 : h.data + kTexture);
        }
        guest::StoreU32(h.base, h.data + kTexture + 0, texture_word);
        guest::StoreU32(h.base, h.data + kTexture + 4, width);
        guest::StoreU32(h.base, h.data + kTexture + 8, height);
        for (int i = 0; i < 256; i++) {
          guest::StoreU32(h.base, node + 4 * i, nw[i]);
        }
        guest::StoreU32(h.base, ui_movies_kRenderer, h.data + kRendererObject);
        guest::StoreU32(h.base, ui_movies_kRenderer + 4, list);
        PPCContext t = CallContext(h);
        t.r3.u64 = context;
        t.r4.u64 = node;  // the scene, not read
        t.r5.u64 = node;
        t.r6.u64 = layer;
        if (v == 0) {
          ITC_FLAG(native_ui_movies) = false;
          (picture ? __imp__sub_8259ED40 : __imp__sub_8259F348)(t, h.base);
          ITC_FLAG(native_ui_movies) = true;
        } else {
          (picture ? ui_movies_hooks::RecordPicture : ui_movies_hooks::RecordPrimitive)(t, h.base);
        }
        result[v] = Read(h.base, h.data, kScratchBytes);
        AppendWord(result[v], guest::LoadU32(h.base, command_list::kOpenChunks));
        AppendWord(result[v], uint32_t(t.r3.u64 >> 32));
        AppendWord(result[v], t.r3.u32);
      }
      size_t at = 0;
      while (at < result[0].size() && result[0][at] == result[1][at]) {
        at++;
      }
      const size_t from = at < result[0].size() ? at : 0;
      const size_t to = std::min(result[0].size(), from + 32);
      results.Check({result[0].begin() + from, result[0].begin() + to},
                    {result[1].begin() + from, result[1].begin() + to},
                    fmt::format("case {}: clip {} textured {} size {} null {} vertices {}, "
                                "from byte {}",
                                c, clip, textured, size_case, null_texture, vertices, from));
    }
    results.Report();
  }
  guest::StoreU32(h.base, ui_movies_kRenderer, saved_renderer);
  guest::StoreU32(h.base, ui_movies_kRenderer + 4, saved_list);
  guest::StoreU32(h.base, command_list::kOpenChunks, saved_open);
}

void DrawWalkCases(Context& h) {
  Random r;  // its own, so the harnesses after it keep their inputs
  const auto random_words = [&](uint32_t* words, int count) {
    for (int i = 0; i < count; i++) {
      words[i] = r.Below(3) == 0 ? uint32_t(r.Next()) : RandomFloat(r, -500, 500);
    }
  };
  {
    Results results("DrawContext::Copy (sub_8259D9F8)");
    for (int c = 0; c < 500; c++) {
      uint32_t words[28], fill[28];
      random_words(words, 28);
      random_words(fill, 28);
      const uint32_t parent = h.data + 256;
      const uint32_t context =
          r.Below(4) == 0 ? parent + 16 * (int32_t(r.Below(7)) - 3) : h.data + 16 * r.Below(4);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        for (int i = 0; i < 28; i++) {
          guest::StoreU32(h.base, context + 4 * i, fill[i]);
        }
        for (int i = 0; i < 28; i++) {
          guest::StoreU32(h.base, parent + 4 * i, words[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = context;
        t.r4.u64 = parent;
        (v == 0 ? __imp__sub_8259D9F8 : NativeCopyDrawContext)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, 512));
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}: context {:X} parent {:X}", c,
                                                context - h.data, parent - h.data));
    }
    results.Report();
  }
  {
    Results results("TextItem::CopyQueued (sub_8259E620)");
    for (int c = 0; c < 500; c++) {
      uint32_t words[32], fill[32];
      random_words(words, 32);
      random_words(fill, 32);
      const uint32_t from = h.data + 512;
      const uint32_t to =
          r.Below(4) == 0 ? from + 16 * (int32_t(r.Below(9)) - 4) : h.data + 16 * r.Below(8);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        for (int i = 0; i < 32; i++) {
          guest::StoreU32(h.base, to + 4 * i, fill[i]);
        }
        for (int i = 0; i < 32; i++) {
          guest::StoreU32(h.base, from + 4 * i, words[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = to;
        t.r4.u64 = from;
        (v == 0 ? __imp__sub_8259E620 : NativeCopyQueuedText)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, to + 80 - h.data));
        out[v].Float(guest::LoadU32(h.base, to + 80));
        out[v].Bytes(Read(h.base, to + 84, h.data + 1024 - (to + 84)));
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}: to {:X} from {:X}", c,
                                                to - h.data, from - h.data));
    }
    results.Report();
  }
  // A list at +0 of the scratch area, its items from +128 (up to 8), the
  // item to add at +1536.
  {
    Results results("TextList::Append (sub_8259F700)");
    for (int c = 0; c < 300; c++) {
      const uint32_t capacity = 1 + r.Below(8);
      const uint32_t count = r.Below(capacity);
      uint32_t item[32];
      random_words(item, 32);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        guest::StoreU32(h.base, h.data + 0, h.data + 128);
        guest::StoreU32(h.base, h.data + 4, capacity);
        guest::StoreU32(h.base, h.data + 8, count);
        for (int i = 0; i < 32; i++) {
          guest::StoreU32(h.base, h.data + 1536 + 4 * i, item[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data;
        t.r4.u64 = h.data + 1536;
        (v == 0 ? __imp__sub_8259F700 : NativeAppendText)(t, h.base);
        const uint32_t slot = h.data + 128 + 128 * count;
        out[v].Bytes(Read(h.base, h.data, slot + 80 - h.data));
        out[v].Float(guest::LoadU32(h.base, slot + 80));
        out[v].Bytes(Read(h.base, slot + 84, h.data + 2048 - (slot + 84)));
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}: {} of {}", c, count, capacity));
    }
    results.Report();
  }
  // The context at +0, the node at +192, the list at +512 with its items
  // from +640.
  {
    Results results("UiMovie::QueueText (sub_8259F770)");
    for (int c = 0; c < 1000; c++) {
      uint32_t cw[28];
      for (int i = 0; i < 16; i++) {
        cw[i] = RandomFloat(r, -800, 800);
      }
      const bool clip_set = r.Below(2) == 0;
      for (int i = 16; i < 20; i++) {
        cw[i] = clip_set ? uint32_t(int32_t(r.Below(1400)) - 700) : 0;
      }
      for (int i = 20; i < 24; i++) {
        cw[i] = r.Below(2) == 0 ? 0x3F800000u : RandomFloat(r, 0, 1);
      }
      const bool with_list = r.Below(5) != 0;
      cw[24] = uint32_t(r.Next());
      cw[25] = with_list ? h.data + 512 : 0;
      cw[26] = uint32_t(r.Next());
      cw[27] = r.Below(2);
      uint32_t nw[18];
      for (int i = 0; i < 18; i++) {
        nw[i] = 0;
      }
      nw[3] = r.Below(2) == 0 ? 0 : RandomFloat(r, -640, 640);
      nw[4] = r.Below(2) == 0 ? 0 : RandomFloat(r, -360, 360);
      nw[5] = r.Below(2) == 0 ? 0 : uint32_t(int32_t(r.Below(200)) - 100);
      nw[6] = r.Below(2) == 0 ? 0 : uint32_t(int32_t(r.Below(200)) - 100);
      nw[7] = r.Below(2) == 0 ? 0x3F800000u : RandomFloat(r, -3, 3);
      nw[8] = r.Below(2) == 0 ? 0x3F800000u : RandomFloat(r, -3, 3);
      nw[9] = r.Below(2) == 0 ? 0 : RandomFloat(r, -720, 720);
      for (int i = 10; i < 14; i++) {
        nw[i] = r.Below(3) == 0 ? uint32_t(int32_t(r.Below(1400)) - 700) : 0;
      }
      for (int i = 14; i < 17; i++) {
        nw[i] = r.Below(256);
      }
      nw[17] = r.Below(2) == 0 ? 0x3F800000u : RandomFloat(r, 0, 1);
      const uint32_t capacity = 1 + r.Below(4);
      const uint32_t count = r.Below(capacity);
      const uint64_t r6 = (r.Next() << 32) |
                          (r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(40));
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        for (int i = 0; i < 28; i++) {
          guest::StoreU32(h.base, h.data + 4 * i, cw[i]);
        }
        for (int i = 0; i < 18; i++) {
          guest::StoreU32(h.base, h.data + 192 + 4 * i, nw[i]);
        }
        guest::StoreU32(h.base, h.data + 512, h.data + 640);
        guest::StoreU32(h.base, h.data + 516, capacity);
        guest::StoreU32(h.base, h.data + 520, count);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data;
        t.r5.u64 = h.data + 192;
        t.r6.u64 = r6;
        (v == 0 ? __imp__sub_8259F770 : NativeQueueText)(t, h.base);
        out[v].Floats(h.base, h.data, 16);
        out[v].Bytes(Read(h.base, h.data + 64, 16));
        out[v].Floats(h.base, h.data + 80, 4);
        out[v].Bytes(Read(h.base, h.data + 96, 512 - 96));
        out[v].Bytes(Read(h.base, h.data + 512, 128));
        for (uint32_t k = 0; k < 4; k++) {  // the items: +16..+79 the matrix, +80 the depth
          const uint32_t at = h.data + 640 + 128 * k;
          out[v].Bytes(Read(h.base, at, 16));
          out[v].Floats(h.base, at + 16, 17);
          out[v].Bytes(Read(h.base, at + 84, 44));
        }
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}: list {} {} of {} layer {:X}", c,
                                                with_list, count, capacity, r6));
    }
    results.Report();
  }
  if (g_draw_fallbacks != 0) {
    ITC_LOG_INFO("harness: ui_movies: {} draw walk cases handed to the original",
                 g_draw_fallbacks);
  }
}

// UiMovie::DrawNode (sub_8259F848) and UiMovie::Draw (sub_8259FA00), the
// whole walk: ui_movies_hooks::RecordDrawNode and Draw (flag on) against the
// originals run with --native_ui_movies off. Small trees of up to 11 nodes
// in a guest block: a root scene whose table points at the nodes after it,
// each node a group (children from the scene's table, after it), a
// sub-scene (its own table), a textured picture, a primitive, a text, a
// transform or junk, some invisible or scaled to (near) nothing, the
// transforms mostly at rest; a made-up command list, renderer and texture
// table as QuadDrawCases', and a text list with room, full (the original
// QueueText) or none. Compared: the scratch area, the block (nodes,
// recorded slots, queued texts), the open chunk count and r3, exactly.
void TreeDrawCases(Context& h) {
  Random r;  // its own, so the harnesses after it keep their inputs
  constexpr uint32_t kList = 0, kRendererObject = 64, kVtable = 128, kContext = 256,
                     kTextList = 384, kTableRef = 1536, kTable = 1600, kTexture = 1664;
  constexpr uint32_t kBlockBytes = 0x10000, kNodeBytes = 1024, kNodes = 12, kLeaf = 11;
  constexpr uint32_t kSlots = 0x3000, kTexts = 0x8000;
  const uint32_t block = memory::Alloc(kBlockBytes, 0);
  if (block == 0) {
    ITC_LOG_ERROR("harness: ui_movies: no block for the tree cases");
    return;
  }
  const uint32_t saved_renderer = guest::LoadU32(h.base, ui_movies_kRenderer);
  const uint32_t saved_list = guest::LoadU32(h.base, ui_movies_kRenderer + 4);
  const uint32_t saved_open = guest::LoadU32(h.base, command_list::kOpenChunks);
  const uint32_t threshold = guest::LoadU32(h.base, 0x82002A44);
  const uint32_t odd[] = {0x7FC00000u, 0xFFC00000u, 0x7F800000u, 0x80000000u, 0x7F800001u,
                          0x00000001u, threshold, threshold + 1, threshold ^ 0x80000000u};
  const auto value = [&](float lo, float hi) {
    return r.Below(40) == 0 ? odd[r.Below(std::size(odd))] : RandomFloat(r, lo, hi);
  };
  constexpr uint32_t kTags[] = {0x47525000, 0x53434500, 0x50494300, 0x50524D00,
                                0x54585400, 0x54464D00, 0x12345678};
  Results results("UiMovie::DrawNode (sub_8259F848) and Draw (sub_8259FA00)");
  std::vector<uint8_t> image(kBlockBytes);
  for (int c = 0; c < 400; c++) {
    const bool through_draw = c % 2 == 1;
    const uint32_t count = 1 + r.Below(kLeaf - 1);  // nodes 1..count after the root
    std::vector<uint32_t> words(kBlockBytes / 4, 0);
    const auto node_at = [&](uint32_t k) { return block + kNodeBytes * k; };
    const auto w = [&](uint32_t k, uint32_t offset) -> uint32_t& {
      return words[(kNodeBytes * k + offset) / 4];
    };
    for (uint32_t k = 0; k <= kLeaf; k++) {
      if (k > count && k < kLeaf) {
        continue;
      }
      for (uint32_t i = 0; i < kNodeBytes / 4; i++) {
        w(k, 4 * i) = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(64);
      }
      const uint32_t tag = k == 0       ? 0x53434500
                           : k == kLeaf ? 0x54464D00
                           : k <= 3     ? kTags[r.Below(std::size(kTags))]
                                        : kTags[2 + r.Below(std::size(kTags) - 2)];
      w(k, 0) = tag;
      w(k, 12) = r.Below(2) ? 0 : value(-640, 640);
      w(k, 16) = r.Below(2) ? 0 : value(-360, 360);
      w(k, 20) = r.Below(2) ? 0 : r.Below(200);
      w(k, 24) = r.Below(2) ? 0 : r.Below(200);
      w(k, 28) = r.Below(4) ? 0x3F800000u : value(-3, 3);
      w(k, 32) = r.Below(4) ? 0x3F800000u : value(-3, 3);
      w(k, 36) = r.Below(2) ? 0 : value(-360, 360);
      for (uint32_t o = 40; o < 56; o += 4) {
        w(k, o) = r.Below(4) == 0 ? r.Below(1280) : 0;
      }
      for (uint32_t o = 56; o < 68; o += 4) {
        w(k, o) = r.Below(256);
      }
      w(k, 68) = value(0, 1);
      w(k, 72) = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(100);
      w(k, 80) = r.Below(6) == 0 ? 0 : 1;
      if (tag == 0x47525000 || tag == 0x53434500) {
        // A scene's table (the root's, a sub-scene's): entry j at +128 + 8 j
        // is node k + 1 + j, or the leaf after the last node, from index
        // [+108] = 0. Its children: any range of the table. A group's
        // children come from the scene it's drawn in, whose entry j is a
        // node after j: entries from k on, so every level goes deeper.
        w(k, 108) = 0;
        for (uint32_t j = 0; j < kNodes; j++) {
          w(k, 128 + 8 * j) = node_at(std::min(k + 1 + j, kLeaf));
        }
        const uint32_t first = tag == 0x47525000 && k != 0 ? k + r.Below(kNodes - k) : r.Below(kNodes);
        w(k, 100) = first;
        w(k, 104) = r.Below(std::min(kNodes - first, 3u) + 1);  // at most 3 children
      } else if (tag == 0x50494300) {
        w(k, 100) = value(0, 512);
        w(k, 104) = value(0, 512);
        w(k, 108) = r.Below(3);
        for (uint32_t o = 116; o < 132; o += 4) {
          w(k, o) = r.Below(512);
        }
        w(k, 132) = r.Below(2) ? 0 : 1 + r.Below(3);
        w(k, 136) = r.Below(2) ? 0 : 1;
        w(k, 140) = 1 + r.Below(2);
        for (uint32_t i = 0; i < 3; i++) {
          w(k, 144 + 16 * i) = r.Below(4);
        }
      } else if (tag == 0x50524D00) {
        w(k, 100) = r.Below(2) ? 0 : 1 + r.Below(3);
        const uint32_t entries = r.Below(3);
        w(k, 104) = 0;
        w(k, 112) = entries;
        const uint32_t vertices = r.Below(7);
        w(k, 116) = vertices;
        for (uint32_t i = 0; i < entries; i++) {
          w(k, 128 + 16 * i) = r.Below(4);
        }
        for (uint32_t i = 0; i < vertices; i++) {
          const uint32_t at = 128 + 16 * entries + 32 * i;
          w(k, at + 0) = value(-300, 300);
          w(k, at + 4) = value(-300, 300);
          w(k, at + 8) = r.Below(256);
          w(k, at + 12) = r.Below(256);
          w(k, at + 16) = r.Below(256);
          w(k, at + 20) = value(0, 1);
          w(k, at + 24) = r.Below(512);
          w(k, at + 28) = r.Below(512);
        }
      }
    }
    for (size_t i = 0; i < words.size(); i++) {
      const uint32_t v = words[i];
      image[4 * i + 0] = uint8_t(v >> 24);
      image[4 * i + 1] = uint8_t(v >> 16);
      image[4 * i + 2] = uint8_t(v >> 8);
      image[4 * i + 3] = uint8_t(v);
    }
    uint32_t cw[28];
    for (int i = 0; i < 16; i++) {
      cw[i] = value(-2, 2);
    }
    const bool clip = r.Below(2) == 0;
    for (int i = 16; i < 20; i++) {
      cw[i] = clip ? r.Below(1400) : 0;
    }
    for (int i = 20; i < 24; i++) {
      cw[i] = value(0, 1);
    }
    cw[24] = 0;
    cw[25] = 0;
    cw[26] = uint32_t(r.Next());
    cw[27] = r.Below(2) ? 0 : 1;
    // 0: none, else room. A full list would grow (sub_8259EC58, never seen
    // in play), which the harness can't run safely (the game's threads
    // faulted after it, 2026-10-24): no full lists.
    const uint32_t text_list_kind = r.Below(4) == 0 ? 0 : 2;
    const uint32_t text_capacity = 64, text_count = r.Below(2);
    const uint32_t layer = r.Below(100);
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Write(h.base, block, image.data(), kBlockBytes);
      const uint32_t list = h.data + kList;
      guest::StoreU32(h.base, list + 8, 1000);
      guest::StoreU32(h.base, list + 12, 0);
      guest::StoreU32(h.base, list + 32, block + kSlots);
      guest::StoreU8(h.base, list + 52, 0);
      guest::StoreU32(h.base, command_list::kOpenChunks, 0);
      guest::StoreU32(h.base, h.data + kRendererObject, h.data + kVtable);
      guest::StoreU32(h.base, h.data + kVtable + 16, 0x8209F5C8);
      const uint32_t context = h.data + kContext;
      for (int i = 0; i < 28; i++) {
        guest::StoreU32(h.base, context + 4 * i, cw[i]);
      }
      const uint32_t text_list = text_list_kind == 0 ? 0 : h.data + kTextList;
      guest::StoreU32(h.base, h.data + kTextList + 0, block + kTexts);
      guest::StoreU32(h.base, h.data + kTextList + 4, text_capacity);
      guest::StoreU32(h.base, h.data + kTextList + 8, text_count);
      guest::StoreU32(h.base, h.data + kTableRef, h.data + kTable);
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreU32(h.base, h.data + kTable + 4 * i, h.data + kTexture);
      }
      guest::StoreU32(h.base, h.data + kTexture + 0, 0x1234);
      guest::StoreU32(h.base, h.data + kTexture + 4, 256);
      guest::StoreU32(h.base, h.data + kTexture + 8, 128);
      if (!through_draw) {
        guest::StoreU32(h.base, context + 96, h.data + kTableRef);
        guest::StoreU32(h.base, context + 100, text_list);
      }
      guest::StoreU32(h.base, ui_movies_kRenderer, h.data + kRendererObject);
      guest::StoreU32(h.base, ui_movies_kRenderer + 4, list);
      PPCContext t = CallContext(h);
      t.r3.u64 = context;
      t.r4.u64 = block;
      if (through_draw) {
        t.r5.u64 = h.data + kTableRef;
        t.r6.u64 = text_list;
        t.r7.u64 = 0xFFFFFFFFFFFFFFFFull;
      } else {
        t.r5.u64 = block;
        t.r6.u64 = layer;
      }
      if (v == 0) {
        ITC_FLAG(native_ui_movies) = false;
        (through_draw ? __imp__sub_8259FA00 : __imp__sub_8259F848)(t, h.base);
        ITC_FLAG(native_ui_movies) = true;
      } else {
        (through_draw ? ui_movies_hooks::Draw : ui_movies_hooks::RecordDrawNode)(t, h.base);
      }
      result[v] = Read(h.base, h.data, kScratchBytes);
      const std::vector<uint8_t> after = Read(h.base, block, kBlockBytes);
      result[v].insert(result[v].end(), after.begin(), after.end());
      AppendWord(result[v], guest::LoadU32(h.base, command_list::kOpenChunks));
      AppendWord(result[v], uint32_t(t.r3.u64 >> 32));
      AppendWord(result[v], t.r3.u32);
    }
    size_t at = 0;
    while (at < result[0].size() && result[0][at] == result[1][at]) {
      at++;
    }
    const size_t from = at < result[0].size() ? at : 0;
    const size_t to = std::min(result[0].size(), from + 32);
    results.Check({result[0].begin() + from, result[0].begin() + to},
                  {result[1].begin() + from, result[1].begin() + to},
                  fmt::format("case {}: draw {} nodes {} texts {}, from byte {}", c, through_draw,
                              count, text_list_kind, from));
  }
  results.Report();
  guest::StoreU32(h.base, ui_movies_kRenderer, saved_renderer);
  guest::StoreU32(h.base, ui_movies_kRenderer + 4, saved_list);
  guest::StoreU32(h.base, command_list::kOpenChunks, saved_open);
  memory::Free(block);
}

}  // namespace

void UiMovies(Context& h) {
  const Function arithmetic[] = {
      {"NodeTrack (sub_825A6508)", __imp__sub_825A6508, sub_825A6508},
      {"Key (sub_825A6518)", __imp__sub_825A6518, sub_825A6518},
      {"VertexOffset (sub_8259D600)", __imp__sub_8259D600, sub_8259D600},
      {"PrimitiveTexture (sub_8259D618)", __imp__sub_8259D618, sub_8259D618},
      {"PictureTexture (sub_8259D648)", __imp__sub_8259D648, sub_8259D648},
  };
  for (const Function& f : arithmetic) {
    Arithmetic(h, f);
  }
  Indexed(h, {"NodeTrackList (sub_825A6528)", __imp__sub_825A6528, sub_825A6528}, 64, 32);
  Indexed(h, {"KeyList (sub_825A6540)", __imp__sub_825A6540, sub_825A6540}, 64, 40);
  Indexed(h, {"SubAnimation (sub_825A6558)", __imp__sub_825A6558, sub_825A6558}, 64, 48);
  Indexed(h, {"UiMovie::Child (sub_8259D658)", __imp__sub_8259D658, sub_8259D658}, 128, 108);
  Indexed(h, {"SceneAnimation (sub_8259D670)", __imp__sub_8259D670, sub_8259D670}, 128, 116);
  PrimitiveVertex(h);
  FindChunkCases(h);
  FileSceneCases(h);
  XmdEntryCases(h);
  SetRendererCases(h);
  ResetDrawContextCases(h);
  KeyCases(h);
  RecorderCases(h);
  HeaderCases(h);
  FixUpAnimationCases(h);
  FixUpSceneCases(h);
  FixUpFileCases(h);
  RestorePoseCases(h);
  ApplyTransformCases(h);
  DrawWalkCases(h);
  QuadDrawCases(h);
  TreeDrawCases(h);
}

}  // namespace harness
