// The type 6 and 7 modes (src/type67_modes/,
// docs/functions/type6-type7-modes.md) against the recompiled functions. The
// originals run with --native_type67_modes off, the plain C++ through the
// register steps (hooks/type67_modes.h).
//
// The state machine Inits: an object block whose machine (at +0, +4 or
// +460) is empty or holds random transitions already (fewer than 200 less
// the Init's, so its fit) with keys of small numbers, some the Init adds (so
// some Adds replace); the original's frame filled with the same random words
// for both (the words it never writes go into the transitions); the object
// block, the frame (but its back chain and saved registers), the scratch
// area and r3 (both halves) compared. Compared with harness::Outputs, all
// exactly (no float math).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/type67_modes.h"
#include "memory/heap.h"
#include "type67_modes/type67_modes.h"

#include <fmt/format.h>

ITC_FLAG_DECLARE(bool, native_type67_modes);

namespace harness {

namespace {

namespace t67 = type67_modes;

constexpr uint32_t kObjectBytes = 0x2A00;  // the node screen's machine at +460

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

uint32_t MachineOffset(uint32_t address) {
  switch (address) {
    case 0x822454B8:  // Type7Screen0::Init
    case 0x8223F9A8:
    case 0x822436A0:
    case 0x82230F80:
    case 0x82240998:
    case 0x82237B78:
    case 0x822470A8:
      return 4;
    case 0x82237320:  // Type6NodeScreen::InitMachine
      return 460;
    default:
      return 0;
  }
}

GuestFunction Recompiled(uint32_t address) {
  switch (address) {
    case 0x82235138: return __imp__sub_82235138;
    case 0x822454B8: return __imp__sub_822454B8;
    case 0x822329A8: return __imp__sub_822329A8;
    case 0x82242420: return __imp__sub_82242420;
    case 0x821C9918: return __imp__sub_821C9918;
    case 0x821C9430: return __imp__sub_821C9430;
    case 0x821CB6E8: return __imp__sub_821CB6E8;
    case 0x821CB090: return __imp__sub_821CB090;
    case 0x821A8210: return __imp__sub_821A8210;
    case 0x8223F9A8: return __imp__sub_8223F9A8;
    case 0x822436A0: return __imp__sub_822436A0;
    case 0x82230F80: return __imp__sub_82230F80;
    case 0x82240998: return __imp__sub_82240998;
    case 0x82237320: return __imp__sub_82237320;
    case 0x82237B78: return __imp__sub_82237B78;
    case 0x822470A8: return __imp__sub_822470A8;
    default: return nullptr;
  }
}

void Inits(Context& h, const Block& object) {
  namespace gm = game_modes;
  for (const t67::InitInfo& info : t67::kInits) {
    Results results(fmt::format("{} (sub_{:08X})", info.name, info.address));
    const uint32_t saves = (32 - info.first_saved) * 8 + 8;
    const uint32_t frame = CallContext(h).r1.u32 - info.frame;
    const uint32_t machine = object.address + MachineOffset(info.address);
    for (uint64_t seed = 0; seed < 12; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 151 + info.address);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, object.address, object.bytes, r);
        WriteRandom(h.base, frame, info.frame, r);
        if (seed % 2 == 0) {
          gm::StateMachineConstruct(h.base, machine);
          guest::StoreU32(h.base, machine + gm::kTransitionCount, 0);
        } else {
          // Keys of small numbers: states below 12, messages below 36, so
          // some match the Init's (Add replaces those).
          const uint32_t count = 8 + r.Below(160);
          guest::StoreU32(h.base, machine + gm::kTransitionCount, count);
          for (uint32_t i = 0; i < gm::kTransitions; i++) {
            const uint32_t at = machine + gm::kTransitionBytes * i;
            guest::StoreU32(h.base, at, r.Below(12));
            guest::StoreU32(h.base, at + 4, r.Below(36));
            guest::StoreU32(h.base, at + 8, r.Below(8) == 0 ? 1 : 0);
            guest::StoreU32(h.base, at + 12, 0);
          }
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = object.address;
        if (v == 0) {
          ITC_FLAG(native_type67_modes) = false;
          Recompiled(info.address)(t, h.base);
          ITC_FLAG(native_type67_modes) = true;
        } else {
          hooks::Type67Init(t, h.base, info.address);
        }
        out[v].Bytes(Read(h.base, object.address, object.bytes));
        out[v].Bytes(Read(h.base, frame + 80, info.frame - 80 - saves));
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

}  // namespace

void Type67Modes(Context& h) {
  const Block object(kObjectBytes);
  if (!object.address) {
    ITC_LOG_ERROR("harness: type67_modes: no block from the allocation layer");
    return;
  }
  Inits(h, object);
}

}  // namespace harness
