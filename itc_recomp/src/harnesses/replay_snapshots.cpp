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

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/replay_snapshots.h"
#include "memory/heap.h"
#include "replay_snapshots/replay_snapshots.h"

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

}  // namespace

void ReplaySnapshots(Context& h) {
  Restores(h);
}

}  // namespace harness
