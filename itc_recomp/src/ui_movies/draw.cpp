// The UI movie library's drawing walk natively (ui_movies.h): the nodes'
// transforms. The draw contexts and nodes are guest objects; the matrices go
// through the matrix math (src/matrix_math/) and are noted for frame
// interpolation as the originals' calls are, with their return addresses.
// Built with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).

#include "ui_movies/ui_movies.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "ui_movies/game.h"

#include <simde/x86/sse2.h>

#include <climits>
#include <cmath>

namespace ui_movies {

using namespace car_physics::ops;

namespace {

constexpr uint32_t kColorScale = 0x82002200;   // the node's colors' scale
constexpr uint32_t kDegrees = 0x82002948;      // degrees to radians
constexpr uint32_t kNearZero = 0x82002A44;     // the rotation's and scale's threshold

// fctiwz, as the translation does it: NaN gives INT_MIN, INT_MAX and above
// INT_MAX, below INT_MIN INT_MIN.
int32_t Fctiwz(double x) {
  if (std::isnan(x)) {
    return INT32_MIN;
  }
  if (x >= double(INT32_MAX)) {
    return INT32_MAX;
  }
  return simde_mm_cvttsd_si32(simde_mm_set_sd(x));
}

// An integer as lfd of its std then fcfid then frsp.
double IntToFloat(int32_t n) {
  return double(float(double(int64_t(n))));
}

// The identity at `m` (64 bytes of guest memory), as the original builds it
// from the vector unit's 1.0 and 0.0 and copies it there.
void Identity(uint8_t* base, uint32_t m) {
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, m + 4 * i, i % 5 == 0 ? 0x3F800000u : 0);
  }
}

// ctx = local * ctx, noted as the original's call returning to `caller`.
void MultiplyInto(uint8_t* base, uint32_t context, uint32_t local, uint32_t caller) {
  matrix_math::Multiply(base, context, local, context);
  interpolation::BlendState_OnMultiply(base, context, local, context, caller);
}

// (x > 0.0 ? x : -x) > the threshold, as the original's compares (a NaN
// fails it).
bool BeyondThreshold(double x, double zero, double threshold) {
  return (x > zero ? x : Fneg(x)) > threshold;
}

}  // namespace

