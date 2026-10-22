// The UI movie library's functions (src/hooks/ui_movies.cpp) against the
// recompiled ones. Movies are live guest objects: every case builds its own
// in the scratch area, so nothing the game holds is read or written.

#include "harness.h"

#include "base/log.h"

namespace harness {

namespace {

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
}

}  // namespace harness
