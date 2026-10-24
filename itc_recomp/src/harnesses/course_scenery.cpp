// The course scenery (src/course_scenery/, docs/functions/course-scenery.md)
// against the recompiled functions. Each case builds its inputs in the
// scratch area: pool objects with random flags, keys and the game's own
// vtables (the base's, the area lights' and the area models'; a vtable in
// the scratch area whose slots are harmless functions for the loads, which
// would start archive reads, and for a key slot that isn't native), pool
// sets whose free and used lists link random objects, the scenery's lists
// with random distances (NaNs and infinities among them), areas and models,
// and configurations' area tables. Both versions run on the same bytes; the
// scratch area (and what an original writes below its stack) and r3 are
// compared, exactly but for the lists' distances (floats). ObjectPools::Init
// and Shutdown take and free physical memory: Init's block is compared with
// its addresses made relative to it; Shutdown's pools are in a block of
// their own made for each version, their objects in the scratch area. The
// originals run with --native_course_scenery off.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "course_scenery/course_scenery.h"
#include "hooks/heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_course_scenery);

namespace harness {

namespace {

namespace cs = course_scenery;

// Runs `body` with the native flag off (v == 0) or on.
template <typename Body>
void With(int v, Body body) {
  const bool saved = ITC_FLAG(native_course_scenery);
  ITC_FLAG(native_course_scenery) = v != 0;
  body();
  ITC_FLAG(native_course_scenery) = saved;
}

// --- The calls, as the adapters (src/hooks/course_scenery.cpp) make them ---

struct GuestCalls {
  PPCContext* ctx;
  uint8_t* base;
};

uint32_t CallGuest(void* user, uint32_t function, const uint32_t* args, int count,
                   uint32_t frame, uint32_t lr) {
  auto& call = *static_cast<GuestCalls*>(user);
  PPCContext& ctx = *call.ctx;
  uint8_t* base = call.base;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  PPCRegister* registers[5] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6, &ctx.r7};
  for (int i = 0; i < count; i++) {
    registers[i]->u64 = args[i];
  }
  ctx.ctr.u64 = function;
  REX_CALL_INDIRECT_FUNC(function);
  ctx.r1.u64 = saved;
  return ctx.r3.u32;
}

void SetIndex(PPCContext& ctx, uint32_t index) {
  if (index == 0xFFFFFFFFu) {
    ctx.r3.s64 = -1;
  } else {
    ctx.r3.u64 = index;
  }
}

// Runs the native version of a function that makes calls.
template <typename Body>
void WithCalls(PPCContext& t, uint8_t* base, Body body) {
  GuestCalls call{&t, base};
  const cs::Calls calls{&call, CallGuest};
  body(calls);
}

// The game's vtables: the base's (key: none; reset: PoolObject::Reset), the
// area lights' (key at +84; PoolObject::Reset) and the area models' (key at
// +64; AreaModel::Reset, which with +56 and +60 0 only calls
// PoolObject::Reset).
constexpr uint32_t kBaseVtable = 0x82006848;
constexpr uint32_t kLightsVtable = 0x8200676C;
constexpr uint32_t kModelVtable = 0x82006778;
// The scratch vtable (at kFakeVtable in the scratch area): slot 0 a key that
// isn't native (sub_820FFE98, which copies two halfwords from r4 to r3: the
// size, 0, over the object's vtable halfwords; returns the object), slots 1
// and 2 PoolObject::Reset (no archive reads).
constexpr uint32_t kFakeVtable = 4032;

void WriteFakeVtable(Context& h) {
  guest::StoreU32(h.base, h.data + kFakeVtable + 0, 0x820FFE98);
  guest::StoreU32(h.base, h.data + kFakeVtable + 4, 0x820FB9D8);
  guest::StoreU32(h.base, h.data + kFakeVtable + 8, 0x820FB9D8);
}

uint32_t RandomFlags(Random& r) {
  // Mostly the bits the game uses, now and then any.
  return r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(64);
}

uint32_t RandomFloatBits(Random& r) {
  switch (r.Below(12)) {
    case 0:
      return 0x7FC00000u | (r.Below(2) << 31);  // NaN
    case 1:
      return 0x7F800000u | (r.Below(2) << 31);  // infinity
    case 2:
      return 0;
    default: {
      const float value = float(r.Below(4000)) / 8.0f - 50.0f;
      return std::bit_cast<uint32_t>(value);
    }
  }
}

void ScratchAndR3(Context& h, Outputs& out, const PPCContext& t) {
  out.Bytes(Read(h.base, h.data, kScratchBytes));
  out.Word(t.r3.u32);
}

// --- Pool objects ---

void ObjectLeaves(Context& h) {
  Random& r = h.random;
  struct Leaf {
    const char* name;
    GuestFunction recompiled;
    void (*native)(PPCContext&, uint8_t*);
  };
  const Leaf leaves[] = {
      {"PoolObject::InUse (sub_820FB8E0)", __imp__sub_820FB8E0,
       [](PPCContext& t, uint8_t* b) { t.r3.u64 = cs::InUse(b, t.r3.u32); }},
      {"PoolObject::IsLoaded (sub_820FB8F0)", __imp__sub_820FB8F0,
       [](PPCContext& t, uint8_t* b) { t.r3.u64 = cs::IsLoaded(b, t.r3.u32); }},
      {"PoolObject::IsVisible (sub_820FB908)", __imp__sub_820FB908,
       [](PPCContext& t, uint8_t* b) { t.r3.u64 = cs::IsVisible(b, t.r3.u32); }},
      {"PoolObject::IsExpiring (sub_820FB920)", __imp__sub_820FB920,
       [](PPCContext& t, uint8_t* b) { t.r3.u64 = cs::IsExpiring(b, t.r3.u32); }},
      {"PoolObject::SetVisible (sub_820FB938)", __imp__sub_820FB938,
       [](PPCContext& t, uint8_t* b) { cs::SetVisible(b, t.r3.u32, t.r4.u32); }},
      {"PoolObject::SetExpiring (sub_820FB968)", __imp__sub_820FB968,
       [](PPCContext& t, uint8_t* b) { cs::SetExpiring(b, t.r3.u32, t.r4.u32); }},
      {"PoolObject::ClearLoadFlags (sub_820FB998)", __imp__sub_820FB998,
       [](PPCContext& t, uint8_t* b) { cs::ClearLoadFlags(b, t.r3.u32); }},
      {"PoolObject::MarkLoaded (sub_820FB9A8)", __imp__sub_820FB9A8,
       [](PPCContext& t, uint8_t* b) { cs::MarkLoaded(b, t.r3.u32); }},
      {"PoolObject::Reset (sub_820FB9D8)", __imp__sub_820FB9D8,
       [](PPCContext& t, uint8_t* b) { cs::ResetObject(b, t.r3.u32); }},
      {"PoolObject::Key (sub_820FB9F0)", __imp__sub_820FB9F0,
       [](PPCContext& t, uint8_t* b) {
         uint32_t size = 0;
         t.r3.u64 = cs::PoolObjectKey(t.r3.u32, &size);
         guest::StoreU32(b, t.r4.u32, size);
       }},
      {"AreaLights::Key (sub_820F1C20)", __imp__sub_820F1C20,
       [](PPCContext& t, uint8_t* b) {
         uint32_t size = 0;
         t.r3.u64 = cs::AreaLightsKey(t.r3.u32, &size);
         guest::StoreU32(b, t.r4.u32, size);
       }},
      {"AreaModel::Key (sub_820F1CD8)", __imp__sub_820F1CD8,
       [](PPCContext& t, uint8_t* b) {
         uint32_t size = 0;
         t.r3.u64 = cs::AreaModelKey(t.r3.u32, &size);
         guest::StoreU32(b, t.r4.u32, size);
       }},
      {"PoolObject::AllocData (sub_820FB8B8)", __imp__sub_820FB8B8,
       [](PPCContext& t, uint8_t* b) { t.r3.u64 = cs::AllocData(b, t.r3.u32, t.r4.u32); }},
      {"PoolObject::PoolObject (sub_820FBB20)", __imp__sub_820FBB20,
       [](PPCContext& t, uint8_t* b) {
         cs::ConstructObject(b, t.r3.u32, t.r4.u32, t.r5.u32, t.r6.u32);
       }},
  };
  for (const Leaf& leaf : leaves) {
    Results results(leaf.name);
    for (int c = 0; c < 48; c++) {
      std::vector<uint8_t> object(64);
      for (auto& byte : object) {
        byte = uint8_t(r.Next());
      }
      const uint32_t flags = RandomFlags(r);
      const uint32_t r4 = c % 3 == 0 ? 0 : (c % 3 == 1 ? 1 : uint32_t(r.Next()));
      const uint32_t r4_high = uint32_t(r.Next());
      const uint32_t size = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(300);
      const uint32_t sizes[3] = {r.Below(200), r.Below(400), r.Below(16)};
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        const uint32_t at = h.data + 64;
        Write(h.base, at, object.data(), uint32_t(object.size()));
        guest::StoreU32(h.base, at + cs::kObjectFlags, flags);
        PPCContext t = CallContext(h);
        t.r3.u64 = at;
        t.r4.u64 = (uint64_t(r4_high) << 32) | r4;
        if (std::strstr(leaf.name, "Key") != nullptr) {
          t.r4.u64 = h.data + 512;
        } else if (std::strstr(leaf.name, "AllocData") != nullptr) {
          t.r3.u64 = at + 4;
          t.r4.u64 = size;
        } else if (std::strstr(leaf.name, "PoolObject (") != nullptr) {
          t.r4.u64 = sizes[0];
          t.r5.u64 = sizes[1];
          t.r6.u64 = sizes[2];
        }
        With(v, [&] {
          if (v == 0) {
            leaf.recompiled(t, h.base);
          } else {
            leaf.native(t, h.base);
          }
        });
        ScratchAndR3(h, out[v], t);
      }
      results.Check(out[0], out[1], fmt::format("case {} flags {:08X} r4 {:08X}", c, flags, r4));
    }
    results.Report();
  }
}

