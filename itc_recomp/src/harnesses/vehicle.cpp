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
void BuildWorld(Context& h, uint32_t region, Random& r);
void BuildBox(Context& h, uint32_t box, Random& r);
void FillVehicleRegion(uint8_t* base, uint32_t region, Random& r, bool nans);
void FillStepInputs(uint8_t* base, uint32_t region, Random& r);
void VehicleRegionOutputs(Outputs& out, uint8_t* base, uint32_t region);
void FillStack(Context& h, Random& r);
}  // namespace car_objects

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
  std::vector<uint32_t> given;
  uint32_t Name(uint32_t w) const {
    for (size_t i = 0; i < given.size(); i++) {
      if (w == given[i] && w != 0) {
        return uint32_t(0xB10C0000u + i);
      }
    }
    return w;
  }
  void FreeAll() {
    for (uint32_t b : given) {
      if (b != 0 && memory::IsHeapBlock(b)) {
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

}  // namespace

void Vehicle(Context& h) {
  Leaves(h);
  Steps(h);
  TrackPositions(h);
  Links(h);
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  const Block world(car_objects::WorldBytes());
  const Block region(car_objects::VehicleRegionBytes());
  if (!world.address || !region.address) {
    ITC_LOG_ERROR("harness: vehicle: no block from the allocation layer");
    return;
  }
  Refreshes(h, world.address, region.address);
  if (hooks::NativeHeapActive()) {
    Removals(h, world.address);
    Lifetime(h, world.address);
  } else {
    ITC_LOG_INFO("harness: vehicle: the vehicles' lifetime skipped (--native_heap off)");
  }
  guest::StoreU32(h.base, kWorldPointer, saved_world);
}

}  // namespace harness
