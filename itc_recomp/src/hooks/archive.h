// The archive's adapters (hooks/archive.cpp) that make guest calls, for the
// harness's native steps.

#pragma once

#include "guest.h"

namespace hooks {

// The request finish (sub_8209D688) without its flag: archive::FinishRequest
// with the callback and the fallback VirtualFree called through `ctx`.
void ArchiveFinishRequest(PPCContext& ctx, uint8_t* base);
// LoaderFunctor::Invoke (sub_8209DD80) without its flag: the functor's +4
// called with r4 as r3.
void ArchiveInvokeFunctor(PPCContext& ctx, uint8_t* base);
// Ucl::Unpack (sub_82294610) without its flag: archive::UclUnpack with
// memcpy's fallback called through `ctx`.
void ArchiveUclUnpack(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
