// The TV camera's route and path math (docs/functions/camera-modes.md): the
// route ranges' camera spot and walk along the route, the fraction of a
// slice a point is past, the TV camera's tracking while another view shows,
// and the camera paths' keys evaluated along the course's route (the keyed
// curves, the route's Catmull-Rom spline and its frame). Plain C++ over
// guest memory, the originals' loads and stores in their order, their float
// steps as the translated code's (src/camera_modes/steps.h); the vector
// unit's work with flush-to-zero on, the rest with it off. The originals'
// stack frames (their locals and the vectors they load back) are written
// the same way, at the frames the callers hand in (`sp` is the caller's r1,
// each frame below it by the original's size); saved registers aren't.

#include "camera_modes/camera_modes.h"

#include "camera_modes/steps.h"

#include "base/log.h"
#include "c_runtime/exit.h"
#include "c_runtime/strings.h"
#include "cameras/cameras.h"
#include "guest.h"
#include "matrix_math/matrix_math.h"
#include "race_overlays/race_overlays.h"
#include "track_route/track_route.h"
#include "track_zones/track_zones.h"

namespace camera_modes {

using namespace steps;

namespace {

constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kRoute = 0x826C5B10;  // the course's route (its file)
constexpr uint32_t kFlip = 0x82001BE0;   // the z flip (-1.0, probably)

// TrackRoute::SplinePoint's Catmull-Rom matrix and its guard (bit 1).
constexpr uint32_t kSplineMatrix = 0x826C2D30;
constexpr uint32_t kSplineGuard = 0x826C2D70;
constexpr uint32_t kSplineAtexit = 0x825DAE08;

uint32_t Load(uint8_t* base, uint32_t a) { return guest::LoadU32(base, a); }
void Store(uint8_t* base, uint32_t a, uint32_t v) { guest::StoreU32(base, a, v); }
uint32_t LoadU16(uint8_t* base, uint32_t a) { return guest::LoadU16(base, a); }
int32_t LoadS16(uint8_t* base, uint32_t a) { return int16_t(guest::LoadU16(base, a)); }
void StoreU16(uint8_t* base, uint32_t a, uint32_t v) { guest::StoreU16(base, a, uint16_t(v)); }
// lfs then stfs: the bits as the translated code keeps them (Matrix::FromFloats' note).
void CopyWord(uint8_t* base, uint32_t to, uint32_t from) { Store(base, to, Load(base, from)); }

// lvx: the aligned 16 bytes holding `address`.
simde__m128 LoadVector(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(base, at), Load(base, at + 4), Load(base, at + 8), Load(base, at + 12));
}
// The vector unit's dot product of x, y and z, in every lane (vdot3).
simde__m128 Dot3(simde__m128 a, simde__m128 b) { return simde_mm_dp_ps(a, b, 0xEF); }
// -(a b - c): the translated vnmsubfp.
simde__m128 NegMulSub(simde__m128 a, simde__m128 b, simde__m128 c) {
  return simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(a, b), c),
                         simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
}
// vsel: mask's lanes from `on`, the others from `off`.
simde__m128 Select(simde__m128 mask, simde__m128 off, simde__m128 on) {
  const simde__m128i m = simde_mm_castps_si128(mask);
  return simde_mm_castsi128_ps(
      simde_mm_or_si128(simde_mm_andnot_si128(m, simde_mm_castps_si128(off)),
                        simde_mm_and_si128(m, simde_mm_castps_si128(on))));
}
simde__m128 ReciprocalRoot(simde__m128 v) {
  return simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(v));
}

// A section of the route's table by its number (the low halfword).
uint32_t SectionOf(uint8_t* base, uint32_t table, uint32_t number) {
  return Load(base, table + ((number << 2) & 0x3FFFC));
}

}  // namespace

// --- The route ranges ---

int64_t RangeCameraAt(uint8_t* base, uint32_t range, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820FE830 RouteRange::CameraAt");
  const uint32_t old = Load(base, range);
  Store(base, range, 1);
  const track_zones::Position p{uint16_t(LoadU16(base, position)),
                                uint16_t(LoadU16(base, position + 2))};
  int64_t result = -1;
  if (track_zones::RangeTest(base, range, p) == 2) {
    result = int64_t(uint64_t(Load(base, track_zones::kSliceCount)));
  }
  Store(base, range, old);
  return result;
}

void RangeAdvance(uint8_t* base, uint32_t out, uint32_t range, int32_t distance) {
  ITC_NATIVE_RUNNING("sub_820FE598 RouteRange::Advance");
  int32_t section = LoadS16(base, range + 4);
  const int32_t end = LoadS16(base, range + 8);
  uint32_t left = uint32_t(distance);
  StoreU16(base, out + 0, 0);
  StoreU16(base, out + 2, 0);
  if (end != section) {
    const uint32_t table = Load(base, Load(base, kRoute) + 12);
    do {
      const uint32_t s = SectionOf(base, table, uint32_t(section));
      const int32_t length = LoadS16(base, s + 0);
      if (int32_t(left) < length) {
        break;
      }
      left -= uint32_t(length);
      int32_t links = 0;
      for (uint32_t i = 0; i < 3; i++) {
        if (LoadU16(base, s + 28 + 4 * i) != 0xFFFF) {
          links++;
        }
      }
      if (links == 1) {
        for (uint32_t i = 0; i < 3; i++) {
          if (LoadU16(base, s + 28 + 4 * i) != 0xFFFF) {
            section = LoadS16(base, s + (i + 7) * 4);
            break;
          }
        }
      } else if (links > 1) {
        // The fork's link that's in the range's list of sections to take
        // (+12: a 64-slot list, its head +272, count +276, data +280).
        int32_t last = -1;
        for (uint32_t i = 0; i < 3; i++) {
          if (section == last) {
            break;
          }
          const uint32_t count = uint32_t(LoadS16(base, range + 276));
          last = LoadS16(base, s + 28 + 4 * i);
          if (count == 0) {
            continue;
          }
          const uint32_t list = range + 12;
          const uint32_t head = uint32_t(LoadS16(base, list + 260)) * 4 + list;
          for (uint32_t j = 0; j < count; j++) {
            uint32_t node = head;
            for (uint32_t k = j; k != 0; k--) {
              node = uint32_t(LoadS16(base, node)) * 4 + list;
            }
            const uint32_t slot = (uint32_t(LoadS16(base, node + 2)) + 67) * 4;
            if (last == int32_t(Load(base, slot + list))) {
              section = last;
              break;
            }
          }
        }
      }
    } while (end != section);
  }
  StoreU16(base, out + 0, uint32_t(section));
  StoreU16(base, out + 2, left);
}

// --- The fraction of a slice a point is past ---

