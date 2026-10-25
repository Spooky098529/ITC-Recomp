// The box-box separation tests and the pair's next check time
// (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kMinusOne = 0x82001BE0;  // the game's float -1.0
constexpr uint32_t kTwo = 0x820021E8;       // 2.0
constexpr uint32_t kClock = 0x826C2D80;     // the collision world's clock
constexpr uint32_t kStep = 0x826C2D84;      // and its step

// A vector's x, y, z (their bits) into a temporary at `at`, its w 0.0, in
// the originals' order (x, w, z, y).
void CopyToTemporary(uint8_t* base, uint32_t at, uint32_t from) {
  CopyWord(base, from + 0, at + 0);
  CopyWord(base, kZero, at + 12);
  CopyWord(base, from + 8, at + 8);
  CopyWord(base, from + 4, at + 4);
}

// The outcode bits of one coordinate: `above` when > 1.0, else `below`
// when < -1.0 (`bgt`, `blt`: NaNs set neither).
uint32_t CoordinateBits(double value, double one, double minus_one, uint32_t above,
                        uint32_t below) {
  if (value > one) {
    return above;
  }
  if (value < minus_one) {
    return below;
  }
  return 0;
}

// A corner's outcode into its +16, as the originals set it.
void CornerOutcode(uint8_t* base, uint32_t corner, double one, double minus_one) {
  guest::StoreU32(base, corner + 16, 0);
  for (uint32_t axis = 0; axis < 3; axis++) {
    const uint32_t bits =
        CoordinateBits(Lfs(base, corner + 4 * axis), one, minus_one, 1u << (2 * axis),
                       2u << (2 * axis));
    if (bits) {
      guest::StoreU32(base, corner + 16, guest::LoadU32(base, corner + 16) | bits);
    }
  }
}

// The face index of the lowest bit of six cache bits (+x, -x, +y, -y, +z,
// -z: faces 0, 3, 1, 4, 2, 5).
uint32_t CachedFace(uint32_t bits) {
  if (bits & 1) {
    return 0;
  }
  if (bits & 2) {
    return 3;
  }
  if (bits & 4) {
    return 1;
  }
  if (bits & 8) {
    return 4;
  }
  return (bits & 0x10) ? 2 : 5;
}

// The four copies a query's shape records hold (A's previous and current,
// B's), when both templates and all four copies are boxes.
struct Copies {
  uint32_t previous_a, current_a, previous_b, current_b;
};
bool BoxCopies(uint8_t* base, uint32_t query, Copies* copies) {
  const uint32_t record_a = guest::LoadU32(base, query + 0);
  const uint32_t record_b = guest::LoadU32(base, query + 4);
  if (!IsBox(base, guest::LoadU32(base, record_a)) ||
      !IsBox(base, guest::LoadU32(base, record_b))) {
    return false;
  }
  copies->previous_a = guest::LoadU32(base, record_a + 4);
  copies->current_a = guest::LoadU32(base, record_a + 8);
  copies->previous_b = guest::LoadU32(base, record_b + 4);
  copies->current_b = guest::LoadU32(base, record_b + 8);
  return true;
}

}  // namespace

