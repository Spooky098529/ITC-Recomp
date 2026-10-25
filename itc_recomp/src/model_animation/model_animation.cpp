#include "model_animation/model_animation.h"

#include "guest.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "xact/xact.h"

#include <cstdlib>

namespace model_animation {

namespace {

using namespace car_physics::ops;

constexpr uint32_t kTwo = 0x820021E8;  // the game's float 2.0
// malloc's largest size (memory/c_runtime_heap.cpp's kMaxSize).
constexpr uint32_t kMaxAllocation = 0xFFFFF000;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// Whether operator delete frees `block` natively: 0, or a block the layer
// gave (else free's error path, the original's).
bool Deletable(uint32_t block) {
  return block == 0 || memory::IsHeapBlock(block);
}

// Whether operator new gives `size` bytes natively (malloc's checks; the
// layer aborts when out of memory): else the new handler and
// std::bad_alloc, the original's.
bool Allocatable(uint8_t* base, uint32_t size) {
  return size <= kMaxAllocation && memory::crt::GetProcessHeap(base) != 0;
}

// operator new[] once Allocatable said yes.
uint32_t NewArray(uint8_t* base, uint32_t size) {
  const auto block = memory::crt::OperatorNewArray(base, size);
  if (!block) {
    ITC_LOG_ERROR("model animation: operator new[]({}) failed after its checks", size);
    std::abort();
  }
  return *block;
}

// The track count a set will have once bound to `section` (TrackSet::View
// then TrackSet::Count), read before anything is written, for the set-ups'
// checks.
uint32_t SectionCount(uint8_t* base, uint32_t section) {
  return section == 0 ? 0 : Load(base, section + 16);
}

// SetupFloat4 and Setup32: `stride`-byte values with a count header, at most
// `max_count` (else operator new[](-1)), the time at +`time`.
bool SetupWithHeader(uint8_t* base, uint32_t set, uint32_t section, uint32_t stride,
                     uint32_t max_count, uint32_t time) {
  const uint32_t old = Load(base, set + 16);
  const uint32_t count = SectionCount(base, section);
  uint32_t size = 0xFFFFFFFFu;
  if (count <= max_count && count * stride <= 0xFFFFFFEFu) {
    size = count * stride + 16;
  }
  if ((old != 0 && !Deletable(old - 16)) || (count != 0 && !Allocatable(base, size))) {
    return false;
  }
  if (old != 0) {
    memory::crt::OperatorDeleteArray(old - 16);
  }
  Store(base, set + 16, 0);
  TrackSetView(base, set, section);
  uint32_t n = TrackSetCount(base, set);
  if (n == 0) {
    return true;
  }
  const uint32_t block = NewArray(base, size);
  uint32_t values = 0;
  if (block != 0) {
    Store(base, block, n);
    values = block + 16;
    int32_t left = int32_t(n) - 1;
    if (left >= 0) {
      const double one = Lfs(base, kOne);
      uint32_t at = values + 12;
      do {
        left--;
        Stfs(base, at, one);
        at += stride;
      } while (left >= 0);
    }
  }
  Store(base, set + 16, values);
  if (!(int32_t(n) > 0)) {
    return true;
  }
  uint32_t offset = 0;
  do {
    const double start = TrackSetStartTime(base, set, 0xFFFFFFFFu);
    const uint32_t at = offset + Load(base, set + 16);
    n--;
    offset += stride;
    Stfs(base, at + time, start);
  } while (n != 0);
  return true;
}

// Frees +16 (less `header`) and clears it.
bool ReleaseValues(uint8_t* base, uint32_t set, uint32_t header) {
  const uint32_t values = Load(base, set + 16);
  if (values != 0 && !Deletable(values - header)) {
    return false;
  }
  if (values != 0) {
    memory::crt::OperatorDeleteArray(values - header);
  }
  Store(base, set + 16, 0);
  return true;
}

}  // namespace

void TrackSetClear(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A6670 TrackSet::Clear");
  Stfs(base, set + 12, Lfs(base, kZero));
  Store(base, set + 0, 0);
  Store(base, set + 4, 0);
  Store(base, set + 8, 0);
}

uint32_t TrackSetConstruct(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A1B70 TrackSet::TrackSet");
  TrackSetClear(base, set);
  Store(base, set + 16, 0);
  return set;
}

void TrackSetView(uint8_t* base, uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A6690 TrackSet::View");
  if (section == 0) {
    Stfs(base, set + 12, Lfs(base, kZero));
    Store(base, set + 0, 0);
    Store(base, set + 4, 0);
    Store(base, set + 8, 0);
    return;
  }
  Store(base, set + 0, section);
  Store(base, set + 4, section + 16);
  Store(base, set + 8, section + 20);
  Stfs(base, set + 12, Lfs(base, section + 4));
}

uint32_t TrackSetCount(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A66D8 TrackSet::Count");
  if (Load(base, set + 0) == 0) {
    return 0;
  }
  return Load(base, Load(base, set + 4));
}

namespace {

// StartTime and EndTime: the track's +`track_offset`, the section's
// +`section_offset` for -1.
double TrackTime(uint8_t* base, uint32_t set, uint32_t track, uint32_t section_offset,
                 uint32_t track_offset) {
  const uint32_t section = Load(base, set + 0);
  if (section == 0) {
    return Lfs(base, kZero);
  }
  if (int32_t(track) == -1) {
    return Lfs(base, section + section_offset);
  }
  return Lfs(base, track * 80 + Load(base, set + 8) + track_offset);
}

}  // namespace

double TrackSetStartTime(uint8_t* base, uint32_t set, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825A1F08 TrackSet::StartTime");
  return TrackTime(base, set, track, 4, 72);
}

double TrackSetEndTime(uint8_t* base, uint32_t set, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825A1F50 TrackSet::EndTime");
  return TrackTime(base, set, track, 8, 76);
}

uint32_t TrackSetTrack(uint8_t* base, uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A2000 TrackSet::Track");
  if (Load(base, set + 0) == 0) {
    return 0;
  }
  return index * 80 + Load(base, set + 8);
}

uint32_t TrackSetChannels(uint8_t* base, uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A2030 TrackSet::Channels");
  const uint32_t section = Load(base, set + 0);
  if (section == 0) {
    return 0;
  }
  return Load(base, index * 80 + Load(base, set + 8) + 68) + section;
}

double KeyInterpolate(uint8_t* base, uint32_t key0, uint32_t key1, double time) {
  ITC_NATIVE_RUNNING("sub_825A66F8 Key::Interpolate");
  const uint32_t flags = Load(base, key0 + 4);
  if ((flags & 2) != 0) {
    // Linear.
    const double v0 = Lfs(base, key0 + 8);
    const double t0 = Lfs(base, key0 + 0);
    const double v1 = Lfs(base, key1 + 8);
    const double t1 = Lfs(base, key1 + 0);
    const double dv = Fsubs(v1, v0);
    const double dt = Fsubs(t1, t0);
    const double x = Fsubs(time, t0);
    return Fmadds(Fdivs(dv, dt), x, v0);
  }
  if ((flags & 8) != 0) {
    // Stepped.
    return Lfs(base, key0 + 8);
  }
  // Cubic Hermite: the first key's out tangent (+16), the second's in (+12).
  const double t0 = Lfs(base, key0 + 0);
  const double t1 = Lfs(base, key1 + 0);
  double f11 = Fsubs(time, t0);
  const double span = Fsubs(t1, t0);
  const double v0 = Lfs(base, key0 + 8);
  const double v1 = Lfs(base, key1 + 8);
  const double f6 = Fsubs(v0, v1);
  const double in1 = Lfs(base, key1 + 12);
  const double two = Lfs(base, kTwo);
  const double out0 = Lfs(base, key0 + 16);
  const double one = Lfs(base, kOne);
  const double s = Fdivs(f11, span);
  double f13 = Fsubs(s, one);
  const double f8 = Fmuls(in1, s);
  double f10 = Fmsubs(f13, two, one);
  f11 = Fmuls(f13, f11);
  f13 = Fmadds(out0, f13, f8);
  f10 = Fmuls(f6, f10);
  f10 = Fmuls(f10, s);
  double f0 = Fmuls(f10, s);
  f0 = Fmadds(f13, f11, f0);
  return Fadds(f0, v0);
}

double CurveSample(uint8_t* base, uint32_t keys, uint32_t count, double time) {
  ITC_NATIVE_RUNNING("sub_825A1F98 Curve::Sample");
  if (!(int32_t(count) > 1)) {
    return Lfs(base, keys + 8);
  }
  const uint32_t end = count * 20 + keys;
  if (!(time < Lfs(base, end - 20))) {
    return Lfs(base, end - 12);
  }
  uint32_t key = keys + 20;
  if (!(time < Lfs(base, key))) {
    do {
      key += 20;
    } while (!(time < Lfs(base, key)));
  }
  if (time == Lfs(base, key)) {
    return Lfs(base, key + 8);
  }
  return KeyInterpolate(base, key - 20, key, time);
}

void TrackSetSampleFloat4(uint8_t* base, uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A1C88 TrackSet::SampleFloat4");
  if (Load(base, set + 0) == 0) {
    return;
  }
  const uint32_t track = TrackSetTrack(base, set, index);
  uint32_t channel = TrackSetChannels(base, set, index);
  const uint32_t offset = index << 4;
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  Stfs(base, offset + Load(base, set + 16), zero);
  Stfs(base, offset + Load(base, set + 16) + 4, zero);
  Stfs(base, offset + Load(base, set + 16) + 8, one);
  if (!(int32_t(Load(base, track + 64)) > 0)) {
    return;
  }
  const uint32_t lanes = index << 2;
  int32_t i = 0;
  do {
    const uint32_t keys = channel + 8;
    const uint32_t values = Load(base, set + 16);
    const uint32_t lane = (Load(base, channel + 4) + lanes) << 2;
    const uint32_t key_count = Load(base, channel + 0);
    const double value = CurveSample(base, keys, key_count, Lfs(base, values + offset + 12));
    Stfs(base, lane + values, value);
    channel = Load(base, channel + 0) * 20 + keys;
    i++;
  } while (i < int32_t(Load(base, track + 64)));
}

void TrackSetSample32(uint8_t* base, uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A0FE0 TrackSet::Sample32");
  if (Load(base, set + 0) == 0) {
    return;
  }
  const uint32_t track = TrackSetTrack(base, set, index);
  uint32_t channel = TrackSetChannels(base, set, index);
  const uint32_t offset = index << 5;
  const uint32_t value = offset + Load(base, set + 16);
  Store(base, value + 20, 0);
  if (!(int32_t(Load(base, track + 64)) > 0)) {
    return;
  }
  int32_t i = 0;
  do {
    const uint32_t component = Load(base, channel + 4);
    const uint32_t keys = channel + 8;
    const uint32_t mask = Load(base, value + 20);
    // slw: shifts of 32 to 63 give 0.
    const uint32_t bit = (component & 0x20) != 0 ? 0 : 1u << (component & 0x1F);
    Store(base, value + 20, bit | mask);
    const uint32_t key_count = Load(base, channel + 0);
    const double sample =
        CurveSample(base, keys, key_count, Lfs(base, offset + Load(base, set + 16) + 16));
    Stfs(base, (component << 2) + value, sample);
    channel = Load(base, channel + 0) * 20 + keys;
    i++;
  } while (i < int32_t(Load(base, track + 64)));
}

namespace {

// Advance and Advance32: the time at value + `time`, `stride`-byte values.
uint32_t AdvanceTime(uint8_t* base, uint32_t set, uint32_t index, double step, uint32_t stride,
                     uint32_t time) {
  if (Load(base, set + 0) == 0) {
    return 0;
  }
  const uint32_t at = index * stride + Load(base, set + 16) + time;
  Stfs(base, at, Fadds(step, Lfs(base, at)));
  const double end = TrackSetEndTime(base, set, index);
  if (Lfs(base, at) > end) {
    Stfs(base, at, TrackSetStartTime(base, set, index));
    return 1;
  }
  const double start = TrackSetStartTime(base, set, index);
  if (!(Lfs(base, at) < start)) {
    return 0;
  }
  Stfs(base, at, TrackSetEndTime(base, set, index));
  return 1;
}

}  // namespace

uint32_t TrackSetAdvance(uint8_t* base, uint32_t set, uint32_t index, double step) {
  ITC_NATIVE_RUNNING("sub_825A1D68 TrackSet::Advance");
  return AdvanceTime(base, set, index, step, 16, 12);
}

uint32_t TrackSetAdvance32(uint8_t* base, uint32_t set, uint32_t index, double step) {
  ITC_NATIVE_RUNNING("sub_825A10A8 TrackSet::Advance32");
  return AdvanceTime(base, set, index, step, 32, 16);
}

uint32_t ModelAnimate(uint8_t* base, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_82594B60 Model::Animate");
  const uint32_t float4s = model + 304;
  uint32_t count = TrackSetCount(base, float4s);
  const double one = Lfs(base, kOne);
  if (int32_t(count) > 0) {
    uint32_t i = 0;
    do {
      TrackSetSampleFloat4(base, float4s, i);
      TrackSetAdvance(base, float4s, i, one);
      i++;
      count = TrackSetCount(base, float4s);
    } while (int32_t(i) < int32_t(count));
  }
  const uint32_t materials = model + 364;
  count = TrackSetCount(base, materials);
  if (int32_t(count) > 0) {
    uint32_t i = 0;
    do {
      TrackSetSample32(base, materials, i);
      TrackSetAdvance32(base, materials, i, one);
      i++;
      count = TrackSetCount(base, materials);
    } while (int32_t(i) < int32_t(count));
  }
  return count;
}

bool TrackSetReleaseArray(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A11D0 TrackSet::ReleaseArray");
  return ReleaseValues(base, set, 16);
}

bool TrackSetRelease(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A1BA8 TrackSet::Release");
  return ReleaseValues(base, set, 0);
}

bool TrackSetSetupFloat4(uint8_t* base, uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A1E08 TrackSet::SetupFloat4");
  return SetupWithHeader(base, set, section, 16, 0x0FFFFFFF, 12);
}

bool TrackSetSetup32(uint8_t* base, uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A1218 TrackSet::Setup32");
  return SetupWithHeader(base, set, section, 32, 0x07FFFFFF, 16);
}

bool TrackSetSetupFloat(uint8_t* base, uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A1BE8 TrackSet::SetupFloat");
  const uint32_t old = Load(base, set + 16);
  const uint32_t count = SectionCount(base, section);
  const uint32_t size = count > 0x3FFFFFFF ? 0xFFFFFFFFu : count << 2;
  if ((old != 0 && !Deletable(old)) || (count != 0 && !Allocatable(base, size))) {
    return false;
  }
  if (old != 0) {
    memory::crt::OperatorDeleteArray(old);
  }
  Store(base, set + 16, 0);
  TrackSetView(base, set, section);
  uint32_t n = TrackSetCount(base, set);
  if (n == 0) {
    return true;
  }
  Store(base, set + 16, NewArray(base, size));
  if (!(int32_t(n) > 0)) {
    return true;
  }
  uint32_t offset = 0;
  do {
    const double start = TrackSetStartTime(base, set, 0xFFFFFFFFu);
    const uint32_t values = Load(base, set + 16);
    n--;
    Stfs(base, offset + values, start);
    offset += 4;
  } while (n != 0);
  return true;
}

bool TrackSetSetup208(uint8_t* base, uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A1A38 TrackSet::Setup208");
  const uint32_t old = Load(base, set + 16);
  if (SectionCount(base, section) != 0 || (old != 0 && !Deletable(old - 16))) {
    return false;
  }
  if (old != 0) {
    memory::crt::OperatorDeleteArray(old - 16);
  }
  Store(base, set + 16, 0);
  TrackSetView(base, set, section);
  // The count is 0 (checked above): nothing is allocated or set up.
  TrackSetCount(base, set);
  return true;
}

// --- The skeleton. ---

namespace {

constexpr uint32_t kSkeletonBaseVtable = 0x8206CE60;
constexpr uint32_t kSkeletonVtable = 0x8206CE50;
constexpr uint32_t kPrimitiveDrawerVtable = 0x820721D4;

// Whether SkeletonBase::Free frees all four natively.
bool BaseFreeable(uint8_t* base, uint32_t skeleton) {
  const uint32_t cursors = Load(base, skeleton + 128);
  if (!Deletable(cursors)) {
    return false;
  }
  for (uint32_t offset : {136u, 144u, 132u}) {
    const uint32_t array = Load(base, skeleton + offset);
    if (array != 0 && !Deletable(array - 16)) {
      return false;
    }
  }
  return true;
}

// Matrix::Multiply as its adapter does it: out = a * b, noted for
// interpolation with the return address of the call it replaces.
void Multiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t caller) {
  matrix_math::Multiply(base, out, a, b);
  interpolation::BlendState_OnMultiply(base, out, a, b, caller);
}

// Two doublewords from `from` to `to` (ld, std, ld, std).
void CopyRow(uint8_t* base, uint32_t from, uint32_t to) {
  for (uint32_t half = 0; half < 16; half += 8) {
    const uint32_t high = Load(base, from + half), low = Load(base, from + half + 4);
    Store(base, to + half, high);
    Store(base, to + half + 4, low);
  }
}

}  // namespace

