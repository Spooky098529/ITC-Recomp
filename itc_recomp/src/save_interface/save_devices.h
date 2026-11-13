// The save devices' construction and teardown natively
// (docs/functions/save-interface.md): SaveDevices::SaveDevices with
// SaveSlot::AllocBuffers, and the destructor with SaveDevices::Destroy and
// SlotArray::Reset (never seen in play). Plain C++ over the guest layout,
// store for store as the recompiled functions; register steps in
// src/hooks/save_interface.cpp (behind --native_save_interface), shared
// with the harness.
//
// The storage devices' notification listener (+56) is stored as
// user::kNoListener without XNotifyCreateListener, as SignIn::SetUp's and
// Network::StartUp's plain C++ do (the user's call, 2026-10-30; B5's
// listener wrapper makes none), and the destructor's CloseHandle on it is
// left out (no listener is made: it isn't a handle).

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace save_interface {

constexpr uint32_t kDevicesVtable = 0x82002198;
constexpr uint32_t kDevicesBaseVtable = 0x82002148;  // probably the interface's base
constexpr uint32_t kSaveSlotVtable = 0x82002190;
constexpr uint32_t kSlotCount = 4;
constexpr uint32_t kSlotBytes = 1000;
constexpr uint32_t kSaveRecordBytes = 308;  // an XCONTENT_DATA

// Whether SaveSlot::AllocBuffers can run natively for `saves`: a process
// heap, and both sizes within what malloc takes (else the original's new
// handler path, never in play: 100 in play).
bool CanAllocBuffers(uint8_t* base, uint32_t saves);
// sub_820A4560 SaveSlot::AllocBuffers: +80 = operator new[] of saves x 308
// bytes, +84 = operator new[] of saves x 4. Only after CanAllocBuffers.
void SlotAllocBuffers(uint8_t* base, uint32_t slot, uint32_t saves);

// sub_820A6160 SaveDevices::SaveDevices: r3 `devices` (116 bytes), r4
// `block` (+0 the run loop, +4 the interface, +8 and +12 the keys), r5
// `saves`. `frame` is the original's 176-byte frame (the two member
// delegates built at +80 and +96); `add` calls the run loop's slot at
// `offset` (8 AddTickHandler, 4 AddListener) with r4 = key, r5 = the
// delegate (guest calls). The words SaveSlot::AllocBuffers' prologue leaves
// below the frame are written as it leaves them. Returns `devices`;
// nullopt, with nothing done, where CanAllocBuffers refuses.
std::optional<uint32_t> SaveDevicesConstruct(
    uint8_t* base, uint32_t devices, uint32_t block, uint32_t saves, uint32_t frame,
    const std::function<void(uint32_t loop, uint32_t offset, uint32_t key, uint32_t delegate)>&
        add);

// sub_820A55F0 SlotArray::Reset: `holder` the word holding a slot array.
// When not 0: with the count header (array - 8) 0, operator delete[] on the
// header; else `destroy` (the first slot's vtable slot 0 with r4 = 3, a
// guest call answering r3); the word then 0. Returns r3 as the original
// leaves it (holder, the header, or destroy's answer).
uint32_t SlotArrayReset(uint8_t* base, uint32_t holder,
                        const std::function<uint32_t(uint32_t slot)>& destroy);
// sub_820A56E8 SaveDevices::Destroy: the vtable, the listener's CloseHandle
// left out, the slots reset (SlotArrayReset on +44 when it isn't 0), the
// base vtable. Returns r3 as the original leaves it after the CloseHandle
// (holder or Reset's answer).
uint32_t SaveDevicesDestroy(uint8_t* base, uint32_t devices,
                            const std::function<uint32_t(uint32_t slot)>& destroy);
// sub_820A6110 SaveDevices::~SaveDevices (deleting): Destroy, then with bit
// 0 of `flags` operator delete. Returns `devices`.
uint32_t SaveDevicesDelete(uint8_t* base, uint32_t devices, uint32_t flags,
                           const std::function<uint32_t(uint32_t slot)>& destroy);

}  // namespace save_interface
