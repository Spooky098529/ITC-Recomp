// The save system's interface (src/save_interface/,
// docs/functions/save-interface.md) against the recompiled functions. The
// game's live objects aren't touched: each case builds its own interface,
// +36 interface, save devices, users and three 1,000-byte save slots in a
// block from the allocation layer (with the game's vtables, so the
// recompiled wrappers reach the same functions), fills the rest with random
// words, and runs the original (with --native_save_interface off, so its
// callees are the originals too) or the plain C++ through the register steps
// (hooks/save_interface.h). The wrappers read the interface from
// [0x826BDC90]: it points at the case's interface while the case runs and
// is put back after. The block, the scratch area and r3 (64 bits) are
// compared with harness::Outputs, all exactly (nothing here is a float).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/save_interface.h"
#include "memory/heap.h"
#include "save_interface/save_interface.h"

#include <fmt/format.h>

#include <functional>
#include <string>

ITC_FLAG_DECLARE(bool, native_save_interface);

namespace harness {

namespace {

namespace si = save_interface;

constexpr uint32_t kBlockBytes = 0x2000;
// The objects in the block.
constexpr uint32_t kInterface = 0x0;
constexpr uint32_t kObject36 = 0x40;
constexpr uint32_t kDevices = 0x100;
constexpr uint32_t kUsers = 0x200;
constexpr uint32_t kFakeVtable = 0x300;  // a vtable copy for the fallback cases
constexpr uint32_t kSlots = 0x800;       // three slots, to 0x13B8

constexpr uint32_t kInterfaceVtable = 0x82000E38;
constexpr uint32_t kObject36Vtable = 0x82000AFC;
constexpr uint32_t kDevicesVtable = 0x82002198;
constexpr uint32_t kUsersVtable = 0x82000FA0;

struct Block {
  Block() : address(memory::Alloc(kBlockBytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t words, Random& random) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(base, address + 4 * i, uint32_t(random.Next()));
  }
}

// The objects linked up over the random words. `fallback` gives the +36
// interface a vtable copy whose +40 and +36 are swapped, so the getters'
// plain C++ refuses and the original runs (only for the getters: the
// wrappers would then call the users' vtable as the save devices').
void Link(uint8_t* base, uint32_t block, Random& random, bool fallback) {
  guest::StoreU32(base, block + kInterface, kInterfaceVtable);
  guest::StoreU32(base, block + kInterface + 8, block + kObject36);
  guest::StoreU32(base, block + kObject36, kObject36Vtable);
  guest::StoreU32(base, block + kObject36 + 88, block + kDevices);
  guest::StoreU32(base, block + kObject36 + 92, block + kUsers);
  guest::StoreU32(base, block + kDevices, kDevicesVtable);
  guest::StoreU32(base, block + kDevices + 44, block + kSlots);
  guest::StoreU32(base, block + kUsers, kUsersVtable);
  // The users: a count of 0 to 5, the list's numbers 0 to 3, each user's
  // sign-in state 0 or not.
  guest::StoreU32(base, block + kUsers + 216, random.Below(6));
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, block + kUsers + 200 + 4 * i, random.Below(4));
    guest::StoreU32(base, block + kUsers + 8 + 40 * i, random.Below(3) == 0 ? 0 : random.Below(5));
  }
  // The slots: a state of 0 to 11 (3 half the time), the requests 0 or 1.
  for (uint32_t s = 0; s < 3; s++) {
    const uint32_t slot = block + kSlots + 1000 * s;
    guest::StoreU32(base, slot + 988, random.Below(2) == 0 ? 3 : random.Below(12));
    for (uint32_t word = 960; word <= 980; word += 4) {
      guest::StoreU32(base, slot + word, random.Below(2));
    }
  }
  if (fallback) {
    for (uint32_t i = 0; i < 16; i++) {
      guest::StoreU32(base, block + kFakeVtable + 4 * i,
                      guest::LoadU32(base, kObject36Vtable + 4 * i));
    }
    guest::StoreU32(base, block + kFakeVtable + 40, si::kFrameworkUsers);
    guest::StoreU32(base, block + kFakeVtable + 36, si::kFrameworkDevices);
    guest::StoreU32(base, block + kObject36, block + kFakeVtable);
  }
}

// A string of 1 to `most` random nonzero units (bytes, or UTF-16 when
// `wide`) and its terminator at `at`.
void RandomString(uint8_t* base, uint32_t at, uint32_t most, bool wide, Random& random) {
  const uint32_t length = 1 + random.Below(most);
  for (uint32_t i = 0; i < length; i++) {
    const uint32_t c = 1 + random.Below(wide ? 0xFFFE : 0xFE);
    if (wide) {
      guest::StoreU16(base, at + 2 * i, uint16_t(c));
    } else {
      guest::StoreU8(base, at + i, uint8_t(c));
    }
  }
  if (wide) {
    guest::StoreU16(base, at + 2 * length, 0);
  } else {
    guest::StoreU8(base, at + length, 0);
  }
}

