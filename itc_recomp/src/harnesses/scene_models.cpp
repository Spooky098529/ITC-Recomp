// The scene models' parts (src/scene_models/, docs/functions/scene-models.md)
// against the recompiled functions. Each case fills a block of guest memory
// with random bytes, writes its inputs there, runs the original and the
// register steps (hooks/scene_models.h) from the same inputs, and compares
// the block, the globals the functions write and r3. The globals are put
// back after each case. Floats are compared as floats only where the
// originals copy them with lfs and stfs from their inputs (CarRecord::Copy's
// +32 of each layer record); the constants the defaults store, and
// everything else, exactly.

#include "harness.h"

#include "hooks/scene_models.h"
#include "memory/heap.h"
#include "scene_models/scene_models.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace harness {

namespace {

namespace sm = scene_models;

// The globals the functions read or write: the accessories' set, the sound
// loads' counts and flags.
struct Region {
  uint32_t address;
  uint32_t bytes;
};
constexpr Region kRegions[] = {
    {0x826C5510, 0x10},
    {0x826D8350, 0x10},
    {0x826D83AC, 0x20},
};

constexpr uint32_t kBlockBytes = 0x4000;

// A float range [offset, offset + 4 x count) of the block.
using FloatRanges = std::vector<std::pair<uint32_t, uint32_t>>;

void AddBlock(Outputs& out, uint8_t* base, uint32_t block, FloatRanges floats) {
  std::sort(floats.begin(), floats.end());
  uint32_t at = 0;
  for (const auto& [offset, count] : floats) {
    if (offset > at) {
      out.Bytes(Read(base, block + at, offset - at));
    }
    out.Floats(base, block + offset, count);
    at = offset + 4 * count;
  }
  if (at < kBlockBytes) {
    out.Bytes(Read(base, block + at, kBlockBytes - at));
  }
}

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A float's bits: now and then a NaN (either sign, quiet or signaling), an
// infinity, a zero or a denormal; else a value of a few hundred.
uint32_t RandomFloat(Random& r) {
  switch (r.Below(12)) {
    case 0: return 0x7FC00000u | (r.Below(2) << 31) | r.Below(0x400000);
    case 1: return 0x7F800001u | (r.Below(2) << 31) | r.Below(0x3FFFFF);
    case 2: return 0x7F800000u | (r.Below(2) << 31);
    case 3: return r.Below(2) << 31;
    case 4: return r.Below(0x800000) | (r.Below(2) << 31);
    default: {
      const float f = (float(r.Below(200000)) - 100000.0f) / 256.0f;
      return std::bit_cast<uint32_t>(f);
    }
  }
}

// One case through both versions: the block filled at random and
// `setup(t)` writing the inputs and the registers (once: its inputs are
// written back for the second run); `floats` the block's float ranges.
void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const FloatRanges& floats, const std::string& what) {
  std::vector<std::vector<uint8_t>> saved;
  for (const Region& region : kRegions) {
    saved.push_back(Read(h.base, region.address, region.bytes));
  }
  RandomBytes(h.base, block, kBlockBytes, r);
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, kBlockBytes);
  std::vector<std::vector<uint8_t>> globals;
  for (const Region& region : kRegions) {
    globals.push_back(Read(h.base, region.address, region.bytes));
  }
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), kBlockBytes);
    for (size_t i = 0; i < globals.size(); i++) {
      Write(h.base, kRegions[i].address, globals[i].data(), kRegions[i].bytes);
    }
    PPCContext t = start;
    (v == 0 ? recompiled : native)(t, h.base);
    AddBlock(out[v], h.base, block, floats);
    for (const Region& region : kRegions) {
      out[v].Bytes(Read(h.base, region.address, region.bytes));
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  for (size_t i = 0; i < saved.size(); i++) {
    Write(h.base, kRegions[i].address, saved[i].data(), kRegions[i].bytes);
  }
  results.Check(out[0], out[1], what);
}

// --- Getters, the sound loads' callback, links ---

