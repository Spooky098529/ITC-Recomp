// The archive's adapters (hooks/archive.cpp) that make guest calls, for the
// harness's native steps.

#pragma once

#include "guest.h"

#include "archive/archive.h"
#include "archive/semaphores.h"

namespace hooks {

// Whether the archive's four semaphores are host ones for the game
// (--native_archive at start-up, latched at the first call: the loader thread
// and the main thread share them, so this never changes while running).
bool ArchiveHostSemaphores();
// The host semaphore the code on this thread takes for a guest Semaphore's
// address: the harness's test one, else the live one when they're host,
// else null (the game's own Semaphore).
archive::HostSemaphore* ArchiveSemaphore(uint32_t address);
// Whether the queue's functions run natively: --native_archive, with the
// semaphores host ones.
bool NativeArchiveQueue();

// The request set-up (sub_8209D490) and the loader's next request
// (sub_8209DD98) without their flag: archive::SetUp and NextRequest with
// their calls (the callbacks, VirtualAlloc and VirtualFree where the native
// layer can't) through `ctx`.
void ArchiveSetUp(PPCContext& ctx, uint8_t* base);
void ArchiveNextRequest(PPCContext& ctx, uint8_t* base);

// The request finish (sub_8209D688) without its flag: archive::FinishRequest
// with the callback and the fallback VirtualFree called through `ctx`.
void ArchiveFinishRequest(PPCContext& ctx, uint8_t* base);
// LoaderFunctor::Invoke (sub_8209DD80) without its flag: the functor's +4
// called with r4 as r3.
void ArchiveInvokeFunctor(PPCContext& ctx, uint8_t* base);
// Ucl::Unpack (sub_82294610) without its flag: archive::UclUnpack with
// memcpy's fallback called through `ctx`.
void ArchiveUclUnpack(PPCContext& ctx, uint8_t* base);

// Archive::Load for native callers (needs NativeArchiveQueue()): the request
// queued through the archive's plain C++ (Queue(BuildLoad(...)), r7 = r8 =
// 0), or handed to the harnesses' recorder (hooks::g_start_up_recorder) when
// one is set, as Archive::Load's adapter does.
archive::LoadCall NativeArchiveLoad(uint8_t* base);
// r3 as Archive::Load's adapter leaves a serial: lwz's (zero-extended), or -1
// (li's).
void SetArchiveSerial(PPCContext& ctx, int64_t serial);

}  // namespace hooks