uint32_t SkeletonBaseConstruct(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259A140 SkeletonBase::SkeletonBase");
  const uint32_t matrix = skeleton + 16;
  const double one = Lfs(base, kOne);
  Store(base, skeleton + 0, kSkeletonBaseVtable);
  for (uint32_t row = 0; row < 4; row++) {
    Stfs(base, matrix + 16 * row + 12, one);
  }
  xact::WaveEntryConstruct(base, skeleton + 80);
  // The identity, built on the original's stack and copied in.
  for (uint32_t word = 0; word < 16; word++) {
    Store(base, matrix + 4 * word, word % 5 == 0 ? 0x3F800000u : 0);
  }
  for (uint32_t offset = 128; offset <= 160; offset += 4) {
    Store(base, skeleton + offset, 0);
  }
  return skeleton;
}

uint32_t SkeletonConstruct(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_82599758 Skeleton::Skeleton");
  SkeletonBaseConstruct(base, skeleton);
  Store(base, skeleton + 176, 0);
  Store(base, skeleton + 0, kSkeletonVtable);
  return skeleton;
}

bool SkeletonBaseFree(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259A228 SkeletonBase::Free");
  if (!BaseFreeable(base, skeleton)) {
    return false;
  }
  const uint32_t cursors = Load(base, skeleton + 128);
  if (cursors != 0) {
    memory::crt::OperatorDeleteArray(cursors);
  }
  for (uint32_t offset : {136u, 144u, 132u}) {
    const uint32_t array = Load(base, skeleton + offset);
    if (array != 0) {
      memory::crt::OperatorDeleteArray(array - 16);
    }
  }
  for (uint32_t offset : {128u, 136u, 144u, 132u}) {
    Store(base, skeleton + offset, 0);
  }
  return true;
}