uint32_t BoxRelativePlacement(uint8_t* base, uint32_t a, uint32_t block, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820BA408 BoxShape::RelativePlacement");
  const double one = Lfs(base, kOne);
  for (uint32_t i = 0; i < 3; i++) {
    const double dot = double(vl::Vec3DotOf(base, b + 48, a + 320 + 32 * i));
    const double distance = Fsubs(dot, Lfs(base, a + 304 + 32 * i));
    Stfs(base, block + 48 + 4 * i, Fmadds(distance, Lfs(base, a + 144 + 4 * i), one));
  }
  // Entry (j, i): dot(B's normal j, A's normal i) x A's inverse half extent
  // i x B's half extent j, the products in the original's orders.
  double d;
  d = double(vl::Vec3DotOf(base, b + 320, a + 320));
  Stfs(base, block + 0, Fmuls(Fmuls(Lfs(base, a + 144), Lfs(base, b + 128)), d));
  d = double(vl::Vec3DotOf(base, b + 320, a + 352));
  Stfs(base, block + 4, Fmuls(Fmuls(Lfs(base, b + 128), d), Lfs(base, a + 148)));
  d = double(vl::Vec3DotOf(base, b + 320, a + 384));
  Stfs(base, block + 8, Fmuls(Fmuls(Lfs(base, a + 152), Lfs(base, b + 128)), d));
  d = double(vl::Vec3DotOf(base, b + 352, a + 320));
  Stfs(base, block + 16, Fmuls(Fmuls(Lfs(base, a + 144), d), Lfs(base, b + 132)));
  d = double(vl::Vec3DotOf(base, b + 352, a + 352));
  Stfs(base, block + 20, Fmuls(Fmuls(d, Lfs(base, b + 132)), Lfs(base, a + 148)));
  d = double(vl::Vec3DotOf(base, b + 352, a + 384));
  Stfs(base, block + 24, Fmuls(Fmuls(Lfs(base, a + 152), d), Lfs(base, b + 132)));
  d = double(vl::Vec3DotOf(base, b + 384, a + 320));
  Stfs(base, block + 32, Fmuls(Fmuls(Lfs(base, a + 144), d), Lfs(base, b + 136)));
  d = double(vl::Vec3DotOf(base, b + 384, a + 352));
  Stfs(base, block + 36, Fmuls(Fmuls(d, Lfs(base, b + 136)), Lfs(base, a + 148)));
  d = double(vl::Vec3DotOf(base, b + 384, a + 384));
  Stfs(base, block + 40, Fmuls(Fmuls(d, Lfs(base, a + 152)), Lfs(base, b + 136)));
  return b + 384;
}

uint32_t BoxRelativeCenter(uint8_t* base, uint32_t a, uint32_t block, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820BA5D8 BoxShape::RelativeCenter");
  const double one = Lfs(base, kOne);
  for (uint32_t i = 0; i < 3; i++) {
    const double dot = double(vl::Vec3DotOf(base, b + 48, a + 320 + 32 * i));
    const double distance = Fsubs(dot, Lfs(base, a + 304 + 32 * i));
    Stfs(base, block + 48 + 4 * i, Fmadds(distance, Lfs(base, a + 144 + 4 * i), one));
  }
  return b + 48;
}

uint32_t BoxBoxOutcode(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820BD0C8 BoxBox::Outcode");
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  uint32_t bits = 0;
  for (uint32_t axis = 0; axis < 3; axis++) {
    const double column1 = Fabs(Lfs(base, block + 16 + 4 * axis));
    const double column2 = Fabs(Lfs(base, block + 32 + 4 * axis));
    const double column0 = Fabs(Lfs(base, block + 0 + 4 * axis));
    const double center = Lfs(base, block + 48 + 4 * axis);
    const double reach = Fadds(Fadds(Fadds(column2, column1), column0), one);
    if (Fsubs(center, reach) > zero) {
      bits |= 1u << (2 * axis);
    } else if (Fadds(center, reach) < zero) {
      bits |= 2u << (2 * axis);
    }
  }
  return bits;
}

