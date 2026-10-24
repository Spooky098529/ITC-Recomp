// The input manager's set-up and the std::list of pointers
// (src/input/input_manager.cpp, src/std_list/ptr_list.cpp,
// docs/functions/input.md) against the recompiled functions. The objects are
// built in the scratch area, its other bytes left at the fill, and the whole
// area is compared after each case (floats where the function stores
// floats, bytes everywhere else), then r3. The originals run with
// --native_input off, so their callees are the recompiled code too. The
// functions that allocate run only with --native_heap (the adapters'
// condition): a node they make is compared by its contents and size, its
// address replaced by a marker wherever it's stored, and freed after.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "std_list/ptr_list.h"

#include "input/input_manager.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <limits>

#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_input);

namespace harness {

namespace {

namespace sl = std_list;
namespace in = input;

// Cases the plain C++ handed to the original (none expected).
int g_fallbacks = 0;

// --- The register steps, as the adapters (src/hooks/input_manager.cpp) ---

void NativeAllocateNodes(PPCContext& ctx, uint8_t* base) {
  if (const auto block = sl::AllocateNodes(base, ctx.r3.u32)) {
    ctx.r3.u64 = *block;
    return;
  }
  g_fallbacks++;
  __imp__sub_82099C08(ctx, base);
}
void NativeIncSize(PPCContext& ctx, uint8_t* base) {
  if (!sl::IncSize(base, ctx.r3.u32, ctx.r4.u32)) {
    g_fallbacks++;
    __imp__sub_820958B8(ctx, base);
  }
}
void NativeConstruct(PPCContext& ctx, uint8_t* base) {
  if (!sl::Construct(base, ctx.r3.u32)) {
    g_fallbacks++;
    __imp__sub_820AD058(ctx, base);
  }
}
void NativeInsert(PPCContext& ctx, uint8_t* base) {
  if (!sl::Insert(base, ctx.r3.u32, ctx.r4.u32, guest::LoadU32(base, ctx.r5.u32))) {
    g_fallbacks++;
    __imp__sub_82095958(ctx, base);
  }
}

#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }
ITC_NATIVE_STEP(NativeLogicalPadConstruct, in::LogicalPadConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativePhysicalPadConstruct, in::PhysicalPadConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeProcessorBaseConstruct, in::InputProcessorBaseConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeProcessorConstruct, in::InputProcessorConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeBind, in::LogicalPadBind(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32),
                                               ctx.r5.u32))
ITC_NATIVE_STEP(NativeAssign, in::InputProcessorAssign(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeGetProcessor,
                ctx.r3.u64 = in::ManagerProcessor(ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32)))
ITC_NATIVE_STEP(NativeGetLogicalPad,
                ctx.r3.u64 = in::ManagerLogicalPad(ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32)))
ITC_NATIVE_STEP(NativeGetPhysicalPad,
                ctx.r3.u64 = in::ManagerPhysicalPad(ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32)))
ITC_NATIVE_STEP(NativeLogicalPadValues, ctx.r3.u64 = in::LogicalPadValues(ctx.r3.u32))
ITC_NATIVE_STEP(NativePhysicalPadValues, ctx.r3.u64 = in::PhysicalPadValues(ctx.r3.u32))
// The tests (the mask read from r4) and their wrappers (given its address).
ITC_NATIVE_STEP(NativeAnyPressed, ctx.r3.u64 = in::InputProcessorAnyPressed(
                                      base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32)))
ITC_NATIVE_STEP(NativePressedOrRepeat, ctx.r3.u64 = in::InputProcessorPressedOrRepeat(
                                           base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32)))
ITC_NATIVE_STEP(NativeTestPressed,
                ctx.r3.u64 = in::InputProcessorTestPressed(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeTestRepeat,
                ctx.r3.u64 = in::InputProcessorTestRepeat(base, ctx.r3.u32, ctx.r4.u32))
#undef ITC_NATIVE_STEP

// The manager's adapters' steps (src/hooks/input_manager.cpp) make guest
// calls, so the cases run the adapters themselves with the flag on (the
// originals with it off): the constructor's and Create's work around the
// dispatcher's calls is theirs. AddListener's step is its plain C++.
void NativeAddListener(PPCContext& ctx, uint8_t* base) {
  const uint32_t listener = ctx.r5.u32;
  if (!in::ManagerAddListener(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32), listener)) {
    g_fallbacks++;
    __imp__sub_82099430(ctx, base);
    return;
  }
  ctx.r3.u64 = listener;
}

