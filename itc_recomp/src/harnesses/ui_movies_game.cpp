// The game's side of the UI movies (src/hooks/ui_movies_game.cpp) against
// the recompiled functions. Movies are live guest objects: every case builds
// its own in a block taken from the system heap for the run (movies span
// about 41 KB, more than the scratch area), so nothing the game holds is
// read or written.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "hooks/strings_and_font.h"
#include "strings_and_font/strings_and_font.h"
#include "ui_movies/game.h"

#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_ui_movies_game);

namespace harness {

namespace {

// The register steps of the functions whose adapters went once their
// callers were native (docs/loops/b7-loop.md, "Removing an adapter"): the
// cases run the plain C++ through these, against the recompiled versions.

// sub_8211A660: r4 = a node, r5 = a number (r3 left).
void NativeSetLayer(PPCContext& ctx, uint8_t* base) {
  ui_movies::SetLayer(base, ctx.r4.u32, ctx.r5.u32);
}

// sub_8211B230: r3 = a list (left), r4 = a position (16-bit).
void NativeLinkFree(PPCContext& ctx, uint8_t* base) {
  ui_movies::LinkFree(base, ctx.r3.u32, int16_t(ctx.r4.u16));
}

// AnimationList::Get: r3 = a 40-byte destination, r4 = a list, r5 = a
// position; returns r3. Where the slot and the destination share memory,
// the original runs (the removed adapter used the game's memcpy there).
void NativeGet(PPCContext& ctx, uint8_t* base) {
  const uint32_t to = ctx.r3.u32, list = ctx.r4.u32;
  const uint32_t from = ui_movies::SlotOf(base, list, ui_movies::LinkAt(base, list, ctx.r5.u32));
  if (!c_runtime::Memcpy(base, to, from, 40)) {
    __imp__sub_8211B298(ctx, base);
    return;
  }
  ctx.r3.u64 = to;
}

// sub_8211B7B8 and sub_8211B9B8: r3 = movie, r4 = a text id; the text's
// +24 or +4.
void NativeTextField24(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ui_movies::TextField(base, ctx.r3.u32, ctx.r4.u32, 24);
}
void NativeTextField4(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ui_movies::TextField(base, ctx.r3.u32, ctx.r4.u32, 4);
}

// sub_8211B940: r3 = movie, r4 = a text id; r3 left at the movie's texts.
void NativeAdvanceTextScroll(PPCContext& ctx, uint8_t* base) {
  const uint32_t movie = ctx.r3.u32;
  ui_movies::AdvanceTextScroll(base, movie, ctx.r4.u32);
  ctx.r3.u64 = movie + ui_movies::kMovieTexts;
}

// Runs the recompiled version with --native_ui_movies_game off, so the
// functions it calls that are ours (its recursions among them) run the
// originals too.
void RunRecompiled(GuestFunction recompiled, PPCContext& t, uint8_t* base) {
  const bool native = ITC_FLAG(native_ui_movies_game);
  ITC_FLAG(native_ui_movies_game) = false;
  recompiled(t, base);
  ITC_FLAG(native_ui_movies_game) = native;
}

constexpr uint32_t kTagGrp = 0x47525000;  // "GRP\0"
constexpr uint32_t kTagPic = 0x50494300;  // "PIC\0"
constexpr uint32_t kTagPrm = 0x50524D00;  // "PRM\0"
constexpr uint32_t kTagSce = 0x53434500;  // "SCE\0"
constexpr uint32_t kTagTfm = 0x54464D00;  // "TFM\0"
constexpr uint32_t kTagTrm = 0x54524D00;  // "TRM\0"
constexpr uint32_t kTagTxt = 0x54585400;  // "TXT\0"

// The odd float words mixed into fields: NaNs of both kinds and signs,
// infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001, 0x7F800000,
                                   0xFF800000, 0x00000001, 0x807FFFFF, 0x00000000, 0x80000000,
                                   0x7F7FFFFF, 0x3F800000};

uint32_t RandomWord(Random& r) {
  return r.Below(4) == 0 ? kOddFloats[r.Below(std::size(kOddFloats))] : uint32_t(r.Next());
}

// The block the cases work in.
struct Block {
  uint32_t at = 0;
  uint32_t bytes = 0;
};

// One case through both versions: the block's first `bytes` filled with
// 0xCD, `setup()` writes the inputs, `args(t)` sets the registers, the
// function runs, then `after()` (putting back live globals the setup
// changed); the bytes, r3 and (with `f1`) f1 are compared, from the first
// difference on (32 bytes of it logged).
template <typename Setup, typename Args>
void Compare(Context& h, Results& results, uint32_t area, uint32_t bytes, Setup setup, Args args,
             GuestFunction recompiled, GuestFunction native, const std::string& what,
             bool f1 = false, const std::function<void()>& after = {}) {
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, area, bytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    if (v == 0) {
      RunRecompiled(recompiled, t, h.base);
    } else {
      native(t, h.base);
    }
    result[v] = Read(h.base, area, bytes);
    if (after) {
      after();
    }
    for (int i = 0; i < 4; i++) {
      result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
    }
    if (f1) {
      const uint64_t bits = t.f1.u64;
      for (int i = 0; i < 8; i++) {
        result[v].push_back(uint8_t(bits >> (56 - 8 * i)));
      }
    }
  }
  size_t at = 0;
  while (at < result[0].size() && result[0][at] == result[1][at]) {
    at++;
  }
  const size_t from = at < result[0].size() ? at : 0;
  const size_t to = std::min(result[0].size(), from + 32);
  results.Check({result[0].begin() + from, result[0].begin() + to},
                {result[1].begin() + from, result[1].begin() + to},
                fmt::format("{}, from byte {}", what, from));
}

// A made-up movie with three copies of one tree (the live, saved and file
// scenes): each scene has six entries (0 a sub-scene, 1 to 5 leaves), the
// sub-scene six entries (leaves), and the leaves random fields with small
// ranges (+100 first, +104 count: children of the scene they're walked in),
// texture entries (+108) and vertex counts (+112, +116), so the pose copies
// stay inside the block and end. Tags: PIC, PRM, TXT, TFM or a random word.
struct Movie {
  static constexpr uint32_t kCopyBytes = 1536;
  static constexpr uint32_t kLeaves = 4;
  static constexpr uint32_t kLeafBytes = 200;
  uint32_t movie = 0;
  uint32_t textures = 0;
  uint32_t scene[3] = {};
  uint32_t sub[3] = {};
  uint32_t leaf[3][kLeaves] = {};

  Movie(uint32_t at) {
    movie = at;
    textures = at + 64;
    for (uint32_t c = 0; c < 3; c++) {
      const uint32_t copy = at + 256 + c * kCopyBytes;
      scene[c] = copy;
      sub[c] = copy + 192;
      for (uint32_t k = 0; k < kLeaves; k++) {
        leaf[c][k] = copy + 384 + kLeafBytes * k;
      }
    }
  }
};

struct MovieSpec {
  uint32_t scene_base[3], sub_base[3];
  uint32_t first, count, sub_first, sub_count;
  uint32_t scene_leaf[6], sub_leaf[6];  // which leaf each entry names
  uint32_t tags[Movie::kLeaves];
  uint32_t fields[3][Movie::kLeaves][50];
  uint32_t ranges[Movie::kLeaves][2];
  uint32_t texture_entry[Movie::kLeaves], textures_count[Movie::kLeaves],
      vertices[Movie::kLeaves];
  uint32_t texture_index[3][Movie::kLeaves][3];
  uint32_t texture_words[16];

  explicit MovieSpec(Random& r) {
    for (uint32_t c = 0; c < 3; c++) {
      scene_base[c] = r.Below(3);
      sub_base[c] = r.Below(3);
    }
    first = r.Below(3);
    count = r.Below(7 - first);
    sub_first = r.Below(3);
    sub_count = r.Below(7 - sub_first);
    for (uint32_t k = 0; k < 6; k++) {
      scene_leaf[k] = r.Below(Movie::kLeaves);
      sub_leaf[k] = r.Below(Movie::kLeaves);
    }
    constexpr uint32_t kLeafTags[] = {kTagPic, kTagPrm, kTagTxt, kTagTfm};
    for (uint32_t k = 0; k < Movie::kLeaves; k++) {
      tags[k] = r.Below(8) == 0 ? uint32_t(r.Next()) : kLeafTags[r.Below(4)];
      // A leaf's children: entries 1 to 5 of the scene it's walked in (not
      // the sub-scene, so the walk ends).
      ranges[k][0] = 1 + r.Below(3);
      ranges[k][1] = r.Below(3);
      texture_entry[k] = r.Below(3);
      textures_count[k] = r.Below(3);
      vertices[k] = r.Below(4);
      for (uint32_t c = 0; c < 3; c++) {
        for (uint32_t w = 0; w < 50; w++) {
          fields[c][k][w] = RandomWord(r);
        }
        for (uint32_t t = 0; t < 3; t++) {
          texture_index[c][k][t] = r.Below(16);
        }
      }
    }
    for (uint32_t& w : texture_words) {
      w = uint32_t(r.Next());
    }
  }

