// The impacts: a corner of one box crossing a face of the other between
// the two placements (docs/functions/collision.md): see collision.h.

#include "collision/collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "guest.h"
#include "vehicle_accessors/vectors.h"

namespace collision {

using namespace car_physics::ops;
namespace vl = vector_library;

namespace {

constexpr uint32_t kMinusOne = 0x82001BE0;  // the game's float -1.0
constexpr uint32_t kTwo = 0x820021E8;       // 2.0
// The impact time's limit (read from guest memory; probably 1.0).
constexpr uint32_t kImpactLimit = 0x82002B90;
// The impact tests by the shapes' types (A's x 3 + B's), and the box-box
// entry's function.
constexpr uint32_t kImpactTests = 0x82600EB4;
constexpr uint32_t kBoxBoxFindImpact = 0x820BF038;

void SwapWords(uint8_t* base, uint32_t a, uint32_t b) {
  const uint32_t at_b = guest::LoadU32(base, b);
  const uint32_t at_a = guest::LoadU32(base, a);
  guest::StoreU32(base, a, at_b);
  guest::StoreU32(base, b, at_a);
}

// Both records' templates and current copies are boxes.
bool CurrentBoxes(uint8_t* base, uint32_t query) {
  for (uint32_t at : {0u, 4u}) {
    const uint32_t record = guest::LoadU32(base, query + at);
    if (!IsBox(base, guest::LoadU32(base, record)) ||
        !IsBox(base, guest::LoadU32(base, record + 8))) {
      return false;
    }
  }
  return true;
}

// One axis of a corner's move against the unit box's faces on it: the
// coordinate `axis` (0, 4 or 8) of the previous corner `p` and the current
// one `c`, the other two `u` and `v`. `positive`: the + face (bit 1, 4 or
// 16), else the - face. The crossing's fraction t when it's under the best
// so far (`best` x the move) and the other two coordinates there are within
// 1.0; `best` becomes t.
bool CrossesFace(uint8_t* base, uint32_t p, uint32_t c, uint32_t axis, uint32_t u, uint32_t v,
                 bool positive, double one, double* best) {
  const double pa = Lfs(base, p + axis);
  double move, t0;
  if (positive) {
    move = Fsubs(pa, Lfs(base, c + axis));
    t0 = Fsubs(pa, one);
  } else {
    move = Fsubs(Lfs(base, c + axis), pa);
    t0 = Fneg(Fadds(pa, one));
  }
  const double pu = Lfs(base, p + u);
  const double pv = Lfs(base, p + v);
  if (!(t0 < Fmuls(move, *best))) {
    return false;
  }
  const double t = Fdivs(t0, move);
  if (!(Fabs(Fmadds(Fsubs(Lfs(base, c + u), pu), t, pu)) < one)) {
    return false;
  }
  if (!(Fabs(Fmadds(Fsubs(Lfs(base, c + v), pv), t, pv)) < one)) {
    return false;
  }
  *best = t;
  return true;
}

}  // namespace

uint32_t BoxBoxSwapForImpact(uint8_t* base, uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BE490 BoxBox::SwapForImpact");
  ContactFlip(base, query + 32);
  SwapWords(base, query + 0, query + 4);
  SwapWords(base, query + 12, query + 20);
  SwapWords(base, query + 8, query + 16);
  return query + 64;
}

uint32_t BoxBoxSwapForPushOut(uint8_t* base, uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BEBF0 BoxBox::SwapForPushOut");
  vl::Vec4ScaleBy(base, query + 96, Lfs(base, kMinusOne));
  ContactFlip(base, query + 32);
  SwapWords(base, query + 0, query + 4);
  SwapWords(base, query + 12, query + 20);
  return query + 64;
}

bool BoxBoxCornerImpacts(uint8_t* base, uint32_t query, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820BECA0 BoxBox::CornerImpacts");
  if (!CurrentBoxes(base, query)) {
    return false;
  }
  *r3 = query;
  double best = Lfs(base, query + 84);
  const double one = Lfs(base, kOne);
  for (uint32_t k = 0; k < 8; k++) {
    // A's corner k in B's unit box, at the previous and current placements.
    const uint32_t previous = guest::LoadU32(base, query + 16) + 32 * k;
    const uint32_t current = guest::LoadU32(base, query + 20) + 32 * k;
    const uint32_t previous_code = guest::LoadU32(base, previous + 96);
    const uint32_t current_code = guest::LoadU32(base, current + 96);
    if ((previous_code & current_code) != 0) {
      continue;
    }
    const uint32_t left = previous_code & ~current_code;
    if (left == 0) {
      continue;
    }
    const uint32_t p = previous + 80, c = current + 80;
    uint32_t face = 0;
    if (left & 1) {
      CrossesFace(base, p, c, 0, 4, 8, true, one, &best);
    } else if (left & 2) {
      if (CrossesFace(base, p, c, 0, 4, 8, false, one, &best)) {
        face = 3;
      }
    }
    if (left & 4) {
      if (CrossesFace(base, p, c, 4, 0, 8, true, one, &best)) {
        face = 1;
      }
    } else if (left & 8) {
      if (CrossesFace(base, p, c, 4, 0, 8, false, one, &best)) {
        face = 4;
      }
    }
    if (left & 16) {
      if (CrossesFace(base, p, c, 8, 0, 4, true, one, &best)) {
        face = 2;
      }
    } else if (left & 32) {
      if (CrossesFace(base, p, c, 8, 0, 4, false, one, &best)) {
        face = 5;
      }
    }
    if (!(best < Lfs(base, query + 84))) {
      continue;
    }
    // The contact: A's current corner, B's face's normal, the depth the
    // corner is in by, the time.
    const uint32_t copy_a = guest::LoadU32(base, guest::LoadU32(base, query + 0) + 8);
    const uint32_t copy_b = guest::LoadU32(base, guest::LoadU32(base, query + 4) + 8);
    uint32_t normal = 0;
    const double distance =
        BoxFaceDistance(base, copy_b, face, BoxCorner(copy_a, k), &normal);
    const uint32_t face_at = BoxFace(copy_b, face);
    ContactSet(base, query + 32, BoxCorner(copy_a, k), face_at + 16, Fneg(distance), best);
    *r3 = query + 64;
  }
  return true;
}

bool BoxBoxFindImpact(uint8_t* base, uint32_t query) {
  ITC_NATIVE_RUNNING("sub_820BF038 BoxBox::FindImpact");
  if (!CurrentBoxes(base, query)) {
    return false;
  }
  uint32_t r3 = 0;
  BoxBoxCornerImpacts(base, query, &r3);
  BoxBoxSwapForImpact(base, query);
  BoxBoxCornerImpacts(base, query, &r3);
  BoxBoxSwapForImpact(base, query);
  return true;
}

bool QueryFindImpact(uint8_t* base, uint32_t query, uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_820BCE80 Query::FindImpact");
  // Both boxes (type 2): entry 8 of the table.
  if (!CurrentBoxes(base, query) ||
      guest::LoadU32(base, kImpactTests + 4 * (2 * 3 + 2)) != kBoxBoxFindImpact) {
    return false;
  }
  CopyWord(base, kZero, query + 80);
  CopyWord(base, kTwo, query + 84);
  CopyWord(base, kTwo, query + 88);
  BoxBoxFindImpact(base, query);
  *result = Lfs(base, query + 84) < Lfs(base, kImpactLimit) ? 1 : 0;
  return true;
}

}  // namespace collision