bool SkeletonBaseDestruct(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259A9D0 SkeletonBase::~SkeletonBase");
  if (!BaseFreeable(base, skeleton)) {
    return false;
  }
  Store(base, skeleton + 0, kSkeletonBaseVtable);
  return SkeletonBaseFree(base, skeleton);
}

bool SkeletonDestruct(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825999C8 Skeleton::~Skeleton");
  const uint32_t values = Load(base, skeleton + 176);
  if (!Deletable(values) || !BaseFreeable(base, skeleton)) {
    return false;
  }
  Store(base, skeleton + 0, kSkeletonVtable);
  if (values != 0) {
    memory::crt::OperatorDeleteArray(values);
  }
  Store(base, skeleton + 176, 0);
  return SkeletonBaseDestruct(base, skeleton);
}

bool SkeletonFreeNodeValues(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825997A0 Skeleton::FreeNodeValues");
  const uint32_t values = Load(base, skeleton + 176);
  if (!Deletable(values)) {
    return false;
  }
  if (values != 0) {
    memory::crt::OperatorDeleteArray(values);
  }
  Store(base, skeleton + 176, 0);
  return true;
}

void SkeletonCursorClear(uint8_t* base, uint32_t cursor) {
  ITC_NATIVE_RUNNING("sub_825A2068 SkeletonCursor::Clear");
  Stfs(base, cursor + 12, Lfs(base, kZero));
  Store(base, cursor + 0, 0);
  Store(base, cursor + 4, 0);
  Store(base, cursor + 8, 0);
  Store(base, cursor + 16, 0);
}