void Links(Context& h, Random& r, uint32_t block) {
  {
    Results results("sub_820F0E50");
    for (int c = 0; c < 16; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            guest::StoreU32(h.base, sm::kAccessories, c == 0 ? 0 : uint32_t(r.Next()));
            t.r3.u64 = r.Next();
          },
          __imp__sub_820F0E50, hooks::SceneModelsAccessories, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneModel::Accessory (sub_820D97B8)");
    for (int c = 0; c < 64; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const uint32_t set = block + 0x400;
            guest::StoreU32(h.base, sm::kAccessories, set);
            for (uint32_t i = 0; i < 3; i++) {
              guest::StoreU32(h.base, set + 4 + 4 * i, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
            }
            // 0-3 mostly, now and then another small number.
            const uint32_t interior = block + 4 * r.Below(16);
            guest::StoreU32(h.base, interior + 140, r.Below(8) == 0 ? r.Below(64) : r.Below(4));
            t.r3.u64 = interior;
          },
          __imp__sub_820D97B8, hooks::SceneModelsAccessory, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("sub_82175EE0 (the sound loads' callback)");
    for (int c = 0; c < 128; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const uint32_t slot = r.Below(7);
            guest::StoreU32(h.base, block, slot);
            // The count: 0, 1 (reaching 2), 2, 255 (wrapping) or random.
            const uint8_t counts[] = {0, 1, 1, 2, 255, uint8_t(r.Next())};
            guest::StoreU8(h.base, sm::kSoundLoadsDone + slot, counts[r.Below(6)]);
            guest::StoreU32(h.base, sm::kSoundLoaded + 4 * slot, uint32_t(r.Next()));
            t.r3.u64 = r.Next();
            t.r6.u64 = block;
          },
          __imp__sub_82175EE0, hooks::SceneModelsOnSoundLoaded, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  struct Setter {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    bool car_model;  // r4 a car model whose +24 is read
  };
  const Setter setters[] = {
      {"sub_820E7378", __imp__sub_820E7378, hooks::SceneModelsWheelsSetRecord, false},
      {"sub_820E70B8", __imp__sub_820E70B8, hooks::SceneModelsSetCarModel, false},
      {"SceneParts::Link (sub_820DF1A8)", __imp__sub_820DF1A8, hooks::SceneModelsPartsLink, true},
      {"sub_820DF1C0", __imp__sub_820DF1C0, hooks::SceneModelsPartsSetRecord, false},
      {"SceneInterior::Link (sub_820E37D0)", __imp__sub_820E37D0, hooks::SceneModelsInteriorLink,
       true},
      {"sub_820E3658", __imp__sub_820E3658, hooks::SceneModelsSetWord11452, false},
  };
  for (const Setter& setter : setters) {
    Results results(setter.name);
    for (int c = 0; c < 32; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            // The setters of a far field (+16776, +37248, +11452) get an
            // object below the block, so the field lands in it.
            const uint32_t object = block - (setter.native == hooks::SceneModelsPartsSetRecord
                                                 ? 37248 - 0x100
                                             : setter.native == hooks::SceneModelsWheelsSetRecord
                                                 ? 16776 - 0x100
                                             : setter.native == hooks::SceneModelsSetWord11452
                                                 ? 11452 - 0x100
                                                 : 0u) +
                                    4 * r.Below(8);
            t.r3.u64 = object;
            if (setter.car_model) {
              const uint32_t model = block + 0x200 + 4 * r.Below(8);
              t.r4.u64 = model;
              // Overlapping now and then: the model's +24 inside the
              // object's fields.
              if (c % 8 == 7) {
                t.r4.u64 = object;
              }
            } else {
              t.r4.u64 = r.Next();
            }
          },
          setter.recompiled, setter.native, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- Copies ---

constexpr uint32_t kLayerOffsets[] = {176, 368, 560, 944, 1328, 1712, 2096, 2480, 2864, 3248};
constexpr uint32_t kLayerCounts[] = {3, 3, 6, 6, 6, 6, 6, 6, 6, 6};

void Copies(Context& h, Random& r, uint32_t block) {
  {
    Results results("CarModel::SetRecord (sub_820D3C70)");
    for (int c = 0; c < 32; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const uint32_t model = block + 4 * r.Below(16);
            const uint32_t record = c % 8 == 7 ? model + 32 + 4 * r.Below(64)  // overlapping
                                               : block + 0x1000 + 4 * r.Below(256);
            t.r3.u64 = model;
            t.r4.u64 = record;
          },
          __imp__sub_820D3C70, hooks::SceneModelsCarModelSetRecord, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarRecord::Copy (sub_820D7EE8)");
    for (int c = 0; c < 64; c++) {
      const bool overlap = c % 16 == 15;
      const bool reverse = c % 2 == 1;
      const uint32_t low = block + 16 * r.Below(8);
      const uint32_t high = overlap ? low + 16 * (1 + r.Below(100)) : block + 0x2000 + 16 * r.Below(8);
      const uint32_t to = reverse ? low : high;
      const uint32_t from = reverse ? high : low;
      FloatRanges floats;
      if (!overlap) {
        for (size_t b = 0; b < 10; b++) {
          for (uint32_t i = 0; i < kLayerCounts[b]; i++) {
            floats.push_back({to - block + kLayerOffsets[b] + 64 * i + 32, 1});
          }
        }
      }
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            for (size_t b = 0; b < 10; b++) {
              for (uint32_t i = 0; i < kLayerCounts[b]; i++) {
                guest::StoreU32(h.base, from + kLayerOffsets[b] + 64 * i + 32, RandomFloat(r));
              }
            }
            t.r3.u64 = to;
            t.r4.u64 = from;
          },
          __imp__sub_820D7EE8, hooks::SceneModelsCarRecordCopy, floats,
          fmt::format("case {}{}", c, overlap ? " (overlapping: the original's)" : ""));
    }
    results.Report();
  }
  {
    Results results("SceneModelRecord::Copy (sub_820DB290)");
    for (int c = 0; c < 48; c++) {
      const bool overlap = c % 16 == 15;
      const bool reverse = c % 2 == 1;
      const uint32_t low = block + 16 * r.Below(8);
      const uint32_t high =
          overlap ? low + 16 * (1 + r.Below(100)) : block + 0x2000 + 16 * r.Below(8);
      const uint32_t to = reverse ? low : high;
      const uint32_t from = reverse ? high : low;
      FloatRanges floats;
      if (!overlap) {
        for (size_t b = 0; b < 10; b++) {
          for (uint32_t i = 0; i < kLayerCounts[b]; i++) {
            floats.push_back({to - block + 16 + kLayerOffsets[b] + 64 * i + 32, 1});
          }
        }
      }
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            for (size_t b = 0; b < 10; b++) {
              for (uint32_t i = 0; i < kLayerCounts[b]; i++) {
                guest::StoreU32(h.base, from + 16 + kLayerOffsets[b] + 64 * i + 32,
                                RandomFloat(r));
              }
            }
            t.r3.u64 = to;
            t.r4.u64 = from;
          },
          __imp__sub_820DB290, hooks::SceneModelsSceneModelRecordCopy, floats,
          fmt::format("case {}{}", c, overlap ? " (overlapping: the original's)" : ""));
    }
    results.Report();
  }
}