// Writes a pool object at `object` with `vtable` (one of the game's, or the
// scratch one), random bytes elsewhere, a random key at +64 and +84, and +56
// and +60 cleared (AreaModel::Reset's model and XPR).
void RandomObject(Context& h, uint32_t object, uint32_t vtable, Random& r, uint32_t key) {
  for (uint32_t i = 0; i < 96; i += 4) {
    guest::StoreU32(h.base, object + i, uint32_t(r.Next()));
  }
  guest::StoreU32(h.base, object + 0, vtable);
  guest::StoreU32(h.base, object + cs::kObjectFlags, RandomFlags(r));
  guest::StoreU32(h.base, object + 56, 0);
  guest::StoreU32(h.base, object + 60, 0);
  guest::StoreU32(h.base, object + 64, key);
  guest::StoreU32(h.base, object + 84, key);
}

uint32_t PickVtable(Context& h, Random& r, bool fake) {
  switch (r.Below(fake ? 4 : 3)) {
    case 0:
      return kBaseVtable;
    case 1:
      return kLightsVtable;
    case 2:
      return kModelVtable;
    default:
      return h.data + kFakeVtable;
  }
}

void Keys(Context& h) {
  Random& r = h.random;
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "PoolObject::SetKey (sub_820FBA00)"
                               : "PoolObject::KeyEquals (sub_820FBA60)");
    for (int c = 0; c < 64; c++) {
      const uint32_t key = r.Below(4) == 0 ? 0 : uint32_t(r.Next());
      const uint32_t other = r.Below(2) == 0 ? key : key ^ (1u << r.Below(32));
      const uint64_t seed = r.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random object_random;
        for (uint64_t i = 0; i < (seed & 15); i++) {
          object_random.Next();
        }
        Fill(h.base, h.data, kScratchBytes);
        WriteFakeVtable(h);
        const uint32_t object = h.data + 128;
        const uint32_t vtable = PickVtable(h, object_random, true);
        RandomObject(h, object, vtable, object_random, key);
        guest::StoreU32(h.base, h.data + 1024, other);
        PPCContext t = CallContext(h);
        t.r3.u64 = object;
        t.r4.u64 = h.data + 1024;
        With(v, [&] {
          if (v == 0) {
            (which == 0 ? __imp__sub_820FBA00 : __imp__sub_820FBA60)(t, h.base);
          } else {
            WithCalls(t, h.base, [&](const cs::Calls& calls) {
              t.r3.u64 = which == 0 ? cs::SetKey(h.base, calls, t.r1.u32, t.r3.u32, t.r4.u32)
                                    : cs::KeyEquals(h.base, calls, t.r1.u32, t.r3.u32,
                                                    t.r4.u32);
            });
          }
        });
        ScratchAndR3(h, out[v], t);
      }
      results.Check(out[0], out[1], fmt::format("case {} key {:08X} other {:08X}", c, key, other));
    }
    results.Report();
  }
}

