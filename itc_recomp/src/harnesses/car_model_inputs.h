// A car model's inputs for the harnesses of its reset and release
// (scene_loaders, car_models): the loads' members at random, with what each
// side allocates kept and taken back after the case. Shared by
// harnesses/scene_loaders.cpp and car_models.cpp.

#pragma once

#include "harness.h"
#include "pending_frees.h"

#include "memory/heap.h"
#include "models/texture_provider.h"
#include "scene_loaders/loads.h"

namespace harness::car_model_inputs {

namespace sl = scene_loaders;

// A word that pointed at the side's own allocation.
constexpr uint32_t kHeld = 0x48454C44;  // 'HELD'

// What a case writes, drawn once for both sides.
struct Plan {
  bool values;               // the skeleton's node values: a heap block, or 0
  uint32_t xpr_kinds[2][2];  // each XPR's header and data block: 0 none, 1 untagged, 2 tagged
  uint32_t header_words[4][3];
  uint32_t provider_slots[2];  // each provider's vector size
  bool shared[3];              // the release's slots (+0, +224, +248): shared, pages
  bool pages[3];
  uint32_t page_bytes[3];
};

inline Plan MakePlan(Random& r) {
  Plan p{};
  p.values = r.Below(3) != 0;
  for (auto& xpr : p.xpr_kinds) {
    for (uint32_t& kind : xpr) {
      kind = r.Below(3);
    }
  }
  for (auto& words : p.header_words) {
    for (uint32_t& word : words) {
      word = uint32_t(r.Next());
    }
  }
  for (uint32_t& slots : p.provider_slots) {
    slots = r.Below(9);
  }
  for (int i = 0; i < 3; i++) {
    p.shared[i] = r.Below(4) == 0;
    p.pages[i] = r.Below(3) != 0;
    p.page_bytes[i] = 1 + r.Below(0x20000);
  }
  return p;
}

// What a side allocated.
struct Held {
  uint32_t values = 0;
  uint32_t pages[3] = {};
};

constexpr uint32_t kSlots[] = {0, 224, 248};  // in the loads

// Writes the plan into the loads at `loads` (their bytes already random),
// with the XPRs' headers and the providers' slots at `scratch` (0x200
// bytes). `release` sets the release's slots' words too.
inline void Write(uint8_t* base, uint32_t loads, uint32_t scratch, const Plan& p, bool release,
                  Held& held) {
  using pending_frees::Store;
  held.values = p.values ? memory::Alloc(48, 0) : 0;
  Store(base, loads + sl::kCarModelLoadsSkeleton + 176, held.values);
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t xpr = loads + sl::kCarModelLoadsXprs[i];
    for (uint32_t b = 0; b < 2; b++) {
      const uint32_t n = 2 * i + b;
      Store(base, xpr + 8 * b,
            pending_frees::WriteBlock(base, p.xpr_kinds[i][b], scratch + 16 * n,
                                      p.header_words[n]));
    }
    const uint32_t provider = loads + sl::kCarModelLoadsProviders[i];
    const uint32_t vector = provider + models::kTextureProviderVector;
    const uint32_t slots = scratch + 0x100 + 64 * i;
    Store(base, provider, models::kTextureProviderVtable);
    Store(base, vector + models::kVectorFirst, slots);
    Store(base, vector + models::kVectorLast, slots + 4 * p.provider_slots[i]);
    Store(base, vector + models::kVectorEnd, slots + 4 * 16);
  }
  for (int i = 0; i < 3 && release; i++) {
    const uint32_t slot = loads + kSlots[i];
    held.pages[i] = p.pages[i] ? memory::AllocPages(p.page_bytes[i], 4096) : 0;
    Store(base, slot + 12, p.shared[i] ? 1 : 0);
    Store(base, slot + 16, held.pages[i]);
  }
}

// After a side: whether each allocation is still alive (then freed), and the
// words left pointing at it marked, so the block compares.
inline void Take(uint8_t* base, uint32_t loads, Held& held, Outputs& out) {
  using pending_frees::Load;
  using pending_frees::Store;
  const uint32_t values_at = loads + sl::kCarModelLoadsSkeleton + 176;
  const bool values = held.values != 0 && memory::IsHeapBlock(held.values);
  out.Word(values);
  if (held.values != 0 && Load(base, values_at) == held.values) {
    Store(base, values_at, kHeld);
  }
  if (values) {
    memory::Free(held.values);
  }
  for (int i = 0; i < 3; i++) {
    const uint32_t at = loads + kSlots[i] + 16;
    const bool pages = held.pages[i] != 0 && memory::IsPages(held.pages[i]);
    out.Word(pages);
    if (held.pages[i] != 0 && Load(base, at) == held.pages[i]) {
      Store(base, at, kHeld);
    }
    if (pages) {
      memory::FreePages(held.pages[i]);
    }
  }
}

}  // namespace harness::car_model_inputs