void SkeletonCursorView(uint8_t* base, uint32_t cursor, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A2090 SkeletonCursor::View");
  if (section == 0) {
    Stfs(base, cursor + 12, Lfs(base, kZero));
    Store(base, cursor + 0, 0);
    Store(base, cursor + 4, 0);
    Store(base, cursor + 8, 0);
    Store(base, cursor + 16, 0);
    return;
  }
  Store(base, cursor + 0, section);
  Store(base, cursor + 4, section + 16);
  Store(base, cursor + 8, section + 20);
  Stfs(base, cursor + 12, Lfs(base, section + 4));
  Store(base, cursor + 16, 0);
}

void SkeletonLoadNodeValues(uint8_t* base, uint32_t skeleton, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825997E0 Skeleton::LoadNodeValues");
  if (Load(base, skeleton + 176) == 0) {
    return;
  }
  SkeletonCursorView(base, Load(base, skeleton + 128), section);
  const uint32_t cursor = Load(base, skeleton + 128);
  uint32_t entry = Load(base, cursor + 8);
  if (!(int32_t(Load(base, Load(base, cursor + 4))) > 0)) {
    return;
  }
  int32_t i = 0;
  do {
    const uint32_t node = Load(base, entry + 0);
    const double a = Lfs(base, entry + 12);
    const uint32_t value = (node << 4) + Load(base, skeleton + 176);
    i++;
    Stfs(base, value + 0, a);
    const double x = Lfs(base, value + 0);
    const double b = Lfs(base, entry + 16);
    entry += 20;
    Stfs(base, value + 4, b);
    Stfs(base, value + 8, x);
  } while (i < int32_t(Load(base, Load(base, Load(base, skeleton + 128) + 4))));
}