// A pool set in the scratch area: the set at +0, its records at +64, two
// pools of up to 8 objects of 96 bytes (no data) from +256, each pool's
// objects split at random between the free and used lists in a random
// order. Returns each pool's object count.
constexpr uint32_t kSet = 0;
constexpr uint32_t kRecords = 64;
constexpr uint32_t kObjects = 256;
constexpr uint32_t kStride = 96;

void RandomPools(Context& h, Random& r, uint32_t counts[2], bool fake_only) {
  const uint32_t set = h.data + kSet;
  guest::StoreU32(h.base, set + 0, 2);
  guest::StoreU32(h.base, set + 4, h.data + kRecords);
  for (uint32_t p = 0; p < 2; p++) {
    const uint32_t record = h.data + kRecords + 24 * p;
    const uint32_t first = h.data + kObjects + p * 8 * kStride;
    const uint32_t n = r.Below(9);
    counts[p] = n;
    guest::StoreU32(h.base, record + 0, n);
    guest::StoreU32(h.base, record + 4, kStride);
    guest::StoreU32(h.base, record + 8, 0);
    guest::StoreU32(h.base, record + 12, first);
    std::vector<uint32_t> order(n);
    for (uint32_t i = 0; i < n; i++) {
      order[i] = i;
    }
    for (uint32_t i = n; i > 1; i--) {
      std::swap(order[i - 1], order[r.Below(i)]);
    }
    const uint32_t used = n == 0 ? 0 : r.Below(n + 1);
    for (uint32_t i = 0; i < n; i++) {
      const uint32_t object = first + kStride * order[i];
      const uint32_t vtable = fake_only ? h.data + kFakeVtable : PickVtable(h, r, false);
      uint32_t key = 0x00010002u;
      if (r.Below(3) != 0) {
        const uint32_t area = r.Below(4);
        key = area << 16 | r.Below(4);
      }
      RandomObject(h, object, vtable, r, key);
      guest::StoreU32(h.base, object + 16, order[i]);
    }
    // The used list (order[0, used)) and the free list (the rest).
    auto link = [&](uint32_t from, uint32_t to, uint32_t head_at) {
      guest::StoreU32(h.base, record + head_at, 0);
      for (uint32_t i = from; i < to; i++) {
        const uint32_t object = first + kStride * order[i];
        guest::StoreU32(h.base, object + 20, i == from ? 0 : first + kStride * order[i - 1]);
        guest::StoreU32(h.base, object + 24, i + 1 == to ? 0 : first + kStride * order[i + 1]);
        if (i == from) {
          guest::StoreU32(h.base, record + head_at, object);
        }
      }
    };
    link(0, used, 20);
    link(used, n, 16);
  }
}