void BoxBoxBlockCorners(uint8_t* base, uint32_t block, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BC978 BoxBox::BlockCorners");
  const uint32_t row0 = block + 0, row1 = block + 16, row2 = block + 32;
  const uint32_t corner = block + 80;  // corner k at + 32k
  // Corner 0: the center plus the three rows.
  vl::Vec3AddCopy(base, frame + 240, block + 48, row0);
  vl::Vec3AddCopy(base, frame + 96, frame + 240, row1);
  vl::Vec3AddCopy(base, frame + 80, frame + 96, row2);
  CopyToTemporary(base, frame + 176, frame + 80);
  vl::Vec4Copy(base, corner + 0, frame + 176);
  // The others, each from one before it and twice a row: (from, row,
  // subtract, temporary, to).
  const double two = Lfs(base, kTwo);
  const struct {
    uint32_t from, row;
    bool subtract;
    uint32_t temporary, to;
  } steps[] = {
      {0, row0, true, 224, 1},  {1, row1, true, 208, 3},  {3, row0, false, 192, 2},
      {2, row2, true, 160, 6},  {6, row1, false, 112, 4}, {4, row0, true, 128, 5},
      {5, row1, true, 144, 7},
  };
  for (const auto& s : steps) {
    vl::Vec3ScaledCopy(base, frame + 80, s.row, two);
    CopyToTemporary(base, frame + s.temporary, frame + 80);
    if (s.subtract) {
      vl::Vec3Difference(base, frame + 96, corner + 32 * s.from, frame + s.temporary);
    } else {
      vl::Vec3Sum(base, frame + 96, corner + 32 * s.from, frame + s.temporary);
    }
    vl::Vec4Copy(base, corner + 32 * s.to, frame + 96);
  }
  const double minus_one = Lfs(base, kMinusOne);
  const double one = Lfs(base, kOne);
  for (uint32_t k = 0; k < 8; k++) {
    CornerOutcode(base, corner + 32 * k, one, minus_one);
  }
}

void BoxBoxMoveCorners(uint8_t* base, uint32_t corners, uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_820BCCE8 BoxBox::MoveCorners");
  const double minus_one = Lfs(base, kMinusOne);
  const double one = Lfs(base, kOne);
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t corner = corners + 32 * k;
    guest::StoreU32(base, corner + 16, 0);
    vl::Vec3AddTo(base, corner, offset);
    for (uint32_t axis = 0; axis < 3; axis++) {
      const uint32_t bits =
          CoordinateBits(Lfs(base, corner + 4 * axis), one, minus_one, 1u << (2 * axis),
                         2u << (2 * axis));
      if (bits) {
        guest::StoreU32(base, corner + 16, guest::LoadU32(base, corner + 16) | bits);
      }
    }
  }
}

namespace {

// B's projected radius on the face's normal (|dot(axis i, n)| x half extent
// i, summed as the original does) and whether B's center lies at least
// that far outside the face; `strict` the second test's `bgt`, else the
// first's "not blt".
bool Outside(uint8_t* base, uint32_t face, uint32_t box, bool strict) {
  const uint32_t normal = face + 16;
  const double d0 = Fabs(double(vl::Vec3DotOf(base, box + 320, normal)));
  const double d1 = Fabs(double(vl::Vec3DotOf(base, box + 352, normal)));
  const double d2 = double(vl::Vec3DotOf(base, box + 384, normal));
  const double part1 = Fmuls(Lfs(base, box + 132), d1);
  const double part2 = Fmadds(Lfs(base, box + 136), Fabs(d2), part1);
  const double radius = Fmadds(Lfs(base, box + 128), d0, part2);
  const double distance =
      Fsubs(double(vl::Vec3DotOf(base, box + 48, normal)), Lfs(base, face + 0));
  return strict ? distance > radius : !(distance < radius);
}

}  // namespace

uint32_t BoxBoxFaceSeparates(uint8_t* base, uint32_t previous_face, uint32_t current_face,
                             uint32_t previous_box, uint32_t current_box) {
  ITC_NATIVE_RUNNING("sub_820BB300 BoxBox::FaceSeparates");
  if (!Outside(base, previous_face, previous_box, false)) {
    return 0;
  }
  return Outside(base, current_face, current_box, true) ? 1 : 0;
}