double SliceFraction(uint8_t* base, uint32_t position, uint32_t point, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820F9B48 TrackRoute::SliceFraction");
  const uint32_t f = sp - 176;
  const uint32_t word = Load(base, position);
  const double px = Lfs(base, point + 0);
  const double py = Lfs(base, point + 4);
  const uint32_t file = Load(base, kRoute);
  const double pz = Lfs(base, point + 8);
  Store(base, f + 80, word);
  const uint32_t section = LoadU16(base, f + 80);
  const int32_t slice = LoadS16(base, f + 82);
  CopyWord(base, f + 124, kOne);
  CopyWord(base, f + 108, kOne);
  const uint32_t s = Load(base, Load(base, file + 12) + (section << 2));
  const int32_t next = slice + 1;
  const bool within = LoadS16(base, s + 0) > next;
  const int32_t n = LoadS16(base, s + 0);
  const uint32_t a = s + uint32_t(n) * 84 + uint32_t(slice) * 12;
  const uint32_t b = s + uint32_t(n) * 96 + uint32_t(slice) * 12;
  // The point's foot on the line through the slice's two edge points.
  const double ax = Lfs(base, a + 124);
  Stfs(base, f + 96, Fsubs(px, ax));
  const double ay = Lfs(base, a + 128);
  Stfs(base, f + 100, Fsubs(py, ay));
  const double az = Lfs(base, a + 132);
  Stfs(base, f + 104, Fsubs(pz, az));
  double dx = Fsubs(Lfs(base, b + 124), ax);
  double dy = Fsubs(Lfs(base, b + 128), ay);
  double dz = Fsubs(Lfs(base, b + 132), az);
  Stfs(base, f + 112, dx);
  Stfs(base, f + 116, dy);
  Stfs(base, f + 120, dz);
  {
    simde__m128 v13 = LoadVector(base, f + 96);
    simde__m128 v0 = LoadVector(base, f + 112);
    FlushToZero flush;
    v13 = Dot3(v0, v13);
    v0 = Dot3(v0, v0);
    Stvx(base, f + 128, v13);
    Stvx(base, f + 144, v0);
  }
  double scale = Fdivs(Lfs(base, f + 128), Lfs(base, f + 144));
  dx = Fmuls(dx, scale);
  dy = Fmuls(dy, scale);
  dz = Fmuls(dz, scale);
  const double qx = Fadds(dx, ax);
  const double qy = Fadds(dy, ay);
  const double qz = Fadds(dz, az);
  // The next slice: on in the section, else the section's link 0.
  uint32_t next_section = section;
  int32_t next_slice;
  if (within) {
    next_slice = int16_t(next);
  } else {
    next_section = uint32_t(track_route::Link(base, kRoute, section, 0));
    next_slice = 0;
  }
  const uint32_t s2 = SectionOf(base, Load(base, file + 12), next_section);
  const int32_t n2 = LoadS16(base, s2 + 0);
  const uint32_t a2 = s2 + uint32_t(n2) * 84 + uint32_t(next_slice) * 12;
  const uint32_t b2 = s2 + uint32_t(n2) * 96 + uint32_t(next_slice) * 12;
  const double ax2 = Lfs(base, a2 + 124);
  Stfs(base, f + 96, Fsubs(px, ax2));
  const double ay2 = Lfs(base, a2 + 128);
  const double ex = Fsubs(px, qx);
  Stfs(base, f + 100, Fsubs(py, ay2));
  const double az2 = Lfs(base, a2 + 132);
  Stfs(base, f + 104, Fsubs(pz, az2));
  const double dx2 = Fsubs(Lfs(base, b2 + 124), ax2);
  const double dy2 = Fsubs(Lfs(base, b2 + 128), ay2);
  const double dz2 = Fsubs(Lfs(base, b2 + 132), az2);
  simde__m128 v13 = LoadVector(base, f + 96);
  Stfs(base, f + 112, dx2);
  Stfs(base, f + 116, dy2);
  Stfs(base, f + 120, dz2);
  Stfs(base, f + 96, ex);
  Stfs(base, f + 100, Fsubs(py, qy));
  Stfs(base, f + 104, Fsubs(pz, qz));
  simde__m128 v0 = LoadVector(base, f + 112);
  simde__m128 v12;
  {
    FlushToZero flush;
    v13 = Dot3(v0, v13);
    v0 = Dot3(v0, v0);
  }
  v12 = LoadVector(base, f + 96);
  Stvx(base, f + 144, v13);
  Stvx(base, f + 128, v0);
  const double scale2 = Fdivs(Lfs(base, f + 144), Lfs(base, f + 128));
  const double qx2 = Fadds(Fmuls(dx2, scale2), ax2);
  const double qy2 = Fadds(Fmuls(dy2, scale2), ay2);
  const double qz2 = Fadds(Fmuls(dz2, scale2), az2);
  Stfs(base, f + 112, Fsubs(qx2, qx));
  Stfs(base, f + 116, Fsubs(qy2, qy));
  Stfs(base, f + 120, Fsubs(qz2, qz));
  v0 = LoadVector(base, f + 112);
  {
    FlushToZero flush;
    v13 = Dot3(v0, v12);
    v0 = Dot3(v0, v0);
    Stvx(base, f + 144, v13);
    Stvx(base, f + 128, v0);
  }
  return Fdivs(Lfs(base, f + 144), Lfs(base, f + 128));
}

// --- The TV camera's tracking ---

void TvTrack(uint8_t* base, uint32_t camera, uint32_t position, uint32_t frame,
             const std::function<void(uint32_t buffer, uint32_t index)>& request) {
  ITC_NATIVE_RUNNING("sub_82156980 TvCamera::Track");
  // A 40-slot list on the stack (frame + 96; List40::'s layout), the
  // ranges the position is on.
  const uint32_t list = frame + 96;
  for (uint32_t i = 0; i < 40; i++) {
    StoreU16(base, list + 4 * i + 2, i);
    StoreU16(base, list + 4 * i + 0, i + 1);
  }
  StoreU16(base, frame + 252, 0xFFFF);
  StoreU16(base, frame + 260, 0xFFFF);
  StoreU16(base, frame + 262, 0xFFFF);
  StoreU16(base, frame + 258, 0xFFFF);
  uint32_t ranges = Load(base, camera + 452);
  StoreU16(base, frame + 256, 0);
  StoreU16(base, frame + 264, 0);
  uint32_t count = 0;
  uint32_t i = 0;
  Store(base, frame + 80, i);
  if (ranges > 0) {
    do {
      const uint32_t range = camera + 456 + i * 1064;
      const track_zones::Position p{uint16_t(LoadU16(base, position)),
                                    uint16_t(LoadU16(base, position + 2))};
      if (track_zones::RangeUpdate(base, range, p) != 0) {
        List40Insert(base, list, Load(base, frame + 80), uint32_t(int32_t(int16_t(count))));
        count = LoadU16(base, frame + 264);
      }
      i++;
      ranges = Load(base, camera + 452);
      Store(base, frame + 80, i);
    } while (i < ranges);
  }
  const uint32_t listed = uint32_t(int32_t(int16_t(count)));
  if (listed == 0) {
    return;
  }
  // Each listed range's path asked for unless a path buffer (+43016, four
  // 28-byte buffers: +12 the index, +24 the state) has it; once one has,
  // none after are asked for (the original's flag isn't cleared).
  const uint32_t head = uint32_t(LoadS16(base, frame + 260)) * 4 + list;
  bool found = false;
  auto node_at = [&](uint32_t j) {
    uint32_t node = head;
    for (uint32_t k = j; k != 0; k--) {
      node = uint32_t(LoadS16(base, node)) * 4 + list;
    }
    return node;
  };
  for (uint32_t j = 0; j < listed; j++) {
    for (uint32_t k = 0; k < 4; k++) {
      const uint32_t node = node_at(j);
      const uint32_t index = Load(base, frame + 268 + uint32_t(LoadS16(base, node + 2)) * 4);
      if (Load(base, camera + 43028 + 28 * k) == index) {
        found = true;
        break;
      }
    }
    if (found) {
      continue;
    }
    const uint32_t node = node_at(j);
    const uint32_t index = Load(base, frame + 268 + uint32_t(LoadS16(base, node + 2)) * 4);
    for (uint32_t k = 0; k < 4; k++) {
      const int32_t state = int32_t(Load(base, camera + 43040 + 28 * k));
      if (state == 0 || state == 3) {
        request(camera + 43016 + 28 * k, index);
        break;
      }
    }
  }
}