uint32_t SkeletonNodeCount(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_82599880 Skeleton::NodeCount");
  if (Load(base, skeleton + 176) == 0) {
    return 0;
  }
  return Load(base, Load(base, skeleton + 80) + 4);
}

MatricesResult SkeletonUpdateNodeMatrices(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259C3A8 Skeleton::UpdateNodeMatrices");
  // The node types the loop below takes, read first (the node file isn't
  // written here): 8 and 16 go to sub_8259B748 and sub_8259BE40.
  for (int32_t i = 0; i < int32_t(Load(base, Load(base, skeleton + 80) + 4)); i++) {
    const uint32_t type = Load(base, Load(base, skeleton + 84) + 160 * uint32_t(i) + 132);
    if (int32_t(type) > 4 && (type == 8 || type == 16)) {
      return MatricesResult::kOriginal;
    }
  }
  const uint32_t root = (Load(base, skeleton + 156) << 6) + Load(base, skeleton + 148);
  Multiply(base, root, root, skeleton + 16, 0x8259C3D4);
  uint32_t matrix = Load(base, skeleton + 148);
  const uint32_t records = Load(base, skeleton + 84);
  if (int32_t(Load(base, Load(base, skeleton + 80) + 4)) > 0) {
    uint32_t type_at = records + 132;
    uint32_t row = matrix + 48;
    uint32_t record = 0;
    int32_t i = 0;
    do {
      if (!(int32_t(Load(base, type_at)) > 4)) {
        const uint32_t parent = Load(base, type_at - 4);
        if (int32_t(parent) != -1) {
          Multiply(base, matrix, matrix, (parent << 6) + Load(base, skeleton + 148), 0x8259C42C);
        }
        const uint32_t source = Load(base, record + Load(base, skeleton + 84) + 92);
        if (int32_t(source) != -1) {
          CopyRow(base, (source << 6) + Load(base, skeleton + 148) + 48, row);
        }
      }
      i++;
      record += 160;
      type_at += 160;
      matrix += 64;
      row += 64;
    } while (i < int32_t(Load(base, Load(base, skeleton + 80) + 4)));
  }
  const uint32_t file = Load(base, skeleton + 80);
  const uint32_t matrices = Load(base, skeleton + 148);
  const uint32_t first = Load(base, file + 12);
  matrix = (first << 6) + matrices;
  uint32_t bone = first * 160 + Load(base, skeleton + 84) + 128;
  if (int32_t(Load(base, file + 8)) > 0) {
    int32_t i = 0;
    do {
      if ((Load(base, bone + 4) & 0x80) != 0) {
        const uint32_t parent = Load(base, bone + 0);
        if (int32_t(parent) != -1) {
          Multiply(base, matrix, matrix, (parent << 6) + Load(base, skeleton + 148), 0x8259C528);
        }
      }
      i++;
      bone += 160;
      matrix += 64;
    } while (i < int32_t(Load(base, Load(base, skeleton + 80) + 8)));
  }
  return MatricesResult::kMultiplied;
}

