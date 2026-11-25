// The save system's interface natively: adapters in front of
// src/save_interface/ (docs/functions/save-interface.md), with the register
// steps the harness shares (hooks/save_interface.h).

#include "hooks/save_interface.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "hooks/app_start_up.h"
#include "hooks/c_runtime_core.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "native/buffers.h"
#include "save_interface/save_devices.h"
#include "save_interface/save_interface.h"
#include "save_interface/save_layer.h"
#include "save_interface/slot_handlers.h"
#include "schema/files_and_saves.h"
#include "schema/save_interface.h"
#include "lifted/sdk.h"

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
              "code) and, with --native_heap, BuildList sub_82290378; with --native_heap, "
              "SaveDevices::Create sub_820A63C8 and the save devices' constructor sub_820A6160 (no "
              "adapter: only Create's step calls it) with SaveSlot::AllocBuffers sub_820A4560 (no "
              "adapter: only the constructor's plain C++ calls it; the notification listener "
              "stored as none without XNotifyCreateListener) and destructor sub_820A6110 with "
              "SaveDevices::Destroy sub_820A56E8 and SlotArray::Reset sub_820A55F0 (no adapters: "
              "only the destructor's plain C++ calls them; the listener's CloseHandle left out)); "
              "the save devices' slot handlers WaitForUser sub_820A4870, WaitForDevice "
              "sub_820A5878, Failed sub_820A4890, Ready sub_820A58D8 and EnumerateStep "
              "sub_820A59C0, the listener delegate's target sub_820A4800 and the framework's "
              "device list sub_820908D8, XGetOverlappedResult sub_82295C98 and GetOverlappedResult "
              "sub_82295F20 (no adapters: only the waits' plain C++ calls them; their waits, never "
              "taken here, WaitForSingleObject through the guest), "
              "the machines CheckStep sub_820A5A88, SaveStep sub_820A5BE8, LoadStep sub_820A5E68 "
              "and DeleteStep sub_820A6010 with the waits they poll (no adapters: WaitCloseItem "
              "sub_820A4C08, WaitDelete sub_820A5290, WaitThumbnail sub_820A5190, WaitClose "
              "sub_820A5070, WaitWrite sub_820A4E68, WaitRead sub_820A4FF8) and SaveDevices::Update "
              "sub_820A4610 (B5's adapter in hooks/notify.cpp; without this flag B5's body) "
              "natively instead of through the "
              "translated code (the same reads and writes; the original where a vtable target "
              "isn't ours; checked by --run_harnesses=save_interface)");

