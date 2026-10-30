// The save system's interface natively: adapters in front of
// src/save_interface/ (docs/functions/save-interface.md), with the register
// steps the harness shares (hooks/save_interface.h).

#include "hooks/save_interface.h"

#include "base/flags.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "native/buffers.h"
#include "save_interface/save_interface.h"
#include "save_interface/save_layer.h"

ITC_FLAG_BOOL(native_save_interface, true, "ITC",
              "Run the save system's interface (docs/functions/save-interface.md: "
              "SaveInterface::Init sub_8209FFB8, its wrappers SlotState sub_820A0018, "
              "RequestDevice sub_820A0208, SaveCount sub_820A02C8, FreeBytes sub_820A0320, "
              "Pending sub_820A0378, Device sub_820A03D0, SaveAt sub_820A0428, Save "
              "sub_820A0490, Load sub_820A0510, UserCount sub_820A05D8 and UserAt "
              "sub_820A0620; the app's getters sub_820943B0 and sub_820943C8 and the +36 "
              "interface's sub_82493700; the users' slots sub_82490B08 and Users::SignedIn "
              "sub_820996D8; the save devices' slots sub_820A5750, sub_820A5768, "
              "sub_820A5780, sub_820A5798, sub_820A57B0, sub_820A57F0, sub_820A5808, "
              "SetRootName sub_820A5580, RequestSave sub_820A52E0 and RequestLoad "
              "sub_820A53E8; the display name's copy sub_82295D48, no adapter: only their "
              "plain C++ calls it in play; XContentCalculateSize sub_82295C18; the front end's "
              "save layer: SaveSlots::OfController sub_8228F780, the slot wrappers by player "
              "sub_8228F9B0, sub_8228F9E8, sub_8228FA90 and sub_8228FAC8, with --native_heap "
              "the save buffers' Create sub_8228F6B0, FreeReplayList sub_8228F7E8 (no adapter: "
              "only Destroy's plain C++ calls it) and Destroy sub_8228F8F8, Replays::Sort "
              "sub_8228F840 (its copies through memcpy's plain C++), the play time's "
              "sub_821109B8 and PlayTime::Update sub_82112838; GameData::Save sub_8228FB00 (GetLocalTime "
              "through the guest), Load sub_8228FBC0 and the game data's check sub_8228FC50; the replays' Count sub_822902A0, "
              "ReadEntry sub_822900E0 (its parse of a replay's name, never in play, on the translated "
              "code) and, with --native_heap, BuildList sub_82290378) natively instead of through the "
              "translated code (the same reads and writes; the original where a vtable target "
              "isn't ours; checked by --run_harnesses=save_interface)");

namespace hooks {

namespace si = save_interface;

namespace {

// The registers from r4 up (PPCContext keeps r3 first, then r0 to r2, then
// r4 on).
uint32_t Reg(PPCContext& ctx, int n) { return (&ctx.r4)[n - 4].u32; }

// A request's arguments from the register of the display name (r4 for the
// interface's wrappers, r5 for the slots').
si::Request RequestFrom(PPCContext& ctx, int first, uint32_t thumbnail_size) {
  return si::Request{Reg(ctx, first),     Reg(ctx, first + 1), Reg(ctx, first + 2),
                     Reg(ctx, first + 3), Reg(ctx, first + 4), Reg(ctx, first + 5),
                     thumbnail_size};
}

}  // namespace

void SaveInterfaceAppDevices(PPCContext& ctx, uint8_t* base) {
  if (const auto devices = si::AppDevices(base, ctx.r3.u32)) {
    ctx.r3.u64 = *devices;
    return;
  }
  __imp__sub_820943B0(ctx, base);
}

void SaveInterfaceAppUsers(PPCContext& ctx, uint8_t* base) {
  if (const auto users = si::AppUsers(base, ctx.r3.u32)) {
    ctx.r3.u64 = *users;
    return;
  }
  __imp__sub_820943C8(ctx, base);
}

void SaveInterfaceFrameworkUsers(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = si::FrameworkUsers(base, ctx.r3.u32);
}

void SaveInterfaceUsersCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = si::UsersCount(base, ctx.r3.u32);
}