bool BoxBoxSeparated(uint8_t* base, uint32_t query, uint32_t cache, uint32_t frame,
                     uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BD280 BoxBox::Separated");
  Copies c;
  if (!BoxCopies(base, query, &c) || !IsBox(base, c.previous_a) || !IsBox(base, c.current_a) ||
      !IsBox(base, c.previous_b) || !IsBox(base, c.current_b)) {
    return false;
  }
  // The cached face (one of A's faces in bits 0x3F, of B's in 0x3F00).
  const uint32_t cached = guest::LoadU32(base, cache);
  if (cached != 0) {
    bool tested = false;
    uint32_t separates = 0;
    if ((cached & 0x3F) != 0) {
      const uint32_t face = CachedFace(cached);
      const uint32_t current_face = BoxFace(c.current_a, face);
      const uint32_t previous_face = BoxFace(c.previous_a, face);
      separates = BoxBoxFaceSeparates(base, previous_face, current_face, c.previous_b,
                                      c.current_b);
      tested = true;
    } else if ((cached & 0x3F00) != 0) {
      const uint32_t face = CachedFace(cached >> 8);
      const uint32_t current_face = BoxFace(c.current_b, face);
      const uint32_t previous_face = BoxFace(c.previous_b, face);
      separates = BoxBoxFaceSeparates(base, previous_face, current_face, c.previous_a,
                                      c.current_a);
      tested = true;
    }
    if (tested && separates != 0) {
      *result = 1;
      return true;
    }
  }
  const uint32_t block_previous_ba = guest::LoadU32(base, query + 8);
  const uint32_t block_current_ba = guest::LoadU32(base, query + 12);
  const uint32_t block_previous_ab = guest::LoadU32(base, query + 16);
  const uint32_t block_current_ab = guest::LoadU32(base, query + 20);
  BoxRelativePlacement(base, c.previous_a, block_previous_ba, c.previous_b);
  guest::StoreU32(base, block_previous_ba + 64, BoxBoxOutcode(base, block_previous_ba));
  BoxRelativePlacement(base, c.current_a, block_current_ba, c.current_b);
  uint32_t outcode = BoxBoxOutcode(base, block_current_ba);
  guest::StoreU32(base, block_current_ba + 64, outcode);
  uint32_t shared = guest::LoadU32(base, block_previous_ba + 64) & outcode;
  if (shared != 0) {
    guest::StoreU32(base, cache, shared);
    *result = 1;
    return true;
  }
  BoxRelativePlacement(base, c.previous_b, block_previous_ab, c.previous_a);
  guest::StoreU32(base, block_previous_ab + 64, BoxBoxOutcode(base, block_previous_ab));
  BoxRelativePlacement(base, c.current_b, block_current_ab, c.current_a);
  outcode = BoxBoxOutcode(base, block_current_ab);
  guest::StoreU32(base, block_current_ab + 64, outcode);
  shared = guest::LoadU32(base, block_previous_ab + 64) & outcode;
  if (shared != 0) {
    guest::StoreU32(base, cache, shared << 8);
    *result = 1;
    return true;
  }
  guest::StoreU32(base, cache, 0);
  for (uint32_t block : {block_previous_ba, block_current_ba, block_previous_ab,
                         block_current_ab}) {
    BoxBoxBlockCorners(base, block, frame - 320);
  }
  *result = 0;
  return true;
}