MatricesResult SkeletonUpdateMatrices(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825998A0 Skeleton::UpdateMatrices");
  if (Load(base, skeleton + 176) == 0) {
    return MatricesResult::kNothing;
  }
  return SkeletonUpdateNodeMatrices(base, skeleton);
}

double CursorCurveSample(uint8_t* base, uint32_t keys, uint32_t cursor, uint32_t count,
                         double time) {
  ITC_NATIVE_RUNNING("sub_825A20E8 CursorCurve::Sample");
  if (!(int32_t(count) > 1)) {
    return Lfs(base, keys + 8);
  }
  const uint32_t end = count * 20 + keys;
  if (!(time < Lfs(base, end - 20))) {
    return Lfs(base, end - 12);
  }
  if (!(time > Lfs(base, keys))) {
    return Lfs(base, keys + 8);
  }
  const uint32_t at = Load(base, cursor);
  if (!(int32_t(at) > 0) || !(int32_t(at) < int32_t(count))) {
    Store(base, cursor, 1);
  }
  uint32_t next = Load(base, cursor) * 20 + keys;
  uint32_t key = next - 20;
  for (;;) {
    if (!(Lfs(base, key) > time) && time < Lfs(base, next)) {
      return KeyInterpolate(base, key, next, time);
    }
    const uint32_t moved = Load(base, cursor) + 1;
    key += 20;
    next += 20;
    Store(base, cursor, moved);
    if (int32_t(moved) < int32_t(count)) {
      continue;
    }
    key = keys;
    Store(base, cursor, 1);
    next = keys + 20;
  }
}

