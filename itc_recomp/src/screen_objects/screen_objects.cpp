// The race's screen objects (see screen_objects.h), store for store as the
// recompiled functions. The float steps are the PowerPC code's: std::fma
// where it has fmadd, each frsp as a float conversion, no other contraction
// (this file builds with -ffp-model=strict and -ffp-contract=off:
// CMakeLists.txt); the constants are read from the image, as the originals
// do.

#include "screen_objects/screen_objects.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "graphics/video_output.h"
#include "guest.h"
#include "schema/screen_objects.h"
#include "screen_objects/uv_trees.h"

#include <bit>
#include <cmath>
#include <cstdlib>

namespace screen_objects {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void StoreDouble(uint8_t* base, uint32_t address, double value) {
  const uint64_t bits = std::bit_cast<uint64_t>(value);
  Store(base, address, uint32_t(bits >> 32));
  Store(base, address + 4, uint32_t(bits));
}
double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>((uint64_t(Load(base, address)) << 32) | Load(base, address + 4));
}
double LoadSingle(uint8_t* base, uint32_t address) {
  return double(std::bit_cast<float>(Load(base, address)));
}
uint32_t SingleBits(double value) { return std::bit_cast<uint32_t>(float(value)); }
double Round(double value) { return double(float(value)); }

using G = schema::ScreenObjectsGlobals;

// AddRingPoint's frame (272 bytes) and its temporaries in it.
constexpr uint32_t kAddFrame = 272;
constexpr uint32_t kValue = 96;       // {y key, u, v}
constexpr uint32_t kColumnA = 128;    // the new column
constexpr uint32_t kPairA = 144;      // {x key, a copy of the column}
constexpr uint32_t kPairB = 176;      // {x key, a copy of that one}
constexpr uint32_t kRingsFrame = 272;

}  // namespace

std::optional<uint32_t> AddRingPoint(uint8_t* base, uint32_t object, double x, double y,
                                     double u, double v, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82132778 ScreenObject::AddRingPoint");
  const uint32_t map = schema::ScreenObject(base, object).uv_map_address();
  const schema::ScreenObjectUvMap m(base, map);
  if (!CanAllocate(base) || m.size() >= kMapMaxSize) {
    return std::nullopt;
  }
  const double thousand = LoadDouble(base, G::thousand_address());
  const double half = LoadDouble(base, G::half_double_address());
  const double thousandth = LoadDouble(base, G::thousandth_address());
  const double key_x = c_runtime::Floor(std::fma(x, thousand, half)) * thousandth;
  const double key_y = c_runtime::Floor(std::fma(y, thousand, half)) * thousandth;
  const auto found = MapFind(base, map, key_x);
  if (!found) {
    return std::nullopt;
  }
  const uint32_t frame = stack - kAddFrame;
  const uint32_t value = frame + kValue;
  const auto store_value = [&] {
    StoreDouble(base, value, key_y);
    Store(base, value + 8, SingleBits(u));
    Store(base, value + 12, SingleBits(v));
  };
  if (found->node == m.head().address) {
    // x's column isn't there: one holding (y, u, v), copied twice into the
    // pair the map copies again, the three destroyed after.
    const uint32_t column_a = frame + kColumnA;
    const uint32_t pair_a = frame + kPairA;
    const uint32_t pair_b = frame + kPairB;
    ColumnConstruct(base, column_a);
    store_value();
    if (!ColumnInsertUnique(base, column_a, value)) {
      ITC_LOG_ERROR("ScreenObject::AddRingPoint: a new column refused its point");
      std::abort();
    }
    StoreDouble(base, pair_a, key_x);
    ColumnConstruct(base, pair_a + 8);
    ColumnCopy(base, pair_a + 8, column_a);
    StoreDouble(base, pair_b, LoadDouble(base, pair_a));
    ColumnConstruct(base, pair_b + 8);
    ColumnCopy(base, pair_b + 8, pair_a + 8);
    if (!MapInsertUnique(base, map, pair_b)) {
      ITC_LOG_ERROR("ScreenObject::AddRingPoint: the map refused a column (a broken tree)");
      std::abort();
    }
    uint32_t r3 = 0;
    for (const uint32_t column : {pair_b + 8, pair_a + 8, column_a}) {
      const auto head = ColumnDestroy(base, column);
      if (!head) {
        ITC_LOG_ERROR("ScreenObject::AddRingPoint: a temporary column not destroyed");
        std::abort();
      }
      r3 = *head;
    }
    return r3;
  }
  // x's column: (y, u, v) added unless y is there.
  const uint32_t column = schema::ScreenObjectUvMapNode(base, found->node).column_address();
  const schema::ScreenObjectUvColumn c(base, column);
  const auto at = ColumnFind(base, column, key_y);
  if (!at || c.size() >= kColumnMaxSize) {
    return std::nullopt;
  }
  if (at->node != c.head().address) {
    return frame + 80;  // Find's iterator out
  }
  store_value();
  // Nothing is changed before a refusal here (the decrement's trap).
  if (!ColumnInsertUnique(base, column, value)) {
    return std::nullopt;
  }
  return frame + 112;  // InsertUnique's result out
}