// --- The camera paths' curves ---

void HermiteBasis(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, double t) {
  ITC_NATIVE_RUNNING("sub_82293A48 Hermite::Basis");
  double f0 = Fmuls(t, t);
  double f9 = Lfs(base, a + 8);
  double f8 = Lfs(base, b + 8);
  const double f7 = Lfs(base, a + 0);
  const double f6 = Lfs(base, b + 0);
  double f13 = Lfs(base, 0x820021E8);  // 2.0
  double f11 = Lfs(base, 0x820021E4);  // 3.0
  double f12 = Fnmsubs(t, f13, f11);
  f11 = Fmuls(f0, f11);
  f12 = Fmuls(f12, t);
  const double f10 = Fmuls(f12, t);
  f12 = Fmuls(f0, t);
  const double f5 = Fnmsubs(f0, f13, f12);
  f11 = Fmsubs(f12, f13, f11);
  f0 = Fsubs(f12, f0);
  f12 = Lfs(base, kOne);
  f13 = Fadds(f5, t);
  f12 = Fadds(f11, f12);
  f9 = Fmuls(f9, f13);
  f11 = Fmadds(f8, f0, f9);
  f11 = Fmadds(f7, f12, f11);
  f11 = Fmadds(f6, f10, f11);
  Stfs(base, out + 0, f11);
  f0 = Fmuls(Lfs(base, b + 12), f0);
  const double b4 = Lfs(base, b + 4);
  f9 = Lfs(base, a + 12);
  f8 = Lfs(base, a + 4);
  f0 = Fmadds(b4, f10, f0);
  f0 = Fmadds(f9, f13, f0);
  f0 = Fmadds(f8, f12, f0);
  Stfs(base, out + 4, f0);
}

double CurveEvaluate(uint8_t* base, uint32_t curve, double t, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820AC060 Curve::Evaluate");
  const uint32_t f = sp - 144;
  uint32_t prev = curve;
  uint32_t next = curve + 24;
  uint32_t end = next;
  while (Load(base, end + 20) != 3) {
    end += 24;
  }
  const double at = Fmuls(Lfs(base, end + 0), t);
  double time;
  for (;;) {
    time = Lfs(base, next + 0);
    if (!(time < at)) {
      break;
    }
    next += 24;
    prev += 24;
  }
  const uint32_t kind = Load(base, prev + 20);
  if (kind < 1) {
    return Lfs(base, prev + 8);
  }
  if (kind == 1) {
    double f0 = Lfs(base, prev + 0);
    const double f13 = Lfs(base, next + 0);
    const double f11 = Fsubs(at, f0);
    f0 = Fsubs(f13, f0);
    const double f12 = Lfs(base, next + 4);
    const double f10 = Lfs(base, prev + 8);
    f0 = Fdivs(f11, f0);
    const double weight = Fsubs(Lfs(base, kOne), f0);
    f0 = Fmuls(f12, f0);
    return Fmadds(weight, f10, f0);
  }
  if (kind < 3) {
    // The two keys as (value, tangent) pairs on the stack (+96, +112), the
    // tangents scaled by the segment's length, Hermite::Basis into +80.
    const double f12 = time;
    CopyWord(base, f + 100, prev + 8);
    const double f13 = Lfs(base, prev + 0);
    CopyWord(base, f + 116, next + 4);
    const double f11 = Fsubs(at, f13);
    const double f10 = Lfs(base, prev + 16);
    CopyWord(base, f + 96, prev + 0);
    const double f9 = Lfs(base, next + 12);
    CopyWord(base, f + 112, next + 0);
    const double length = Fsubs(f12, f13);
    Stfs(base, f + 104, length);
    Stfs(base, f + 120, length);
    Stfs(base, f + 108, Fmuls(f10, length));
    Stfs(base, f + 124, Fmuls(f9, length));
    HermiteBasis(base, f + 80, f + 96, f + 112, Fdivs(f11, length));
    return Lfs(base, f + 84);
  }
  // A key of kind 3 or more: the stack's word at +80, as the original
  // leaves it (never seen: only a curve's end key has kind 3).
  return Lfs(base, f + 80);
}

double KeyValue(uint8_t* base, uint32_t pair, double t, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820AC178 Key::Value");
  const uint32_t f = sp - 96;
  const int32_t kind = int32_t(Load(base, pair + 0));
  double value;
  if (kind == 0) {
    value = Lfs(base, pair + 4);
  } else if (kind == 1) {
    value = CurveEvaluate(base, Load(base, pair + 4), t, f);
  } else {
    value = Lfs(base, kZero);
  }
  return Fmuls(value, Lfs(base, 0x82002834));
}

void KeyVec3(uint8_t* base, uint32_t pairs, uint32_t out, double t, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820AC1D0 Key::Vec3");
  const uint32_t f = sp - 112;
  Stfs(base, out + 0, KeyValue(base, pairs + 0, t, f));
  Stfs(base, out + 4, KeyValue(base, pairs + 8, t, f));
  Stfs(base, out + 8, KeyValue(base, pairs + 16, t, f));
  CopyWord(base, out + 12, kOne);
}

// --- The course's route as a spline ---

