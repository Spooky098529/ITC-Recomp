// The vehicle code above the car physics (src/vehicle/,
// docs/functions/vehicle.md) against the recompiled functions. The
// originals run with --native_vehicle off (their callees' own flags as they
// are: those were checked by their harnesses), the plain C++ through the
// register steps (hooks/vehicle_native.h). The inputs go in blocks from the
// allocation layer; the course's mesh and route (the collision world's
// harness's), the race data's flags, the fake collision world and the
// vehicle's region (the car physics' harness's) where a function's callees
// read them, with the game's globals put back after. Compared with
// harness::Outputs: floats only where the code loads and stores floats,
// heap blocks by their number.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_physics/car_physics.h"
#include "collision/collision.h"
#include "harnesses/test_route.h"
#include "hooks/heap.h"
#include "hooks/vehicle_native.h"
#include "memory/heap.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"
#include "vehicle/vehicle.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_vehicle);

namespace harness {

// The collision world's harness's course (src/harnesses/collision_world.cpp).
uint32_t TestCourseBytes();
void FillTestCourseTable(uint8_t* base, uint32_t area, uint32_t table);
void BuildTestCourse(uint8_t* base, uint32_t area, uint32_t table, Random& r);

// The car physics' harness's world and vehicle
// (src/harnesses/car_physics_objects.cpp).
namespace car_objects {
uint32_t WorldBytes();
uint32_t WorldActiveObject(uint8_t* base, uint32_t region);
uint32_t VehicleRegionBytes();
void BuildWorld(Context& h, uint32_t region, Random& r, bool keep_one_free = false);
void BuildBox(Context& h, uint32_t box, Random& r);
void FillVehicleRegion(uint8_t* base, uint32_t region, Random& r, bool nans);
void FillStepInputs(uint8_t* base, uint32_t region, Random& r);
void VehicleRegionOutputs(Outputs& out, uint8_t* base, uint32_t region);
void FillStack(Context& h, Random& r);
}  // namespace car_objects

// Vehicle::SetupFromCar's cases' writer (src/harnesses/car_physics.cpp).
bool SetupFromCarFits(uint8_t* base, uint32_t car, uint32_t r1);
void WriteSetupFromCarCase(uint8_t* base, uint32_t data, uint32_t car, uint32_t r1,
                           uint32_t stack_end, uint64_t seed, int group);

namespace {

namespace vh = vehicle;

constexpr uint32_t kWorldPointer = 0x826C2D88;
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kLinkedTried = 0x826C5B04, kLinkedTriedBytes = 16;

// A block of guest memory for the cases, freed at the end.
struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// The original with --native_vehicle off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_vehicle) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_vehicle) = true;
}

// A float's bits: now and then NaN (any sign, payload), an infinity, a zero
// of either sign, a denormal; else a number.
uint32_t RandomFloat(Random& r, float range = 10.0f) {
  switch (r.Below(16)) {
    case 0:
      return 0x7F800001u | (uint32_t(r.Next()) & 0x807FFFFFu);
    case 1:
      return r.Below(2) ? 0x7F800000u : 0xFF800000u;
    case 2:
      return r.Below(2) ? 0u : 0x80000000u;
    case 3:
      return (uint32_t(r.Next()) & 0x807FFFFFu) | 1u;
    default:
      return std::bit_cast<uint32_t>((float(r.Below(20001)) / 10000.0f - 1.0f) * range);
  }
}

// Heap blocks named by their place in `given` (the two sides allocate at
// different addresses).
struct Blocks {
  std::vector<uint32_t> given, made;
  uint32_t Name(uint32_t w) const {
    for (size_t i = 0; i < given.size(); i++) {
      if (w == given[i] && w != 0) {
        return uint32_t(0xB10C0000u + i);
      }
    }
    for (size_t i = 0; i < made.size(); i++) {
      if (w == made[i]) {
        return uint32_t(0x0E550000u + i);
      }
    }
    return w;
  }
  // A block the call made (a heap block not given).
  void Note(uint32_t w) {
    if (w != 0 && memory::IsHeapBlock(w) &&
        std::find(given.begin(), given.end(), w) == given.end() &&
        std::find(made.begin(), made.end(), w) == made.end()) {
      made.push_back(w);
    }
  }
  void FreeAll() {
    for (uint32_t b : given) {
      if (b != 0 && memory::IsHeapBlock(b)) {
        memory::Free(b);
      }
    }
    for (uint32_t b : made) {
      if (memory::IsHeapBlock(b)) {
        memory::Free(b);
      }
    }
  }
};

// --- The leaves ---

void Leaves(Context& h) {
  {
    Results results("Matrix::FromRows (sub_820BFBB8)");
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 31 + 7);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        PPCContext t = CallContext(h);
        // The rows anywhere in the scratch area (out may be one of them).
        t.r3.u64 = h.data + 4 * r.Below(256);
        t.r4.u64 = h.data + 4 * r.Below(256);
        t.r5.u64 = h.data + 4 * r.Below(256);
        t.r6.u64 = h.data + 4 * r.Below(256);
        t.r7.u64 = h.data + 4 * r.Below(256);
        (v == 0 ? Original<__imp__sub_820BFBB8> : hooks::VehicleMatrixFromRows)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("TrackPosition::Copy (sub_820C2150)");
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 37 + 11);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 8 * r.Below(256);
        t.r4.u64 = h.data + 2048 + 8 * r.Below(200);
        (v == 0 ? Original<__imp__sub_820C2150> : hooks::VehicleTrackPositionCopy)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // The game's four curves (the table at 0x82600EE8 ends where the
    // surface tables at 0x826014A0 start).
    Results results("CameraCurve::Load (sub_820BFCE8)");
    for (uint64_t seed = 0; seed < 40; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 41 + 13);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 4 * r.Below(256);
        t.r4.u64 = seed % 4;
        t.r5.u64 = r.Next();
        (v == 0 ? Original<__imp__sub_820BFCE8> : hooks::VehicleCameraCurveLoad)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- RoutePosition::Step: the course's route (test_route.h) installed at
// 0x826C5B10 and the race data's flags (its +20) for BranchMasksFor ---

void Steps(Context& h) {
  namespace tr = test_route;
  const Block area(tr::kRouteBytes + 0x100);
  const Block table(4 * tr::kTableEntries);
  const Block race(0x40);
  if (!area.address || !table.address || !race.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  tr::FillTable(h.base, table.address, area.address);
  const uint32_t saved_route = guest::LoadU32(h.base, track_route::kRoute);
  const uint32_t saved_race = guest::LoadU32(h.base, kRaceData);
  Results results("RoutePosition::Step (sub_82170F90)");
  const uint32_t flags[] = {0, 0x4, 0x8, 0x10, 0x20, 0x3C};
  for (uint64_t seed = 0; seed < 400; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 43 + 17);
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, area.address, area.bytes, r);
      const tr::Route route = tr::BuildRoute(h.base, area.address, table.address, r);
      guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
      WriteRandom(h.base, race.address, race.bytes, r);
      guest::StoreU32(h.base, race.address + 20,
                      r.Below(4) == 0 ? uint32_t(r.Next()) : flags[r.Below(6)]);
      guest::StoreU32(h.base, kRaceData, race.address);
      const uint32_t position = h.data + 4 * r.Below(256);
      tr::RandomPosition(h.base, position, route, r);
      PPCContext t = CallContext(h);
      t.r3.u64 = position;
      const int32_t steps[] = {-1, 1, 0, 2, -3};
      t.r4.u64 = uint64_t(int64_t(r.Below(4) == 0 ? int32_t(r.Below(40)) - 20 : steps[r.Below(5)]));
      t.r5.u64 = r.Below(4) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(12)) - 1));
      (v == 0 ? Original<__imp__sub_82170F90> : hooks::VehicleRoutePositionStep)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Bytes(Read(h.base, area.address, area.bytes));
      // The kind on the stack (its frame + 80).
      out[v].Word(guest::LoadU32(h.base, CallContext(h).r1.u32 - 112 + 80));
      out[v].Word(t.r3.u32);
      guest::StoreU32(h.base, track_route::kRoute, saved_route);
      guest::StoreU32(h.base, kRaceData, saved_race);
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// --- The track positions: the course's mesh and route (the collision
// world's harness's) ---