  void Write(uint8_t* base, const Movie& m) const {
    guest::StoreU32(base, m.movie + 16, m.scene[0]);
    guest::StoreU32(base, m.movie + 20, m.scene[1]);
    guest::StoreU32(base, m.movie + 24, m.scene[2]);
    guest::StoreU32(base, m.movie + 28, m.textures);
    for (uint32_t i = 0; i < 16; i++) {
      guest::StoreU32(base, m.textures + 4 * i, texture_words[i]);
    }
    for (uint32_t c = 0; c < 3; c++) {
      guest::StoreU32(base, m.scene[c], kTagSce);
      guest::StoreU32(base, m.scene[c] + 100, first);
      guest::StoreU32(base, m.scene[c] + 104, count);
      guest::StoreU32(base, m.scene[c] + 108, scene_base[c]);
      guest::StoreU32(base, m.sub[c], kTagSce);
      guest::StoreU32(base, m.sub[c] + 100, sub_first);
      guest::StoreU32(base, m.sub[c] + 104, sub_count);
      guest::StoreU32(base, m.sub[c] + 108, sub_base[c]);
      for (uint32_t k = 0; k < 6; k++) {
        guest::StoreU32(base, m.scene[c] + 128 + 8 * (scene_base[c] + k),
                        k == 0 ? m.sub[c] : m.leaf[c][scene_leaf[k]]);
        guest::StoreU32(base, m.sub[c] + 128 + 8 * (sub_base[c] + k), m.leaf[c][sub_leaf[k]]);
      }
      for (uint32_t k = 0; k < Movie::kLeaves; k++) {
        const uint32_t leaf = m.leaf[c][k];
        for (uint32_t w = 0; w < 50; w++) {
          guest::StoreU32(base, leaf + 4 * w, fields[c][k][w]);
        }
        guest::StoreU32(base, leaf, tags[k]);
        guest::StoreU32(base, leaf + 100, ranges[k][0]);
        guest::StoreU32(base, leaf + 104, ranges[k][1]);
        guest::StoreU32(base, leaf + 108, texture_entry[k]);
        guest::StoreU32(base, leaf + 112, textures_count[k]);
        guest::StoreU32(base, leaf + 116, vertices[k]);
        for (uint32_t t = 0; t < 3; t++) {
          // A picture's texture entries (16 bytes from +144): their word +0
          // indexes the movie's textures.
          guest::StoreU32(base, leaf + 144 + 16 * t, texture_index[c][k][t]);
        }
      }
    }
  }
};

// A lookup's indices: -1 or the sub-scene (0), and an entry (0 to 5).
std::pair<uint32_t, uint32_t> Lookup(Random& r) {
  return {r.Below(2) == 0 ? 0xFFFFFFFFu : 0u, r.Below(6)};
}

