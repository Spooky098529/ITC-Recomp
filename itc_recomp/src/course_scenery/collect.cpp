// The course scenery's collection of what a view sees and the course set's
// draws (course_scenery.h; docs/functions/course-scenery.md). Built with
// strict floats (CMakeLists.txt): the distances are the originals' single
// precision steps, each rounded as theirs.

#include "course_scenery/course_scenery.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "cameras/cameras.h"
#include "game_data/data_file.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "scenery_lists/scenery_lists.h"
#include "schema/camera_modes.h"
#include "schema/cameras.h"
#include "schema/course_scenery.h"
#include "schema/race_states.h"
#include "schema/scenery_lists.h"

#include <bit>
#include <cmath>
#include <initializer_list>

namespace course_scenery {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// A doubleword as std stores it.
void StoreDoubleword(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

// A float as lfs loads it.
double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}

using Manager = schema::Scenery;
using Viewport = schema::SceneryViewport;
using FileEntry = schema::SceneryFileEntry;
using Item = schema::SceneryListItem;
using List = schema::SceneryItemList;
using VisibleArea = schema::SceneryVisibleArea;
using PassDesc = schema::SceneryPassDesc;
using G = schema::CourseSceneryGlobals;

constexpr uint32_t kHalf = 0x82001BE4;        // the far distance's share the eye is moved by
constexpr uint32_t kAreaMargin = 0x82002A20;  // 70.0, past the far distance
constexpr uint32_t kModelMargin = 0x8200257C;
constexpr uint32_t kNoDistance = 0x82002A90;  // the nearest corner's start

// The camera pass `index` (592 bytes each from 0x826BAC00).
schema::CameraPass PassAt(uint8_t* base, uint32_t index) {
  return schema::CameraPass(
      base, index * schema::CameraPass::kSize + schema::CamerasGlobals::passes_address());
}
// The views' masks' record `i` (14 of 16 bytes, G::view_masks).
schema::SceneryViewMask ViewMaskAt(uint8_t* base, uint32_t i) {
  return schema::SceneryViewMask(base, schema::CourseSceneryGlobals::view_masks_address() +
                                           schema::SceneryViewMask::kSize * i);
}
// The viewport of player `view` ([[0x826C2CE8] + 4 x view]).
uint32_t ViewportOf(uint8_t* base, uint32_t view) {
  return Load(base, schema::CamerasGlobals::viewports(base).first_address() + 4 * view);
}

// The OR of the bits of `view`'s records whose camera pass is enabled.
uint32_t ViewMask(uint8_t* base, uint32_t view) {
  uint32_t mask = 0;
  for (uint32_t i = 0; i < 14; i++) {
    const schema::SceneryViewMask record = ViewMaskAt(base, i);
    if (record.view() != view) {
      continue;
    }
    if (PassAt(base, record.camera_pass()).enabled() != 0) {
      mask |= record.bits();
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
  const Manager m(base, manager);
  const Viewport v(base, viewport);
  const schema::CourseSet set = m.course_set();
  const double far = v.far_distance();
  const uint32_t file = set.area_file().address;
  const double far_plus = float(far + Lfs(base, kAreaMargin));
  const uint32_t count = std::byteswap(schema::SceneryDataFile(base, file).count());  // little-endian
  if (count == 0) {
    return r3;
  }
  const double half = Lfs(base, kHalf);
  const double zero = double(G::zero(base));
  const double none = Lfs(base, kNoDistance);
  for (uint32_t area = 0; area < count; area++) {
    const uint32_t entry = game_data::GetEntry(base, file, area, false);
    const uint32_t corners = FileEntry(base, entry).corners_address();
    r3 = copy;
    const double eye_x = v.eye_x();
    const double eye_z = v.eye_z();
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
    r3 = ListAdd(base, m.list_a_pool_address(), nearest, area, 0, ViewMask(base, view), manager);
  }
  return r3;
}

uint32_t CollectModels(uint8_t* base, uint32_t stack, uint32_t manager, uint32_t view,
                       uint32_t viewport, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_820F32D0 Scenery::CollectModels");
  const uint32_t copy = stack - 320 + 80;
  const Manager m(base, manager);
  const Viewport v(base, viewport);
  const List list_a(base, m.list_a_pool_address());
  const schema::CourseSet set = m.course_set();
  const double far_plus = float(v.far_distance() + Lfs(base, kModelMargin));
  const uint32_t file = set.area_file().address;
  if (list_a.count() == 0) {
    return r3;
  }
  const double zero = double(G::zero(base));
  const double none = Lfs(base, kNoDistance);
  for (uint32_t i = 0; i < list_a.count(); i++) {
    const Item item(base, Load(base, list_a.pointers() + 4 * i));
    const uint32_t area = item.area();
    const uint32_t entry = game_data::GetEntry(base, file, area, false);
    r3 = entry;
    // The area's models: a big-endian count at +0, 96-byte records from +96
    // (+0 the model's number, +32 its four corners).
    const uint32_t models = FileEntry(base, entry).count();
    uint32_t record = FileEntry(base, entry).model_number_address();
    for (uint32_t k = models; k != 0; k--, record += 96) {
      const uint32_t number = Load(base, record);
      const uint32_t corners = record + 32;
      r3 = copy;
      const double eye_x = v.eye_x();
      const double eye_z = v.eye_z();
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
      r3 = ListAdd(base, m.list_b_pool_address(), nearest, area, number, mask, manager);
    }
  }
  return r3;
}

uint32_t Update(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t manager,
                const UpdateRegisters& registers, bool native_queue) {
  ITC_NATIVE_RUNNING("sub_820F4558 Scenery::Update");
  constexpr uint32_t kCameraPassUpdate = 0x820A3978;
  const uint32_t frame = stack - 208;
  guest::StoreU32(base, frame, stack);
  const Manager m(base, manager);
  const uint32_t list_a = m.list_a_pool_address(), list_b = m.list_b_pool_address();
  const double far = Viewport(base, ViewportOf(base, 0)).far_distance();
  // The camera passes the table names, set from their viewports when enabled.
  for (uint32_t i = 0; i < 14; i++) {
    const schema::SceneryViewMask record = ViewMaskAt(base, i);
    const uint32_t pass = PassAt(base, record.camera_pass()).address;
    if (schema::CameraPass(base, pass).enabled() == 0) {
      continue;
    }
    cameras::FromViewport(base, pass, record.view(), record.viewport_entry());
    const uint32_t from_viewport = frame - 112;
    guest::StoreU32(base, from_viewport, frame);
    const uint32_t args[1] = {pass};
    calls.call(calls.user, kCameraPassUpdate, args, 1, from_viewport, 0x820AA534);
  }
  ListClear(base, list_a);
  ListClear(base, list_b);
  // A list sorted and cut to its cap when above 1 item; `limit` before the
  // cut (list B's LimitLargeModels).
  auto sort_and_cut = [&](uint32_t list, uint32_t cap_at, const auto& limit) {
    const List l(base, list);
    const uint32_t count = l.count();
    if (count <= 1) {
      return;
    }
    ListSort(base, list, 0, int32_t(count - 1));
    limit();
    const uint32_t sorted = l.count();
    const uint32_t cap = Load(base, cap_at);
    l.set_count(sorted > cap ? cap : sorted);
  };
  for (int32_t view = 0; view < int32_t(m.view_count_minus_1() + 1); view++) {
    CollectAreas(base, frame, manager, uint32_t(view),
                 ViewportOf(base, uint32_t(view)), manager);
  }
  sort_and_cut(list_a, m.area_count_streamed_address(), [] {});
  uint32_t views = 0;
  for (; int32_t(views) < int32_t(m.view_count_minus_1() + 1); views++) {
    CollectModels(base, frame, manager, views,
                  ViewportOf(base, views), manager);
  }
  sort_and_cut(list_b, m.model_cap_address(), [&] {
    // What the original's stack holds where LimitLargeModels' table reads
    // past its five words: CollectModels' saves of r25 and r26, then
    // LimitLargeModels' own __savegprlr_27 (r27 to r31, the link register).
    StoreDoubleword(base, frame - 64, registers.r25);
    StoreDoubleword(base, frame - 56, registers.r26);
    StoreDoubleword(base, frame - 48, 0xFFFFFFFF826C0000ull);
    StoreDoubleword(base, frame - 40, 0xFFFFFFFF826BAC00ull);
    StoreDoubleword(base, frame - 32, uint64_t(4) * views);
    StoreDoubleword(base, frame - 24, views);
    StoreDoubleword(base, frame - 16, registers.r3);
    guest::StoreU32(base, frame - 8, 0x820F46D0);
    LimitLargeModels(base, manager, frame);
  });
  uint32_t r3;
  if (m.mode() == 0) {
    ListLoadAll(base, calls, frame, manager, list_a, native_queue);
    r3 = ListLoadAll(base, calls, frame, manager, list_b, native_queue);
  } else {
    ListStream(base, calls, frame, manager, list_a, native_queue);
    r3 = ListStream(base, calls, frame, manager, list_b, native_queue);
  }
  // The visible areas: list A's loaded items with a mask, with list B's
  // loaded models of the same area within the far distance.
  const List a(base, list_a), b(base, list_b);
  m.set_visible_count(0);
  for (uint32_t i = 0; i < a.count(); i++) {
    const Item item(base, Load(base, a.pointers() + 4 * i));
    const VisibleArea entry(base, m.visible_count() * VisibleArea::kSize + m.visible_areas().address);
    r3 = item.object().address;
    if (r3 == 0) {
      continue;
    }
    r3 = IsLoaded(base, r3);
    if (r3 == 0) {
      continue;
    }
    const uint32_t mask = item.mask();
    if (mask == 0) {
      continue;
    }
    entry.set_mask(mask);
    entry.set_model_count(0);
    entry.set_lights(item.object().address);
    const uint32_t models = b.count();
    for (uint32_t n = 0; n < models; n++) {
      const Item model(base, Load(base, b.pointers() + 4 * n));
      if (model.mask() == 0) {
        continue;
      }
      if (far < model.distance()) {
        break;
      }
      if (model.area() != item.area()) {
        continue;
      }
      r3 = model.object().address;
      if (r3 == 0) {
        continue;
      }
      r3 = IsLoaded(base, r3);
      if (r3 == 0) {
        continue;
      }
      r3 = model.object().address;
      SetVisible(base, r3, 1);
      const uint32_t count = entry.model_count();
      guest::StoreU32(base, entry.models_address() + 4 * count, model.object().address);
      entry.set_model_count(entry.model_count() + 1);
    }
    if (entry.model_count() != 0) {
      r3 = item.object().address;
      SetVisible(base, r3, 1);
      m.set_visible_count(m.visible_count() + 1);
    }
  }
  if (m.mode() != 0) {
    ReleaseUnused(base, calls, frame, manager, list_a);
    r3 = ReleaseUnused(base, calls, frame, manager, list_b);
  }
  return r3;
}

namespace {

using ListGlobals = schema::SceneryListsGlobals;
constexpr uint32_t kViewCopy = ListGlobals::view_address();  // the camera pass's view
constexpr uint32_t kTextureProviderSet = 0x82594718;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kDrawShape = 0x82147D88;
constexpr uint32_t kPointListDraw = 0x82148058;
constexpr uint32_t kFrameBytes = 384;

uint32_t GuestCall(const scenery_lists::DrawCalls& calls, uint32_t frame, uint32_t lr,
                   uint32_t function, std::initializer_list<uint32_t> args) {
  return calls.call(calls.user, function, args.begin(), int(args.size()), frame, lr);
}

}  // namespace

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

uint32_t DrawSetListsViewProjection(uint8_t* base, uint32_t pass) {
  return PassAt(base, PassDesc(base, pass).camera_pass()).view_projection_address();
}

uint32_t DrawSetLists(uint8_t* base, const scenery_lists::DrawCalls& calls, uint32_t stack,
                      uint32_t manager, uint32_t pass) {
  ITC_NATIVE_RUNNING("sub_820F52A8 Scenery::DrawSetLists");
  const uint32_t frame = stack - kFrameBytes;
  guest::StoreU32(base, frame, stack);  // the back chain, as stwu
  const Manager m(base, manager);
  const PassDesc p(base, pass);
  const schema::CourseSet s = m.course_set();
  const schema::CameraPass camera_pass = PassAt(base, p.camera_pass());
  const uint32_t camera = camera_pass.address;
  const uint32_t provider = m.texture_provider_address();
  const uint32_t textures = s.texture_animations().address;
  // The set's ModelLists drawn here (its entries 4, 5 and 6).
  const uint32_t list_a = s.list_c_count_address(), list_b = s.list_d_count_address(),
                 list_c = s.list_e_count_address();
  const uint32_t points = s.points_address();
  // The provider's slot 1 = the set's XPR.
  const uint32_t set_slot = m.texture_provider().set();
  if (set_slot != kTextureProviderSet ||
      !scenery_lists::TextureProviderSet(base, provider, 1, s.xpr_address())) {
    GuestCall(calls, frame, 0x820F5340, set_slot, {provider, 1, s.xpr_address()});
  }
  G::set_key_a(base, p.key_a());
  G::set_key_b(base, p.key_b());
  matrix_math::Copy(base, kViewCopy, camera_pass.view_address());
  interpolation::BlendState_OnCopy(base, kViewCopy, camera_pass.view_address(), 0x820F5368);
  uint32_t r3 = kViewCopy;
  const uint32_t view_projection = camera_pass.view_projection_address();  // the original's r24
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
  const double zero = double(G::zero(base));
  const double far = double(ListGlobals::point_range_far(base));
  const double near = double(ListGlobals::point_range_near(base));
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
  for (uint32_t i = 0; int32_t(i) < int32_t(schema::ModelList(base, list_a).count()); i++) {
    const uint32_t model = ListModel(base, list_a + 4, i);
    schema::SceneryModel(base, model).set_texture_provider(provider);
    schema::SceneryModel(base, model).set_animations(textures);
    draw(model, frame + 176, 0x820F5454);
  }
  prepare(0x820F54A8);
  for (uint32_t i = 0; int32_t(i) < int32_t(schema::ModelList(base, list_b).count()); i++) {
    r3 = scenery_lists::IsBEntryOn(base, int32_t(i));
    if (r3 == 0) {
      continue;
    }
    const uint32_t model = ListModel(base, list_b + 4, i);
    scenery_lists::ResetDrawState(base, model);
    schema::SceneryModel(base, model).set_texture_provider(provider);
    schema::SceneryModel(base, model).set_animations(textures);
    draw(model, frame + 160, 0x820F5550);
  }
  if (int32_t(schema::ModelList(base, list_c).count()) > 0) {
    prepare(0x820F55B0);
    for (uint32_t i = 0; int32_t(i) < int32_t(schema::ModelList(base, list_c).count()); i++) {
      r3 = scenery_lists::IsCEntryOn(base, i);
      if (r3 == 0) {
        continue;
      }
      const uint32_t model = ListModel(base, list_c + 4, i);
      scenery_lists::ResetDrawState(base, model);
      schema::SceneryModel(base, model).set_texture_provider(provider);
      schema::SceneryModel(base, model).set_animations(textures);
      draw(model, frame + 144, 0x820F5658);
    }
  }
  if (int32_t(s.points_count()) > 0) {
    prepare(0x820F56B8);
    for (uint32_t i = 0; int32_t(i) < int32_t(s.points_count()); i++) {
      r3 = scenery_lists::PointListIsEntryOn(base, int32_t(i));
      if (r3 == 0) {
        continue;
      }
      const schema::RaceProgress race = schema::RaceStatesGlobals::progress(base);
      s.set_points_provider(provider);
      s.set_points_animations(textures);
      // Entry 0's lap record's lap time count (76 bytes a record).
      s.set_points_race_word(
          Load(base, race.e0_lap_record() * 76u + race.e0_lap_time_count_address()));
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
