// The replay's snapshots (src/replay_snapshots/,
// docs/functions/replay-snapshots.md) against the recompiled functions. The
// original runs with --native_replay_snapshots off, the plain C++ through
// the register steps (hooks/replay_snapshots.h). Every block is filled with
// the same random words for both sides (NaN and infinity words planted
// among them), and every block, the scratch area and r3 are compared
// exactly (the lfs and stfs pairs keep the words' bits in both).
//
// Record5232::Restore: a record put back into a vehicle with its rigid body
// (+336) and collision object (+340, its two placed shape copies at +36 and
// +40); cases with the record's collision flag (+4080) 0, with no object,
// with both copies the same shape, and with the first copy over the
// object's +40 (the original reads +40 before it writes the first copy).
//
// RaceSnapshot::Save: the seven vehicles (the game's [0x826C2E34] pointed
// at a block of them, each with its body, object and copies) and the
// racers' manager ([0x826D82F4] at a block) saved into a snapshot block;
// vehicles in use or not (+6148), with or without an object. The game's
// two pointers are put back after each side.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/replay_snapshots.h"
#include "memory/heap.h"
#include "replay_snapshots/replay_snapshots.h"
#include "vehicle/vehicle.h"

#include <fmt/format.h>

#include <memory>

ITC_FLAG_DECLARE(bool, native_replay_snapshots);

namespace harness {

namespace {

namespace rs = replay_snapshots;

constexpr uint32_t kVehicleBytes = 6272;
constexpr uint32_t kBodyBytes = 528;
constexpr uint32_t kObjectBytes = 512;  // the first copy at object - 8 writes to +488
constexpr uint32_t kShapeBytes = 512;

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

// Random words, with NaNs (quiet and signaling, either sign) and
// infinities planted at random words.
void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  static constexpr uint32_t kSpecial[] = {0x7F800001, 0xFF800001, 0x7FC00000, 0xFFC00001,
                                          0x7F800000, 0xFF800000, 0x7FBFFFFF, 0x00000001};
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = r.Below(16) == 0 ? kSpecial[r.Below(8)] : uint32_t(r.Next());
    guest::StoreU32(base, address + i, word);
  }
}

