// The course scenery's collection of what a view sees and the course set's
// draws (course_scenery.h; docs/functions/course-scenery.md). Built with
// strict floats (CMakeLists.txt): the distances are the originals' single
// precision steps, each rounded as theirs.

#include "course_scenery/course_scenery.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "game_data/data_file.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "scenery_lists/scenery_lists.h"

#include <bit>
#include <cmath>
#include <initializer_list>

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

namespace {

constexpr uint32_t kRangeNear = 0x82002814;  // the draws' range
constexpr uint32_t kRangeFar = 0x82006804;
constexpr uint32_t kDrawKeyA = 0x8296B90C;   // the pass's +24
constexpr uint32_t kDrawKeyB = 0x8296B900;   // the pass's +28
constexpr uint32_t kViewCopy = 0x8265ECA0;   // the camera pass's view
constexpr uint32_t kRaceTable = 0x826D82E8;  // the point lists' +616 from here
constexpr uint32_t kTextureProviderSet = 0x82594718;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kDrawShape = 0x82147D88;
constexpr uint32_t kPointListDraw = 0x82148058;
constexpr uint32_t kFrameBytes = 384;

uint32_t GuestCall(const scenery_lists::DrawCalls& calls, uint32_t frame, uint32_t lr,
                   uint32_t function, std::initializer_list<uint32_t> args) {
  return calls.call(calls.user, function, args.begin(), int(args.size()), frame, lr);
}

// The model of a list's node `index` steps from its head (the List64 at
// `nodes`: +260 the head, each node +0 the next and +2 its slot; the models
// from +268).
uint32_t ListModel(uint8_t* base, uint32_t nodes, uint32_t index) {
  auto node_at = [&](uint32_t at) {
    return (uint32_t(int32_t(int16_t(guest::LoadU16(base, at)))) << 2) + nodes;
  };
  uint32_t node = node_at(nodes + 260);
  for (uint32_t k = index; k != 0; k--) {
    node = node_at(node);
  }
  const int32_t slot = int16_t(guest::LoadU16(base, node + 2));
  return Load(base, (uint32_t(slot + 67) << 2) + nodes);
}

}  // namespace

uint32_t DrawSetListsViewProjection(uint8_t* base, uint32_t pass) {
  return Load(base, pass + 20) * 592u + kCameraPasses + 144;
}