void NodeCases(Context& h, const Block& block) {
  struct Setter {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Setter setters[] = {
      {"UiMovie::FindNode (sub_8211A578)", __imp__sub_8211A578, sub_8211A578},
      {"UiMovie::SetNodeColor (sub_8211A5B8)", __imp__sub_8211A5B8, sub_8211A5B8},
      {"UiMovie::SetNodeTexture (sub_8211A708)", __imp__sub_8211A708, sub_8211A708},
      {"UiMovie::ShowNode (sub_8211AC00)", __imp__sub_8211AC00, sub_8211AC00},
      {"UiMovie::HideNode (sub_8211AC48)", __imp__sub_8211AC48, sub_8211AC48},
      {"UiMovie::SetPictureTexture (sub_8211AAA8)", __imp__sub_8211AAA8, sub_8211AAA8},
      {"UiMovie::GetPictureTexture (sub_8211AB28)", __imp__sub_8211AB28, sub_8211AB28},
      {"UiMovie::ResetNode (sub_8211A1E0)", __imp__sub_8211A1E0, sub_8211A1E0},
  };
  const Movie m(block.at);
  for (const Setter& s : setters) {
    Results results(s.name);
    for (int i = 0; i < 300; i++) {
      const MovieSpec spec(h.random);
      auto [sub, child] = Lookup(h.random);
      // The picture's texture: a leaf (entry 0 is the sub-scene, whose +108
      // isn't a texture entry).
      if (s.native == sub_8211AAA8 || s.native == sub_8211AB28) {
        child = 1 + h.random.Below(5);
      }
      const uint32_t r6 = s.native == sub_8211A5B8 || h.random.Below(3) == 0
                              ? h.random.Below(40)
                              : uint32_t(h.random.Next());
      Compare(h, results, block.at, 256 + 3 * Movie::kCopyBytes,
              [&] { spec.Write(h.base, m); },
              [&](PPCContext& t) {
                t.r3.u64 = m.movie;
                t.r4.u64 = sub;
                t.r5.u64 = child;
                t.r6.u64 = r6;
              },
              s.recompiled, s.native, fmt::format("case {}: r4 {:X} r5 {}", i, sub, child));
    }
    results.Report();
  }
}

void ResetPoseCases(Context& h, const Block& block) {
  Results results("UiMovie::ResetPose (sub_8211ABA8)");
  const Movie m(block.at);
  for (int i = 0; i < 300; i++) {
    const MovieSpec spec(h.random);
    Compare(h, results, block.at, 256 + 3 * Movie::kCopyBytes, [&] { spec.Write(h.base, m); },
            [&](PPCContext& t) { t.r3.u64 = m.movie; }, __imp__sub_8211ABA8, sub_8211ABA8,
            fmt::format("case {}: range {} {}", i, spec.first, spec.count));
  }
  results.Report();
}

// sub_8211A630 (a node's color) and sub_8211A660 (its layer): a node, the
// color or number random and at the edges.
void LeafSetterCases(Context& h, const Block& block) {
  {
    Results results("sub_8211A630 (a node's color)");
    for (int i = 0; i < 300; i++) {
      const uint32_t color = i < 40 ? uint32_t(i) : uint32_t(h.random.Next()) % 200;
      const uint32_t r3 = uint32_t(h.random.Next());
      const uint32_t node = block.at + 4 * h.random.Below(32);
      Compare(h, results, block.at, 256, [] {},
              [&](PPCContext& t) {
                t.r3.u64 = r3;
                t.r4.u64 = node;
                t.r5.u64 = color;
              },
              __imp__sub_8211A630, sub_8211A630, fmt::format("color {}", color));
    }
    results.Report();
  }
  {
    Results results("sub_8211A660 (a node's layer)");
    for (int i = 0; i < 400; i++) {
      const uint32_t n = i < 80 ? uint32_t(i) : uint32_t(h.random.Next());
      const uint32_t r3 = uint32_t(h.random.Next());
      const uint32_t node = block.at + 4 * h.random.Below(32);
      Compare(h, results, block.at, 256, [] {},
              [&](PPCContext& t) {
                t.r3.u64 = r3;
                t.r4.u64 = node;
                t.r5.u64 = n;
              },
              __imp__sub_8211A660, NativeSetLayer, fmt::format("n {:X}", n));
    }
    results.Report();
  }
}

// sub_8211A6C0: chains of chunks from the live scene (offsets 112 to 200,
// so a cleared text id never lands on a later chunk's header), tagged TXT,
// SCE, GRP or random, ending with TRM.
void ClearTextIdsCases(Context& h, const Block& block) {
  Results results("sub_8211A6C0 (the texts' ids cleared)");
  constexpr uint32_t kTags[] = {kTagTxt, kTagSce, kTagGrp, kTagTxt};
  for (int i = 0; i < 300; i++) {
    std::vector<std::pair<uint32_t, uint32_t>> chain;
    uint32_t at = 0;
    const uint32_t count = h.random.Below(40);
    for (uint32_t k = 0; k < count; k++) {
      const uint32_t tag = h.random.Below(6) == 0 ? uint32_t(h.random.Next()) : kTags[h.random.Below(4)];
      chain.push_back({at, k == 0 && h.random.Below(2) ? kTagTxt : tag});
      at += 112 + 4 * h.random.Below(23);
    }
    chain.push_back({at, kTagTrm});
    std::vector<uint32_t> words(at / 4 + 32);
    for (uint32_t& w : words) {
      w = uint32_t(h.random.Next());
    }
    const uint32_t chunks = block.at + 64;
    Compare(h, results, block.at, 64 + uint32_t(words.size()) * 4,
            [&] {
              for (uint32_t k = 0; k < words.size(); k++) {
                guest::StoreU32(h.base, chunks + 4 * k, words[k]);
              }
              for (size_t k = 0; k < chain.size(); k++) {
                guest::StoreU32(h.base, chunks + chain[k].first, chain[k].second);
                if (k + 1 < chain.size()) {
                  guest::StoreU32(h.base, chunks + chain[k].first + 4,
                                  chain[k + 1].first - chain[k].first);
                }
              }
              guest::StoreU32(h.base, block.at + 16, chunks);
            },
            [&](PPCContext& t) { t.r3.u64 = block.at; }, __imp__sub_8211A6C0, sub_8211A6C0,
            fmt::format("case {}: {} chunks", i, chain.size()));
  }
  results.Report();
}

// UiMovie::SwapTexture: texture i of 64 (and i out of them, inside the
// block), a random texture.
void SwapTextureCases(Context& h, const Block& block) {
  Results results("UiMovie::SwapTexture (sub_8211AB90)");
  for (int i = 0; i < 300; i++) {
    const uint32_t index = h.random.Below(96);
    const uint32_t texture = uint32_t(h.random.Next());
    std::vector<uint32_t> words(96);
    for (uint32_t& w : words) {
      w = uint32_t(h.random.Next());
    }
    Compare(h, results, block.at, 512,
            [&] {
              guest::StoreU32(h.base, block.at + 28, block.at + 64);
              for (uint32_t k = 0; k < 96; k++) {
                guest::StoreU32(h.base, block.at + 64 + 4 * k, words[k]);
              }
            },
            [&](PPCContext& t) {
              t.r3.u64 = block.at;
              t.r4.u64 = index;
              t.r5.u64 = texture;
            },
            __imp__sub_8211AB90, sub_8211AB90, fmt::format("i {}", index));
  }
  results.Report();
}

// The quads' and text items' set-up and the copy: items at random places in
// 1 KB of random words (odd floats among them); the copy's two items apart,
// or overlapping (where the original runs).
void ItemCases(Context& h, const Block& block) {
  struct Maker {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Maker makers[] = {
      {"Quad::Quad (sub_8211A948)", __imp__sub_8211A948, sub_8211A948},
      {"TextItem::TextItem (sub_8211A818)", __imp__sub_8211A818, sub_8211A818},
      {"TextItem::~TextItem (sub_8211A8C8)", __imp__sub_8211A8C8, sub_8211A8C8},
      {"sub_8211AED0 (a text item copied)", __imp__sub_8211AED0, sub_8211AED0},
  };
  for (const Maker& f : makers) {
    Results results(f.name);
    for (int i = 0; i < 300; i++) {
      std::vector<uint32_t> words(256);
      for (uint32_t& w : words) {
        w = RandomWord(h.random);
      }
      const uint32_t to = block.at + 4 * h.random.Below(96);
      const uint32_t from = i % 4 == 3 ? to + 4 * h.random.Below(40) - 80
                                       : block.at + 512 + 4 * h.random.Below(96);
      Compare(h, results, block.at, 1024,
              [&] {
                for (uint32_t k = 0; k < 256; k++) {
                  guest::StoreU32(h.base, block.at + 4 * k, words[k]);
                }
              },
              [&](PPCContext& t) {
                t.r3.u64 = to;
                t.r4.u64 = from;
              },
              f.recompiled, f.native,
              fmt::format("case {}: at {:X} from {:X}", i, to - block.at, from - block.at));
    }
    results.Report();
  }
}

// An animation list in a valid state: `used` links in the list's chain and
// the rest in the free chain (in a random order, -1 ending each), each
// link's slot a permutation of 0 to 31, the slots' words random with the
// matches the time lookups look for planted at +0, +4 and +8.
struct ListSpec {
  uint32_t used;
  uint16_t order[32];
  uint16_t slot[32];
  uint32_t slots[32][10];
  uint32_t other[4];

  ListSpec(Random& r, uint32_t used_count) : used(used_count) {
    for (uint16_t k = 0; k < 32; k++) {
      order[k] = k;
      slot[k] = k;
    }
    for (uint32_t k = 31; k > 0; k--) {
      std::swap(order[k], order[r.Below(k + 1)]);
      std::swap(slot[k], slot[r.Below(k + 1)]);
    }
    for (auto& words : slots) {
      for (uint32_t& w : words) {
        w = RandomWord(r);
      }
      // Small indices, so the lookups' keys match some.
      words[0] = r.Below(4);
      words[1] = r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(3);
      words[2] = r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(6);
    }
    for (uint32_t& w : other) {
      w = uint32_t(r.Next());
    }
  }

  void Write(uint8_t* base, uint32_t list) const {
    auto chain = [&](uint32_t from, uint32_t to, uint32_t head) {
      guest::StoreU16(base, list + head, from < to ? order[from] : 0xFFFF);
      for (uint32_t k = from; k < to; k++) {
        guest::StoreU16(base, list + 4 * order[k], k + 1 < to ? order[k + 1] : 0xFFFF);
      }
    };
    for (uint32_t k = 0; k < 32; k++) {
      guest::StoreU16(base, list + 4 * k + 2, slot[k]);
      for (uint32_t w = 0; w < 10; w++) {
        guest::StoreU32(base, list + 140 + 40 * k + 4 * w, slots[k][w]);
      }
    }
    chain(0, used, 132);
    chain(used, 32, 128);
    guest::StoreU16(base, list + 130, 0xFFFF);
    guest::StoreU16(base, list + 134, 0xFFFF);
    guest::StoreU16(base, list + 136, uint16_t(used));
  }
};

constexpr uint32_t kListBytes = 140 + 40 * 32;

void AnimationListCases(Context& h, const Block& block) {
  const uint32_t list = block.at + 64;
  const uint32_t outside = list + kListBytes + 64;  // a slot to insert, or the copy's destination
  {
    Results results("sub_8211A9E8 (the animation lists set up)");
    for (int i = 0; i < 40; i++) {
      const uint32_t movie = block.at + 16 * h.random.Below(16);
      Compare(h, results, block.at, 0xA100, [] {}, [&](PPCContext& t) { t.r3.u64 = movie; },
              __imp__sub_8211A9E8, sub_8211A9E8, fmt::format("movie +{:X}", movie - block.at));
    }
    results.Report();
  }
  struct Op {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Op ops[] = {
      {"sub_8211B230 (a free link linked in)", __imp__sub_8211B230, NativeLinkFree},
      {"AnimationList::Insert (sub_8211B4D0)", __imp__sub_8211B4D0, sub_8211B4D0},
      {"AnimationList::RemoveAt (sub_8211B080)", __imp__sub_8211B080, sub_8211B080},
      {"AnimationList::Get (sub_8211B298)", __imp__sub_8211B298, NativeGet},
  };
  for (const Op& op : ops) {
    Results results(op.name);
    for (int i = 0; i < 400; i++) {
      const bool links = op.native == NativeLinkFree || op.native == sub_8211B4D0;
      // Linking needs a free link; removing and getting a used one.
      const uint32_t used = links ? h.random.Below(32) : 1 + h.random.Below(32);
      const ListSpec spec(h.random, i == 0 ? (links ? 0 : 32) : used);
      const uint32_t position = links ? h.random.Below(spec.used + 1) : h.random.Below(spec.used);
      // The slot copied: apart, or (one case in five) inside the list.
      const uint32_t other =
          i % 5 == 4 ? list + 140 + 4 * h.random.Below(320) : outside + 4 * h.random.Below(16);
      std::vector<uint32_t> words(16);
      for (uint32_t& w : words) {
        w = RandomWord(h.random);
      }
      Compare(h, results, block.at, 64 + kListBytes + 256,
              [&] {
                spec.Write(h.base, list);
                for (uint32_t k = 0; k < 16; k++) {
                  guest::StoreU32(h.base, outside + 4 * k, words[k]);
                }
              },
              [&](PPCContext& t) {
                if (op.native == NativeGet) {
                  t.r3.u64 = other;
                  t.r4.u64 = list;
                  t.r5.u64 = position;
                } else {
                  t.r3.u64 = list;
                  t.r4.u64 = op.native == sub_8211B4D0 ? other : position;
                  t.r5.u64 = position;
                }
              },
              op.recompiled, op.native,
              fmt::format("case {}: {} used, position {}, other +{:X}", i, spec.used, position,
                          other - block.at));
    }
    results.Report();
  }
  // The time lookups: both lists of a movie, keys matching some slots or
  // none.
  struct Lookup {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Lookup lookups[] = {
      {"UiMovie::AnimationTime (sub_8211BA00)", __imp__sub_8211BA00, sub_8211BA00},
      {"UiMovie::NamedAnimationTime (sub_8211BB18)", __imp__sub_8211BB18, sub_8211BB18},
  };
  const uint32_t movie = block.at;
  for (const Lookup& f : lookups) {
    Results results(f.name);
    for (int i = 0; i < 400; i++) {
      const ListSpec queue(h.random, h.random.Below(33));
      const ListSpec looping(h.random, h.random.Below(33));
      const uint32_t r4 = h.random.Below(5) == 0 ? 0xFFFFFFFFu : h.random.Below(7);
      const uint32_t r5 = h.random.Below(5);
      Compare(h, results, movie + 0x90B4, 0x9640 - 0x90B4 + kListBytes,
              [&] {
                queue.Write(h.base, movie + 0x90B4);
                looping.Write(h.base, movie + 0x9640);
              },
              [&](PPCContext& t) {
                t.r3.u64 = movie;
                t.r4.u64 = r4;
                t.r5.u64 = r5;
              },
              f.recompiled, f.native,
              fmt::format("case {}: {} and {} used, r4 {:X} r5 {}", i, queue.used, looping.used,
                          r4, r5),
              true);
    }
    results.Report();
  }
}

// The pairs' lookups and TextMap::Set: maps of random pairs with keys from a
// small set (so lookups match some), counts from 0 to 128 (Set's append at
// 128 lands on the count itself) and a few huge ones for the lookups.
void PairsCases(Context& h, const Block& block) {
  struct Lookup {
    const char* name;
    GuestFunction recompiled, native;
    bool in_place;  // the pairs at +0 and the count at +1024, else +0 a pointer and +8 the count
  };
  const Lookup lookups[] = {
      {"TextMap::Find (sub_8211B310)", __imp__sub_8211B310, sub_8211B310, true},
      {"TextMap::Set (sub_8211B370)", __imp__sub_8211B370, sub_8211B370, true},
      {"UiMovies::Find (sub_8211B3F0)", __imp__sub_8211B3F0, sub_8211B3F0, false},
  };
  const uint32_t map = block.at + 64;
  const uint32_t pairs = block.at + 2048;
  const uint32_t key_address = block.at + 16, value_address = block.at + 20;
  for (const Lookup& f : lookups) {
    Results results(f.name);
    for (int i = 0; i < 500; i++) {
      const bool set = f.native == sub_8211B370;
      uint32_t count = i < 3 ? uint32_t(i) : h.random.Below(129);
      if (!set && i % 50 == 49) {
        count = 129 + h.random.Below(64);  // past the 128 pairs: they read on
      }
      std::vector<uint32_t> words(2 * 200);
      for (uint32_t& w : words) {
        w = h.random.Below(3) == 0 ? uint32_t(h.random.Next()) : h.random.Below(200);
      }
      const uint32_t key = h.random.Below(4) == 0 ? 0xFFFFFFFFu : h.random.Below(200);
      const uint32_t value = uint32_t(h.random.Next());
      Compare(h, results, block.at, 4096,
              [&] {
                const uint32_t at = f.in_place ? map : pairs;
                for (uint32_t k = 0; k < words.size(); k++) {
                  guest::StoreU32(h.base, at + 4 * k, words[k]);
                }
                if (f.in_place) {
                  guest::StoreU32(h.base, map + 1024, count);
                } else {
                  guest::StoreU32(h.base, map, pairs);
                  guest::StoreU32(h.base, map + 4, 256);
                  guest::StoreU32(h.base, map + 8, count);
                }
                guest::StoreU32(h.base, key_address, key);
                guest::StoreU32(h.base, value_address, value);
              },
              [&](PPCContext& t) {
                t.r3.u64 = map;
                t.r4.u64 = key_address;
                t.r5.u64 = value_address;
              },
              f.recompiled, f.native, fmt::format("case {}: count {} key {:X}", i, count, key));
    }
    results.Report();
  }
}

// sub_8211AD00 on the live renderer object (its getters through the
// vtables) with a made-up movie table: the table's header, the screen's
// globals and the library's renderer globals are kept and put back after
// each run.
void ScreenStartCases(Context& h, const Block& block) {
  Results results("sub_8211AD00 (a screen's movie table reset)");
  constexpr uint32_t kGlobals = 0x826C5CA0, kGlobalBytes = 0x40;  // to the table's header and past it
  constexpr uint32_t kRendererGlobals = 0x82970C18;
  const std::vector<uint8_t> globals = Read(h.base, kGlobals, kGlobalBytes);
  const std::vector<uint8_t> renderer = Read(h.base, kRendererGlobals, 8);
  const uint32_t entries = block.at + 256;
  for (int i = 0; i < 100; i++) {
    const uint32_t count = i < 2 ? uint32_t(i) : h.random.Below(65);
    std::vector<uint32_t> words(2 * 64 + 8);
    for (uint32_t& w : words) {
      w = uint32_t(h.random.Next());
    }
    uint32_t screen[9];
    for (uint32_t& w : screen) {
      w = uint32_t(h.random.Next()) | 1;
    }
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, block.at, 1024);
      for (uint32_t k = 0; k < words.size(); k++) {
        guest::StoreU32(h.base, entries + 4 * k, words[k]);
      }
      for (uint32_t k = 0; k < 9; k++) {
        guest::StoreU32(h.base, kGlobals + 4 + 4 * k, screen[k]);
      }
      guest::StoreU32(h.base, 0x826C5CCC, entries);
      guest::StoreU32(h.base, 0x826C5CD0, 64);
      guest::StoreU32(h.base, 0x826C5CD4, count);
      guest::StoreU32(h.base, kRendererGlobals, 0);
      guest::StoreU32(h.base, kRendererGlobals + 4, 0);
      PPCContext t = CallContext(h);
      if (v == 0) {
        RunRecompiled(__imp__sub_8211AD00, t, h.base);
      } else {
        sub_8211AD00(t, h.base);
      }
      result[v] = Read(h.base, block.at, 1024);
      std::vector<uint8_t> g = Read(h.base, kGlobals, kGlobalBytes);
      std::vector<uint8_t> r = Read(h.base, kRendererGlobals, 8);
      result[v].insert(result[v].end(), g.begin(), g.end());
      result[v].insert(result[v].end(), r.begin(), r.end());
      for (int b = 0; b < 4; b++) {
        result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * b)));
      }
      Write(h.base, kGlobals, globals.data(), kGlobalBytes);
      Write(h.base, kRendererGlobals, renderer.data(), 8);
    }
    size_t at = 0;
    while (at < result[0].size() && result[0][at] == result[1][at]) {
      at++;
    }
    const size_t from = at < result[0].size() ? at : 0;
    const size_t to = std::min(result[0].size(), from + 32);
    results.Check({result[0].begin() + from, result[0].begin() + to},
                  {result[1].begin() + from, result[1].begin() + to},
                  fmt::format("case {}: count {}, from byte {}", i, count, from));
  }
  results.Report();
}