void Restores(Context& h) {
  const Block record(rs::kRecordBytes), vehicle(kVehicleBytes), body(kBodyBytes);
  // The object, with room below it for the case whose first copy covers
  // its +40 (the copy at object - 8).
  const Block object_room(kObjectBytes + 16), shape_a(kShapeBytes), shape_b(kShapeBytes);
  const uint32_t object = object_room.address + 16;
  const Block* blocks[] = {&record, &vehicle, &body, &object_room, &shape_a, &shape_b};
  for (const Block* b : blocks) {
    if (!b->address) {
      ITC_LOG_ERROR("harness: replay_snapshots: no block from the allocation layer");
      return;
    }
  }
  Results results("Record5232::Restore (sub_821044D8)");
  for (uint64_t seed = 0; seed < 40; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 977 + 31);
      WriteRandom(h.base, h.data, kScratchBytes, r);
      for (const Block* b : blocks) {
        WriteRandom(h.base, b->address, b->bytes, r);
      }
      const uint32_t kind = uint32_t(seed % 6);
      guest::StoreU32(h.base, vehicle.address + 336, body.address);
      guest::StoreU32(h.base, vehicle.address + 340, kind == 2 ? 0 : object);
      guest::StoreU32(h.base, object + 36, kind == 4 ? object - 8 : shape_a.address);
      guest::StoreU32(h.base, object + 40, kind == 3 ? shape_a.address : shape_b.address);
      // The record's collision flag: 0, 1 or any other word.
      guest::StoreU32(h.base, record.address + 4080,
                      kind == 1 ? 0 : (kind == 5 ? uint32_t(r.Next()) | 1 : 1));
      PPCContext t = CallContext(h);
      t.r3.u64 = record.address;
      t.r4.u64 = vehicle.address;
      if (v == 0) {
        ITC_FLAG(native_replay_snapshots) = false;
        __imp__sub_821044D8(t, h.base);
        ITC_FLAG(native_replay_snapshots) = true;
      } else {
        hooks::Record5232Restore(t, h.base);
      }
      for (const Block* b : blocks) {
        out[v].Bytes(Read(h.base, b->address, b->bytes));
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// The seven vehicles with their parts: the vehicles' block, and one block
// of bodies, objects and copies (kPartsBytes each).
constexpr uint32_t kManagerBytes = 2400;
constexpr uint32_t kSnapshotBytes = 38928;
constexpr uint32_t kPartsBytes = kBodyBytes + kObjectBytes + 2 * kShapeBytes;

struct World {
  World() : vehicles(vehicle::kVehiclesBytes), parts(7 * kPartsBytes), manager(kManagerBytes) {}
  bool Ok() const { return vehicles.address && parts.address && manager.address; }
  // Random words everywhere, then the pointers: vehicle i in use unless
  // bit i of `unused`, without an object when bit i of `no_object`.
  void Fill(uint8_t* base, Random& r, uint32_t unused, uint32_t no_object) const {
    WriteRandom(base, vehicles.address, vehicles.bytes, r);
    WriteRandom(base, parts.address, parts.bytes, r);
    WriteRandom(base, manager.address, manager.bytes, r);
    for (uint32_t i = 0; i < 7; i++) {
      const uint32_t v = vehicles.address + vehicle::kVehicleBytes * i;
      const uint32_t p = parts.address + kPartsBytes * i;
      const uint32_t object = p + kBodyBytes;
      if (unused >> i & 1) {
        guest::StoreU32(base, v + 6148, 0);
      } else if (guest::LoadU32(base, v + 6148) == 0) {
        guest::StoreU32(base, v + 6148, 0x81);
      }
      guest::StoreU32(base, v + 336, p);
      guest::StoreU32(base, v + 340, (no_object >> i & 1) ? 0 : object);
      guest::StoreU32(base, object + 36, object + kObjectBytes);
      guest::StoreU32(base, object + 40, object + kObjectBytes + kShapeBytes);
    }
  }
  void Read(uint8_t* base, Outputs& out) const {
    out.Bytes(harness::Read(base, vehicles.address, vehicles.bytes));
    out.Bytes(harness::Read(base, parts.address, parts.bytes));
    out.Bytes(harness::Read(base, manager.address, manager.bytes));
  }
  Block vehicles, parts, manager;
};

// The game's vehicles and manager pointers at the world's blocks while
// alive.
class Pointed {
 public:
  Pointed(uint8_t* base, const World& world)
      : base_(base),
        vehicles_(guest::LoadU32(base, vehicle::kVehicles)),
        manager_(guest::LoadU32(base, rs::kManager)) {
    guest::StoreU32(base, vehicle::kVehicles, world.vehicles.address);
    guest::StoreU32(base, rs::kManager, world.manager.address);
  }
  ~Pointed() {
    guest::StoreU32(base_, vehicle::kVehicles, vehicles_);
    guest::StoreU32(base_, rs::kManager, manager_);
  }
  Pointed(const Pointed&) = delete;
  Pointed& operator=(const Pointed&) = delete;

 private:
  uint8_t* base_;
  uint32_t vehicles_, manager_;
};

void SnapshotSaves(Context& h) {
  const World world;
  const Block snapshot(kSnapshotBytes);
  if (!world.Ok() || !snapshot.address) {
    ITC_LOG_ERROR("harness: replay_snapshots: no block from the allocation layer");
    return;
  }
  Results results("RaceSnapshot::Save (sub_82104AB8)");
  for (uint64_t seed = 0; seed < 24; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 613 + 7);
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, snapshot.address, snapshot.bytes, r);
      // All in use, none, or random; objects all, or some missing.
      const uint32_t unused = seed == 0 ? 0 : (seed == 1 ? 0x7F : r.Below(128));
      const uint32_t no_object = seed % 3 == 2 ? r.Below(128) : 0;
      world.Fill(h.base, r, unused, no_object);
      PPCContext t = CallContext(h);
      t.r3.u64 = snapshot.address;
      {
        const Pointed pointed(h.base, world);
        if (v == 0) {
          ITC_FLAG(native_replay_snapshots) = false;
          __imp__sub_82104AB8(t, h.base);
          ITC_FLAG(native_replay_snapshots) = true;
        } else {
          hooks::RaceSnapshotSave(t, h.base);
        }
      }
      out[v].Bytes(Read(h.base, snapshot.address, snapshot.bytes));
      world.Read(h.base, out[v]);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

}  // namespace

void ReplaySnapshots(Context& h) {
  Restores(h);
  SnapshotSaves(h);
}

}  // namespace harness