// A heap block a case made: its address replaced by this marker wherever
// it's stored.
constexpr uint32_t kNodeMarker = 0xB10C0001;

uint32_t Normalize(uint32_t word, uint32_t node) {
  return node != 0 && word == node ? kNodeMarker : word;
}

// Guest addresses of words the function stores as floats.
using FloatWords = std::vector<uint32_t>;

// The scratch area as words (exact, with the node's address replaced), the
// words in `floats` as floats.
void AreaWords(Context& h, Outputs& out, uint32_t node, const FloatWords& floats = {}) {
  for (uint32_t at = 0; at < kScratchBytes; at += 4) {
    const uint32_t address = h.data + at;
    const uint32_t word = guest::LoadU32(h.base, address);
    if (std::find(floats.begin(), floats.end(), address) != floats.end()) {
      out.Float(word);
    } else {
      out.Word(Normalize(word, node));
    }
  }
}

// The node: whether it's a heap block, its size and its first `words` words
// (normalized); then freed.
void NodeOut(Context& h, Outputs& out, uint32_t node, uint32_t words) {
  const bool block = node != 0 && memory::IsHeapBlock(node);
  out.Word(block);
  if (!block) {
    return;
  }
  out.Word(memory::Size(node));
  for (uint32_t i = 0; i < words; i++) {
    out.Word(Normalize(guest::LoadU32(h.base, node + 4 * i), node));
  }
  memory::crt::OperatorDelete(node);
}

// One case through both versions: the area filled, `setup()` writes the
// inputs (the same both times: it's handed a copy of the generator's
// state), `args(t)` sets the registers, the original (flag off) or the
// native step runs, then `collect(out, t)` keeps what's compared.
template <typename Setup, typename Args, typename Collect>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, Collect collect, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_input) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_input) = true;
    collect(out[v], t);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// --- The std::list of pointers ---

void AllocateNodesCases(Context& h) {
  Results results("sub_82099C08 PtrList::AllocateNodes");
  for (uint32_t i = 0; i < 64; i++) {
    // 1 (every call in play), 0, and small counts.
    const uint32_t count = i < 16 ? 1 : i < 20 ? 0 : h.random.Below(200);
    Case(
        h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = count; }, __imp__sub_82099C08,
        NativeAllocateNodes,
        [&](Outputs& out, const PPCContext& t) {
          const uint32_t node = t.r3.u32;
          AreaWords(h, out, node);
          out.Word(Normalize(node, node));
          NodeOut(h, out, node, 0);  // its contents aren't set
        },
        fmt::format("count {}", count));
  }
  results.Report();
}

void IncSizeCases(Context& h) {
  Results results("sub_820958B8 PtrList::IncSize");
  const uint32_t list = h.data + 64;
  for (int i = 0; i < 200; i++) {
    uint32_t size = 0, count = 1;
    Case(
        h, results,
        [&] {
          Random& r = h.random;
          // Up to the limit (0x3FFFFFFF in all), never past it: past it is
          // the original's length_error, which the plain C++ hands back.
          size = i < 50 ? r.Below(8) : i < 100 ? 0x3FFFFFFFu - r.Below(4) : r.Below(0x3FFFFFFF);
          const uint32_t room = 0x3FFFFFFFu - size;
          count = i % 3 == 0 ? 1 : i % 3 == 1 ? 0 : uint32_t(r.Next() % (uint64_t(room) + 1));
          if (count > room) {
            count = room;
          }
          guest::StoreU32(h.base, list + sl::kSize, size);
        },
        [&](PPCContext& t) {
          t.r3.u64 = list;
          t.r4.u64 = count;
        },
        __imp__sub_820958B8, NativeIncSize,
        [&](Outputs& out, const PPCContext& t) {
          AreaWords(h, out, 0);
          out.Word(t.r3.u32);
        },
        fmt::format("case {}", i));
  }
  results.Report();
}

