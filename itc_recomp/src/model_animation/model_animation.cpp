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

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// Whether operator delete frees `block` natively: 0, or a block the layer
// gave (else free's error path, the original's).
bool Deletable(uint32_t block) {
  return block == 0 || memory::IsHeapBlock(block);
}

// Whether operator new gives `size` bytes natively (malloc's checks; the
// layer aborts when out of memory): else the new handler and
// std::bad_alloc, the original's.
bool Allocatable(uint32_t size) {
  return size <= kMaxAllocation && memory::crt::GetProcessHeap() != 0;
}

// operator new[] once Allocatable said yes.
uint32_t NewArray(uint32_t size) {
  const auto block = memory::crt::OperatorNewArray(size);
  if (!block) {
    ITC_LOG_ERROR("model animation: operator new[]({}) failed after its checks", size);
    std::abort();
  }
  return *block;
}

// The track count a set will have once bound to `section` (TrackSet::View
// then TrackSet::Count), read before anything is written, for the set-ups'
// checks.
uint32_t SectionCount(uint32_t section) {
  return section == 0 ? 0 : Section(section).track_count();
}

// SetupFloat4 and Setup32: `stride`-byte values with a count header, at most
// `max_count` (else operator new[](-1)), the time at +`time`.
bool SetupWithHeader(uint32_t set, uint32_t section, uint32_t stride,
                     uint32_t max_count, uint32_t time) {
  const Set s(set);
  const uint32_t old = s.values().address;
  const uint32_t count = SectionCount(section);
  uint32_t size = 0xFFFFFFFFu;
  if (count <= max_count && count * stride <= 0xFFFFFFEFu) {
    size = count * stride + 16;
  }
  if ((old != 0 && !Deletable(old - 16)) || (count != 0 && !Allocatable(size))) {
    return false;
  }
  if (old != 0) {
    memory::crt::OperatorDeleteArray(old - 16);
  }
  s.set_values(0u);
  TrackSetView(set, section);
  uint32_t n = TrackSetCount(set);
  if (n == 0) {
    return true;
  }
  // A blob in PC memory (E5 step 5): the header and values in the guest's
  // byte order, read by address as before; operator delete[] frees it.
  const uint32_t block = schema::TrackValueBlock::New(size).address;
  uint32_t values = 0;
  if (block != 0) {
    const schema::TrackValueBlock values_block(block);
    values_block.set_count(n);
    values = values_block.values_address();
    int32_t left = int32_t(n) - 1;
    if (left >= 0) {
      const double one = Lfs(kOne);
      uint32_t at = values + 12;
      do {
        left--;
        Stfs(at, one);
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
    const double start = TrackSetStartTime(set, 0xFFFFFFFFu);
    const uint32_t at = offset + s.values().address;
    n--;
    offset += stride;
    Stfs(at + time, start);
  } while (n != 0);
  return true;
}

// Frees +16 (less `header`) and clears it.
bool ReleaseValues(uint32_t set, uint32_t header) {
  const Set s(set);
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

void TrackSetClear(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A6670 TrackSet::Clear");
  const Set s(set);
  s.set_start_time(float(Lfs(kZero)));
  s.set_section(0u);
  s.set_track_count_pointer(0);
  s.set_tracks(0u);
}

uint32_t TrackSetConstruct(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A1B70 TrackSet::TrackSet");
  TrackSetClear(set);
  Set(set).set_values(0u);
  return set;
}

void TrackSetView(uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A6690 TrackSet::View");
  const Set s(set);
  if (section == 0) {
    s.set_start_time(float(Lfs(kZero)));
    s.set_section(0u);
    s.set_track_count_pointer(0);
    s.set_tracks(0u);
    return;
  }
  const Section sec(section);
  s.set_section(section);
  s.set_track_count_pointer(sec.track_count_address());
  s.set_tracks(sec.tracks_address());
  s.set_start_time(sec.start_time());
}

uint32_t TrackSetCount(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A66D8 TrackSet::Count");
  const Set s(set);
  if (!s.section()) {
    return 0;
  }
  return Load(s.track_count_pointer());
}

namespace {

// StartTime and EndTime: the track's start or end time, the section's for
// -1.
double TrackTime(uint32_t set, uint32_t track, bool end) {
  const Set s(set);
  const Section section = s.section();
  if (!section) {
    return Lfs(kZero);
  }
  if (int32_t(track) == -1) {
    return double(end ? section.end_time() : section.start_time());
  }
  const Track t(track * 80 + s.tracks().address);
  return double(end ? t.end_time() : t.start_time());
}

}  // namespace

double TrackSetStartTime(uint32_t set, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825A1F08 TrackSet::StartTime");
  return TrackTime(set, track, false);
}

double TrackSetEndTime(uint32_t set, uint32_t track) {
  ITC_NATIVE_RUNNING("sub_825A1F50 TrackSet::EndTime");
  return TrackTime(set, track, true);
}

uint32_t TrackSetTrack(uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A2000 TrackSet::Track");
  const Set s(set);
  if (!s.section()) {
    return 0;
  }
  return index * 80 + s.tracks().address;
}

uint32_t TrackSetChannels(uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A2030 TrackSet::Channels");
  const Set s(set);
  const uint32_t section = s.section().address;
  if (section == 0) {
    return 0;
  }
  return Track(index * 80 + s.tracks().address).channels_offset() + section;
}

double KeyInterpolate(uint32_t key0, uint32_t key1, double time) {
  ITC_NATIVE_RUNNING("sub_825A66F8 Key::Interpolate");
  const Key k0(key0), k1(key1);
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
  const double two = G::float_two();
  const double out0 = k0.out_tangent();
  const double one = Lfs(kOne);
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

double CurveSample(uint32_t keys, uint32_t count, double time) {
  ITC_NATIVE_RUNNING("sub_825A1F98 Curve::Sample");
  if (!(int32_t(count) > 1)) {
    return Key(keys).value();
  }
  const uint32_t end = count * 20 + keys;
  const Key last(end - 20);
  if (!(time < double(last.time()))) {
    return last.value();
  }
  uint32_t key = keys + 20;
  if (!(time < double(Key(key).time()))) {
    do {
      key += 20;
    } while (!(time < double(Key(key).time())));
  }
  if (time == double(Key(key).time())) {
    return Key(key).value();
  }
  return KeyInterpolate(key - 20, key, time);
}

void TrackSetSampleFloat4(uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A1C88 TrackSet::SampleFloat4");
  const Set s(set);
  if (!s.section()) {
    return;
  }
  const Track track(TrackSetTrack(set, index));
  uint32_t channel = TrackSetChannels(set, index);
  const uint32_t offset = index << 4;
  const double zero = Lfs(kZero);
  const double one = Lfs(kOne);
  // The values are an array (16 bytes a track: x, y, z, the time).
  Stfs(offset + s.values().address, zero);
  Stfs(offset + s.values().address + 4, zero);
  Stfs(offset + s.values().address + 8, one);
  if (!(int32_t(track.channel_count()) > 0)) {
    return;
  }
  const uint32_t lanes = index << 2;
  int32_t i = 0;
  do {
    const Channel c(channel);
    const uint32_t keys = c.keys_address();
    const uint32_t values = s.values().address;
    const uint32_t lane = (c.component() + lanes) << 2;
    const uint32_t key_count = c.key_count();
    const double value = CurveSample(keys, key_count, Lfs(values + offset + 12));
    Stfs(lane + values, value);
    channel = c.key_count() * 20 + keys;
    i++;
  } while (i < int32_t(track.channel_count()));
}

void TrackSetSample32(uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825A0FE0 TrackSet::Sample32");
  const Set s(set);
  if (!s.section()) {
    return;
  }
  const Track track(TrackSetTrack(set, index));
  uint32_t channel = TrackSetChannels(set, index);
  const uint32_t offset = index << 5;
  // The values are an array (32 bytes a track: +16 the time, +20 the mask).
  const uint32_t value = offset + s.values().address;
  Store(value + 20, 0);
  if (!(int32_t(track.channel_count()) > 0)) {
    return;
  }
  int32_t i = 0;
  do {
    const Channel c(channel);
    const uint32_t component = c.component();
    const uint32_t keys = c.keys_address();
    const uint32_t mask = Load(value + 20);
    // slw: shifts of 32 to 63 give 0.
    const uint32_t bit = (component & 0x20) != 0 ? 0 : 1u << (component & 0x1F);
    Store(value + 20, bit | mask);
    const uint32_t key_count = c.key_count();
    const double sample =
        CurveSample(keys, key_count, Lfs(offset + s.values().address + 16));
    Stfs((component << 2) + value, sample);
    channel = c.key_count() * 20 + keys;
    i++;
  } while (i < int32_t(track.channel_count()));
}

namespace {

// Advance and Advance32: the time at value + `time`, `stride`-byte values.
uint32_t AdvanceTime(uint32_t set, uint32_t index, double step, uint32_t stride,
                     uint32_t time) {
  const Set s(set);
  if (!s.section()) {
    return 0;
  }
  const uint32_t at = index * stride + s.values().address + time;
  Stfs(at, Fadds(step, Lfs(at)));
  const double end = TrackSetEndTime(set, index);
  if (Lfs(at) > end) {
    Stfs(at, TrackSetStartTime(set, index));
    return 1;
  }
  const double start = TrackSetStartTime(set, index);
  if (!(Lfs(at) < start)) {
    return 0;
  }
  Stfs(at, TrackSetEndTime(set, index));
  return 1;
}

}  // namespace

uint32_t TrackSetAdvance(uint32_t set, uint32_t index, double step) {
  ITC_NATIVE_RUNNING("sub_825A1D68 TrackSet::Advance");
  return AdvanceTime(set, index, step, 16, 12);
}

uint32_t TrackSetAdvance32(uint32_t set, uint32_t index, double step) {
  ITC_NATIVE_RUNNING("sub_825A10A8 TrackSet::Advance32");
  return AdvanceTime(set, index, step, 32, 16);
}

uint32_t ModelAnimate(uint32_t model) {
  ITC_NATIVE_RUNNING("sub_82594B60 Model::Animate");
  const schema::Model m(model);
  const uint32_t float4s = m.track_set_float4_address();
  uint32_t count = TrackSetCount(float4s);
  const double one = Lfs(kOne);
  if (int32_t(count) > 0) {
    uint32_t i = 0;
    do {
      TrackSetSampleFloat4(float4s, i);
      TrackSetAdvance(float4s, i, one);
      i++;
      count = TrackSetCount(float4s);
    } while (int32_t(i) < int32_t(count));
  }
  const uint32_t materials = m.track_set_material_address();
  count = TrackSetCount(materials);
  if (int32_t(count) > 0) {
    uint32_t i = 0;
    do {
      TrackSetSample32(materials, i);
      TrackSetAdvance32(materials, i, one);
      i++;
      count = TrackSetCount(materials);
    } while (int32_t(i) < int32_t(count));
  }
  return count;
}

bool TrackSetReleaseArray(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A11D0 TrackSet::ReleaseArray");
  return ReleaseValues(set, 16);
}

bool TrackSetRelease(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_825A1BA8 TrackSet::Release");
  return ReleaseValues(set, 0);
}

bool TrackSetSetupFloat4(uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A1E08 TrackSet::SetupFloat4");
  return SetupWithHeader(set, section, 16, 0x0FFFFFFF, 12);
}

bool TrackSetSetup32(uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A1218 TrackSet::Setup32");
  return SetupWithHeader(set, section, 32, 0x07FFFFFF, 16);
}

bool TrackSetSetupFloat(uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A1BE8 TrackSet::SetupFloat");
  const Set s(set);
  const uint32_t old = s.values().address;
  const uint32_t count = SectionCount(section);
  const uint32_t size = count > 0x3FFFFFFF ? 0xFFFFFFFFu : count << 2;
  if ((old != 0 && !Deletable(old)) || (count != 0 && !Allocatable(size))) {
    return false;
  }
  if (old != 0) {
    memory::crt::OperatorDeleteArray(old);
  }
  s.set_values(0u);
  TrackSetView(set, section);
  uint32_t n = TrackSetCount(set);
  if (n == 0) {
    return true;
  }
  s.set_values(NewArray(size));
  if (!(int32_t(n) > 0)) {
    return true;
  }
  uint32_t offset = 0;
  do {
    const double start = TrackSetStartTime(set, 0xFFFFFFFFu);
    const uint32_t values = s.values().address;
    n--;
    Stfs(offset + values, start);
    offset += 4;
  } while (n != 0);
  return true;
}

bool TrackSetSetup208(uint32_t set, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A1A38 TrackSet::Setup208");
  const Set s(set);
  const uint32_t old = s.values().address;
  if (SectionCount(section) != 0 || (old != 0 && !Deletable(old - 16))) {
    return false;
  }
  if (old != 0) {
    memory::crt::OperatorDeleteArray(old - 16);
  }
  s.set_values(0u);
  TrackSetView(set, section);
  // The count is 0 (checked above): nothing is allocated or set up.
  TrackSetCount(set);
  return true;
}

// --- The skeleton. ---

namespace {

constexpr uint32_t kSkeletonBaseVtable = 0x8206CE60;

// Whether SkeletonBase::Free frees all four natively.
bool BaseFreeable(uint32_t skeleton) {
  const Skel sk(skeleton);
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
void Multiply(uint32_t out, uint32_t a, uint32_t b, uint32_t caller) {
  matrix_math::Multiply(out, a, b);
  interpolation::BlendState_OnMultiply(out, a, b, caller);
}

// Two doublewords from `from` to `to` (ld, std, ld, std).
void CopyRow(uint32_t from, uint32_t to) {
  for (uint32_t half = 0; half < 16; half += 8) {
    const uint32_t high = Load(from + half), low = Load(from + half + 4);
    Store(to + half, high);
    Store(to + half + 4, low);
  }
}

}  // namespace

uint32_t SkeletonBaseConstruct(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259A140 SkeletonBase::SkeletonBase");
  const Skel sk(skeleton);
  const uint32_t matrix = sk.matrix_address();
  const double one = Lfs(kOne);
  sk.set_vtable(kSkeletonBaseVtable);
  for (uint32_t row = 0; row < 4; row++) {
    Stfs(matrix + 16 * row + 12, one);
  }
  xact::WaveEntryConstruct(sk.node_file_address());
  // The identity, built on the original's stack and copied in.
  for (uint32_t word = 0; word < 16; word++) {
    Store(matrix + 4 * word, word % 5 == 0 ? 0x3F800000u : 0);
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

uint32_t SkeletonConstruct(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_82599758 Skeleton::Skeleton");
  SkeletonBaseConstruct(skeleton);
  const Skel sk(skeleton);
  sk.set_node_values(0);
  sk.set_vtable(Skel::kVtable);
  return skeleton;
}

bool SkeletonBaseFree(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259A228 SkeletonBase::Free");
  if (!BaseFreeable(skeleton)) {
    return false;
  }
  const Skel sk(skeleton);
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

bool SkeletonBaseDestruct(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259A9D0 SkeletonBase::~SkeletonBase");
  if (!BaseFreeable(skeleton)) {
    return false;
  }
  Skel(skeleton).set_vtable(kSkeletonBaseVtable);
  return SkeletonBaseFree(skeleton);
}

bool SkeletonDestruct(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825999C8 Skeleton::~Skeleton");
  const Skel sk(skeleton);
  const uint32_t values = sk.node_values();
  if (!Deletable(values) || !BaseFreeable(skeleton)) {
    return false;
  }
  sk.set_vtable(Skel::kVtable);
  if (values != 0) {
    memory::crt::OperatorDeleteArray(values);
  }
  sk.set_node_values(0);
  return SkeletonBaseDestruct(skeleton);
}

bool SkeletonFreeNodeValues(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825997A0 Skeleton::FreeNodeValues");
  const Skel sk(skeleton);
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

bool TrackSetReleasable(uint32_t set, bool array) {
  const uint32_t values = Set(set).values().address;
  return values == 0 || Deletable(values - (array ? 16 : 0));
}

bool SkeletonDestructible(uint32_t skeleton) {
  return Deletable(Skel(skeleton).node_values()) && BaseFreeable(skeleton);
}

bool SkeletonBaseDestructible(uint32_t skeleton) {
  return BaseFreeable(skeleton);
}

void NodeFileView(uint32_t view, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_825A21A8 NodeFile::View");
  const schema::NodeFileView v(view);
  const NodeFile f(file);
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
uint32_t CountedArray(uint32_t count, uint32_t bytes, uint32_t limit,
                      std::initializer_list<uint32_t> ones, double one) {
  // A blob in PC memory (E5 step 5): the count header and the entries in
  // the guest's byte order, read by address as before; operator delete[]
  // frees it (SkeletonBase::Free).
  const uint32_t array =
      schema::SkeletonCountedArray::New(ArrayBytes(count, bytes, limit)).address;
  Store(array, count);
  for (int32_t j = 0; j <= int32_t(count - 1); j++) {
    const uint32_t entry = array + 16 + bytes * uint32_t(j);
    for (const uint32_t offset : ones) {
      Stfs(entry + offset, one);
    }
    if (bytes == 144) {
      Store(entry + 132, 0);
      for (uint32_t w = 0; w < 16; w++) {
        Store(entry + 64 + 4 * w, 0);
      }
    }
  }
  return array + 16;
}

}  // namespace

bool SkeletonSetUp(uint32_t skeleton, uint32_t file, uint32_t matrices,
                   int32_t instances) {
  ITC_NATIVE_RUNNING("sub_8259A9E0 SkeletonBase::SetUp");
  // The checks first: the sizes operator new[] takes (the counts are the
  // file's, which the view only points at), and Free's.
  const uint32_t count = instances > 0 ? uint32_t(instances) : 1;
  const uint32_t nodes = NodeFile(file).node_count();
  const uint32_t entries = NodeFile(file).bone_count();
  if (count > kCursorLimit || !Allocatable(count * 20) ||
      (nodes != 0 && !Allocatable(ArrayBytes(nodes, 144, kNodeLimit))) ||
      (nodes != 0 && matrices == 0 && !Allocatable(ArrayBytes(nodes, 64, kMatrixLimit))) ||
      (entries != 0 && !Allocatable(ArrayBytes(entries, 64, kMatrixLimit))) ||
      !BaseFreeable(skeleton)) {
    return false;
  }
  SkeletonBaseFree(skeleton);
  const Skel sk(skeleton);
  sk.set_instance_count(instances);
  if (instances <= 0) {
    sk.set_instance_count(1);
  }
  const uint32_t cursors = NewArray(count * 20);
  for (uint32_t k = 0; k < count; k++) {
    SkeletonCursorClear(cursors + 20 * k);
  }
  sk.set_cursors(cursors);
  NodeFileView(sk.node_file_address(), file);
  const double one = Lfs(kOne);
  // The node file's +4 the node count, +8 the bones, +12 the first bone.
  const uint32_t node_count = NodeFile(sk.node_file()).node_count();
  if (node_count != 0) {
    sk.set_node_states(CountedArray(node_count, 144, kNodeLimit, {12, 28, 44, 60}, one));
    if (matrices == 0) {
      const uint32_t again = NodeFile(sk.node_file()).node_count();
      const uint32_t array = CountedArray(again, 64, kMatrixLimit, {12, 28, 44, 60}, one);
      sk.set_own_matrices(array);
      sk.set_matrices(array);
    } else {
      sk.set_matrices(matrices);
    }
  }
  const uint32_t entry_count = NodeFile(sk.node_file()).bone_count();
  if (entry_count != 0) {
    sk.set_bone_matrices(CountedArray(entry_count, 64, kMatrixLimit, {12, 28, 44}, one));
    for (int32_t i = 0; i < int32_t(NodeFile(sk.node_file()).bone_count()); i++) {
      Store(sk.bone_matrices() + 64 * uint32_t(i) + 52, 1);
    }
  }
  sk.set_bone_states(sk.node_states() + 144 * NodeFile(sk.node_file()).first_bone());
  return true;
}

bool SkeletonBind(uint32_t skeleton, uint32_t file, uint32_t matrices) {
  ITC_NATIVE_RUNNING("sub_82599A20 Skeleton::Bind");
  const uint32_t nodes = NodeFile(file).node_count();
  const uint32_t values_bytes = nodes > 0x0FFFFFFFu ? 0xFFFFFFFFu : nodes * 16;
  const Skel sk(skeleton);
  if (!Deletable(sk.node_values()) ||
      (nodes != 0 && !Allocatable(values_bytes)) ||
      !SkeletonSetUp(skeleton, file, matrices, 1)) {
    return false;
  }
  const uint32_t values = sk.node_values();
  if (values != 0) {
    memory::crt::OperatorDeleteArray(values);
  }
  sk.set_node_values(0);
  const uint32_t count = NodeFile(sk.node_file()).node_count();
  if (count != 0) {
    // A blob in PC memory (E5 step 5), read by address as before; operator
    // delete[] frees it.
    sk.set_node_values(schema::SkeletonNodeValue::New(values_bytes).address);
  }
  const double zero = Lfs(kZero);
  int32_t n = int32_t(SkeletonNodeCount(skeleton));
  for (int32_t i = 0; i < n; i++) {
    const uint32_t value = sk.node_values() + 16 * uint32_t(i);
    Stfs(value + 0, zero);
    Stfs(value + 4, zero);
    Stfs(value + 8, zero);
    n = int32_t(SkeletonNodeCount(skeleton));
  }
  return true;
}

void SkeletonCursorClear(uint32_t cursor) {
  ITC_NATIVE_RUNNING("sub_825A2068 SkeletonCursor::Clear");
  const Cursor c(cursor);
  c.set_start_time(float(Lfs(kZero)));
  c.set_section(0u);
  c.set_entry_count_pointer(0);
  c.set_entries(0u);
  c.set_unknown_16(0);
}

void SkeletonCursorView(uint32_t cursor, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825A2090 SkeletonCursor::View");
  const Cursor c(cursor);
  if (section == 0) {
    c.set_start_time(float(Lfs(kZero)));
    c.set_section(0u);
    c.set_entry_count_pointer(0);
    c.set_entries(0u);
    c.set_unknown_16(0);
    return;
  }
  const Section sec(section);
  c.set_section(section);
  c.set_entry_count_pointer(sec.track_count_address());
  c.set_entries(sec.tracks_address());
  c.set_start_time(sec.start_time());
  c.set_unknown_16(0);
}

void SkeletonLoadNodeValues(uint32_t skeleton, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_825997E0 Skeleton::LoadNodeValues");
  const Skel sk(skeleton);
  if (sk.node_values() == 0) {
    return;
  }
  SkeletonCursorView(sk.cursors().address, section);
  const Cursor cursor = sk.cursors();
  uint32_t entry = cursor.entries().address;
  if (!(int32_t(Load(cursor.entry_count_pointer())) > 0)) {
    return;
  }
  int32_t i = 0;
  do {
    const NodeEntry e(entry);
    const uint32_t node = e.node();
    const double a = e.time();
    // The node values are an array (a float4 a node).
    const uint32_t value = (node << 4) + sk.node_values();
    i++;
    Stfs(value + 0, a);
    const double x = Lfs(value + 0);
    const double b = e.unknown_16();
    entry += 20;
    Stfs(value + 4, b);
    Stfs(value + 8, x);
  } while (i < int32_t(Load(sk.cursors().entry_count_pointer())));
}

uint32_t SkeletonNodeCount(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_82599880 Skeleton::NodeCount");
  const Skel sk(skeleton);
  if (sk.node_values() == 0) {
    return 0;
  }
  return NodeFile(sk.node_file()).node_count();
}

MatricesResult SkeletonUpdateNodeMatrices(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_8259C3A8 Skeleton::UpdateNodeMatrices");
  // The node types the loop below takes, read first (the node file isn't
  // written here): 8 and 16 go to sub_8259B748 and sub_8259BE40.
  const Skel sk(skeleton);
  for (int32_t i = 0; i < int32_t(NodeFile(sk.node_file()).node_count()); i++) {
    const uint32_t type = NodeRecord(sk.node_records() + NodeRecord::kSize * uint32_t(i)).type();
    if (int32_t(type) > 4 && (type == 8 || type == 16)) {
      return MatricesResult::kOriginal;
    }
  }
  const uint32_t root = (sk.root_index() << 6) + sk.matrices();
  Multiply(root, root, sk.matrix_address(), 0x8259C3D4);
  uint32_t matrix = sk.matrices();
  const uint32_t records = sk.node_records();
  if (int32_t(NodeFile(sk.node_file()).node_count()) > 0) {
    uint32_t row = NodeMatrix(matrix).row3_xy_address();
    uint32_t record = 0;
    int32_t i = 0;
    do {
      if (!(int32_t(NodeRecord(records + record).type()) > 4)) {
        const uint32_t parent = uint32_t(NodeRecord(records + record).parent());
        if (int32_t(parent) != -1) {
          Multiply(matrix, matrix, (parent << 6) + sk.matrices(), 0x8259C42C);
        }
        const uint32_t source = NodeRecord(record + sk.node_records()).translation_node();
        if (int32_t(source) != -1) {
          CopyRow(NodeMatrix((source << 6) + sk.matrices()).row3_xy_address(), row);
        }
      }
      i++;
      record += NodeRecord::kSize;
      matrix += 64;
      row += 64;
    } while (i < int32_t(NodeFile(sk.node_file()).node_count()));
  }
  const uint32_t file = sk.node_file();
  const uint32_t matrices = sk.matrices();
  const uint32_t first = NodeFile(file).first_bone();
  matrix = (first << 6) + matrices;
  uint32_t bone = first * NodeRecord::kSize + sk.node_records();
  if (int32_t(NodeFile(file).bone_count()) > 0) {
    int32_t i = 0;
    do {
      if ((NodeRecord(bone).type() & 0x80) != 0) {
        const uint32_t parent = uint32_t(NodeRecord(bone).parent());
        if (int32_t(parent) != -1) {
          Multiply(matrix, matrix, (parent << 6) + sk.matrices(), 0x8259C528);
        }
      }
      i++;
      bone += NodeRecord::kSize;
      matrix += 64;
    } while (i < int32_t(NodeFile(sk.node_file()).bone_count()));
  }
  return MatricesResult::kMultiplied;
}

MatricesResult SkeletonUpdateMatrices(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825998A0 Skeleton::UpdateMatrices");
  if (Skel(skeleton).node_values() == 0) {
    return MatricesResult::kNothing;
  }
  return SkeletonUpdateNodeMatrices(skeleton);
}

double CursorCurveSample(uint32_t keys, uint32_t cursor, uint32_t count,
                         double time) {
  ITC_NATIVE_RUNNING("sub_825A20E8 CursorCurve::Sample");
  const Key first(keys);
  if (!(int32_t(count) > 1)) {
    return first.value();
  }
  const uint32_t end = count * 20 + keys;
  const Key last(end - 20);
  if (!(time < double(last.time()))) {
    return last.value();
  }
  if (!(time > double(first.time()))) {
    return first.value();
  }
  const uint32_t at = Load(cursor);
  if (!(int32_t(at) > 0) || !(int32_t(at) < int32_t(count))) {
    Store(cursor, 1);
  }
  uint32_t next = Load(cursor) * 20 + keys;
  uint32_t key = next - 20;
  for (;;) {
    if (!(double(Key(key).time()) > time) && time < double(Key(next).time())) {
      return KeyInterpolate(key, next, time);
    }
    const uint32_t moved = Load(cursor) + 1;
    key += 20;
    next += 20;
    Store(cursor, moved);
    if (int32_t(moved) < int32_t(count)) {
      continue;
    }
    key = keys;
    Store(cursor, 1);
    next = keys + 20;
  }
}

bool SkeletonResetPose(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_82599AF0 Skeleton::ResetPose");
  const Skel sk(skeleton);
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
    for (uint32_t k = 0; k < 4; k++) words[k] = Load(from + 4 * k);
    for (uint32_t k = 0; k < 4; k++) Store(to + 4 * k, words[k]);
  };
  for (int32_t i = 0; i < int32_t(SkeletonNodeCount(skeleton)); i++) {
    const NodeState state(out);
    const bool extra = (NodeRecord(record).type() & 0x40) != 0;
    // The original loads +88's, +92's and the first half of +96's before
    // its first stores, then the rest.
    uint32_t bw[4], aw[4], cw[4];
    for (uint32_t k = 0; k < 4; k++) bw[k] = Load(b + 4 * k);
    for (uint32_t k = 0; k < 4; k++) aw[k] = Load(a + 4 * k);
    cw[0] = Load(c + 0);
    cw[1] = Load(c + 4);
    for (uint32_t k = 0; k < 4; k++) Store(state.translation_x_address() + 4 * k, bw[k]);
    cw[2] = Load(c + 8);
    cw[3] = Load(c + 12);
    for (uint32_t k = 0; k < 4; k++) Store(state.angle_x_address() + 4 * k, aw[k]);
    for (uint32_t k = 0; k < 4; k++) Store(state.scale_x_address() + 4 * k, cw[k]);
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
  SkeletonApplyAnimation(skeleton);
  return true;
}

namespace {

// The identity's 16 words (StoreIdentity's) stored at `to` in order, as
// the original's copy of its frame's identity (eight doubleword moves).
void StoreIdentityAt(uint32_t to) {
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t i = 0; i < 16; i++) {
    Store(to + 4 * i, i % 5 == 0 ? one : 0);
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

bool SkeletonLocalMatrices(uint32_t skeleton, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82599C58 Skeleton::LocalMatrices");
  const Skel sk(skeleton);
  if (sk.node_values() == 0) {
    return false;
  }
  const uint32_t frame = stack - 320;
  const uint32_t turn = frame - 80;  // the rotations' own stack (r1 - 80)
  bool floats = false;
  uint32_t node = sk.node_states();
  uint32_t out = sk.matrices();
  uint32_t file_matrix = sk.file_matrices();
  int32_t count = int32_t(SkeletonNodeCount(skeleton));
  if (count > 0) {
    floats = true;
    const double two_pi = G::two_pi();
    const double inverse = G::inverse_two_pi();
    uint32_t record = 0;
    for (int32_t i = 0; i < count; i++) {
      if (int32_t(NodeRecord(sk.node_records() + record).type()) <= 4) {
        // The angles into [-pi, pi]: pi with each one's sign (its stored
        // bits ORed into 0x40490FDB), added, times 1 / (2 pi), truncated,
        // that many 2 pi taken off.
        // (An lfs then stfs of one word keeps its bits.) The angles' words
        // and the original's frame temporaries (+80 to +135, +144's
        // identity) only this function reads: host locals (E5 step 6).
        const NodeState state(node);
        const NodeMatrix m(out);
        const double a = state.angle_x();
        const uint32_t a_bits = Load(state.angle_x_address());
        const double b = state.angle_y();
        const uint32_t b_bits = Load(state.angle_y_address());
        const double c = state.angle_z();
        const uint32_t c_bits = Load(state.angle_z_address());
        const auto signed_pi = [](uint32_t bits) {
          return double(std::bit_cast<float>((bits & 0x80000000u) | 0x40490FDBu));
        };
        const double pi_a = signed_pi(a_bits);
        const double pi_b = signed_pi(b_bits);
        const double pi_c = signed_pi(c_bits);
        double f9 = Fadds(pi_c, c);
        f9 = Fmuls(f9, inverse);
        const double f11 = Fmuls(Fadds(pi_a, a), inverse);
        const double f10 = Fmuls(Fadds(pi_b, b), inverse);
        const int64_t ka = Fctiwz(f11);
        const int64_t kb64 = Fctiwz(f10);
        const int64_t kc64 = Fctiwz(f9);
        const double a2 = Fnmsubs(double(float(double(ka))), two_pi, a);
        state.set_angle_x(float(a2));
        const double b2 = Fnmsubs(double(float(double(kb64))), two_pi, b);
        state.set_angle_y(float(b2));
        const double c2 = Fnmsubs(double(float(double(kc64))), two_pi, c);
        state.set_angle_z(float(c2));
        StoreIdentityAt(out);
        // The scale's words, as words.
        const uint32_t sx = Load(state.scale_x_address());
        const uint32_t sy = Load(state.scale_y_address());
        const uint32_t sz = Load(state.scale_z_address());
        Stfs(frame + 96, a2);
        Store(m.m00_address(), sx);
        Store(m.m11_address(), sy);
        Store(m.m22_address(), sz);
        matrix_math::RotateX(out, out, frame + 96, turn);
        Stfs(frame + 96, b2);
        matrix_math::RotateY(out, out, frame + 96, turn);
        Stfs(frame + 96, c2);
        matrix_math::RotateZ(out, out, frame + 96, turn);
        Multiply(out, out, file_matrix, 0x82599E70);
        const uint32_t x = Load(state.translation_x_address());
        const uint32_t y = Load(state.translation_y_address());
        const uint32_t z = Load(state.translation_z_address());
        Store(m.m30_address(), x);
        Store(m.m31_address(), y);
        Store(m.m32_address(), z);
      }
      record += NodeRecord::kSize;
      node += NodeState::kSize;
      out += NodeMatrix::kSize;
      file_matrix += 64;
      count = int32_t(SkeletonNodeCount(skeleton));
    }
  }
  // The bones (+132 bit 0x80) from the file's first bone ([+80] +12), [+80]
  // + 8 of them: their 144-byte records from +140, no wrap or scale.
  const uint32_t file = sk.node_file();
  const uint32_t first = NodeFile(file).first_bone() << 6;
  out = sk.matrices() + first;
  file_matrix = first + sk.file_matrices();
  uint32_t bone = sk.bone_states();
  for (int32_t k = 0; k < int32_t(NodeFile(sk.node_file()).bone_count()); k++) {
    const uint32_t index = NodeFile(sk.node_file()).first_bone() + uint32_t(k);
    if ((NodeRecord(index * NodeRecord::kSize + sk.node_records()).type() & 0x80) != 0) {
      floats = true;
      const NodeState state(bone);
      const NodeMatrix m(out);
      StoreIdentityAt(out);
      CopyWord(state.angle_x_address(), frame + 108);
      matrix_math::RotateX(out, out, frame + 108, turn);
      CopyWord(state.angle_y_address(), frame + 108);
      matrix_math::RotateY(out, out, frame + 108, turn);
      CopyWord(state.angle_z_address(), frame + 108);
      matrix_math::RotateZ(out, out, frame + 108, turn);
      Multiply(out, out, file_matrix, 0x82599FC8);
      const uint32_t x = Load(state.translation_x_address());
      const uint32_t y = Load(state.translation_y_address());
      const uint32_t z = Load(state.translation_z_address());
      Store(m.m30_address(), x);
      Store(m.m31_address(), y);
      Store(m.m32_address(), z);
    }
    bone += NodeState::kSize;
    out += NodeMatrix::kSize;
    file_matrix += 64;
  }
  return floats;
}

void SkeletonApplyAnimation(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_825998B8 Skeleton::ApplyAnimation");
  const Skel sk(skeleton);
  Cursor cursor = sk.cursors();
  if (!cursor.section()) {
    return;
  }
  uint32_t entry = cursor.entries().address;
  if (!(int32_t(Load(cursor.entry_count_pointer())) > 0)) {
    return;
  }
  uint32_t offset = 0;
  int32_t i = 0;
  do {
    const NodeEntry e(entry);
    const uint32_t node = e.node();
    const NodeEntry entry_now(cursor.entries().address + offset);
    // The node values (a float4 a node, +8 its time) and the node states
    // (144 bytes, the channels' cursor words from +64) are arrays.
    const uint32_t time_at = (node << 4) + sk.node_values();
    const uint32_t records = sk.node_states();
    const uint32_t section = cursor.section().address;
    const uint32_t channels = e.channel_count();
    const uint32_t record = node * 144 + records;
    uint32_t channel = entry_now.channels_offset() + section;
    const double time = Lfs(time_at + 8);
    if (int32_t(channels) > 0) {
      int32_t j = 0;
      do {
        const Channel c(channel);
        const uint32_t component = c.component();
        const uint32_t keys = c.keys_address();
        const uint32_t key_count = c.key_count();
        const double value = CursorCurveSample(keys, ((component + 16) << 2) + record,
                                               key_count, time);
        Stfs((component << 2) + record, value);
        channel = c.key_count() * 20 + keys;
        j++;
      } while (j < int32_t(e.channel_count()));
    }
    cursor = sk.cursors();
    i++;
    offset += 20;
    entry += 20;
  } while (i < int32_t(Load(cursor.entry_count_pointer())));
}

bool PrimitiveDrawerConstruct(uint32_t drawer) {
  ITC_NATIVE_RUNNING("sub_825A1378 PrimitiveDrawer::PrimitiveDrawer");
  const schema::PrimitiveDrawer d(drawer);
  d.set_vtable(schema::PrimitiveDrawer::kVtable);
  const auto block = memory::crt::OperatorNew(4);
  if (!block) {
    return false;
  }
  d.set_declaration(*block);
  return true;
}

void RecordClear(uint32_t record) {
  ITC_NATIVE_RUNNING("sub_825A18D8 RecordClear");
  const schema::PhysicalPadRecord rec(record);
  rec.set_unknown_0(float(Lfs(kZero)));
  rec.set_unknown_4(0);
  rec.set_unknown_8(0);
}

}  // namespace model_animation
