// Hand-written replacements for recompiled vehicle functions.
//
// Each function here has the same name as a generated sub_XXXXXXXX. The
// generated definitions are weak, so these take their place at link time.
// See docs/functions.md for what is known about each address.

#include "generated/default/itc_pch.h"

#include <bit>

namespace {

// Reads a big-endian float from guest memory.
float LoadGuestFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(REX_LOAD_U32(address));
}

// Offsets into the vehicle object.
constexpr uint32_t kVehicleSpeedOffset = 48;  // float, metres per second

constexpr float kMetresPerSecondToKmh = 3.6f;

float Vehicle_GetSpeedKmh(uint8_t* base, uint32_t vehicle) {
  float speed = LoadGuestFloat(base, vehicle + kVehicleSpeedOffset);
  return speed * kMetresPerSecondToKmh;
}

}  // namespace

// Guest entry points. These keep the generated names so the game's calls and
// the function table reach them; they only unpack registers and forward.

// Vehicle_GetSpeedKmh: r3 = vehicle, returns f1.
extern "C" REX_FUNC(sub_820C0128) {
  REX_FUNC_PROLOGUE();
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(Vehicle_GetSpeedKmh(base, ctx.r3.u32));
}
