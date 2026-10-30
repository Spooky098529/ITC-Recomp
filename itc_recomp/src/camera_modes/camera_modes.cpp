// The camera modes' small pieces (docs/functions/camera-modes.md): the
// director's handlers, the lists, the zoom, the quaternion steps, the
// camera animations' table, the color filter and the fade cues. Store for
// store as the recompiled functions.

#include "camera_modes/camera_modes.h"

#include "camera_modes/steps.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "cameras/cameras.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "post_effects/post_effects.h"
#include "track_zones/track_zones.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace camera_modes {

using namespace steps;

namespace {

constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kHalf = 0x82001BE4;  // 0.5
constexpr uint32_t kTwo = 0x820021E8;   // 2.0
constexpr uint32_t kTwoPi = 0x82001FA8;
constexpr uint32_t kOverTwoPi = 0x82001FA4;
constexpr uint32_t kRadiansPerDegree = 0x82002948;

constexpr uint32_t kViewMode = 1860;  // a viewport's view mode

int32_t LoadS32(uint8_t* base, uint32_t a) { return int32_t(guest::LoadU32(base, a)); }
int32_t LoadS16(uint8_t* base, uint32_t a) { return int16_t(guest::LoadU16(base, a)); }

uint32_t Viewport0(uint8_t* base) {
  return guest::LoadU32(base, guest::LoadU32(base, kViewports));
}

// The node `steps` steps from a list's head (`head` its offset), as the
// originals walk it: (index << 2) + list, 32 bits.
uint32_t Walk(uint8_t* base, uint32_t list, uint32_t head, uint32_t steps) {
  uint32_t node = (uint32_t(LoadS16(base, list + head)) << 2) + list;
  for (uint32_t n = steps; n != 0; n--) {
    node = (uint32_t(LoadS16(base, node)) << 2) + list;
  }
  return node;
}

}  // namespace

// --- The director's handlers ---

uint32_t ToView4(uint8_t* base, uint32_t director) {
  ITC_NATIVE_RUNNING("sub_82152400 CameraDirector::ToView4");
  guest::StoreU32(base, Viewport0(base) + kViewMode, 4);
  const uint32_t viewports = guest::LoadU32(base, kViewports);
  uint32_t r3 = director;
  if (guest::LoadU32(base, viewports + 8) != 0) {
    guest::StoreU32(base, viewports + 8, 0);
    const uint32_t record = vehicle_accessors::RacerRecordsAt(
        base, guest::LoadU32(base, kRaceData) + 0x59AE8, 0xFFFFFFFFu);
    const int32_t kind = LoadS32(base, record + 3924);
    guest::StoreU32(base, director + kFollowed, kind == 2 ? 1 : 0);
    guest::StoreU32(base, director + kFollowedInput, 0);
    r3 = record;
  }
  guest::StoreU32(base, director + kViewKept, guest::LoadU32(base, director + kView));
  return r3;
}

void ToView5(uint8_t* base, uint32_t director) {
  ITC_NATIVE_RUNNING("sub_821524B8 CameraDirector::ToView5");
  guest::StoreU32(base, Viewport0(base) + kViewMode, 5);
  guest::StoreU32(base, director + kViewKept, guest::LoadU32(base, director + kView));
}

void ToView3(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821524D8 CameraDirector::ToView3");
  guest::StoreU32(base, Viewport0(base) + kViewMode, 3);
}

void ToView3IfCourse(uint8_t* base, uint32_t director) {
  ITC_NATIVE_RUNNING("sub_82153160 CameraDirector::ToView3IfCourse");
  const uint32_t race = guest::LoadU32(base, kRaceData);
  if (LoadS32(base, race + 0xA3AF0) != 1) {
    return;
  }
  const int32_t course = LoadS32(base, race + 4);
  if (course == 8 || course == 9 || course == 10) {
    return;
  }
  guest::StoreU32(base, Viewport0(base) + kViewMode, 3);
  const uint32_t view = guest::LoadU32(base, director + kView);
  if (int32_t(view) == 3) {
    return;
  }
  guest::StoreU32(base, director + kViewKept, view);
}

// --- Small helpers ---

void ResetRoll(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821524F0 Viewports::ResetRoll");
  const double zero = Lfs(base, kZero);
  cameras::ViewportSetRoll(base, guest::LoadU32(base, guest::LoadU32(base, kViewports)), zero);
  cameras::ViewportSetRoll(base, guest::LoadU32(base, guest::LoadU32(base, kViewports) + 4),
                           zero);
}