void SaveInterfaceUsersSignedIn(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = si::UsersSignedIn(base, ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfaceSlotRequestDevice(PPCContext& ctx, uint8_t* base) {
  si::SlotRequestDevice(base, ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfaceSlotDevice(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = si::SlotDevice(base, ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfaceSlotFreeBytes(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = si::SlotFreeBytes(base, ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfaceSlotSaveCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = si::SlotSaveCount(base, ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfaceSlotSaveAt(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = si::SlotSaveAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u64);
}

void SaveInterfaceSlotState(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = si::SlotState(base, ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfaceSlotPending(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = si::SlotPending(base, ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfaceSetRootName(PPCContext& ctx, uint8_t* base) {
  si::SetRootName(base, ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfaceSlotRequestSave(PPCContext& ctx, uint8_t* base) {
  const auto answer = si::SlotRequestSave(base, ctx.r3.u32, ctx.r4.u32,
                                          RequestFrom(ctx, 5, guest::LoadU32(base, ctx.r1.u32 + 84)));
  if (!answer) {
    __imp__sub_820A52E0(ctx, base);
    return;
  }
  ctx.r3.u64 = *answer;
}

void SaveInterfaceSlotRequestLoad(PPCContext& ctx, uint8_t* base) {
  const auto answer = si::SlotRequestLoad(base, ctx.r3.u32, ctx.r4.u32, RequestFrom(ctx, 5, 0));
  if (!answer) {
    __imp__sub_820A53E8(ctx, base);
    return;
  }
  ctx.r3.u64 = *answer;
}

void SaveInterfaceCopyDisplayName(PPCContext& ctx, uint8_t* base) {
  si::CopyDisplayName(base, ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfaceContentSize(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = si::ContentSize(ctx.r3.u32, ctx.r4.u32);
}


// A wrapper of r3 (and r4) whose plain C++ answers r3, or nullopt.
#define ITC_SAVE_INTERFACE_WRAPPER(address, step, call) \
  void step(PPCContext& ctx, uint8_t* base) {           \
    if (const auto answer = call) {                     \
      ctx.r3.u64 = *answer;                             \
      return;                                           \
    }                                                   \
    __imp__sub_##address(ctx, base);                    \
  }

ITC_SAVE_INTERFACE_WRAPPER(8209FFB8, SaveInterfaceInit, si::Init(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A0208, SaveInterfaceRequestDevice,
                           si::InterfaceRequestDevice(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A0018, SaveInterfaceSlotStateOf,
                           si::InterfaceSlotState(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A02C8, SaveInterfaceSaveCount,
                           si::InterfaceSaveCount(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A0320, SaveInterfaceFreeBytes,
                           si::InterfaceFreeBytes(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A0378, SaveInterfacePending, si::InterfacePending(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A03D0, SaveInterfaceDevice, si::InterfaceDevice(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A0428, SaveInterfaceSaveAt,
                           si::InterfaceSaveAt(base, ctx.r3.u32, ctx.r4.u64))
ITC_SAVE_INTERFACE_WRAPPER(820A0490, SaveInterfaceSave,
                           si::InterfaceSave(base, ctx.r3.u32, RequestFrom(ctx, 4, ctx.r10.u32)))
ITC_SAVE_INTERFACE_WRAPPER(820A0510, SaveInterfaceLoad,
                           si::InterfaceLoad(base, ctx.r3.u32, RequestFrom(ctx, 4, 0)))
ITC_SAVE_INTERFACE_WRAPPER(820A05D8, SaveInterfaceUserCount, si::InterfaceUserCount(base))
ITC_SAVE_INTERFACE_WRAPPER(820A0620, SaveInterfaceUserAt, si::InterfaceUserAt(base, ctx.r3.u32))

ITC_SAVE_INTERFACE_WRAPPER(8228F780, SaveInterfaceSlotOfController,
                           si::SlotOfController(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228F9B0, SaveInterfacePlayerRequestDevice,
                           si::PlayerRequestDevice(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228F9E8, SaveInterfacePlayerSlotState,
                           si::PlayerSlotState(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228FA90, SaveInterfacePlayerDevice, si::PlayerDevice(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228FAC8, SaveInterfacePlayerPending,
                           si::PlayerPending(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(822902A0, SaveInterfaceReplaysCount, si::ReplaysCount(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(822900E0, SaveInterfaceReplaysReadEntry,
                           si::ReplaysReadEntry(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228FBC0, SaveInterfaceGameDataLoad, si::GameDataLoad(base, ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228FC50, SaveInterfaceGameDataCheck,
                           si::GameDataCheck(base, ctx.r3.u32))

#undef ITC_SAVE_INTERFACE_WRAPPER

void SaveInterfaceBuffersCreate(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    __imp__sub_8228F6B0(ctx, base);
    return;
  }
  if (const auto r3 = si::SaveBuffersCreate(base)) {
    ctx.r3.u64 = *r3;
  }
}

void SaveInterfaceBuffersFreeReplayList(PPCContext& ctx, uint8_t* base) {
  const uint32_t records = guest::LoadU32(base, guest::LoadU32(base, si::kSaveBuffers) + 12);
  if (!hooks::NativeHeapActive() || (records != 0 && !memory::IsPages(records))) {
    __imp__sub_8228F7E8(ctx, base);
    return;
  }
  if (const auto r3 = si::SaveBuffersFreeReplayList(base)) {
    ctx.r3.u64 = *r3;
  }
}

void SaveInterfaceBuffersDestroy(PPCContext& ctx, uint8_t* base) {
  if (!si::SaveBuffersDestroyNative(base)) {
    __imp__sub_8228F8F8(ctx, base);
    return;
  }
  ctx.r3.u64 = si::SaveBuffersDestroy(base);
}

void SaveInterfaceGameDataSave(PPCContext& ctx, uint8_t* base) {
  const uint64_t stack = ctx.r1.u64;
  const uint32_t frame = ctx.r1.u32 - 368;
  guest::StoreU32(base, frame, ctx.r1.u32);  // the frame's back chain, as the original's
  const auto in_frame = [&](uint32_t lr, auto call) {
    ctx.r1.u64 = frame;
    ctx.lr = lr;
    call();
    ctx.r1.u64 = stack;
  };
  si::GameDataCalls calls;
  calls.local_time = [&](uint32_t out) {
    in_frame(0x8228FB34, [&] {
      ctx.r3.u64 = out;
      sub_822968B8(ctx, base);
    });
  };
  calls.swprintf = [&](const std::array<uint64_t, 8>& regs) {
    in_frame(0x8228FB60, [&] {
      ctx.r3.u64 = regs[0];
      ctx.r4.u64 = regs[1];
      ctx.r5.u64 = regs[2];
      ctx.r6.u64 = regs[3];
      ctx.r7.u64 = regs[4];
      ctx.r8.u64 = regs[5];
      ctx.r9.u64 = regs[6];
      ctx.r10.u64 = regs[7];
      sub_8210A490(ctx, base);
    });
  };
  if (const auto r3 = si::GameDataSave(base, frame, calls)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_8228FB00(ctx, base);
}

void SaveInterfaceReplaysBuildList(PPCContext& ctx, uint8_t* base) {
  if (!si::ReplaysBuildListNative(base)) {
    __imp__sub_82290378(ctx, base);
    return;
  }
  const uint64_t stack = ctx.r1.u64;
  const uint32_t frame = ctx.r1.u32 - 144;
  guest::StoreU32(base, frame, ctx.r1.u32);  // the frame's back chain, as the original's
  const auto read_entry = [&](uint32_t index, uint32_t record, uint32_t entry) {
    ctx.r1.u64 = frame;
    ctx.r3.u64 = index;
    ctx.r4.u64 = record;
    ctx.r5.u64 = entry;
    ctx.lr = 0x822904E0;
    sub_822900E0(ctx, base);  // the original (no adapter): a "rep" save's parse
    ctx.r1.u64 = stack;
    return ctx.r3.u32;
  };
  if (const auto r3 = si::ReplaysBuildList(base, ctx.r3.u32, frame, read_entry)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_82290378(ctx, base);
}

void SaveInterfaceReplaysSort(PPCContext& ctx, uint8_t* base) {
  if (!native::Buffers_NativeMemcpy()) {
    __imp__sub_8228F840(ctx, base);
    return;
  }
  if (const auto r3 = si::ReplaysSort(base, ctx.r3.u32, ctx.r1.u32 - 176 + 80)) {
    ctx.r3.u64 = *r3;
  }
}

void SaveInterfacePlayTimeAdd(PPCContext& ctx, uint8_t* base) {
  si::PlayTimeAdd(base, ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfacePlayTimeUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.r4.u64 = ctx.r3.u64 + 48;
  ctx.r3.u64 = si::PlayTimeUpdate(base, ctx.r3.u32, ctx.r1.u32 - 96 + 80);
}

}  // namespace hooks

// No adapter (removed 2026-10-28: in play only the request setters' plain
// C++ calls it; the flag switches them, and the register step above stays
// for the harness) for the display name's copy sub_82295D48. None for
// SaveBuffers::FreeReplayList sub_8228F7E8 either: its one caller,
// SaveBuffers::Destroy, is ours (its register step is the harness's).
// No adapter (removed 2026-10-28: only the plain C++ of `Replays::BuildList` calls it in play)
// for Replays::Sort sub_8228F840.
// No adapter (removed 2026-10-28: only the plain C++ of `Replays::BuildList` (its guest call for a `rep` save, where the plain C++ hands back, now reaches the original directly) calls it in play)
// for Replays::ReadEntry sub_822900E0.
// No adapter (removed 2026-10-28: only the plain C++ of `Replays::BuildList` calls it in play)
// for Replays::Count sub_822902A0.
// No adapter (removed 2026-10-28: only the plain C++ of the save layer (the slot wrappers by player, `GameData::Save`, `Load` and `Check`, the replays' `Count`, `ReadEntry` and `BuildList`) calls it in play)
// for SaveSlots::OfController sub_8228F780.
// No adapter (removed 2026-10-28: only the plain C++ of `GameData::Check` and the replays' `Count` and `ReadEntry` calls it in play)
// for SaveInterface::SaveAt sub_820A0428.
// No adapter (removed 2026-10-28: only the plain C++ of `SaveSlots::Pending`, `GameData::Check` and the replays' `Count` and `ReadEntry` calls it in play)
// for SaveInterface::Pending sub_820A0378.
// No adapter (removed 2026-10-28: only the plain C++ of `GameData::Check` and the replays' `Count` and `BuildList` calls it in play)
// for SaveInterface::SaveCount sub_820A02C8.
// No adapter (removed 2026-10-28: only the plain C++ of `SaveSlots::State`, `GameData::Check` and the replays' `Count` and `ReadEntry` calls it in play)
// for SaveInterface::SlotState sub_820A0018.
// No adapter (removed 2026-10-28: only `GameData::Load`'s plain C++ calls it in play)
// for SaveInterface::Load sub_820A0510.
// No adapter (removed 2026-10-28: only `GameData::Save`'s plain C++ calls it in play)
// for SaveInterface::Save sub_820A0490.
// No adapter (removed 2026-10-28: only `GameData::Check`'s plain C++ calls it in play)
// for SaveInterface::FreeBytes sub_820A0320.
// No adapter (removed 2026-10-28: only `SaveSlots::OfController`'s plain C++ calls it in play)
// for SaveInterface::UserAt sub_820A0620.
// No adapter (removed 2026-10-28: only `SaveSlots::OfController`'s plain C++ calls it in play)
// for SaveInterface::UserCount sub_820A05D8.
// No adapter (removed 2026-10-28: only `SaveSlots::Device`'s plain C++ calls it in play)
// for SaveInterface::Device sub_820A03D0.
// No adapter (removed 2026-10-28: only `SaveSlots::RequestDevice`'s plain C++ calls it in play)
// for SaveInterface::RequestDevice sub_820A0208.

#define ITC_SAVE_INTERFACE_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {            \
    REX_FUNC_PROLOGUE();                          \
    if (!ITC_FLAG(native_save_interface)) {       \
      __imp__sub_##address(ctx, base);            \
      return;                                     \
    }                                             \
    hooks::step(ctx, base);                       \
  }

ITC_SAVE_INTERFACE_ADAPTER(820943B0, SaveInterfaceAppDevices)
ITC_SAVE_INTERFACE_ADAPTER(820943C8, SaveInterfaceAppUsers)
ITC_SAVE_INTERFACE_ADAPTER(82493700, SaveInterfaceFrameworkUsers)
ITC_SAVE_INTERFACE_ADAPTER(82490B08, SaveInterfaceUsersCount)
ITC_SAVE_INTERFACE_ADAPTER(820996D8, SaveInterfaceUsersSignedIn)
ITC_SAVE_INTERFACE_ADAPTER(820A5750, SaveInterfaceSlotRequestDevice)
ITC_SAVE_INTERFACE_ADAPTER(820A5768, SaveInterfaceSlotDevice)
ITC_SAVE_INTERFACE_ADAPTER(820A5780, SaveInterfaceSlotFreeBytes)
ITC_SAVE_INTERFACE_ADAPTER(820A5798, SaveInterfaceSlotSaveCount)
ITC_SAVE_INTERFACE_ADAPTER(820A57B0, SaveInterfaceSlotSaveAt)
ITC_SAVE_INTERFACE_ADAPTER(820A57F0, SaveInterfaceSlotState)
ITC_SAVE_INTERFACE_ADAPTER(820A5808, SaveInterfaceSlotPending)
ITC_SAVE_INTERFACE_ADAPTER(820A5580, SaveInterfaceSetRootName)
ITC_SAVE_INTERFACE_ADAPTER(820A52E0, SaveInterfaceSlotRequestSave)
ITC_SAVE_INTERFACE_ADAPTER(820A53E8, SaveInterfaceSlotRequestLoad)
ITC_SAVE_INTERFACE_ADAPTER(82295C18, SaveInterfaceContentSize)
// SaveInterface::Init's adapter written out: the app_start_up harness's
// recorder first.
extern "C" REX_FUNC(sub_8209FFB8) {
  REX_FUNC_PROLOGUE();
  if (hooks::StartUpRecorded(ctx, 0x8209FFB8)) {
    return;
  }
  if (!ITC_FLAG(native_save_interface)) {
    __imp__sub_8209FFB8(ctx, base);
    return;
  }
  hooks::SaveInterfaceInit(ctx, base);
}
ITC_SAVE_INTERFACE_ADAPTER(8228F9B0, SaveInterfacePlayerRequestDevice)
ITC_SAVE_INTERFACE_ADAPTER(8228F9E8, SaveInterfacePlayerSlotState)
ITC_SAVE_INTERFACE_ADAPTER(8228FA90, SaveInterfacePlayerDevice)
ITC_SAVE_INTERFACE_ADAPTER(8228FAC8, SaveInterfacePlayerPending)
ITC_SAVE_INTERFACE_ADAPTER(8228F6B0, SaveInterfaceBuffersCreate)
ITC_SAVE_INTERFACE_ADAPTER(8228F8F8, SaveInterfaceBuffersDestroy)
ITC_SAVE_INTERFACE_ADAPTER(821109B8, SaveInterfacePlayTimeAdd)
ITC_SAVE_INTERFACE_ADAPTER(82112838, SaveInterfacePlayTimeUpdate)
ITC_SAVE_INTERFACE_ADAPTER(8228FB00, SaveInterfaceGameDataSave)
ITC_SAVE_INTERFACE_ADAPTER(8228FBC0, SaveInterfaceGameDataLoad)
ITC_SAVE_INTERFACE_ADAPTER(8228FC50, SaveInterfaceGameDataCheck)
ITC_SAVE_INTERFACE_ADAPTER(82290378, SaveInterfaceReplaysBuildList)
