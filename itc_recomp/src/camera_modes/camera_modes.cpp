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
#include "schema/camera_modes.h"
#include "schema/race_hud_machine.h"
#include "schema/race_states.h"

namespace camera_modes {

using namespace steps;

using G = schema::CameraModesGlobals;
using Director = schema::CameraDirector;

namespace {

constexpr uint32_t kOne = G::one_address();    // 1.0
constexpr uint32_t kZero = G::zero_address();  // 0.0
constexpr uint32_t kHalf = G::half_address();  // 0.5
constexpr uint32_t kTwo = G::two_address();    // 2.0
constexpr uint32_t kTwoPi = G::two_pi_address();
constexpr uint32_t kOverTwoPi = G::over_two_pi_address();
constexpr uint32_t kRadiansPerDegree = G::radians_per_degree_address();

int32_t LoadS16(uint8_t* base, uint32_t a) { return int16_t(guest::LoadU16(base, a)); }

schema::Viewport Viewport0(uint8_t* base) { return G::viewports(base).first(); }

// The node `steps` steps from a list's head (`head` its address), as the
// originals walk it: (index << 2) + list, 32 bits.
uint32_t Walk(uint8_t* base, uint32_t list, uint32_t head, uint32_t steps) {
  uint32_t node = (uint32_t(LoadS16(base, head)) << 2) + list;
  for (uint32_t n = steps; n != 0; n--) {
    node = (uint32_t(LoadS16(base, node)) << 2) + list;
  }
  return node;
}

}  // namespace

// --- The director's handlers ---

uint32_t ToView4(uint8_t* base, uint32_t director) {
  ITC_NATIVE_RUNNING("sub_82152400 CameraDirector::ToView4");
  Viewport0(base).set_view_mode(4);
  const schema::Viewports viewports = G::viewports(base);
  const Director d(base, director);
  uint32_t r3 = director;
  if (viewports.split() != 0) {
    viewports.set_split(0);
    const uint32_t record =
        vehicle_accessors::RacerRecordsAt(
            base, schema::RaceData(base, G::race_data(base)).results_address(), 0xFFFFFFFFu);
    const int32_t kind = int32_t(schema::RacerResultsRecord(base, record).finish());
    d.set_followed(kind == 2 ? 1 : 0);
    d.set_followed_input(0);
    r3 = record;
  }
  d.set_view_kept(d.view());
  return r3;
}

void ToView5(uint8_t* base, uint32_t director) {
  ITC_NATIVE_RUNNING("sub_821524B8 CameraDirector::ToView5");
  Viewport0(base).set_view_mode(5);
  const Director d(base, director);
  d.set_view_kept(d.view());
}

void ToView3(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821524D8 CameraDirector::ToView3");
  Viewport0(base).set_view_mode(3);
}

void ToView3IfCourse(uint8_t* base, uint32_t director) {
  ITC_NATIVE_RUNNING("sub_82153160 CameraDirector::ToView3IfCourse");
  const uint32_t race = G::race_data(base);
  if (int32_t(schema::RaceData(base, race).kind()) != 1) {
    return;
  }
  const int32_t course = int32_t(schema::RaceData(base, race).route_course());
  if (course == 8 || course == 9 || course == 10) {
    return;
  }
  Viewport0(base).set_view_mode(3);
  const Director d(base, director);
  const uint32_t view = d.view();
  if (int32_t(view) == 3) {
    return;
  }
  d.set_view_kept(view);
}

// --- Small helpers ---

void ResetRoll(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821524F0 Viewports::ResetRoll");
  const double zero = Lfs(base, kZero);
  cameras::ViewportSetRoll(base, G::viewports(base).first().address, zero);
  cameras::ViewportSetRoll(base, G::viewports(base).second().address, zero);
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
  const schema::CameraQuat quat(base, q);
  const schema::CameraVec3 vec(base, v);
  const schema::CameraVec4 to(base, out);
  const double qx = quat.x(), qy = quat.y(), qz = quat.z();
  const double vz = vec.z(), vy = vec.y();
  double f3 = Fmuls(vz, qx);
  const double vx = vec.x();
  double f5 = Fmuls(vy, qy);
  const double qw = quat.w();
  double f4 = Fmuls(vx, qy);
  const double one = Lfs(base, kOne);
  const double f2 = Fmuls(vy, qz);
  to.set_w(float(one));
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
  to.set_z(float(Fmadds(qz, f6, f11)));
  to.set_y(float(Fmadds(qy, f6, f10)));
  to.set_x(float(Fmadds(qx, f6, f9)));
}

void MatrixFromQuat(uint8_t* base, uint32_t out, const uint32_t q[4], uint32_t scratch) {
  ITC_NATIVE_RUNNING("sub_82152698 Matrix::FromQuat");
  const double y = double(std::bit_cast<float>(q[1]));
  const double z = double(std::bit_cast<float>(q[2]));
  const double x = double(std::bit_cast<float>(q[0]));
  const double w = double(std::bit_cast<float>(q[3]));
  const schema::CameraMatrix m(base, out);
  simde__m128 dot;
  {
    FlushToZero ftz;
    const simde__m128 v = VectorOf(q[0], q[1], q[2], q[3]);
    dot = simde_mm_dp_ps(v, v, 0xFF);
  }
  const double zero = Lfs(base, kZero);
  m.set_m03(float(zero));
  m.set_m13(float(zero));
  m.set_m23(float(zero));
  m.set_m30(float(zero));
  const double one = Lfs(base, kOne);
  m.set_m31(float(zero));
  m.set_m32(float(zero));
  m.set_m33(float(one));
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
  m.set_m01(float(Fadds(xy, wz)));
  m.set_m10(float(Fsubs(xy, wz)));
  m.set_m12(float(Fadds(yz, wx)));
  m.set_m20(float(Fadds(xz, wy)));
  m.set_m21(float(Fsubs(yz, wx)));
  m.set_m00(float(Fsubs(one, f9)));
  m.set_m02(float(Fsubs(xz, wy)));
  m.set_m11(float(Fsubs(one, f8)));
  m.set_m22(float(Fsubs(one, f7)));
}

void MatrixFromFloats(uint8_t* base, uint32_t out, const uint32_t from[16]) {
  ITC_NATIVE_RUNNING("sub_82090000 Matrix::FromFloats");
  const uint32_t one = guest::LoadU32(base, kOne);
  const schema::CameraMatrix m(base, out);
  guest::StoreU32(base, m.m03_address(), one);
  guest::StoreU32(base, m.m13_address(), one);
  guest::StoreU32(base, m.m23_address(), one);
  guest::StoreU32(base, m.m33_address(), one);
  // lfs then stfs: the bits as the hardware keeps them (the translated
  // code's conversions there are folded away).
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, m.address + 4 * i, guest::LoadU32(base, from[i]));
  }
}