void MatrixFromRows3(uint8_t* base, uint32_t out, uint32_t x, uint32_t y, uint32_t z) {
  ITC_NATIVE_RUNNING("sub_820AB5F0 Matrix::FromRows3");
  constexpr uint32_t one = 0x3F800000;
  Stvx(base, out + 0, VectorOf(one, 0, 0, 0));
  Stvx(base, out + 16, VectorOf(0, one, 0, 0));
  Stvx(base, out + 32, VectorOf(0, 0, one, 0));
  Stvx(base, out + 48, VectorOf(0, 0, 0, one));
  for (uint32_t i = 0; i < 4; i++) {
    CopyWord(base, out + 0 + 4 * i, x + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    CopyWord(base, out + 16 + 4 * i, y + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    CopyWord(base, out + 32 + 4 * i, z + 4 * i);
  }
}

void SplinePoint(uint8_t* base, uint32_t out, uint32_t matrix, uint32_t position, double t,
                 uint32_t sp, const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_820AB6A0 TrackRoute::SplinePoint");
  const uint32_t f = sp - 720;
  // The position into the caller's frame (the original's stw r5,756(r1)).
  Store(base, f + 756, position);
  const double one = Lfs(base, kOne);
  for (uint32_t i = 0; i < 4; i++) {
    CopyWord(base, f + 380 + 16 * i, kOne);
  }
  CopyWord(base, f + 204, kOne);
  CopyWord(base, f + 188, kOne);
  CopyWord(base, f + 444, kOne);
  CopyWord(base, f + 460, kOne);
  CopyWord(base, f + 316, kOne);
  CopyWord(base, f + 332, kOne);
  CopyWord(base, f + 348, kOne);
  CopyWord(base, f + 364, kOne);
  const int32_t before = LoadS16(base, f + 758) - 1;
  const uint32_t section = LoadU16(base, f + 756);
  CopyWord(base, f + 340, kOne);
  CopyWord(base, f + 348, kOne);
  CopyWord(base, f + 220, kOne);
  const double zero = Lfs(base, kZero);
  CopyWord(base, f + 336, kZero);
  StoreU16(base, f + 160, section);
  CopyWord(base, f + 344, kZero);
  StoreU16(base, f + 162, uint32_t(before));
  for (uint32_t i = 0; i < 6; i++) {
    StoreU16(base, f + 164 + 2 * i, 0xFFFF);
  }
  const uint32_t file = Load(base, kRoute);
  // The four points (f + 160: section and slice halfwords): the slice before
  // the position (over the section's first back link, 3 to 5, when it's the
  // section's first), the position and the two after it, forks taken at the
  // first link that isn't the point's section yet.
  if (before < 0) {
    const uint32_t s = SectionOf(base, Load(base, file + 12), section);
    for (uint32_t k = 3; k < 6; k++) {
      if (LoadU16(base, s + 28 + 4 * k) != 0xFFFF) {
        StoreU16(base, f + 160, LoadU16(base, s + (k + 7) * 4));
        StoreU16(base, f + 162, LoadU16(base, s + k * 4 + 30));
        break;
      }
    }
  }
  const uint32_t table = Load(base, file + 12);
  int32_t stored;
  do {
    int32_t current = LoadS16(base, f + 160);
    stored = 1;
    for (uint32_t e = f + 164; e != f + 176; e += 4) {
      const uint32_t prev_section = LoadU16(base, e - 4);
      const int32_t next = LoadS16(base, e - 2) + 1;
      const uint32_t prev_s = Load(base, table + (prev_section << 2));
      if (LoadS16(base, prev_s + 0) > next) {
        stored++;
        StoreU16(base, e + 2, uint32_t(next));
        StoreU16(base, e + 0, prev_section);
        continue;
      }
      const uint32_t s = SectionOf(base, table, uint32_t(current));
      int32_t links = 0;
      for (uint32_t i = 0; i < 3; i++) {
        if (LoadU16(base, s + 28 + 4 * i) != 0xFFFF) {
          links++;
        }
      }
      int32_t pick = -1;
      if (links == 1) {
        for (uint32_t i = 0; i < 3; i++) {
          if (LoadU16(base, s + 28 + 4 * i) != 0xFFFF) {
            pick = int32_t(i);
            break;
          }
        }
      } else if (links > 1) {
        for (uint32_t i = 0; i < 3; i++) {
          const int32_t link = LoadS16(base, s + 28 + 4 * i);
          if (link != -1 && LoadS16(base, e) != link) {
            pick = int32_t(i);
            break;
          }
        }
      }
      if (pick < 0) {
        continue;
      }
      const uint32_t link_section = LoadU16(base, s + (uint32_t(pick) + 7) * 4);
      const uint32_t link_slice = LoadU16(base, s + uint32_t(pick) * 4 + 30);
      current = int16_t(link_section);
      stored++;
      StoreU16(base, e + 2, link_slice);
      StoreU16(base, e + 0, link_section);
    }
  } while (stored < 4);
  // The points' centers (each slice's record +124) and the two edge points
  // (after the records: +124 + n x 84 and + n x 96, 12 bytes a slice) at the
  // second and third.
  const uint32_t table2 = Load(base, file + 12);
  auto section_at = [&](uint32_t e) { return Load(base, table2 + (LoadU16(base, e) << 2)); };
  const uint32_t s0 = section_at(f + 160), s1 = section_at(f + 164);
  const uint32_t s2 = section_at(f + 168), s3 = section_at(f + 172);
  const uint32_t sl0 = uint32_t(LoadS16(base, f + 162)), sl1 = uint32_t(LoadS16(base, f + 166));
  const uint32_t sl2 = uint32_t(LoadS16(base, f + 170)), sl3 = uint32_t(LoadS16(base, f + 174));
  const uint32_t p0 = s0 + sl0 * 84, p1 = s1 + sl1 * 84, p2 = s2 + sl2 * 84, p3 = s3 + sl3 * 84;
  const uint32_t n1 = uint32_t(LoadS16(base, s1 + 0));
  const uint32_t n2 = uint32_t(LoadS16(base, s2 + 0));
  const uint32_t e1a = s1 + n1 * 84 + sl1 * 12, e1b = s1 + n1 * 96 + sl1 * 12;
  const uint32_t e2a = s2 + n2 * 84 + sl2 * 12, e2b = s2 + n2 * 96 + sl2 * 12;
  const double f29 = Lfs(base, p0 + 124), f28 = Lfs(base, p0 + 128), f27 = Lfs(base, p0 + 132);
  const double f26 = Lfs(base, p3 + 124), f25 = Lfs(base, p3 + 128), f24 = Lfs(base, p3 + 132);
  const double f20 = Lfs(base, p2 + 124), f19 = Lfs(base, p2 + 128), f18 = Lfs(base, p2 + 132);
  const double f23 = Lfs(base, p1 + 124), f22 = Lfs(base, p1 + 128), f21 = Lfs(base, p1 + 132);
  const double f16 = Lfs(base, e1a + 124), f15 = Lfs(base, e1a + 128),
               f14 = Lfs(base, e1a + 132);
  const uint32_t guard = Load(base, kSplineGuard);
  for (uint32_t i = 0; i < 3; i++) {
    CopyWord(base, f + 304 + 4 * i, e1b + 124 + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    CopyWord(base, f + 448 + 4 * i, e2a + 124 + 4 * i);
  }
  for (uint32_t i = 0; i < 2; i++) {
    CopyWord(base, f + 320 + 4 * i, e2b + 124 + 4 * i);
  }
  CopyWord(base, f + 328, e2b + 132);
  CopyWord(base, f + 240, kFlip);
  if ((guard & 1) == 0) {
    // The Catmull-Rom matrix, made once from floats on the stack.
    CopyWord(base, f + 260, kZero);
    CopyWord(base, f + 248, kZero);
    CopyWord(base, f + 264, kZero);
    CopyWord(base, f + 272, kZero);
    CopyWord(base, f + 268, kOne);
    CopyWord(base, f + 256, 0x820021E8);
    CopyWord(base, f + 284, 0x820021E8);
    CopyWord(base, f + 276, 0x820021F8);
    CopyWord(base, f + 288, 0x82002828);
    CopyWord(base, f + 236, kZero);
    CopyWord(base, f + 280, kFlip);
    CopyWord(base, f + 244, kFlip);
    CopyWord(base, f + 252, 0x82002824);
    CopyWord(base, f + 224, kOne);
    CopyWord(base, f + 228, 0x820021E4);
    CopyWord(base, f + 232, kFlip);
    Store(base, f + 148, f + 260);
    Store(base, kSplineGuard, guard | 1);
    Store(base, f + 140, f + 248);
    Store(base, f + 132, f + 256);
    Store(base, f + 124, f + 264);
    Store(base, f + 116, f + 272);
    Store(base, f + 108, f + 268);
    Store(base, f + 100, f + 236);
    Store(base, f + 92, f + 280);
    Store(base, f + 84, f + 244);
    const uint32_t from[16] = {f + 232, f + 228, f + 288, f + 224, f + 284, f + 252,
                               f + 276, f + 244, f + 280, f + 236, f + 268, f + 272,
                               f + 264, f + 256, f + 248, f + 260};
    MatrixFromFloats(base, kSplineMatrix, from);
    c_runtime::AtexitFrom(base, kSplineAtexit, atexit);
  }
  // (t³, t², t, 1) through the basis and the four points.
  const double f31 = t;
  double f0 = Fmuls(f31, f31);
  Stfs(base, f + 168, f31);
  CopyWord(base, f + 172, kOne);
  for (uint32_t i = 0; i < 3; i++) {
    CopyWord(base, f + 368 + 4 * i, p0 + 124 + 4 * i);
  }
  CopyWord(base, f + 380, kOne);
  for (uint32_t i = 0; i < 3; i++) {
    CopyWord(base, f + 384 + 4 * i, p1 + 124 + 4 * i);
  }
  CopyWord(base, f + 396, kOne);
  for (uint32_t i = 0; i < 3; i++) {
    CopyWord(base, f + 400 + 4 * i, p2 + 124 + 4 * i);
  }
  CopyWord(base, f + 412, kOne);
  for (uint32_t i = 0; i < 3; i++) {
    CopyWord(base, f + 416 + 4 * i, p3 + 124 + 4 * i);
  }
  CopyWord(base, f + 428, kOne);
  Stfs(base, f + 160, Fmuls(f0, f31));
  Stfs(base, f + 164, f0);
  matrix_math::Vec4Transform(base, f + 160, kSplineMatrix, f + 160);
  matrix_math::Vec4Transform(base, f + 160, f + 368, f + 160);
  // The point, the tangent (+192: the chords P3 - P1 and P2 - P0 blended)
  // and the side (+176: the edge vectors at P1 and P2 blended).
  double f13 = Fsubs(f26, f23);
  double f6 = Lfs(base, f + 160);
  f0 = Fsubs(one, f31);
  double f12 = Fsubs(f20, f29);
  double f11 = Fsubs(f19, f28);
  double f9 = Fsubs(f25, f22);
  double f10 = Fsubs(f18, f27);
  const simde__m128 zeros = simde_mm_setzero_ps();
  simde__m128 v10 = simde_mm_set1_ps(0.5f);
  double f8 = Fsubs(f24, f21);
  double f7 = Fmuls(f13, f31);
  f13 = Lfs(base, 0x82001BE4);
  Stfs(base, out + 0, Fmuls(f6, f13));
  Stfs(base, out + 4, Fmuls(Lfs(base, f + 164), f13));
  Stfs(base, out + 8, Fmuls(Lfs(base, f + 168), f13));
  f13 = Fmuls(f12, f0);
  f12 = Fmuls(f11, f0);
  f9 = Fmuls(f9, f31);
  f11 = Fmuls(f10, f0);
  f10 = Lfs(base, f + 448);
  f8 = Fmuls(f8, f31);
  Stfs(base, f + 192, Fadds(f7, f13));
  Stfs(base, f + 196, Fadds(f9, f12));
  f9 = Lfs(base, f + 320);
  Stfs(base, f + 200, Fadds(f8, f11));
  f13 = Lfs(base, f + 304);
  f12 = Lfs(base, f + 308);
  f10 = Fsubs(f10, f9);
  f11 = Lfs(base, f + 312);
  f13 = Fsubs(f16, f13);
  f9 = Lfs(base, f + 452);
  f12 = Fsubs(f15, f12);
  f8 = Lfs(base, f + 324);
  f11 = Fsubs(f14, f11);
  simde__m128 v7 = LoadVector(base, f + 192);
  simde__m128 v0, v13, v9, v11, v6;
  {
    FlushToZero flush;
    v0 = Dot3(v7, v7);
    v13 = ReciprocalRoot(v0);
    v9 = simde_mm_mul_ps(v0, v10);
    v11 = simde_mm_cmpeq_ps(v0, zeros);
    v6 = simde_mm_mul_ps(v13, v13);
  }
  f9 = Fsubs(f9, f8);
  f7 = Lfs(base, f + 328);
  f8 = Lfs(base, f + 456);
  f13 = Fmuls(f13, f0);
  f8 = Fsubs(f8, f7);
  {
    FlushToZero flush;
    v10 = NegMulSub(v9, v6, v10);
  }
  f10 = Fmuls(f10, f31);
  v9 = simde_mm_set1_ps(0.5f);
  f12 = Fmuls(f12, f0);
  f0 = Fmuls(f11, f0);
  f9 = Fmuls(f9, f31);
  f8 = Fmuls(f8, f31);
  f13 = Fadds(f13, f10);
  Stfs(base, f + 176, f13);
  simde__m128 v5;
  {
    FlushToZero flush;
    v5 = simde_mm_add_ps(simde_mm_mul_ps(v13, v10), v13);
  }
  Stfs(base, f + 180, Fadds(f12, f9));
  Stfs(base, f + 184, Fadds(f0, f8));
  v6 = LoadVector(base, f + 176);
  {
    // Both normalized (a zero length left as is), the up vector (+208) their
    // cross product, its y at +336.
    FlushToZero flush;
    simde__m128 v12 = Dot3(v6, v6);
    v13 = ReciprocalRoot(v12);
    v10 = simde_mm_cmpeq_ps(v12, zeros);
    const simde__m128 v8 = simde_mm_mul_ps(v7, v5);
    const simde__m128 v4 = simde_mm_mul_ps(v12, v9);
    v7 = simde_mm_mul_ps(v13, v13);
    v11 = Select(v11, v8, v0);
    Stvx(base, f + 192, v11);
    v9 = NegMulSub(v4, v7, v9);
    v0 = simde_mm_add_ps(simde_mm_mul_ps(v13, v9), v13);
    v13 = simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v11), 0x78));
    v11 = simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v11), 0x9C));
    v0 = simde_mm_mul_ps(v6, v0);
    v10 = Select(v10, v0, v12);
    v0 = simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v10), 0x9C));
    Stvx(base, f + 176, v10);
    v12 = simde_mm_castsi128_ps(simde_mm_shuffle_epi32(simde_mm_castps_si128(v10), 0x78));
    v0 = simde_mm_mul_ps(v0, v13);
    v9 = LoadVector(base, f + 336);
    v0 = NegMulSub(v12, v11, v0);
    Stvx(base, f + 208, v0);
    v0 = Dot3(v0, v9);
    Stvx(base, f + 336, v0);
  }
  if (Lfs(base, f + 336) < zero) {
    // Upside down: the side and the up vector flipped.
    const double flip = Lfs(base, f + 240);
    for (const uint32_t at : {176u, 180u, 184u, 208u, 212u, 216u}) {
      Stfs(base, f + at, Fmuls(Lfs(base, f + at), flip));
    }
  }
  MatrixFromRows3(base, matrix, f + 192, f + 208, f + 176);
}