void Pools(Context& h) {
  Random& r = h.random;
  enum Kind { kPool, kObject, kAcquire, kFind, kLoad, kRelease, kKinds };
  const char* names[kKinds] = {
      "ObjectPools::Pool (sub_820FBB10)",    "ObjectPools::Object (sub_820FC0E8)",
      "ObjectPools::Acquire (sub_820FBF08)", "ObjectPools::Find (sub_820FBE80)",
      "ObjectPools::Load (sub_820FBFD0)",    "ObjectPools::Release (sub_820FC038)"};
  for (int kind = 0; kind < kKinds; kind++) {
    Results results(names[kind]);
    for (int c = 0; c < 64; c++) {
      const uint64_t seed = r.Next();
      const uint32_t key_pick = r.Below(5);
      uint32_t counts[2] = {};
      {
        Random probe;
        for (uint64_t i = 0; i < (seed & 31); i++) {
          probe.Next();
        }
        RandomPools(h, probe, counts, kind == kLoad);
      }
      const uint32_t pool = r.Below(2);
      // Release needs an object on the used list: its index is picked from
      // the pool as built.
      uint32_t index = counts[pool] == 0 ? 0 : r.Below(counts[pool]);
      // Load calls through the object's vtable: a pool without objects
      // has none there (case 0 loads object 0 instead, which isn't called).
      if (kind == kLoad && c != 0 && counts[pool] == 0) {
        continue;
      }
      if (kind == kRelease) {
        const uint32_t used = guest::LoadU32(h.base, h.data + kRecords + 24 * pool + 20);
        if (used == 0) {
          continue;
        }
        index = guest::LoadU32(h.base, used + 16);
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random probe;
        for (uint64_t i = 0; i < (seed & 31); i++) {
          probe.Next();
        }
        Fill(h.base, h.data, kScratchBytes);
        WriteFakeVtable(h);
        uint32_t ignored[2];
        RandomPools(h, probe, ignored, kind == kLoad);
        const uint32_t key = h.data + 3072;
        guest::StoreU32(h.base, key, key_pick == 4 ? 0x00010002u : (key_pick << 16) | key_pick);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + kSet;
        t.r4.u64 = pool;
        t.r5.u64 = (kind == kAcquire || kind == kFind) ? key : index;
        // Load on object 0: a pool whose first object is at 0.
        if (kind == kLoad && c == 0) {
          guest::StoreU32(h.base, h.data + kRecords + 24 * pool + 12, 0);
          t.r5.u64 = 0;
        }
        With(v, [&] {
          if (v == 0) {
            const GuestFunction functions[kKinds] = {__imp__sub_820FBB10, __imp__sub_820FC0E8,
                                                     __imp__sub_820FBF08, __imp__sub_820FBE80,
                                                     __imp__sub_820FBFD0, __imp__sub_820FC038};
            functions[kind](t, h.base);
            return;
          }
          WithCalls(t, h.base, [&](const cs::Calls& calls) {
            const uint32_t s = t.r1.u32, a = t.r3.u32, b = t.r4.u32, d = t.r5.u32;
            switch (kind) {
              case kPool:
                t.r3.u64 = cs::PoolRecord(h.base, a, b);
                break;
              case kObject:
                t.r3.u64 = cs::PoolObjectAt(h.base, a, b, d);
                break;
              case kAcquire:
                SetIndex(t, cs::Acquire(h.base, calls, s, a, b, d));
                break;
              case kFind:
                SetIndex(t, cs::Find(h.base, calls, s, a, b, d));
                break;
              case kLoad:
                t.r3.u64 = cs::LoadObject(h.base, calls, s, a, b, d);
                break;
              default:
                t.r3.u64 = cs::Release(h.base, calls, s, a, b, d);
                break;
            }
          });
        });
        ScratchAndR3(h, out[v], t);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {} pool {} counts {} {} index {}", c, pool, counts[0],
                                counts[1], index));
    }
    results.Report();
  }
}