bool BoxBoxSeparatedAfterMove(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BD578 BoxBox::SeparatedAfterMove");
  Copies c;
  if (!BoxCopies(base, query, &c)) {
    return false;
  }
  const uint32_t block_current_ba = guest::LoadU32(base, query + 12);
  const uint32_t block_previous_ba = guest::LoadU32(base, query + 8);
  const uint32_t block_previous_ab = guest::LoadU32(base, query + 16);
  const uint32_t block_current_ab = guest::LoadU32(base, query + 20);
  // The current blocks' centers before (+80, +112 of the frame, w 0.0).
  CopyToTemporary(base, frame + 80, block_current_ba + 48);
  BoxRelativeCenter(base, c.current_a, block_current_ba, c.current_b);
  uint32_t outcode = BoxBoxOutcode(base, block_current_ba);
  guest::StoreU32(base, block_current_ba + 64, outcode);
  if ((guest::LoadU32(base, block_previous_ba + 64) & outcode) != 0) {
    *result = 1;
    return true;
  }
  CopyToTemporary(base, frame + 112, block_current_ab + 48);
  BoxRelativeCenter(base, c.current_b, block_current_ab, c.current_a);
  outcode = BoxBoxOutcode(base, block_current_ab);
  guest::StoreU32(base, block_current_ab + 64, outcode);
  if ((guest::LoadU32(base, block_previous_ab + 64) & outcode) != 0) {
    *result = 1;
    return true;
  }
  // The corners moved by the centers' change.
  vl::Vec3SubCopy(base, frame + 144, block_current_ba + 48, frame + 80);
  CopyToTemporary(base, frame + 96, frame + 144);
  BoxBoxMoveCorners(base, block_current_ba + 80, frame + 96);
  vl::Vec3SubCopy(base, frame + 144, block_current_ab + 48, frame + 112);
  CopyToTemporary(base, frame + 128, frame + 144);
  BoxBoxMoveCorners(base, block_current_ab + 80, frame + 128);
  *result = 0;
  return true;
}

bool QuerySeparated(uint8_t* base, uint32_t query, uint32_t cache, uint32_t frame,
                    uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BD508 Query::Separated");
  return BoxBoxSeparated(base, query, cache, frame - 160, result);
}

bool QuerySeparatedAfterMove(uint8_t* base, uint32_t query, uint32_t frame, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BD6E0 Query::SeparatedAfterMove");
  return BoxBoxSeparatedAfterMove(base, query, frame - 240, result);
}

double CollisionPairNextCheckTime(uint8_t* base, uint32_t pair, uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BBB20 CollisionPair::NextCheckTime");
  const double step = Lfs(base, kStep);
  const double clock = Lfs(base, kClock);
  const uint32_t a = guest::LoadU32(base, pair + 16);
  const uint32_t b = guest::LoadU32(base, pair + 20);
  if (guest::LoadU32(base, a + 272) == 0 && guest::LoadU32(base, b + 272) == 0 &&
      guest::LoadU32(base, a + 276) == 0 && guest::LoadU32(base, b + 276) == 0) {
    const double speed_a = double(va::RigidBodyFloat48(base, guest::LoadU32(base, a + 16)));
    const double speed_b = double(va::RigidBodyFloat48(
        base, guest::LoadU32(base, guest::LoadU32(base, pair + 20) + 16)));
    const double speeds = Fadds(speed_b, speed_a);
    if (speeds != Lfs(base, kZero)) {
      const uint32_t center_b =
          CollisionShapeCurrentCenter(base, guest::LoadU32(base, pair + 20) + 32);
      const uint32_t center_a =
          CollisionShapeCurrentCenter(base, guest::LoadU32(base, pair + 16) + 32);
      vl::Vec3Difference(base, frame + 80, center_a, center_b);
      const double gap = double(vl::Vec3Length(base, frame + 80));
      const double radius_a =
          ShapeRadius(base, guest::LoadU32(base, guest::LoadU32(base, pair + 16) + 20));
      const uint32_t shape_b = guest::LoadU32(base, guest::LoadU32(base, pair + 20) + 20);
      const double radius_b = ShapeRadius(base, shape_b);
      *r3 = shape_b;
      return Fadds(Fdivs(Fsubs(gap, Fadds(radius_b, radius_a)), speeds), clock);
    }
    // r3 as RigidBodyFloat48's adapter leaves it: B's body.
    *r3 = guest::LoadU32(base, guest::LoadU32(base, pair + 20) + 16);
    return Fadds(clock, step);
  }
  *r3 = pair;
  return Fadds(clock, step);
}

}  // namespace collision