void RoutePoint(uint8_t* base, uint32_t out, uint32_t position, uint32_t offset, double t,
                uint32_t sp, const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_820ABDA8 CameraPath::RoutePoint");
  const uint32_t f = sp - 208;
  CopyWord(base, f + 124, kOne);
  CopyWord(base, f + 140, kOne);
  CopyWord(base, f + 156, kOne);
  CopyWord(base, f + 172, kOne);
  CopyWord(base, f + 108, kOne);
  track_route::ClampPosition(base, f + 80, position);
  SplinePoint(base, out, f + 112, Load(base, f + 80), t, f, atexit);
  matrix_math::Vec4Transform(base, f + 96, f + 112, offset);
  Stfs(base, out + 0, Fadds(Lfs(base, out + 0), Lfs(base, f + 96)));
  const double y = Fadds(Lfs(base, f + 100), Lfs(base, out + 4));
  const double out_z = Lfs(base, out + 8);
  const double z = Fadds(Lfs(base, f + 104), out_z);
  Stfs(base, out + 4, y);
  Stfs(base, out + 8, z);
}

// --- CameraPath::Evaluate ---

namespace {

// The eye or target (`to`) blended toward `placed` (the caller's frame)
// moved by `by` (a position, or none) by the weight: to = to (1 - w) + (by
// + placed) w, the sum kept at `placed`.
void BlendPlaced(uint8_t* base, uint32_t to, uint32_t placed, uint32_t transform, double one,
                 double weight) {
  const double keep = Fsubs(one, weight);
  double x = Fadds(Lfs(base, transform + 0), Lfs(base, placed + 0));
  double y = Fadds(Lfs(base, transform + 4), Lfs(base, placed + 4));
  double z = Fadds(Lfs(base, transform + 8), Lfs(base, placed + 8));
  Stfs(base, to + 0, Fmuls(Lfs(base, to + 0), keep));
  x = Fmuls(x, weight);
  Stfs(base, to + 4, Fmuls(Lfs(base, to + 4), keep));
  y = Fmuls(y, weight);
  Stfs(base, to + 8, Fmuls(Lfs(base, to + 8), keep));
  z = Fmuls(z, weight);
  const double to_x = Lfs(base, to + 0);
  Stfs(base, placed + 0, x);
  Stfs(base, placed + 4, y);
  Stfs(base, placed + 8, z);
  Stfs(base, to + 0, Fadds(to_x, x));
  Stfs(base, to + 4, Fadds(Lfs(base, to + 4), y));
  Stfs(base, to + 8, Fadds(Lfs(base, to + 8), z));
}

}  // namespace