// ObjectPools::Init: one to three pools from descriptors in the scratch area
// (+512), each with 0 to 4 objects of 1 to 200 bytes and as many or more
// data buffers of 0 to 300; pools 0 and 1 now and then with the game's
// factory (Scenery::MakePoolObject, whose pools 0 and 1 are plain pool
// objects with their own vtables). Compared: the scratch area, r3 and what
// Init writes in the block, each with addresses inside the block made
// relative to it.
void PoolsInit(Context& h) {
  Random& r = h.random;
  Results results("ObjectPools::Init (sub_820FBBE8)");
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: ObjectPools::Init: the native heap is off, no cases");
    results.Report();
    return;
  }
  for (int c = 0; c < 32; c++) {
    const uint32_t count = 1 + r.Below(3);
    uint32_t descriptor[3][6] = {};
    uint32_t size = (count * 24 + 63) & ~63u;
    for (uint32_t i = 0; i < count; i++) {
      descriptor[i][0] = i;
      descriptor[i][1] = 1 + r.Below(200);
      descriptor[i][2] = r.Below(300);
      descriptor[i][3] = r.Below(5);
      descriptor[i][4] = descriptor[i][3] + r.Below(2);
      descriptor[i][5] = i < 2 && r.Below(2) == 0 ? 0x820F2638 : 0;
      size += ((descriptor[i][1] + 63) & ~63u) * descriptor[i][3] +
              ((descriptor[i][2] + 63) & ~63u) * descriptor[i][4];
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t i = 0; i < count; i++) {
        for (uint32_t k = 0; k < 6; k++) {
          guest::StoreU32(h.base, h.data + 512 + 24 * i + 4 * k, descriptor[i][k]);
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data + kSet;
      t.r4.u64 = count;
      t.r5.u64 = h.data + 512;
      With(v, [&] {
        if (v == 0) {
          __imp__sub_820FBBE8(t, h.base);
        } else {
          WithCalls(t, h.base, [&](const cs::Calls& calls) {
            uint32_t r3 = 0;
            cs::Init(h.base, calls, t.r1.u32, t.r3.u32, t.r4.u32, t.r5.u32, &r3);
            t.r3.u64 = r3;
          });
        }
      });
      const uint32_t block = guest::LoadU32(h.base, h.data + kSet + 4);
      auto relative = [&](uint32_t word) {
        return word >= block && word < block + size ? word - block : word;
      };
      std::vector<uint8_t> scratch = Read(h.base, h.data, kScratchBytes);
      for (uint32_t i = 0; i + 4 <= kScratchBytes; i += 4) {
        const uint32_t word = guest::LoadU32(h.base, h.data + i);
        const uint32_t value = relative(word);
        for (int b = 0; b < 4; b++) {
          scratch[i + b] = uint8_t(value >> (24 - 8 * b));
        }
      }
      out[v].Bytes(scratch);
      out[v].Word(relative(t.r3.u32));
      // What Init writes in the block: the records and each object's header
      // (40 bytes; 60 with the factory, which also clears +56); the rest is
      // the allocator's.
      std::vector<uint8_t> bytes;
      auto add = [&](uint32_t address, uint32_t length) {
        for (uint32_t i = 0; i < length; i += 4) {
          const uint32_t value = relative(guest::LoadU32(h.base, address + i));
          for (int b = 0; b < 4; b++) {
            bytes.push_back(uint8_t(value >> (24 - 8 * b)));
          }
        }
      };
      add(block, count * 24);
      uint32_t cursor = block + ((count * 24 + 63) & ~63u);
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t object_size = (descriptor[i][1] + 63) & ~63u;
        const uint32_t data_size = (descriptor[i][2] + 63) & ~63u;
        for (uint32_t k = 0; k < descriptor[i][3]; k++) {
          add(cursor + k * (object_size + data_size), descriptor[i][5] != 0 ? 60 : 40);
        }
        cursor += object_size * descriptor[i][3] + data_size * descriptor[i][4];
      }
      out[v].Bytes(bytes);
      memory::FreePhysical(block);
    }
    results.Check(out[0], out[1], fmt::format("case {} pools {} size {}", c, count, size));
  }
  results.Report();
}

