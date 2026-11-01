// The cockpit body's and the traffic body's file-loaded callbacks and
// textures bound to their part models, as plain C++
// (docs/functions/scene-loaders.md): Cockpit::OnFileLoaded (sub_820D6528),
// TrafficCar::OnFileLoaded (sub_820D8430), Cockpit::BindTextures (sub_820D6108) and TrafficCar::BindTextures
// (sub_820D7D38). Guest data
// shared with the recompiled code, so these work on guest memory with the
// originals' loads and stores in their order; sprintf's text goes into the
// original's frame (read by nothing after the return), its saved registers
// aren't written. Adapters: src/hooks/scene_loaders.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "car_models/plate.h"
#include "models/model_load.h"

namespace scene_loaders {

constexpr uint32_t kCockpitBindFrameBytes = 1168;
constexpr uint32_t kTrafficBindFrameBytes = 1184;

// Cockpit::BindTextures (sub_820D6108): 0 while a part slot (state +556 +
// 24i), an XPR2's slot (+420 + 24i) or the node file (+196) is loading (2).
// Else the name ([0x826018C8 + 4 x block], block = [[+24] + 32]) printed
// into the frame's +80: with +48 0 "%s%s" and "_inte_on", else "%s%s%d"
// with "_inte_on_0" and +48; for blocks below 18 the index
// XPR::FindByName(+448, name) and +3504 = XPR::GetTexture(+448, index); the
// six part models' (+688 + 464i) +48 = +496 (the provider) and, for blocks
// below 18, +452 = XPR::GetTexture(+448, index). Returns 1. `stack` is the
// guest r1 at entry. nullopt, with nothing done, where sprintf's plain C++
// declines (the original runs).
std::optional<uint32_t> CockpitBindTextures(uint8_t* base, uint32_t cockpit, uint32_t stack);

// TrafficCar::BindTextures (sub_820D7D38): 0 while the file's slot (+3764) is
// loading (2). Else for the six part models (+4256 + 464i): +48 = +4064 (the
// provider), Model::SetVinylTextures(part, [+3728], 0); when +192 is above 0
// and +7056 isn't -1, "%s%s%d" of the name ([0x82601980 + 4 x +32]),
// "_ex_0" (0x820050AC) and +192 into the frame's +80, +7060 =
// XPR::FindByName(+4016, it), and when found the part's +460 =
// XPR::GetTexture(+4016, +7060) and its +436 = +7056. Returns 1. nullopt
// where sprintf's plain C++ declines (the original then runs from the
// start: the words written before are the ones it writes again).
std::optional<uint32_t> TrafficBindTextures(uint8_t* base, uint32_t traffic, uint32_t stack);

// The guest calls the file-loaded callbacks make, each from the callback's
// frame with the original's return address: Model::Load's own
// (`model_load`), and the originals of Model::Load (r5 = 0), XPR::Create,
// Skeleton::Bind (r5 = 0) and Skeleton::UpdateMatrices where their plain C++
// declines; the texture provider's vtable slot +16 (r4 = 0, r5 = the XPR),
// answering the r3 it leaves; MakePlate's Resource::Release (from its frame
// below the callback's).
struct FileLoadedCalls {
  models::LoadCalls model_load;
  std::function<void(uint32_t model, uint32_t file)> model_load_original;
  std::function<void(uint32_t xpr, uint32_t file)> xpr_create;
  std::function<uint64_t(uint32_t provider, uint32_t xpr)> provider_set;
  std::function<void(uint32_t skeleton, uint32_t file)> skeleton_bind;
  std::function<void(uint32_t skeleton)> update_matrices;
  car_models::ReleaseCall plate_release;
};

constexpr uint32_t kCockpitLoadedFrameBytes = 272;

// Whether Cockpit::OnFileLoaded runs natively for this request (nothing
// changed): not for a part's slot whose buffer LoadSlot::ReleaseBuffer
// wouldn't free natively.
bool CockpitFileLoadedNative(uint8_t* base, uint32_t request, uint32_t cockpit);

// Cockpit::OnFileLoaded (sub_820D6528, Archive::Load's callback for
// CarInterior::LoadSetCBody): r3 = the request, r4 = the buffer, r6 = the
// cockpit. The first of the six parts' slots whose request (+540 + 24i) is
// `request`: Model::Load(+688 + 464i, the slot's buffer +552 + 24i, 0),
// LoadSlot::ReleaseBuffer (+536 + 24i), state +556 + 24i = 3. Else the
// first of the two XPR2s' slots (+404 + 24i): XPR::Create(+448 + 24i,
// `buffer`), the provider's (+496 + 20i) slot +16, state +420 + 24i = 3.
// Else the node file's (+180): the skeleton (+208) bound to [+192] and
// posed, +196 = 3, the last node named "acce_null" (0x82004FDC, a plain
// byte compare; names at [+292] + 160n) sets +3472 = [+356] + 64n; with
// +3472 set, +3488 = its row 3 (16 bytes) with x, y, z times 0.1, +3500 =
// 1.0, transformed by the identity turned a quarter about y (built in the
// frame: the identity at +96, copied to +160, the angle at +80). Returns
// the r3 the original leaves: ReleaseBuffer's, the provider's, the node
// count (0 after the search), +3488 (Vec4::Transform's), or `request` when
// none matches. `stack` is the guest r1 at entry. Only where
// CockpitFileLoadedNative and the models' lifetime are native.
uint64_t CockpitFileLoaded(uint8_t* base, uint32_t request, uint32_t buffer, uint32_t cockpit,
                           uint32_t stack, const FileLoadedCalls& calls);

constexpr uint32_t kTrafficLoadedFrameBytes = 1360;

// Whether TrafficCar::OnFileLoaded runs natively (nothing changed): memcpy
// native, and for the file's request (+3748) the slot's (+3744) buffer
// releasable natively.
bool TrafficFileLoadedNative(uint8_t* base, uint32_t request, uint32_t traffic);

// TrafficCar::OnFileLoaded (sub_820D8430, Archive::Load's callback for
// TrafficCar::Load): r3 = the request, r6 = the traffic car. "%s%s" of the
// name ([0x82601980 + 4 x +32]) and the suffix at 0x820050B8 into the
// frame's +240 (sprintf); then only for the file's request (+3748), the
// container [+3760]: entry 0 (DataFile::GetEntry(file, 0, 1), asked twice)
// made the XPR +4016 (XPR::Create) and handed to the provider +4064 (its
// slot +16); the plate's textures (MakePlate into +7040 to +7052, the 48
// bytes at +3664 by value: 16 of them memcpy'd to the frame's +80); +7056 =
// XPR::FindByName(+4016, the name); entry 1 the skeleton +3776 bound and
// posed; the nodes "tire_fl", "tire_fr", "tire_rl", "tire_rr" (names at
// [+3860] + 160n, plain byte compares, every match) copied (Matrix::Copy
// from [+3924] + 64n) to +7136, +7200, +7264, +7328; their row 3 to +7072 +
// 16i, w = 1.0, x, y, z times 0.1, each transformed by the identity turned a
// quarter about y (the frame's +112 copied to +176, the angle at +96), then
// w = 0.0; entries 2 to 7 into the six part models (Model::Load(+4256 +
// 464i, entry, 0)); LoadSlot::ReleaseBuffer (+3744), +3764 = 3. Returns the
// r3 the original leaves (sprintf's count, or ReleaseBuffer's); nullopt,
// with nothing done, where sprintf's plain C++ declines. `stack` is the
// guest r1 at entry. Only where TrafficFileLoadedNative and the models'
// lifetime are native.
std::optional<uint64_t> TrafficFileLoaded(uint8_t* base, uint32_t request, uint32_t traffic,
                                          uint32_t stack, const FileLoadedCalls& calls);

}  // namespace scene_loaders