// The strings in the scratch area.
constexpr uint32_t kDisplayName = 0x100;  // UTF-16, up to 120 units
constexpr uint32_t kFileName = 0x200;     // up to 40 bytes
constexpr uint32_t kPath = 0x240;         // up to 120 bytes

struct CaseSetup {
  bool fallback = false;
  bool strings = false;
};

// One case through both versions: the block and the scratch area filled from
// the case's random words, the objects linked, [0x826BDC90] the case's
// interface; `args(t, block)` sets the registers.
void Case(Context& h, Results& results, const Block& block, const CaseSetup& setup,
          const std::function<void(PPCContext&, uint32_t, Random&)>& args, GuestFunction recompiled,
          GuestFunction native, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  const uint32_t live = guest::LoadU32(h.base, si::kInterfaceGlobal);
  for (int v = 0; v < 2; v++) {
    Random random = start;
    WriteRandom(h.base, h.data, kScratchBytes / 4, random);
    WriteRandom(h.base, block.address, kBlockBytes / 4, random);
    Link(h.base, block.address, random, setup.fallback);
    if (setup.strings) {
      RandomString(h.base, h.data + kDisplayName, 120, true, random);
      RandomString(h.base, h.data + kFileName, 40, false, random);
      RandomString(h.base, h.data + kPath, 120, false, random);
    }
    PPCContext t = CallContext(h);
    args(t, block.address, random);
    after = random;
    guest::StoreU32(h.base, si::kInterfaceGlobal, block.address + kInterface);
    ITC_FLAG(native_save_interface) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_save_interface) = true;
    out[v].Word(guest::LoadU32(h.base, si::kInterfaceGlobal) - block.address);
    guest::StoreU32(h.base, si::kInterfaceGlobal, live);
    out[v].Bytes(Read(h.base, block.address, kBlockBytes));
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

using Args = std::function<void(PPCContext&, uint32_t block, uint32_t c, Random& random)>;

void Run(Context& h, const Block& block, const char* name, GuestFunction recompiled,
         GuestFunction native, int cases, const Args& args, bool strings = false,
         bool fallbacks = false) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    CaseSetup setup;
    setup.strings = strings;
    setup.fallback = fallbacks && c % 4 == 3;
    Case(
        h, results, block, setup,
        [&](PPCContext& t, uint32_t b, Random& random) { args(t, b, uint32_t(c), random); },
        recompiled, native, fmt::format("case {}{}", c, setup.fallback ? " (fallback)" : ""));
  }
  results.Report();
}

// The game's vtables hold the targets the plain C++ looks for (else every
// case would run the original on both sides and match without checking
// anything).
void Vtables(Context& h) {
  Results results("save_interface vtable targets");
  struct Slot {
    uint32_t vtable, offset, target;
  };
  static constexpr Slot kSlotsWanted[] = {
      {kInterfaceVtable, 12, si::kAppDevices},
      {kInterfaceVtable, 8, si::kAppUsers},
      {kObject36Vtable, 40, si::kFrameworkDevices},
      {kObject36Vtable, 36, si::kFrameworkUsers},
      {kUsersVtable, 4, si::kUsersCount},
      {kUsersVtable, 8, si::kUsersSignedIn},
      {kDevicesVtable, 4, si::kSlotRequestDevice},
      {kDevicesVtable, 8, si::kSlotDevice},
      {kDevicesVtable, 12, si::kSlotFreeBytes},
      {kDevicesVtable, 16, si::kSlotSaveCount},
      {kDevicesVtable, 20, si::kSlotSaveAt},
      {kDevicesVtable, 28, si::kSlotState},
      {kDevicesVtable, 32, si::kSlotPending},
      {kDevicesVtable, 56, si::kSetRootName},
      {kDevicesVtable, 60, si::kSlotRequestSave},
      {kDevicesVtable, 64, si::kSlotRequestLoad},
  };
  for (const Slot& s : kSlotsWanted) {
    Outputs out[2];
    out[0].Word(s.target);
    out[1].Word(guest::LoadU32(h.base, s.vtable + s.offset));
    results.Check(out[0], out[1], fmt::format("{:08X} +{}", s.vtable, s.offset));
  }
  results.Report();
}

uint32_t Slot(Random& random) { return random.Below(3); }

}  // namespace