// ObjectPools::Shutdown: a set whose records are in a block of physical
// memory made for each version (freed by the function), their objects in
// the scratch area (RandomPools' with the game's vtables). Compared: the
// scratch area and r3 (the block, made 0).
void PoolsShutdown(Context& h) {
  Random& r = h.random;
  Results results("ObjectPools::Shutdown (sub_820FBDC8)");
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: ObjectPools::Shutdown: the native heap is off, no cases");
    results.Report();
    return;
  }
  for (int c = 0; c < 24; c++) {
    const uint64_t seed = r.Next();
    const bool empty = c == 0;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random probe;
      for (uint64_t i = 0; i < (seed & 31); i++) {
        probe.Next();
      }
      Fill(h.base, h.data, kScratchBytes);
      uint32_t counts[2];
      RandomPools(h, probe, counts, false);
      const uint32_t block = memory::AllocPhysical(64, 0, 0xFFFFFFFFu, 64, 4);
      for (uint32_t i = 0; i < 48; i += 4) {
        guest::StoreU32(h.base, block + i, guest::LoadU32(h.base, h.data + kRecords + i));
      }
      guest::StoreU32(h.base, h.data + kSet + 4, block);
      if (empty) {
        guest::StoreU32(h.base, h.data + kSet + 0, 0);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data + kSet;
      With(v, [&] {
        if (v == 0) {
          __imp__sub_820FBDC8(t, h.base);
        } else {
          WithCalls(t, h.base, [&](const cs::Calls& calls) {
            uint32_t r3 = 0;
            cs::Shutdown(h.base, calls, t.r1.u32, t.r3.u32, &r3);
            t.r3.u64 = r3;
          });
        }
      });
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32 == block ? 0 : t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// --- The scenery's lists ---

void ModelAssets(Context& h) {
  Random& r = h.random;
  Results results("SceneryConfig::ModelAsset (sub_820F1530)");
  for (int c = 0; c < 96; c++) {
    const uint32_t areas = r.Below(8);
    uint32_t entries[8][2];
    for (auto& entry : entries) {
      switch (r.Below(6)) {
        case 0:
          entry[0] = r.Below(100);
          entry[1] = entry[0] - 1 - r.Below(3);  // first above last
          break;
        case 1:
          entry[0] = r.Below(2) == 0 ? 0 : 0xFFFFFFF0u + r.Below(16);
          entry[1] = 0xFFFFFFFFu;
          break;
        default:
          entry[0] = r.Below(100);
          entry[1] = entry[0] + r.Below(12);
          break;
      }
    }
    const uint32_t area = r.Below(10);
    const uint32_t model = r.Below(8) == 0 ? 0xFFFFFFF0u + r.Below(16) : r.Below(16);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      const uint32_t config = h.data, table = h.data + 256;
      guest::StoreU32(h.base, config + 28, table);
      guest::StoreU32(h.base, table + 16, areas);
      for (uint32_t i = 0; i < 8; i++) {
        guest::StoreU32(h.base, table + 24 + 12 * i, entries[i][0]);
        guest::StoreU32(h.base, table + 28 + 12 * i, entries[i][1]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = config;
      t.r4.u64 = area;
      t.r5.u64 = model;
      With(v, [&] {
        if (v == 0) {
          __imp__sub_820F1530(t, h.base);
        } else {
          SetIndex(t, cs::ModelAsset(h.base, t.r3.u32, t.r4.u32, t.r5.u32));
        }
      });
      ScratchAndR3(h, out[v], t);
    }
    results.Check(out[0], out[1], fmt::format("case {} area {} model {:08X}", c, area, model));
  }
  results.Report();
}

// A list in the scratch area: the list at +0 (28 bytes), its pointer array
// at +64 (up to 64 items), its items at +512 (16 bytes each), filled in a
// random order; areas and models below 6 (so keys repeat).
constexpr uint32_t kList = 0;
constexpr uint32_t kPointers = 64;
constexpr uint32_t kItems = 512;
constexpr uint32_t kMaxItems = 64;

struct ListInput {
  uint32_t count;
  uint32_t order[kMaxItems];
  uint32_t item[kMaxItems][4];
};

ListInput RandomList(Random& r, uint32_t count, uint32_t areas) {
  ListInput in{};
  in.count = count;
  for (uint32_t i = 0; i < kMaxItems; i++) {
    in.order[i] = i;
  }
  for (uint32_t i = kMaxItems; i > 1; i--) {
    std::swap(in.order[i - 1], in.order[r.Below(i)]);
  }
  for (auto& item : in.item) {
    item[0] = RandomFloatBits(r);
    item[1] = r.Below(4) == 0 ? uint32_t(r.Next()) : 1u << r.Below(14);
    item[2] = (r.Below(areas) << 16) | r.Below(6);
    item[3] = uint32_t(r.Next());
  }
  return in;
}

void WriteList(Context& h, const ListInput& in) {
  const uint32_t list = h.data + kList;
  guest::StoreU32(h.base, list + 4, 0);
  guest::StoreU32(h.base, list + 8, in.count);
  guest::StoreU32(h.base, list + 16, h.data + kPointers);
  guest::StoreU32(h.base, list + 20, h.data + kItems);
  for (uint32_t i = 0; i < kMaxItems; i++) {
    const uint32_t item = h.data + kItems + 16 * i;
    for (uint32_t k = 0; k < 4; k++) {
      guest::StoreU32(h.base, item + 4 * k, in.item[i][k]);
    }
    guest::StoreU32(h.base, h.data + kPointers + 4 * i, h.data + kItems + 16 * in.order[i]);
  }
}

// The scratch area with the items' distances as floats.
void ListOutputs(Context& h, Outputs& out, const PPCContext& t) {
  out.Bytes(Read(h.base, h.data, kItems));
  for (uint32_t i = 0; i < kMaxItems; i++) {
    const uint32_t item = h.data + kItems + 16 * i;
    out.Floats(h.base, item, 1);
    out.Bytes(Read(h.base, item + 4, 12));
  }
  const uint32_t end = kItems + 16 * kMaxItems;
  out.Bytes(Read(h.base, h.data + end, kScratchBytes - end));
  out.Word(t.r3.u32);
}

void Lists(Context& h) {
  Random& r = h.random;
  {
    Results results("SceneryList::Clear (sub_820F1588)");
    for (int c = 0; c < 8; c++) {
      const ListInput in = RandomList(r, r.Below(kMaxItems), 6);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        WriteList(h, in);
        guest::StoreU32(h.base, h.data + kList + 4, 7);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 4000;
        t.r4.u64 = h.data + kList;
        With(v, [&] {
          if (v == 0) {
            __imp__sub_820F1588(t, h.base);
          } else {
            cs::ListClear(h.base, t.r4.u32);
          }
        });
        ListOutputs(h, out[v], t);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneryList::Add (sub_820F1888)");
    for (int c = 0; c < 128; c++) {
      const uint32_t count = c < 4 ? 0 : r.Below(kMaxItems - 1);
      const ListInput in = RandomList(r, count, 6);
      const uint32_t area = r.Below(10) == 0 ? 0x10000u + r.Below(6) : r.Below(7);
      const uint32_t model = r.Below(10) == 0 ? 0x10000u + r.Below(6) : r.Below(7);
      const uint32_t mask = uint32_t(r.Next());
      double distance = double(std::bit_cast<float>(RandomFloatBits(r)));
      if (r.Below(8) == 0) {
        distance = double(r.Below(100000)) / 7.0;  // not a float
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        WriteList(h, in);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 4000;
        t.r4.u64 = h.data + kList;
        t.f1.f64 = distance;
        t.r6.u64 = area;
        t.r7.u64 = model;
        t.r8.u64 = mask;
        With(v, [&] {
          if (v == 0) {
            __imp__sub_820F1888(t, h.base);
          } else {
            t.r3.u64 = cs::ListAdd(h.base, t.r4.u32, t.f1.f64, t.r6.u32, t.r7.u32, t.r8.u32,
                                   t.r3.u32);
          }
        });
        ListOutputs(h, out[v], t);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {} count {} area {:X} model {:X} distance {}", c, count,
                                area, model, distance));
    }
    results.Report();
  }
  {
    Results results("SceneryList::Sort (sub_820F1930)");
    for (int c = 0; c < 64; c++) {
      const uint32_t count = 1 + r.Below(kMaxItems);
      ListInput in = RandomList(r, count, 6);
      // Now and then few distinct distances, or none but finite ones.
      for (uint32_t i = 0; i < kMaxItems; i++) {
        if (c % 3 == 1) {
          in.item[i][0] = std::bit_cast<uint32_t>(float(r.Below(4)));
        } else if (c % 3 == 2 && std::isnan(std::bit_cast<float>(in.item[i][0]))) {
          in.item[i][0] = 0;
        }
      }
      const int32_t first = int32_t(r.Below(count));
      const int32_t last = c < 8 ? int32_t(count) - 1 : first + int32_t(r.Below(count - first));
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        WriteList(h, in);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 4000;
        t.r4.u64 = h.data + kList;
        t.r5.s64 = first;
        t.r6.s64 = last;
        With(v, [&] {
          if (v == 0) {
            __imp__sub_820F1930(t, h.base);
          } else {
            cs::ListSort(h.base, t.r4.u32, t.r5.s32, t.r6.s32);
          }
        });
        ListOutputs(h, out[v], t);
      }
      results.Check(out[0], out[1], fmt::format("case {} items {} to {}", c, first, last));
    }
    results.Report();
  }
  {
    // The manager in the scratch area at +2048 (list B at its +144 is put
    // at the scratch list: the manager's +144 is the list itself, so the
    // manager is at the list - 144).
    Results results("Scenery::LimitLargeModels (sub_820F2B80)");
    for (int c = 0; c < 96; c++) {
      const uint32_t count = r.Below(kMaxItems);
      // Areas 0-4 mostly; now and then up to 9 (past the table).
      const ListInput in = RandomList(r, count, c % 4 == 3 ? 10 : 5);
      ListInput large = in;
      for (auto& item : large.item) {
        // Models around the thresholds (9 to 54).
        item[2] = (item[2] & 0xFFFF0000u) | r.Below(60);
      }
      const uint32_t cap = r.Below(4) == 0 ? 0 : r.Below(count + 2);
      const uint32_t views = r.Below(3);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        WriteList(h, large);
        const uint32_t manager = h.data + 1600;
        // The manager's +144 is list B: the scratch list's words copied there.
        for (uint32_t i = 0; i < 28; i += 4) {
          guest::StoreU32(h.base, manager + 144 + i, guest::LoadU32(h.base, h.data + kList + i));
        }
        guest::StoreU32(h.base, manager + 12, views);
        guest::StoreU32(h.base, manager + 104, cap);
        PPCContext t = CallContext(h);
        t.r3.u64 = manager;
        With(v, [&] {
          if (v == 0) {
            __imp__sub_820F2B80(t, h.base);
          } else {
            t.r12.u64 = t.lr;
            __savegprlr_27(t, h.base);
            cs::LimitLargeModels(h.base, t.r3.u32, t.r1.u32);
            t.r3.u64 = t.r3.u64 + 100;
          }
        });
        ListOutputs(h, out[v], t);
        out[v].Bytes(Read(h.base, t.r1.u32 - 80, 80));
      }
      results.Check(out[0], out[1],
                    fmt::format("case {} count {} cap {} views {}", c, count, cap, views));
    }
    results.Report();
  }
  {
    Results results("ModelList::Init (sub_820F1A20)");
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      const uint64_t seed = r.Next();
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes, uint8_t(seed));
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 16 + 4 * (seed & 7);
        With(v, [&] {
          if (v == 0) {
            __imp__sub_820F1A20(t, h.base);
          } else {
            cs::ModelListInit(h.base, t.r3.u32);
          }
        });
        ScratchAndR3(h, out[v], t);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The game's own global, put back after.
    Results results("Scenery::ClearGlobal (sub_820F1480)");
    const uint32_t saved = guest::LoadU32(h.base, cs::kManager);
    for (int c = 0; c < 2; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32(h.base, cs::kManager, 0x12345678u + c);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data;
        With(v, [&] {
          if (v == 0) {
            __imp__sub_820F1480(t, h.base);
          } else {
            cs::ClearGlobal(h.base);
          }
        });
        out[v].Word(guest::LoadU32(h.base, cs::kManager));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    guest::StoreU32(h.base, cs::kManager, saved);
    results.Report();
  }
}

}  // namespace

void CourseScenery(Context& h) {
  ObjectLeaves(h);
  Keys(h);
  Pools(h);
  PoolsInit(h);
  PoolsShutdown(h);
  ModelAssets(h);
  Lists(h);
}

}  // namespace harness
