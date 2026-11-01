// The cockpit body's and the traffic body's textures bound to their part
// models, as plain C++ (docs/functions/scene-loaders.md): Cockpit::BindTextures
// (sub_820D6108) and TrafficCar::BindTextures (sub_820D7D38). Guest data
// shared with the recompiled code, so these work on guest memory with the
// originals' loads and stores in their order; sprintf's text goes into the
// original's frame (read by nothing after the return), its saved registers
// aren't written. Adapters: src/hooks/scene_loaders.cpp.

#pragma once

#include <cstdint>
#include <optional>

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

}  // namespace scene_loaders
