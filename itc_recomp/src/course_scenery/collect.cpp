// The course scenery's collection of what a view sees and the course set's
// draws (course_scenery.h; docs/functions/course-scenery.md). Built with
// strict floats (CMakeLists.txt): the distances are the originals' single
// precision steps, each rounded as theirs.

#include "course_scenery/course_scenery.h"

#include "guest.h"

#include "base/log.h"
#include "game_data/data_file.h"

#include <bit>
#include <cmath>

namespace course_scenery {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// A float as lfs loads it.
double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}

constexpr uint32_t kZero = 0x82000A34;        // 0.0: inside an area's rectangle
constexpr uint32_t kHalf = 0x82001BE4;        // the far distance's share the eye is moved by
constexpr uint32_t kAreaMargin = 0x82002A20;  // 70.0, past the far distance
constexpr uint32_t kModelMargin = 0x8200257C;
constexpr uint32_t kNoDistance = 0x82002A90;  // the nearest corner's start

// The views' masks: 14 records of 16 bytes (+0 a view, +8 a camera pass,
// +12 its bits), and the camera passes (592 bytes each, +588 enabled).
constexpr uint32_t kViewMasks = 0x82602F80;
constexpr uint32_t kCameraPasses = 0x826BAC00;

// The OR of the bits of `view`'s records whose camera pass is enabled.
uint32_t ViewMask(uint8_t* base, uint32_t view) {
  uint32_t mask = 0;
  for (uint32_t i = 0; i < 14; i++) {
    const uint32_t record = kViewMasks + 16 * i;
    if (Load(base, record) != view) {
      continue;
    }
    if (Load(base, Load(base, record + 8) * 592u + kCameraPasses + 588) != 0) {
      mask |= Load(base, record + 12);
    }
  }
  return mask;
}

// A corner's distance from the eye in x and z, as the originals' fsubs,
// fmuls, fmadds and fsqrts round it.
double CornerDistance(double x, double z, double eye_x, double eye_z) {
  const double dx = float(x - eye_x);
  const double dz = float(z - eye_z);
  double sum = float(dx * dx);
  sum = float(std::fma(dz, dz, sum));
  return float(std::sqrt(sum));
}

// Whether (x, z) is inside the rectangle of corners 0 and 2 (16-byte points
// at `corners`), as the originals' four compares (a NaN is inside).
bool Inside(uint8_t* base, uint32_t corners, double x, double z) {
  return !(x < Lfs(base, corners + 0)) && !(x > Lfs(base, corners + 32)) &&
         !(z < Lfs(base, corners + 8)) && !(z > Lfs(base, corners + 40));
}

}  // namespace

uint32_t CollectAreas(uint8_t* base, uint32_t stack, uint32_t manager, uint32_t view,
                      uint32_t viewport, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_820F3098 Scenery::CollectAreas");
  // memcpy's r3: the corners' copy in the original's frame.
  const uint32_t copy = stack - 288 + 80;
  const uint32_t set = Load(base, manager + 112);
  const double far = Lfs(base, viewport + 72);
  const uint32_t file = Load(base, set + 1056);
  const double far_plus = float(far + Lfs(base, kAreaMargin));
  const uint32_t count = std::byteswap(Load(base, file));  // little-endian
  if (count == 0) {
    return r3;
  }
  const double half = Lfs(base, kHalf);
  const double zero = Lfs(base, kZero);
  const double none = Lfs(base, kNoDistance);
  for (uint32_t area = 0; area < count; area++) {
    const uint32_t entry = game_data::GetEntry(base, file, area, false);
    const uint32_t corners = entry + 32;
    r3 = copy;
    const double eye_x = Lfs(base, viewport + 1600);
    const double eye_z = Lfs(base, viewport + 1608);
    double nearest = none;
    if (Inside(base, corners, eye_x, eye_z)) {
      nearest = zero;
    } else {
      // The eye moved by half the far distance each way in x and z: inside
      // the rectangle, that's the distance; else the corners' nearest so far.
      const double shift = float(far * half);
      for (uint32_t i = 0; i < 4; i++) {
        const double x = float(eye_x + ((i & 1) != 0 ? shift : -shift));
        const double z = float(eye_z + ((i & 2) != 0 ? shift : -shift));
        if (Inside(base, corners, x, z)) {
          nearest = shift;
          break;
        }
        const double distance = CornerDistance(Lfs(base, corners + 16 * i),
                                               Lfs(base, corners + 16 * i + 8), eye_x, eye_z);
        if (!(distance > nearest)) {
          nearest = distance;
        }
      }
    }
    if (far_plus < nearest) {
      continue;
    }
    r3 = ListAdd(base, manager + 116, nearest, area, 0, ViewMask(base, view), manager);
  }
  return r3;
}

uint32_t CollectModels(uint8_t* base, uint32_t stack, uint32_t manager, uint32_t view,
                       uint32_t viewport, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_820F32D0 Scenery::CollectModels");
  const uint32_t copy = stack - 320 + 80;
  const uint32_t list_a = manager + 116;
  const uint32_t set = Load(base, manager + 112);
  const double far_plus = float(Lfs(base, viewport + 72) + Lfs(base, kModelMargin));
  const uint32_t file = Load(base, set + 1056);
  if (Load(base, list_a + 8) == 0) {
    return r3;
  }
  const double zero = Lfs(base, kZero);
  const double none = Lfs(base, kNoDistance);
  for (uint32_t i = 0; i < Load(base, list_a + 8); i++) {
    const uint32_t item = Load(base, Load(base, list_a + 16) + 4 * i);
    const uint32_t area = guest::LoadU16(base, item + 8);
    const uint32_t entry = game_data::GetEntry(base, file, area, false);
    r3 = entry;
    // The area's models: a big-endian count at +0, 96-byte records from +96
    // (+0 the model's number, +32 its four corners).
    const uint32_t models = Load(base, entry);
    uint32_t record = entry + 96;
    for (uint32_t k = models; k != 0; k--, record += 96) {
      const uint32_t number = Load(base, record);
      const uint32_t corners = record + 32;
      r3 = copy;
      const double eye_x = Lfs(base, viewport + 1600);
      const double eye_z = Lfs(base, viewport + 1608);
      double nearest = none;
      for (uint32_t c = 0; c < 4; c++) {
        const double distance = CornerDistance(Lfs(base, corners + 16 * c),
                                               Lfs(base, corners + 16 * c + 8), eye_x, eye_z);
        if (!(distance > nearest)) {
          nearest = distance;
        }
      }
      if (far_plus < nearest) {
        continue;
      }
      if (Inside(base, corners, eye_x, eye_z)) {
        nearest = zero;
      }
      const uint32_t mask = ViewMask(base, view);
      if (mask == 0) {
        continue;
      }
      r3 = ListAdd(base, manager + 144, nearest, area, number, mask, manager);
    }
  }
  return r3;
}

}  // namespace course_scenery