void SkeletonApplyAnimation(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825998B8 Skeleton::ApplyAnimation");
  uint32_t cursor = Load(base, skeleton + 128);
  if (Load(base, cursor + 0) == 0) {
    return;
  }
  uint32_t entry = Load(base, cursor + 8);
  if (!(int32_t(Load(base, Load(base, cursor + 4))) > 0)) {
    return;
  }
  uint32_t offset = 0;
  int32_t i = 0;
  do {
    const uint32_t node = Load(base, entry + 0);
    const uint32_t entry_now = Load(base, cursor + 8) + offset;
    const uint32_t time_at = (node << 4) + Load(base, skeleton + 176);
    const uint32_t records = Load(base, skeleton + 136);
    const uint32_t section = Load(base, cursor + 0);
    const uint32_t channels = Load(base, entry + 4);
    const uint32_t record = node * 144 + records;
    uint32_t channel = Load(base, entry_now + 8) + section;
    const double time = Lfs(base, time_at + 8);
    if (int32_t(channels) > 0) {
      int32_t j = 0;
      do {
        const uint32_t component = Load(base, channel + 4);
        const uint32_t keys = channel + 8;
        const uint32_t key_count = Load(base, channel + 0);
        const double value = CursorCurveSample(base, keys, ((component + 16) << 2) + record,
                                               key_count, time);
        Stfs(base, (component << 2) + record, value);
        channel = Load(base, channel + 0) * 20 + keys;
        j++;
      } while (j < int32_t(Load(base, entry + 4)));
    }
    cursor = Load(base, skeleton + 128);
    i++;
    offset += 20;
    entry += 20;
  } while (i < int32_t(Load(base, Load(base, cursor + 4))));
}

bool PrimitiveDrawerConstruct(uint8_t* base, uint32_t drawer) {
  ITC_NATIVE_RUNNING("sub_825A1378 PrimitiveDrawer::PrimitiveDrawer");
  Store(base, drawer + 0, kPrimitiveDrawerVtable);
  const auto block = memory::crt::OperatorNew(base, 4);
  if (!block) {
    return false;
  }
  Store(base, drawer + 4, *block);
  return true;
}

void RecordClear(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_825A18D8 RecordClear");
  Stfs(base, record + 0, Lfs(base, kZero));
  Store(base, record + 4, 0);
  Store(base, record + 8, 0);
}

}  // namespace model_animation