// sub_8211BD10: a made-up scene (96 entries) whose top range (entries 0 to
// 39, or past them) holds leaves (PIC, PRM, TXT, TFM or a random tag),
// groups (their ranges in entries 40 to 95, leaves only) and two sub-scenes
// (entries of leaves); layer offsets mostly 0 to 80, some negative or huge,
// and top ranges past 64 children (both of which leave it to the original).
// Its frames (16.8 KB each) go on a stack at the block's top.
void LayersCases(Context& h, const Block& block) {
  Results results("sub_8211BD10 (the layers)");
  constexpr uint32_t kNodes = 96, kNodeBytes = 128;
  const uint32_t scene = block.at;
  const uint32_t nodes = block.at + 1024;
  const uint32_t subs = nodes + kNodes * kNodeBytes;
  const uint32_t area = subs + 2 * 1024 - block.at;
  const uint32_t stack = block.at + block.bytes - 512;
  for (int i = 0; i < 300; i++) {
    Random& r = h.random;
    std::vector<uint32_t> tags(kNodes), offsets(kNodes), first(kNodes), count(kNodes);
    // Nodes 0 to 59 leaves, 60 to 89 groups or leaves, the top's entries
    // may also name the sub-scenes (as nodes 96 and 97).
    constexpr uint32_t kLeafTags[] = {kTagPic, kTagPrm, kTagTxt, kTagTfm};
    for (uint32_t k = 0; k < kNodes; k++) {
      tags[k] = k >= 60 && r.Below(2) ? kTagGrp
                                       : (r.Below(8) == 0 ? uint32_t(r.Next()) : kLeafTags[r.Below(4)]);
      const uint32_t kind = r.Below(40);
      offsets[k] = kind == 0 ? 0xFFFFFFFFu - r.Below(5)
                             : (kind == 1 ? uint32_t(r.Next()) & 0x7FFFFFFF : r.Below(81));
      first[k] = 40 + r.Below(40);
      count[k] = r.Below(96 - first[k] + 1);
    }
    std::vector<uint32_t> entries(kNodes), sub_entries[2];
    for (uint32_t k = 0; k < kNodes; k++) {
      // The top region names any node or a sub-scene; the rest only leaves.
      entries[k] = k < 40 ? (r.Below(10) == 0 ? kNodes + r.Below(2) : r.Below(90)) : r.Below(60);
    }
    for (auto& e : sub_entries) {
      e.resize(16);
      for (uint32_t& x : e) {
        x = r.Below(60);
      }
    }
    const uint32_t sub_counts[2] = {r.Below(17), r.Below(17)};
    const uint32_t sub_offsets[2] = {r.Below(81), r.Below(81)};
    const bool top_is_group = r.Below(3) == 0;
    const uint32_t top_first = r.Below(10);
    const uint32_t top_count = r.Below(10) == 0 ? 60 + r.Below(20) : r.Below(31);
    const uint32_t layer = r.Below(60), by_bucket = r.Below(2);
    const uint32_t movie = uint32_t(r.Next());
    auto node = [&](uint32_t k) {
      return k < kNodes ? nodes + kNodeBytes * k : subs + 1024 * (k - kNodes);
    };
    // A group as the top: one of 90 to 95, which no entry names (so it
    // isn't its own child).
    const uint32_t top = top_is_group ? node(90 + r.Below(6)) : scene;
    Compare(h, results, block.at, area,
            [&] {
              guest::StoreU32(h.base, scene, kTagSce);
              guest::StoreU32(h.base, scene + 108, 0);
              for (uint32_t k = 0; k < kNodes; k++) {
                guest::StoreU32(h.base, scene + 128 + 8 * k, node(entries[k]));
                const uint32_t n = node(k);
                guest::StoreU32(h.base, n, tags[k]);
                guest::StoreU32(h.base, n + 72, offsets[k]);
                guest::StoreU32(h.base, n + 100, first[k]);
                guest::StoreU32(h.base, n + 104, count[k]);
              }
              for (uint32_t m = 0; m < 2; m++) {
                const uint32_t sub = node(kNodes + m);
                guest::StoreU32(h.base, sub, kTagSce);
                guest::StoreU32(h.base, sub + 72, sub_offsets[m]);
                guest::StoreU32(h.base, sub + 100, 0);
                guest::StoreU32(h.base, sub + 104, sub_counts[m]);
                guest::StoreU32(h.base, sub + 108, 0);
                for (uint32_t k = 0; k < 16; k++) {
                  guest::StoreU32(h.base, sub + 128 + 8 * k, node(sub_entries[m][k]));
                }
              }
              // The top's range, written last (over a group's own).
              guest::StoreU32(h.base, top + 100, top_first);
              guest::StoreU32(h.base, top + 104, top_count);
            },
            [&](PPCContext& t) {
              t.r1.u64 = stack;
              t.r3.u64 = movie;
              t.r4.u64 = scene;
              t.r5.u64 = top;
              t.r6.u64 = layer;
              t.r7.u64 = by_bucket;
            },
            __imp__sub_8211BD10, sub_8211BD10,
            fmt::format("case {}: top {:X} range {} {} by bucket {}", i, top - block.at, top_first,
                        top_count, by_bucket));
  }
  results.Report();
}

