// The game's side of the UI movies (src/hooks/ui_movies_game.cpp) against
// the recompiled functions. Movies are live guest objects: every case builds
// its own in a block taken from the system heap for the run (movies span
// about 41 KB, more than the scratch area), so nothing the game holds is
// read or written.

#include "harness.h"

#include "base/log.h"

#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <iterator>
#include <string>
#include <vector>

namespace harness {

namespace {

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
// function runs; the bytes, r3 and (with `f1`) f1 are compared, from the
// first difference on (32 bytes of it logged).
template <typename Setup, typename Args>
void Compare(Context& h, Results& results, uint32_t area, uint32_t bytes, Setup setup, Args args,
             GuestFunction recompiled, GuestFunction native, const std::string& what,
             bool f1 = false) {
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, area, bytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    (v == 0 ? recompiled : native)(t, h.base);
    result[v] = Read(h.base, area, bytes);
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
              __imp__sub_8211A660, sub_8211A660, fmt::format("n {:X}", n));
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

}  // namespace

void UiMoviesGame(Context& h) {
  auto* memory = rex::system::kernel_state()->memory();
  Block block;
  block.bytes = 0x10000;
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
  memory->SystemHeapFree(block.at);
}

}  // namespace harness
