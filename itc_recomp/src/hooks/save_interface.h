// The save system's interface's register steps (src/save_interface/,
// docs/functions/save-interface.md), shared by the adapters
// (src/hooks/save_interface.cpp, behind --native_save_interface) and the
// harness (without the flag). Each takes the original's registers, leaves r3
// as the original does, and runs the original (`__imp__`) where the plain
// C++ answers nullopt (a vtable target that isn't ours).

#pragma once

#include "guest.h"

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

}  // namespace hooks
