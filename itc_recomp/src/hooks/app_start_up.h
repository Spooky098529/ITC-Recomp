// The app's start-up's adapters, for other code and the harness
// (src/hooks/app_start_up.cpp).

#pragma once

#include "guest.h"

namespace hooks {

// For the app_start_up harness: while set, App::StartUp's callees' adapters
// (SceneTarget::Create, Archive::Init, ActionMappers::Init,
// CameraPasses::Init, ScreenFades::Init, Billboards::Init,
// SoundGlobals::Init, SaveInterface::Init, Network::SetUp, Text::Init,
// Players::ResetControllers, Archive::WaitIdle; and Archive::Load, for the
// data load's harness; Career::EventQuery's and Race::SetUp's register
// steps, for the location screen's) hand their call to it first;
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
// Movie::Movie (sub_820AEA88: r3 the movie, r4 the block) and
// MovieWrapper::MovieWrapper (sub_82593DC0: r3 the wrapper), each with the
// original's prologue saves; need --native_heap (the adapters check).
void MovieConstructSteps(PPCContext& ctx, uint8_t* base);
void MovieWrapperSteps(PPCContext& ctx, uint8_t* base);
// AudioManager::Create (sub_82119B38: r3 the owner, r4 the name, r5 the
// count's address) in frames like the original's, with its constructor and
// StartXAudio; needs --native_heap (the adapter checks).
void AudioManagerCreateSteps(PPCContext& ctx, uint8_t* base);
// AudioEngine::StartXAudio (sub_8259CD50: r3 the engine, not read) alone,
// for the harness (no adapter: only AudioManager's plain C++ calls it).
void StartXAudioSteps(PPCContext& ctx, uint8_t* base);
// The second camera class (sub_820A8B88: r3 the object, r4 the block) in a
// frame like the original's, and for the harness its camera's and list's
// constructors alone (sub_820A9E28, sub_820A98C0: r3 the block; no
// adapters); need --native_heap (the adapter checks).
void AppCameraSteps(PPCContext& ctx, uint8_t* base);
void AppCameraCameraConstructSteps(PPCContext& ctx, uint8_t* base);
void AppCameraListConstructSteps(PPCContext& ctx, uint8_t* base);
// App::App (sub_82094590: r3 the app, r4 the block) in a frame like the
// original's, its saved registers set as it sets them; needs --native_heap
// (the adapter checks).
void AppConstructSteps(PPCContext& ctx, uint8_t* base);
// App::Create (sub_82094AB8: r3 the block) in a frame like the original's,
// App::App through its adapter; needs --native_heap (the adapter checks).
void AppCreateSteps(PPCContext& ctx, uint8_t* base);
void TextInitSteps(PPCContext& ctx, uint8_t* base);
void FontConstructSteps(PPCContext& ctx, uint8_t* base);
// Whether Text::Init's and Font::Font's native steps can run.
bool TextInitNative();

}  // namespace hooks
