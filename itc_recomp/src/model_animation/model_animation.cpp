#include "model_animation/model_animation.h"

#include "guest.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "schema/model_animation.h"
#include "schema/models.h"
#include "xact/xact.h"

#include <cstdlib>
#include <initializer_list>

namespace model_animation {

namespace {

using namespace car_physics::ops;

using G = schema::ModelAnimationGlobals;
using Set = schema::TrackSet;
using Section = schema::AnimationSection;
using Track = schema::AnimationTrack;
using Channel = schema::AnimationChannel;
using Key = schema::AnimationKey;
using Skel = schema::Skeleton;
using Cursor = schema::SkeletonCursor;
using NodeEntry = schema::SkeletonNodeEntry;
// models.md's types: the node file, its records, the node states and matrices.
using NodeFile = schema::NodeFile;
using NodeRecord = schema::SkeletonNodeRecord;
using NodeState = schema::SkeletonNodeState;
using NodeMatrix = schema::SkeletonMatrix;
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
  return section == 0 ? 0 : Section(base, section).track_count();
}

// SetupFloat4 and Setup32: `stride`-byte values with a count header, at most
// `max_count` (else operator new[](-1)), the time at +`time`.
bool SetupWithHeader(uint8_t* base, uint32_t set, uint32_t section, uint32_t stride,
                     uint32_t max_count, uint32_t time) {
  const Set s(base, set);
  const uint32_t old = s.values().address;
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
  s.set_values(0u);
  TrackSetView(base, set, section);
  uint32_t n = TrackSetCount(base, set);
  if (n == 0) {
    return true;
  }
  // A blob in PC memory (E5 step 5): the header and values in the guest's
  // byte order, read by address as before; operator delete[] frees it.
  const uint32_t block = schema::TrackValueBlock::New(base, size).address;
  uint32_t values = 0;
  if (block != 0) {
    const schema::TrackValueBlock values_block(base, block);
    values_block.set_count(n);
    values = values_block.values_address();
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
  s.set_values(values);
  if (!(int32_t(n) > 0)) {
    return true;
  }
  uint32_t offset = 0;
  do {
    const double start = TrackSetStartTime(base, set, 0xFFFFFFFFu);
    const uint32_t at = offset + s.values().address;
    n--;
    offset += stride;
    Stfs(base, at + time, start);
  } while (n != 0);
  return true;
}

// Frees +16 (less `header`) and clears it.
bool ReleaseValues(uint8_t* base, uint32_t set, uint32_t header) {
  const Set s(base, set);
  const uint32_t values = s.values().address;
  if (values != 0 && !Deletable(values - header)) {
    return false;
  }
  if (values != 0) {
    memory::crt::OperatorDeleteArray(values - header);
  }
  s.set_values(0u);
  return true;
}

}  // namespace

void TrackSetClear(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A6670 TrackSet::Clear");
  const Set s(base, set);
  s.set_start_time(float(Lfs(base, kZero)));
  s.set_section(0u);
  s.set_track_count_pointer(0);
  s.set_tracks(0u);
}

uint32_t TrackSetConstruct(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A1B70 TrackSet::TrackSet");
  TrackSetClear(base, set);
  Set(base, set).set_values(0u);
  return set;
}

void TrackSetView(uint8_t* base, uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A6690 TrackSet::View");
  const Set s(base, set);
  if (section == 0) {
    s.set_start_time(float(Lfs(base, kZero)));
    s.set_section(0u);
    s.set_track_count_pointer(0);
    s.set_tracks(0u);
    return;
  }
  const Section sec(base, section);
  s.set_section(section);
  s.set_track_count_pointer(sec.track_count_address());
  s.set_tracks(sec.tracks_address());
  s.set_start_time(sec.start_time());
}

uint32_t TrackSetCount(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A66D8 TrackSet::Count");
  const Set s(base, set);
  if (!s.section()) {
    return 0;
  }
  return Load(base, s.track_count_pointer());
}

namespace {

// StartTime and EndTime: the track's start or end time, the section's for
// -1.
double TrackTime(uint8_t* base, uint32_t set, uint32_t track, bool end) {
  const Set s(base, set);
  const Section section = s.section();
  if (!section) {
    return Lfs(base, kZero);
  }
  if (int32_t(track) == -1) {
    return double(end ? section.end_time() : section.start_time());
  }
  const Track t(base, track * 80 + s.tracks().address);
  return double(end ? t.end_time() : t.start_time());
}

}  // namespace

double TrackSetStartTime(uint8_t* base, uint32_t set, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825A1F08 TrackSet::StartTime");
  return TrackTime(base, set, track, false);
}

double TrackSetEndTime(uint8_t* base, uint32_t set, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825A1F50 TrackSet::EndTime");
  return TrackTime(base, set, track, true);
}

uint32_t TrackSetTrack(uint8_t* base, uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A2000 TrackSet::Track");
  const Set s(base, set);
  if (!s.section()) {
    return 0;
  }
  return index * 80 + s.tracks().address;
}

uint32_t TrackSetChannels(uint8_t* base, uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A2030 TrackSet::Channels");
  const Set s(base, set);
  const uint32_t section = s.section().address;
  if (section == 0) {
    return 0;
  }
  return Track(base, index * 80 + s.tracks().address).channels_offset() + section;
}

double KeyInterpolate(uint8_t* base, uint32_t key0, uint32_t key1, double time) {
  ITC_NATIVE_RUNNING("sub_825A66F8 Key::Interpolate");
  const Key k0(base, key0), k1(base, key1);
  const uint32_t flags = k0.flags();
  if ((flags & 2) != 0) {
    // Linear.
    const double v0 = k0.value();
    const double t0 = k0.time();
    const double v1 = k1.value();
    const double t1 = k1.time();
    const double dv = Fsubs(v1, v0);
    const double dt = Fsubs(t1, t0);
    const double x = Fsubs(time, t0);
    return Fmadds(Fdivs(dv, dt), x, v0);
  }
  if ((flags & 8) != 0) {
    // Stepped.
    return k0.value();
  }
  // Cubic Hermite: the first key's out tangent, the second's in.
  const double t0 = k0.time();
  const double t1 = k1.time();
  double f11 = Fsubs(time, t0);
  const double span = Fsubs(t1, t0);
  const double v0 = k0.value();
  const double v1 = k1.value();
  const double f6 = Fsubs(v0, v1);
  const double in1 = k1.in_tangent();
  const double two = G::float_two(base);
  const double out0 = k0.out_tangent();
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
    return Key(base, keys).value();
  }
  const uint32_t end = count * 20 + keys;
  const Key last(base, end - 20);
  if (!(time < double(last.time()))) {
    return last.value();
  }
  uint32_t key = keys + 20;
  if (!(time < double(Key(base, key).time()))) {
    do {
      key += 20;
    } while (!(time < double(Key(base, key).time())));
  }
  if (time == double(Key(base, key).time())) {
    return Key(base, key).value();
  }
  return KeyInterpolate(base, key - 20, key, time);
}

