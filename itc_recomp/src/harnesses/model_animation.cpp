// The model animation (src/model_animation/, docs/functions/model-animation.md)
// against the recompiled functions. Each case fills the scratch area, writes
// a model's track sets, their sections, tracks, channels and keys (random
// times, often equal, sorted or not; random flags; NaNs, infinities and
// denormals among the floats), runs the original (with
// --native_model_animation off, so its callees are the recompiled code too)
// or the plain C++, and compares the whole scratch area: the words the
// functions store as floats (the values, the sets' +12) with
// harness::Outputs' Float (two NaNs match), everything else exactly, then r3
// or f1. The set-ups and releases allocate and free through the C runtime's
// heap: they run only with --native_heap (their adapters' condition); the
// blocks they make are compared by contents, not address, and freed.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <iterator>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_model_animation);

namespace harness {

namespace {

namespace ma = model_animation;

// The register steps: the cases run the plain C++ through these, with the
// registers each adapter (or caller) leaves.
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeClear, ma::TrackSetClear(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeConstruct, ctx.r3.u64 = ma::TrackSetConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeView, ma::TrackSetView(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeCount, ctx.r3.u64 = ma::TrackSetCount(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeStartTime, ctx.f1.f64 = ma::TrackSetStartTime(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeEndTime, ctx.f1.f64 = ma::TrackSetEndTime(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeTrack, ctx.r3.u64 = ma::TrackSetTrack(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeChannels, ctx.r3.u64 = ma::TrackSetChannels(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeCurveSample,
                ctx.f1.f64 = ma::CurveSample(base, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeKeyInterpolate,
                ctx.f1.f64 = ma::KeyInterpolate(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSampleFloat4, ma::TrackSetSampleFloat4(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeAdvance,
                ctx.r3.u64 = ma::TrackSetAdvance(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeSample32, ma::TrackSetSample32(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeAdvance32,
                ctx.r3.u64 = ma::TrackSetAdvance32(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64))
ITC_NATIVE_STEP(NativeAnimate, ctx.r3.u64 = ma::ModelAnimate(base, ctx.r3.u32))

// The lifetime's steps as their adapters: the original for what the plain
// C++ hands back.
#define ITC_LIFETIME_STEP(name, address, call)  \
  void name(PPCContext& ctx, uint8_t* base) {   \
    ctx.fpscr.disableFlushMode();               \
    if (!(call)) {                              \
      __imp__sub_##address(ctx, base);          \
    }                                           \
  }

ITC_LIFETIME_STEP(NativeReleaseArray, 825A11D0, ma::TrackSetReleaseArray(base, ctx.r3.u32))
ITC_LIFETIME_STEP(NativeRelease, 825A1BA8, ma::TrackSetRelease(base, ctx.r3.u32))
ITC_LIFETIME_STEP(NativeSetupFloat4, 825A1E08,
                  ma::TrackSetSetupFloat4(base, ctx.r3.u32, ctx.r4.u32))
ITC_LIFETIME_STEP(NativeSetupFloat, 825A1BE8, ma::TrackSetSetupFloat(base, ctx.r3.u32, ctx.r4.u32))
ITC_LIFETIME_STEP(NativeSetup32, 825A1218, ma::TrackSetSetup32(base, ctx.r3.u32, ctx.r4.u32))
ITC_LIFETIME_STEP(NativeSetup208, 825A1A38, ma::TrackSetSetup208(base, ctx.r3.u32, ctx.r4.u32))

#undef ITC_LIFETIME_STEP
#undef ITC_NATIVE_STEP

// NaNs of both kinds and signs, infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001,
                                   0x7F800000, 0xFF800000, 0x00000001, 0x807FFFFF,
                                   0x00000000, 0x80000000, 0x7F7FFFFF, 0x3F800000};

// A float word: mostly quarter steps in [-4, 4] (so times are often equal),
// some ordinary values, odd floats and random bits.
uint32_t FloatWord(Random& r) {
  switch (r.Below(10)) {
    case 0:
      return kOddFloats[r.Below(std::size(kOddFloats))];
    case 1:
      return uint32_t(r.Next());
    case 2:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(1 << 20)) - (1 << 19)) / 4096.0f);
    default:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(33)) - 16) / 4.0f);
  }
}

// A time: as FloatWord, without the odd ones most of the time.
uint32_t TimeWord(Random& r) {
  if (r.Below(8) == 0) {
    return FloatWord(r);
  }
  return std::bit_cast<uint32_t>(float(int32_t(r.Below(33)) - 16) / 4.0f);
}

uint32_t FlagsWord(Random& r) {
  static const uint32_t kFlags[] = {0, 2, 8, 10, 1, 4, 0x12, 0x80000008};
  return r.Below(8) == 0 ? uint32_t(r.Next()) : kFlags[r.Below(std::size(kFlags))];
}

void StoreFloatWord(uint8_t* base, uint32_t address, uint32_t word) {
  guest::StoreU32(base, address, word);
}

// `count` 20-byte keys at `keys`: times sorted most of the time.
void WriteKeys(Context& h, uint32_t keys, uint32_t count) {
  std::vector<uint32_t> words;
  for (uint32_t i = 0; i < count; i++) {
    words.push_back(TimeWord(h.random));
  }
  if (h.random.Below(4) != 0) {
    std::sort(words.begin(), words.end(), [](uint32_t a, uint32_t b) {
      const float fa = std::bit_cast<float>(a), fb = std::bit_cast<float>(b);
      if (std::isnan(fa) || std::isnan(fb)) {
        return a < b;
      }
      return fa < fb;
    });
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t key = keys + 20 * i;
    StoreFloatWord(h.base, key + 0, words[i]);
    guest::StoreU32(h.base, key + 4, FlagsWord(h.random));
    StoreFloatWord(h.base, key + 8, FloatWord(h.random));
    StoreFloatWord(h.base, key + 12, FloatWord(h.random));
    StoreFloatWord(h.base, key + 16, FloatWord(h.random));
  }
}

// The area's layout for the sampling cases.
struct Layout {
  uint32_t model;      // +304 and +364: the two sets
  uint32_t sections[2];
  uint32_t values[2];  // 16- and 32-byte values
};

Layout MakeLayout(const Context& h) {
  return {h.data, {h.data + 512, h.data + 1536}, {h.data + 3072, h.data + 3200}};
}

constexpr uint32_t kMaxTracks = 3;
constexpr uint32_t kMaxChannels = 2;
constexpr uint32_t kMaxKeys = 4;

// A section at `section`: start and end times, `tracks` 80-byte tracks, each
// with up to kMaxChannels channels of up to kMaxKeys keys after them.
// `components`: the channels' components are below it (else random).
void WriteSection(Context& h, uint32_t section, uint32_t tracks, uint32_t components) {
  StoreFloatWord(h.base, section + 4, TimeWord(h.random));
  StoreFloatWord(h.base, section + 8, TimeWord(h.random));
  guest::StoreU32(h.base, section + 16, tracks);
  uint32_t channel_at = 20 + 80 * kMaxTracks;
  for (uint32_t t = 0; t < tracks; t++) {
    const uint32_t track = section + 20 + 80 * t;
    const uint32_t channels = h.random.Below(kMaxChannels + 1);
    guest::StoreU32(h.base, track + 64, channels);
    guest::StoreU32(h.base, track + 68, channel_at);
    StoreFloatWord(h.base, track + 72, TimeWord(h.random));
    StoreFloatWord(h.base, track + 76, TimeWord(h.random));
    for (uint32_t c = 0; c < channels; c++) {
      const uint32_t channel = section + channel_at;
      const uint32_t keys = h.random.Below(kMaxKeys + 1);
      guest::StoreU32(h.base, channel + 0, keys);
      guest::StoreU32(h.base, channel + 4, h.random.Below(components));
      WriteKeys(h, channel + 8, keys);
      channel_at += 8 + 20 * keys;
    }
  }
}

// A set bound to `section` (or cleared), its values at `values`.
void WriteSet(Context& h, uint32_t set, uint32_t section, uint32_t values) {
  guest::StoreU32(h.base, set + 0, section);
  guest::StoreU32(h.base, set + 4, section == 0 ? 0 : section + 16);
  guest::StoreU32(h.base, set + 8, section == 0 ? 0 : section + 20);
  StoreFloatWord(h.base, set + 12, TimeWord(h.random));
  guest::StoreU32(h.base, set + 16, values);
}

// `count` values of `stride` bytes: floats (the 32-byte values' +20, the
// mask, a word).
void WriteValues(Context& h, uint32_t values, uint32_t count, uint32_t stride) {
  for (uint32_t i = 0; i < count * stride / 4; i++) {
    StoreFloatWord(h.base, values + 4 * i, i % 2 == 1 ? TimeWord(h.random) : FloatWord(h.random));
  }
}

// The scratch area into `out`: the words at `floats` as floats, the rest
// exactly.
void AreaOutputs(Context& h, Outputs& out, const std::vector<uint32_t>& floats) {
  std::vector<uint32_t> sorted = floats;
  std::sort(sorted.begin(), sorted.end());
  uint32_t at = h.data;
  for (uint32_t word : sorted) {
    if (word < at || word + 4 > h.data + kScratchBytes) {
      continue;
    }
    if (word > at) {
      out.Bytes(Read(h.base, at, word - at));
    }
    out.Float(guest::LoadU32(h.base, word));
    at = word + 4;
  }
  if (at < h.data + kScratchBytes) {
    out.Bytes(Read(h.base, at, h.data + kScratchBytes - at));
  }
}

// The float words of the layout: the sets' +12 and every value word but the
// 32-byte values' masks (+20).
std::vector<uint32_t> LayoutFloats(const Layout& l) {
  std::vector<uint32_t> floats = {l.model + 304 + 12, l.model + 364 + 12};
  for (uint32_t i = 0; i < kMaxTracks * 4 + 4; i++) {
    floats.push_back(l.values[0] + 4 * i);
  }
  for (uint32_t i = 0; i < kMaxTracks * 8 + 8; i++) {
    if (i % 8 != 5) {
      floats.push_back(l.values[1] + 4 * i);
    }
  }
  return floats;
}

enum class Result { kNone, kR3, kF1 };

// One case through both versions: the area filled, `setup()` writes the
// inputs (the generator's state the same both times), `args(t)` sets the
// registers, the original (flag off) or the native step runs, and the area
// (`floats` as floats), then r3 or f1, are compared.
template <typename Setup, typename Args>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, Result result, const std::vector<uint32_t>& floats,
          const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_model_animation) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_model_animation) = true;
    AreaOutputs(h, out[v], floats);
    if (result == Result::kR3) {
      out[v].Word(t.r3.u32);
    } else if (result == Result::kF1) {
      out[v].Double(t.f1.u64);
    }
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// A model with both sets bound (or not) to random sections, and values.
void WriteModel(Context& h, const Layout& l) {
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t set = l.model + (i == 0 ? 304 : 364);
    const uint32_t tracks = h.random.Below(kMaxTracks + 1);
    const bool bound = h.random.Below(6) != 0;
    // The components: the float4's four lanes; the 32-byte values' first
    // five words (+20 is the mask, a word).
    WriteSection(h, l.sections[i], tracks, i == 0 ? 4 : 5);
    WriteSet(h, set, bound ? l.sections[i] : 0, l.values[i]);
    WriteValues(h, l.values[i], kMaxTracks, i == 0 ? 16 : 32);
  }
}

void Sampling(Context& h) {
  const Layout l = MakeLayout(h);
  const std::vector<uint32_t> floats = LayoutFloats(l);
  const uint32_t sets[2] = {l.model + 304, l.model + 364};
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
    Result result;
    uint32_t set;  // which set (0 or 1)
    bool time;     // f1 = a step or a time
  };
  const Function functions[] = {
      {"sub_825A6670 TrackSet::Clear", __imp__sub_825A6670, NativeClear, Result::kNone, 0},
      {"sub_825A1B70 TrackSet::TrackSet", __imp__sub_825A1B70, NativeConstruct, Result::kR3, 0},
      {"sub_825A6690 TrackSet::View", __imp__sub_825A6690, NativeView, Result::kNone, 0},
      {"sub_825A66D8 TrackSet::Count", __imp__sub_825A66D8, NativeCount, Result::kR3, 0},
      {"sub_825A1F08 TrackSet::StartTime", __imp__sub_825A1F08, NativeStartTime, Result::kF1, 0},
      {"sub_825A1F50 TrackSet::EndTime", __imp__sub_825A1F50, NativeEndTime, Result::kF1, 0},
      {"sub_825A2000 TrackSet::Track", __imp__sub_825A2000, NativeTrack, Result::kR3, 0},
      {"sub_825A2030 TrackSet::Channels", __imp__sub_825A2030, NativeChannels, Result::kR3, 0},
      {"sub_825A1C88 TrackSet::SampleFloat4", __imp__sub_825A1C88, NativeSampleFloat4,
       Result::kNone, 0},
      {"sub_825A1D68 TrackSet::Advance", __imp__sub_825A1D68, NativeAdvance, Result::kR3, 0,
       true},
      {"sub_825A0FE0 TrackSet::Sample32", __imp__sub_825A0FE0, NativeSample32, Result::kNone, 1},
      {"sub_825A10A8 TrackSet::Advance32", __imp__sub_825A10A8, NativeAdvance32, Result::kR3, 1,
       true},
      {"sub_82594B60 Model::Animate", __imp__sub_82594B60, NativeAnimate, Result::kR3, 0},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (int i = 0; i < 400; i++) {
      uint32_t index = 0, section = 0;
      uint64_t step = 0;
      Case(
          h, results,
          [&] {
            WriteModel(h, l);
            const uint32_t tracks = guest::LoadU32(h.base, l.sections[f.set] + 16);
            // The index: one of the tracks (as the callers pass), else any
            // below kMaxTracks; -1 now and then for the times.
            index = tracks != 0 && h.random.Below(4) != 0 ? h.random.Below(tracks)
                                                          : h.random.Below(kMaxTracks);
            if (h.random.Below(5) == 0) {
              index = 0xFFFFFFFFu;
              if (std::string(f.name).find("Time") == std::string::npos) {
                index = 0;
              }
            }
            section = h.random.Below(5) == 0 ? 0 : l.sections[f.set];
            step = std::bit_cast<uint64_t>(
                h.random.Below(3) != 0 ? 1.0
                                       : double(std::bit_cast<float>(FloatWord(h.random))));
          },
          [&](PPCContext& t) {
            t.r3.u64 = f.name == std::string("sub_82594B60 Model::Animate") ? l.model
                                                                             : sets[f.set];
            t.r4.u64 = f.recompiled == __imp__sub_825A6690 ? section : index;
            t.f1.u64 = step;
          },
          f.recompiled, f.native, f.result, floats, fmt::format("case {}", i));
    }
    results.Report();
  }
}

// Curve::Sample and Key::Interpolate on keys alone: f1 compared.
void Curves(Context& h) {
  const uint32_t keys = h.data + 64;
  {
    Results results("sub_825A1F98 Curve::Sample");
    for (int i = 0; i < 2000; i++) {
      uint32_t count = 0;
      uint64_t time = 0;
      Case(
          h, results,
          [&] {
            count = h.random.Below(8);
            if (h.random.Below(20) == 0) {
              count = uint32_t(-int32_t(h.random.Below(3)));  // 0, -1, -2: the first key
            }
            WriteKeys(h, keys, std::max<uint32_t>(int32_t(count) > 0 ? count : 1, 1));
            time = std::bit_cast<uint64_t>(double(std::bit_cast<float>(TimeWord(h.random))));
          },
          [&](PPCContext& t) {
            t.r4.u64 = keys;
            t.r5.u64 = count;
            t.f1.u64 = time;
          },
          __imp__sub_825A1F98, NativeCurveSample, Result::kF1, {}, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_825A66F8 Key::Interpolate");
    for (int i = 0; i < 2000; i++) {
      uint64_t time = 0;
      Case(
          h, results,
          [&] {
            WriteKeys(h, keys, 2);
            time = std::bit_cast<uint64_t>(double(std::bit_cast<float>(TimeWord(h.random))));
          },
          [&](PPCContext& t) {
            t.r3.u64 = keys;
            t.r4.u64 = keys + 20;
            t.f1.u64 = time;
          },
          __imp__sub_825A66F8, NativeKeyInterpolate, Result::kF1, {},
          fmt::format("case {}", i));
    }
    results.Report();
  }
}

// The set-ups and releases: a set at the area's start whose old values (or
// none) come from the allocation layer, a section with 0 to 4 tracks. After
// each side, the set (its values pointer as whether it's set), the new
// block's count header and values, and whether the old block was freed are
// compared, and the new block is freed.
void Lifetime(Context& h) {
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: model_animation: the set-ups and releases skipped (--native_heap off)");
    return;
  }
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t stride;  // the values' size (0: a release)
    uint32_t header;  // 16: a count header before the values
    uint32_t time;    // where a value's time is
  };
  const Function functions[] = {
      {"sub_825A11D0 TrackSet::ReleaseArray", __imp__sub_825A11D0, NativeReleaseArray, 0, 16, 0},
      {"sub_825A1BA8 TrackSet::Release", __imp__sub_825A1BA8, NativeRelease, 0, 0, 0},
      {"sub_825A1E08 TrackSet::SetupFloat4", __imp__sub_825A1E08, NativeSetupFloat4, 16, 16, 12},
      {"sub_825A1BE8 TrackSet::SetupFloat", __imp__sub_825A1BE8, NativeSetupFloat, 4, 0, 0},
      {"sub_825A1218 TrackSet::Setup32", __imp__sub_825A1218, NativeSetup32, 32, 16, 16},
      {"sub_825A1A38 TrackSet::Setup208", __imp__sub_825A1A38, NativeSetup208, 208, 16, 0},
  };
  const uint32_t set = h.data;
  const uint32_t section = h.data + 64;
  for (const Function& f : functions) {
    Results results(f.name);
    for (int i = 0; i < 200; i++) {
      Outputs out[2];
      const Random start = h.random;
      for (int v = 0; v < 2; v++) {
        h.random = start;
        Fill(h.base, h.data, kScratchBytes);
        const bool has_old = h.random.Below(3) != 0;
        const uint32_t old_count = 1 + h.random.Below(3);
        uint32_t old = 0;
        if (has_old) {
          old = *memory::crt::OperatorNewArray(h.base, 16 + 16 * old_count);
          guest::StoreU32(h.base, old, old_count);
        }
        // Setup208's original sets up tracks with sub_825A6608 and
        // sub_825A1938 (never in play), which read 352-byte tracks this
        // layout doesn't have: its cases keep to sections with none.
        const uint32_t tracks = f.stride == 208 ? 0 : h.random.Below(5);
        WriteSection(h, section, tracks, 4);
        const bool bound = h.random.Below(5) != 0;
        WriteSet(h, set, h.random.Below(2) == 0 ? section : 0, has_old ? old + f.header : 0);
        PPCContext t = CallContext(h);
        t.r3.u64 = set;
        t.r4.u64 = bound ? section : 0;
        ITC_FLAG(native_model_animation) = v == 1;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        ITC_FLAG(native_model_animation) = true;
        out[v].Bytes(Read(h.base, set, 12));
        out[v].Float(guest::LoadU32(h.base, set + 12));
        const uint32_t values = guest::LoadU32(h.base, set + 16);
        out[v].Word(values != 0);
        out[v].Word(has_old && memory::IsHeapBlock(old));
        out[v].Bytes(Read(h.base, set + 20, kScratchBytes - 20));
        if (values != 0 && f.stride != 0) {
          const uint32_t count = guest::LoadU32(h.base, set + 4) != 0
                                     ? guest::LoadU32(h.base, guest::LoadU32(h.base, set + 4))
                                     : 0;
          if (f.header != 0) {
            out[v].Word(guest::LoadU32(h.base, values - 16));
          }
          for (uint32_t k = 0; k < count * f.stride / 4; k++) {
            out[v].Float(guest::LoadU32(h.base, values + 4 * k));
          }
          memory::crt::OperatorDeleteArray(values - f.header);
        }
        if (has_old && memory::IsHeapBlock(old)) {
          memory::crt::OperatorDeleteArray(old);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", i));
    }
    results.Report();
  }
}

}  // namespace

void ModelAnimation(Context& h) {
  Sampling(h);
  Curves(h);
  Lifetime(h);
}

}  // namespace harness