// A track position's 96 bytes: the cell and route position, +8 to +15 and
// +64 to +95 exactly, the point, color and normal (+16 to +63) floats.
void PositionOutputs(Outputs& out, uint8_t* base, uint32_t position) {
  out.Bytes(Read(base, position, 16));
  out.Floats(base, position + 16, 12);
  out.Bytes(Read(base, position + 64, 32));
}

// A track position on the course: the cell and route position the point's
// slice, near it, the other section's or off the course; a point over it.
void CoursePosition(uint8_t* base, uint32_t position, uint32_t point, Random& r) {
  const float x = float(int32_t(r.Below(2000)) - 1000) / 100.0f;
  const float y = float(r.Below(1000)) / 1000.0f + float(r.Below(3)) * 0.5f;
  const float z = float(r.Below(5900)) / 100.0f + 0.05f;
  guest::StoreFloat(base, point + 0, x);
  guest::StoreFloat(base, point + 4, y);
  guest::StoreFloat(base, point + 8, z);
  const int32_t section = z >= 30.0f ? 1 : 0;
  int32_t slice = std::clamp(int32_t(z / 5.0f) - 6 * section, 0, 5);
  int32_t cell_section = section, cell_slice = slice;
  int32_t route_section = section, route_slice = slice;
  switch (r.Below(5)) {
    case 0:
      cell_slice = std::clamp(slice + int32_t(r.Below(3)) - 1, 0, 5);
      break;
    case 1:
      route_slice = std::clamp(slice + int32_t(r.Below(3)) - 1, 0, 5);
      break;
    case 2:
      cell_section = 1 - section;
      cell_slice = int32_t(r.Below(6));
      break;
    default:
      break;
  }
  guest::StoreU16(base, position + 0, uint16_t(cell_section));
  guest::StoreU16(base, position + 2, uint16_t(cell_slice));
  guest::StoreU16(base, position + 4, uint16_t(route_section));
  guest::StoreU16(base, position + 6, uint16_t(route_slice));
  guest::StoreFloat(base, position + 28, 1.0f);
  guest::StoreU16(base, position + 80, uint16_t(int32_t(r.Below(8)) - 2));
}