std::optional<double> ZoomFromFov(uint8_t* base, double degrees, double width) {
  ITC_NATIVE_RUNNING("sub_82152548 Camera::ZoomFromFov");
  const double half = Lfs(base, kHalf);
  const double two_pi = Lfs(base, kTwoPi);
  const double x = Fmuls(Fmuls(degrees, half), Lfs(base, kRadiansPerDegree));
  // pi with the angle's sign (its stored float's sign bit kept, the rest
  // 0x40490FDB inserted), added, times 1 / (2 pi), truncated, that many 2 pi
  // taken off.
  const double pi = double(std::bit_cast<float>((Bits(x) & 0x80000000u) | 0x40490FDBu));
  const double turns = Fmuls(Fadds(pi, x), Lfs(base, kOverTwoPi));
  const double n = Single(double(int64_t(Fctiwz(turns))));
  const double wrapped = Fnmsubs(n, two_pi, x);
  // tan (sub_825A8A50) as its adapter gives it with nearest rounding set.
  double t;
  if ((Fpscr::getcsr() & Fpscr::RoundMaskVal) != 0 || !c_runtime::Tan(wrapped, &t)) {
    return std::nullopt;
  }
  return Fmuls(Fmuls(Single(t), width), half);
}

void QuatRotate(uint8_t* base, uint32_t out, uint32_t q, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_821525F8 Quat::Rotate");
  const double qx = Lfs(base, q + 0), qy = Lfs(base, q + 4), qz = Lfs(base, q + 8);
  const double vz = Lfs(base, v + 8), vy = Lfs(base, v + 4);
  double f3 = Fmuls(vz, qx);
  const double vx = Lfs(base, v + 0);
  double f5 = Fmuls(vy, qy);
  const double qw = Lfs(base, q + 12);
  double f4 = Fmuls(vx, qy);
  const double one = Lfs(base, kOne);
  const double f2 = Fmuls(vy, qz);
  Stfs(base, out + 12, one);
  double f7 = Fmuls(qx, qx);
  double f6 = Lfs(base, kTwo);
  f3 = Fmsubs(vx, qz, f3);
  f5 = Fmadds(vz, qz, f5);
  f4 = Fmsubs(vy, qx, f4);
  f7 = Fmsubs(qw, qw, f7);
  const double f8 = Fmuls(qw, f6);
  f5 = Fmadds(vx, qx, f5);
  f7 = Fnmsubs(qy, qy, f7);
  f6 = Fmuls(f5, f6);
  f5 = Fmsubs(vz, qy, f2);
  f7 = Fnmsubs(qz, qz, f7);
  double f11 = Fmuls(vz, f7);
  double f10 = Fmuls(vy, f7);
  double f9 = Fmuls(vx, f7);
  f11 = Fmadds(f4, f8, f11);
  f10 = Fmadds(f3, f8, f10);
  f9 = Fmadds(f5, f8, f9);
  Stfs(base, out + 8, Fmadds(qz, f6, f11));
  Stfs(base, out + 4, Fmadds(qy, f6, f10));
  Stfs(base, out + 0, Fmadds(qx, f6, f9));
}