void TrackSetSampleFloat4(uint8_t* base, uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A1C88 TrackSet::SampleFloat4");
  const Set s(base, set);
  if (!s.section()) {
    return;
  }
  const Track track(base, TrackSetTrack(base, set, index));
  uint32_t channel = TrackSetChannels(base, set, index);
  const uint32_t offset = index << 4;
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  // The values are an array (16 bytes a track: x, y, z, the time).
  Stfs(base, offset + s.values().address, zero);
  Stfs(base, offset + s.values().address + 4, zero);
  Stfs(base, offset + s.values().address + 8, one);
  if (!(int32_t(track.channel_count()) > 0)) {
    return;
  }
  const uint32_t lanes = index << 2;
  int32_t i = 0;
  do {
    const Channel c(base, channel);
    const uint32_t keys = c.keys_address();
    const uint32_t values = s.values().address;
    const uint32_t lane = (c.component() + lanes) << 2;
    const uint32_t key_count = c.key_count();
    const double value = CurveSample(base, keys, key_count, Lfs(base, values + offset + 12));
    Stfs(base, lane + values, value);
    channel = c.key_count() * 20 + keys;
    i++;
  } while (i < int32_t(track.channel_count()));
}

void TrackSetSample32(uint8_t* base, uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A0FE0 TrackSet::Sample32");
  const Set s(base, set);
  if (!s.section()) {
    return;
  }
  const Track track(base, TrackSetTrack(base, set, index));
  uint32_t channel = TrackSetChannels(base, set, index);
  const uint32_t offset = index << 5;
  // The values are an array (32 bytes a track: +16 the time, +20 the mask).
  const uint32_t value = offset + s.values().address;
  Store(base, value + 20, 0);
  if (!(int32_t(track.channel_count()) > 0)) {
    return;
  }
  int32_t i = 0;
  do {
    const Channel c(base, channel);
    const uint32_t component = c.component();
    const uint32_t keys = c.keys_address();
    const uint32_t mask = Load(base, value + 20);
    // slw: shifts of 32 to 63 give 0.
    const uint32_t bit = (component & 0x20) != 0 ? 0 : 1u << (component & 0x1F);
    Store(base, value + 20, bit | mask);
    const uint32_t key_count = c.key_count();
    const double sample =
        CurveSample(base, keys, key_count, Lfs(base, offset + s.values().address + 16));
    Stfs(base, (component << 2) + value, sample);
    channel = c.key_count() * 20 + keys;
    i++;
  } while (i < int32_t(track.channel_count()));
}

