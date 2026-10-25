// The screen objects' adapter paths the harness shares
// (src/hooks/screen_objects.cpp, docs/functions/screen-objects.md).

#pragma once

#include "guest.h"

namespace hooks {

// ScreenObject::Release (sub_8212F0B8; r3 = the object): its plain C++
// with the video output and Resource::Release as guest calls from a
// 128-byte frame like the original's. Needs --native_video_output (the
// device lock). r3 as the original leaves it.
void ScreenObjectRelease(PPCContext& ctx, uint8_t* base);
// ScreenObject::~ScreenObject (sub_82132FC8; r3 = the object): its plain
// C++ from a 96-byte frame, Release's calls from 128 bytes below it. Needs
// --native_video_output and the allocation layer (--native_heap). r3 as the
// original leaves it.
void ScreenObjectDestroy(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