void MatrixFromQuat(uint8_t* base, uint32_t out, const uint32_t q[4], uint32_t scratch) {
  ITC_NATIVE_RUNNING("sub_82152698 Matrix::FromQuat");
  const double y = double(std::bit_cast<float>(q[1]));
  const double z = double(std::bit_cast<float>(q[2]));
  const double x = double(std::bit_cast<float>(q[0]));
  const double w = double(std::bit_cast<float>(q[3]));
  simde__m128 dot;
  {
    FlushToZero ftz;
    const simde__m128 v = VectorOf(q[0], q[1], q[2], q[3]);
    dot = simde_mm_dp_ps(v, v, 0xFF);
  }
  const double zero = Lfs(base, kZero);
  Stfs(base, out + 12, zero);
  Stfs(base, out + 28, zero);
  Stfs(base, out + 44, zero);
  Stfs(base, out + 48, zero);
  const double one = Lfs(base, kOne);
  Stfs(base, out + 52, zero);
  Stfs(base, out + 56, zero);
  Stfs(base, out + 60, one);
  Stvx(base, scratch, dot);
  const double s = Fdivs(Lfs(base, kTwo), Lfs(base, scratch));
  const double zs = Fmuls(z, s);
  const double ys = Fmuls(y, s);
  const double xs = Fmuls(x, s);
  const double zz = Fmuls(z, zs);
  const double yy = Fmuls(y, ys);
  const double xx = Fmuls(x, xs);
  const double wy = Fmuls(w, ys);
  const double wx = Fmuls(w, xs);
  const double xy = Fmuls(ys, x);
  const double wz = Fmuls(w, zs);
  const double xz = Fmuls(zs, x);
  const double yz = Fmuls(zs, y);
  const double f9 = Fadds(zz, yy);
  const double f8 = Fadds(zz, xx);
  const double f7 = Fadds(yy, xx);
  Stfs(base, out + 4, Fadds(xy, wz));
  Stfs(base, out + 16, Fsubs(xy, wz));
  Stfs(base, out + 24, Fadds(yz, wx));
  Stfs(base, out + 32, Fadds(xz, wy));
  Stfs(base, out + 36, Fsubs(yz, wx));
  Stfs(base, out + 0, Fsubs(one, f9));
  Stfs(base, out + 8, Fsubs(xz, wy));
  Stfs(base, out + 20, Fsubs(one, f8));
  Stfs(base, out + 40, Fsubs(one, f7));
}

void MatrixFromFloats(uint8_t* base, uint32_t out, const uint32_t from[16]) {
  ITC_NATIVE_RUNNING("sub_82090000 Matrix::FromFloats");
  const uint32_t one = guest::LoadU32(base, kOne);
  guest::StoreU32(base, out + 12, one);
  guest::StoreU32(base, out + 28, one);
  guest::StoreU32(base, out + 44, one);
  guest::StoreU32(base, out + 60, one);
  // lfs then stfs: the bits as the hardware keeps them (the translated
  // code's conversions there are folded away).
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, out + 4 * i, guest::LoadU32(base, from[i]));
  }
}

// --- The lists ---

void List40Link(uint8_t* base, uint32_t list, int16_t position) {
  ITC_NATIVE_RUNNING("sub_82153228 List40::Link");
  uint32_t after;
  if (position == 0) {
    after = list + 164;
  } else {
    uint32_t node = uint32_t(LoadS16(base, list + 164));
    int32_t n = position;
    for (;;) {
      node = (node << 2) + list;
      n--;
      if (n == 0) {
        break;
      }
      node = uint32_t(LoadS16(base, node));
    }
    after = node;
  }
  const uint32_t free_node = (uint32_t(LoadS16(base, list + 160)) << 2) + list;
  const uint16_t free_index = guest::LoadU16(base, list + 160);
  guest::StoreU16(base, list + 160, guest::LoadU16(base, free_node));
  guest::StoreU16(base, free_node, free_index);
  guest::StoreU16(base, free_node, guest::LoadU16(base, after));
  guest::StoreU16(base, after, free_index);
  guest::StoreU16(base, list + 168, uint16_t(guest::LoadU16(base, list + 168) + 1));
}

void List40Insert(uint8_t* base, uint32_t list, uint32_t word, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_821544E0 List40::Insert");
  List40Link(base, list, int16_t(uint16_t(position)));
  const uint32_t node = Walk(base, list, 164, position);
  const uint32_t slot = uint32_t(LoadS16(base, node + 2));
  guest::StoreU32(base, ((slot + 43) << 2) + list, word);
}

void List16Insert20(uint8_t* base, uint32_t list, uint32_t record, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_82154458 List16::Insert20");
  track_zones::List16Link(base, list, int16_t(uint16_t(position)));
  const uint32_t node = Walk(base, list, 68, position);
  uint32_t to = uint32_t(int64_t(LoadS16(base, node + 2)) * 20) + list + 76;
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, to + 4 * i, guest::LoadU32(base, record + 4 * i));
  }
}

bool List16Insert(uint8_t* base, uint32_t list, uint32_t record, uint32_t position,
                  uint32_t size, uint32_t* copy_to) {
  if (size == 32) {
    ITC_NATIVE_RUNNING("sub_82154378 List16::Insert32");
  } else {
    ITC_NATIVE_RUNNING("sub_821543E8 List16::Insert608");
  }
  track_zones::List16Link(base, list, int16_t(uint16_t(position)));
  const uint32_t node = Walk(base, list, 68, position);
  *copy_to = uint32_t(int64_t(LoadS16(base, node + 2)) * int64_t(size)) + list + 76;
  return c_runtime::Memcpy(base, *copy_to, record, size);
}