// The texts' accessors and UiMovies::IsLoaded: a movie's text map (counts 0
// to 128, small keys so lookups match some) with records in the block, and
// the movie table (0x826C5CCC) pointed at made-up movies. An id not found
// reads the words at 0x826C5CD8 (a text record) and 0x826C5CDC (a movie),
// pointed into the block too; the table's header and those words are put
// back after each run.
void TextCases(Context& h, const Block& block) {
  constexpr uint32_t kGlobals = 0x826C5CCC, kGlobalBytes = 0x14;  // the table's header, the two
  const std::vector<uint8_t> globals = Read(h.base, kGlobals, kGlobalBytes);
  const auto put_back = [&] { Write(h.base, kGlobals, globals.data(), kGlobalBytes); };
  const uint32_t movie = block.at;
  const uint32_t map = movie + 0x9BCC;
  const uint32_t records = block.at + 0xA000;  // 8 records of 32 bytes
  struct Accessor {
    const char* name;
    GuestFunction recompiled, native;
    bool writes;  // a record of 0 is written through
  };
  const Accessor accessors[] = {
      {"UiMovie::TextString (sub_8211B720)", __imp__sub_8211B720, sub_8211B720, false},
      {"sub_8211B770 (a text's +20)", __imp__sub_8211B770, sub_8211B770, false},
      {"sub_8211B7B8 (a text's +24)", __imp__sub_8211B7B8, NativeTextField24, false},
      {"sub_8211B9B8 (a text's +4)", __imp__sub_8211B9B8, NativeTextField4, false},
      {"sub_8211B940 (a text's scroll counter)", __imp__sub_8211B940, NativeAdvanceTextScroll, true},
  };
  for (const Accessor& f : accessors) {
    Results results(f.name);
    for (int i = 0; i < 400; i++) {
      const uint32_t count = i < 2 ? uint32_t(i) : h.random.Below(129);
      std::vector<uint32_t> pairs(2 * 128);
      for (uint32_t k = 0; k < 128; k++) {
        pairs[2 * k] = h.random.Below(8) == 0 ? uint32_t(h.random.Next()) : h.random.Below(160);
        pairs[2 * k + 1] = !f.writes && h.random.Below(5) == 0 ? 0 : records + 32 * h.random.Below(8);
      }
      std::vector<uint32_t> words(64);
      for (uint32_t& w : words) {
        w = RandomWord(h.random);
      }
      const uint32_t none = !f.writes && h.random.Below(3) == 0 ? 0 : records + 32 * h.random.Below(8);
      const uint32_t id = h.random.Below(6) == 0 ? 0xFFFFFFFFu : h.random.Below(160);
      Compare(h, results, block.at + 0x9B00, 0x600,
              [&] {
                for (uint32_t k = 0; k < pairs.size(); k++) {
                  guest::StoreU32(h.base, map + 4 * k, pairs[k]);
                }
                guest::StoreU32(h.base, map + 1024, count);
                for (uint32_t k = 0; k < words.size(); k++) {
                  guest::StoreU32(h.base, records + 4 * k, words[k]);
                }
                guest::StoreU32(h.base, 0x826C5CD8, none);
              },
              [&](PPCContext& t) {
                t.r3.u64 = movie;
                t.r4.u64 = id;
              },
              f.recompiled, f.native, fmt::format("case {}: count {} id {:X}", i, count, id), false,
              put_back);
    }
    results.Report();
  }
  {
    Results results("UiMovies::IsLoaded (sub_8211BE98)");
    const uint32_t pairs = block.at + 0xA200;
    for (int i = 0; i < 300; i++) {
      const uint32_t count = i < 2 ? uint32_t(i) : h.random.Below(17);
      std::vector<uint32_t> words(2 * 16);
      for (uint32_t k = 0; k < 16; k++) {
        words[2 * k] = h.random.Below(24);
        words[2 * k + 1] = block.at + 16 * h.random.Below(8);
      }
      uint32_t flags[8];
      for (uint32_t& w : flags) {
        w = h.random.Below(2) == 0 ? 0 : RandomWord(h.random);
      }
      const uint32_t none = block.at + 16 * h.random.Below(8);
      const uint32_t id = h.random.Below(24);
      Compare(h, results, block.at + 0x9F00, 0x400,
              [&] {
                for (uint32_t k = 0; k < words.size(); k++) {
                  guest::StoreU32(h.base, pairs + 4 * k, words[k]);
                }
                for (uint32_t k = 0; k < 8; k++) {
                  guest::StoreU32(h.base, block.at + 16 * k + 0x9FD0, flags[k]);
                }
                guest::StoreU32(h.base, 0x826C5CCC, pairs);
                guest::StoreU32(h.base, 0x826C5CD0, 16);
                guest::StoreU32(h.base, 0x826C5CD4, count);
                guest::StoreU32(h.base, 0x826C5CDC, none);
              },
              [&](PPCContext& t) { t.r3.u64 = id; }, __imp__sub_8211BE98, sub_8211BE98,
              fmt::format("case {}: count {} id {}", i, count, id), false, put_back);
    }
    results.Report();
  }
}