void PathEvaluate(uint8_t* base, uint32_t key, uint32_t out, uint32_t range, uint32_t transform,
                  double t, uint32_t sp, const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_820AC248 CameraPath::Evaluate");
  const uint32_t f = sp - 272;
  double time = t;
  const double one = Lfs(base, kOne);
  CopyWord(base, f + 108, kOne);
  CopyWord(base, f + 156, kOne);
  const double zero = Lfs(base, kZero);
  if (time < zero) {
    time = zero;
  } else if (time > one) {
    time = one;
  }
  // A (kind, value) pair: 0 the float, 1 the curve at the time, else
  // `otherwise`.
  auto pair = [&](uint32_t at, double otherwise) {
    const int32_t kind = int32_t(Load(base, at + 0));
    if (kind == 0) {
      return Lfs(base, at + 4);
    }
    if (kind == 1) {
      return CurveEvaluate(base, Load(base, at + 4), time, f);
    }
    return otherwise;
  };
  Stfs(base, out + 64, Fmuls(pair(key + 8, zero), Lfs(base, 0x8200283C)));
  Stfs(base, out + 68, Fdivs(Lfs(base, 0x82002838), pair(key + 16, zero)));
  KeyVec3(base, key + 24, out + 16, time, f);
  KeyVec3(base, key + 120, out + 32, time, f);
  bool eye_moved = false;
  bool target_moved = false;
  // The eye and target moved by the transform (+0 its position, +16 its
  // matrix) by their weights (+72, +168).
  double weight = pair(key + 72, zero);
  if (weight != zero) {
    CopyWord(base, f + 124, kOne);
    KeyVec3(base, key + 48, f + 112, time, f);
    matrix_math::Vec3SwapXZ(base, f + 112);
    matrix_math::Vec4Transform(base, f + 112, transform + 16, f + 112);
    eye_moved = true;
    BlendPlaced(base, out + 16, f + 112, transform, one, weight);
  }
  weight = pair(key + 168, zero);
  if (weight != zero) {
    CopyWord(base, f + 140, kOne);
    KeyVec3(base, key + 144, f + 128, time, f);
    matrix_math::Vec3SwapXZ(base, f + 128);
    matrix_math::Vec4Transform(base, f + 128, transform + 16, f + 128);
    target_moved = true;
    BlendPlaced(base, out + 32, f + 128, transform, one, weight);
  }
  // Then placed on the course's route (+104 and +200 the weights, +80 and
  // +176 the offsets, +112 and +208 the distances along the range in
  // slices, the fraction the spline's).
  auto on_route = [&](uint32_t to, uint32_t offset_key, uint32_t distance_key, bool target) {
    KeyVec3(base, offset_key, f + 144, time, f);
    matrix_math::Vec3SwapXZ(base, f + 144);
    const double distance = pair(distance_key, zero);
    Store(base, f + 80, uint32_t(Fctiwz(distance)));
    const uint32_t whole = Load(base, f + 80);
    RangeAdvance(base, f + 88, range, int32_t(whole));
    const int64_t wide = int32_t(whole);
    Store(base, f + 80, uint32_t(uint64_t(wide) >> 32));
    Store(base, f + 84, uint32_t(wide));
    const double slices = Single(double(wide));
    RoutePoint(base, f + 96, Load(base, f + 88), f + 144, Fsubs(distance, slices), f, atexit);
    const double keep = Fsubs(one, weight);
    const double to_x = Lfs(base, to + 0);
    const double x = target ? Fmuls(weight, Lfs(base, f + 96)) : Fmuls(Lfs(base, f + 96), weight);
    const double y = Fmuls(Lfs(base, f + 100), weight);
    const double z = Fmuls(Lfs(base, f + 104), weight);
    if (!target) {
      Stfs(base, f + 96, x);
      Stfs(base, f + 100, y);
      Stfs(base, f + 104, z);
    }
    Stfs(base, to + 0, Fmuls(to_x, keep));
    Stfs(base, to + 4, Fmuls(Lfs(base, to + 4), keep));
    Stfs(base, to + 8, Fmuls(Lfs(base, to + 8), keep));
    Stfs(base, to + 0, Fadds(Lfs(base, to + 0), x));
    Stfs(base, to + 4, Fadds(Lfs(base, to + 4), y));
    Stfs(base, to + 8, Fadds(Lfs(base, to + 8), z));
  };
  weight = pair(key + 104, zero);
  if (weight != zero) {
    on_route(out + 16, key + 80, key + 112, false);
    eye_moved = true;
  }
  weight = pair(key + 200, zero);
  if (weight != zero) {
    on_route(out + 32, key + 176, key + 208, true);
    target_moved = true;
  }
  // Left in the path's own space: z flipped.
  const double flip = Lfs(base, kFlip);
  if (!eye_moved) {
    Stfs(base, out + 24, Fmuls(Lfs(base, out + 24), flip));
  }
  if (!target_moved) {
    Stfs(base, out + 40, Fmuls(Lfs(base, out + 40), flip));
  }
}


