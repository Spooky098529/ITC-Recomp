// The action mappers (src/action_mappers/, docs/functions/action-mappers.md)
// against the recompiled functions. The objects (repeat and curve objects,
// listeners, the client list and its nodes) are built in the scratch area,
// its other bytes left at the fill, and the whole area is compared after
// each case: floats where the function stores floats, bytes everywhere
// else, then r3 (and f1 where it returns a float). The originals run with
// --native_action_mappers off, so their callees are the recompiled code
// too. The client list's global ([0x82970C14]) is pointed at the scratch
// area's list for a case, compared, and put back.

#include "harness.h"

#include "action_mappers/action_mappers.h"
#include "base/flags.h"
#include "base/log.h"

#include <fmt/format.h>

#include <bit>
#include <cstring>
#include <limits>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_action_mappers);

namespace harness {

namespace {

namespace am = action_mappers;

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeCurveApply, ctx.f1.f64 = am::CurveApply(base, ctx.r3.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeRepeatUpdate,
                ctx.r3.u64 = am::RepeatUpdate(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeIndexOf, ctx.r3.u64 = am::IndexOf(ctx.r4.u32))
ITC_NATIVE_STEP(NativeListenerHeld,
                ctx.r3.u64 = am::ListenerHeld(base, ctx.r3.u32,
                                              guest::LoadU32(base, ctx.r4.u32)))
ITC_NATIVE_STEP(NativeListenerTest, ctx.r3.u64 = am::ListenerTest(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeClientNodeConstruct, am::ClientNodeConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeClientListConstruct, am::ClientListConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClientListAdd, am::ClientListAdd(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClientListCount,
                ctx.r3.u64 = am::ClientListCount(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeClientListBoundCount,
                ctx.r3.u64 = am::ClientListBoundCount(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeClientListFind,
                ctx.r3.u64 = am::ClientListFind(base, ctx.r3.u32, ctx.r4.u32))

#undef ITC_NATIVE_STEP

// Byte ranges of the scratch area holding floats, [from, to).
using FloatRanges = std::vector<std::pair<uint32_t, uint32_t>>;

struct Options {
  FloatRanges floats;
  bool f1 = false;
  bool r3 = true;
};

// One case through both versions: the area filled, `setup()` writes the
// inputs (the same both times: it's handed a copy of the generator's state),
// `args(t)` sets the registers, the original (flag off) or the native step
// runs, and the area, the client list's global, r3 and f1 are compared. The
// global is put back afterwards.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, const Options& options, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  const uint32_t list_global = guest::LoadU32(h.base, am::kClientList);
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_action_mappers) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_action_mappers) = true;
    uint32_t at = 0;
    for (const auto& [from, to] : options.floats) {
      if (from > at) {
        out[v].Bytes(Read(h.base, h.data + at, from - at));
      }
      out[v].Floats(h.base, h.data + from, (to - from) / 4);
      at = to;
    }
    out[v].Bytes(Read(h.base, h.data + at, kScratchBytes - at));
    out[v].Word(guest::LoadU32(h.base, am::kClientList));
    guest::StoreU32(h.base, am::kClientList, list_global);
    if (options.r3) {
      out[v].Word(t.r3.u32);
    }
    if (options.f1) {
      out[v].Double(t.f1.u64);
    }
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

uint32_t Bits(float f) {
  return std::bit_cast<uint32_t>(f);
}

// A float: mostly in [-range, range], sometimes an edge value.
float RandomFloat(Random& r, float range) {
  static const float kEdges[] = {0.0f,
                                 -0.0f,
                                 1.0f,
                                 -1.0f,
                                 0.5f,
                                 -0.5f,
                                 0.25f,
                                 std::numeric_limits<float>::infinity(),
                                 -std::numeric_limits<float>::infinity(),
                                 std::numeric_limits<float>::quiet_NaN(),
                                 -std::numeric_limits<float>::quiet_NaN(),
                                 std::numeric_limits<float>::denorm_min(),
                                 std::numeric_limits<float>::max(),
                                 1e-30f};
  if (r.Below(6) == 0) {
    return kEdges[r.Below(uint32_t(std::size(kEdges)))];
  }
  return (float(r.Below(2000001)) / 1000000.0f - 1.0f) * range;
}

// --- The curves ---

void Curves(Context& h) {
  Results results("sub_8210B5B8 ActionCurve::Apply");
  const uint32_t curve = h.data + 64;
  for (int i = 0; i < 600; i++) {
    // The game's settings (center 0, dead zone up to 0.3, maximum 1), then
    // random ones.
    const bool game = i < 300;
    Case(
        h, results,
        [&] {
          Random& r = h.random;
          guest::StoreU32(h.base, curve, 0x82007F70);
          const float center = game ? 0.0f : RandomFloat(r, 1.0f);
          const float dead = game ? float(r.Below(31)) / 100.0f : RandomFloat(r, 0.5f);
          const float maximum = game ? 1.0f : RandomFloat(r, 2.0f);
          guest::StoreU32(h.base, curve + 4, Bits(center));
          guest::StoreU32(h.base, curve + 8, Bits(dead));
          guest::StoreU32(h.base, curve + 12, Bits(maximum));
          guest::StoreU32(h.base, h.data, Bits(RandomFloat(r, 1.5f)));
        },
        [&](PPCContext& t) {
          t.r3.u64 = curve;
          t.f1.f64 = double(guest::LoadFloat(h.base, h.data));
        },
        __imp__sub_8210B5B8, NativeCurveApply, {{{curve + 4 - h.data, curve + 16 - h.data}}, true},
        fmt::format("case {}", i));
  }
  results.Report();
}

// --- The repeats ---

void Repeats(Context& h) {
  Results results("sub_8210B568 ActionRepeat::Update");
  const uint32_t repeat = h.data + 64;
  for (int i = 0; i < 600; i++) {
    Case(
        h, results,
        [&] {
          Random& r = h.random;
          // The game's delay and period (20 and 3 or the settings'), the
          // count around them, then anything.
          const uint32_t delay = i < 400 ? 1 + r.Below(25) : uint32_t(r.Next());
          const uint32_t period = i < 400 ? 1 + r.Below(10) : uint32_t(r.Next());
          uint32_t count = r.Below(40);
          if (i % 4 == 0) {
            count = delay + period;
          } else if (i % 4 == 1) {
            count = delay + r.Below(3) - 1;
          } else if (i >= 400) {
            count = uint32_t(r.Next());
          }
          guest::StoreU32(h.base, repeat, 0x82007F68);
          guest::StoreU32(h.base, repeat + 4, count);
          guest::StoreU32(h.base, repeat + 8, delay);
          guest::StoreU32(h.base, repeat + 12, period);
          guest::StoreU32(h.base, h.data, uint32_t(r.Next()));
        },
        [&](PPCContext& t) {
          t.r3.u64 = repeat;
          const uint32_t held = guest::LoadU32(h.base, h.data);
          // Held 1, not held 0, or a word whose low byte decides.
          t.r4.u64 = i % 3 == 0 ? 1 : i % 3 == 1 ? 0 : (uint64_t(held) << 32 | held);
        },
        __imp__sub_8210B568, NativeRepeatUpdate, {}, fmt::format("case {}", i));
  }
  results.Report();
}

// --- IndexOf ---

void Indexes(Context& h) {
  Results results("sub_8210B140 ActionMapper::IndexOf");
  std::vector<uint64_t> ids;
  for (uint64_t id = 0; id < 400; id++) {
    ids.push_back(id);
  }
  for (uint64_t id : {0xFFFFFFFFull, 0x80000000ull, 0x100000097ull, 0xFFFFFFFF00000097ull,
                      151ull + 0x80000000ull}) {
    ids.push_back(id);
  }
  for (int i = 0; i < 100; i++) {
    ids.push_back(h.random.Next());
  }
  for (uint64_t id : ids) {
    Case(
        h, results, [] {}, [&](PPCContext& t) { t.r4.u64 = id; }, __imp__sub_8210B140,
        NativeIndexOf, {}, fmt::format("id {:X}", id));
  }
  results.Report();
}

// --- The held tests ---

void HeldTests(Context& h) {
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "sub_825924E0 InputProcessor::AllHeld"
                               : "sub_8258CF10 InputProcessor::TestHeld");
    const uint32_t listener = h.data + 64;
    const uint32_t mask = h.data + 32;
    for (int i = 0; i < 400; i++) {
      Case(
          h, results,
          [&] {
            Random& r = h.random;
            // The mappers' masks (one bit), then any.
            const uint32_t m = i < 200 ? 1u << r.Below(32) : uint32_t(r.Next());
            uint32_t held = uint32_t(r.Next());
            if (i % 3 == 0) {
              held |= m;
            } else if (i % 7 == 0) {
              held = 0;
            }
            guest::StoreU32(h.base, mask, i % 50 == 0 ? 0 : m);
            guest::StoreU32(h.base, listener + 272, held);
          },
          [&](PPCContext& t) {
            t.r3.u64 = listener;
            t.r4.u64 = mask;
          },
          which == 0 ? __imp__sub_825924E0 : __imp__sub_8258CF10,
          which == 0 ? NativeListenerHeld : NativeListenerTest, {}, fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- The client list ---

// A list at `list` with `count` nodes (12 bytes each from `nodes`) of
// random types 0-3 linked in order, its counts random; the global pointed
// at it (or 0 for no list).
void RandomList(Context& h, uint32_t list, uint32_t nodes, uint32_t count, bool global) {
  Random& r = h.random;
  guest::StoreU32(h.base, list, 0x820713C0);
  guest::StoreU32(h.base, list + 4, count ? nodes : 0);
  guest::StoreU32(h.base, list + 8, count ? nodes + 12 * (count - 1) : 0);
  for (uint32_t k = 0; k < 6; k++) {
    guest::StoreU32(h.base, list + 12 + 4 * k, r.Below(10));
  }
  for (uint32_t k = 0; k < count; k++) {
    const uint32_t node = nodes + 12 * k;
    guest::StoreU32(h.base, node, 0x82071380);
    guest::StoreU32(h.base, node + 4, r.Below(4));
    guest::StoreU32(h.base, node + 8, k + 1 < count ? node + 12 : 0);
  }
  guest::StoreU32(h.base, am::kClientList, global ? list : 0);
}

void ClientLists(Context& h) {
  const uint32_t list = h.data + 64;
  const uint32_t nodes = h.data + 128;
  const uint32_t added = h.data + 1024;
  {
    Results results("sub_8259C6D0 ClientNode::ClientNode");
    for (uint32_t type : {0u, 1u, 2u, 3u, 0xFFFFFFFFu}) {
      Case(
          h, results, [] {},
          [&](PPCContext& t) {
            t.r3.u64 = nodes;
            t.r4.u64 = type;
          },
          __imp__sub_8259C6D0, NativeClientNodeConstruct, {}, fmt::format("type {}", type));
    }
    results.Report();
  }
  {
    Results results("sub_8259C7B8 ClientList::ClientList");
    for (int i = 0; i < 4; i++) {
      Case(
          h, results, [&] { RandomList(h, list, nodes, i, i % 2); },
          [&](PPCContext& t) { t.r3.u64 = list + 4 * i; }, __imp__sub_8259C7B8,
          NativeClientListConstruct, {}, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_8259C960 ClientList::Add");
    for (int i = 0; i < 60; i++) {
      const uint32_t count = i % 6;
      Case(
          h, results,
          [&] {
            RandomList(h, list, nodes, count, i % 10 != 9);
            guest::StoreU32(h.base, added, 0x82071380);
            guest::StoreU32(h.base, added + 4, h.random.Below(3));
            guest::StoreU32(h.base, added + 8, 0);
          },
          [&](PPCContext& t) { t.r3.u64 = added; }, __imp__sub_8259C960, NativeClientListAdd,
          {}, fmt::format("case {} ({} nodes)", i, count));
    }
    results.Report();
  }
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "sub_8259C868 ClientList::Count"
                               : "sub_8259C898 ClientList::BoundCount");
    for (int i = 0; i < 60; i++) {
      Case(
          h, results, [&] { RandomList(h, list, nodes, 0, true); },
          [&](PPCContext& t) {
            t.r3.u64 = list;
            t.r4.u64 = i % 3;
            t.r5.u64 = i % 4 == 0 ? 0 : i % 4 == 1 ? 1 : i % 4 == 2 ? 0x100 : 0xFFFFFF01;
          },
          which == 0 ? __imp__sub_8259C868 : __imp__sub_8259C898,
          which == 0 ? NativeClientListCount : NativeClientListBoundCount, {},
          fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_8259C818 ClientList::Find");
    for (int i = 0; i < 200; i++) {
      const uint32_t count = i % 9;
      uint32_t type = 0, n = 0;
      Case(
          h, results,
          [&] {
            RandomList(h, list, nodes, count, i % 20 != 19);
            type = i % 50 == 7 ? 0xFFFFFFFF : h.random.Below(4);
            n = h.random.Below(5);
          },
          [&](PPCContext& t) {
            t.r3.u64 = type;
            t.r4.u64 = n;
          },
          __imp__sub_8259C818, NativeClientListFind, {}, fmt::format("case {} ({} nodes)", i, count));
    }
    results.Report();
  }
}

}  // namespace

void ActionMappers(Context& h) {
  Curves(h);
  Repeats(h);
  Indexes(h);
  HeldTests(h);
  ClientLists(h);
}

}  // namespace harness