// The moves between a movie's two lists (UiMovie::MoveAnimation and the
// rest): random lists (0 to 32 used: moving into a full one follows the
// original's -1 links) with small keys, so some slots match, and a random
// last started slot for LoopLast.
void MoveCases(Context& h, const Block& block) {
  struct Move {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Move moves[] = {
      {"UiMovie::MoveAnimation (sub_8211DBE0)", __imp__sub_8211DBE0, sub_8211DBE0},
      {"UiMovie::MoveNamedAnimation (sub_8211DCE8)", __imp__sub_8211DCE8, sub_8211DCE8},
      {"UiMovie::Loop (sub_8211DDD8)", __imp__sub_8211DDD8, sub_8211DDD8},
      {"UiMovie::LoopNamed (sub_8211DDE0)", __imp__sub_8211DDE0, sub_8211DDE0},
      {"UiMovie::LoopLast (sub_8211DDE8)", __imp__sub_8211DDE8, sub_8211DDE8},
  };
  const uint32_t movie = block.at;
  for (const Move& f : moves) {
    Results results(f.name);
    for (int i = 0; i < 400; i++) {
      const ListSpec queue(h.random, i == 0 ? 0 : h.random.Below(33));
      const ListSpec looping(h.random, i == 1 ? 0 : h.random.Below(33));
      const uint32_t r4 = h.random.Below(5) == 0 ? 0xFFFFFFFFu : h.random.Below(6);
      const uint32_t r5 = h.random.Below(4);
      const uint32_t r6 = h.random.Below(3) == 0 ? uint32_t(h.random.Next()) : h.random.Below(2);
      uint32_t last[10];
      for (uint32_t& w : last) {
        w = RandomWord(h.random);
      }
      last[0] = h.random.Below(4);
      last[1] = h.random.Below(3) == 0 ? 0xFFFFFFFFu : h.random.Below(3);
      last[2] = h.random.Below(2) == 0 ? 0xFFFFFFFFu : h.random.Below(6);
      Compare(h, results, movie + 0x9000, 0xA000 - 0x9000,
              [&] {
                queue.Write(h.base, movie + 0x90B4);
                looping.Write(h.base, movie + 0x9640);
                for (uint32_t k = 0; k < 10; k++) {
                  guest::StoreU32(h.base, movie + 0x9FD8 + 4 * k, last[k]);
                }
              },
              [&](PPCContext& t) {
                t.r3.u64 = movie;
                t.r4.u64 = r4;
                t.r5.u64 = r5;
                t.r6.u64 = r6;
              },
              f.recompiled, f.native,
              fmt::format("case {}: {} and {} used, r4 {:X} r5 {} r6 {:X}", i, queue.used,
                          looping.used, r4, r5, r6));
    }
    results.Report();
  }
}

// The starts (UiMovie::Play and PlayNamed): a live scene whose children 0
// to 2 are sub-scenes and whose animations (and theirs) are records with
// odd times (+12), named animations' entries pointing at them, random lists
// (the looping one's keys small, so some starts find theirs there).
void PlayCases(Context& h, const Block& block) {
  struct Start {
    const char* name;
    GuestFunction recompiled, native;
    bool named;
  };
  const Start starts[] = {
      {"UiMovie::Play (sub_8211D978)", __imp__sub_8211D978, sub_8211D978, false},
      {"UiMovie::PlayNamed (sub_8211DAC8)", __imp__sub_8211DAC8, sub_8211DAC8, true},
  };
  const uint32_t movie = block.at;
  const uint32_t scene = block.at + 0xA100;
  const uint32_t sub = block.at + 0xA200;        // three of 0x100 bytes
  const uint32_t animations = block.at + 0xA500;  // eight of 32 bytes
  for (const Start& f : starts) {
    Results results(f.name);
    for (int i = 0; i < 400; i++) {
      const ListSpec queue(h.random, h.random.Below(33));
      const ListSpec looping(h.random, h.random.Below(33));
      uint32_t times[8], named_words[8], links[3 * 8 + 8];
      for (uint32_t& w : times) {
        // Signaling NaNs often: the time's word is copied through a float.
        w = h.random.Below(6) == 0 ? (h.random.Below(2) ? 0x7F800001u : 0xFFA00001u)
                                   : RandomWord(h.random);
      }
      for (uint32_t& w : named_words) {
        w = uint32_t(h.random.Next());
      }
      for (uint32_t& w : links) {
        w = animations + 32 * h.random.Below(8);
      }
      uint32_t r4 = h.random.Below(5) == 0 ? 0xFFFFFFFFu : h.random.Below(f.named ? 6 : 3);
      if (f.named && h.random.Below(10) == 0) {
        r4 = 0x80000000u | uint32_t(h.random.Next());
      }
      const uint32_t r5 = h.random.Below(4);
      uint32_t flags[4];
      for (uint32_t& w : flags) {
        w = h.random.Below(2) == 0 ? 0 : uint32_t(h.random.Next());
      }
      Compare(h, results, block.at, 0xA600,
              [&] {
                queue.Write(h.base, movie + 0x90B4);
                looping.Write(h.base, movie + 0x9640);
                guest::StoreU32(h.base, movie + 16, scene);
                guest::StoreU32(h.base, scene + 108, 0);
                guest::StoreU32(h.base, scene + 116, 8);
                for (uint32_t k = 0; k < 3; k++) {
                  guest::StoreU32(h.base, scene + 128 + 8 * k, sub + 0x100 * k);
                  guest::StoreU32(h.base, sub + 0x100 * k + 116, 0);
                  for (uint32_t j = 0; j < 8; j++) {
                    guest::StoreU32(h.base, sub + 0x100 * k + 128 + 8 * j, links[8 * k + j]);
                  }
                }
                for (uint32_t j = 0; j < 8; j++) {
                  guest::StoreU32(h.base, scene + 128 + 8 * (8 + j), links[24 + j]);
                  guest::StoreU32(h.base, animations + 32 * j + 12, times[j]);
                  guest::StoreU32(h.base, movie + 176 + 36 * j + 28, named_words[j]);
                  guest::StoreU32(h.base, movie + 176 + 36 * j + 32, animations + 32 * j);
                }
              },
              [&](PPCContext& t) {
                t.r3.u64 = movie;
                t.r4.u64 = r4;
                const uint32_t* args = flags;
                if (f.named) {
                  t.r5.u64 = args[0];
                  t.r6.u64 = args[1];
                  t.r7.u64 = args[2];
                  t.r8.u64 = args[3];
                } else {
                  t.r5.u64 = r5;
                  t.r6.u64 = args[0];
                  t.r7.u64 = args[1];
                  t.r8.u64 = args[2];
                  t.r9.u64 = args[3];
                }
              },
              f.recompiled, f.native,
              fmt::format("case {}: {} and {} used, r4 {:X} r5 {}", i, queue.used, looping.used,
                          r4, r5));
    }
    results.Report();
  }
}

// --- The texts drawn: UiMovie::DrawTexts and UiMovie::ScrollOffset ---

// sub_8211B800 (UiMovie::ScrollOffset, no adapter: only DrawTexts' plain
// C++ calls it): r3 = movie, r4 = the draw parameters; f1 the x, r3 left
// at the text's value in the movie's map (TextMap::Find's).
void NativeScrollOffset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t movie = ctx.r3.u32, params = ctx.r4.u32;
  ctx.f1.f64 = ui_movies::ScrollOffset(base, movie, params);
  ctx.r3.u64 = ui_movies::TextMapFindKey(base, movie + ui_movies::kMovieTexts,
                                         guest::LoadU32(base, params + 120));
}

// What Text::Draw was handed (its stand-in, hooks::g_text_draw_stand_in):
// the registers, the draw parameters' 124 bytes and the text to its end.
struct TextDraw {
  uint32_t r1, r3, r4, r5;
  std::vector<uint8_t> params, text;
};
std::vector<TextDraw>* g_text_draws = nullptr;