// --- The TV camera's update ---

namespace {

uint64_t Load64(uint8_t* base, uint32_t a) {
  return (uint64_t(Load(base, a)) << 32) | Load(base, a + 4);
}
void Store64(uint8_t* base, uint32_t a, uint64_t v) {
  Store(base, a, uint32_t(v >> 32));
  Store(base, a + 4, uint32_t(v));
}
void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t words) {
  for (uint32_t i = 0; i < words; i++) {
    CopyWord(base, to + 4 * i, from + 4 * i);
  }
}

// Whether a camera path's record (+0 and +4 the two ends of its slices, in
// either order) covers the slice `x`, as TvCamera::Step's two walks test it
// (unsigned compares).
bool Covers(uint8_t* base, uint32_t record, uint32_t x) {
  const uint32_t a = Load(base, record + 0);
  if (!(x < a)) {
    if (Load(base, record + 4) > x) {
      return true;
    }
    if (x > a) {
      return false;
    }
  }
  return Load(base, record + 4) < x;
}

// The first record from the buffer's +4 to its +8 (20 bytes apart; the
// first one tested whatever +8 holds) covering `x`, or 0.
uint32_t CoveringRecord(uint8_t* base, uint32_t buffer, uint32_t x) {
  uint32_t record = Load(base, buffer + 4);
  while (!Covers(base, record, x)) {
    record += 20;
    if (record > Load(base, buffer + 8)) {
      return 0;
    }
  }
  return record;
}

// The four path buffers (+43016, 28 bytes apart: +12 the index, +24 the
// state): those empty or loaded (0 or 3) whose index isn't `spot` asked
// for `index`.
void RequestFree(uint8_t* base, uint32_t camera, int32_t spot, uint32_t index, uint32_t back,
                 const TvStepCalls& calls) {
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t buffer = camera + 43016 + 28 * k;
    const int32_t state = int32_t(Load(base, buffer + 24));
    if (state == 0 || state == 3) {
      if (spot != int32_t(Load(base, buffer + 12))) {
        calls.request(buffer, index, back);
      }
    }
  }
}

}  // namespace