void TrackPositions(Context& h) {
  const Block area(TestCourseBytes());
  const Block table(4 * 0x10000);
  const Block vehicle(vh::kVehicleBytes + 0x300);  // the body at +6272
  if (!area.address || !table.address || !vehicle.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  FillTestCourseTable(h.base, area.address, table.address);
  const uint32_t saved_route = guest::LoadU32(h.base, track_route::kRoute);
  uint8_t saved_mesh[12], saved_tried[kLinkedTriedBytes];
  std::memcpy(saved_mesh, h.base + track_collision::kMesh, sizeof(saved_mesh));
  std::memcpy(saved_tried, h.base + kLinkedTried, kLinkedTriedBytes);
  Results results("Vehicle::UpdateTrackPositions (sub_820C1D78)");
  const uint32_t v_ = vehicle.address, body = v_ + vh::kVehicleBytes;
  for (uint64_t seed = 0; seed < 200; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 47 + 19);
      WriteRandom(h.base, area.address, area.bytes, r);
      BuildTestCourse(h.base, area.address, table.address, r);
      WriteRandom(h.base, vehicle.address, vehicle.bytes, r);
      guest::StoreU32(h.base, v_ + 336, body);
      CoursePosition(h.base, v_ + 5232, body + 208, r);
      for (uint32_t i = 0; i < 4; i++) {
        CoursePosition(h.base, v_ + 5328 + 96 * i, v_ + 480 + 368 * i, r);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = v_;
      (v == 0 ? Original<__imp__sub_820C1D78> : hooks::VehicleUpdateTrackPositions)(t, h.base);
      PositionOutputs(out[v], h.base, v_ + 5232);
      for (uint32_t i = 0; i < 4; i++) {
        PositionOutputs(out[v], h.base, v_ + 5328 + 96 * i);
      }
      out[v].Bytes(Read(h.base, kLinkedTried, kLinkedTriedBytes));
      // (r3 isn't compared: TrackPosition::Update's plain C++ doesn't keep
      // the original's, which no caller reads.)
      std::memcpy(h.base + kLinkedTried, saved_tried, kLinkedTriedBytes);
      std::memcpy(h.base + track_collision::kMesh, saved_mesh, sizeof(saved_mesh));
      guest::StoreU32(h.base, track_route::kRoute, saved_route);
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// --- The vehicle's shape and world ---

// LinkTrackPositions: a collision object whose shape record's template and
// copies are real boxes. Compared: the vehicle, the boxes, r3.
void Links(Context& h) {
  const Block block(vh::kVehicleBytes + 320 + 3 * 496 + 16);
  if (!block.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  const uint32_t v_ = block.address, object = v_ + vh::kVehicleBytes;
  const uint32_t boxes = object + 320;
  Results results("Vehicle::LinkTrackPositions (sub_820C1D08)");
  for (uint64_t seed = 0; seed < 100; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 53 + 23);
      WriteRandom(h.base, block.address, block.bytes, r);
      for (uint32_t k = 0; k < 3; k++) {
        car_objects::BuildBox(h, boxes + 496 * k, r);
      }
      guest::StoreU32(h.base, v_ + 340, object);
      guest::StoreU32(h.base, object + 32 + 0, boxes);
      guest::StoreU32(h.base, object + 32 + 4, boxes + 496 * (1 + r.Below(2)));
      guest::StoreU32(h.base, object + 32 + 8, boxes + 496 * r.Below(3));
      PPCContext t = CallContext(h);
      t.r3.u64 = v_;
      (v == 0 ? Original<__imp__sub_820C1D08> : hooks::VehicleLinkTrackPositions)(t, h.base);
      out[v].Bytes(Read(h.base, block.address, block.bytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// Refresh: the car physics' fake world and vehicle region (as its
// RefreshFromBody cases: the wheels' track positions 0). Compared: the
// region (its outputs), r3, f1.
void Refreshes(Context& h, uint32_t world, uint32_t region) {
  for (int group = 0; group < 2; group++) {
    Results results(std::string("Vehicle::Refresh (sub_820C0118)") +
                    (group == 1 ? ", NaNs and infinities anywhere" : ""));
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 59 + 29 + 1000 * uint64_t(group));
        car_objects::BuildWorld(h, world, r);
        car_objects::FillVehicleRegion(h.base, region, r, group == 1);
        for (uint32_t w = 0; w < 4; w++) {
          guest::StoreU32(h.base, region + 416 + 368 * w + 20, 0);
        }
        car_objects::FillStack(h, r);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = region;
        (v == 0 ? Original<__imp__sub_820C0118> : hooks::VehicleRefresh)(t, h.base);
        car_objects::VehicleRegionOutputs(out[v], h.base, region);
        out[v].Word(t.r3.u32);
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// RemoveFromWorld: the fake world, a vehicle whose +340 is 0 or an active
// object, +344 0 or a box from the layer. Compared: the world (exactly), the
// vehicle's +336 to +347 and +6148, whether the box was freed, r3.
void Removals(Context& h, uint32_t world) {
  const Block block(vh::kVehicleBytes);
  if (!block.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  const uint32_t v_ = block.address;
  Results results("Vehicle::RemoveFromWorld (sub_820C20E0)");
  for (uint64_t seed = 0; seed < 200; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 61 + 31);
      car_objects::BuildWorld(h, world, r);
      WriteRandom(h.base, v_, block.bytes, r);
      Blocks blocks;
      uint32_t shape = 0, object = 0;
      if (r.Below(4) != 0) {
        shape = memory::Alloc(496, 0);
        car_objects::BuildBox(h, shape, r);
        blocks.given.push_back(shape);
      }
      const uint32_t active = car_objects::WorldActiveObject(h.base, world);
      if (active != 0 && r.Below(3) != 0) {
        object = active;
        const uint32_t next = guest::LoadU32(h.base, active + 228);
        if (next != 0 && r.Below(2) == 0) {
          object = next;
        }
      }
      guest::StoreU32(h.base, v_ + 340, object);
      guest::StoreU32(h.base, v_ + 344, shape);
      PPCContext t = CallContext(h);
      t.r3.u64 = v_;
      (v == 0 ? Original<__imp__sub_820C20E0> : hooks::VehicleRemoveFromWorld)(t, h.base);
      out[v].Bytes(Read(h.base, world, car_objects::WorldBytes()));
      out[v].Word(guest::LoadU32(h.base, v_ + 336));
      out[v].Word(blocks.Name(guest::LoadU32(h.base, v_ + 340)));
      out[v].Word(blocks.Name(guest::LoadU32(h.base, v_ + 344)));
      out[v].Word(guest::LoadU32(h.base, v_ + 6148));
      for (uint32_t b : blocks.given) {
        out[v].Word(memory::IsHeapBlock(b) ? 1 : 0);
      }
      out[v].Word(blocks.Name(t.r3.u32));
      blocks.FreeAll();
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// --- The seven vehicles ---

// Create: the block made (its bytes, all zeroed), [0x826C2E34] and r3 as
// the block's number. Destroy: seven vehicles in a block from the layer,
// each with a body (0 or a block), a collision object (0 or one of the
// fake world's active ones, each at most once) and a shape (0 or a box);
// [0x826C2E34] the block or 0. Compared: the world, which blocks were
// freed, [0x826C2E34].
void Lifetime(Context& h, uint32_t world) {
  const uint32_t saved = guest::LoadU32(h.base, vh::kVehicles);
  {
    Results results("Vehicles::Create (sub_820C3BD0)");
    for (uint64_t seed = 0; seed < 8; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 67 + 37);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        PPCContext t = CallContext(h);
        (v == 0 ? Original<__imp__sub_820C3BD0> : hooks::VehiclesCreate)(t, h.base);
        const uint32_t made = guest::LoadU32(h.base, vh::kVehicles);
        out[v].Word(made != 0 && memory::IsHeapBlock(made) ? 1 : 0);
        out[v].Word(t.r3.u32 == made ? 1 : 0);
        if (made != 0) {
          out[v].Bytes(Read(h.base, made, vh::kVehiclesBytes));
          memory::Free(made);
        }
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        guest::StoreU32(h.base, vh::kVehicles, saved);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("Vehicles::Destroy (sub_820C3B78)");
    for (uint64_t seed = 0; seed < 60; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 71 + 41);
        car_objects::BuildWorld(h, world, r);
        Blocks blocks;
        const uint32_t vehicles = memory::Alloc(vh::kVehiclesBytes, 0);
        blocks.given.push_back(vehicles);
        WriteRandom(h.base, vehicles, vh::kVehiclesBytes, r);
        std::vector<uint32_t> active;
        for (uint32_t o = car_objects::WorldActiveObject(h.base, world); o != 0;
             o = guest::LoadU32(h.base, o + 228)) {
          active.push_back(o);
        }
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t vehicle = vehicles + vh::kVehicleBytes * i;
          uint32_t body = 0, shape = 0, object = 0;
          if (r.Below(4) != 0) {
            body = memory::Alloc(528, 0);
            WriteRandom(h.base, body, 528, r);
            blocks.given.push_back(body);
          }
          if (r.Below(4) != 0) {
            shape = memory::Alloc(496, 0);
            car_objects::BuildBox(h, shape, r);
            blocks.given.push_back(shape);
          }
          if (!active.empty() && r.Below(2) == 0) {
            const uint32_t k = r.Below(uint32_t(active.size()));
            object = active[k];
            active.erase(active.begin() + k);
          }
          guest::StoreU32(h.base, vehicle + 336, body);
          guest::StoreU32(h.base, vehicle + 340, object);
          guest::StoreU32(h.base, vehicle + 344, shape);
        }
        const bool none = r.Below(8) == 0;
        guest::StoreU32(h.base, vh::kVehicles, none ? 0 : vehicles);
        PPCContext t = CallContext(h);
        (v == 0 ? Original<__imp__sub_820C3B78> : hooks::VehiclesDestroy)(t, h.base);
        out[v].Bytes(Read(h.base, world, car_objects::WorldBytes()));
        for (uint32_t b : blocks.given) {
          out[v].Word(memory::IsHeapBlock(b) ? 1 : 0);
        }
        out[v].Word(guest::LoadU32(h.base, vh::kVehicles));
        blocks.FreeAll();
        guest::StoreU32(h.base, vh::kVehicles, saved);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- The input and the queries ---

// ApplyInput: the car physics' vehicle region (numbers, or NaNs and
// infinities anywhere), a car input record in the scratch area (the kinds
// 0 and 2 mostly; the flags that send it to callees never seen in play now
// and then: the original then runs on both sides). Compared: the region,
// the bytes +2424 and +2249, the frame's +80 to +95, r3, f1.
void Inputs(Context& h, uint32_t region) {
  const uint32_t frame = CallContext(h).r1.u32 - 176;
  const uint8_t kinds[] = {0, 0, 0, 2, 2, 2, 3, 0xFF, 1};
  const int16_t values[] = {0, 32767, -32767, 1, -1, 16000, -32768};
  for (int group = 0; group < 2; group++) {
    Results results(std::string("Vehicle::ApplyInput (sub_820BFD30)") +
                    (group == 1 ? ", NaNs and infinities anywhere" : ""));
    for (uint64_t seed = 0; seed < 300; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 73 + 43 + 1000 * uint64_t(group));
        car_objects::FillVehicleRegion(h.base, region, r, group == 1);
        car_objects::FillStack(h, r);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        const uint32_t input = h.data + 4 * r.Below(128);
        uint32_t flags = 0;
        for (uint32_t bit : {0x02000000u, 0x80000000u, 0x200u, 0x10000u, 0x2000u, 0x1u, 0x2u,
                             0x20000000u, 0x08000000u}) {
          const bool rare = bit == 0x2000u || bit == 0x1u || bit == 0x2u || bit == 0x20000000u ||
                            bit == 0x08000000u;
          if (r.Below(rare ? 16 : 2) == 0) {
            flags |= bit;
          }
        }
        flags |= uint32_t(r.Next()) & 0x0040F000u;  // other bits now and then
        guest::StoreU32(h.base, input + 0, flags);
        for (uint32_t at : {4u, 6u, 8u, 10u}) {
          guest::StoreU16(h.base, input + at,
                          uint16_t(r.Below(2) == 0 ? values[r.Below(7)] : int16_t(r.Next())));
        }
        guest::StoreU8(h.base, input + 17, kinds[r.Below(sizeof(kinds))]);
        guest::StoreU8(h.base, region + 2249, uint8_t(r.Below(4) == 0));
        guest::StoreU8(h.base, region + 2424, uint8_t(r.Below(2)));
        if (r.Below(2) == 0) {
          guest::StoreU32(h.base, region + 2420, r.Below(2) == 0 ? 0 : 0x43C30000u);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = region;
        t.r4.u64 = input;
        (v == 0 ? Original<__imp__sub_820BFD30> : hooks::VehicleApplyInput)(t, h.base);
        car_objects::VehicleRegionOutputs(out[v], h.base, region);
        out[v].Word(guest::LoadU8(h.base, region + 2424));
        out[v].Word(guest::LoadU8(h.base, region + 2249));
        out[v].Bytes(Read(h.base, frame + 80, 16));
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// SlipWheelValue: the vehicle region, the wheels on the ground now and
// then, the threshold's flag and value (0x826C2E3C, 0x826C2E38; put back
// after). Compared: the region, the two globals, the frame's +80 to +159,
// f1.
void Slips(Context& h, uint32_t region) {
  constexpr uint32_t kFlag = 0x826C2E3C, kThreshold = 0x826C2E38;
  const uint32_t saved_flag = guest::LoadU32(h.base, kFlag);
  const uint32_t saved_threshold = guest::LoadU32(h.base, kThreshold);
  const uint32_t frame = CallContext(h).r1.u32 - 240;
  for (int group = 0; group < 2; group++) {
    Results results(std::string("Vehicle::SlipWheelValue (sub_820C0310)") +
                    (group == 1 ? ", NaNs and infinities anywhere" : ""));
    for (uint64_t seed = 0; seed < 300; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 79 + 47 + 1000 * uint64_t(group));
        car_objects::FillVehicleRegion(h.base, region, r, group == 1);
        car_objects::FillStack(h, r);
        for (uint32_t w = 0; w < 4; w++) {
          guest::StoreU8(h.base, region + 433 + 368 * w, uint8_t(r.Below(3) != 0));
        }
        guest::StoreU32(h.base, kFlag, uint32_t(r.Next()) & ~1u | uint32_t(r.Below(2)));
        guest::StoreU32(h.base, kThreshold,
                        r.Below(4) == 0 ? 0 : std::bit_cast<uint32_t>(float(r.Below(300)) / 100.0f));
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = region;
        t.f1.f64 = group == 1 && r.Below(8) == 0 ? double(std::bit_cast<float>(RandomFloat(r)))
                                                : double(r.Below(200)) / 10.0;
        (v == 0 ? Original<__imp__sub_820C0310> : hooks::VehicleSlipWheelValue)(t, h.base);
        car_objects::VehicleRegionOutputs(out[v], h.base, region);
        out[v].Word(guest::LoadU32(h.base, kFlag));
        out[v].Float(guest::LoadU32(h.base, kThreshold));
        out[v].Floats(h.base, frame + 80, 20);
        out[v].Double(t.f1.u64);
        guest::StoreU32(h.base, kFlag, saved_flag);
        guest::StoreU32(h.base, kThreshold, saved_threshold);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// NearestHumanVehicle: a fake race manager (0x826D82F4: the racer count at
// +2364, the racers' records at +2308, each record's +20 its vehicle's
// index) and the vehicles' block (0x826C2E34: each +6148 flags 0, 0x1,
// 0x80, 0x81 or 0x2, +336 a body with its position +208); both put back
// after. Compared: the frame's +80 to +223 (the point, the pairs' floats,
// the vectors as floats, the racers exactly), r3 by the vehicle's index.
void Nearest(Context& h) {
  constexpr uint32_t kManager = 0x826D82F4;
  const Block vehicles(vh::kVehiclesBytes);
  const Block manager(0x1000);
  const Block bodies(7 * 528);
  if (!vehicles.address || !manager.address || !bodies.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  const uint32_t saved_manager = guest::LoadU32(h.base, kManager);
  const uint32_t saved_vehicles = guest::LoadU32(h.base, vh::kVehicles);
  const uint32_t frame = CallContext(h).r1.u32 - 304;
  const uint32_t kinds[] = {0, 0x1, 0x80, 0x81, 0x2};
  for (int group = 0; group < 2; group++) {
    Results results(std::string("Racers::NearestHumanVehicle (sub_820C2248)") +
                    (group == 1 ? ", NaNs and infinities anywhere" : ""));
    for (uint64_t seed = 0; seed < 300; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 83 + 53 + 1000 * uint64_t(group));
        car_objects::FillStack(h, r);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, manager.address, manager.bytes, r);
        const uint32_t count = r.Below(8);
        guest::StoreU32(h.base, manager.address + 2364, count);
        for (uint32_t k = 0; k < 7; k++) {
          const uint32_t record = manager.address + 0xA00 + 32 * k;
          guest::StoreU32(h.base, manager.address + 4 * (577 + k), record);
          guest::StoreU32(h.base, record + 20, r.Below(7));
          const uint32_t vehicle = vehicles.address + vh::kVehicleBytes * k;
          const uint32_t body = bodies.address + 528 * k;
          guest::StoreU32(h.base, vehicle + 6148, kinds[r.Below(5)]);
          guest::StoreU32(h.base, vehicle + 336, body);
          for (uint32_t i = 0; i < 3; i++) {
            guest::StoreU32(h.base, body + 208 + 4 * i,
                            group == 1 ? RandomFloat(r, 100.0f)
                                       : std::bit_cast<uint32_t>(
                                             float(int32_t(r.Below(20001)) - 10000) / 100.0f));
          }
          guest::StoreU32(h.base, body + 220, uint32_t(r.Next()));
        }
        guest::StoreU32(h.base, kManager, manager.address);
        guest::StoreU32(h.base, vh::kVehicles, vehicles.address);
        const uint32_t point = h.data + 16 * r.Below(64);
        for (uint32_t i = 0; i < 4; i++) {
          guest::StoreU32(h.base, point + 4 * i,
                          group == 1 ? RandomFloat(r, 100.0f)
                                     : std::bit_cast<uint32_t>(
                                           float(int32_t(r.Below(20001)) - 10000) / 100.0f));
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = point;
        t.r4.u64 = r.Below(8) == 0 ? r.Below(7) : r.Below(std::max(count, 1u));
        (v == 0 ? Original<__imp__sub_820C2248> : hooks::VehicleNearestHuman)(t, h.base);
        out[v].Floats(h.base, frame + 80, 4);
        for (uint32_t k = 0; k < 8; k++) {
          out[v].Float(guest::LoadU32(h.base, frame + 96 + 8 * k));
          out[v].Word(guest::LoadU32(h.base, frame + 100 + 8 * k));
        }
        out[v].Bytes(Read(h.base, frame + 160, 32));
        out[v].Floats(h.base, frame + 192, 8);
        out[v].Word(t.r3.u32 - vehicles.address);
        guest::StoreU32(h.base, kManager, saved_manager);
        guest::StoreU32(h.base, vh::kVehicles, saved_vehicles);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- The power curve ---

// A curve's 1,064 bytes: the tables' and range's words as floats, the
// peak, count and limits (+1024 to +1039) exactly.
void CurveOutputs(Outputs& out, uint8_t* base, uint32_t curve) {
  out.Floats(base, curve, 256);
  out.Bytes(Read(base, curve + 1024, 16));
  out.Floats(base, curve + 1040, 6);
}

// The stack below `r1` that ShiftPeak writes: its doublewords (r1 - 576 to
// r1 - 537) exactly, the remapped table (r1 - 528 to r1 - 17) as floats.
void ShiftStackOutputs(Outputs& out, uint8_t* base, uint32_t r1) {
  out.Bytes(Read(base, r1 - 576, 40));
  out.Floats(base, r1 - 528, (528 - 16) / 4);
}

// A curve as Build leaves them (rpm rising by 400, torques and powers),
// with its peak and count, now and then odd: a peak past the count, a count
// of 0 or 1, a NaN or an infinity in a torque or an rpm.
void RandomCurve(uint8_t* base, uint32_t curve, Random& r, bool nans) {
  WriteRandom(base, curve, 1064, r);
  const uint32_t count = r.Below(4) == 0 ? r.Below(3) : 1 + r.Below(32);
  for (uint32_t i = 0; i < 32; i++) {
    const uint32_t e = curve + 16 * i;
    guest::StoreFloat(base, e + 0, 400.0f * float(i + 1));
    guest::StoreFloat(base, e + 4, float(r.Below(5000)) / 10.0f);
    guest::StoreFloat(base, e + 8, float(r.Below(5000)) / 10.0f);
    if (nans && r.Below(16) == 0) {
      guest::StoreU32(base, e + 4 * r.Below(2), RandomFloat(r));
    }
  }
  guest::StoreU32(base, curve + 1024, r.Below(8) == 0 ? r.Below(34) : r.Below(std::max(count, 1u)));
  guest::StoreU32(base, curve + 1028, count);
  guest::StoreFloat(base, curve + 1040, 800.0f);
  guest::StoreFloat(base, curve + 1044, 4000.0f + float(r.Below(80)) * 100.0f);
  if (nans && r.Below(8) == 0) {
    guest::StoreU32(base, curve + 1040 + 4 * r.Below(2), RandomFloat(r));
  }
}

// The 33 points Build reads (32 torques of 0 to 1, now and then below
// 0.01, and the scale at +128).
void RandomPoints(uint8_t* base, uint32_t points, Random& r, bool nans) {
  for (uint32_t i = 0; i < 32; i++) {
    const uint32_t k = r.Below(8);
    const float value = k == 0 ? 0.0f : k == 1 ? 0.005f : float(r.Below(1000)) / 1000.0f;
    guest::StoreU32(base, points + 4 * i,
                    nans && r.Below(16) == 0 ? RandomFloat(r) : std::bit_cast<uint32_t>(value));
  }
  guest::StoreFloat(base, points + 128, 100.0f + float(r.Below(4000)) / 10.0f);
}

void Curves(Context& h) {
  const Block block(0x2000);  // the curve at +0x40, a vehicle's car data
  if (!block.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  const uint32_t curve = block.address + 0x40;
  const uint32_t r1 = CallContext(h).r1.u32;
  for (int group = 0; group < 2; group++) {
    const std::string suffix = group == 1 ? ", NaNs and infinities" : "";
    {
      Results results("PowerCurve::ShiftPeak (sub_820CE1D8)" + suffix);
      for (uint64_t seed = 0; seed < 300; seed++) {
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Random r(seed * 89 + 59 + 1000 * uint64_t(group));
          car_objects::FillStack(h, r);
          RandomCurve(h.base, curve, r, group == 1);
          PPCContext t = CallContext(h);
          t.r3.u64 = curve;
          t.r4.u64 = r.Below(8) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(51)) - 25));
          (v == 0 ? Original<__imp__sub_820CE1D8> : hooks::VehicleShiftPeak)(t, h.base);
          CurveOutputs(out[v], h.base, curve);
          ShiftStackOutputs(out[v], h.base, r1);
          out[v].Word(t.r3.u32);
        }
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
      results.Report();
    }
    for (int from_engine = 0; from_engine < 2; from_engine++) {
      Results results(std::string(from_engine ? "PowerCurve::FromEngine (sub_820CE7D8)"
                                              : "PowerCurve::Build (sub_820CE430)") +
                      suffix);
      for (uint64_t seed = 0; seed < 300; seed++) {
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Random r(seed * 97 + 61 + 1000 * uint64_t(group) + 100000 * uint64_t(from_engine));
          car_objects::FillStack(h, r);
          WriteRandom(h.base, block.address, block.bytes, r);
          RandomCurve(h.base, curve, r, group == 1);
          // Build leaves +1024 as it was when no torque is above 0.0: a
          // peak within the table.
          guest::StoreU32(h.base, curve + 1024, r.Below(32));
          const uint32_t engine = block.address + 0x600;
          RandomPoints(h.base, engine + 24, r, group == 1);
          guest::StoreFloat(h.base, engine + 12, 4000.0f + float(r.Below(80)) * 100.0f);
          PPCContext t = CallContext(h);
          t.r3.u64 = curve;
          t.r4.u64 = from_engine ? engine : engine + 24;
          (v == 0 ? (from_engine ? Original<__imp__sub_820CE7D8> : Original<__imp__sub_820CE430>)
                  : (from_engine ? hooks::VehicleCurveFromEngine : hooks::VehicleBuildCurve))(
              t, h.base);
          CurveOutputs(out[v], h.base, curve);
          out[v].Bytes(Read(h.base, r1 - 112 + 80, 16));
          ShiftStackOutputs(out[v], h.base, r1 - 112);
          out[v].Word(t.r3.u32);
        }
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
      results.Report();
    }
    {
      // Reset: the status record in the block, the vehicle's car data
      // +1268 (vehicle + 4644) with its points.
      Results results("VehicleStatus::Reset (sub_820BFC48)" + suffix);
      for (uint64_t seed = 0; seed < 200; seed++) {
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Random r(seed * 101 + 67 + 1000 * uint64_t(group));
          car_objects::FillStack(h, r);
          WriteRandom(h.base, block.address, block.bytes, r);
          const uint32_t vehicle = block.address + 0x100 - 4644;  // its +4644 in the block
          const uint32_t engine = vehicle + 4644;
          RandomPoints(h.base, engine + 24, r, group == 1);
          guest::StoreFloat(h.base, engine + 12, 4000.0f + float(r.Below(80)) * 100.0f);
          const uint32_t status = block.address + 0x400;
          const uint32_t frame = r1 - 1168;
          // The curve on the stack: its +1024 within the table (see Build).
          guest::StoreU32(h.base, frame + 80 + 1024, r.Below(32));
          PPCContext t = CallContext(h);
          t.r3.u64 = status;
          t.r4.u64 = vehicle;
          (v == 0 ? Original<__imp__sub_820BFC48> : hooks::VehicleStatusReset)(t, h.base);
          out[v].Float(guest::LoadU32(h.base, status + 0));
          out[v].Bytes(Read(h.base, status + 4, 72));
          out[v].Float(guest::LoadU32(h.base, status + 76));
          out[v].Word(guest::LoadU32(h.base, status + 80));
          out[v].Floats(h.base, status + 84, 6);
          CurveOutputs(out[v], h.base, frame + 80);
          // The outer ShiftPeak's stack, but for the words its table
          // doesn't reach that Build's call left: Build's back chain
          // (frame - 112) and its ShiftPeak's register saves (frame - 128 to
          // frame - 113), not written by the plain C++.
          out[v].Bytes(Read(h.base, frame - 576, 40));
          out[v].Floats(h.base, frame - 528, (528 - 128) / 4);
          out[v].Floats(h.base, frame - 108, (108 - 16) / 4);
          ShiftStackOutputs(out[v], h.base, frame - 112);
          out[v].Bytes(Read(h.base, frame - 112 + 80, 16));
          out[v].Word(t.r3.u32);
        }
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
      results.Report();
    }
  }
}

// --- The vehicle set up from a car ---

// SetUpSurfaceGrip: a vehicle and a car of random words, the car's flags,
// tire types (0 to 3, the tables' types) and grades (0 to 4, now and then
// any). Compared: the vehicle (the records as floats, the pointers and
// +2496 exactly), the frame's temporaries (+80 to +299; the grade's
// doubleword at +296 exactly), r3.
void Grips(Context& h) {
  const Block block(vh::kVehicleBytes + 0x1000);
  if (!block.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  const uint32_t v_ = block.address, car = block.address + vh::kVehicleBytes;
  const uint32_t frame = CallContext(h).r1.u32 - 480;
  for (int group = 0; group < 2; group++) {
    Results results(std::string("Vehicle::SetUpSurfaceGrip (sub_820C1E00)") +
                    (group == 1 ? ", odd grades" : ""));
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 103 + 71 + 1000 * uint64_t(group));
        car_objects::FillStack(h, r);
        WriteRandom(h.base, block.address, block.bytes, r);
        guest::StoreU32(h.base, car + 3888, uint32_t(r.Next()) & ~0x6u | (r.Below(4) << 1));
        for (uint32_t at : {1712u, 1720u}) {
          guest::StoreU32(h.base, car + at, r.Below(4));
          guest::StoreU32(h.base, car + at + 4,
                          group == 1 && r.Below(2) == 0 ? uint32_t(r.Next()) : r.Below(5));
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = v_;
        t.r4.u64 = car;
        (v == 0 ? Original<__imp__sub_820C1E00> : hooks::VehicleSurfaceGrip)(t, h.base);
        out[v].Word(guest::LoadU32(h.base, v_ + 2496));
        out[v].Bytes(Read(h.base, v_ + 2668, 36));
        out[v].Bytes(Read(h.base, v_ + 2796, 36));
        out[v].Floats(h.base, v_ + 5716, (6148 - 5716) / 4);
        out[v].Floats(h.base, frame + 80, (296 - 80) / 4);
        out[v].Bytes(Read(h.base, frame + 296, 8));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// SetUp and ResetForCar: the fake world (one object free), the car physics'
// vehicle region, the car and the car data (v + 3376) by SetupFromCar's
// cases' writer (its groups: numbers only, one NaN, anything anywhere),
// then the fields SetUp reads (the nitrous' level 0 to 6, the tire types
// and grades, +3892), for ResetForCar the old collision object and shape.
// Compared: the vehicle (pointers by their number, the rest as floats),
// the new body and box, which blocks were freed, the world, the frames'
// temporaries (SetUp's +80 to +155, SurfaceGrip's), r3 and f1.
// Whether the car data keeps the physics' set-up within guest memory: the
// gearing's speed steps call EngineTorque, which reads its curve at an
// index from max(x, the idle floor +1628), so the floor must be a number
// from 0.0 to 4.0 (random data sends it out of guest memory, where the
// game's own data doesn't), with 1 to 7 gears and the curve's numbers.
bool GearingSane(uint8_t* base, uint32_t data) {
  auto number = [&](uint32_t at, float low, float high) {
    const float x = guest::LoadFloat(base, at);
    return std::isfinite(x) && x >= low && x < high;
  };
  const uint32_t gears = guest::LoadU32(base, data + 1268);
  if (gears < 1 || gears > 7 || !number(data + 1628, 0.0f, 4.0f)) {
    return false;
  }
  for (uint32_t i = 0; i < 34; i++) {
    if (!number(data + 1292 + 4 * i, -1e6f, 1e6f)) {
      return false;
    }
  }
  return true;
}

void SetUps(Context& h, uint32_t world, uint32_t region) {
  const Block cars(0x1000);
  if (!cars.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  const uint32_t car = cars.address;
  const uint32_t r1 = CallContext(h).r1.u32;
  const uint32_t frame = r1 - 224;
  // The ground probe's mesh: the collision world harness's course (put back
  // after).
  const Block course(TestCourseBytes());
  const Block table(4 * 0x10000);
  if (!course.address || !table.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  FillTestCourseTable(h.base, course.address, table.address);
  const uint32_t saved_route = guest::LoadU32(h.base, track_route::kRoute);
  uint8_t saved_mesh[12], saved_tried[kLinkedTriedBytes];
  std::memcpy(saved_mesh, h.base + track_collision::kMesh, sizeof(saved_mesh));
  std::memcpy(saved_tried, h.base + kLinkedTried, kLinkedTriedBytes);
  // One group: the car's numbers kept moderate (the gearing's tables).
  const char* const kGroups[] = {""};
  for (int reset = 0; reset < 2; reset++) {
    for (int group = 0; group < 1; group++) {
      Results results(std::string(reset ? "Vehicle::ResetForCar (sub_820C33B8)"
                                        : "Vehicle::SetUp (sub_820C3150)") +
                      kGroups[group]);
      uint32_t left_out = 0;
      for (uint64_t seed = 0; seed < 300; seed++) {
        Outputs out[2];
        bool skip = false;
        for (int v = -1; v < 2 && !skip; v++) {
          Random r(seed * 107 + 73 + 1000 * uint64_t(group) + 100000 * uint64_t(reset));
          WriteRandom(h.base, course.address, course.bytes, r);
          BuildTestCourse(h.base, course.address, table.address, r);
          car_objects::BuildWorld(h, world, r, true);
          car_objects::FillVehicleRegion(h.base, region, r, false);
          car_objects::FillStack(h, r);
          const uint32_t v_ = region;
          WriteSetupFromCarCase(h.base, v_ + 3376, car, frame, r1, seed * 7 + 5, 0);
          // The car's copied parts (block 1, +2240 to +3260: the gearbox,
          // engine and tires) and the car data the loader derives from as
          // positive numbers where the writer left a value outside 0.001
          // to 10,000 (the gearing's set-up reads its tables at indices
          // from them: see GearingSane), but for the integers (block 1's
          // +356, +360, +368; the car data's +172, +176, +1268).
          auto moderate = [&](uint32_t at) {
            const float x = guest::LoadFloat(h.base, at);
            if (!(std::isfinite(x) && x >= 1e-3f && x < 1e4f)) {
              guest::StoreFloat(h.base, at, 0.1f + float(r.Below(1900)) / 1000.0f);
            }
          };
          for (uint32_t i = 0; i < 1020; i += 4) {
            if (i != 356 && i != 360 && i != 368) {
              moderate(car + 2240 + i);
            }
          }
          for (uint32_t i = 0; i < 0x740; i += 4) {
            if (i != 172 && i != 176 && i != 1268) {
              moderate(v_ + 3376 + i);
            }
          }
          // The gear count (the car data's +1268, copied from block 1's
          // +368, the car's +2608): 1 to 7, as the physics' set-up's cases.
          guest::StoreU32(h.base, car + 2608, 1 + r.Below(7));
          guest::StoreU32(h.base, car + 1676, r.Below(7));
          // The physics' ground probe after its sync moves the wheels: their
          // track positions and the body's 0 (no mesh here), as the car
          // physics' RefreshFromBody cases.
          for (uint32_t w = 0; w < 4; w++) {
            guest::StoreU32(h.base, v_ + 416 + 368 * w + 20, 0);
          }
          guest::StoreU32(h.base, v_ + 2984, 0);
          // VehicleStatus::Reset's curve on the stack: its +1024 within the
          // table (Build leaves it when no torque is above 0.0, and the
          // random torques may all be).
          guest::StoreU32(h.base, frame - 1168 + 80 + 1024, r.Below(32));
          for (uint32_t at : {1712u, 1720u}) {
            guest::StoreU32(h.base, car + at, r.Below(4));
            guest::StoreU32(h.base, car + at + 4, r.Below(5));
          }
          guest::StoreU32(h.base, car + 3892, r.Below(2));
          guest::StoreU32(h.base, car + 1656, r.Below(12));
          if (r.Below(3) == 0) {
            guest::StoreU8(h.base, car + 76, 0);
          }
          Blocks blocks;
          if (reset) {
            const uint32_t active = car_objects::WorldActiveObject(h.base, world);
            guest::StoreU32(h.base, v_ + 340, active != 0 && r.Below(3) != 0 ? active : 0);
            uint32_t shape = 0;
            if (r.Below(4) != 0) {
              shape = memory::Alloc(496, 0);
              car_objects::BuildBox(h, shape, r);
              blocks.given.push_back(shape);
            }
            guest::StoreU32(h.base, v_ + 344, shape);
          }
          if (v < 0) {
            // A first run of the car loader (its plain C++, as both sides
            // run it) to see the car data the physics' set-up will read.
            double ignored = 0.0;
            const bool sane = car_physics::VehicleSetupFromCar(h.base, v_ + 3376, car, frame - 368,
                                                               &ignored) &&
                              GearingSane(h.base, v_ + 3376);
            blocks.FreeAll();
            if (!sane) {
              skip = true;
              left_out++;
            }
            continue;
          }
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          t.r3.u64 = v_;
          t.r4.u64 = car;
          if (reset) {
            (v == 0 ? Original<__imp__sub_820C33B8> : hooks::VehicleResetForCar)(t, h.base);
          } else {
            (v == 0 ? Original<__imp__sub_820C3150> : hooks::VehicleSetUp)(t, h.base);
          }
          const uint32_t body = guest::LoadU32(h.base, v_ + 336);
          const uint32_t shape = guest::LoadU32(h.base, v_ + 344);
          blocks.Note(body);
          blocks.Note(shape);
          const uint32_t object = guest::LoadU32(h.base, v_ + 340);
          if (object != 0) {
            blocks.Note(guest::LoadU32(h.base, object + 36));
            blocks.Note(guest::LoadU32(h.base, object + 40));
          }
          Outputs& o = out[v];
          for (uint32_t i = 0; i < vh::kVehicleBytes; i += 4) {
            const uint32_t w = guest::LoadU32(h.base, v_ + i);
            const bool pointer = i == 272 || i == 336 || i == 340 || i == 344 || i == 5712 ||
                                 i == 2984 || (i >= 2668 && i < 2704) || (i >= 2796 && i < 2832);
            pointer ? o.Word(blocks.Name(w)) : o.Float(w);
          }
          if (body != 0 && body != region + 0x2000) {
            o.Floats(h.base, body, 528 / 4);
          }
          if (shape != 0) {
            o.Bytes(Read(h.base, shape, 4));
            o.Floats(h.base, shape + 4, (496 - 4) / 4);
          }
          for (uint32_t b : blocks.given) {
            o.Word(memory::IsHeapBlock(b) ? 1 : 0);
          }
          for (uint32_t b : blocks.made) {
            o.Word(memory::Size(b));
          }
          for (uint32_t i = 0; i < car_objects::WorldBytes(); i += 4) {
            o.Word(blocks.Name(guest::LoadU32(h.base, world + i)));
          }
          o.Bytes(Read(h.base, car, 0x1000));
          o.Bytes(Read(h.base, frame + 80, 48));
          o.Floats(h.base, frame + 128, 7);
          o.Floats(h.base, frame - 480 + 80, (296 - 80) / 4);
          o.Bytes(Read(h.base, frame - 480 + 296, 8));
          o.Bytes(Read(h.base, kLinkedTried, kLinkedTriedBytes));
          o.Word(blocks.Name(t.r3.u32));
          o.Double(t.f1.u64);
          blocks.FreeAll();
          std::memcpy(h.base + kLinkedTried, saved_tried, kLinkedTriedBytes);
          std::memcpy(h.base + track_collision::kMesh, saved_mesh, sizeof(saved_mesh));
          guest::StoreU32(h.base, track_route::kRoute, saved_route);
        }
        if (!skip) {
          results.Check(out[0], out[1], fmt::format("seed {}", seed));
        }
      }
      results.Report();
      ITC_LOG_INFO("harness: {}{}: {} cases left out (car data the gearing's set-up would "
                   "read out of its tables with)",
                   reset ? "Vehicle::ResetForCar" : "Vehicle::SetUp", kGroups[group], left_out);
    }
  }
  std::memcpy(h.base + track_collision::kMesh, saved_mesh, sizeof(saved_mesh));
  guest::StoreU32(h.base, track_route::kRoute, saved_route);
}

// --- The tick ---

// The tick's and the status update's inputs on the car physics' vehicle
// region (numbers): the course installed (the collision world harness's:
// the mesh for the track positions, the route for the status), the car's
// and the wheels' track positions on it with the body over it, the race
// data's flags, the status record's fields, the engine's floor (+1628 of
// the region's car data) and rpm within its curve (EngineTorque reads it at
// an index from them).
void FillTickInputs(Context& h, uint32_t region, uint32_t race, Random& r) {
  const uint32_t v_ = region;
  const uint32_t body = guest::LoadU32(h.base, v_ + 336);
  CoursePosition(h.base, v_ + 5232, body + 208, r);
  for (uint32_t i = 0; i < 4; i++) {
    CoursePosition(h.base, v_ + 5328 + 96 * i, v_ + 480 + 368 * i, r);
  }
  guest::StoreU32(h.base, race + 20, r.Below(2) == 0 ? 0 : 0x4u << r.Below(4));
  guest::StoreU32(h.base, kRaceData, race);
  const uint32_t s = v_ + 6160;
  guest::StoreU32(h.base, s + 8, r.Below(2));
  guest::StoreFloat(h.base, s + 104, 50.0f + float(r.Below(4000)) / 10.0f);
  guest::StoreFloat(h.base, region + 0x1900 + 1628, float(r.Below(500)) / 1000.0f);
  guest::StoreFloat(h.base, v_ + 2228, float(r.Below(1200)) / 1000.0f);
  guest::StoreFloat(h.base, v_ + 4648, 6000.0f + float(r.Below(3000)));
}

// VehicleStatus::Update and Vehicle::Tick. Compared: the region (its
// outputs), the status record's +8 exactly, the input record, the
// searches' tried list. (r3 isn't: the status update's is what its last
// callee leaves, which no caller reads.)
void Ticks(Context& h, uint32_t world, uint32_t region) {
  const Block course(TestCourseBytes());
  const Block table(4 * 0x10000);
  const Block race(0x40);
  const Block cars(0x800);
  if (!course.address || !table.address || !race.address || !cars.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  FillTestCourseTable(h.base, course.address, table.address);
  const uint32_t saved_route = guest::LoadU32(h.base, track_route::kRoute);
  const uint32_t saved_race = guest::LoadU32(h.base, kRaceData);
  uint8_t saved_mesh[12], saved_tried[kLinkedTriedBytes];
  std::memcpy(saved_mesh, h.base + track_collision::kMesh, sizeof(saved_mesh));
  std::memcpy(saved_tried, h.base + kLinkedTried, kLinkedTriedBytes);
  constexpr uint32_t kInputs = 0x826C5B30;
  for (int tick = 0; tick < 2; tick++) {
    Results results(tick ? "Vehicle::Tick (sub_820C3020)" : "VehicleStatus::Update (sub_820C23E8)");
    for (uint64_t seed = 0; seed < (tick ? 150u : 300u); seed++) {
      Outputs out[2];
      const uint32_t racer = uint32_t(seed % 7);
      const uint32_t saved_input = guest::LoadU32(h.base, kInputs + 4 * racer);
      for (int v = 0; v < 2; v++) {
        Random r(seed * 109 + 79 + 1000 * uint64_t(tick));
        WriteRandom(h.base, course.address, course.bytes, r);
        BuildTestCourse(h.base, course.address, table.address, r);
        car_objects::BuildWorld(h, world, r);
        car_objects::FillVehicleRegion(h.base, region, r, false);
        if (tick) {
          car_objects::FillStepInputs(h.base, region, r);
        }
        car_objects::FillStack(h, r);
        FillTickInputs(h, region, race.address, r);
        WriteRandom(h.base, cars.address, cars.bytes, r);
        const uint32_t car = r.Below(3) == 0 ? 0 : cars.address;
        guest::StoreU32(h.base, cars.address + 1656, r.Below(12));
        guest::StoreU32(h.base, cars.address + 1664, r.Below(6));
        // The input record (in the scratch area): the pad's or an AI
        // driver's, without the flags of the callees never seen in play.
        WriteRandom(h.base, h.data, kScratchBytes, r);
        const uint32_t input = h.data + 0x100;
        guest::StoreU32(h.base, input + 0, uint32_t(r.Next()) & 0x82010200u);
        guest::StoreU8(h.base, input + 17, uint8_t(r.Below(2) == 0 ? 0 : 2));
        guest::StoreU8(h.base, region + 2249, 0);
        guest::StoreU32(h.base, kInputs + 4 * racer, input);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        const uint32_t flag = r.Below(2);
        if (tick) {
          t.r3.u64 = region;
          t.r4.u64 = racer | (uint64_t(r.Next()) & ~0xFFull);
          t.r5.u64 = car;
          t.r6.u64 = flag;
          (v == 0 ? Original<__imp__sub_820C3020> : hooks::VehicleTick)(t, h.base);
        } else {
          t.r3.u64 = region + 6160;
          t.r4.u64 = region;
          t.r5.u64 = car;
          t.r6.u64 = flag;
          (v == 0 ? Original<__imp__sub_820C23E8> : hooks::VehicleStatusUpdate)(t, h.base);
        }
        car_objects::VehicleRegionOutputs(out[v], h.base, region);
        out[v].Word(guest::LoadU32(h.base, region + 6160 + 8));
        out[v].Bytes(Read(h.base, input, 20));
        out[v].Bytes(Read(h.base, kLinkedTried, kLinkedTriedBytes));
        std::memcpy(h.base + kLinkedTried, saved_tried, kLinkedTriedBytes);
        std::memcpy(h.base + track_collision::kMesh, saved_mesh, sizeof(saved_mesh));
        guest::StoreU32(h.base, track_route::kRoute, saved_route);
        guest::StoreU32(h.base, kRaceData, saved_race);
        guest::StoreU32(h.base, kInputs + 4 * racer, saved_input);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- Vehicle::PlaceOnRoute: the tick's inputs (the course, the race data's
// flags, the vehicle region), a route position on the course whose slices
// back and on are on it too (section 0's slices 1 to 5, section 1's 0 to
// 4: steps across the sections' link included), a collision object of
// boxes on the vehicle's body (the fake world's objects' records aren't
// boxes, which ResetTransform's original can't place), the offset 0.0 (its
// other path, sub_820FA8F8's, never seen in play, is the whole original
// and gets no cases, as ApplyInput's never-seen callees). Compared: the
// region, the object and its boxes, the frame's temporaries (+80 to +447:
// the route positions and the track position's copy exactly, the rest as
// floats), the searches' tried list. (r3 isn't: no caller reads it.)

void Placements(Context& h, uint32_t world, uint32_t region) {
  const Block course(TestCourseBytes());
  const Block table(4 * 0x10000);
  const Block race(0x40);
  const Block object_block(0x800);  // the object at +0, its boxes at +0x200, +0x400, +0x600
  if (!course.address || !table.address || !race.address || !object_block.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  FillTestCourseTable(h.base, course.address, table.address);
  const uint32_t saved_route = guest::LoadU32(h.base, track_route::kRoute);
  const uint32_t saved_race = guest::LoadU32(h.base, kRaceData);
  uint8_t saved_mesh[12], saved_tried[kLinkedTriedBytes];
  std::memcpy(saved_mesh, h.base + track_collision::kMesh, sizeof(saved_mesh));
  std::memcpy(saved_tried, h.base + kLinkedTried, kLinkedTriedBytes);
  const uint32_t frame = CallContext(h).r1.u32 - 560;
  for (int group = 0; group < 2; group++) {
    Results results(std::string("Vehicle::PlaceOnRoute (sub_820C3608)") +
                    (group == 1 ? ", NaNs and infinities in the vehicle and the speed" : ""));
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 113 + 83 + 1000 * uint64_t(group));
        WriteRandom(h.base, course.address, course.bytes, r);
        BuildTestCourse(h.base, course.address, table.address, r);
        car_objects::BuildWorld(h, world, r);
        car_objects::FillVehicleRegion(h.base, region, r, group == 1);
        car_objects::FillStack(h, r);
        FillTickInputs(h, region, race.address, r);
        // The slices' two edge points (the tables after the slice records,
        // which BuildTestCourse leaves random): 6 m to each side.
        const uint32_t sections = guest::LoadU32(h.base, guest::LoadU32(h.base, track_route::kRoute) + 12);
        for (uint32_t k = 0; k < 2; k++) {
          const uint32_t sec = guest::LoadU32(h.base, sections + 4 * k);
          const uint32_t count = guest::LoadU16(h.base, sec);
          for (uint32_t n = 0; n < count; n++) {
            const uint32_t center = sec + 124 + 84 * n;
            for (uint32_t side = 0; side < 2; side++) {
              const uint32_t edge = sec + 124 + count * (side == 0 ? 84 : 96) + 12 * n;
              guest::StoreFloat(h.base, edge + 0, guest::LoadFloat(h.base, center + 0) +
                                                      (side == 0 ? -6.0f : 6.0f) +
                                                      float(r.Below(100)) / 100.0f);
              guest::StoreFloat(h.base, edge + 4, 0.5f + float(r.Below(100)) / 1000.0f);
              guest::StoreFloat(h.base, edge + 8, guest::LoadFloat(h.base, center + 8) +
                                                      float(int32_t(r.Below(100)) - 50) / 100.0f);
            }
          }
        }
        const uint32_t o = object_block.address;
        for (uint32_t i = 0; i < object_block.bytes; i += 4) {
          guest::StoreU32(h.base, o + i, group == 1 ? RandomFloat(r)
                                                : std::bit_cast<uint32_t>(
                                                      float(int32_t(r.Below(2001)) - 1000) / 100.0f));
        }
        for (uint32_t box = 0x200; box <= 0x600; box += 0x200) {
          car_objects::BuildBox(h, o + box, r);
        }
        guest::StoreU32(h.base, o + 16, guest::LoadU32(h.base, region + 336));
        guest::StoreU32(h.base, o + 32, o + 0x200);
        guest::StoreU32(h.base, o + 36, o + 0x400);
        guest::StoreU32(h.base, o + 40, o + 0x600);
        if (r.Below(2) == 0) {
          guest::StoreU32(h.base, o + 32 + 112, 0);
        }
        guest::StoreU32(h.base, region + 340, o);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        const uint32_t position = h.data + 0x100;
        const uint32_t section = r.Below(2);
        guest::StoreU16(h.base, position + 0, uint16_t(section));
        guest::StoreU16(h.base, position + 2, uint16_t(section == 0 ? 1 + r.Below(5) : r.Below(5)));
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = region;
        t.r4.u64 = position;
        t.f1.f64 = double(std::bit_cast<float>(
            group == 1 ? RandomFloat(r, 300.0f)
                       : std::bit_cast<uint32_t>(float(r.Below(3001)) / 10.0f)));
        t.f2.f64 = r.Below(2) == 0 ? 0.0 : -0.0;
        t.r6.u64 = uint64_t(int64_t(int32_t(r.Below(8)) - 3));
        t.r7.u64 = r.Below(2) == 0 ? 0 : (r.Next() | 1);
        (v == 0 ? Original<__imp__sub_820C3608> : hooks::VehiclePlaceOnRoute)(t, h.base);
        car_objects::VehicleRegionOutputs(out[v], h.base, region);
        // The object's and its boxes' words: floats where the code loads and
        // stores floats (as the car physics' harness's ObjectOutputs and
        // BoxOutputs), the rest exactly.
        for (uint32_t i = 0; i < 320; i += 4) {
          const uint32_t w = guest::LoadU32(h.base, o + i);
          const bool is_float = (i >= 48 && i < 148) || (i >= 160 && i < 192) ||
                                (i >= 208 && i < 228) || i >= 304;
          is_float ? out[v].Float(w) : out[v].Word(w);
        }
        out[v].Bytes(Read(h.base, o + 320, 0x200 - 320));
        for (uint32_t box = 0x200; box <= 0x600; box += 0x200) {
          for (uint32_t i = 0; i < 496; i += 4) {
            const uint32_t w = guest::LoadU32(h.base, o + box + i);
            (i == 0 || i == 112 || (i >= 160 && i < 176)) ? out[v].Word(w) : out[v].Float(w);
          }
          out[v].Bytes(Read(h.base, o + box + 496, 16));
        }
        out[v].Bytes(Read(h.base, frame + 80, 16));
        out[v].Floats(h.base, frame + 96, (144 - 96) / 4);
        out[v].Bytes(Read(h.base, frame + 144, 16));
        out[v].Floats(h.base, frame + 160, (176 - 160) / 4);
        out[v].Bytes(Read(h.base, frame + 176, 16));
        out[v].Floats(h.base, frame + 192, (352 - 192) / 4);
        out[v].Bytes(Read(h.base, frame + 352, 96));
        out[v].Bytes(Read(h.base, kLinkedTried, kLinkedTriedBytes));
        std::memcpy(h.base + kLinkedTried, saved_tried, kLinkedTriedBytes);
        std::memcpy(h.base + track_collision::kMesh, saved_mesh, sizeof(saved_mesh));
        guest::StoreU32(h.base, track_route::kRoute, saved_route);
        guest::StoreU32(h.base, kRaceData, saved_race);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

}  // namespace

// For the race world's harness (src/harnesses/race_world.cpp): the tick's
// inputs on the car physics' vehicle region (see FillTickInputs above).
namespace vehicle_cases {
void FillTickInputs(Context& h, uint32_t region, uint32_t race, Random& r) {
  harness::FillTickInputs(h, region, race, r);
}
}  // namespace vehicle_cases

void Vehicle(Context& h) {
  Leaves(h);
  Steps(h);
  TrackPositions(h);
  Links(h);
  Curves(h);
  Grips(h);
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  const Block world(car_objects::WorldBytes());
  const Block region(car_objects::VehicleRegionBytes());
  if (!world.address || !region.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  Refreshes(h, world.address, region.address);
  Inputs(h, region.address);
  Slips(h, region.address);
  Nearest(h);
  Ticks(h, world.address, region.address);
  Placements(h, world.address, region.address);
  if (hooks::NativeHeapActive()) {
    Removals(h, world.address);
    Lifetime(h, world.address);
    SetUps(h, world.address, region.address);
  } else {
    ITC_LOG_INFO("harness: vehicle: the vehicles' lifetime skipped (--native_heap off)");
  }
  guest::StoreU32(h.base, kWorldPointer, saved_world);
}

}  // namespace harness