void ConstructCases(Context& h) {
  Results results("sub_820AD058 PtrList::PtrList");
  for (int i = 0; i < 32; i++) {
    const uint32_t list = h.data + 64 + 4 * uint32_t(i);
    Case(
        h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = list; }, __imp__sub_820AD058,
        NativeConstruct,
        [&](Outputs& out, const PPCContext& t) {
          const uint32_t node = guest::LoadU32(h.base, list + sl::kHead);
          AreaWords(h, out, node);
          out.Word(t.r3.u32);
          NodeOut(h, out, node, 2);  // the head's value isn't set
        },
        fmt::format("list at +{}", 64 + 4 * i));
  }
  results.Report();
}

void InsertCases(Context& h) {
  Results results("sub_82095958 PtrList::Insert");
  const uint32_t list = h.data + 16;
  const uint32_t value = h.data + 32;
  const uint32_t nodes = h.data + 64;  // the head, then up to 7 nodes
  for (int i = 0; i < 200; i++) {
    uint32_t where = 0;
    Case(
        h, results,
        [&] {
          Random& r = h.random;
          // A list of 0-7 nodes (the input manager's are empty, the sound
          // manager's grow), linked in order around the head.
          const uint32_t count = i < 40 ? 0 : r.Below(8);
          for (uint32_t k = 0; k <= count; k++) {
            const uint32_t node = nodes + sl::kNodeBytes * k;
            const uint32_t next = nodes + sl::kNodeBytes * (k == count ? 0 : k + 1);
            const uint32_t previous = nodes + sl::kNodeBytes * (k == 0 ? count : k - 1);
            guest::StoreU32(h.base, node + sl::kNext, next);
            guest::StoreU32(h.base, node + sl::kPrevious, previous);
            guest::StoreU32(h.base, node + sl::kValue, uint32_t(r.Next()));
          }
          guest::StoreU32(h.base, list + sl::kHead, nodes);
          guest::StoreU32(h.base, list + sl::kSize, i % 5 == 4 ? r.Below(0x3FFFFFFF) : count);
          guest::StoreU32(h.base, value, uint32_t(r.Next()));
          // Before the head (at the end: the input manager's) or any node.
          where = nodes + sl::kNodeBytes * (i < 100 ? 0 : r.Below(count + 1));
        },
        [&](PPCContext& t) {
          t.r3.u64 = list;
          // The iterator by value: its list in the high word, as
          // InputManager::AddListener passes it.
          t.r4.u64 = (uint64_t(list) << 32) | where;
          t.r5.u64 = value;
        },
        __imp__sub_82095958, NativeInsert,
        [&](Outputs& out, const PPCContext& t) {
          const uint32_t node = guest::LoadU32(h.base, where + sl::kPrevious);
          AreaWords(h, out, node);
          out.Word(t.r3.u32);
          NodeOut(h, out, node, 3);
        },
        fmt::format("case {}", i));
  }
  results.Report();
}

// --- The pads and processors ---

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
                                 std::numeric_limits<float>::infinity(),
                                 -std::numeric_limits<float>::infinity(),
                                 std::numeric_limits<float>::quiet_NaN(),
                                 -std::numeric_limits<float>::quiet_NaN(),
                                 std::numeric_limits<float>::denorm_min(),
                                 std::numeric_limits<float>::max()};
  if (r.Below(6) == 0) {
    return kEdges[r.Below(uint32_t(std::size(kEdges)))];
  }
  return (float(r.Below(2000001)) / 1000000.0f - 1.0f) * range;
}

// An object's address in the scratch area: 16-byte aligned (the game's
// processors are, and Input::Update's native path wants them so), at a few
// offsets, leaving `bytes` inside.
uint32_t ObjectAt(Context& h, int i, uint32_t bytes) {
  const uint32_t room = (kScratchBytes - bytes) / 16;
  return h.data + 16 * uint32_t(i % int(room < 8 ? room : 8));
}

FloatWords ProcessorConstantFloats(uint32_t p) {
  FloatWords floats = {p + 16, p + 20, p + 24, p + 28};
  for (uint32_t w = p + 44; w <= p + 124; w += 16) {
    floats.push_back(w);
  }
  return floats;
}

