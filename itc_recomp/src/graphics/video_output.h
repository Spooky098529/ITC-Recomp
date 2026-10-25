// The game's video output's device lock as plain C++
// (docs/functions/video-output.md): VideoOutput::VideoOutput,
// VideoOutput::LockDevice and UnlockDevice, the device's thread word
// (Device::AcquireThreadOwnership and ReleaseThreadOwnership) and the
// current thread's id. The video output is guest memory (renderer + 64);
// its device lock, the game's semaphore of count 1 at +224 (an event and a
// critical section on kernel imports), is a host std::binary_semaphore in
// its place (the user's call, 2026-10-24: a semaphore may be given back by
// another thread than the one that took it, a mutex may not), one a video
// output, made by its constructor. The guest semaphore's 40 bytes at +224
// are no longer written. The game's Semaphore class stays recompiled for
// its other users (the archive loader's four). Adapters:
// src/hooks/video_output.cpp.

#pragma once

#include <cstdint>

namespace graphics::video_output {

constexpr uint32_t kVtable = 0x8206CAF8;
constexpr uint32_t kDevice = 140;          // the device
constexpr uint32_t kLock = 224;            // the guest semaphore's place (40 bytes)
constexpr uint32_t kLockBytes = 40;
constexpr uint32_t kObjectBytes = 308;     // through +304
constexpr uint32_t kVblankCount = 0x8296BD30;
constexpr uint32_t kDeviceThread = 10760;  // the device's owning thread

// sub_825B9A60: the current (guest) thread's id, as the game reads it from
// r13 +256's +332.
uint32_t CurrentThreadId();

// sub_8229D9D0 Device::AcquireThreadOwnership: device +10760 = the current
// thread's id. Returns it (the original leaves it in r3).
uint32_t AcquireThreadOwnership(uint8_t* base, uint32_t device);

// sub_8229DA10 Device::ReleaseThreadOwnership: device +10760 = 0.
void ReleaseThreadOwnership(uint8_t* base, uint32_t device);

// sub_8258DB18 VideoOutput::VideoOutput: the fields set and its device lock
// made (count 1; a lock made before at the same address is put back to 1).
// Returns the video output.
uint32_t Construct(uint8_t* base, uint32_t video_output);

// sub_8258D8C8 VideoOutput::LockDevice: waits for the device lock, marks the
// device as the calling thread's and returns the device (+140).
uint32_t LockDevice(uint8_t* base, uint32_t video_output);

// sub_8258D908 VideoOutput::UnlockDevice: the device in the word at `local`
// let go (its thread word cleared), the word = 0, the lock given back.
void UnlockDevice(uint8_t* base, uint32_t video_output, uint32_t local);

}  // namespace graphics::video_output