std::optional<uint32_t> BuildRings(uint8_t* base, uint32_t object, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821329B8 ScreenObject::BuildRings");
  const schema::ScreenObject obj(base, object);
  if (!CanAllocate(base) || schema::ScreenObjectUvMap(base, obj.uv_map_address()).size() != 0) {
    return std::nullopt;
  }
  uint32_t r3 = 0;
  const uint32_t frame = stack - kRingsFrame;
  const double half = LoadSingle(base, G::half_address());
  const double degree = LoadSingle(base, G::degree_address());
  const double step = LoadSingle(base, G::ring_step_address());
  // One point: its vertex appended, then AddRingPoint.
  const auto point = [&](double angle, double radius) {
    const double u = Round(std::fma(Round(c_runtime::Cos(angle)), radius, half));
    const double v = Round(std::fma(Round(c_runtime::Sin(angle)), radius, half));
    const double width = Round(double(int64_t(obj.width())));
    const double height = Round(double(int64_t(obj.height())));
    const double px = Round(width * u);
    const double py = Round(height * v);
    const uint32_t count = obj.ring_vertex_count();
    const uint32_t vertex = obj.ring_vertices_address() + count * 20;
    Store(base, vertex + 0, SingleBits(px));
    Store(base, vertex + 4, SingleBits(py));
    Store(base, vertex + 8, 0xFFFFFFFF);
    Store(base, vertex + 12, SingleBits(u));
    Store(base, vertex + 16, SingleBits(v));
    obj.set_ring_vertex_count(count + 1);
    const auto added = AddRingPoint(base, object, px, py, u, v, frame);
    if (!added) {
      ITC_LOG_ERROR("ScreenObject::BuildRings: AddRingPoint refused a point after its checks");
      std::abort();
    }
    r3 = *added;
  };
  for (uint32_t ring = 0; ring < 20; ring++) {
    const double inner = Round(Round(double(int64_t(ring))) * step);
    const double outer = Round(Round(double(int64_t(ring + 1))) * step);
    for (uint32_t degrees = 0; degrees < 360; degrees += 10) {
      const double angle = Round(Round(double(int64_t(degrees))) * degree);
      point(angle, inner);
      point(angle, outer);
    }
  }
  return r3;
}

bool SetSpeed(uint8_t* base, uint32_t object, double strength) {
  ITC_NATIVE_RUNNING("sub_8212FCC8 ScreenObject::SetSpeed");
  const schema::ScreenObject obj(base, object);
  if (obj.running() != 0) {
    return false;
  }
  const double low = LoadSingle(base, G::zero_address());
  const double high = LoadSingle(base, G::one_address());
  obj.set_speed_set(1);
  if (strength > high) {
    strength = high;
  } else if (strength < low) {
    strength = low;
  }
  Store(base, obj.strength_address(), SingleBits(strength));
  return true;
}

uint32_t Release(uint8_t* base, uint32_t object, uint32_t frame, const ReleaseCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8212F0B8 ScreenObject::Release");
  namespace vo = graphics::video_output;
  const uint32_t local = frame + 80;
  Store(base, local, vo::LockDevice(base, calls.video_output(0)));
  const schema::ScreenObject obj(base, object);
  calls.release(obj.vertex_declaration());
  obj.set_vertex_declaration(0);
  calls.release(obj.texture());
  obj.set_texture(0);
  const uint32_t video_output = calls.video_output(1);
  vo::UnlockDevice(base, video_output, local);
  return video_output;
}

uint32_t Destroy(uint8_t* base, uint32_t object, uint32_t frame, const ReleaseCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82132FC8 ScreenObject::~ScreenObject");
  Release(base, object, frame - 128, calls);
  const uint32_t map = schema::ScreenObject(base, object).uv_map_address();
  const auto head = MapDestroy(base, map);
  if (!head) {
    ITC_LOG_ERROR("ScreenObject::~ScreenObject: the map at {:08X} not destroyed", map);
    std::abort();
  }
  return *head;
}

}  // namespace screen_objects