// A constructor: r3 the object; the area (floats where it stores them), r3.
void ConstructorCases(Context& h, const char* name, GuestFunction recompiled,
                      GuestFunction native, uint32_t bytes, FloatWords (*floats)(uint32_t)) {
  Results results(name);
  for (int i = 0; i < 16; i++) {
    const uint32_t object = ObjectAt(h, i, bytes) + (i >= 8 ? 4 * uint32_t(i - 8) : 0);
    Case(
        h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = object; }, recompiled, native,
        [&](Outputs& out, const PPCContext& t) {
          AreaWords(h, out, 0, floats(object));
          out.Word(t.r3.u32);
        },
        fmt::format("object at +{}", object - h.data));
  }
  results.Report();
}

void PadCases(Context& h) {
  ConstructorCases(h, "sub_82591B38 LogicalPad::LogicalPad", __imp__sub_82591B38,
                   NativeLogicalPadConstruct, 196, [](uint32_t) { return FloatWords{}; });
  ConstructorCases(h, "sub_82591B68 PhysicalPad::PhysicalPad", __imp__sub_82591B68,
                   NativePhysicalPadConstruct, 212,
                   [](uint32_t p) { return FloatWords{p + 168, p + 180}; });
  ConstructorCases(h, "sub_825920D0 InputProcessorBase::InputProcessorBase",
                   __imp__sub_825920D0, NativeProcessorBaseConstruct, 416,
                   ProcessorConstantFloats);
  ConstructorCases(h, "sub_8258CE60 InputProcessor::InputProcessor", __imp__sub_8258CE60,
                   NativeProcessorConstruct, 416, ProcessorConstantFloats);

  {
    Results results("sub_825918F0 LogicalPad::Bind");
    const uint32_t pad = h.data + 256;
    const uint32_t index = h.data;
    for (int i = 0; i < 100; i++) {
      uint32_t device = 0;
      Case(
          h, results,
          [&] {
            Random& r = h.random;
            // The game's indices (0-3), then others the pad's room allows.
            guest::StoreU32(h.base, index, i < 40 ? uint32_t(i % 4) : r.Below(64));
            device = uint32_t(r.Next());
          },
          [&](PPCContext& t) {
            t.r3.u64 = pad;
            t.r4.u64 = index;
            t.r5.u64 = device;
          },
          __imp__sub_825918F0, NativeBind,
          [&](Outputs& out, const PPCContext& t) {
            AreaWords(h, out, 0);
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", i));
    }
    results.Report();
  }

  // The leaves returning an address from r3 (and the index at r4's).
  struct Getter {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Getter kGetters[] = {
      {"sub_82098CB8 InputManager::GetProcessor", __imp__sub_82098CB8, NativeGetProcessor},
      {"sub_82098CD0 InputManager::GetLogicalPad", __imp__sub_82098CD0, NativeGetLogicalPad},
      {"sub_82098FE0 InputManager::GetPhysicalPad", __imp__sub_82098FE0, NativeGetPhysicalPad},
      {"sub_82591B30 LogicalPad::Values", __imp__sub_82591B30, NativeLogicalPadValues},
      {"sub_820988B8 PhysicalPad::Values", __imp__sub_820988B8, NativePhysicalPadValues},
  };
  for (const Getter& g : kGetters) {
    Results results(g.name);
    const uint32_t index = h.data;
    for (int i = 0; i < 60; i++) {
      uint32_t object = 0;
      Case(
          h, results,
          [&] {
            Random& r = h.random;
            object = i < 20 ? 0x40000000u + 16 * r.Below(0x10000) : uint32_t(r.Next());
            guest::StoreU32(h.base, index, i < 30 ? uint32_t(i % 5) : uint32_t(r.Next()));
          },
          [&](PPCContext& t) {
            t.r3.u64 = object;
            t.r4.u64 = index;
          },
          g.recompiled, g.native,
          [&](Outputs& out, const PPCContext& t) {
            AreaWords(h, out, 0);
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", i));
    }
    results.Report();
  }

  {
    Results results("sub_82098D60 InputProcessor::Assign");
    for (int i = 0; i < 100; i++) {
      // The two processors apart (the manager's and a mapper's listener),
      // 16-byte aligned or not.
      const uint32_t source = h.data + 16 + (i % 2 ? 4 : 0);
      const uint32_t destination = h.data + 2048 + (i % 3 == 2 ? 8 : 0);
      Case(
          h, results,
          [&] {
            Random& r = h.random;
            for (uint32_t at = 0; at < 416; at += 4) {
              const bool is_float = at >= 16 && at < 264;
              guest::StoreU32(h.base, source + at,
                              is_float ? Bits(RandomFloat(r, 2.0f)) : uint32_t(r.Next()));
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = destination;
            t.r4.u64 = source;
          },
          __imp__sub_82098D60, NativeAssign,
          [&](Outputs& out, const PPCContext& t) {
            FloatWords floats;
            for (uint32_t at = 128; at < 264; at += 4) {
              floats.push_back(destination + at);
            }
            AreaWords(h, out, 0, floats);
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", i));
    }
    results.Report();
  }

  // The button tests: the pressed (+264) and held (+272) words, the delay
  // (+276) and the counters (+284) set, a mask at the area's start.
  struct Test {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Test kTests[] = {
      {"sub_82592500 InputProcessor::AnyPressed", __imp__sub_82592500, NativeAnyPressed},
      {"sub_8258CEB0 InputProcessor::TestPressed", __imp__sub_8258CEB0, NativeTestPressed},
      {"sub_82592520 InputProcessor::PressedOrRepeat", __imp__sub_82592520,
       NativePressedOrRepeat},
      {"sub_8258CEE0 InputProcessor::TestRepeat", __imp__sub_8258CEE0, NativeTestRepeat},
  };
  for (const Test& test : kTests) {
    Results results(test.name);
    const uint32_t mask = h.data;
    const uint32_t processor = h.data + 64;
    for (int i = 0; i < 300; i++) {
      Case(
          h, results,
          [&] {
            Random& r = h.random;
            // Masks of one button (the game's), none, or several.
            const uint32_t kind = r.Below(4);
            guest::StoreU32(h.base, mask,
                            kind == 0 ? 0 : kind == 3 ? uint32_t(r.Next()) : 1u << r.Below(32));
            const uint32_t words = r.Below(3);
            guest::StoreU32(h.base, processor + 264,
                            words == 0 ? 0 : words == 1 ? ~0u : uint32_t(r.Next()));
            guest::StoreU32(h.base, processor + 272,
                            words == 0 ? 0 : words == 1 ? ~0u : uint32_t(r.Next()));
            const uint32_t delay = r.Below(4) == 0 ? uint32_t(r.Next()) : 20;
            guest::StoreU32(h.base, processor + 276, delay);
            for (uint32_t k = 0; k < 32; k++) {
              const uint32_t pick = r.Below(5);
              guest::StoreU32(h.base, processor + 284 + 4 * k,
                              pick == 0   ? 1
                              : pick == 1 ? delay
                              : pick == 2 ? uint32_t(r.Next())
                                          : r.Below(30));
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = processor;
            t.r4.u64 = mask;
          },
          test.recompiled, test.native,
          [&](Outputs& out, const PPCContext& t) {
            AreaWords(h, out, 0);
            out.Word(t.r3.u32);
          },
          fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- The manager ---

// Words of guest memory: exact, with the nodes' addresses (and the
// manager's, for Create) replaced by markers, `floats` as floats.
void BlockWords(Context& h, Outputs& out, uint32_t address, uint32_t bytes,
                const std::vector<uint32_t>& blocks, const FloatWords& floats) {
  for (uint32_t at = address; at < address + bytes; at += 4) {
    uint32_t word = guest::LoadU32(h.base, at);
    if (std::find(floats.begin(), floats.end(), at) != floats.end()) {
      out.Float(word);
      continue;
    }
    for (size_t k = 0; k < blocks.size(); k++) {
      if (blocks[k] != 0 && word == blocks[k]) {
        word = kNodeMarker + uint32_t(k);
        break;
      }
    }
    out.Word(word);
  }
}

// The words a manager's constructor stores as floats: its processors' dead
// zones and w's, its physical pads' records.
FloatWords ManagerFloats(uint32_t manager) {
  FloatWords floats;
  for (uint32_t i = 0; i < 5; i++) {
    for (uint32_t w : ProcessorConstantFloats(manager + in::kManagerProcessors +
                                              in::kProcessorBytes * i)) {
      floats.push_back(w);
    }
  }
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t pad = manager + in::kManagerPhysicalPads + in::kPhysicalPadBytes * i;
    floats.push_back(pad + 168);
    floats.push_back(pad + 180);
  }
  return floats;
}

// A test dispatcher whose slots +4 and +8 are LogicalPad::Bind
// (sub_825918F0): called with (dispatcher, &message, &functor), it keeps the
// functor's address at +8 + 8 x message (and a 1 at +4 + 8 x message), so
// the functor the constructor built on its stack can be read back.
struct Dispatcher {
  uint32_t arguments, dispatcher, vtable;
};
Dispatcher MakeDispatcher(Context& h, uint32_t at, uint32_t word) {
  Dispatcher d{at, at + 32, at + 112};
  guest::StoreU32(h.base, d.arguments, d.dispatcher);
  guest::StoreU32(h.base, d.arguments + 4, word);
  for (uint32_t k = 0; k < 80; k += 4) {
    guest::StoreU32(h.base, d.dispatcher + k, 0);
  }
  guest::StoreU32(h.base, d.dispatcher, d.vtable);
  guest::StoreU32(h.base, d.vtable + 4, 0x825918F0);
  guest::StoreU32(h.base, d.vtable + 8, 0x825918F0);
  return d;
}

// The dispatcher's words and the two functors (12 bytes each, on the
// constructor's stack).
void DispatcherOut(Context& h, Outputs& out, const Dispatcher& d,
                   const std::vector<uint32_t>& blocks) {
  BlockWords(h, out, d.dispatcher, 80, blocks, {});
  for (uint32_t message : {5u, 7u}) {
    const uint32_t functor = guest::LoadU32(h.base, d.dispatcher + 8 + 8 * message);
    out.Word(functor - h.ctx.r1.u32);
    BlockWords(h, out, functor, 12, blocks, {});
  }
}

// The manager's five head nodes (their addresses, in `blocks` from 1),
// compared as nodes and freed.
std::vector<uint32_t> HeadNodes(Context& h, uint32_t manager) {
  std::vector<uint32_t> nodes;
  for (uint32_t i = 0; i < 5; i++) {
    nodes.push_back(guest::LoadU32(
        h.base, manager + in::kManagerListeners + in::kListenerListBytes * i + sl::kHead));
  }
  return nodes;
}
void HeadNodesOut(Context& h, Outputs& out, const std::vector<uint32_t>& blocks) {
  for (size_t k = 1; k < blocks.size(); k++) {
    const uint32_t node = blocks[k];
    const bool block = node != 0 && memory::IsHeapBlock(node);
    out.Word(block);
    if (block) {
      out.Word(memory::Size(node));
      BlockWords(h, out, node, 8, blocks, {});  // the head's value isn't set
      memory::crt::OperatorDelete(node);
    }
  }
}

void ManagerCases(Context& h) {
  const uint32_t area = memory::Alloc(0x2000, 0);
  {
    Results results("sub_820991E0 InputManager::InputManager");
    const uint32_t manager = area;
    for (int i = 0; i < 8; i++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, area, 0x2000);
        const Dispatcher d = MakeDispatcher(h, area + 4000, 0x12340000u + uint32_t(i));
        PPCContext t = CallContext(h);
        t.r3.u64 = manager;
        t.r4.u64 = d.arguments;
        ITC_FLAG(native_input) = v == 1;
        (v == 0 ? __imp__sub_820991E0 : sub_820991E0)(t, h.base);
        ITC_FLAG(native_input) = true;
        std::vector<uint32_t> blocks = {0};
        for (uint32_t node : HeadNodes(h, manager)) {
          blocks.push_back(node);
        }
        BlockWords(h, out[v], area, 0x2000, blocks, ManagerFloats(manager));
        DispatcherOut(h, out[v], d, blocks);
        out[v].Word(t.r3.u32);
        HeadNodesOut(h, out[v], blocks);
      }
      results.Check(out[0], out[1], fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_820993E8 Input::Create");
    for (int i = 0; i < 8; i++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, area, 0x2000);
        const Dispatcher d = MakeDispatcher(h, area + 4000, 0x56780000u + uint32_t(i));
        PPCContext t = CallContext(h);
        t.r3.u64 = d.arguments;
        ITC_FLAG(native_input) = v == 1;
        (v == 0 ? __imp__sub_820993E8 : sub_820993E8)(t, h.base);
        ITC_FLAG(native_input) = true;
        const uint32_t manager = t.r3.u32;
        std::vector<uint32_t> blocks = {manager};
        for (uint32_t node : HeadNodes(h, manager)) {
          blocks.push_back(node);
        }
        out[v].Word(manager != 0 && memory::IsHeapBlock(manager));
        out[v].Word(memory::Size(manager));
        BlockWords(h, out[v], manager, in::kManagerBytes, blocks, ManagerFloats(manager));
        BlockWords(h, out[v], area, 0x2000, blocks, {});
        DispatcherOut(h, out[v], d, blocks);
        HeadNodesOut(h, out[v], blocks);
        memory::crt::OperatorDelete(manager);
      }
      results.Check(out[0], out[1], fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_82099430 InputManager::AddListener");
    const uint32_t manager = area;
    const uint32_t heads = area + 0x1000;
    const uint32_t listener = area + 0x1100;
    const uint32_t index = area + 0x1300;
    for (int i = 0; i < 100; i++) {
      Outputs out[2];
      const Random start = h.random;
      uint32_t node = 0;
      for (int v = 0; v < 2; v++) {
        h.random = start;
        Random& r = h.random;
        Fill(h.base, area, 0x2000);
        // The five processors' words (floats where the update keeps them),
        // the lists empty with their heads in the area.
        for (uint32_t at = 0; at < 5 * in::kProcessorBytes; at += 4) {
          const uint32_t offset = at % in::kProcessorBytes;
          const bool is_float = offset >= 16 && offset < 264;
          guest::StoreU32(h.base, manager + in::kManagerProcessors + at,
                          is_float ? Bits(RandomFloat(r, 2.0f)) : uint32_t(r.Next()));
        }
        for (uint32_t k = 0; k < 5; k++) {
          const uint32_t list = manager + in::kManagerListeners + in::kListenerListBytes * k;
          const uint32_t head = heads + 16 * k;
          guest::StoreU32(h.base, head + sl::kNext, head);
          guest::StoreU32(h.base, head + sl::kPrevious, head);
          guest::StoreU32(h.base, list + sl::kHead, head);
          guest::StoreU32(h.base, list + sl::kSize, 0);
        }
        const uint32_t pad = uint32_t(i % 5);
        guest::StoreU32(h.base, index, pad);
        PPCContext t = CallContext(h);
        t.r3.u64 = manager;
        t.r4.u64 = index;
        t.r5.u64 = listener;
        ITC_FLAG(native_input) = v == 1;
        (v == 0 ? __imp__sub_82099430 : NativeAddListener)(t, h.base);
        ITC_FLAG(native_input) = true;
        node = guest::LoadU32(h.base, heads + 16 * pad + sl::kPrevious);
        FloatWords floats;
        for (uint32_t at = 128; at < 264; at += 4) {
          floats.push_back(listener + at);
        }
        BlockWords(h, out[v], area, 0x2000, {node}, floats);
        out[v].Word(t.r3.u32);
        NodeOut(h, out[v], node, 3);
      }
      results.Check(out[0], out[1], fmt::format("pad {}", i % 5));
    }
    results.Report();
  }
  memory::Free(area);
}

}  // namespace

void Input(Context& h) {
  g_fallbacks = 0;
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: input: the std::list cases need --native_heap; skipped");
  } else {
    AllocateNodesCases(h);
    ConstructCases(h);
    InsertCases(h);
    ManagerCases(h);
  }
  IncSizeCases(h);
  PadCases(h);
  if (g_fallbacks != 0) {
    ITC_LOG_ERROR("harness: input: {} cases handed to the original", g_fallbacks);
  }
}

}  // namespace harness
