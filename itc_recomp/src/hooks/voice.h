// The voice engine left out (src/hooks/voice.cpp): the network manager's
// voice set-up's plain C++, for its adapter and the native network set-up.

#pragma once

#include <cstdint>

namespace voice {

// sub_8214F6E8 Network::SetUpVoice without the engine (the user's call,
// 2026-10-30): `machine_id` (the id's address) to the manager's +456, the
// tick count to +444; returns the tick count.
uint32_t SetUpWithoutEngine(uint8_t* base, uint32_t manager, uint32_t machine_id);

}  // namespace voice