void SaveInterface(Context& h) {
  Block block;
  if (!block.address) {
    ITC_LOG_ERROR("harness: save_interface: no block from the allocation layer");
    return;
  }
  Vtables(h);
  const uint32_t data = h.data;

  // The getters.
  Run(h, block, "App::SaveDevices (sub_820943B0)", __imp__sub_820943B0,
      hooks::SaveInterfaceAppDevices, 8,
      [&](PPCContext& t, uint32_t b, uint32_t, Random& random) { t.r3.u64 = b + kInterface; }, false, true);
  Run(h, block, "App::Users (sub_820943C8)", __imp__sub_820943C8, hooks::SaveInterfaceAppUsers,
      8, [&](PPCContext& t, uint32_t b, uint32_t, Random& random) { t.r3.u64 = b + kInterface; }, false, true);
  Run(h, block, "word +92 (sub_82493700)", __imp__sub_82493700,
      hooks::SaveInterfaceFrameworkUsers, 4,
      [&](PPCContext& t, uint32_t b, uint32_t, Random& random) { t.r3.u64 = b + 4 * random.Below(64); });
  Run(h, block, "word +216 (sub_82490B08)", __imp__sub_82490B08, hooks::SaveInterfaceUsersCount,
      4, [&](PPCContext& t, uint32_t b, uint32_t, Random& random) { t.r3.u64 = b + 4 * random.Below(64); });
  Run(h, block, "Users::SignedIn (sub_820996D8)", __imp__sub_820996D8,
      hooks::SaveInterfaceUsersSignedIn, 24, [&](PPCContext& t, uint32_t b, uint32_t, Random& random) {
        t.r3.u64 = b + kUsers;
        t.r4.u64 = random.Below(7);
      });

  // The save devices' slots.
  const auto slot_args = [&](PPCContext& t, uint32_t b, uint32_t, Random& random) {
    t.r3.u64 = b + kDevices;
    t.r4.u64 = Slot(random);
    t.r5.u64 = random.Next();
  };
  Run(h, block, "SaveSlot::RequestDevice (sub_820A5750)", __imp__sub_820A5750,
      hooks::SaveInterfaceSlotRequestDevice, 6, slot_args);
  Run(h, block, "SaveSlot::Device (sub_820A5768)", __imp__sub_820A5768,
      hooks::SaveInterfaceSlotDevice, 6, slot_args);
  Run(h, block, "SaveSlot::FreeBytes (sub_820A5780)", __imp__sub_820A5780,
      hooks::SaveInterfaceSlotFreeBytes, 6, slot_args);
  Run(h, block, "SaveSlot::SaveCount (sub_820A5798)", __imp__sub_820A5798,
      hooks::SaveInterfaceSlotSaveCount, 6, slot_args);
  Run(h, block, "SaveSlot::SaveAt (sub_820A57B0)", __imp__sub_820A57B0,
      hooks::SaveInterfaceSlotSaveAt, 12, [&](PPCContext& t, uint32_t b, uint32_t c, Random& random) {
        t.r3.u64 = b + kDevices;
        t.r4.u64 = Slot(random);
        // Small indices, and whole random registers (the original adds in 64
        // bits).
        t.r5.u64 = c % 2 == 0 ? random.Below(20) : random.Next();
      });
  Run(h, block, "SaveSlot::State (sub_820A57F0)", __imp__sub_820A57F0,
      hooks::SaveInterfaceSlotState, 6, slot_args);
  Run(h, block, "SaveSlot::Pending (sub_820A5808)", __imp__sub_820A5808,
      hooks::SaveInterfaceSlotPending, 8, [&](PPCContext& t, uint32_t b, uint32_t c, Random& random) {
        t.r3.u64 = b + kDevices;
        t.r4.u64 = Slot(random);
        if (c % 2 == 1) {  // big request words: the sum's carry past 32 bits
          for (uint32_t word = 960; word <= 980; word += 4) {
            guest::StoreU32(h.base, b + kSlots + 1000 * t.r4.u32 + word,
                            0xF0000000u | uint32_t(random.Next()));
          }
        }
      });
  Run(h, block, "SaveDevices::SetRootName (sub_820A5580)", __imp__sub_820A5580,
      hooks::SaveInterfaceSetRootName, 6,
      [&](PPCContext& t, uint32_t b, uint32_t, Random& random) {
        t.r3.u64 = b + kDevices;
        t.r4.u64 = data + kFileName;
      },
      true);
  const auto request = [&](PPCContext& t, uint32_t b, uint32_t first, Random& random) {
    // The display name, the file name, the path, then random words (the
    // registers from r4 up: PPCContext keeps r3 first).
    (&t.r4)[first - 4].u64 = data + kDisplayName;
    (&t.r4)[first - 3].u64 = data + kFileName;
    (&t.r4)[first - 2].u64 = data + kPath;
    for (uint32_t r = first + 3; r <= 10; r++) {
      (&t.r4)[r - 4].u64 = random.Next();
    }
    (void)b;
  };
  Run(h, block, "SaveSlot::RequestSave (sub_820A52E0)", __imp__sub_820A52E0,
      hooks::SaveInterfaceSlotRequestSave, 12,
      [&](PPCContext& t, uint32_t b, uint32_t, Random& random) {
        t.r3.u64 = b + kDevices;
        t.r4.u64 = Slot(random);
        request(t, b, 5, random);
        guest::StoreU32(h.base, t.r1.u32 + 84, uint32_t(random.Next()));
      },
      true);
  Run(h, block, "SaveSlot::RequestLoad (sub_820A53E8)", __imp__sub_820A53E8,
      hooks::SaveInterfaceSlotRequestLoad, 12,
      [&](PPCContext& t, uint32_t b, uint32_t, Random& random) {
        t.r3.u64 = b + kDevices;
        t.r4.u64 = Slot(random);
        request(t, b, 5, random);
      },
      true);

  // The display name's copy and the content's size.
  Run(h, block, "the display name's copy (sub_82295D48)", __imp__sub_82295D48,
      hooks::SaveInterfaceCopyDisplayName, 6,
      [&](PPCContext& t, uint32_t b, uint32_t, Random& random) {
        t.r3.u64 = b + kSlots + 504 + 2 * random.Below(4);
        t.r4.u64 = data + kDisplayName;
      },
      true);
  Run(h, block, "XContentCalculateSize (sub_82295C18)", __imp__sub_82295C18,
      hooks::SaveInterfaceContentSize, 24, [&](PPCContext& t, uint32_t, uint32_t c, Random& random) {
        static constexpr uint32_t kBytes[] = {1231088, 129400, 0, 1, 4096, 0xFFFFFFFF};
        if (c < 6) {
          t.r3.u64 = kBytes[c];
          t.r4.u64 = c == 5 ? 0xFFFFFFFF : 0;
        } else {
          t.r3.u64 = c % 2 == 0 ? random.Below(1 << 28) : random.Next();
          t.r4.u64 = c % 3 == 0 ? random.Below(16) : random.Next();
        }
      });

  // The interface's wrappers.
  Run(h, block, "SaveInterface::Init (sub_8209FFB8)", __imp__sub_8209FFB8,
      hooks::SaveInterfaceInit, 4,
      [&](PPCContext& t, uint32_t b, uint32_t, Random& random) { t.r3.u64 = b + kInterface; });
  const auto wrapper_args = [&](PPCContext& t, uint32_t, uint32_t, Random& random) {
    t.r3.u64 = Slot(random);
    t.r4.u64 = random.Below(20);
  };
  Run(h, block, "SaveInterface::SlotState (sub_820A0018)", __imp__sub_820A0018,
      hooks::SaveInterfaceSlotStateOf, 16, wrapper_args);
  Run(h, block, "SaveInterface::RequestDevice (sub_820A0208)", __imp__sub_820A0208,
      hooks::SaveInterfaceRequestDevice, 8, wrapper_args);
  Run(h, block, "SaveInterface::SaveCount (sub_820A02C8)", __imp__sub_820A02C8,
      hooks::SaveInterfaceSaveCount, 8, wrapper_args);
  Run(h, block, "SaveInterface::FreeBytes (sub_820A0320)", __imp__sub_820A0320,
      hooks::SaveInterfaceFreeBytes, 8, wrapper_args);
  Run(h, block, "SaveInterface::Pending (sub_820A0378)", __imp__sub_820A0378,
      hooks::SaveInterfacePending, 8, wrapper_args);
  Run(h, block, "SaveInterface::Device (sub_820A03D0)", __imp__sub_820A03D0,
      hooks::SaveInterfaceDevice, 8, wrapper_args);
  Run(h, block, "SaveInterface::SaveAt (sub_820A0428)", __imp__sub_820A0428,
      hooks::SaveInterfaceSaveAt, 8, wrapper_args);
  Run(h, block, "SaveInterface::Save (sub_820A0490)", __imp__sub_820A0490,
      hooks::SaveInterfaceSave, 12,
      [&](PPCContext& t, uint32_t b, uint32_t, Random& random) {
        t.r3.u64 = Slot(random);
        request(t, b, 4, random);
      },
      true);
  Run(h, block, "SaveInterface::Load (sub_820A0510)", __imp__sub_820A0510,
      hooks::SaveInterfaceLoad, 12,
      [&](PPCContext& t, uint32_t b, uint32_t, Random& random) {
        t.r3.u64 = Slot(random);
        request(t, b, 4, random);
      },
      true);
  Run(h, block, "SaveInterface::UserCount (sub_820A05D8)", __imp__sub_820A05D8,
      hooks::SaveInterfaceUserCount, 8, wrapper_args);
  Run(h, block, "SaveInterface::UserAt (sub_820A0620)", __imp__sub_820A0620,
      hooks::SaveInterfaceUserAt, 16,
      [&](PPCContext& t, uint32_t, uint32_t, Random& random) { t.r3.u64 = random.Below(7); });
}

}  // namespace harness
