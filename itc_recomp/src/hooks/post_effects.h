// The post effects' adapter paths that native callers share
// (src/hooks/post_effects.cpp, docs/functions/post-effects.md).

#pragma once

#include "guest.h"

#include "post_effects/post_effects.h"

namespace hooks {

// RenderTextures::Release (sub_821332F0; r3 = the address of an effect's
// entry pointer): its plain C++, or the original where the frees aren't
// the allocation layer's (--native_c_runtime_heap, --native_heap, or a
// block not the layer's). r3 as the original leaves it.
void PostEffectsReleaseTexture(PPCContext& ctx, uint8_t* base);
// PostEffect::ReleaseSurface (sub_82133C60; r3 = the address of an
// effect's surface pointer): its plain C++ with the video output and
// Resource::Release as guest calls from a 112-byte frame, or the original
// without --native_video_output. r3 as the original leaves it.
void PostEffectsReleaseSurface(PPCContext& ctx, uint8_t* base);
// The destructors' calls: the two above on a field's address.
post_effects::DestroyCalls PostEffectsDestroyCalls(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
