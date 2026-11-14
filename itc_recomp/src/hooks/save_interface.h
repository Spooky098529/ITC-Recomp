// The save system's interface's register steps (src/save_interface/,
// docs/functions/save-interface.md), shared by the adapters
// (src/hooks/save_interface.cpp, behind --native_save_interface) and the
// harness (without the flag). Each takes the original's registers, leaves r3
// as the original does, and runs the original (`__imp__`) where the plain
// C++ answers nullopt (a vtable target that isn't ours).

#pragma once

#include "guest.h"
#include "save_interface/slot_handlers.h"

namespace hooks {

// The getters: r3 = the object.
void SaveInterfaceAppDevices(PPCContext& ctx, uint8_t* base);      // sub_820943B0
void SaveInterfaceAppUsers(PPCContext& ctx, uint8_t* base);        // sub_820943C8
void SaveInterfaceFrameworkUsers(PPCContext& ctx, uint8_t* base);  // sub_82493700
void SaveInterfaceUsersCount(PPCContext& ctx, uint8_t* base);      // sub_82490B08
// r3 = the users, r4 = an index.
void SaveInterfaceUsersSignedIn(PPCContext& ctx, uint8_t* base);   // sub_820996D8

// The save devices' slots: r3 = the save devices, r4 = a slot (r5 the
// index for SaveAt).
void SaveInterfaceSlotRequestDevice(PPCContext& ctx, uint8_t* base);  // sub_820A5750
void SaveInterfaceSlotDevice(PPCContext& ctx, uint8_t* base);         // sub_820A5768
void SaveInterfaceSlotFreeBytes(PPCContext& ctx, uint8_t* base);      // sub_820A5780
void SaveInterfaceSlotSaveCount(PPCContext& ctx, uint8_t* base);      // sub_820A5798
void SaveInterfaceSlotSaveAt(PPCContext& ctx, uint8_t* base);         // sub_820A57B0
void SaveInterfaceSlotState(PPCContext& ctx, uint8_t* base);          // sub_820A57F0
void SaveInterfaceSlotPending(PPCContext& ctx, uint8_t* base);        // sub_820A5808
// r3 = the save devices, r4 = the name.
void SaveInterfaceSetRootName(PPCContext& ctx, uint8_t* base);        // sub_820A5580
// r4 = slot, r5 to r10 and the stack's +84 (RequestSave's thumbnail size).
void SaveInterfaceSlotRequestSave(PPCContext& ctx, uint8_t* base);    // sub_820A52E0
void SaveInterfaceSlotRequestLoad(PPCContext& ctx, uint8_t* base);    // sub_820A53E8

// sub_82295D48: r3 = destination, r4 = source (r3 left).
void SaveInterfaceCopyDisplayName(PPCContext& ctx, uint8_t* base);
// sub_82295C18: r3 = bytes, r4 = a count.
void SaveInterfaceContentSize(PPCContext& ctx, uint8_t* base);

// The interface's wrappers.
void SaveInterfaceInit(PPCContext& ctx, uint8_t* base);           // sub_8209FFB8
void SaveInterfaceSlotStateOf(PPCContext& ctx, uint8_t* base);    // sub_820A0018
void SaveInterfaceRequestDevice(PPCContext& ctx, uint8_t* base);  // sub_820A0208
void SaveInterfaceSaveCount(PPCContext& ctx, uint8_t* base);      // sub_820A02C8
void SaveInterfaceFreeBytes(PPCContext& ctx, uint8_t* base);      // sub_820A0320
void SaveInterfacePending(PPCContext& ctx, uint8_t* base);        // sub_820A0378
void SaveInterfaceDevice(PPCContext& ctx, uint8_t* base);         // sub_820A03D0
void SaveInterfaceSaveAt(PPCContext& ctx, uint8_t* base);         // sub_820A0428
void SaveInterfaceSave(PPCContext& ctx, uint8_t* base);           // sub_820A0490
void SaveInterfaceLoad(PPCContext& ctx, uint8_t* base);           // sub_820A0510
void SaveInterfaceUserCount(PPCContext& ctx, uint8_t* base);      // sub_820A05D8
void SaveInterfaceUserAt(PPCContext& ctx, uint8_t* base);         // sub_820A0620

// The front end's save layer (save_interface/save_layer.h).
// sub_8228F780 SaveSlots::OfController: r3 = a controller.
void SaveInterfaceSlotOfController(PPCContext& ctx, uint8_t* base);
// The slot wrappers by player: r3 = a player.
void SaveInterfacePlayerRequestDevice(PPCContext& ctx, uint8_t* base);  // sub_8228F9B0
void SaveInterfacePlayerSlotState(PPCContext& ctx, uint8_t* base);      // sub_8228F9E8
void SaveInterfacePlayerDevice(PPCContext& ctx, uint8_t* base);         // sub_8228FA90
void SaveInterfacePlayerPending(PPCContext& ctx, uint8_t* base);        // sub_8228FAC8
// The save buffers (no arguments); the original without the native heap.
void SaveInterfaceBuffersCreate(PPCContext& ctx, uint8_t* base);          // sub_8228F6B0
void SaveInterfaceBuffersFreeReplayList(PPCContext& ctx, uint8_t* base);  // sub_8228F7E8
void SaveInterfaceBuffersDestroy(PPCContext& ctx, uint8_t* base);         // sub_8228F8F8
// sub_8228F840 Replays::Sort: r3 = the records (the copy in the frame the
// original makes, r1 - 176 + 80).
void SaveInterfaceReplaysSort(PPCContext& ctx, uint8_t* base);
// sub_821109B8: r3 = out, r4 = the stats (r3 left).
void SaveInterfacePlayTimeAdd(PPCContext& ctx, uint8_t* base);
// sub_82112838 PlayTime::Update: r3 = the save data (out in the frame the
// original makes, r1 - 96 + 80).
void SaveInterfacePlayTimeUpdate(PPCContext& ctx, uint8_t* base);

// sub_8228FB00 GameData::Save (no arguments; its frame r1 - 368, the guest
// calls GetLocalTime and, where its plain C++ refuses, swprintf from it).
void SaveInterfaceGameDataSave(PPCContext& ctx, uint8_t* base);
// sub_8228FBC0 GameData::Load and sub_8228FC50 GameData::Check: r3 = a
// player.
void SaveInterfaceGameDataLoad(PPCContext& ctx, uint8_t* base);
void SaveInterfaceGameDataCheck(PPCContext& ctx, uint8_t* base);

// sub_822902A0 Replays::Count: r3 = the count (out).
void SaveInterfaceReplaysCount(PPCContext& ctx, uint8_t* base);
// sub_822900E0 Replays::ReadEntry: r3 = the save's index, r4 = the record,
// r5 = the list entry; the original for a "rep" save.
void SaveInterfaceReplaysReadEntry(PPCContext& ctx, uint8_t* base);
// sub_82290378 Replays::BuildList: r3 = add an empty first entry; its frame
// r1 - 144 (ReadEntry's original a guest call from it).
void SaveInterfaceReplaysBuildList(PPCContext& ctx, uint8_t* base);

// The save devices' construction and teardown (save_interface/save_devices.h),
// each writing its prologue's saves as the original does; the original
// without the native heap (or where the plain C++ refuses).
// sub_820A63C8 SaveDevices::Create: r3 = the block, r4 = the saves:
// operator new(116) and the constructor from its 112-byte frame (r3 the
// save devices or 0).
void SaveDevicesCreateSteps(PPCContext& ctx, uint8_t* base);
// sub_820A6160 SaveDevices::SaveDevices (no adapter: only Create's step
// calls it): r3 = it, r4 = the block, r5 = the
// saves to enumerate; the run loop's slots from its 176-byte frame.
void SaveDevicesConstructSteps(PPCContext& ctx, uint8_t* base);
// sub_820A4560 SaveSlot::AllocBuffers: r3 = a slot, r4 = the saves (no
// adapter: only the constructor's plain C++ calls it).
void SaveSlotAllocBuffersSteps(PPCContext& ctx, uint8_t* base);
// sub_820A6110 SaveDevices::~SaveDevices: r3 = it, r4 = the flags.
void SaveDevicesDeleteSteps(PPCContext& ctx, uint8_t* base);
// sub_820A56E8 SaveDevices::Destroy: r3 = it (no adapter: only the
// destructor's plain C++ calls it).
void SaveDevicesDestroySteps(PPCContext& ctx, uint8_t* base);
// sub_820A55F0 SlotArray::Reset: r3 = the word holding the slots (no
// adapter: only Destroy's plain C++ calls it).
void SlotArrayResetSteps(PPCContext& ctx, uint8_t* base);

// The save devices' slot handlers (save_interface/slot_handlers.h), each
// writing its prologue's saves as the original does: r3 = the save devices,
// r4 = a slot; r3 the slot's next state.
void SlotWaitForUserSteps(PPCContext& ctx, uint8_t* base);    // sub_820A4870
void SlotWaitForDeviceSteps(PPCContext& ctx, uint8_t* base);  // sub_820A5878
void SlotFailedSteps(PPCContext& ctx, uint8_t* base);         // sub_820A4890
void SlotReadySteps(PPCContext& ctx, uint8_t* base);          // sub_820A58D8
void SlotEnumerateStepSteps(PPCContext& ctx, uint8_t* base);  // sub_820A59C0
// sub_820A4800 SaveDevices::OnEvent: r3 = the save devices, r4 = the run
// loop, r5 = the event; the run loop's slots 4 and 3 guest calls from its
// frame.
void DevicesOnEventSteps(PPCContext& ctx, uint8_t* base);
// sub_820908D8, the framework's +56 interface's slot 1: r3 = the interface.
void FrameworkDeviceListSteps(PPCContext& ctx, uint8_t* base);

// sub_82295C98 XGetOverlappedResult: r3 = the overlapped, r4 = the result
// (out), r5 = wait; sub_82295F20 GetOverlappedResult: r3 = a handle, r4 = the
// overlapped, r5 = the bytes (out), r6 = wait. Their waits
// (WaitForSingleObject sub_822970A0) guest calls from their frames.
void XGetOverlappedResultSteps(PPCContext& ctx, uint8_t* base);
void GetOverlappedResultSteps(PPCContext& ctx, uint8_t* base);
// The kernel's guest calls (WaitForSingleObject, CloseHandle) for the plain
// C++, through `ctx`.
save_interface::KernelCalls KernelCallsOf(PPCContext& ctx, uint8_t* base);

// The waits the machines poll (r4 = slot; no adapters: only the machines'
// plain C++ calls them; for the harness).
void SlotWaitCloseItemSteps(PPCContext& ctx, uint8_t* base);  // sub_820A4C08
void SlotWaitDeleteSteps(PPCContext& ctx, uint8_t* base);     // sub_820A5290
void SlotWaitThumbnailSteps(PPCContext& ctx, uint8_t* base);  // sub_820A5190
void SlotWaitCloseSteps(PPCContext& ctx, uint8_t* base);      // sub_820A5070
void SlotWaitWriteSteps(PPCContext& ctx, uint8_t* base);      // sub_820A4E68
void SlotWaitReadSteps(PPCContext& ctx, uint8_t* base);       // sub_820A4FF8
// The machines: r3 = the save devices, r4 = slot; r3 the next state.
void SlotCheckStepSteps(PPCContext& ctx, uint8_t* base);   // sub_820A5A88
void SlotSaveStepSteps(PPCContext& ctx, uint8_t* base);    // sub_820A5BE8
void SlotLoadStepSteps(PPCContext& ctx, uint8_t* base);    // sub_820A5E68
void SlotDeleteStepSteps(PPCContext& ctx, uint8_t* base);  // sub_820A6010

// sub_820A4610 SaveDevices::Update: r3 = the save devices (its adapter is
// B5's, src/hooks/notify.cpp, which runs this with --native_save_interface).
void SaveDevicesUpdateSteps(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
