// The app's start-up's adapters, for other code and the harness
// (src/hooks/app_start_up.cpp).

#pragma once

#include "guest.h"

namespace hooks {

// For the app_start_up harness: while set, App::StartUp's callees' adapters
// (SceneTarget::Create, Archive::Init, ActionMappers::Init,
// CameraPasses::Init, ScreenFades::Init, Billboards::Init,
// SoundGlobals::Init, SaveInterface::Init, Network::SetUp, Text::Init,
// Players::ResetControllers, Archive::WaitIdle) hand their call to it first;
// when it answers true, the adapter returns (the recorder set r3), so the
// harness sees what App::StartUp hands each without setting anything up
// again.
using StartUpRecorder = bool (*)(PPCContext& ctx, uint32_t address);
extern StartUpRecorder g_start_up_recorder;
inline bool StartUpRecorded(PPCContext& ctx, uint32_t address) {
  return g_start_up_recorder != nullptr && g_start_up_recorder(ctx, address);
}

// The register steps without the flag (the adapters' and the harness's):
// App::StartUp (sub_82106548: r3 the interface), Text::Init (sub_8210ADD0)
// and Font::Font (sub_82109228: r3 the font; no adapter of its own), each
// from frames like the originals'. Text::Init's and Font::Font's need the
// device lock native and --native_heap (DeviceUsersNative).
void AppStartUpSteps(PPCContext& ctx, uint8_t* base);
void TextInitSteps(PPCContext& ctx, uint8_t* base);
void FontConstructSteps(PPCContext& ctx, uint8_t* base);
// Whether Text::Init's and Font::Font's native steps can run.
bool TextInitNative();

}  // namespace hooks