void TextDrawStandIn(PPCContext& ctx, uint8_t* base) {
  TextDraw d{ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, Read(base, ctx.r4.u32, 124), {}};
  for (uint32_t k = 0; k < 2048; k++) {
    const uint16_t unit = guest::LoadU16(base, ctx.r5.u32 + 2 * k);
    d.text.push_back(uint8_t(unit >> 8));
    d.text.push_back(uint8_t(unit));
    if (unit == 0) {
      break;
    }
  }
  g_text_draws->push_back(std::move(d));
}

// Bytes with the float words at `floats` (offsets into them) compared as
// floats: taken out (zeroed) and added after.
void BytesWithFloats(Outputs& out, std::vector<uint8_t> bytes,
                     std::initializer_list<uint32_t> floats) {
  std::vector<uint32_t> words;
  for (uint32_t offset : floats) {
    uint32_t word = 0;
    for (uint32_t i = 0; i < 4; i++) {
      word = (word << 8) | bytes[offset + i];
      bytes[offset + i] = 0;
    }
    words.push_back(word);
  }
  out.Bytes(bytes);
  for (uint32_t word : words) {
    out.Float(word);
  }
}

// The draw parameters' float words.
#define ITC_PARAMS_FLOATS(at) \
  at + 16, at + 24, at + 28, at + 32, at + 36, at + 40, at + 96, at + 100, at + 104

// A made-up world for the texts in the block: the movie (its map of texts
// at +0x9BCC), its text list's items (+0xA000, four of 128 bytes), the TXT
// nodes (+0xA400, four of 160), the text records (+0xA800, eight of 32),
// their strings (+0xA900, eight of 128 bytes), the font (+0xB000; [0x826C5C00]
// pointed at it) with its four pages (+0xF200) sharing one glyph table
// (+0xF300: 256 codes, the advance at +2) and the draw parameters for
// ScrollOffset's own cases (+0xF800). The texts are characters below 256,
// new lines, page breaks and colors. With `scrolls`, the values a scrolling
// text's loop depends on are kept where it ends (a scale of 0.25-2, a small
// letter spacing, a gap of 1,000, a finite speed of 0-4: the original loops
// forever, or nearly, otherwise).
struct TextWorld {
  static constexpr uint32_t kItems = 0xA000, kNodes = 0xA400, kRecords = 0xA800,
                            kStrings = 0xA900, kFont = 0xB000, kPages = 0xF200,
                            kGlyphs = 0xF300, kParams = 0xF800, kBytes = 0xF900;
  uint32_t at;
  explicit TextWorld(uint32_t block) : at(block) {}

  void Write(Context& h, bool scrolls, uint32_t map_count) const {
    uint8_t* base = h.base;
    Random& r = h.random;
    // The strings.
    for (uint32_t s = 0; s < 8; s++) {
      const uint32_t string = at + kStrings + 128 * s;
      const uint32_t length = s == 0 ? 0 : r.Below(60);
      for (uint32_t k = 0; k < length; k++) {
        const uint32_t pick = r.Below(30);
        uint16_t unit = uint16_t(1 + r.Below(255));
        if (pick == 0) unit = 0xFFD1;
        else if (pick < 3) unit = '\n';
        else if (pick == 3) unit = 0xFFD6;
        else if (pick == 4) unit = uint16_t(0xFFF6 + r.Below(10));
        guest::StoreU16(base, string + 2 * k, unit);
      }
      guest::StoreU16(base, string + 2 * length, 0);
    }
    // The records: +0 a string, +4 scrolling, +8 the counter, +12 the delay,
    // +16 the speed, +20 the page, +24 the lines to skip.
    for (uint32_t k = 0; k < 8; k++) {
      const uint32_t record = at + kRecords + 32 * k;
      guest::StoreU32(base, record + 0, at + kStrings + 128 * r.Below(8));
      guest::StoreU32(base, record + 4, scrolls && r.Below(2) == 0 ? uint32_t(r.Next()) | 1 : 0);
      guest::StoreU32(base, record + 8, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(100));
      guest::StoreU32(base, record + 12, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(50));
      guest::StoreU32(base, record + 16,
                      scrolls ? (r.Below(8) == 0 ? 0x7FC00000u
                                                 : std::bit_cast<uint32_t>(float(r.Below(17)) / 4.0f))
                              : RandomWord(r));
      guest::StoreU32(base, record + 20, r.Below(3));
      guest::StoreU32(base, record + 24, r.Below(3));
      if (scrolls) {
        // The ticks past the delay small: the counter and delay both small.
        guest::StoreU32(base, record + 8, r.Below(100));
        guest::StoreU32(base, record + 12, r.Below(50));
      }
    }
    // The map: ids 0-9 to records (some 0), the count.
    const uint32_t map = at + ui_movies::kMovieTexts;
    for (uint32_t k = 0; k < 128; k++) {
      guest::StoreU32(base, map + 8 * k, r.Below(10));
      guest::StoreU32(base, map + 8 * k + 4,
                      r.Below(8) == 0 ? 0 : at + kRecords + 32 * r.Below(8));
    }
    guest::StoreU32(base, map + 1024, map_count);
    guest::StoreU32(base, 0x826C5CD8, r.Below(3) == 0 ? 0 : at + kRecords + 32 * r.Below(8));
    // The font: four pages, one glyph table.
    const uint32_t font = at + kFont;
    for (uint32_t p = 0; p < 4; p++) {
      guest::StoreU32(base, font + 20 + 4 * p, at + kPages + 16 * p);
      guest::StoreU32(base, at + kPages + 16 * p + 4, at + kGlyphs);
    }
    for (uint32_t c = 0; c < 256; c++) {
      guest::StoreU32(base, at + kGlyphs + 4 * c, uint32_t(r.Below(256) << 2) | r.Below(4));
    }
    guest::StoreU32(base, strings_and_font::kFont, font);
  }

  // A TXT node's fields the drawing reads.
  void WriteNode(Context& h, uint32_t node, bool scrolls) const {
    uint8_t* base = h.base;
    Random& r = h.random;
    for (uint32_t k = 0; k < 160; k += 4) {
      guest::StoreU32(base, node + k, RandomWord(r));
    }
    auto small_float = [&](int32_t range) {
      return r.Below(5) == 0 && !scrolls
                 ? RandomWord(r)
                 : std::bit_cast<uint32_t>(float(int32_t(r.Below(2 * range)) - range));
    };
    guest::StoreU32(base, node + 20, scrolls ? r.Below(64) : uint32_t(r.Next()));
    guest::StoreU32(base, node + 24, uint32_t(r.Next()));
    for (uint32_t k : {56u, 60u, 64u}) {
      guest::StoreU32(base, node + k, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(256));
    }
    guest::StoreU32(base, node + 68, r.Below(4) == 0 ? RandomWord(r)
                                                     : std::bit_cast<uint32_t>(float(r.Below(5)) / 4.0f));
    guest::StoreU32(base, node + 76, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(32));
    guest::StoreU32(base, node + 100, scrolls ? std::bit_cast<uint32_t>(1000.0f) : small_float(500));
    guest::StoreU32(base, node + 104, small_float(500));
    guest::StoreU32(base, node + 116, scrolls ? std::bit_cast<uint32_t>(float(r.Below(5)))
                                              : small_float(8));
    guest::StoreU32(base, node + 120, small_float(8));
    guest::StoreU32(base, node + 108, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(12));
  }

  // A text item: +0 the node, +16 its matrix (the scale +16 kept in 0.25-2
  // with `scrolls`), +80 the depth, +84 the color, +100 the clip rectangle.
  void WriteItem(Context& h, uint32_t item, uint32_t node, bool scrolls) const {
    uint8_t* base = h.base;
    Random& r = h.random;
    for (uint32_t k = 0; k < 128; k += 4) {
      guest::StoreU32(base, item + k, RandomWord(r));
    }
    guest::StoreU32(base, item + 0, node);
    for (uint32_t k = 0; k < 16; k++) {
      const uint32_t word =
          r.Below(6) == 0 && !scrolls
              ? RandomWord(r)
              : std::bit_cast<uint32_t>(float(int32_t(r.Below(64)) - 16) / 8.0f);
      guest::StoreU32(base, item + 16 + 4 * k, word);
    }
    if (scrolls) {
      guest::StoreU32(base, item + 16, std::bit_cast<uint32_t>(float(1 + r.Below(8)) / 4.0f));
    }
    for (uint32_t k = 0; k < 4; k++) {
      guest::StoreU32(base, item + 84 + 4 * k,
                      r.Below(5) == 0 ? RandomWord(r)
                                      : std::bit_cast<uint32_t>(float(r.Below(5)) / 4.0f));
      guest::StoreU32(base, item + 100 + 4 * k, uint32_t(int32_t(r.Below(1400)) - 100));
    }
  }
};

