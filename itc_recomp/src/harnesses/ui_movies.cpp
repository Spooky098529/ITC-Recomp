// The UI movie library's functions (src/hooks/ui_movies.cpp) against the
// recompiled ones. Movies are live guest objects: every case builds its own
// in the scratch area, so nothing the game holds is read or written.

#include "harness.h"

#include "base/log.h"
#include "command_list/command_list.h"

#include <algorithm>
#include <bit>
#include <cmath>
#include <iterator>
#include <limits>

namespace harness {

namespace {

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
    int kind;  // 0 textured quad, 1 untextured, 2 the resets
  };
  const Recorder recorders[] = {
      {"RecordQuad, textured (sub_8259DAD0)", __imp__sub_8259DAD0, sub_8259DAD0, 0},
      {"RecordQuad, untextured (sub_8259E098)", __imp__sub_8259E098, sub_8259E098, 1},
      {"the clip and state resets (sub_8259D7B0)", __imp__sub_8259D7B0, sub_8259D7B0, 2},
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
      // The slots it needs, for "just room".
      uint32_t needed = 8;
      if (f.kind != 2) {
        const uint32_t blend[] = {5, 5, 5, 6, 4};
        needed = (f.kind == 0 ? 5 : 3) + (mode < 5 ? blend[mode] : 0) + 6 +
                 ((vertices * 24 + 15) >> 4) + 3;
      }
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
        guest::StoreU32(h.base, quad + 84, words[0]);
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
}

}  // namespace harness