uint32_t DrawSetLists(uint8_t* base, const scenery_lists::DrawCalls& calls, uint32_t stack,
                      uint32_t manager, uint32_t pass) {
  ITC_NATIVE_RUNNING("sub_820F52A8 Scenery::DrawSetLists");
  const uint32_t frame = stack - kFrameBytes;
  guest::StoreU32(base, frame, stack);  // the back chain, as stwu
  const uint32_t set = Load(base, manager + 112);
  const uint32_t camera = Load(base, pass + 20) * 592u + kCameraPasses;
  const uint32_t provider = manager + 228;
  const uint32_t textures = Load(base, set + 1060);
  const uint32_t list_a = set + 1596, list_b = set + 2124, list_c = set + 2652;
  const uint32_t points = set + 3180;
  // The provider's slot 1 = the set's XPR.
  const uint32_t set_slot = Load(base, Load(base, provider) + 16);
  if (set_slot != kTextureProviderSet ||
      !scenery_lists::TextureProviderSet(base, provider, 1, set + 64)) {
    GuestCall(calls, frame, 0x820F5340, set_slot, {provider, 1, set + 64});
  }
  guest::StoreU32(base, kDrawKeyA, Load(base, pass + 24));
  guest::StoreU32(base, kDrawKeyB, Load(base, pass + 28));
  matrix_math::Copy(base, kViewCopy, camera + 16);
  interpolation::BlendState_OnCopy(base, kViewCopy, camera + 16, 0x820F5368);
  uint32_t r3 = kViewCopy;
  const uint32_t view_projection = camera + 144;  // the original's r24
  const uint32_t world = frame + 80;
  const uint32_t copy = frame + 192;
  // Before each list: the identity at +80 and the view-projection's copy at
  // +192 (memcpy natively where it can, else its original).
  auto prepare = [&](uint32_t lr) {
    for (uint32_t row = 0; row < 4; row++) {
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreU32(base, world + 16 * row + 4 * i, row == i ? 0x3F800000u : 0u);
      }
    }
    if (c_runtime::Memcpy(base, copy, view_projection, 64)) {
      r3 = copy;
    } else {
      r3 = GuestCall(calls, frame, lr, kMemcpy, {copy, view_projection, 64});
    }
  };
  const double zero = Lfs(base, kZero);
  const double far = Lfs(base, kRangeFar);
  const double near = Lfs(base, kRangeNear);
  // A model's shape 0 with the range at `range`: natively, else (nothing
  // done) through its adapter.
  auto draw = [&](uint32_t model, uint32_t range, uint32_t lr) {
    guest::StoreFloat(base, range + 0, float(near));
    guest::StoreFloat(base, range + 4, float(far));
    guest::StoreFloat(base, range + 8, float(zero));
    guest::StoreFloat(base, range + 12, float(zero));
    const uint32_t draw_frame = frame - scenery_lists::kSceneryDrawShapeFrame;
    guest::StoreU32(base, draw_frame, frame);
    const auto drawn = scenery_lists::SceneryDrawShape(base, calls, draw_frame, model,
                                                       copy, world, 0, 0, 0, range, 0,
                                                       view_projection);
    if (drawn) {
      r3 = drawn->r3;
    } else {
      r3 = GuestCall(calls, frame, lr, kDrawShape, {model, copy, world, 0, 0, 0, range, 0});
    }
  };
  prepare(0x820F53B4);
  for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, list_a)); i++) {
    const uint32_t model = ListModel(base, list_a + 4, i);
    guest::StoreU32(base, model + 48, provider);
    guest::StoreU32(base, model + 436, textures);
    draw(model, frame + 176, 0x820F5454);
  }
  prepare(0x820F54A8);
  for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, list_b)); i++) {
    r3 = scenery_lists::IsBEntryOn(base, int32_t(i));
    if (r3 == 0) {
      continue;
    }
    const uint32_t model = ListModel(base, list_b + 4, i);
    scenery_lists::ResetDrawState(base, model);
    guest::StoreU32(base, model + 48, provider);
    guest::StoreU32(base, model + 436, textures);
    draw(model, frame + 160, 0x820F5550);
  }
  if (int32_t(Load(base, list_c)) > 0) {
    prepare(0x820F55B0);
    for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, list_c)); i++) {
      r3 = scenery_lists::IsCEntryOn(base, i);
      if (r3 == 0) {
        continue;
      }
      const uint32_t model = ListModel(base, list_c + 4, i);
      scenery_lists::ResetDrawState(base, model);
      guest::StoreU32(base, model + 48, provider);
      guest::StoreU32(base, model + 436, textures);
      draw(model, frame + 144, 0x820F5658);
    }
  }
  if (int32_t(Load(base, points + 620)) > 0) {
    prepare(0x820F56B8);
    for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, points + 620)); i++) {
      r3 = scenery_lists::PointListIsEntryOn(base, int32_t(i));
      if (r3 == 0) {
        continue;
      }
      const uint32_t race = Load(base, kRaceTable) + 2184;
      guest::StoreU32(base, points + 612, provider);
      guest::StoreU32(base, points + 608, textures);
      guest::StoreU32(base, points + 616, Load(base, Load(base, race + 836) * 76u + race + 84));
      const uint32_t draw_frame = frame - scenery_lists::kPointListDrawFrame;
      guest::StoreU32(base, draw_frame, frame);
      const auto drawn = scenery_lists::PointListDraw(base, calls, draw_frame, points, i, camera,
                                                      view_projection, points);
      if (drawn) {
        r3 = *drawn;
      } else {
        r3 = GuestCall(calls, frame, 0x820F5714, kPointListDraw, {points, i, camera});
      }
    }
  }
  return r3;
}

}  // namespace course_scenery
