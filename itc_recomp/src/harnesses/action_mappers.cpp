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

ITC_NATIVE_STEP(NativeCurveUnsigned,
                ctx.f1.f64 = am::CurveUnsigned(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
                ctx.r3.u64 = guest::LoadU32(base, ctx.r3.u32 + 4 * (ctx.r4.u32 + 45)))
ITC_NATIVE_STEP(NativeCurveCentered,
                ctx.f1.f64 = am::CurveCentered(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
                ctx.r3.u64 = guest::LoadU32(base, ctx.r3.u32 + 4 * (ctx.r4.u32 + 45)))
ITC_NATIVE_STEP(NativeAnalog, double f1 = ctx.f1.f64;
                ctx.r3.u64 = am::Analog(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, &f1);
                ctx.f1.f64 = f1)
ITC_NATIVE_STEP(NativeHysteresis, ctx.r3.u64 = am::Hysteresis(base, ctx.r3.u32, ctx.r4.u32,
                                                               ctx.f1.f64, ctx.r6.u32))
ITC_NATIVE_STEP(NativeWords,
                ctx.r3.u64 = am::Words(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                       guest::LoadU32(base, ctx.r1.u32 - 64)))
ITC_NATIVE_STEP(NativeRebuild, double f1 = 0;
                ctx.r3.u64 = am::Rebuild(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                         uint32_t(ctx.r25.u64 >> 32), &f1);
                ctx.f1.f64 = f1)
ITC_NATIVE_STEP(NativeMapperUpdate, double f1 = 0;
                ctx.r3.u64 = am::MapperUpdate(base, ctx.r3.u32, uint32_t(ctx.r25.u64 >> 32),
                                              &f1);
                ctx.f1.f64 = f1)
ITC_NATIVE_STEP(NativeUpdateAll, double f1 = ctx.f1.f64;
                ctx.r3.u64 = am::UpdateAll(base, ctx.r3.u32, uint32_t(ctx.r25.u64 >> 32), &f1);
                ctx.f1.f64 = f1)

ITC_NATIVE_STEP(NativeHeld, ctx.r3.u64 = am::Held(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativePressed, ctx.r3.u64 = am::Pressed(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeFired, ctx.r3.u64 = am::Fired(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeAnalogRaw, ctx.r3.u64 = am::AnalogRaw(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSetThreshold,
                ctx.r3.u64 = am::SetThreshold(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64,
                                              ctx.f2.f64, ctx.r7.u32))
ITC_NATIVE_STEP(NativeSetRepeat, ctx.r3.u64 = am::SetRepeat(base, ctx.r3.u32, ctx.r4.u32,
                                                             ctx.r5.u32, ctx.r6.u32))
ITC_NATIVE_STEP(NativeSetCurve, ctx.r3.u64 = am::SetCurve(base, ctx.r3.u32, ctx.r4.u32,
                                                           ctx.f1.f64, ctx.f2.f64, ctx.f3.f64))
ITC_NATIVE_STEP(NativeResetWords, ctx.r3.u64 = am::ResetWords(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeResetAnalog, ctx.r3.u64 = am::ResetAnalog(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeInitRepeats, ctx.r3.u64 = am::InitRepeats(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeInitCurves, ctx.r3.u64 = am::InitCurves(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeResetStages, ctx.r3.u64 = am::ResetStages(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeMapperConstruct, am::MapperConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeInputClientListConstruct,
                am::InputClientListConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeInputClientConstruct, am::InputClientConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeInputClientListAdd, am::InputClientListAdd(base, ctx.r4.u32);
                ctx.r3.u64 = ctx.r4.u32)

void NativeConfigure(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f[3] = {ctx.f1.f64, ctx.f2.f64, ctx.f3.f64};
  ctx.r3.u64 = am::Configure(base, ctx.r3.u32, f);
  ctx.f1.f64 = f[0];
  ctx.f2.f64 = f[1];
  ctx.f3.f64 = f[2];
}

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

// --- The update ---

// A mapper in the scratch area: the game's mapper 0 copied (its pointers
// into itself moved), its state made random. `game` keeps the game's
// configuration (kinds, links, thresholds, delays, curves); otherwise those
// are random too. The logical pad's values (40 floats) at `pad`.
constexpr uint32_t kMapper = 16;    // the mapper's offset in the area
constexpr uint32_t kPad = 2928;     // the pad values'
constexpr uint32_t kStates = 3100;  // a states word (sub_8210BBA0's r5)
constexpr uint32_t kList = 3200;    // a client list

std::vector<uint8_t> g_game_mapper;
uint32_t g_game_mapper_address = 0;

bool GameMapper(Context& h) {
  if (!g_game_mapper.empty()) {
    return true;
  }
  g_game_mapper_address = guest::LoadU32(h.base, 0x826BAB80);
  if (g_game_mapper_address == 0) {
    return false;
  }
  g_game_mapper = Read(h.base, g_game_mapper_address, 2896);
  return true;
}

void RandomMapper(Context& h, bool game) {
  Random& r = h.random;
  uint8_t* base = h.base;
  const uint32_t m = h.data + kMapper;
  Write(base, m, g_game_mapper.data(), 2896);
  const uint32_t delta = m - g_game_mapper_address;
  auto move = [&](uint32_t field) {
    guest::StoreU32(base, m + field, guest::LoadU32(base, m + field) + delta);
  };
  for (uint32_t k = 0; k < 4; k++) {
    move(am::kConfig + 4 * k);
  }
  for (uint32_t i = 0; i < 30; i++) {
    move(am::kRepeatPointers + 4 * i);
    move(am::kCurvePointers + 4 * i);
  }
  guest::StoreU32(base, m + am::kPadValues, h.data + kPad);
  // The client node: type 2, no next.
  guest::StoreU32(base, m + 4, 2);
  guest::StoreU32(base, m + 8, 0);
  // The words (held, held before, pressed, released, fired, pending).
  for (uint32_t k = 0; k < 6; k++) {
    guest::StoreU32(base, m + am::kWords + 4 * k, uint32_t(r.Next()));
  }
  if (!game) {
    for (uint32_t i = 0; i < 30; i++) {
      guest::StoreU32(base, m + 12 + 4 * i, r.Below(3));                    // kinds
      guest::StoreU32(base, m + 132 + 4 * i, r.Below(4));                   // analog kinds
      guest::StoreU32(base, m + 252 + 4 * i, r.Below(4) ? 30 : r.Below(31));  // links
    }
  }
  for (uint32_t i = 0; i < 30; i++) {
    // The raw and analog values before (the update overwrites most).
    guest::StoreU16(base, m + am::kRaw + 2 * i, uint16_t(r.Next()));
    guest::StoreU16(base, m + am::kAnalog + 2 * i, uint16_t(r.Next()));
    guest::StoreU32(base, m + am::kAnalog + 60 + 4 * i, Bits(RandomFloat(r, 1.0f)));
    // The threshold entry: on, off, suppress ticks, counter, state.
    const uint32_t e = m + am::kThresholds + 20 * i;
    if (!game) {
      guest::StoreU32(base, e, Bits(RandomFloat(r, 1.0f)));
      guest::StoreU32(base, e + 4, r.Below(4) ? Bits(RandomFloat(r, 1.0f))
                                              : guest::LoadU32(base, e));
      guest::StoreU32(base, e + 8, r.Below(4));
    }
    guest::StoreU32(base, e + 12, r.Below(3) ? 0 : r.Below(4));
    guest::StoreU8(base, e + 16, uint8_t(r.Below(2)));
    // The repeat object: its count, and (not game) its delay and period.
    const uint32_t repeat = m + 592 + 16 * i;
    if (!game) {
      guest::StoreU32(base, repeat + 8, r.Below(25));
      guest::StoreU32(base, repeat + 12, r.Below(10));
    }
    const uint32_t delay = guest::LoadU32(base, repeat + 8);
    const uint32_t period = guest::LoadU32(base, repeat + 12);
    guest::StoreU32(base, repeat + 4,
                    r.Below(3) == 0 ? delay + period : r.Below(delay + period + 3));
    // The curve object (not game): center, dead zone, maximum.
    if (!game) {
      const uint32_t curve = m + 1372 + 16 * i;
      guest::StoreU32(base, curve + 4, Bits(RandomFloat(r, 0.5f)));
      guest::StoreU32(base, curve + 8, Bits(RandomFloat(r, 0.5f)));
      guest::StoreU32(base, curve + 12, Bits(r.Below(4) ? 1.0f : RandomFloat(r, 2.0f)));
    }
  }
  // The listener's held buttons: mostly a few of the mappers' masks.
  uint32_t held = 0;
  if (r.Below(8) == 0) {
    held = uint32_t(r.Next());
  } else {
    static const uint32_t kMasks[] = {1,    2,     4,     8,      16,      32,      64,
                                      128,  4096,  65536, 16384,  262144,  1048576, 2097152};
    for (uint32_t k = r.Below(4); k > 0; k--) {
      held |= kMasks[r.Below(14)];
    }
  }
  guest::StoreU32(base, m + am::kListener + 272, held);
  // The pad's values: sticks and triggers in [-1, 1], sometimes an edge.
  for (uint32_t k = 0; k < 40; k++) {
    guest::StoreU32(base, h.data + kPad + 4 * k, Bits(RandomFloat(r, 1.0f)));
  }
  guest::StoreU32(base, h.data + kStates, uint32_t(r.Next()));
}

const Options& UpdateOptions() {
  // The analog floats (+1132 to +1252) as floats; r3 and f1.
  static const Options options{
      {{kMapper + am::kAnalog + 60, kMapper + am::kAnalog + 180}}, true, true};
  return options;
}

void Updates(Context& h) {
  if (!GameMapper(h)) {
    ITC_LOG_INFO("harness: action mappers: no mapper at [0x826BAB80], the update not checked");
    return;
  }
  const uint32_t m = h.data + kMapper;
  const Options& options = UpdateOptions();
  auto run = [&](const char* name, int cases, GuestFunction recompiled, GuestFunction native,
                 auto args) {
    Results results(name);
    for (int i = 0; i < cases; i++) {
      const bool game = i < cases / 2;
      Case(
          h, results, [&] { RandomMapper(h, game); },
          [&](PPCContext& t) {
            t.r25.u64 = h.random.Next();
            args(t, i);
          },
          recompiled, native, options, fmt::format("case {}{}", i, game ? " (game's)" : ""));
    }
    results.Report();
  };
  run("sub_8210B4A8 ActionMapper::CurveUnsigned", 300, __imp__sub_8210B4A8, NativeCurveUnsigned,
      [&](PPCContext& t, int i) {
        t.r3.u64 = m + am::kAnalog;
        t.r4.u64 = i % 30;
        t.r5.u64 = h.random.Next();
      });
  run("sub_8210B500 ActionMapper::CurveCentered", 300, __imp__sub_8210B500, NativeCurveCentered,
      [&](PPCContext& t, int i) {
        t.r3.u64 = m + am::kAnalog;
        t.r4.u64 = i % 30;
        t.r5.u64 = i % 5 == 0 ? 0x7FFF + h.random.Below(3) : h.random.Next();
      });
  run("sub_8210B7D0 ActionMapper::Analog", 200, __imp__sub_8210B7D0, NativeAnalog,
      [&](PPCContext& t, int) {
        t.r3.u64 = m + am::kAnalog;
        t.r4.u64 = m + am::kConfig + 8;
        t.r5.u64 = m + am::kRaw;
      });
  run("sub_8210B718 ActionMapper::Hysteresis", 600, __imp__sub_8210B718, NativeHysteresis,
      [&](PPCContext& t, int i) {
        t.r3.u64 = m + am::kWords;
        t.r4.u64 = i % 30;
        t.f1.f64 = double(RandomFloat(h.random, 1.0f));
        t.r5.u64 = h.random.Next();
        t.r6.u64 = m + am::kConfig;
      });
  run("sub_8210BBA0 ActionMapper::Words", 300, __imp__sub_8210BBA0, NativeWords,
      [&](PPCContext& t, int) {
        t.r3.u64 = m + am::kWords;
        t.r4.u64 = m + am::kConfig;
        t.r5.u64 = h.data + kStates;
        // The stack word the held word is built in.
        guest::StoreU32(h.base, t.r1.u32 - 64, uint32_t(h.random.Next()));
      });
  run("sub_8210C0E0 ActionMapper::Rebuild", 300, __imp__sub_8210C0E0, NativeRebuild,
      [&](PPCContext& t, int) {
        t.r3.u64 = m + am::kWords;
        t.r4.u64 = m + am::kConfig;
        t.r5.u64 = m + am::kRaw;
      });
  run("sub_8210C370 ActionMapper::Update", 400, __imp__sub_8210C370, NativeMapperUpdate,
      [&](PPCContext& t, int) { t.r3.u64 = m; });
  // ActionMappers::Update on a client list in the area holding the mapper
  // (bound count 0 or 1).
  run("sub_8210B018 ActionMappers::Update", 100, __imp__sub_8210B018, NativeUpdateAll,
      [&](PPCContext& t, int i) {
        const uint32_t list = h.data + kList;
        guest::StoreU32(h.base, list, 0x82007F18);
        guest::StoreU32(h.base, list + 4, m);
        guest::StoreU32(h.base, list + 8, m);
        for (uint32_t k = 0; k < 6; k++) {
          guest::StoreU32(h.base, list + 12 + 4 * k, k == 2 || k == 5 ? (i % 10 != 0) : 0);
        }
        guest::StoreU32(h.base, am::kClientList, list);
        t.r3.u64 = list;
      });
}

// --- The read slots, the setters and Configure ---

void Slots(Context& h) {
  if (!GameMapper(h)) {
    return;
  }
  const uint32_t m = h.data + kMapper;
  // Ids around 151-180, then any.
  auto id = [&](int i) -> uint64_t {
    if (i % 10 == 9) {
      return h.random.Next();
    }
    return 140 + h.random.Below(50);
  };
  struct Slot {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Slot reads[] = {
      {"sub_8210BE70 ActionMapper::Held", __imp__sub_8210BE70, NativeHeld},
      {"sub_8210BF40 ActionMapper::Pressed", __imp__sub_8210BF40, NativePressed},
      {"sub_8210C010 ActionMapper::Fired", __imp__sub_8210C010, NativeFired},
      {"sub_8210BA28 ActionMapper::AnalogRaw", __imp__sub_8210BA28, NativeAnalogRaw},
  };
  for (const Slot& slot : reads) {
    Results results(slot.name);
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, [&] { RandomMapper(h, true); },
          [&](PPCContext& t) {
            t.r3.u64 = m;
            t.r4.u64 = id(i);
          },
          slot.recompiled, slot.native, UpdateOptions(), fmt::format("case {}", i));
    }
    results.Report();
  }
  // The thresholds' and curves' floats as floats.
  Options setters;
  setters.f1 = false;
  setters.floats = {{kMapper + 1376, kMapper + 1372 + 480}};
  for (uint32_t i = 0; i < 30; i++) {
    setters.floats.push_back({kMapper + am::kThresholds + 20 * i,
                              kMapper + am::kThresholds + 20 * i + 8});
  }
  // Curves' ranges: one range of the whole objects would mark vtables as
  // floats, so each curve's three floats.
  setters.floats.erase(setters.floats.begin());
  FloatRanges curves;
  for (uint32_t i = 0; i < 30; i++) {
    curves.push_back({kMapper + 1372 + 16 * i + 4, kMapper + 1372 + 16 * i + 16});
  }
  curves.insert(curves.end(), setters.floats.begin(), setters.floats.end());
  setters.floats = curves;
  {
    Results results("sub_8210B698 ActionMapper::SetThreshold");
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, [&] { RandomMapper(h, i % 2); },
          [&](PPCContext& t) {
            t.r3.u64 = m;
            t.r4.u64 = id(i);
            t.f1.f64 = double(RandomFloat(h.random, 1.0f));
            t.f2.f64 = i % 7 == 0 ? double(RandomFloat(h.random, 1.0f)) * 1.000001
                                  : double(RandomFloat(h.random, 1.0f));
            t.r7.u64 = h.random.Next();
          },
          __imp__sub_8210B698, NativeSetThreshold, setters, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_8210BDF0 ActionMapper::SetRepeat");
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, [&] { RandomMapper(h, i % 2); },
          [&](PPCContext& t) {
            t.r3.u64 = m;
            t.r4.u64 = id(i);
            t.r5.u64 = h.random.Next();
            t.r6.u64 = h.random.Next();
          },
          __imp__sub_8210BDF0, NativeSetRepeat, setters, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_8210BE30 ActionMapper::SetCurve");
    for (int i = 0; i < 300; i++) {
      Case(
          h, results, [&] { RandomMapper(h, i % 2); },
          [&](PPCContext& t) {
            t.r3.u64 = m;
            t.r4.u64 = id(i);
            t.f1.f64 = double(RandomFloat(h.random, 1.0f));
            // A double that isn't a single, rounded by the store.
            t.f2.f64 = double(RandomFloat(h.random, 1.0f)) + (i % 5 == 0 ? 1e-12 : 0.0);
            t.f3.f64 = double(RandomFloat(h.random, 2.0f));
          },
          __imp__sub_8210BE30, NativeSetCurve, setters, fmt::format("case {}", i));
    }
    results.Report();
  }
  // Configure: the four mappers' entries pointed at the area's mapper (the
  // same one four times), the settings' index the game's or another table
  // entry; the globals put back after each case.
  {
    Results results("sub_8209F228 ActionMappers::Configure");
    constexpr uint32_t kTable = 0x826BAB80;
    constexpr uint32_t kIndex = 0x82600B84;
    const std::vector<uint8_t> table = Read(h.base, kTable, 32);
    const uint32_t index = guest::LoadU32(h.base, kIndex);
    for (int i = 0; i < 8; i++) {
      Case(
          h, results,
          [&] {
            RandomMapper(h, false);
            for (uint32_t j = 0; j < 4; j++) {
              guest::StoreU32(h.base, kTable + 8 * j, m);
            }
          },
          [&](PPCContext& t) { t.r3.u64 = i < 4 ? index : uint32_t(i - 4); },
          __imp__sub_8209F228, NativeConfigure, setters, fmt::format("index {}", i < 4 ? index : uint32_t(i - 4)));
      Write(h.base, kTable, table.data(), 32);
      guest::StoreU32(h.base, kIndex, index);
    }
    results.Report();
  }
}

// --- The set-up ---

void SetUp(Context& h) {
  if (!GameMapper(h)) {
    return;
  }
  const uint32_t m = h.data + kMapper;
  // The analog floats, the curves' floats and the thresholds' on and off as
  // floats.
  Options options;
  options.f1 = false;
  for (uint32_t i = 0; i < 30; i++) {
    options.floats.push_back({kMapper + am::kAnalog + 60 + 4 * i, kMapper + am::kAnalog + 64 + 4 * i});
  }
  for (uint32_t i = 0; i < 30; i++) {
    options.floats.push_back({kMapper + 1372 + 16 * i + 4, kMapper + 1372 + 16 * i + 16});
  }
  for (uint32_t i = 0; i < 30; i++) {
    options.floats.push_back(
        {kMapper + am::kThresholds + 20 * i, kMapper + am::kThresholds + 20 * i + 8});
  }
  struct Step {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t r3;
  };
  const Step steps[] = {
      {"sub_8210B8A8 ActionMapper::ResetWords", __imp__sub_8210B8A8, NativeResetWords,
       m + am::kWords},
      {"sub_8210B438 ActionMapper::ResetAnalog", __imp__sub_8210B438, NativeResetAnalog,
       m + am::kAnalog},
      {"sub_8210BB38 ActionMapper::InitRepeats", __imp__sub_8210BB38, NativeInitRepeats,
       m + am::kWords},
      {"sub_8210BAB0 ActionMapper::InitCurves", __imp__sub_8210BAB0, NativeInitCurves,
       m + am::kAnalog},
      {"sub_8210C078 ActionMapper::ResetStages", __imp__sub_8210C078, NativeResetStages,
       m + am::kWords},
  };
  for (const Step& step : steps) {
    Results results(step.name);
    for (int i = 0; i < 40; i++) {
      Case(
          h, results,
          [&] {
            RandomMapper(h, i % 2);
            // Pointers to the objects scrambled (the inits set them again).
            if (i % 4 >= 2) {
              for (uint32_t k = 0; k < 30; k++) {
                const uint32_t j = h.random.Below(30);
                guest::StoreU32(h.base, m + am::kRepeatPointers + 4 * k, m + 592 + 16 * j);
                guest::StoreU32(h.base, m + am::kCurvePointers + 4 * k, m + 1372 + 16 * j);
              }
            }
          },
          [&](PPCContext& t) { t.r3.u64 = step.r3; }, step.recompiled, step.native, options,
          fmt::format("case {}", i));
    }
    results.Report();
  }
  const uint32_t list = h.data + kList;
  {
    Results results("sub_8210AEB0 InputClientList::InputClientList");
    for (int i = 0; i < 4; i++) {
      Case(
          h, results, [&] { RandomList(h, list, h.data + 64, i, i % 2); },
          [&](PPCContext& t) {
            t.r3.u64 = list;
            t.r4.u64 = h.random.Next();
          },
          __imp__sub_8210AEB0, NativeInputClientListConstruct, {}, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_8210AE60 InputClient::InputClient");
    for (uint32_t type : {0u, 1u, 2u, 3u}) {
      Case(
          h, results, [] {},
          [&](PPCContext& t) {
            t.r3.u64 = h.data + 64;
            t.r4.u64 = type;
          },
          __imp__sub_8210AE60, NativeInputClientConstruct, {}, fmt::format("type {}", type));
    }
    results.Report();
  }
  {
    Results results("sub_8210AF00 InputClientList::Add");
    const uint32_t added = h.data + 1024;
    for (int i = 0; i < 30; i++) {
      Case(
          h, results,
          [&] {
            RandomList(h, list, h.data + 128, i % 6, i % 10 != 9);
            guest::StoreU32(h.base, added, 0x82007ED8);
            guest::StoreU32(h.base, added + 4, h.random.Below(3));
            guest::StoreU32(h.base, added + 8, 0);
          },
          [&](PPCContext& t) {
            t.r3.u64 = list;
            t.r4.u64 = added;
          },
          __imp__sub_8210AF00, NativeInputClientListAdd, {}, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The constructor over a random mapper (what the area held), for the
    // four players and others; its listener's constants as floats too.
    Results results("sub_8210C180 ActionMapper::ActionMapper");
    Options constructed = options;
    constructed.floats.push_back({kMapper + am::kListener + 16, kMapper + am::kListener + 32});
    for (uint32_t k = 0; k < 6; k++) {
      const uint32_t w = kMapper + am::kListener + 44 + 16 * k;
      constructed.floats.push_back({w, w + 4});
    }
    for (int i = 0; i < 24; i++) {
      Case(
          h, results, [&] { RandomMapper(h, i % 2); },
          [&](PPCContext& t) {
            t.r3.u64 = m;
            t.r4.u64 = i < 16 ? uint32_t(i % 4) : uint32_t(h.random.Next());
          },
          __imp__sub_8210C180, NativeMapperConstruct, constructed, fmt::format("case {}", i));
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
  Updates(h);
  Slots(h);
  SetUp(h);
}

}  // namespace harness