uint32_t ApplyTransform(uint8_t* base, uint32_t context, uint32_t node, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8259E6C8 UiMovie::ApplyTransform");
  uint32_t r3 = context;
  // The clip: the node's rectangle offset by the context's +32 and +36 (as
  // integers), kept or intersected with the context's (sub_8259D730,
  // merged).
  const uint32_t n40 = guest::LoadU32(base, node + 40);
  if (n40 != 0 || guest::LoadU32(base, node + 44) != 0 || guest::LoadU32(base, node + 48) != 0 ||
      guest::LoadU32(base, node + 52) != 0) {
    const uint32_t x = uint32_t(Fctiwz(Lfs(base, context + 32)));
    const uint32_t n48 = guest::LoadU32(base, node + 48);
    const uint32_t y = uint32_t(Fctiwz(Lfs(base, context + 36)));
    const uint32_t n44 = guest::LoadU32(base, node + 44);
    const uint32_t n52 = guest::LoadU32(base, node + 52);
    const uint32_t clip = context + 64;
    const uint32_t rect[4] = {x + n40, y + n44, x + n48, y + n52};
    r3 = clip;
    if (guest::LoadU32(base, clip + 0) == 0 && guest::LoadU32(base, clip + 4) == 0 &&
        guest::LoadU32(base, clip + 8) == 0 && guest::LoadU32(base, clip + 12) == 0) {
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreU32(base, clip + 4 * i, rect[i]);
      }
    } else {
      ITC_NATIVE_RUNNING("sub_8259D730 (merged into UiMovie::ApplyTransform)");
      // +0 and +12 the larger, +4 and +8 the smaller (signed), as read.
      const int32_t a0 = int32_t(guest::LoadU32(base, clip + 0));
      guest::StoreU32(base, clip + 0, uint32_t(a0 > int32_t(rect[0]) ? a0 : int32_t(rect[0])));
      const int32_t a1 = int32_t(guest::LoadU32(base, clip + 4));
      guest::StoreU32(base, clip + 4, uint32_t(a1 < int32_t(rect[1]) ? a1 : int32_t(rect[1])));
      const int32_t a2 = int32_t(guest::LoadU32(base, clip + 8));
      guest::StoreU32(base, clip + 8, uint32_t(a2 < int32_t(rect[2]) ? a2 : int32_t(rect[2])));
      const int32_t a3 = int32_t(guest::LoadU32(base, clip + 12));
      guest::StoreU32(base, clip + 12, uint32_t(a3 > int32_t(rect[3]) ? a3 : int32_t(rect[3])));
    }
  }
  // The color: r, g, b times the node's integers times the scale, alpha
  // times the node's float.
  {
    const double r = Lfs(base, context + 80);
    const double g = Lfs(base, context + 84);
    const double b = Lfs(base, context + 88);
    const double a = Lfs(base, context + 92);
    const double nr = IntToFloat(int32_t(guest::LoadU32(base, node + 56)));
    const double scale = Lfs(base, kColorScale);
    Stfs(base, context + 80, Fmuls(Fmuls(nr, r), scale));
    const double ng = IntToFloat(int32_t(guest::LoadU32(base, node + 60)));
    Stfs(base, context + 84, Fmuls(Fmuls(ng, g), scale));
    const double nb = IntToFloat(int32_t(guest::LoadU32(base, node + 64)));
    Stfs(base, context + 88, Fmuls(Fmuls(nb, b), scale));
    Stfs(base, context + 92, Fmuls(Lfs(base, node + 68), a));
  }
  const double zero = Lfs(base, kZero);
  const uint32_t local = frame + 176;
  // Translated by the position plus the pivot.
  {
    const uint32_t n20 = guest::LoadU32(base, node + 20);
    if (n20 != 0 || !(Lfs(base, node + 12) == zero) || guest::LoadU32(base, node + 24) != 0 ||
        !(Lfs(base, node + 16) == zero)) {
      Identity(base, local);
      const double px = Lfs(base, node + 12);
      const double py = Lfs(base, node + 16);
      Stfs(base, local + 56, zero);
      Stfs(base, local + 48, Fadds(px, IntToFloat(int32_t(n20))));
      Stfs(base, local + 52,
           Fadds(py, IntToFloat(int32_t(guest::LoadU32(base, node + 24)))));
      MultiplyInto(base, context, local, 0x8259E940);
      r3 = context;
    }
  }
  // Turned by the rotation (degrees).
  const double threshold = Lfs(base, kNearZero);
  {
    const double angle = Lfs(base, node + 36);
    if (BeyondThreshold(angle, zero, threshold)) {
      const uint32_t turn = frame + 240;
      Identity(base, turn);
      Stfs(base, frame + 80, Fmuls(angle, Lfs(base, kDegrees)));
      matrix_math::RotateZ(base, turn, turn, frame + 80, frame - 80);
      MultiplyInto(base, context, turn, 0x8259EA08);
      r3 = context;
    }
  }
  // Scaled, unless both factors are near zero; +108 set when either isn't
  // 1.0.
  {
    const double sx = Lfs(base, node + 28);
    if (BeyondThreshold(sx, zero, threshold) ||
        BeyondThreshold(Lfs(base, node + 32), zero, threshold)) {
      Identity(base, local);
      // lfs then stfs: the words' bits (as CopyWord).
      CopyWord(base, node + 28, local + 0);
      CopyWord(base, node + 32, local + 20);
      MultiplyInto(base, context, local, 0x8259EAD8);
      r3 = context;
      const double one = Lfs(base, kOne);
      if (!(Lfs(base, node + 28) == one) || !(Lfs(base, node + 32) == one)) {
        guest::StoreU32(base, context + 108, 1);
      }
    }
  }
  // Translated back by the pivot.
  {
    const uint32_t n20 = guest::LoadU32(base, node + 20);
    if (n20 != 0 || guest::LoadU32(base, node + 24) != 0) {
      Identity(base, local);
      const uint32_t n24 = guest::LoadU32(base, node + 24);
      Stfs(base, local + 56, zero);
      Stfs(base, local + 48, IntToFloat(int32_t(0u - n20)));
      Stfs(base, local + 52, IntToFloat(int32_t(0u - n24)));
      MultiplyInto(base, context, local, 0x8259EBC4);
      r3 = context;
    }
  }
  return r3;
}