// --- The lists ---

void List40Link(uint8_t* base, uint32_t list, int16_t position) {
  ITC_NATIVE_RUNNING("sub_82153228 List40::Link");
  const schema::List40 l(base, list);
  uint32_t after;
  if (position == 0) {
    after = l.head_address();
  } else {
    uint32_t node = uint32_t(int16_t(l.head()));
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
  const uint32_t free_node = (uint32_t(int16_t(l.free())) << 2) + list;
  const uint16_t free_index = l.free();
  l.set_free(guest::LoadU16(base, free_node));
  guest::StoreU16(base, free_node, free_index);
  guest::StoreU16(base, free_node, guest::LoadU16(base, after));
  guest::StoreU16(base, after, free_index);
  l.set_count(uint16_t(l.count() + 1));
}

void List40Insert(uint8_t* base, uint32_t list, uint32_t word, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_821544E0 List40::Insert");
  List40Link(base, list, int16_t(uint16_t(position)));
  const uint32_t node = Walk(base, list, schema::List40(base, list).head_address(), position);
  const uint32_t slot = uint32_t(LoadS16(base, node + 2));
  guest::StoreU32(base, ((slot + 43) << 2) + list, word);
}

void List16Insert20(uint8_t* base, uint32_t list, uint32_t record, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_82154458 List16::Insert20");
  track_zones::List16Link(base, list, int16_t(uint16_t(position)));
  const uint32_t node = Walk(base, list, schema::List16(base, list).head_address(), position);
  uint32_t to = uint32_t(int64_t(LoadS16(base, node + 2)) * 20) + list + schema::List16::kSize;
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
  const uint32_t node = Walk(base, list, schema::List16(base, list).head_address(), position);
  *copy_to =
      uint32_t(int64_t(LoadS16(base, node + 2)) * int64_t(size)) + list + schema::List16::kSize;
  return c_runtime::Memcpy(base, *copy_to, record, size);
}

bool List16Copy32(uint8_t* base, uint32_t out, uint32_t list, uint32_t position,
                  uint32_t* copy_from) {
  ITC_NATIVE_RUNNING("sub_82154300 List16::Copy32");
  const uint32_t node = Walk(base, list, schema::List16(base, list).head_address(), position);
  *copy_from = (uint32_t(LoadS16(base, node + 2)) << 5) + list + schema::List16::kSize;
  return c_runtime::Memcpy(base, out, *copy_from, 32);
}

// --- The camera animations' table ---

void AnimsClear(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82150078 CameraAnims::Clear");
  G::set_anim_count(base, 0);
}

uint32_t AnimsFind(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82150300 CameraAnims::Find");
  const uint32_t count = G::anim_count(base);
  uint32_t entry = G::anims_address();
  for (uint32_t i = 0; i < count; i++, entry += schema::CameraAnimEntry::kSize) {
    const schema::CameraAnimEntry e(base, entry);
    if (e.id() == id) {
      return e.objects_address();
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
  const schema::FadeCue c(base, cue);
  const int32_t start = c.start();
  if (start == c.frame()) {
    const uint32_t kind = c.kind();
    if (kind <= 1) {
      const uint32_t passes = uint32_t(c.end()) - uint32_t(start);
      const uint32_t red = c.red();
      const uint32_t green = c.green();
      const uint32_t blue = c.blue();
      if (kind == 0) {
        mode_tick::ScreenFadesFadeIn(base, passes, red, green, blue, 0);
      } else {
        mode_tick::ScreenFadesFadeOut(base, passes, red, green, blue, 0);
      }
    }
  }
  if (c.hold() == 0) {
    const int32_t frame = c.frame();
    if (c.end() > frame) {
      c.set_frame(frame + 1);
    }
  }
}

}  // namespace camera_modes
