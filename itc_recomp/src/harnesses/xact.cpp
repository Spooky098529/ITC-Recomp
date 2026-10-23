// XACT (src/xact/, docs/functions/xact.md) against the recompiled
// functions. The objects (node pools, lists, settings, variables, cues,
// faders) are built in the scratch area, its other bytes left at the fill,
// and the whole area is compared after each case: floats where the
// function stores floats, bytes everywhere else. The originals run with
// --native_xact off, so their callees are the recompiled code too. Cases
// where the original traps (a hash table size of 0) aren't run: the native
// version hands those to the original. The cases allocating or freeing
// (the pools' chunks) run only while --native_heap routes the game's heap
// to the native layer, and compare blocks by their offsets.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "xact/xact.h"

#include <fmt/format.h>

#include <bit>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_xact);

namespace harness {

namespace {

namespace xa = xact;

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }

void NativeHashName(PPCContext& ctx, uint8_t* base) {
  uint32_t hash = 0;
  if (!xa::HashName(base, ctx.r4.u32, ctx.r5.u32, &hash)) {
    ctx.r3.u64 = 0xDEADDEAD;  // not run: the original traps
    return;
  }
  ctx.r3.u64 = hash;
}
ITC_NATIVE_STEP(NativeNodePoolConstruct, xa::NodePoolConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeNodePoolFree, xa::NodePoolFree(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeNodePoolAlloc, ctx.r3.u64 = xa::NodePoolAlloc(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeNodePoolFreeChunks, ctx.r3.u64 = xa::NodePoolFreeChunks(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeEngineAddCue, xa::EngineAddCue(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeGlobalSettingsBaseConstruct,
                xa::GlobalSettingsBaseConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeGlobalSettingsConstruct,
                xa::GlobalSettingsConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeVariableInit,
                xa::VariableInit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                 ctx.r7.u32, ctx.r8.u32))
ITC_NATIVE_STEP(NativeCueDetachSound, xa::CueDetachSound(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeCueFindVariable,
                ctx.r3.u64 = xa::CueFindVariable(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeFaderConstruct, xa::FaderConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeFaderDestruct, xa::FaderDestruct(base, ctx.r3.u32))

// The optional-returning functions: 0xDEADDEAD where the native version
// declines (the adapter would run the original).
#define ITC_NATIVE_OPTIONAL(name, expr)                    \
  void name(PPCContext& ctx, uint8_t* base) {              \
    const auto result = (expr);                            \
    ctx.r3.u64 = result ? *result : 0xDEADDEADu;           \
  }
ITC_NATIVE_STEP(NativeSoundBankGetHeader, ctx.r3.u64 = xa::SoundBankGetHeader(base, ctx.r3.u32))
ITC_NATIVE_OPTIONAL(NativeSoundBankGetSimpleCues, xa::SoundBankGetSimpleCues(base, ctx.r3.u32))
ITC_NATIVE_OPTIONAL(NativeSoundBankGetWaveBankNames,
                    xa::SoundBankGetWaveBankNames(base, ctx.r3.u32))
ITC_NATIVE_OPTIONAL(NativeSoundBankGetPointer,
                    xa::SoundBankGetPointer(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_OPTIONAL(NativeSoundBankGetCueCount, xa::SoundBankGetCueCount(base, ctx.r3.u32))
ITC_NATIVE_OPTIONAL(NativeSoundBankFirstEvent,
                    xa::SoundBankFirstEvent(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_OPTIONAL(NativeSoundBankNextEvent,
                    xa::SoundBankNextEvent(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_OPTIONAL(NativeSoundBankGetCueRecord,
                    xa::SoundBankGetCueRecord(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeWaveBankGetEntry,
                ctx.r3.u64 = xa::WaveBankGetEntry(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeWaveBankGetSeekTable,
                ctx.r3.u64 = xa::WaveBankGetSeekTable(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeWaveBankGetName, ctx.r3.u64 = xa::WaveBankGetName(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeWaveBankGetEntryCount,
                ctx.r3.u64 = xa::WaveBankGetEntryCount(base, ctx.r3.u32))
void NativeWaveBankGetWaveFormat(PPCContext& ctx, uint8_t* base) {
  if (!xa::WaveBankGetWaveFormat(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    ctx.r3.u64 = 0xDEADDEAD;
    return;
  }
  ctx.r3.u64 = xa::WaveBankGetEntry(base, ctx.r3.u32, ctx.r4.u32);
}
ITC_NATIVE_STEP(NativeWaveEntryConstruct, xa::WaveEntryConstruct(base, ctx.r3.u32))
void NativeWaveEntrySet(PPCContext& ctx, uint8_t* base) {
  if (!xa::WaveEntrySet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    ctx.r3.u64 = 0xDEADDEAD;
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 12;
}
ITC_NATIVE_STEP(NativeWaveEntryGetLoopRegion, ctx.r3.u64 = xa::WaveEntryGetLoopRegion(ctx.r3.u32))
ITC_NATIVE_STEP(NativeWaveEntryGetDuration,
                ctx.r3.u64 = xa::WaveEntryGetDuration(base, ctx.r3.u32))
void NativeWaveEntrySamplesToMs(PPCContext& ctx, uint8_t* base) {
  uint32_t ms = 0;
  ctx.r3.u64 = xa::WaveEntrySamplesToMs(base, ctx.r3.u32, ctx.r4.u32, &ms) ? ms : 0xDEADDEADu;
}
// FindXmaBlock leaves r3 (the entry) as it was.
ITC_NATIVE_STEP(NativeWaveEntryFindXmaBlock,
                xa::WaveEntryFindXmaBlock(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                          ctx.r7.u32))
ITC_NATIVE_OPTIONAL(NativeWaveEntrySamplesToBytes,
                    xa::WaveEntrySamplesToBytes(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))

// The odd float words mixed into inputs: NaNs of both kinds and signs,
// infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001, 0x7F800000,
                                   0xFF800000, 0x00000001, 0x807FFFFF, 0x00000000, 0x80000000,
                                   0x7F7FFFFF, 0x3F800000};

uint32_t FloatWord(Random& random) {
  switch (random.Below(3)) {
    case 0:
      return kOddFloats[random.Below(std::size(kOddFloats))];
    case 1:
      return uint32_t(random.Next());
    default: {
      const float f = float(int32_t(random.Below(20001)) - 10000) / 7.0f;
      return std::bit_cast<uint32_t>(f);
    }
  }
}

void RandomWords(Context& h, uint32_t address, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(h.base, address + 4 * i, FloatWord(h.random));
  }
}

// Where a case's function stores floats: [offset, end) in the scratch area.
using FloatRanges = std::vector<std::pair<uint32_t, uint32_t>>;

// The scratch area's outputs: the float ranges as floats, the rest as bytes,
// then r3.
void Collect(Context& h, Outputs& out, const FloatRanges& floats, const PPCContext& t) {
  uint32_t at = 0;
  for (const auto& [from, to] : floats) {
    if (from > at) {
      out.Bytes(Read(h.base, h.data + at, from - at));
    }
    out.Floats(h.base, h.data + from, (to - from) / 4);
    at = to;
  }
  out.Bytes(Read(h.base, h.data + at, kScratchBytes - at));
  out.Word(t.r3.u32);
}

// One case through both versions: the area filled, `setup()` writes the
// inputs (the same both times: it's handed a copy of the generator's state),
// `args(t)` sets the registers, the original (flag off) or the native step
// runs, and the area and r3 are compared.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, const FloatRanges& floats, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_xact) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_xact) = true;
    Collect(h, out[v], floats, t);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

void Hash(Context& h) {
  Results results("xact name hash");
  const uint32_t d = h.data;
  const uint32_t sizes[] = {1, 2, 7, 31, 97, 255, 0x8000, 0xFFFF, 0x10005, 0xFFFFFFFFu};
  // The game's category names, then random strings (bytes 1-255).
  const char* names[] = {"", "Global", "Default", "Music", "BGM", "SE", "car_se", "Voice"};
  for (const char* name : names) {
    for (uint32_t size : sizes) {
      Case(
          h, results,
          [&] {
            const uint32_t length = uint32_t(std::char_traits<char>::length(name));
            Write(h.base, d + 64, name, length + 1);
          },
          [&](PPCContext& t) { t.r3.u64 = 0x12345678; t.r4.u64 = d + 64; t.r5.u64 = size; },
          __imp__sub_825BFC50, NativeHashName, {},
          fmt::format("XACTHashName \"{}\" {:#x}", name, size));
    }
  }
  for (int i = 0; i < 200; i++) {
    const uint32_t length = h.random.Below(i < 100 ? 40 : 1000);
    uint32_t size = sizes[h.random.Below(std::size(sizes))];
    if (i % 3 == 0) {
      size = uint32_t(h.random.Next()) | 1;
    }
    Case(
        h, results,
        [&] {
          for (uint32_t c = 0; c < length; c++) {
            guest::StoreU8(h.base, d + 64 + c, uint8_t(1 + h.random.Below(255)));
          }
          guest::StoreU8(h.base, d + 64 + length, 0);
        },
        [&](PPCContext& t) { t.r4.u64 = d + 64; t.r5.u64 = size; }, __imp__sub_825BFC50,
        NativeHashName, {}, fmt::format("XACTHashName random {} ({} bytes)", i, length));
  }
  results.Report();
}

// A pool at `pool` with `used` nodes on its used list and `free` on its free
// list, the nodes from `nodes` (12 bytes apart, in a random order), the
// count random or right.
void RandomPool(Context& h, uint32_t pool, uint32_t nodes, uint32_t used, uint32_t free) {
  RandomWords(h, pool, 54);
  std::vector<uint32_t> order(used + free);
  for (uint32_t i = 0; i < order.size(); i++) {
    order[i] = nodes + 12 * i;
  }
  for (uint32_t i = uint32_t(order.size()); i > 1; i--) {
    std::swap(order[i - 1], order[h.random.Below(i)]);
  }
  for (uint32_t node : order) {
    RandomWords(h, node, 3);
  }
  uint32_t previous = 0;
  for (uint32_t i = 0; i < used; i++) {
    const uint32_t node = order[i];
    guest::StoreU32(h.base, node + 8, previous);
    guest::StoreU32(h.base, node + 4, 0);
    if (previous != 0) {
      guest::StoreU32(h.base, previous + 4, node);
    }
    previous = node;
  }
  guest::StoreU32(h.base, pool + 204, used ? order[0] : 0);
  guest::StoreU32(h.base, pool + 208, used ? order[used - 1] : 0);
  guest::StoreU32(h.base, pool + 212, h.random.Below(2) ? used : uint32_t(h.random.Next()));
  uint32_t head = 0;
  for (uint32_t i = used; i < used + free; i++) {
    guest::StoreU32(h.base, order[i] + 4, head);
    head = order[i];
  }
  guest::StoreU32(h.base, pool, head);
}

void Pools(Context& h) {
  Results results("xact node pools");
  const uint32_t d = h.data;
  constexpr uint32_t kPool = 256, kNodes = 1024;
  for (int i = 0; i < 6; i++) {
    const bool filled = i % 2;
    Case(
        h, results, [&] { if (filled) RandomWords(h, d + kPool, 60); },
        [&](PPCContext& t) { t.r3.u64 = d + kPool; }, __imp__sub_825C2060,
        NativeNodePoolConstruct, {}, fmt::format("XACTNodePool::XACTNodePool {}", i));
  }
  for (int i = 0; i < 150; i++) {
    const uint32_t used = 1 + h.random.Below(i < 20 ? 2 : 12);
    const uint32_t free = h.random.Below(4);
    const uint32_t which = i % 4 == 0 ? 0 : i % 4 == 1 ? used - 1 : h.random.Below(used);
    uint32_t node = 0;
    Case(
        h, results,
        [&] {
          RandomPool(h, d + kPool, d + kNodes, used, free);
          // The used list's `which`th node, walked from the head.
          node = guest::LoadU32(h.base, d + kPool + 204);
          for (uint32_t k = 0; k < which; k++) {
            node = guest::LoadU32(h.base, node + 4);
          }
        },
        [&](PPCContext& t) { t.r3.u64 = d + kPool; t.r4.u64 = node; }, __imp__sub_825C2C60,
        NativeNodePoolFree, {},
        fmt::format("XACTNodePool::Free {} (node {} of {}, {} free)", i, which, used, free));
  }
  for (int i = 0; i < 40; i++) {
    const uint32_t used = h.random.Below(6);
    const uint32_t free = 1 + h.random.Below(5);
    Case(
        h, results, [&] { RandomPool(h, d + kPool, d + kNodes, used, free); },
        [&](PPCContext& t) { t.r3.u64 = d + kPool; }, __imp__sub_825C2CC8, NativeNodePoolAlloc,
        {}, fmt::format("XACTNodePool::Alloc {} ({} free)", i, free));
  }
  results.Report();

  // Allocating a chunk, and freeing the chunks: each version's blocks
  // differ, so words pointing into a version's chunks are compared as
  // (chunk number, offset), and the chunks' bytes as well.
  Results heap("xact node pool chunks");
  if (hooks::NativeHeapActive()) {
    for (int i = 0; i < 12; i++) {
      const uint32_t used = h.random.Below(4);
      Outputs out[2];
      const Random start = h.random;
      for (int v = 0; v < 2; v++) {
        h.random = start;
        Fill(h.base, h.data, kScratchBytes);
        RandomPool(h, d + kPool, d + kNodes, used, 0);
        // A chunk list: the inline chunk, or none.
        const bool inline_chunk = i % 2;
        guest::StoreU32(h.base, d + kPool + 4, inline_chunk ? d + kPool + 8 : 0);
        guest::StoreU32(h.base, d + kPool + 8, 0);
        PPCContext t = CallContext(h);
        t.r3.u64 = d + kPool;
        ITC_FLAG(native_xact) = v == 1;
        (v == 0 ? __imp__sub_825C2CC8 : NativeNodePoolAlloc)(t, h.base);
        ITC_FLAG(native_xact) = true;
        const uint32_t chunk = guest::LoadU32(h.base, d + kPool + 4);
        const uint32_t node = t.r3.u32;
        auto relative = [&](uint32_t word) {
          return word >= chunk && word < chunk + 196 ? 0x40000000u | (word - chunk) : word;
        };
        out[v].Word(node == 0 ? 0 : relative(node));
        out[v].Word(relative(guest::LoadU32(h.base, d + kPool)));
        for (uint32_t k = 0; k < 49; k++) {
          out[v].Word(relative(guest::LoadU32(h.base, chunk + 4 * k)));
        }
        guest::StoreU32(h.base, d + kPool + 4, 0);
        guest::StoreU32(h.base, d + kPool, 0);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        memory::crt::XMemFree(chunk, 0x20840001);
      }
      heap.Check(out[0], out[1], fmt::format("XACTNodePool::Alloc new chunk {}", i));
    }
    for (int i = 0; i < 12; i++) {
      const uint32_t chunks = h.random.Below(5);
      const bool inline_chunk = i % 3 != 0;
      Outputs out[2];
      const Random start = h.random;
      for (int v = 0; v < 2; v++) {
        h.random = start;
        Fill(h.base, h.data, kScratchBytes);
        RandomPool(h, d + kPool, d + kNodes, h.random.Below(3), h.random.Below(3));
        // The chunk list: the allocated ones, then the inline one (last, as
        // the pool makes it), or only allocated ones.
        uint32_t next = 0;
        if (inline_chunk) {
          next = d + kPool + 8;
          guest::StoreU32(h.base, next, 0);
        }
        for (uint32_t c = 0; c < chunks; c++) {
          const uint32_t block = memory::crt::XMemAlloc(h.base, 196, 0x20840001);
          RandomWords(h, block, 49);
          guest::StoreU32(h.base, block, next);
          next = block;
        }
        guest::StoreU32(h.base, d + kPool + 4, next);
        PPCContext t = CallContext(h);
        t.r3.u64 = d + kPool;
        ITC_FLAG(native_xact) = v == 1;
        (v == 0 ? __imp__sub_825C1F30 : NativeNodePoolFreeChunks)(t, h.base);
        ITC_FLAG(native_xact) = true;
        Collect(h, out[v], {}, t);
      }
      heap.Check(out[0], out[1],
                 fmt::format("XACTNodePool::FreeChunks {} ({} chunks{})", i, chunks,
                             inline_chunk ? " and the inline one" : ""));
    }
  }
  heap.Report();
}

void Objects(Context& h) {
  Results results("xact objects");
  const uint32_t d = h.data;
  // XACTEngine::AddCue: the engine at +0 (its list's sentinel at +28: +4
  // the head, +8 the tail), up to five cues from +512, 64 bytes apart.
  for (int i = 0; i < 60; i++) {
    const uint32_t count = h.random.Below(5);
    Case(
        h, results,
        [&] {
          RandomWords(h, d, 64);
          const uint32_t sentinel = d + 28;
          uint32_t previous = sentinel;
          for (uint32_t c = 0; c < count; c++) {
            const uint32_t node = d + 512 + 64 * c + 12;
            RandomWords(h, node - 12, 16);
            guest::StoreU32(h.base, previous + 4, node);
            guest::StoreU32(h.base, node + 8, previous);
            previous = node;
          }
          guest::StoreU32(h.base, previous + 4, sentinel);
          guest::StoreU32(h.base, sentinel + 8, previous);
          RandomWords(h, d + 1024, 16);
        },
        [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = d + 1024; }, __imp__sub_825C1518,
        NativeEngineAddCue, {}, fmt::format("XACTEngine::AddCue {} ({} cues)", i, count));
  }
  for (int i = 0; i < 6; i++) {
    const uint32_t engine = i < 2 ? 0x40001000u + 0x100 * i : uint32_t(h.random.Next());
    auto setup = [&] { RandomWords(h, d + 64, 48); };
    auto args = [&](PPCContext& t) { t.r3.u64 = d + 64; t.r4.u64 = engine; };
    Case(h, results, setup, args, __imp__sub_825C3508, NativeGlobalSettingsBaseConstruct, {},
         fmt::format("XACTGlobalSettingsBase::XACTGlobalSettingsBase {}", i));
    Case(h, results, setup, args, __imp__sub_825C21B0, NativeGlobalSettingsConstruct, {},
         fmt::format("XACTGlobalSettings::XACTGlobalSettings {}", i));
  }
  // XACTVariable::Init: the variable at +256 (its value float at +20), the
  // record at an unaligned offset from +1024.
  for (int i = 0; i < 60; i++) {
    const uint32_t record = d + 1024 + h.random.Below(16);
    const uint32_t engine = uint32_t(h.random.Next());
    const uint32_t cue = i % 3 == 0 ? 0 : uint32_t(h.random.Next());
    const uint32_t index = uint32_t(h.random.Next());
    const uint32_t r8 = i % 2 ? 0 : uint32_t(h.random.Next());
    Case(
        h, results,
        [&] {
          RandomWords(h, d + 256, 64);
          for (uint32_t b = 0; b < 32; b++) {
            guest::StoreU8(h.base, d + 1024 + b, uint8_t(h.random.Next()));
          }
          const uint32_t word = FloatWord(h.random);
          for (uint32_t b = 0; b < 4; b++) {
            guest::StoreU8(h.base, record + 1 + b, uint8_t(word >> (24 - 8 * b)));
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = d + 256;
          t.r4.u64 = engine;
          t.r5.u64 = cue;
          t.r6.u64 = index;
          t.r7.u64 = record;
          t.r8.u64 = r8;
        },
        __imp__sub_825C1DE0, NativeVariableInit, {{256 + 20, 256 + 24}},
        fmt::format("XACTVariable::Init {}", i));
  }
  for (int i = 0; i < 6; i++) {
    auto setup = [&] { RandomWords(h, d + 128, 96); };
    auto args = [&](PPCContext& t) { t.r3.u64 = d + 128; };
    Case(h, results, setup, args, __imp__sub_825C4FC0, NativeCueDetachSound, {},
         fmt::format("XACTCue::DetachSound {}", i));
    Case(h, results, setup, args, __imp__sub_825CC0A0, NativeFaderConstruct,
         {{128 + 4, 128 + 8}, {128 + 20, 128 + 24}}, fmt::format("XACTFader::XACTFader {}", i));
    Case(h, results, setup, args, __imp__sub_825CC0D0, NativeFaderDestruct, {},
         fmt::format("XACTFader::~XACTFader {}", i));
  }
  // XACTCue::FindVariable: the cue at +0 (the list's sentinel at +324, its
  // head at +328), up to eight variables from +1024, 28 bytes apart (their
  // next at +4 and index at +244 don't overlap), in a random order.
  for (int i = 0; i < 120; i++) {
    const uint32_t count = h.random.Below(9);
    const uint32_t wanted = i % 5 == 0 ? uint32_t(h.random.Next()) : h.random.Below(6) |
                                                                          (i % 2 ? 0x30000u : 0);
    Case(
        h, results,
        [&] {
          RandomWords(h, d, 90);
          RandomWords(h, d + 1024, 128);
          std::vector<uint32_t> order(count);
          for (uint32_t k = 0; k < count; k++) {
            order[k] = d + 1024 + 28 * k;
          }
          for (uint32_t k = count; k > 1; k--) {
            std::swap(order[k - 1], order[h.random.Below(k)]);
          }
          uint32_t previous = d + 328 - 4;  // the head word as a "next" field
          for (uint32_t v : order) {
            guest::StoreU32(h.base, previous + 4, v);
            guest::StoreU16(h.base, v + 244, uint16_t(h.random.Below(6)));
            previous = v;
          }
          guest::StoreU32(h.base, previous + 4, d + 324);
        },
        [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = wanted; }, __imp__sub_825C5758,
        NativeCueFindVariable, {},
        fmt::format("XACTCue::FindVariable {} ({} variables)", i, count));
  }
  results.Report();
}


constexpr uint32_t kSoundBankVtable = 0x82075A90;
constexpr uint32_t kWaveBankVtable = 0x82075AE8;

void RandomBytes(Context& h, uint32_t address, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU8(h.base, address + i, uint8_t(h.random.Next()));
  }
}

// A sound bank at the area's start (vtable `vtable`, the buffer at +260
// pointing to +1024), the buffer's bytes random.
void SoundBank(Context& h, uint32_t vtable) {
  RandomWords(h, h.data, 80);
  guest::StoreU32(h.base, h.data, vtable);
  guest::StoreU32(h.base, h.data + 260, h.data + 1024);
  RandomBytes(h, h.data + 1024, 1024);
}

void SoundBanks(Context& h) {
  Results results("xact sound banks");
  const uint32_t d = h.data;
  const uint32_t header = d + 1024;
  // A vtable in the area whose slot +64 isn't GetHeader: the native
  // versions decline (0xDEADDEAD); the originals call through it, so these
  // cases only check the declines.
  for (int i = 0; i < 4; i++) {
    for (int f = 0; f < 6; f++) {
      Outputs out[2];
      Fill(h.base, d, kScratchBytes);
      SoundBank(h, d + 3072);
      RandomWords(h, d + 3072, 32);
      PPCContext t = CallContext(h);
      t.r3.u64 = d;
      t.r4.u64 = d + 512;
      static constexpr GuestFunction kNative[] = {
          NativeSoundBankGetSimpleCues, NativeSoundBankGetWaveBankNames,
          NativeSoundBankGetPointer,    NativeSoundBankGetCueCount,
          NativeSoundBankFirstEvent,    NativeSoundBankNextEvent};
      kNative[f](t, h.base);
      out[0].Word(0xDEADDEAD);
      out[1].Word(t.r3.u32);
      results.Check(out[0], out[1], fmt::format("sound bank slot +64 not GetHeader {} {}", i, f));
    }
  }
  for (int i = 0; i < 40; i++) {
    const bool near = i % 2;
    auto setup = [&] {
      SoundBank(h, kSoundBankVtable);
      if (near) {
        guest::StoreU32(h.base, header + 26, h.random.Below(512));
        guest::StoreU32(h.base, header + 50, h.random.Below(512));
      }
    };
    auto args = [&](PPCContext& t) { t.r3.u64 = d; };
    Case(h, results, setup, args, __imp__sub_825C7AA8, NativeSoundBankGetHeader, {},
         fmt::format("XACTSoundBank::GetHeader {}", i));
    Case(h, results, setup, args, __imp__sub_825C7AB0, NativeSoundBankGetSimpleCues, {},
         fmt::format("XACTSoundBank::GetSimpleCues {}", i));
    Case(h, results, setup, args, __imp__sub_825C7B70, NativeSoundBankGetWaveBankNames, {},
         fmt::format("XACTSoundBank::GetWaveBankNames {}", i));
    Case(h, results, setup, args, __imp__sub_825C6928, NativeSoundBankGetCueCount, {},
         fmt::format("XACTSoundBank::GetCueCount {}", i));
    const uint32_t offset = i < 10 ? i * 0x1001 : uint32_t(h.random.Next());
    Case(h, results, setup, [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = offset; },
         __imp__sub_825C7C90, NativeSoundBankGetPointer, {},
         fmt::format("XACTSoundBank::GetPointer {}", i));
  }
  // The event iterator at +512 over a buffer of random bytes: each event
  // type (the low 5 bits of the word at the cursor) with small and random
  // counts, and the index reaching the count or not.
  for (int i = 0; i < 320; i++) {
    const uint32_t type = uint32_t(i) % 32;
    const uint32_t cursor = h.random.Below(400);
    const uint32_t n = i % 3 == 0 ? h.random.Below(0x10000) : h.random.Below(40);
    const uint8_t count = uint8_t(h.random.Below(4));
    const uint8_t index = i % 4 == 0 ? uint8_t(count - 1) : uint8_t(h.random.Next());
    auto setup = [&] {
      SoundBank(h, kSoundBankVtable);
      RandomWords(h, d + 512, 4);
      guest::StoreU32(h.base, d + 512, h.random.Below(400));
      guest::StoreU32(h.base, d + 516, cursor);
      guest::StoreU8(h.base, d + 520, count);
      guest::StoreU8(h.base, d + 521, index);
      const uint32_t word = guest::LoadU32(h.base, header + cursor);
      guest::StoreU32(h.base, header + cursor, (word & ~0x1Fu) | type);
      // The variable events' counts, 16 bits at +2 past their fixed parts.
      const uint32_t fixed = type == 3 ? 9 : type == 6 ? 16 : type == 11 ? 41 : 7;
      guest::StoreU16(h.base, header + cursor + fixed + 2, uint16_t(n));
    };
    auto args = [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = d + 512; };
    Case(h, results, setup, args, __imp__sub_825C7F98, NativeSoundBankNextEvent, {},
         fmt::format("XACTSoundBank::NextEvent {} (type {})", i, type));
    if (i < 40) {
      Case(h, results, setup, args, __imp__sub_825C7F30, NativeSoundBankFirstEvent, {},
           fmt::format("XACTSoundBank::FirstEvent {}", i));
    }
  }
  // GetCueRecord: simple cues only (a complex cue's table, sub_825C7B10,
  // never ran; the native version leaves it to the original).
  for (int i = 0; i < 60; i++) {
    const uint32_t simple = 1 + h.random.Below(i < 30 ? 20 : 0xFFFF);
    const uint32_t index = h.random.Below(simple) | (i % 3 == 0 ? 0xABCD0000u : 0);
    auto setup = [&] {
      SoundBank(h, kSoundBankVtable);
      guest::StoreU16(h.base, header + 9, uint16_t(simple));
      guest::StoreU32(h.base, header + 26, h.random.Below(1024));
    };
    Case(h, results, setup, [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = index; },
         __imp__sub_825C8230, NativeSoundBankGetCueRecord, {},
         fmt::format("XACTSoundBank::GetCueRecord {} ({} of {})", i, index & 0xFFFF, simple));
  }
  for (int i = 0; i < 4; i++) {
    Outputs out[2];
    Fill(h.base, d, kScratchBytes);
    SoundBank(h, kSoundBankVtable);
    guest::StoreU16(h.base, header + 9, uint16_t(i));
    PPCContext t = CallContext(h);
    t.r3.u64 = d;
    t.r4.u64 = i + h.random.Below(5);
    NativeSoundBankGetCueRecord(t, h.base);
    out[0].Word(0xDEADDEAD);
    out[1].Word(t.r3.u32);
    results.Check(out[0], out[1], fmt::format("XACTSoundBank::GetCueRecord complex {}", i));
  }
  results.Report();
}

// A wave bank at the area's start: the entry table (+272) at +1024, the
// bank data (+284) at +256, the seek segment (+288) at +2048.
void WaveBank(Context& h, uint32_t entries) {
  const uint32_t d = h.data;
  RandomWords(h, d, 80);
  guest::StoreU32(h.base, d, kWaveBankVtable);
  guest::StoreU32(h.base, d + 272, d + 1024);
  guest::StoreU32(h.base, d + 284, d + 512);
  guest::StoreU32(h.base, d + 288, d + 2048);
  RandomWords(h, d + 512, 24);
  guest::StoreU32(h.base, d + 516, entries);
  RandomBytes(h, d + 1024, 1024);
  RandomBytes(h, d + 2048, 1024);
}

// A seek table at `at`: the count (1-12), then cumulative sample counts
// (increasing, or random for some).
void SeekTable(Context& h, uint32_t at, bool sorted) {
  const uint32_t count = 1 + h.random.Below(12);
  guest::StoreU32(h.base, at, count);
  uint32_t total = 0;
  for (uint32_t i = 0; i < count; i++) {
    total += sorted ? 1 + h.random.Below(200000) : uint32_t(h.random.Next());
    guest::StoreU32(h.base, at + 4 + 4 * i, sorted ? total : uint32_t(h.random.Next()));
  }
}

void WaveBanks(Context& h) {
  Results results("xact wave banks");
  const uint32_t d = h.data;
  for (int i = 0; i < 60; i++) {
    const uint32_t entries = 1 + h.random.Below(40);
    const uint32_t index = (i < 30 ? h.random.Below(entries) : uint32_t(h.random.Next() & 0x3F)) |
                           (i % 4 == 0 ? 0x12340000u : 0);
    auto setup = [&] {
      WaveBank(h, entries);
      for (uint32_t k = 0; k < 64; k++) {
        const bool none = h.random.Below(3) == 0;
        guest::StoreU32(h.base, d + 2048 + 4 * k, none ? 0xFFFFFFFFu : h.random.Below(256));
      }
    };
    auto args = [&](PPCContext& t) { t.r3.u64 = d; t.r4.u64 = index; t.r5.u64 = d + 3584 + 1; };
    Case(h, results, setup, args, __imp__sub_825C8398, NativeWaveBankGetEntry, {},
         fmt::format("XACTWaveBank::GetEntry {}", i));
    Case(h, results, setup, args, __imp__sub_825C8580, NativeWaveBankGetSeekTable, {},
         fmt::format("XACTWaveBank::GetSeekTable {}", i));
    Case(h, results, setup, args, __imp__sub_825C85B8, NativeWaveBankGetName, {},
         fmt::format("XACTWaveBank::GetName {}", i));
    Case(h, results, setup, args, __imp__sub_825C85E8, NativeWaveBankGetEntryCount, {},
         fmt::format("XACTWaveBank::GetEntryCount {}", i));
    if (index % 0x10000 < 40) {
      Case(h, results, setup, args, __imp__sub_825CC320, NativeWaveBankGetWaveFormat, {},
           fmt::format("XACTWaveBank::GetWaveFormat {}", i));
    }
  }
  // The wave entry accessor at +3200 (+0 valid, +4 index, +8 the bank, +12
  // the entry's copy), the bank at the area's start.
  constexpr uint32_t kEntry = 3200;
  for (int i = 0; i < 60; i++) {
    const uint32_t index = h.random.Below(40) | (i % 4 == 0 ? 0x55550000u : 0);
    auto setup = [&] {
      WaveBank(h, 40);
      RandomWords(h, d + kEntry, 12);
    };
    auto args = [&](PPCContext& t) { t.r3.u64 = d + kEntry; t.r4.u64 = index; t.r5.u64 = d; };
    Case(h, results, setup, args, __imp__sub_825CED88, NativeWaveEntryConstruct, {},
         fmt::format("XACTWaveEntry::XACTWaveEntry {}", i));
    Case(h, results, setup, args, __imp__sub_825CED98, NativeWaveEntrySet, {},
         fmt::format("XACTWaveEntry::Set {}", i));
    Case(h, results, setup, args, __imp__sub_825CEDF8, NativeWaveEntryGetLoopRegion, {},
         fmt::format("XACTWaveEntry::GetLoopRegion {}", i));
    Case(h, results, setup, args, __imp__sub_825CEE00, NativeWaveEntryGetDuration, {},
         fmt::format("XACTWaveEntry::GetDuration {}", i));
  }
  for (int i = 0; i < 80; i++) {
    const uint32_t samples = i < 40 ? h.random.Below(10000000) : uint32_t(h.random.Next());
    auto setup = [&] {
      RandomWords(h, d + kEntry, 12);
      if (i % 2) {  // a real rate (44.1 kHz) in bits 5-22
        const uint32_t format = guest::LoadU32(h.base, d + kEntry + 16);
        guest::StoreU32(h.base, d + kEntry + 16, (format & ~(0x3FFFFu << 5)) | (44100u << 5));
      }
      if ((guest::LoadU32(h.base, d + kEntry + 16) & (0x3FFFFu << 5)) == 0) {
        guest::StoreU32(h.base, d + kEntry + 16, guest::LoadU32(h.base, d + kEntry + 16) | 0x20);
      }
    };
    Case(h, results, setup, [&](PPCContext& t) { t.r3.u64 = d + kEntry; t.r4.u64 = samples; },
         __imp__sub_825CEE70, NativeWaveEntrySamplesToMs, {},
         fmt::format("XACTWaveEntry::SamplesToMs {}", i));
  }
  // FindXmaBlock: a seek table at +2048 (count 1-12; some unsorted), the
  // two outputs at +3584 and +3588, positions around the blocks' bounds.
  for (int i = 0; i < 200; i++) {
    const bool sorted = i % 5 != 0;
    const uint32_t tag = i % 4;
    auto setup = [&] {
      RandomWords(h, d + kEntry, 12);
      const uint32_t format = guest::LoadU32(h.base, d + kEntry + 16);
      guest::StoreU32(h.base, d + kEntry + 16, (format & ~3u) | tag);
      SeekTable(h, d + 2048, sorted);
    };
    uint32_t position = 0;
    auto args = [&](PPCContext& t) {
      // A position near one of the table's values (read after the set-up).
      const uint32_t count = guest::LoadU32(h.base, d + 2048);
      const uint32_t near = guest::LoadU32(h.base, d + 2052 + 4 * h.random.Below(count));
      position = i % 7 == 0 ? uint32_t(h.random.Next()) : near + h.random.Below(400) - 200;
      t.r3.u64 = d + kEntry;
      t.r4.u64 = position;
      t.r5.u64 = d + 2048;
      t.r6.u64 = d + 3584;
      t.r7.u64 = d + 3588;
    };
    Case(h, results, setup, args, __imp__sub_825CEF50, NativeWaveEntryFindXmaBlock, {},
         fmt::format("XACTWaveEntry::FindXmaBlock {}", i));
  }
  // SamplesToBytes: the entry at +3200 for the bank at the area's start
  // (its seek segment at +2048: the wave's offset word, then the table
  // after the entry count's words), by format tag; the out word at +3584
  // or none.
  for (int i = 0; i < 200; i++) {
    const uint32_t tag = i % 4;
    const bool out = i % 3 != 0;
    const uint32_t index = h.random.Below(4);
    auto setup = [&] {
      WaveBank(h, 4);
      RandomWords(h, d + kEntry, 12);
      guest::StoreU32(h.base, d + kEntry + 8, d);
      guest::StoreU16(h.base, d + kEntry + 4, uint16_t(index));
      const uint32_t format = guest::LoadU32(h.base, d + kEntry + 16);
      guest::StoreU32(h.base, d + kEntry + 16, (format & ~3u) | tag);
      for (uint32_t k = 0; k < 4; k++) {
        guest::StoreU32(h.base, d + 2048 + 4 * k, 64 * k);
      }
      SeekTable(h, d + 2048 + 16 + 64 * index, i % 5 != 0);
    };
    Case(
        h, results, setup,
        [&](PPCContext& t) {
          t.r3.u64 = d + kEntry;
          t.r4.u64 = i % 7 == 0 ? uint32_t(h.random.Next()) : h.random.Below(1200000);
          t.r5.u64 = out ? d + 3584 : 0;
        },
        __imp__sub_825CF080, NativeWaveEntrySamplesToBytes, {},
        fmt::format("XACTWaveEntry::SamplesToBytes {} (tag {})", i, tag));
  }
  results.Report();
}

}  // namespace

void Xact(Context& h) {
  Hash(h);
  Pools(h);
  Objects(h);
  SoundBanks(h);
  WaveBanks(h);
}

}  // namespace harness