uint32_t CopyDrawContext(uint8_t* base, uint32_t context, uint32_t parent) {
  ITC_NATIVE_RUNNING("sub_8259D9F8 DrawContext::Copy");
  // The w column set to 1.0 first (then overwritten by the copy), as read.
  const double one = Lfs(base, kOne);
  Stfs(base, context + 12, one);
  Stfs(base, context + 28, one);
  Stfs(base, context + 44, one);
  Stfs(base, context + 60, one);
  matrix_math::Copy(base, context, parent);
  interpolation::BlendState_OnCopy(base, context, parent, 0x8259DA30);
  for (uint32_t at = 64; at < 112; at += 4) {  // the clip, the color, +96..+108
    guest::StoreU32(base, context + at, guest::LoadU32(base, parent + at));
  }
  return context;
}

uint32_t CopyQueuedText(uint8_t* base, uint32_t to, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_8259E620 TextItem::CopyQueued");
  guest::StoreU32(base, to + 0, guest::LoadU32(base, from + 0));
  matrix_math::Copy(base, to + 16, from + 16);
  interpolation::BlendState_OnCopy(base, to + 16, from + 16, 0x8259E650);
  CopyWord(base, from + 80, to + 80);  // lfs then stfs: its bits
  for (uint32_t at = 84; at < 116; at += 4) {  // the color and the clip
    guest::StoreU32(base, to + at, guest::LoadU32(base, from + at));
  }
  return to;
}

bool TextListFull(uint8_t* base, uint32_t list) {
  return guest::LoadU32(base, list + 8) >= guest::LoadU32(base, list + 4);
}

bool AppendText(uint8_t* base, uint32_t list, uint32_t item, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8259F700 TextList::Append");
  if (TextListFull(base, list)) {
    return false;  // the grow (sub_8259EC58, never seen in play): the original
  }
  const uint32_t count = guest::LoadU32(base, list + 8);
  const uint32_t slot = (count << 7) + guest::LoadU32(base, list + 0);
  guest::StoreU32(base, list + 8, count + 1);
  *r3 = CopyQueuedText(base, slot, item);
  return true;
}

bool QueueText(uint8_t* base, uint32_t context, uint32_t node, uint32_t layer, uint32_t frame,
               uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8259F770 UiMovie::QueueText");
  const uint32_t list = guest::LoadU32(base, context + 100);
  if (list != 0 && TextListFull(base, list)) {
    return false;
  }
  *r3 = ApplyTransform(base, context, node, frame - 352);
  if (guest::LoadU32(base, context + 100) == 0) {
    return true;
  }
  const uint32_t item = frame + 96;
  MakeTextItem(base, item);
  guest::StoreU32(base, item + 0, node);
  matrix_math::Copy(base, item + 16, context);
  interpolation::BlendState_OnCopy(base, item + 16, context, 0x8259F7B4);
  uint32_t color[4], clip[4];
  for (uint32_t i = 0; i < 4; i++) {
    color[i] = guest::LoadU32(base, context + 80 + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    clip[i] = guest::LoadU32(base, context + 64 + 4 * i);
  }
  // The layer as an unsigned 32-bit integer (std then lfd, fcfid, frsp).
  const double depth = double(float(double(int64_t(uint64_t(layer)))));
  Stfs(base, item + 80, Fmuls(depth, Lfs(base, 0x82071520)));
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, item + 84 + 4 * i, color[i]);
  }
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, item + 100 + 4 * i, clip[i]);
  }
  uint32_t appended = 0;
  AppendText(base, guest::LoadU32(base, context + 100), item, &appended);
  *r3 = ClearTextItem(base, item);
  return true;
}

}  // namespace ui_movies
