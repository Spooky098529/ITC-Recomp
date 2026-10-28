// The save system's interface natively (docs/functions/save-interface.md):
// the app's interface at [0x826BDC90] (app + 16) and its wrappers over the
// save devices' slots and the users, the save devices' small vtable slots
// (the slot getters and the request setters) and the users' two slots.
// Plain C++ over the guest layout, load for load and store for store as the
// recompiled functions; adapters in src/hooks/save_interface.cpp (behind
// --native_save_interface), register steps shared with the harness in
// src/hooks/save_interface.h.
//
// The wrappers reach their objects through vtables. Each checks every
// target it would call first (reads only) and answers nullopt, with nothing
// done, where one isn't the function this file has (the adapter then runs
// the original from the start); in play every target is.

#pragma once

#include <cstdint>
#include <optional>

namespace save_interface {

// The interface's global (SaveInterface::Init stores it).
constexpr uint32_t kInterfaceGlobal = 0x826BDC90;
// The content's root name SaveInterface::Init gives the save devices.
constexpr uint32_t kRootName = 0x82001ED4;  // "TXR360"

// The vtable targets the wrappers go through.
constexpr uint32_t kAppDevices = 0x820943B0;     // the interface's +12
constexpr uint32_t kAppUsers = 0x820943C8;       // the interface's +8
constexpr uint32_t kFrameworkDevices = 0x82090788;  // the +36 interface's +40
constexpr uint32_t kFrameworkUsers = 0x82493700;    // the +36 interface's +36
constexpr uint32_t kUsersCount = 0x82490B08;     // the users' +4
constexpr uint32_t kUsersSignedIn = 0x820996D8;  // the users' +8
constexpr uint32_t kSlotRequestDevice = 0x820A5750;  // the save devices' +4
constexpr uint32_t kSlotDevice = 0x820A5768;         // +8
constexpr uint32_t kSlotFreeBytes = 0x820A5780;      // +12
constexpr uint32_t kSlotSaveCount = 0x820A5798;      // +16
constexpr uint32_t kSlotSaveAt = 0x820A57B0;         // +20
constexpr uint32_t kSlotState = 0x820A57F0;          // +28
constexpr uint32_t kSlotPending = 0x820A5808;        // +32
constexpr uint32_t kSetRootName = 0x820A5580;        // +56
constexpr uint32_t kSlotRequestSave = 0x820A52E0;    // +60
constexpr uint32_t kSlotRequestLoad = 0x820A53E8;    // +64

// The target of `object`'s vtable slot at byte `offset`.
uint32_t VirtualTarget(uint8_t* base, uint32_t object, uint32_t offset);

// sub_820943B0, the interface's +12: the save devices (the +36 interface at
// interface + 8, its +40: sub_82090788, the word at +88). nullopt where
// that target isn't sub_82090788.
std::optional<uint32_t> AppDevices(uint8_t* base, uint32_t interface);
// sub_820943C8, the interface's +8: the users (the +36 interface's +36:
// sub_82493700, the word at +92). nullopt where that target isn't
// sub_82493700.
std::optional<uint32_t> AppUsers(uint8_t* base, uint32_t interface);
// sub_82493700: the word at +92.
uint32_t FrameworkUsers(uint8_t* base, uint32_t object);

// sub_82490B08: the word at +216 (the users' signed-in count).
uint32_t UsersCount(uint8_t* base, uint32_t users);
// sub_820996D8 Users::SignedIn: the index-th signed-in user's number, or -1
// (sign-extended, as the original's li).
uint64_t UsersSignedIn(uint8_t* base, uint32_t users, uint32_t index);

// The save devices' slot functions: `devices` the save devices, `slot` a
// slot's index (its 1,000 bytes at [devices + 44] + 1,000 x slot).
uint32_t SlotAddress(uint8_t* base, uint32_t devices, uint32_t slot);
void SlotRequestDevice(uint8_t* base, uint32_t devices, uint32_t slot);  // sub_820A5750
uint32_t SlotDevice(uint8_t* base, uint32_t devices, uint32_t slot);     // sub_820A5768
uint64_t SlotFreeBytes(uint8_t* base, uint32_t devices, uint32_t slot);  // sub_820A5780
uint32_t SlotSaveCount(uint8_t* base, uint32_t devices, uint32_t slot);  // sub_820A5798
// sub_820A57B0: the enumeration buffer (+80) + 308 x index, in 64 bits as
// the original adds them.
uint64_t SlotSaveAt(uint8_t* base, uint32_t devices, uint32_t slot, uint64_t index);
uint32_t SlotState(uint8_t* base, uint32_t devices, uint32_t slot);      // sub_820A57F0
// sub_820A5808: the request words summed in 64 bits.
uint64_t SlotPending(uint8_t* base, uint32_t devices, uint32_t slot);
// sub_820A5580 SaveDevices::SetRootName: `name` copied with its terminator
// to +16, byte by byte.
void SetRootName(uint8_t* base, uint32_t devices, uint32_t name);

// sub_820A52E0 SaveSlot::RequestSave and sub_820A53E8 SaveSlot::RequestLoad
// (no thumbnail: 0 for both): 0 unless the slot's state (the devices' +28,
// virtual) is 3; else the names copied in (wcscpy's plain C++ for the
// display name, sub_82295D48), the data, its size, the thumbnail and its
// size stored, the request word (+968 save, +972 load) 1, and 1. nullopt
// where the +28 target isn't sub_820A57F0.
struct Request {
  uint32_t display_name, file_name, path, data, size, thumbnail, thumbnail_size;
};
std::optional<uint64_t> SlotRequestSave(uint8_t* base, uint32_t devices, uint32_t slot,
                                        const Request& request);
std::optional<uint64_t> SlotRequestLoad(uint8_t* base, uint32_t devices, uint32_t slot,
                                        const Request& request);

// sub_82295D48: wcscpy (sub_825A7F30)'s plain C++ (the display name's copy).
void CopyDisplayName(uint8_t* base, uint32_t destination, uint32_t source);
// sub_82295C18, probably XContentCalculateSize: `bytes` and `count` as a
// size in 4 KB blocks, with the table blocks for every 170, plus 10;
// returns the bytes (64-bit, as the original's shift leaves it).
uint64_t ContentSize(uint32_t bytes, uint32_t count);

// The interface's wrappers (sub_8209FFB8 to sub_820A0620), each through
// [0x826BDC90]; nullopt where a target isn't this file's.
// sub_8209FFB8 SaveInterface::Init: the global = `interface`, the save
// devices given the root name. Returns r3 as the original leaves it (the
// save devices); nullopt where a target isn't ours (nothing done).
std::optional<uint32_t> Init(uint8_t* base, uint32_t interface);
std::optional<uint32_t> InterfaceSlotState(uint8_t* base, uint32_t slot);       // sub_820A0018
// sub_820A0208: returns r3 as the original leaves it (the save devices).
std::optional<uint32_t> InterfaceRequestDevice(uint8_t* base, uint32_t slot);
std::optional<uint32_t> InterfaceSaveCount(uint8_t* base, uint32_t slot);       // sub_820A02C8
std::optional<uint32_t> InterfaceFreeBytes(uint8_t* base, uint32_t slot);       // sub_820A0320
std::optional<uint64_t> InterfacePending(uint8_t* base, uint32_t slot);         // sub_820A0378
std::optional<uint32_t> InterfaceDevice(uint8_t* base, uint32_t slot);          // sub_820A03D0
std::optional<uint64_t> InterfaceSaveAt(uint8_t* base, uint32_t slot, uint64_t index);  // sub_820A0428
std::optional<uint64_t> InterfaceSave(uint8_t* base, uint32_t slot, const Request& request);  // sub_820A0490
std::optional<uint64_t> InterfaceLoad(uint8_t* base, uint32_t slot, const Request& request);  // sub_820A0510
// Whether SaveInterface::Save (`load` false) or Load would run natively:
// the interface's targets and the save devices' request setter and +28 are
// this file's (reads only, for callers that write before they call it).
bool InterfaceRequestReady(uint8_t* base, bool load);
std::optional<uint32_t> InterfaceUserCount(uint8_t* base);                      // sub_820A05D8
std::optional<uint64_t> InterfaceUserAt(uint8_t* base, uint32_t index);         // sub_820A0620

}  // namespace save_interface