void ScrollOffsetCases(Context& h, const Block& block) {
  constexpr uint32_t kGlobals = 0x826C5CCC, kGlobalBytes = 0x14;
  const std::vector<uint8_t> globals = Read(h.base, kGlobals, kGlobalBytes);
  const uint32_t font_word = guest::LoadU32(h.base, strings_and_font::kFont);
  const TextWorld world(block.at);
  Results results("UiMovie::ScrollOffset (sub_8211B800)");
  for (int i = 0; i < 1000; i++) {
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, block.at, TextWorld::kBytes);
      Random& r = h.random;
      const uint32_t params = block.at + TextWorld::kParams;
      world.Write(h, true, 1 + r.Below(12));
      // No record of 0 here (the original reads its +8 at address 8): only
      // DrawTexts calls it, for a text whose record scrolls.
      for (uint32_t k = 0; k < 128; k++) {
        const uint32_t value = block.at + ui_movies::kMovieTexts + 8 * k + 4;
        if (guest::LoadU32(h.base, value) == 0) {
          guest::StoreU32(h.base, value, block.at + TextWorld::kRecords);
        }
      }
      guest::StoreU32(h.base, 0x826C5CD8, block.at + TextWorld::kRecords + 32 * r.Below(8));
      // The parameters as DrawTexts builds them; a counter past the delay
      // (the loop) only with the values it ends with.
      for (uint32_t k = 0; k < 124; k += 4) {
        guest::StoreU32(h.base, params + k, RandomWord(r));
      }
      guest::StoreU32(h.base, params + 0, uint32_t(int32_t(r.Below(2000)) - 500));
      guest::StoreU32(h.base, params + 8, r.Below(64));
      guest::StoreU32(h.base, params + 28, std::bit_cast<uint32_t>(float(1 + r.Below(8)) / 4.0f));
      guest::StoreU32(h.base, params + 36, std::bit_cast<uint32_t>(float(1 + r.Below(8)) / 4.0f));
      guest::StoreU32(h.base, params + 60, r.Below(4));
      guest::StoreU32(h.base, params + 68, r.Below(2) == 0 ? 1000 : 100 + r.Below(500));
      guest::StoreU32(h.base, params + 76, r.Below(5));
      guest::StoreU32(h.base, params + 84, r.Below(5));
      guest::StoreU32(h.base, params + 120, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(12));
      PPCContext t = CallContext(h);
      t.r3.u64 = block.at;
      t.r4.u64 = params;
      t.f1.u64 = r.Next();
      if (v == 0) {
        RunRecompiled(__imp__sub_8211B800, t, h.base);
      } else {
        NativeScrollOffset(t, h.base);
      }
      out[v].Bytes(Read(h.base, block.at, TextWorld::kBytes));
      out[v].Double(t.f1.u64);
      out[v].Word(t.r3.u32);
      Write(h.base, kGlobals, globals.data(), kGlobalBytes);
      guest::StoreU32(h.base, strings_and_font::kFont, font_word);
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// DrawTexts through its adapter against the original: 0 to 4 queued items
// on made-up nodes and texts, Text::Draw answered by the stand-in (what it
// was handed compared). Compared: the block, the frame's stores (from its
// +80 to the text buffer's end; filled with 0xCD first), each Text::Draw's
// registers, parameters and text, and r3.
void DrawTextsCases(Context& h, const Block& block) {
  constexpr uint32_t kGlobals = 0x826C5CCC, kGlobalBytes = 0x14;
  const std::vector<uint8_t> globals = Read(h.base, kGlobals, kGlobalBytes);
  const uint32_t font_word = guest::LoadU32(h.base, strings_and_font::kFont);
  const TextWorld world(block.at);
  std::vector<TextDraw> draws;
  g_text_draws = &draws;
  hooks::g_text_draw_stand_in = TextDrawStandIn;
  Results results("UiMovie::DrawTexts (sub_8211CD90)");
  for (int i = 0; i < 600; i++) {
    const Random start = h.random;
    Outputs out[2];
    uint32_t items = 0;
    bool scrolling = false;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Random& r = h.random;
      draws.clear();
      Fill(h.base, block.at, TextWorld::kBytes);
      const bool scrolls = r.Below(2) == 0;
      world.Write(h, scrolls, r.Below(13));
      const uint32_t list = block.at + 40;
      const uint32_t count = i < 2 ? uint32_t(i) : r.Below(5);
      guest::StoreU32(h.base, list + 0, block.at + TextWorld::kItems);
      guest::StoreU32(h.base, list + 8, count);
      items = count;
      scrolling = scrolls;
      for (uint32_t k = 0; k < 4; k++) {
        world.WriteNode(h, block.at + TextWorld::kNodes + 160 * k, scrolls);
      }
      for (uint32_t k = 0; k < 4; k++) {
        world.WriteItem(h, block.at + TextWorld::kItems + 128 * k,
                        block.at + TextWorld::kNodes + 160 * r.Below(4), scrolls);
      }
      const uint32_t r5 = r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2);
      PPCContext t = CallContext(h);
      const uint32_t frame = t.r1.u32 - ui_movies::kDrawTextsFrame;
      Fill(h.base, frame - 512, ui_movies::kDrawTextsFrame + 512);
      t.r3.u64 = block.at;
      t.r4.u64 = list;
      t.r5.u64 = r5;
      if (v == 0) {
        RunRecompiled(__imp__sub_8211CD90, t, h.base);
      } else {
        sub_8211CD90(t, h.base);
      }
      std::vector<uint8_t> bytes = Read(h.base, block.at, TextWorld::kBytes);
      std::vector<uint32_t> node_floats;
      for (uint32_t k = 0; k < 4; k++) {
        for (uint32_t offset : {28u, 32u}) {
          const uint32_t at = TextWorld::kNodes + 160 * k + offset;
          node_floats.push_back(guest::LoadU32(h.base, block.at + at));
          std::fill(bytes.begin() + at, bytes.begin() + at + 4, 0);
        }
      }
      out[v].Bytes(bytes);
      for (uint32_t word : node_floats) {
        out[v].Float(word);
      }
      constexpr uint32_t kFrom = 80, kTo = ui_movies::kDrawTextsBuffer + 4096;
      constexpr uint32_t p = ui_movies::kDrawTextsParams - kFrom;
      BytesWithFloats(out[v], Read(h.base, frame + kFrom, kTo - kFrom),
                      {96 - kFrom, 100 - kFrom, 104 - kFrom, 108 - kFrom, ITC_PARAMS_FLOATS(p)});
      out[v].Word(uint32_t(draws.size()));
      for (const TextDraw& d : draws) {
        out[v].Word(d.r1);
        out[v].Word(d.r3);
        out[v].Word(d.r4);
        out[v].Word(d.r5);
        BytesWithFloats(out[v], d.params, {ITC_PARAMS_FLOATS(0u)});
        out[v].Bytes(d.text);
      }
      out[v].Word(t.r3.u32);
      Write(h.base, kGlobals, globals.data(), kGlobalBytes);
      guest::StoreU32(h.base, strings_and_font::kFont, font_word);
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: {} items{}", i, items, scrolling ? ", scrolling" : ""));
  }
  hooks::g_text_draw_stand_in = nullptr;
  g_text_draws = nullptr;
  results.Report();
}
#undef ITC_PARAMS_FLOATS

}  // namespace

void UiMoviesGame(Context& h) {
  auto* memory = rex::system::kernel_state()->memory();
  Block block;
  block.bytes = 0x30000;  // the layers' frames at its top
  block.at = memory->SystemHeapAlloc(block.bytes);
  if (!block.at) {
    ITC_LOG_ERROR("harness: ui_movies_game: no block from the system heap");
    return;
  }
  NodeCases(h, block);
  ResetPoseCases(h, block);
  LeafSetterCases(h, block);
  ClearTextIdsCases(h, block);
  SwapTextureCases(h, block);
  ItemCases(h, block);
  AnimationListCases(h, block);
  PairsCases(h, block);
  ScreenStartCases(h, block);
  LayersCases(h, block);
  TextCases(h, block);
  MoveCases(h, block);
  PlayCases(h, block);
  ScrollOffsetCases(h, block);
  DrawTextsCases(h, block);
  memory->SystemHeapFree(block.at);
}

}  // namespace harness