bool List16Copy32(uint8_t* base, uint32_t out, uint32_t list, uint32_t position,
                  uint32_t* copy_from) {
  ITC_NATIVE_RUNNING("sub_82154300 List16::Copy32");
  const uint32_t node = Walk(base, list, 68, position);
  *copy_from = (uint32_t(LoadS16(base, node + 2)) << 5) + list + 76;
  return c_runtime::Memcpy(base, out, *copy_from, 32);
}

// --- The camera animations' table ---

void AnimsClear(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82150078 CameraAnims::Clear");
  guest::StoreU32(base, kAnimCount, 0);
}

uint32_t AnimsFind(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82150300 CameraAnims::Find");
  const uint32_t count = guest::LoadU32(base, kAnimCount);
  uint32_t entry = kAnims;
  for (uint32_t i = 0; i < count; i++, entry += kAnimBytes) {
    if (guest::LoadU32(base, entry) == id) {
      return entry + 4;
    }
  }
  return 0;
}

// --- The color filter and the fade cues ---

bool ColorFilterSetStyle(uint8_t* base, uint32_t style) {
  const uint32_t filter = post_effects::Get(base, 2);
  if (filter != 0) {
    // Its slots 4, 6 and 7: ColorFilter::SetColor0, Set8112 and Set8116.
    const uint32_t vtable = guest::LoadU32(base, filter);
    if (guest::LoadU32(base, vtable + 16) != 0x821350F8 ||
        guest::LoadU32(base, vtable + 24) != 0x82135138 ||
        guest::LoadU32(base, vtable + 28) != 0x82135140) {
      return false;
    }
  }
  ITC_NATIVE_RUNNING("sub_82152108 ColorFilter::SetStyle");
  if (filter == 0) {
    return true;
  }
  const uint32_t kind = guest::LoadU32(base, style);
  double r, g, b, last;
  switch (kind) {
    case 0:
      last = Lfs(base, kZero);
      post_effects::ColorFilterSet(base, filter, 8112, last);
      r = g = b = Lfs(base, kOne);
      break;
    case 1:
      post_effects::ColorFilterSet(base, filter, 8112, Lfs(base, 0x82002D14));
      last = Lfs(base, kOne);
      r = g = b = last;
      break;
    case 2: {
      post_effects::ColorFilterSet(base, filter, 8112, Lfs(base, 0x82002BF8));
      const double c = Lfs(base, 0x820027A8);
      post_effects::ColorFilterSetColor0(base, filter, c, c, c);
      post_effects::ColorFilterSet(base, filter, 8116, Lfs(base, kZero));
      return true;
    }
    case 3:
      post_effects::ColorFilterSet(base, filter, 8112, Lfs(base, 0x82009398));
      last = Lfs(base, kHalf);
      b = last;
      g = Lfs(base, 0x82009394);
      r = Lfs(base, 0x820029F0);
      break;
    default:
      return true;
  }
  post_effects::ColorFilterSetColor0(base, filter, r, g, b);
  post_effects::ColorFilterSet(base, filter, 8116, last);
  return true;
}

void FadeCueStep(uint8_t* base, uint32_t cue) {
  ITC_NATIVE_RUNNING("sub_82152290 FadeCue::Step");
  const int32_t start = LoadS32(base, cue + 0);
  if (start == LoadS32(base, cue + 24)) {
    const uint32_t kind = guest::LoadU32(base, cue + 8);
    if (kind <= 1) {
      const uint32_t passes = guest::LoadU32(base, cue + 4) - uint32_t(start);
      const uint32_t red = guest::LoadU32(base, cue + 12);
      const uint32_t green = guest::LoadU32(base, cue + 16);
      const uint32_t blue = guest::LoadU32(base, cue + 20);
      if (kind == 0) {
        mode_tick::ScreenFadesFadeIn(base, passes, red, green, blue, 0);
      } else {
        mode_tick::ScreenFadesFadeOut(base, passes, red, green, blue, 0);
      }
    }
  }
  if (LoadS32(base, cue + 28) == 0) {
    const int32_t frame = LoadS32(base, cue + 24);
    if (LoadS32(base, cue + 4) > frame) {
      guest::StoreU32(base, cue + 24, uint32_t(frame + 1));
    }
  }
}

}  // namespace camera_modes