// --- Records' defaults ---

void Defaults(Context& h, Random& r, uint32_t block) {
  struct Reset {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Reset resets[] = {
      {"sub_820D45B0", __imp__sub_820D45B0, hooks::SceneModelsCarRecordSetW},
      {"sub_820D3B38", __imp__sub_820D3B38, hooks::SceneModelsRecord12Reset},
      {"sub_820D0A20", __imp__sub_820D0A20, hooks::SceneModelsLayerReset},
      {"CarRecord::Reset (sub_820D0A98)", __imp__sub_820D0A98, hooks::SceneModelsCarRecordReset},
      {"SceneModelRecord::SceneModelRecord (sub_820DB240)", __imp__sub_820DB240,
       hooks::SceneModelsSceneModelRecordConstruct},
      {"sub_820D34B8", __imp__sub_820D34B8, hooks::SceneModelsCockpitRecordReset},
      {"sub_820D3558", __imp__sub_820D3558, hooks::SceneModelsPartsRecordReset},
      {"sub_820D3840", __imp__sub_820D3840, hooks::SceneModelsInteriorRecordReset},
      {"sub_820D3900", __imp__sub_820D3900, hooks::SceneModelsWheelsRecordReset},
      {"sub_820D34E0", __imp__sub_820D34E0, hooks::SceneModelsPool4RecordReset},
  };
  for (const Reset& reset : resets) {
    Results results(reset.name);
    for (int c = 0; c < 16; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) { t.r3.u64 = block + 0x100 + 4 * r.Below(64); },
          reset.recompiled, reset.native, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The car model's pages ---

void Pages(Context& h, Random& r, uint32_t block) {
  Results results("sub_820C3CA0 (VirtualAlloc)");
  const uint32_t sizes[] = {0, 1, 16, 4095, 4096, 4097, 0x10000, 0x23456};
  for (int c = 0; c < 24; c++) {
    const uint32_t size = c < 8 ? sizes[c] : 1 + r.Below(0x40000);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      const uint32_t at = block + 4 * r.Below(16);
      PPCContext t = CallContext(h);
      t.r3.u64 = r.Next();
      t.r4.u64 = at;
      t.r5.u64 = size;
      (v == 0 ? __imp__sub_820C3CA0 : hooks::SceneModelsAllocPages)(t, h.base);
      const uint32_t pages = t.r3.u32;
      out[v].Word(guest::LoadU32(h.base, at) == pages);
      out[v].Word(pages != 0);
      out[v].Word(pages != 0 && memory::IsPages(pages));
      out[v].Word(pages % 4096);
      bool cleared = true;
      for (uint32_t i = 0; pages != 0 && i < size; i++) {
        cleared = cleared && guest::LoadU8(h.base, pages + i) == 0;
      }
      out[v].Word(cleared);
      if (pages != 0 && memory::IsPages(pages)) {
        memory::FreePages(pages);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {} (size {:#x})", c, size));
  }
  results.Report();
}

}  // namespace

void SceneModels(Context& h) {
  Random r(0x7363656E65ull);  // "scene"
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Links(h, r, block);
  Copies(h, r, block);
  Defaults(h, r, block);
  Pages(h, r, block);
  memory::Free(allocation);
}

}  // namespace harness