void TvStep(uint8_t* base, uint32_t camera, uint32_t frame, const TvStepCalls& calls,
            const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_821562F8 TvCamera::Step");
  const uint32_t f = frame;
  const uint32_t tracker = camera + 32;
  TargetTrack(base, tracker, Load(base, camera + 16), f - 256);
  // The track position at f + 80, the transform (the tracked point and the
  // turn) at f + 176.
  const uint32_t position = Load(base, camera + 260);
  const uint64_t point_high = Load64(base, tracker + 8);
  Store(base, f + 80, position);
  Store64(base, f + 176, Load64(base, tracker + 0));
  Store64(base, f + 184, point_high);
  if (!c_runtime::Memcpy(base, f + 192, camera + 96, 64)) {
    calls.copy(f + 192, camera + 96, 64);
  }
  // The ranges the position is on, listed (a 40-slot list at f + 256;
  // List40::'s layout, its data words at f + 428).
  const uint32_t list = f + 256;
  for (uint32_t i = 0; i < 40; i++) {
    StoreU16(base, list + 4 * i + 2, i);
    StoreU16(base, list + 4 * i + 0, i + 1);
  }
  uint32_t ranges = Load(base, camera + 452);
  StoreU16(base, f + 416, 0);
  StoreU16(base, f + 412, 0xFFFF);
  StoreU16(base, f + 422, 0xFFFF);
  StoreU16(base, f + 420, 0xFFFF);
  StoreU16(base, f + 418, 0xFFFF);
  StoreU16(base, f + 424, 0);
  uint32_t count = 0;
  uint32_t head = 0xFFFF;
  uint32_t i = 0;
  Store(base, f + 88, i);
  if (ranges > 0) {
    do {
      const track_zones::Position p{uint16_t(LoadU16(base, f + 80)),
                                    uint16_t(LoadU16(base, f + 82))};
      if (track_zones::RangeUpdate(base, camera + 456 + i * 1064, p) != 0) {
        List40Insert(base, list, Load(base, f + 88), uint32_t(int32_t(int16_t(count))));
        count = LoadU16(base, f + 424);
      }
      i++;
      ranges = Load(base, camera + 452);
      Store(base, f + 88, i);
    } while (i < ranges);
    head = LoadU16(base, f + 420);
  }
  const uint32_t listed = uint32_t(int32_t(int16_t(count)));
  if (listed == 0) {
    return;
  }
  auto node_at = [&](uint32_t j) {
    uint32_t node = uint32_t(int32_t(int16_t(head))) * 4 + list;
    for (uint32_t k = j; k != 0; k--) {
      node = uint32_t(LoadS16(base, node)) * 4 + list;
    }
    return node;
  };
  auto data_of = [&](uint32_t node) {
    return Load(base, uint32_t(LoadS16(base, node + 2)) * 4 + f + 428);
  };
  // The spot: the one kept (+360) when it's listed among more than one, else
  // the first listed.
  bool kept = false;
  int32_t spot = 0;
  if (listed > 1) {
    const uint32_t current = Load(base, camera + 360);
    for (uint32_t j = 0; j < listed; j++) {
      if (current == data_of(node_at(j))) {
        spot = int32_t(current);
        kept = spot != -1;
        break;
      }
    }
  }
  if (!kept) {
    spot = int32_t(data_of(node_at(0)));
  }
  Store(base, camera + 360, uint32_t(spot));
  const uint32_t range = camera + 456 + uint32_t(spot) * 1064;
  const uint32_t slice = uint32_t(RangeCameraAt(base, range, f + 80));
  // The path buffer holding the spot's path.
  uint32_t buffer = 0;
  bool held = false;
  for (uint32_t k = 0; k < 4; k++) {
    if (int32_t(Load(base, camera + 43028 + 28 * k)) == spot) {
      buffer = camera + 28 * k + 43016;
      held = true;
      break;
    }
  }
  if (!held || buffer == 0) {
    for (uint32_t k = 0; k < 4; k++) {
      const uint32_t b = camera + 43016 + 28 * k;
      const int32_t state = int32_t(Load(base, b + 24));
      if (state == 0 || state == 3) {
        if (Load(base, camera + 360) != Load(base, b + 12)) {
          calls.request(b, uint32_t(spot), 0x82156544);
        }
      }
    }
    return;
  }
  if (int32_t(Load(base, buffer + 24)) != 3) {
    return;
  }
  const uint32_t record = CoveringRecord(base, buffer, slice);
  if (record == 0) {
    return;
  }
  // t: the slice's fraction plus its place between the record's ends, over
  // their span, clamped to 0-1.
  const uint32_t a = Load(base, record + 0);
  const uint32_t b = Load(base, record + 4);
  uint32_t low, high;
  if (b > a) {
    low = a;
    high = b;
  } else {
    low = b;
    high = a;
  }
  const int64_t span_words = int64_t(int32_t(high - low));
  Store64(base, f + 88, uint64_t(span_words));
  const double span = Single(double(span_words));
  const double fraction = SliceFraction(base, f + 80, tracker, f);
  const uint64_t past_words = uint64_t(slice - low);
  const double along = Fdivs(fraction, span);
  Store64(base, f + 88, past_words);
  double past = Single(double(int64_t(past_words)));
  const double zero = Lfs(base, kZero);
  past = Fdivs(past, span);
  const double one = Lfs(base, kOne);
  double t = Fadds(along, past);
  if (t < zero) {
    t = zero;
  } else if (t > one) {
    t = one;
  }
  // The path's state (+43520): 0 to 1; 1 to 2 near the far end; 2 to 0 back
  // at the near end.
  const uint32_t state_at = camera + 43520;
  const uint32_t state = Load(base, state_at);
  if (state < 1) {
    Store(base, state_at, 1);
  } else if (state == 1) {
    if (!(int32_t(high - 1) > int32_t(slice))) {
      Store(base, state_at, 2);
    }
  } else if (state < 3) {
    if (!(int32_t(low) < int32_t(slice))) {
      Store(base, state_at, 0);
    }
  }
  PathEvaluate(base, Load(base, record + 16), camera + 368, range, f + 176, t, f, atexit);
  cameras::ViewportSetZoom(base, Load(base, camera + 20), Lfs(base, camera + 436));
  {
    const uint32_t viewport = Load(base, camera + 20);
    const double zoom = Lfs(base, camera + 436);
    const double ratio = Fdivs(Lfs(base, viewport + 1868), Lfs(base, viewport + 1864));
    cameras::SetNear(base, Fmuls(ratio, zoom));
  }
  cameras::ViewportSetRoll(base, Load(base, camera + 20), Lfs(base, camera + 432));
  CopyWords(base, Load(base, camera + 20) + 1600, camera + 384, 4);
  CopyWords(base, Load(base, camera + 20) + 1616, camera + 400, 4);
  {
    const uint32_t viewport = Load(base, camera + 20);
    CopyWords(base, viewport + 1632, viewport + 1712, 4);
  }
  CopyWords(base, Load(base, camera + 20) + 1648, tracker, 4);
  // The slice changed since +352: the next record's (+356 past this
  // record's end, the way the slice moved).
  const int32_t last = int32_t(Load(base, camera + 352));
  if (int32_t(slice) == last) {
    return;
  }
  if (int32_t(slice) > last) {
    uint32_t end = Load(base, record + 0);
    const uint32_t other = Load(base, record + 4);
    if (!(end > other)) {
      end = other;
    }
    Store(base, camera + 356, end + 1);
  } else if (int32_t(slice) < last) {
    uint32_t end = Load(base, record + 0);
    const uint32_t other = Load(base, record + 4);
    if (!(end < other)) {
      end = other;
    }
    Store(base, camera + 356, end - 1);
  }
  const uint32_t next = CoveringRecord(base, buffer, Load(base, camera + 356));
  if (next != 0) {
    Stfs(base, f + 124, one);
    Stfs(base, f + 140, one);
    Stfs(base, f + 156, one);
    const double at = next < record ? one : zero;
    PathEvaluate(base, Load(base, next + 16), f + 96, range, f + 176, at, f, atexit);
    CopyWords(base, Load(base, camera + 20) + 1840, f + 112, 4);
    Store(base, camera + 352, slice);
    return;
  }
  // None: the position's section's twelve links, each on a range a path
  // buffer doesn't hold asked for.
  const uint32_t table = Load(base, Load(base, kRoute) + 12);
  const uint32_t section = Load(base, table + (LoadU16(base, f + 80) << 2));
  uint32_t link = section + 30;
  for (uint32_t n = 12; n != 0; n--, link += 4) {
    const uint32_t to = LoadU16(base, link - 2);
    int32_t pick = -1;
    const uint32_t to_slice = LoadU16(base, link);
    StoreU16(base, f + 88, to);
    StoreU16(base, f + 90, to_slice);
    if (int16_t(to) == -1) {
      continue;
    }
    uint32_t r = 0;
    if (Load(base, camera + 452) > 0) {
      uint32_t at = camera + 456;
      do {
        const track_zones::Position p{uint16_t(LoadU16(base, f + 88)),
                                      uint16_t(LoadU16(base, f + 90))};
        if (track_zones::RangeTest(base, at, p) == 2) {
          pick = int32_t(r);
          break;
        }
        r++;
        at += 1064;
      } while (r < Load(base, camera + 452));
    }
    bool has = false;
    for (uint32_t k = 0; k < 4; k++) {
      if (pick == int32_t(Load(base, camera + 43028 + 28 * k))) {
        has = true;
        break;
      }
    }
    if (has) {
      continue;
    }
    RequestFree(base, camera, spot, uint32_t(pick), 0x821568D8, calls);
  }
  CopyWords(base, Load(base, camera + 20) + 1840, camera + 384, 4);
  Store(base, camera + 352, slice);
}

void TvUpdate(uint8_t* base, uint32_t camera, uint32_t frame, const TvUpdateCalls& calls,
              const c_runtime::AtexitCall& atexit) {
  ITC_NATIVE_RUNNING("sub_821593D8 TvCamera::Update");
  constexpr uint32_t kStyle = 0x826D82C8;
  constexpr uint32_t kGuard = 0x826D82CC;
  const uint32_t chase = camera + 43136;
  const uint32_t update = Load(base, Load(base, chase));
  if (update == 0x82154808) {
    ChaseUpdate(base, chase, frame - 144, calls.video_output);
  } else {
    calls.slot0(update, chase);
  }
  if (Load(base, camera + 448) == 0) {
    TvStep(base, camera, frame - 720, calls.step, atexit);
  }
  // The attract race's color filter: none once, then while +43520 is 0 one
  // of the four styles by rand.
  const uint32_t guard = Load(base, kGuard);
  if ((guard & 1) == 0) {
    Store(base, kGuard, guard | 1);
    Store(base, kStyle, 0);
  }
  if (Load(base, camera + 43520) == 0) {
    Store(base, frame + 80, 0);
    Store(base, frame + 84, 1);
    Store(base, frame + 88, 2);
    Store(base, frame + 92, 3);
    const int32_t drawn = int32_t(race_overlays::Rand(base));
    const int32_t pick = drawn % 4;
    Store(base, kStyle, Load(base, frame + 80 + (uint32_t(pick) << 2)));
  }
  if (!ColorFilterSetStyle(base, kStyle)) {
    calls.set_style_original(kStyle);
  }
}

}  // namespace camera_modes
