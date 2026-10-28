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
#include "save_interface/save_layer.h"

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

constexpr uint32_t kSaveDataSlot = 0x826C5B58;  // player 0's save data (SaveData::Of)

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
  // When not 0, [0x828161F0] (the save buffers' header) points here while
  // the case runs (put back after).
  uint32_t save_buffers = 0;
  // Bytes below the stack pointer the original leaves in its frame (filled
  // with random words first, compared after): from r1 - stack_below.
  uint32_t stack_below = 0;
  uint32_t stack_bytes = 0;
  // A second such range.
  uint32_t stack2_below = 0;
  uint32_t stack2_bytes = 0;
  // When not 0, player 0's save data slot ([0x826C5B58]) points here while
  // the case runs (put back after).
  uint32_t save_data = 0;
  // When not 0, `extra_bytes` there are compared too.
  uint32_t extra = 0;
  uint32_t extra_bytes = 0;
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
  const uint32_t live_buffers = guest::LoadU32(h.base, si::kSaveBuffers);
  const uint32_t live_save = guest::LoadU32(h.base, kSaveDataSlot);
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
    const uint32_t stack = t.r1.u32 - setup.stack_below;
    WriteRandom(h.base, stack, setup.stack_bytes / 4, random);
    const uint32_t stack2 = t.r1.u32 - setup.stack2_below;
    WriteRandom(h.base, stack2, setup.stack2_bytes / 4, random);
    if (setup.extra != 0) {
      WriteRandom(h.base, setup.extra, setup.extra_bytes / 4, random);
    }
    args(t, block.address, random);
    after = random;
    guest::StoreU32(h.base, si::kInterfaceGlobal, block.address + kInterface);
    if (setup.save_buffers != 0) {
      guest::StoreU32(h.base, si::kSaveBuffers, setup.save_buffers);
    }
    if (setup.save_data != 0) {
      guest::StoreU32(h.base, kSaveDataSlot, setup.save_data);
    }
    ITC_FLAG(native_save_interface) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_save_interface) = true;
    out[v].Word(guest::LoadU32(h.base, si::kInterfaceGlobal) - block.address);
    guest::StoreU32(h.base, si::kInterfaceGlobal, live);
    if (setup.save_buffers != 0) {
      out[v].Word(guest::LoadU32(h.base, si::kSaveBuffers) - setup.save_buffers);
      guest::StoreU32(h.base, si::kSaveBuffers, live_buffers);
    }
    if (setup.save_data != 0) {
      out[v].Word(guest::LoadU32(h.base, kSaveDataSlot) - setup.save_data);
      guest::StoreU32(h.base, kSaveDataSlot, live_save);
    }
    out[v].Bytes(Read(h.base, stack, setup.stack_bytes));
    out[v].Bytes(Read(h.base, stack2, setup.stack2_bytes));
    if (setup.extra != 0) {
      out[v].Bytes(Read(h.base, setup.extra, setup.extra_bytes));
    }
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
         bool fallbacks = false, CaseSetup base_setup = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    CaseSetup setup = base_setup;
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

// A player whose controller word ([0x826BABA4 + 4 x player]) lands at
// `word` in the scratch area (so the game's table isn't read or written),
// that word a controller 0 to 4.
uint32_t PlayerAt(uint8_t* base, uint32_t word, Random& random) {
  guest::StoreU32(base, word, random.Below(5));
  return (word - si::kControllers) >> 2;
}

constexpr uint32_t kRecords = 0x1400;  // Replays::Sort's records in the block
constexpr uint32_t kHeader = 0x300;    // a save buffers' header in the scratch area