namespace {

// Advance and Advance32: the time at value + `time`, `stride`-byte values.
uint32_t AdvanceTime(uint8_t* base, uint32_t set, uint32_t index, double step, uint32_t stride,
                     uint32_t time) {
  const Set s(base, set);
  if (!s.section()) {
    return 0;
  }
  const uint32_t at = index * stride + s.values().address + time;
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
  const schema::Model m(base, model);
  const uint32_t float4s = m.track_set_float4_address();
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
  const uint32_t materials = m.track_set_material_address();
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
  const Set s(base, set);
  const uint32_t old = s.values().address;
  const uint32_t count = SectionCount(base, section);
  const uint32_t size = count > 0x3FFFFFFF ? 0xFFFFFFFFu : count << 2;
  if ((old != 0 && !Deletable(old)) || (count != 0 && !Allocatable(base, size))) {
    return false;
  }
  if (old != 0) {
    memory::crt::OperatorDeleteArray(old);
  }
  s.set_values(0u);
  TrackSetView(base, set, section);
  uint32_t n = TrackSetCount(base, set);
  if (n == 0) {
    return true;
  }
  s.set_values(NewArray(base, size));
  if (!(int32_t(n) > 0)) {
    return true;
  }
  uint32_t offset = 0;
  do {
    const double start = TrackSetStartTime(base, set, 0xFFFFFFFFu);
    const uint32_t values = s.values().address;
    n--;
    Stfs(base, offset + values, start);
    offset += 4;
  } while (n != 0);
  return true;
}

bool TrackSetSetup208(uint8_t* base, uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A1A38 TrackSet::Setup208");
  const Set s(base, set);
  const uint32_t old = s.values().address;
  if (SectionCount(base, section) != 0 || (old != 0 && !Deletable(old - 16))) {
    return false;
  }
  if (old != 0) {
    memory::crt::OperatorDeleteArray(old - 16);
  }
  s.set_values(0u);
  TrackSetView(base, set, section);
  // The count is 0 (checked above): nothing is allocated or set up.
  TrackSetCount(base, set);
  return true;
}

// --- The skeleton. ---

namespace {

constexpr uint32_t kSkeletonBaseVtable = 0x8206CE60;

// Whether SkeletonBase::Free frees all four natively.
bool BaseFreeable(uint8_t* base, uint32_t skeleton) {
  const Skel sk(base, skeleton);
  const uint32_t cursors = sk.cursors().address;
  if (!Deletable(cursors)) {
    return false;
  }
  // The three arrays with a count header, in the original's order.
  const auto freeable = [](uint32_t array) { return array == 0 || Deletable(array - 16); };
  return freeable(sk.node_states()) && freeable(sk.bone_matrices()) &&
         freeable(sk.own_matrices());
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
  const Skel sk(base, skeleton);
  const uint32_t matrix = sk.matrix_address();
  const double one = Lfs(base, kOne);
  sk.set_vtable(kSkeletonBaseVtable);
  for (uint32_t row = 0; row < 4; row++) {
    Stfs(base, matrix + 16 * row + 12, one);
  }
  xact::WaveEntryConstruct(base, sk.node_file_address());
  // The identity, built on the original's stack and copied in.
  for (uint32_t word = 0; word < 16; word++) {
    Store(base, matrix + 4 * word, word % 5 == 0 ? 0x3F800000u : 0);
  }
  sk.set_cursors(0u);
  sk.set_own_matrices(0);
  sk.set_node_states(0);
  sk.set_bone_states(0);
  sk.set_bone_matrices(0);
  sk.set_matrices(0);
  sk.set_unknown_152(0);
  sk.set_root_index(0);
  sk.set_instance_count(0);
  return skeleton;
}

uint32_t SkeletonConstruct(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_82599758 Skeleton::Skeleton");
  SkeletonBaseConstruct(base, skeleton);
  const Skel sk(base, skeleton);
  sk.set_node_values(0);
  sk.set_vtable(Skel::kVtable);
  return skeleton;
}

bool SkeletonBaseFree(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259A228 SkeletonBase::Free");
  if (!BaseFreeable(base, skeleton)) {
    return false;
  }
  const Skel sk(base, skeleton);
  const uint32_t cursors = sk.cursors().address;
  if (cursors != 0) {
    memory::crt::OperatorDeleteArray(cursors);
  }
  // The three arrays with a count header, in the original's order.
  const auto free_counted = [](uint32_t array) {
    if (array != 0) {
      memory::crt::OperatorDeleteArray(array - 16);
    }
  };
  free_counted(sk.node_states());
  free_counted(sk.bone_matrices());
  free_counted(sk.own_matrices());
  sk.set_cursors(0u);
  sk.set_node_states(0);
  sk.set_bone_matrices(0);
  sk.set_own_matrices(0);
  return true;
}

bool SkeletonBaseDestruct(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259A9D0 SkeletonBase::~SkeletonBase");
  if (!BaseFreeable(base, skeleton)) {
    return false;
  }
  Skel(base, skeleton).set_vtable(kSkeletonBaseVtable);
  return SkeletonBaseFree(base, skeleton);
}

bool SkeletonDestruct(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825999C8 Skeleton::~Skeleton");
  const Skel sk(base, skeleton);
  const uint32_t values = sk.node_values();
  if (!Deletable(values) || !BaseFreeable(base, skeleton)) {
    return false;
  }
  sk.set_vtable(Skel::kVtable);
  if (values != 0) {
    memory::crt::OperatorDeleteArray(values);
  }
  sk.set_node_values(0);
  return SkeletonBaseDestruct(base, skeleton);
}

bool SkeletonFreeNodeValues(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825997A0 Skeleton::FreeNodeValues");
  const Skel sk(base, skeleton);
  const uint32_t values = sk.node_values();
  if (!Deletable(values)) {
    return false;
  }
  if (values != 0) {
    memory::crt::OperatorDeleteArray(values);
  }
  sk.set_node_values(0);
  return true;
}

bool TrackSetReleasable(uint8_t* base, uint32_t set, bool array) {
  const uint32_t values = Set(base, set).values().address;
  return values == 0 || Deletable(values - (array ? 16 : 0));
}

bool SkeletonDestructible(uint8_t* base, uint32_t skeleton) {
  return Deletable(Skel(base, skeleton).node_values()) && BaseFreeable(base, skeleton);
}

bool SkeletonBaseDestructible(uint8_t* base, uint32_t skeleton) {
  return BaseFreeable(base, skeleton);
}

void NodeFileView(uint8_t* base, uint32_t view, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_825A21A8 NodeFile::View");
  const schema::NodeFileView v(base, view);
  const NodeFile f(base, file);
  v.set_file(file);
  // The file's seven offsets from +16 made pointers, one at a time.
  v.set_node_records(f.node_records_offset() + file);
  v.set_rest_translations(f.rest_translations_offset() + file);
  v.set_rest_angles(f.rest_angles_offset() + file);
  v.set_rest_scales(f.rest_scales_offset() + file);
  v.set_unknown_20(f.unknown_32() + file);
  v.set_matrices(f.matrices_offset() + file);
  v.set_unknown_28(f.unknown_40() + file);
  const uint32_t tail = f.tail_offset() + file;
  v.set_tail(tail);
  v.set_tail_16(tail + 16);
  v.set_unknown_40(f.unknown_48() + file);
  const uint32_t node_count = f.node_count();
  const uint32_t bone_count = f.bone_count();
  v.set_non_bone_count(node_count - bone_count);
}

namespace {

// operator new[]'s size for `count` elements of `bytes` with a 16-byte
// header, as the compiler works it out: -1 past `limit` or when it would
// pass -17.
uint32_t ArrayBytes(uint32_t count, uint32_t bytes, uint32_t limit) {
  if (count > limit) {
    return 0xFFFFFFFFu;
  }
  const uint32_t body = count * bytes;
  return body > 0xFFFFFFEFu ? 0xFFFFFFFFu : body + 16;
}

constexpr uint32_t kNodeLimit = 0x01C71C71;    // 144-byte entries
constexpr uint32_t kMatrixLimit = 0x03FFFFFF;  // 64-byte entries
constexpr uint32_t kCursorLimit = 0x0CCCCCCC;  // 20-byte cursors

// A counted array (its count at +0, the elements from +16): the element
// count's 64-byte (or 144-byte) entries get 1.0 at `ones`.
uint32_t CountedArray(uint8_t* base, uint32_t count, uint32_t bytes, uint32_t limit,
                      std::initializer_list<uint32_t> ones, double one) {
  const uint32_t array = NewArray(base, ArrayBytes(count, bytes, limit));
  Store(base, array, count);
  for (int32_t j = 0; j <= int32_t(count - 1); j++) {
    const uint32_t entry = array + 16 + bytes * uint32_t(j);
    for (const uint32_t offset : ones) {
      Stfs(base, entry + offset, one);
    }
    if (bytes == 144) {
      Store(base, entry + 132, 0);
      for (uint32_t w = 0; w < 16; w++) {
        Store(base, entry + 64 + 4 * w, 0);
      }
    }
  }
  return array + 16;
}

}  // namespace

bool SkeletonSetUp(uint8_t* base, uint32_t skeleton, uint32_t file, uint32_t matrices,
                   int32_t instances) {
  ITC_NATIVE_RUNNING("sub_8259A9E0 SkeletonBase::SetUp");
  // The checks first: the sizes operator new[] takes (the counts are the
  // file's, which the view only points at), and Free's.
  const uint32_t count = instances > 0 ? uint32_t(instances) : 1;
  const uint32_t nodes = NodeFile(base, file).node_count();
  const uint32_t entries = NodeFile(base, file).bone_count();
  if (count > kCursorLimit || !Allocatable(base, count * 20) ||
      (nodes != 0 && !Allocatable(base, ArrayBytes(nodes, 144, kNodeLimit))) ||
      (nodes != 0 && matrices == 0 && !Allocatable(base, ArrayBytes(nodes, 64, kMatrixLimit))) ||
      (entries != 0 && !Allocatable(base, ArrayBytes(entries, 64, kMatrixLimit))) ||
      !BaseFreeable(base, skeleton)) {
    return false;
  }
  SkeletonBaseFree(base, skeleton);
  const Skel sk(base, skeleton);
  sk.set_instance_count(instances);
  if (instances <= 0) {
    sk.set_instance_count(1);
  }
  const uint32_t cursors = NewArray(base, count * 20);
  for (uint32_t k = 0; k < count; k++) {
    SkeletonCursorClear(base, cursors + 20 * k);
  }
  sk.set_cursors(cursors);
  NodeFileView(base, sk.node_file_address(), file);
  const double one = Lfs(base, kOne);
  // The node file's +4 the node count, +8 the bones, +12 the first bone.
  const uint32_t node_count = NodeFile(base, sk.node_file()).node_count();
  if (node_count != 0) {
    sk.set_node_states(CountedArray(base, node_count, 144, kNodeLimit, {12, 28, 44, 60}, one));
    if (matrices == 0) {
      const uint32_t again = NodeFile(base, sk.node_file()).node_count();
      const uint32_t array = CountedArray(base, again, 64, kMatrixLimit, {12, 28, 44, 60}, one);
      sk.set_own_matrices(array);
      sk.set_matrices(array);
    } else {
      sk.set_matrices(matrices);
    }
  }
  const uint32_t entry_count = NodeFile(base, sk.node_file()).bone_count();
  if (entry_count != 0) {
    sk.set_bone_matrices(CountedArray(base, entry_count, 64, kMatrixLimit, {12, 28, 44}, one));
    for (int32_t i = 0; i < int32_t(NodeFile(base, sk.node_file()).bone_count()); i++) {
      Store(base, sk.bone_matrices() + 64 * uint32_t(i) + 52, 1);
    }
  }
  sk.set_bone_states(sk.node_states() + 144 * NodeFile(base, sk.node_file()).first_bone());
  return true;
}

bool SkeletonBind(uint8_t* base, uint32_t skeleton, uint32_t file, uint32_t matrices) {
  ITC_NATIVE_RUNNING("sub_82599A20 Skeleton::Bind");
  const uint32_t nodes = NodeFile(base, file).node_count();
  const uint32_t values_bytes = nodes > 0x0FFFFFFFu ? 0xFFFFFFFFu : nodes * 16;
  const Skel sk(base, skeleton);
  if (!Deletable(sk.node_values()) ||
      (nodes != 0 && !Allocatable(base, values_bytes)) ||
      !SkeletonSetUp(base, skeleton, file, matrices, 1)) {
    return false;
  }
  const uint32_t values = sk.node_values();
  if (values != 0) {
    memory::crt::OperatorDeleteArray(values);
  }
  sk.set_node_values(0);
  const uint32_t count = NodeFile(base, sk.node_file()).node_count();
  if (count != 0) {
    sk.set_node_values(NewArray(base, values_bytes));
  }
  const double zero = Lfs(base, kZero);
  int32_t n = int32_t(SkeletonNodeCount(base, skeleton));
  for (int32_t i = 0; i < n; i++) {
    const uint32_t value = sk.node_values() + 16 * uint32_t(i);
    Stfs(base, value + 0, zero);
    Stfs(base, value + 4, zero);
    Stfs(base, value + 8, zero);
    n = int32_t(SkeletonNodeCount(base, skeleton));
  }
  return true;
}

void SkeletonCursorClear(uint8_t* base, uint32_t cursor) {
  ITC_NATIVE_RUNNING("sub_825A2068 SkeletonCursor::Clear");
  const Cursor c(base, cursor);
  c.set_start_time(float(Lfs(base, kZero)));
  c.set_section(0u);
  c.set_entry_count_pointer(0);
  c.set_entries(0u);
  c.set_unknown_16(0);
}

void SkeletonCursorView(uint8_t* base, uint32_t cursor, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A2090 SkeletonCursor::View");
  const Cursor c(base, cursor);
  if (section == 0) {
    c.set_start_time(float(Lfs(base, kZero)));
    c.set_section(0u);
    c.set_entry_count_pointer(0);
    c.set_entries(0u);
    c.set_unknown_16(0);
    return;
  }
  const Section sec(base, section);
  c.set_section(section);
  c.set_entry_count_pointer(sec.track_count_address());
  c.set_entries(sec.tracks_address());
  c.set_start_time(sec.start_time());
  c.set_unknown_16(0);
}

void SkeletonLoadNodeValues(uint8_t* base, uint32_t skeleton, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825997E0 Skeleton::LoadNodeValues");
  const Skel sk(base, skeleton);
  if (sk.node_values() == 0) {
    return;
  }
  SkeletonCursorView(base, sk.cursors().address, section);
  const Cursor cursor = sk.cursors();
  uint32_t entry = cursor.entries().address;
  if (!(int32_t(Load(base, cursor.entry_count_pointer())) > 0)) {
    return;
  }
  int32_t i = 0;
  do {
    const NodeEntry e(base, entry);
    const uint32_t node = e.node();
    const double a = e.time();
    // The node values are an array (a float4 a node).
    const uint32_t value = (node << 4) + sk.node_values();
    i++;
    Stfs(base, value + 0, a);
    const double x = Lfs(base, value + 0);
    const double b = e.unknown_16();
    entry += 20;
    Stfs(base, value + 4, b);
    Stfs(base, value + 8, x);
  } while (i < int32_t(Load(base, sk.cursors().entry_count_pointer())));
}

uint32_t SkeletonNodeCount(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_82599880 Skeleton::NodeCount");
  const Skel sk(base, skeleton);
  if (sk.node_values() == 0) {
    return 0;
  }
  return NodeFile(base, sk.node_file()).node_count();
}

MatricesResult SkeletonUpdateNodeMatrices(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259C3A8 Skeleton::UpdateNodeMatrices");
  // The node types the loop below takes, read first (the node file isn't
  // written here): 8 and 16 go to sub_8259B748 and sub_8259BE40.
  const Skel sk(base, skeleton);
  for (int32_t i = 0; i < int32_t(NodeFile(base, sk.node_file()).node_count()); i++) {
    const uint32_t type = NodeRecord(base, sk.node_records() + NodeRecord::kSize * uint32_t(i)).type();
    if (int32_t(type) > 4 && (type == 8 || type == 16)) {
      return MatricesResult::kOriginal;
    }
  }
  const uint32_t root = (sk.root_index() << 6) + sk.matrices();
  Multiply(base, root, root, sk.matrix_address(), 0x8259C3D4);
  uint32_t matrix = sk.matrices();
  const uint32_t records = sk.node_records();
  if (int32_t(NodeFile(base, sk.node_file()).node_count()) > 0) {
    uint32_t row = NodeMatrix(base, matrix).row3_xy_address();
    uint32_t record = 0;
    int32_t i = 0;
    do {
      if (!(int32_t(NodeRecord(base, records + record).type()) > 4)) {
        const uint32_t parent = uint32_t(NodeRecord(base, records + record).parent());
        if (int32_t(parent) != -1) {
          Multiply(base, matrix, matrix, (parent << 6) + sk.matrices(), 0x8259C42C);
        }
        const uint32_t source = NodeRecord(base, record + sk.node_records()).translation_node();
        if (int32_t(source) != -1) {
          CopyRow(base, NodeMatrix(base, (source << 6) + sk.matrices()).row3_xy_address(), row);
        }
      }
      i++;
      record += NodeRecord::kSize;
      matrix += 64;
      row += 64;
    } while (i < int32_t(NodeFile(base, sk.node_file()).node_count()));
  }
  const uint32_t file = sk.node_file();
  const uint32_t matrices = sk.matrices();
  const uint32_t first = NodeFile(base, file).first_bone();
  matrix = (first << 6) + matrices;
  uint32_t bone = first * NodeRecord::kSize + sk.node_records();
  if (int32_t(NodeFile(base, file).bone_count()) > 0) {
    int32_t i = 0;
    do {
      if ((NodeRecord(base, bone).type() & 0x80) != 0) {
        const uint32_t parent = uint32_t(NodeRecord(base, bone).parent());
        if (int32_t(parent) != -1) {
          Multiply(base, matrix, matrix, (parent << 6) + sk.matrices(), 0x8259C528);
        }
      }
      i++;
      bone += NodeRecord::kSize;
      matrix += 64;
    } while (i < int32_t(NodeFile(base, sk.node_file()).bone_count()));
  }
  return MatricesResult::kMultiplied;
}

MatricesResult SkeletonUpdateMatrices(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825998A0 Skeleton::UpdateMatrices");
  if (Skel(base, skeleton).node_values() == 0) {
    return MatricesResult::kNothing;
  }
  return SkeletonUpdateNodeMatrices(base, skeleton);
}

double CursorCurveSample(uint8_t* base, uint32_t keys, uint32_t cursor, uint32_t count,
                         double time) {
  ITC_NATIVE_RUNNING("sub_825A20E8 CursorCurve::Sample");
  const Key first(base, keys);
  if (!(int32_t(count) > 1)) {
    return first.value();
  }
  const uint32_t end = count * 20 + keys;
  const Key last(base, end - 20);
  if (!(time < double(last.time()))) {
    return last.value();
  }
  if (!(time > double(first.time()))) {
    return first.value();
  }
  const uint32_t at = Load(base, cursor);
  if (!(int32_t(at) > 0) || !(int32_t(at) < int32_t(count))) {
    Store(base, cursor, 1);
  }
  uint32_t next = Load(base, cursor) * 20 + keys;
  uint32_t key = next - 20;
  for (;;) {
    if (!(double(Key(base, key).time()) > time) && time < double(Key(base, next).time())) {
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

bool SkeletonResetPose(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_82599AF0 Skeleton::ResetPose");
  const Skel sk(base, skeleton);
  if (sk.node_values() == 0) {
    return false;
  }
  uint32_t a = sk.rest_angles();
  uint32_t record = sk.node_records();
  uint32_t out = sk.node_states();
  uint32_t b = sk.rest_translations();
  uint32_t c = sk.rest_scales();
  // 16 bytes as the original's two doubleword moves.
  auto copy16 = [&](uint32_t to, uint32_t from) {
    uint32_t words[4];
    for (uint32_t k = 0; k < 4; k++) words[k] = Load(base, from + 4 * k);
    for (uint32_t k = 0; k < 4; k++) Store(base, to + 4 * k, words[k]);
  };
  for (int32_t i = 0; i < int32_t(SkeletonNodeCount(base, skeleton)); i++) {
    const NodeState state(base, out);
    const bool extra = (NodeRecord(base, record).type() & 0x40) != 0;
    // The original loads +88's, +92's and the first half of +96's before
    // its first stores, then the rest.
    uint32_t bw[4], aw[4], cw[4];
    for (uint32_t k = 0; k < 4; k++) bw[k] = Load(base, b + 4 * k);
    for (uint32_t k = 0; k < 4; k++) aw[k] = Load(base, a + 4 * k);
    cw[0] = Load(base, c + 0);
    cw[1] = Load(base, c + 4);
    for (uint32_t k = 0; k < 4; k++) Store(base, state.translation_x_address() + 4 * k, bw[k]);
    cw[2] = Load(base, c + 8);
    cw[3] = Load(base, c + 12);
    for (uint32_t k = 0; k < 4; k++) Store(base, state.angle_x_address() + 4 * k, aw[k]);
    for (uint32_t k = 0; k < 4; k++) Store(base, state.scale_x_address() + 4 * k, cw[k]);
    if (extra) {
      // The record's +64 (16 bytes, not in models.md's type) to the state's +48.
      copy16(out + 48, record + 64);
    }
    out += NodeState::kSize;
    record += NodeRecord::kSize;
    a += 16;
    b += 16;
    c += 16;
  }
  SkeletonApplyAnimation(base, skeleton);
  return true;
}

namespace {

// sub_82599C58's identity (vupkd3d128's (3.0, 3.0, 0.0, 1.0) shuffled into
// four rows by vpermwi128: the 1.0 at row i's word i), stored aligned at
// `at` & ~15 as stvx does.
void StoreIdentity(uint8_t* base, uint32_t at) {
  const uint32_t aligned = at & ~0xFu;
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t i = 0; i < 16; i++) {
    Store(base, aligned + 4 * i, i % 5 == 0 ? one : 0);
  }
}

// std: a doubleword, big-endian.
void StoreDoubleword(uint8_t* base, uint32_t at, uint64_t value) {
  Store(base, at, uint32_t(value >> 32));
  Store(base, at + 4, uint32_t(value));
}

// The 64 bytes at `from` to `to`, eight doubleword moves.
void Copy64(uint8_t* base, uint32_t from, uint32_t to) {
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t hi = Load(base, from + 8 * i);
    const uint32_t lo = Load(base, from + 8 * i + 4);
    Store(base, to + 8 * i, hi);
    Store(base, to + 8 * i + 4, lo);
  }
}

// fctiwz: NaN gives INT_MIN, INT_MAX and above INT_MAX, others truncated
// (INT_MIN below it).
int32_t Fctiwz(double x) {
  if (std::isnan(x)) {
    return INT32_MIN;
  }
  if (x >= double(INT32_MAX)) {
    return INT32_MAX;
  }
  if (x <= double(INT32_MIN) - 1.0) {
    return INT32_MIN;
  }
  return int32_t(x);
}

}  // namespace

bool SkeletonLocalMatrices(uint8_t* base, uint32_t skeleton, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82599C58 Skeleton::LocalMatrices");
  const Skel sk(base, skeleton);
  if (sk.node_values() == 0) {
    return false;
  }
  const uint32_t frame = stack - 320;
  const uint32_t turn = frame - 80;  // the rotations' own stack (r1 - 80)
  bool floats = false;
  uint32_t node = sk.node_states();
  uint32_t out = sk.matrices();
  uint32_t file_matrix = sk.file_matrices();
  int32_t count = int32_t(SkeletonNodeCount(base, skeleton));
  if (count > 0) {
    floats = true;
    const double two_pi = G::two_pi(base);
    const double inverse = G::inverse_two_pi(base);
    uint32_t record = 0;
    for (int32_t i = 0; i < count; i++) {
      if (int32_t(NodeRecord(base, sk.node_records() + record).type()) <= 4) {
        // The angles into [-pi, pi]: pi with each one's sign (its stored
        // bits ORed into 0x40490FDB), added, times 1 / (2 pi), truncated,
        // that many 2 pi taken off.
        // (An lfs then stfs of one word keeps its bits: CopyWord.)
        const NodeState state(base, node);
        const NodeMatrix m(base, out);
        const double a = state.angle_x();
        CopyWord(base, state.angle_x_address(), frame + 80);
        const double b = state.angle_y();
        CopyWord(base, state.angle_y_address(), frame + 92);
        const double c = state.angle_z();
        CopyWord(base, state.angle_z_address(), frame + 96);
        const auto signed_pi = [&](uint32_t at) {
          return (Load(base, at) & 0x80000000u) | 0x40490FDBu;
        };
        Store(base, frame + 100, signed_pi(frame + 80));
        Store(base, frame + 104, signed_pi(frame + 92));
        Store(base, frame + 108, signed_pi(frame + 96));
        double f9 = Fadds(Lfs(base, frame + 108), c);
        StoreIdentity(base, frame + 144);
        f9 = Fmuls(f9, inverse);
        const double f11 = Fmuls(Fadds(Lfs(base, frame + 100), a), inverse);
        const double f10 = Fmuls(Fadds(Lfs(base, frame + 104), b), inverse);
        Store(base, frame + 80, uint32_t(Fctiwz(f11)));
        const int32_t kb = Fctiwz(f10);
        const int32_t kc = Fctiwz(f9);
        const int64_t ka = int32_t(Load(base, frame + 80));
        Store(base, frame + 84, uint32_t(kb));
        StoreDoubleword(base, frame + 112, uint64_t(ka));
        const int64_t kb64 = int32_t(Load(base, frame + 84));
        Store(base, frame + 88, uint32_t(kc));
        StoreDoubleword(base, frame + 120, uint64_t(kb64));
        const double a2 = Fnmsubs(double(float(double(ka))), two_pi, a);
        state.set_angle_x(float(a2));
        const int64_t kc64 = int32_t(Load(base, frame + 88));
        StoreDoubleword(base, frame + 128, uint64_t(kc64));
        const double b2 = Fnmsubs(double(float(double(kb64))), two_pi, b);
        state.set_angle_y(float(b2));
        const double c2 = Fnmsubs(double(float(double(kc64))), two_pi, c);
        state.set_angle_z(float(c2));
        Copy64(base, frame + 144, out);
        // The scale's words, as words.
        const uint32_t sx = Load(base, state.scale_x_address());
        const uint32_t sy = Load(base, state.scale_y_address());
        const uint32_t sz = Load(base, state.scale_z_address());
        Stfs(base, frame + 96, a2);
        Store(base, m.m00_address(), sx);
        Store(base, m.m11_address(), sy);
        Store(base, m.m22_address(), sz);
        matrix_math::RotateX(base, out, out, frame + 96, turn);
        Stfs(base, frame + 96, b2);
        matrix_math::RotateY(base, out, out, frame + 96, turn);
        Stfs(base, frame + 96, c2);
        matrix_math::RotateZ(base, out, out, frame + 96, turn);
        Multiply(base, out, out, file_matrix, 0x82599E70);
        const uint32_t x = Load(base, state.translation_x_address());
        const uint32_t y = Load(base, state.translation_y_address());
        const uint32_t z = Load(base, state.translation_z_address());
        Store(base, m.m30_address(), x);
        Store(base, m.m31_address(), y);
        Store(base, m.m32_address(), z);
      }
      record += NodeRecord::kSize;
      node += NodeState::kSize;
      out += NodeMatrix::kSize;
      file_matrix += 64;
      count = int32_t(SkeletonNodeCount(base, skeleton));
    }
  }
  // The bones (+132 bit 0x80) from the file's first bone ([+80] +12), [+80]
  // + 8 of them: their 144-byte records from +140, no wrap or scale.
  const uint32_t file = sk.node_file();
  const uint32_t first = NodeFile(base, file).first_bone() << 6;
  out = sk.matrices() + first;
  file_matrix = first + sk.file_matrices();
  uint32_t bone = sk.bone_states();
  for (int32_t k = 0; k < int32_t(NodeFile(base, sk.node_file()).bone_count()); k++) {
    const uint32_t index = NodeFile(base, sk.node_file()).first_bone() + uint32_t(k);
    if ((NodeRecord(base, index * NodeRecord::kSize + sk.node_records()).type() & 0x80) != 0) {
      floats = true;
      const NodeState state(base, bone);
      const NodeMatrix m(base, out);
      StoreIdentity(base, frame + 144);
      Copy64(base, frame + 144, out);
      CopyWord(base, state.angle_x_address(), frame + 108);
      matrix_math::RotateX(base, out, out, frame + 108, turn);
      CopyWord(base, state.angle_y_address(), frame + 108);
      matrix_math::RotateY(base, out, out, frame + 108, turn);
      CopyWord(base, state.angle_z_address(), frame + 108);
      matrix_math::RotateZ(base, out, out, frame + 108, turn);
      Multiply(base, out, out, file_matrix, 0x82599FC8);
      const uint32_t x = Load(base, state.translation_x_address());
      const uint32_t y = Load(base, state.translation_y_address());
      const uint32_t z = Load(base, state.translation_z_address());
      Store(base, m.m30_address(), x);
      Store(base, m.m31_address(), y);
      Store(base, m.m32_address(), z);
    }
    bone += NodeState::kSize;
    out += NodeMatrix::kSize;
    file_matrix += 64;
  }
  return floats;
}

void SkeletonApplyAnimation(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825998B8 Skeleton::ApplyAnimation");
  const Skel sk(base, skeleton);
  Cursor cursor = sk.cursors();
  if (!cursor.section()) {
    return;
  }
  uint32_t entry = cursor.entries().address;
  if (!(int32_t(Load(base, cursor.entry_count_pointer())) > 0)) {
    return;
  }
  uint32_t offset = 0;
  int32_t i = 0;
  do {
    const NodeEntry e(base, entry);
    const uint32_t node = e.node();
    const NodeEntry entry_now(base, cursor.entries().address + offset);
    // The node values (a float4 a node, +8 its time) and the node states
    // (144 bytes, the channels' cursor words from +64) are arrays.
    const uint32_t time_at = (node << 4) + sk.node_values();
    const uint32_t records = sk.node_states();
    const uint32_t section = cursor.section().address;
    const uint32_t channels = e.channel_count();
    const uint32_t record = node * 144 + records;
    uint32_t channel = entry_now.channels_offset() + section;
    const double time = Lfs(base, time_at + 8);
    if (int32_t(channels) > 0) {
      int32_t j = 0;
      do {
        const Channel c(base, channel);
        const uint32_t component = c.component();
        const uint32_t keys = c.keys_address();
        const uint32_t key_count = c.key_count();
        const double value = CursorCurveSample(base, keys, ((component + 16) << 2) + record,
                                               key_count, time);
        Stfs(base, (component << 2) + record, value);
        channel = c.key_count() * 20 + keys;
        j++;
      } while (j < int32_t(e.channel_count()));
    }
    cursor = sk.cursors();
    i++;
    offset += 20;
    entry += 20;
  } while (i < int32_t(Load(base, cursor.entry_count_pointer())));
}

bool PrimitiveDrawerConstruct(uint8_t* base, uint32_t drawer) {
  ITC_NATIVE_RUNNING("sub_825A1378 PrimitiveDrawer::PrimitiveDrawer");
  const schema::PrimitiveDrawer d(base, drawer);
  d.set_vtable(schema::PrimitiveDrawer::kVtable);
  const auto block = memory::crt::OperatorNew(base, 4);
  if (!block) {
    return false;
  }
  d.set_declaration(*block);
  return true;
}

void RecordClear(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_825A18D8 RecordClear");
  const schema::PhysicalPadRecord rec(base, record);
  rec.set_unknown_0(float(Lfs(base, kZero)));
  rec.set_unknown_4(0);
  rec.set_unknown_8(0);
}

}  // namespace model_animation