ITC_FLAG_DECLARE(bool, native_c_runtime_core);
namespace hooks {

namespace si = save_interface;

namespace {

// The registers from r4 up (our register view keeps r4 to r10 in order).
uint32_t Reg(lifted::Regs& ctx, int n) { return (&ctx.r4)[n - 4].u32; }

// A request's arguments from the register of the display name (r4 for the
// interface's wrappers, r5 for the slots').
si::Request RequestFrom(lifted::Regs& ctx, int first, uint32_t thumbnail_size) {
  return si::Request{Reg(ctx, first),     Reg(ctx, first + 1), Reg(ctx, first + 2),
                     Reg(ctx, first + 3), Reg(ctx, first + 4), Reg(ctx, first + 5),
                     thumbnail_size};
}

}  // namespace

ITC_REGS_STEP(SaveInterfaceAppDevices) {
  if (const auto devices = si::AppDevices(ctx.r3.u32)) {
    ctx.r3.u64 = *devices;
    return;
  }
  lifted::original::c820943B0(ctx);
}

ITC_REGS_STEP(SaveInterfaceAppUsers) {
  if (const auto users = si::AppUsers(ctx.r3.u32)) {
    ctx.r3.u64 = *users;
    return;
  }
  lifted::original::c820943C8(ctx);
}

ITC_REGS_STEP(SaveInterfaceFrameworkUsers) {
  ctx.r3.u64 = si::FrameworkUsers(ctx.r3.u32);
}

ITC_REGS_STEP(SaveInterfaceUsersCount) {
  ctx.r3.u64 = si::UsersCount(ctx.r3.u32);
}

ITC_REGS_STEP(SaveInterfaceUsersSignedIn) {
  ctx.r3.u64 = si::UsersSignedIn(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SaveInterfaceSlotRequestDevice) {
  si::SlotRequestDevice(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SaveInterfaceSlotDevice) {
  ctx.r3.u64 = si::SlotDevice(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SaveInterfaceSlotFreeBytes) {
  ctx.r3.u64 = si::SlotFreeBytes(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SaveInterfaceSlotSaveCount) {
  ctx.r3.u64 = si::SlotSaveCount(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SaveInterfaceSlotSaveAt) {
  ctx.r3.u64 = si::SlotSaveAt(ctx.r3.u32, ctx.r4.u32, ctx.r5.u64);
}

ITC_REGS_STEP(SaveInterfaceSlotState) {
  ctx.r3.u64 = si::SlotState(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SaveInterfaceSlotPending) {
  ctx.r3.u64 = si::SlotPending(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SaveInterfaceSetRootName) {
  si::SetRootName(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SaveInterfaceSlotRequestSave) {
  const auto answer = si::SlotRequestSave(ctx.r3.u32, ctx.r4.u32,
                                          RequestFrom(ctx, 5, guest::LoadU32(ctx.r1.u32 + 84)));
  if (!answer) {
    lifted::original::c820A52E0(ctx);
    return;
  }
  ctx.r3.u64 = *answer;
}

ITC_REGS_STEP(SaveInterfaceSlotRequestLoad) {
  const auto answer = si::SlotRequestLoad(ctx.r3.u32, ctx.r4.u32, RequestFrom(ctx, 5, 0));
  if (!answer) {
    lifted::original::c820A53E8(ctx);
    return;
  }
  ctx.r3.u64 = *answer;
}

ITC_REGS_STEP(SaveInterfaceCopyDisplayName) {
  si::CopyDisplayName(ctx.r3.u32, ctx.r4.u32);
}

void SaveInterfaceContentSize(lifted::Regs& ctx, uint8_t*) {
  ctx.r3.u64 = si::ContentSize(ctx.r3.u32, ctx.r4.u32);
}


// A wrapper of r3 (and r4) whose plain C++ answers r3, or nullopt.
#define ITC_SAVE_INTERFACE_WRAPPER(address, step, call) \
  ITC_REGS_STEP(step) {           \
    if (const auto answer = call) {                     \
      ctx.r3.u64 = *answer;                             \
      return;                                           \
    }                                                   \
    lifted::original::c##address(ctx);                    \
  }

ITC_SAVE_INTERFACE_WRAPPER(8209FFB8, SaveInterfaceInit, si::Init(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A0208, SaveInterfaceRequestDevice,
                           si::InterfaceRequestDevice(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A0018, SaveInterfaceSlotStateOf,
                           si::InterfaceSlotState(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A02C8, SaveInterfaceSaveCount,
                           si::InterfaceSaveCount(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A0320, SaveInterfaceFreeBytes,
                           si::InterfaceFreeBytes(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A0378, SaveInterfacePending, si::InterfacePending(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A03D0, SaveInterfaceDevice, si::InterfaceDevice(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(820A0428, SaveInterfaceSaveAt,
                           si::InterfaceSaveAt(ctx.r3.u32, ctx.r4.u64))
ITC_SAVE_INTERFACE_WRAPPER(820A0490, SaveInterfaceSave,
                           si::InterfaceSave(ctx.r3.u32, RequestFrom(ctx, 4, ctx.r10.u32)))
ITC_SAVE_INTERFACE_WRAPPER(820A0510, SaveInterfaceLoad,
                           si::InterfaceLoad(ctx.r3.u32, RequestFrom(ctx, 4, 0)))
ITC_SAVE_INTERFACE_WRAPPER(820A05D8, SaveInterfaceUserCount, si::InterfaceUserCount())
ITC_SAVE_INTERFACE_WRAPPER(820A0620, SaveInterfaceUserAt, si::InterfaceUserAt(ctx.r3.u32))

ITC_SAVE_INTERFACE_WRAPPER(8228F780, SaveInterfaceSlotOfController,
                           si::SlotOfController(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228F9B0, SaveInterfacePlayerRequestDevice,
                           si::PlayerRequestDevice(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228F9E8, SaveInterfacePlayerSlotState,
                           si::PlayerSlotState(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228FA90, SaveInterfacePlayerDevice, si::PlayerDevice(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228FAC8, SaveInterfacePlayerPending,
                           si::PlayerPending(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(822902A0, SaveInterfaceReplaysCount, si::ReplaysCount(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(822900E0, SaveInterfaceReplaysReadEntry,
                           si::ReplaysReadEntry(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228FBC0, SaveInterfaceGameDataLoad, si::GameDataLoad(ctx.r3.u32))
ITC_SAVE_INTERFACE_WRAPPER(8228FC50, SaveInterfaceGameDataCheck,
                           si::GameDataCheck(ctx.r3.u32))

#undef ITC_SAVE_INTERFACE_WRAPPER

ITC_REGS_STEP(SaveInterfaceBuffersCreate) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c8228F6B0(ctx);
    return;
  }
  if (const auto r3 = si::SaveBuffersCreate()) {
    ctx.r3.u64 = *r3;
  }
}

ITC_REGS_STEP(SaveInterfaceBuffersFreeReplayList) {
  const uint32_t records =
      schema::SaveInterfaceGlobals::save_buffers().records().address;
  if (!hooks::NativeHeapActive() || (records != 0 && !memory::IsPages(records))) {
    lifted::original::c8228F7E8(ctx);
    return;
  }
  if (const auto r3 = si::SaveBuffersFreeReplayList()) {
    ctx.r3.u64 = *r3;
  }
}

ITC_REGS_STEP(SaveInterfaceBuffersDestroy) {
  if (!si::SaveBuffersDestroyNative()) {
    lifted::original::c8228F8F8(ctx);
    return;
  }
  ctx.r3.u64 = si::SaveBuffersDestroy();
}

ITC_REGS_STEP(SaveInterfaceGameDataSave) {
  const uint64_t stack = ctx.r1.u64;
  const uint32_t frame = ctx.r1.u32 - 368;
  guest::StoreU32(frame, ctx.r1.u32);  // the frame's back chain, as the original's
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
      lifted::Enter(ctx, game::GetLocalTime);
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
      lifted::Enter(ctx, game::swprintf_8210A490);
    });
  };
  if (const auto r3 = si::GameDataSave(frame, calls)) {
    ctx.r3.u64 = *r3;
    return;
  }
  lifted::original::c8228FB00(ctx);
}

ITC_REGS_STEP(SaveInterfaceReplaysBuildList) {
  if (!si::ReplaysBuildListNative()) {
    lifted::original::c82290378(ctx);
    return;
  }
  const uint64_t stack = ctx.r1.u64;
  const uint32_t frame = ctx.r1.u32 - 144;
  guest::StoreU32(frame, ctx.r1.u32);  // the frame's back chain, as the original's
  const auto read_entry = [&](uint32_t index, uint32_t record, uint32_t entry) {
    ctx.r1.u64 = frame;
    ctx.r3.u64 = index;
    ctx.r4.u64 = record;
    ctx.r5.u64 = entry;
    ctx.lr = 0x822904E0;
    lifted::CallFromContext(ctx, 0x822900E0);  // the original (no adapter): a "rep" save's parse
    ctx.r1.u64 = stack;
    return ctx.r3.u32;
  };
  if (const auto r3 = si::ReplaysBuildList(ctx.r3.u32, frame, read_entry)) {
    ctx.r3.u64 = *r3;
    return;
  }
  lifted::original::c82290378(ctx);
}

ITC_REGS_STEP(SaveInterfaceReplaysSort) {
  if (!native::Buffers_NativeMemcpy()) {
    lifted::original::c8228F840(ctx);
    return;
  }
  if (const auto r3 = si::ReplaysSort(ctx.r3.u32, ctx.r1.u32 - 176 + 80)) {
    ctx.r3.u64 = *r3;
  }
}

ITC_REGS_STEP(SaveInterfacePlayTimeAdd) {
  si::PlayTimeAdd(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SaveInterfacePlayTimeUpdate) {
  ctx.r4.u64 = ctx.r3.u64 + 48;
  ctx.r3.u64 = si::PlayTimeUpdate(ctx.r3.u32, ctx.r1.u32 - 96 + 80);
}

namespace {

// The prologues' saves (the return address and r26 to r31 above a frame,
// written as 0 for the registers the view doesn't carry) aren't written
// since E5 step 6: nothing reads them. A frame's back chain is written only
// where a guest call is made with it as r1.

// The first slot's vtable slot 0 with r4 = 3 (the slots' vector deleting
// destructor), a guest call from SlotArray::Reset's frame at `sp` (its
// return address 0x820A5638, its back chain `above` written before the
// call); r3 its answer.
std::function<uint32_t(uint32_t)> DestroySlots(lifted::Regs& ctx, uint32_t sp,
                                               uint32_t above) {
  return [&ctx, sp, above](uint32_t slots) {
    const uint64_t stack = ctx.r1.u64;
    guest::StoreU32(sp, above);
    ctx.r1.u64 = sp;
    ctx.r3.u64 = slots;
    ctx.r4.u64 = 3;
    ctx.ctr.u64 = guest::LoadU32(guest::LoadU32(slots));
    ctx.lr = 0x820A5638;
    lifted::CallFromContext(ctx, ctx.ctr.u32);
    ctx.r1.u64 = stack;
    return ctx.r3.u32;
  };
}

// SaveDevices::Destroy's 96-byte frame below `sp`, and Reset's 96 under it
// when Destroy calls it: the slots' destructor's stack pointer, and the
// frame above it (its back chain).
struct SlotsFrame {
  uint32_t sp, above;
};
SlotsFrame DestroyFrames(uint32_t sp, uint32_t devices) {
  const uint32_t frame = sp - 96;
  if (schema::SaveDevices(devices).slots<schema::View>().address != 0) {
    return {frame - 96, frame};
  }
  return {frame, sp};
}

}  // namespace

ITC_REGS_STEP(SaveDevicesConstructSteps) {
  const uint64_t self = ctx.r3.u64;
  const uint32_t devices = ctx.r3.u32;
  const uint32_t block = ctx.r4.u32;
  const uint32_t saves = ctx.r5.u32;
  if (!hooks::NativeHeapActive() || !si::CanAllocBuffers(saves)) {
    lifted::original::c820A6160(ctx);
    return;
  }
  const uint64_t lr = ctx.lr;
  // CanAllocBuffers checked above: the plain C++ doesn't refuse.
  guest_call::InFrame(ctx, 176, 0x820A61D8, [&](uint32_t frame) {
    si::SaveDevicesConstruct(devices, block, saves, frame,
        [&](uint32_t loop, uint32_t offset, uint32_t key, uint32_t delegate) {
          ctx.r3.u64 = loop;
          ctx.r4.u64 = key;
          ctx.r5.u64 = delegate;
          ctx.ctr.u64 = guest::LoadU32(guest::LoadU32(loop) + offset);
          ctx.lr = offset == 8 ? 0x820A61D8 : 0x820A621C;
          lifted::CallFromContext(ctx, ctx.ctr.u32);
        });
  });
  ctx.lr = lr;
  ctx.r3.u64 = self;  // the original's mr r3,r31
}

ITC_REGS_STEP(SaveDevicesCreateSteps) {
  const uint32_t block = ctx.r3.u32;
  const uint32_t saves = ctx.r4.u32;
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820A63C8(ctx);
    return;
  }
  const uint64_t lr = ctx.lr;
  const auto devices = si::SaveDevicesCreate(saves, [&](uint32_t devices) {
    // The constructor from Create's 112-byte frame.
    guest_call::InFrame(ctx, 112, 0x820A6400, [&](uint32_t) {
      ctx.r3.u64 = devices;
      ctx.r4.u64 = block;
      ctx.r5.u64 = saves;
      SaveDevicesConstructSteps(ctx);
    });
    ctx.lr = lr;
  });
  if (!devices) {
    lifted::original::c820A63C8(ctx);
    return;
  }
  ctx.r3.u64 = *devices;
}

ITC_REGS_STEP(SaveSlotAllocBuffersSteps) {
  if (!hooks::NativeHeapActive() || !si::CanAllocBuffers(ctx.r4.u32)) {
    lifted::original::c820A4560(ctx);
    return;
  }
  const uint32_t slot = ctx.r3.u32;
  si::SlotAllocBuffers(slot, ctx.r4.u32);
  // The second operator new[]'s answer.
  ctx.r3.u64 = schema::SaveSlot(slot).open_failed();
}

ITC_REGS_STEP(SaveDevicesDeleteSteps) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820A6110(ctx);
    return;
  }
  const uint32_t devices = ctx.r3.u32;
  const uint32_t flags = ctx.r4.u32;
  // The original's 112-byte frame; Destroy's (and Reset's) under it.
  const uint64_t self = ctx.r3.u64;
  const SlotsFrame slots = DestroyFrames(ctx.r1.u32 - 112, devices);
  si::SaveDevicesDelete(devices, flags, DestroySlots(ctx, slots.sp, slots.above));
  ctx.r3.u64 = self;  // the original's mr r3,r31
}

ITC_REGS_STEP(SaveDevicesDestroySteps) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820A56E8(ctx);
    return;
  }
  const uint32_t devices = ctx.r3.u32;
  const SlotsFrame slots = DestroyFrames(ctx.r1.u32, devices);
  ctx.r3.u64 =
      si::SaveDevicesDestroy(devices, DestroySlots(ctx, slots.sp, slots.above));
}

ITC_REGS_STEP(SlotArrayResetSteps) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820A55F0(ctx);
    return;
  }
  // Reset's 96-byte frame.
  ctx.r3.u64 = si::SlotArrayReset(ctx.r3.u32,
                                  DestroySlots(ctx, ctx.r1.u32 - 96, ctx.r1.u32));
}

namespace {

// The call into an adapter as a CallSite: its stack pointer, return address
// and r27 to r31 (the caller's: not carried by the view, 0).
si::CallSite Site(const lifted::Regs& ctx) {
  return si::CallSite{ctx.r1.u32, uint32_t(ctx.lr), 0, 0, 0, 0, 0};
}

// A guest call to `function` from native code at `at` (its stack pointer
// and return address as the original's call has them; its nonvolatile
// registers, for the callee's prologue saves, aren't carried by the view);
// r3 and the other argument registers set by the caller. The stack pointer
// put back after.
void GuestCallAt(lifted::Regs& ctx, uint32_t function, const si::CallSite& at) {
  const uint64_t r1 = ctx.r1.u64;
  ctx.r1.u64 = at.sp;
  ctx.lr = at.lr;
  lifted::CallFromContext(ctx, function);
  ctx.r1.u64 = r1;
}

}  // namespace

ITC_REGS_STEP(SlotWaitForUserSteps) {
  ctx.r3.u64 = si::SlotWaitForUser(ctx.r4.u32);
}

ITC_REGS_STEP(SlotWaitForDeviceSteps) {
  ctx.r3.u64 = si::SlotWaitForDevice(ctx.r4.u32, Site(ctx));
}

ITC_REGS_STEP(SlotFailedSteps) {
  ctx.r3.u64 = si::SlotFailed(ctx.r4.u32);
}

ITC_REGS_STEP(SlotReadySteps) {
  ctx.r3.u64 = si::SlotReady(ctx.r4.u32, Site(ctx));
}

ITC_REGS_STEP(SlotEnumerateStepSteps) {
  ctx.r3.u64 = si::SlotEnumerateStep(ctx.r3.u32, ctx.r4.u32, Site(ctx));
}

ITC_REGS_STEP(DevicesOnEventSteps) {
  const uint32_t loop = ctx.r4.u32;
  si::DevicesOnEvent(ctx.r3.u32, loop, ctx.r5.u32, Site(ctx),
                     [&](uint32_t offset, uint32_t key, const si::CallSite& at) {
                       ctx.r3.u64 = loop;
                       ctx.r4.u64 = key;
                       GuestCallAt(ctx,
                                   guest::LoadU32(guest::LoadU32(loop) + offset), at);
                     });
}

ITC_REGS_STEP(FrameworkDeviceListSteps) {
  ctx.r3.u64 = si::FrameworkDeviceList(ctx.r3.u32);
}

save_interface::KernelCalls KernelCallsOf(lifted::Regs& ctx) {
  si::KernelCalls kernel;
  kernel.pcr = ctx.r13.u32;
  kernel.wait = [&ctx](uint32_t handle, const si::CallSite& at) {
    ctx.r3.u64 = handle;
    ctx.r4.s64 = -1;
    GuestCallAt(ctx, 0x822970A0, at);  // WaitForSingleObject
    return ctx.r3.u32;
  };
  kernel.close = [&ctx](uint32_t handle, const si::CallSite& at) {
    if (ITC_FLAG(native_c_runtime_core)) {
      // CloseHandle's plain C++ (docs/functions/c-runtime-core.md): NtClose,
      // no guest stack used; its r3 as the guest call left it.
      ctx.r3.u64 = c_runtime::core::CloseHandle(handle, hooks::CRuntimeCoreCalls(ctx));
      return;
    }
    ctx.r3.u64 = handle;
    GuestCallAt(ctx, 0x82294E20, at);  // CloseHandle
  };
  return kernel;
}

ITC_REGS_STEP(XGetOverlappedResultSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t overlapped = ctx.r3.u32, result = ctx.r4.u32, wait = ctx.r5.u32;
  ctx.r3.u64 = si::XGetOverlappedResult(overlapped, result, wait, Site(ctx), kernel);
}

ITC_REGS_STEP(GetOverlappedResultSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t handle = ctx.r3.u32, overlapped = ctx.r4.u32, bytes = ctx.r5.u32,
                 wait = ctx.r6.u32;
  ctx.r3.u64 = si::GetOverlappedResult(handle, overlapped, bytes, wait, Site(ctx), kernel);
}

ITC_REGS_STEP(SlotWaitCloseItemSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t slot = ctx.r4.u32;
  ctx.r3.s64 = si::SlotWaitCloseItem(slot, Site(ctx), kernel);
}

ITC_REGS_STEP(SlotWaitDeleteSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t slot = ctx.r4.u32;
  ctx.r3.s64 = si::SlotWaitDelete(slot, Site(ctx), kernel);
}

ITC_REGS_STEP(SlotWaitThumbnailSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t slot = ctx.r4.u32;
  ctx.r3.s64 = si::SlotWaitThumbnail(slot, Site(ctx), kernel);
}

ITC_REGS_STEP(SlotWaitCloseSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t slot = ctx.r4.u32;
  ctx.r3.s64 = si::SlotWaitClose(slot, Site(ctx), kernel);
}

ITC_REGS_STEP(SlotWaitWriteSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t slot = ctx.r4.u32;
  ctx.r3.s64 = si::SlotWaitWrite(slot, Site(ctx), kernel);
}

ITC_REGS_STEP(SlotWaitReadSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t slot = ctx.r4.u32;
  ctx.r3.s64 = si::SlotWaitRead(slot, Site(ctx), kernel);
}

ITC_REGS_STEP(SlotCheckStepSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t devices = ctx.r3.u32, slot = ctx.r4.u32;
  ctx.r3.u64 = si::SlotCheckStep(devices, slot, Site(ctx), kernel);
}

ITC_REGS_STEP(SlotSaveStepSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t devices = ctx.r3.u32, slot = ctx.r4.u32;
  ctx.r3.u64 = si::SlotSaveStep(devices, slot, Site(ctx), kernel);
}

ITC_REGS_STEP(SlotLoadStepSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t devices = ctx.r3.u32, slot = ctx.r4.u32;
  ctx.r3.u64 = si::SlotLoadStep(devices, slot, Site(ctx), kernel);
}

ITC_REGS_STEP(SlotDeleteStepSteps) {
  const auto kernel = KernelCallsOf(ctx);
  const uint32_t devices = ctx.r3.u32, slot = ctx.r4.u32;
  ctx.r3.u64 = si::SlotDeleteStep(devices, slot, Site(ctx), kernel);
}

ITC_REGS_STEP(SaveDevicesUpdateSteps) {
  const auto kernel = KernelCallsOf(ctx);
  ctx.r3.u64 = si::DevicesUpdate(ctx.r3.u64, ctx.r4.u64, Site(ctx), kernel,
      [&ctx](uint32_t function, uint64_t r3, uint64_t r4, const si::CallSite& at) {
        ctx.r3.u64 = r3;
        ctx.r4.u64 = r4;
        GuestCallAt(ctx, function, at);
        return ctx.r3.u64;
      });
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
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_save_interface)) { \
    hooks::step(ctx);                       \
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
ITC_NATIVE_ENTRY_ALWAYS(8209FFB8) {
  
  if (hooks::StartUpRecorded(ctx, 0x8209FFB8)) {
    return;
  }
  if (!ITC_FLAG(native_save_interface)) {
    lifted::original::c8209FFB8(ctx);
    return;
  }
  hooks::SaveInterfaceInit(ctx);
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
// The save devices' creation (from Framework::Init, once at boot) and
// destructor (vtable slot 0, never in play).
ITC_SAVE_INTERFACE_ADAPTER(820A63C8, SaveDevicesCreateSteps)
ITC_SAVE_INTERFACE_ADAPTER(820A6110, SaveDevicesDeleteSteps)
// No adapter (removed 2026-11-13: only SaveDevices::Create's step calls it in
// play) for SaveDevices::SaveDevices sub_820A6160. None for
// SaveSlot::AllocBuffers sub_820A4560 (only the constructor's plain C++
// calls it), SaveDevices::Destroy sub_820A56E8 and
// SlotArray::Reset sub_820A55F0 (only the destructor's plain C++ calls
// them): their register steps are the harness's.
// The save devices' slot handlers (through the save devices' +76 to +112,
// from SaveDevices::Update) and the listener delegate's target (never in
// play); the framework's +56 interface's slot 1 (through its vtable).
ITC_SAVE_INTERFACE_ADAPTER(820A4870, SlotWaitForUserSteps)
ITC_SAVE_INTERFACE_ADAPTER(820A5878, SlotWaitForDeviceSteps)
ITC_SAVE_INTERFACE_ADAPTER(820A4890, SlotFailedSteps)
ITC_SAVE_INTERFACE_ADAPTER(820A58D8, SlotReadySteps)
ITC_SAVE_INTERFACE_ADAPTER(820A59C0, SlotEnumerateStepSteps)
ITC_SAVE_INTERFACE_ADAPTER(820A4800, DevicesOnEventSteps)
ITC_SAVE_INTERFACE_ADAPTER(820908D8, FrameworkDeviceListSteps)
// No adapters (removed 2026-11-13: in play only the save slots' waits'
// plain C++ calls them; their other callers are Dropped or never in play)
// for XGetOverlappedResult sub_82295C98 and GetOverlappedResult
// sub_82295F20: their register steps above are the harness's.
// The save devices' machines (states 6 to 9); the waits they poll have no
// adapters (only their plain C++ calls them; the steps above are the
// harness's).
ITC_SAVE_INTERFACE_ADAPTER(820A5A88, SlotCheckStepSteps)
ITC_SAVE_INTERFACE_ADAPTER(820A5BE8, SlotSaveStepSteps)
ITC_SAVE_INTERFACE_ADAPTER(820A5E68, SlotLoadStepSteps)
ITC_SAVE_INTERFACE_ADAPTER(820A6010, SlotDeleteStepSteps)