// The save buffers' Create and Destroy over the layer's pages, the global
// pointing at the case's header while they run (put back after). Each
// version's pages are made and freed in the same order, so the layer's first
// fit gives both the same addresses; the header's words are compared as
// offsets from it, the buffers by whether they're the layer's pages after.
void SaveBuffers(Context& h) {
  const uint32_t live = guest::LoadU32(h.base, si::kSaveBuffers);
  {
    Results results("SaveBuffers::Create (sub_8228F6B0)");
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        t.r3.u64 = 0x12345678;
        // Case 3: a header already there (nothing done).
        guest::StoreU32(h.base, si::kSaveBuffers, c == 3 ? h.data + kHeader : 0);
        ITC_FLAG(native_save_interface) = v == 1;
        (v == 0 ? __imp__sub_8228F6B0 : hooks::SaveInterfaceBuffersCreate)(t, h.base);
        ITC_FLAG(native_save_interface) = true;
        const uint32_t header = guest::LoadU32(h.base, si::kSaveBuffers);
        out[v].Word(t.r3.u32 == 0x12345678 ? 1 : 0);
        if (c == 3) {
          out[v].Word(header - h.data);
        } else {
          out[v].Word(memory::IsPages(header) ? 1 : 0);
          for (uint32_t offset = 0; offset < 24; offset += 4) {
            const uint32_t word = guest::LoadU32(h.base, header + offset);
            out[v].Word(offset < 12 ? (word != 0 && memory::IsPages(word) ? 1 : 0) : word);
          }
          const uint32_t sizes[3] = {si::kGameDataBytes, si::kReplayBytes,
                                     si::kReplayRecordBytes};
          for (uint32_t i = 0; i < 3; i++) {
            const uint32_t buffer = guest::LoadU32(h.base, header + 4 * i);
            out[v].Bytes(Read(h.base, buffer, 64));
            out[v].Bytes(Read(h.base, buffer + sizes[i] - 4, 4));
          }
          out[v].Word(t.r3.u32 == guest::LoadU32(h.base, header + 8) ? 1 : 0);
          for (uint32_t i = 0; i < 3; i++) {
            memory::FreePages(guest::LoadU32(h.base, header + 4 * i));
          }
          memory::FreePages(header);
        }
        guest::StoreU32(h.base, si::kSaveBuffers, live);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "SaveBuffers::FreeReplayList (sub_8228F7E8)"
                               : "SaveBuffers::Destroy (sub_8228F8F8)");
    for (int c = 0; c < 12; c++) {
      Outputs out[2];
      const Random start = h.random;
      Random after;
      for (int v = 0; v < 2; v++) {
        Random random = start;
        const uint32_t header = memory::AllocPages(24, 4096);
        uint32_t buffers[4];
        for (uint32_t i = 0; i < 4; i++) {
          buffers[i] = random.Below(3) == 0 ? 0 : memory::AllocPages(1 + random.Below(20000), 4096);
          guest::StoreU32(h.base, header + 4 * i, buffers[i]);
        }
        guest::StoreU32(h.base, header + 16, uint32_t(random.Next()));
        guest::StoreU32(h.base, header + 20, uint32_t(random.Next()));
        after = random;
        PPCContext t = CallContext(h);
        t.r3.u64 = 0x12345678;
        guest::StoreU32(h.base, si::kSaveBuffers, header);
        ITC_FLAG(native_save_interface) = v == 1;
        if (which == 0) {
          (v == 0 ? __imp__sub_8228F7E8 : hooks::SaveInterfaceBuffersFreeReplayList)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_8228F8F8 : hooks::SaveInterfaceBuffersDestroy)(t, h.base);
        }
        ITC_FLAG(native_save_interface) = true;
        const uint32_t global = guest::LoadU32(h.base, si::kSaveBuffers);
        out[v].Word(global == header ? 1 : global);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        for (uint32_t i = 0; i < 4; i++) {
          const bool kept = buffers[i] != 0 && memory::IsPages(buffers[i]);
          out[v].Word(kept ? 1 : 0);
          if (kept) {
            memory::FreePages(buffers[i]);
          }
        }
        const bool header_kept = memory::IsPages(header);
        out[v].Word(header_kept ? 1 : 0);
        if (header_kept) {
          for (uint32_t offset = 0; offset < 16; offset += 4) {
            const uint32_t word = guest::LoadU32(h.base, header + offset);
            out[v].Word(word == buffers[offset / 4] ? 1 : word);
          }
          out[v].Bytes(Read(h.base, header + 16, 8));
          memory::FreePages(header);
        }
        guest::StoreU32(h.base, si::kSaveBuffers, live);
      }
      h.random = after;
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

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

  // The front end's save layer.
  Run(h, block, "SaveSlots::OfController (sub_8228F780)", __imp__sub_8228F780,
      hooks::SaveInterfaceSlotOfController, 24,
      [&](PPCContext& t, uint32_t, uint32_t, Random& random) { t.r3.u64 = random.Below(6); });
  const auto player_args = [&](PPCContext& t, uint32_t, uint32_t, Random& random) {
    t.r3.u64 = PlayerAt(h.base, data + 0x400 + 4 * random.Below(16), random);
  };
  Run(h, block, "SaveSlots::RequestDevice (sub_8228F9B0)", __imp__sub_8228F9B0,
      hooks::SaveInterfacePlayerRequestDevice, 16, player_args);
  Run(h, block, "SaveSlots::State (sub_8228F9E8)", __imp__sub_8228F9E8,
      hooks::SaveInterfacePlayerSlotState, 16, player_args);
  Run(h, block, "SaveSlots::Device (sub_8228FA90)", __imp__sub_8228FA90,
      hooks::SaveInterfacePlayerDevice, 16, player_args);
  Run(h, block, "SaveSlots::Pending (sub_8228FAC8)", __imp__sub_8228FAC8,
      hooks::SaveInterfacePlayerPending, 16, player_args);
  SaveBuffers(h);
  {
    CaseSetup sort;
    sort.save_buffers = data + kHeader;
    sort.stack_below = 96;
    sort.stack_bytes = 36;
    Run(
        h, block, "Replays::Sort (sub_8228F840)", __imp__sub_8228F840,
        hooks::SaveInterfaceReplaysSort, 24,
        [&](PPCContext& t, uint32_t b, uint32_t c, Random& random) {
          const uint32_t count = c < 3 ? c : random.Below(13);
          guest::StoreU32(h.base, data + kHeader + 20, count);
          for (uint32_t i = 0; i < 12; i++) {  // numbers with ties, some negative
            guest::StoreU32(h.base, b + kRecords + 36 * i + 28, random.Below(9) - 2);
          }
          t.r3.u64 = b + kRecords;
        },
        false, false, sort);
  }
  Run(h, block, "PlayTime::Add (sub_821109B8)", __imp__sub_821109B8,
      hooks::SaveInterfacePlayTimeAdd, 24, [&](PPCContext& t, uint32_t b, uint32_t c, Random& random) {
        const uint32_t stats = b + 0x200 * random.Below(4);
        if (c % 2 == 0) {  // the play time's fields in range, a short time since
          guest::StoreU8(h.base, stats + 6036, uint8_t(random.Below(60)));
          guest::StoreU8(h.base, stats + 6037, uint8_t(random.Below(60)));
          guest::StoreU8(h.base, stats + 6038, uint8_t(random.Below(30)));
          guest::StoreU32(h.base, stats + 6040,
                          guest::LoadU32(h.base, si::kPlayTimeFrames) - random.Below(500000));
        }
        t.r3.u64 = data + 0x500;
        t.r4.u64 = stats;
      });
  {
    CaseSetup update;
    update.stack_below = 16;
    update.stack_bytes = 8;
    Run(
        h, block, "PlayTime::Update (sub_82112838)", __imp__sub_82112838,
        hooks::SaveInterfacePlayTimeUpdate, 12,
        [&](PPCContext& t, uint32_t b, uint32_t, Random& random) {
          t.r3.u64 = b + 0x100 * random.Below(4);
        },
        false, false, update);
  }

  // The game data's save, load and check.
  {
    // Save: its frame r1 - 368; the name (+96), the time (+80) and the
    // parameter area (+32) from r1 - 368 to r1 - 16 (the saved r31 and lr
    // above are the prologue's). PlayTime::Update's out below it (r1 - 384)
    // isn't compared: the original's SaveSlots::OfController saves its
    // registers over it afterwards (r31 there), which the plain C++ doesn't.
    CaseSetup save;
    save.stack_below = 368;
    save.stack_bytes = 352;
    save.save_data = block.address + 0x400;
    Run(
        h, block, "GameData::Save (sub_8228FB00)", __imp__sub_8228FB00,
        hooks::SaveInterfaceGameDataSave, 12,
        [&](PPCContext&, uint32_t b, uint32_t, Random& random) {
          const uint32_t stats = b + 0x400 + 48;
          guest::StoreU8(h.base, stats + 6036, uint8_t(random.Below(60)));
          guest::StoreU8(h.base, stats + 6037, uint8_t(random.Below(60)));
          guest::StoreU8(h.base, stats + 6038, uint8_t(random.Below(30)));
          guest::StoreU32(h.base, stats + 6040,
                          guest::LoadU32(h.base, si::kPlayTimeFrames) - random.Below(500000));
        },
        false, false, save);
  }
  {
    const uint32_t buffer = memory::Alloc(si::kGameDataBytes, 0);
    CaseSetup load;
    load.save_buffers = data + kHeader;
    load.extra = buffer;
    load.extra_bytes = si::kGameDataBytes;
    Run(
        h, block, "GameData::Load (sub_8228FBC0)", __imp__sub_8228FBC0,
        hooks::SaveInterfaceGameDataLoad, 12,
        [&](PPCContext& t, uint32_t, uint32_t, Random& random) {
          guest::StoreU32(h.base, data + kHeader + 0, buffer);
          t.r3.u64 = PlayerAt(h.base, data + 0x400 + 4 * random.Below(16), random);
        },
        false, false, load);
    memory::Free(buffer);
  }
  Run(h, block, "GameData::Check (sub_8228FC50)", __imp__sub_8228FC50,
      hooks::SaveInterfaceGameDataCheck, 32, [&](PPCContext& t, uint32_t b, uint32_t c, Random& random) {
        // Up to four enumerated saves at b + kRecords, each named SystemData.bin,
        // a replay's name, a prefix of it or random bytes; the free bytes
        // around the size (188,416).
        static constexpr char kNames[][16] = {"SystemData.bin", "rep_1_2_3", "SystemData.bi",
                                              "SystemData.binx"};
        for (uint32_t e = 0; e < 4; e++) {
          const uint32_t name = b + kRecords + 308 * e + 264;
          const uint32_t pick = random.Below(6);
          if (pick < 4) {
            Write(h.base, name, kNames[pick], 16);
          } else {
            RandomString(h.base, name, 20, false, random);
          }
        }
        for (uint32_t s = 0; s < 3; s++) {
          const uint32_t slot = b + kSlots + 1000 * s;
          guest::StoreU32(h.base, slot + 80, b + kRecords);
          guest::StoreU32(h.base, slot + 984, random.Below(5));
          guest::StoreU32(h.base, slot + 40, uint32_t(random.Next()));
          guest::StoreU32(h.base, slot + 44, 188416 - 4096 + random.Below(8192));
          if (c % 2 == 0) {  // nothing pending half the time
            for (uint32_t word = 960; word <= 980; word += 4) {
              guest::StoreU32(h.base, slot + word, 0);
            }
          }
        }
        t.r3.u64 = PlayerAt(h.base, data + 0x400 + 4 * random.Below(16), random);
      });
}

}  // namespace harness
