// The save devices' slot handlers natively (docs/functions/save-interface.md,
// "The slot handlers"): the ten functions at the save devices' +76 to +112
// that SaveDevices::Update runs for each slot by its state (+988), each
// returning the slot's next state, with the waits they poll and the
// listener delegate's target. Plain C++ over the guest layout, store for
// store as the recompiled functions; register steps in
// src/hooks/save_interface.cpp (behind --native_save_interface), shared with
// the harness (save_handlers). The B4 steps they start are
// src/saves/slot_steps.h's plain C++.
//
// The stack: each function writes its prologue's saves (the return address,
// the nonvolatile registers it saves, its back chain) above and at its
// stack pointer as the original leaves them, from the CallSite its caller
// gives: the caller's stack pointer, the call's return address and the
// caller's r27 to r31 at the call. Guest calls get the CallSite the
// original's call has, so the recompiled callee saves the same registers.

#pragma once

#include <cstdint>
#include <functional>

namespace save_interface {

// A call as the original makes it: the stack pointer at the call, its
// return address, and the nonvolatile registers r27 to r31 there.
struct CallSite {
  uint32_t sp = 0;
  uint32_t lr = 0;
  uint64_t r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
};

// A prologue's saves above `at.sp`: the return address at sp - 8, r`first`
// to r31 (first 32: none) from sp - 8 x (33 - first), and the back chain of
// its `frame_bytes`-byte frame. Returns the frame's stack pointer.
uint32_t WritePrologue(uint8_t* base, const CallSite& at, uint32_t frame_bytes, int first);

// The slot handler addresses (the save devices' +76 + 4 x state).
constexpr uint32_t kSlotWaitForUser = 0x820A4870;      // 0
constexpr uint32_t kSlotWaitForDevice = 0x820A5878;    // 1
constexpr uint32_t kSlotFailed = 0x820A4890;           // 2
constexpr uint32_t kSlotReady = 0x820A58D8;            // 3
constexpr uint32_t kSlotWaitDeviceSelector = 0x820A48B8;  // 4 (B4's)
constexpr uint32_t kSlotEnumerateStep = 0x820A59C0;    // 5

// sub_820A4870 SaveSlot::WaitForUser (state 0): 3 when the slot's user
// (+956) is 0 to 3, else 0. Leaf.
uint32_t SlotWaitForUser(uint8_t* base, uint32_t slot);
// sub_820A4890 SaveSlot::Failed (state 2): 2 until the acknowledgment
// (+980) is set; then +980 = 0, +992 = 0 and 3. Leaf.
uint32_t SlotFailed(uint8_t* base, uint32_t slot);
// sub_820A5878 SaveSlot::WaitForDevice (state 1): with the device request
// (+960) the device selector (4 when it answers 1, else 1); without, 3 when
// the slot has a device (+36), else 1. A 96-byte frame (r31 saved).
uint32_t SlotWaitForDevice(uint8_t* base, uint32_t slot, const CallSite& at);
// sub_820A58D8 SaveSlot::Ready (state 3): the first request set (+960 the
// device selector, +964 the enumeration, +968 the save, +972 the load, +976
// the delete) started; 1 without a device. A 96-byte frame (r31 saved).
uint32_t SlotReady(uint8_t* base, uint32_t slot, const CallSite& at);
// sub_820A59C0 SaveSlot::EnumerateStep (state 5): by the sub-step at +96, 1
// the enumeration, 2 its wait. A 112-byte frame (r30, r31 saved).
uint32_t SlotEnumerateStep(uint8_t* base, uint32_t devices, uint32_t slot, const CallSite& at);

// sub_820A4800 SaveDevices::OnEvent (the listener delegate's target): for an
// event whose word (+0) is 1, the run loop's slot 4 with r4 = devices + 4
// (removes the tick handler) and its slot 3 with r4 = devices + 8 (removes
// the listener): `call(offset, key, at)` (guest calls through the vtable,
// r3 = the run loop). A 112-byte frame (r30, r31 saved).
void DevicesOnEvent(uint8_t* base, uint32_t devices, uint32_t loop, uint32_t event,
                    const CallSite& at,
                    const std::function<void(uint32_t offset, uint32_t key, const CallSite& at)>&
                        call);

// sub_820908D8, the framework's +56 interface's slot 1 (the device list
// SaveDevices::Update reads the users from): sub_82493700's plain C++ on
// `object` - 20 (the word at object + 72).
uint32_t FrameworkDeviceList(uint8_t* base, uint32_t object);

// The kernel's calls the overlapped results and the close's wait make
// through the guest (never in play: every overlapped here is done at once
// and B4's steps make no handles). `pcr` is r13, for the thread's last
// error (last_error.h).
struct KernelCalls {
  uint32_t pcr = 0;
  // WaitForSingleObject (sub_822970A0, kept recompiled: the user's call,
  // 2026-10-24) with r3 = `handle`, r4 = -1, at `at`; its r3.
  std::function<uint32_t(uint32_t handle, const CallSite& at)> wait;
  // CloseHandle (sub_82294E20) on `handle` at `at`.
  std::function<void(uint32_t handle, const CallSite& at)> close;
};

// sub_82295C98 XGetOverlappedResult: `overlapped`, `result` (out, from +4;
// 0 none), `wait`: while +0 is 997 (pending), with `wait` and an event
// (+12) WaitForSingleObject on it (258 a timeout: 996; another nonzero
// answer: GetLastError), without, 996. Else +4 into `result` and +0. A
// 112-byte frame (r30, r31 saved).
uint32_t XGetOverlappedResult(uint8_t* base, uint32_t overlapped, uint32_t result, uint32_t wait,
                              const CallSite& at, const KernelCalls& kernel);
// sub_82295F20 GetOverlappedResult: `handle`, `overlapped`, `bytes` (out),
// `wait`: while +0 is 259 (STATUS_PENDING), with `wait` WaitForSingleObject
// on the event (+16) or the handle (258: SetLastError(996) and 0; another
// nonzero answer: 0), without, SetLastError(996) and 0. Else +4 into
// `bytes` and 1, or for a failing status (negative) SetLastErrorFromStatus
// and 0. A 112-byte frame (r30, r31 saved).
uint32_t GetOverlappedResult(uint8_t* base, uint32_t handle, uint32_t overlapped, uint32_t bytes,
                             uint32_t wait, const CallSite& at, const KernelCalls& kernel);

}  // namespace save_interface
