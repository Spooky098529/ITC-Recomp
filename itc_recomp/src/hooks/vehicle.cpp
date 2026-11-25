// Hand-written replacements for recompiled vehicle functions.
//
// Each function here has the same name as a generated sub_XXXXXXXX. The
// generated definitions are weak, so these take their place at link time.
// See docs/functions/ and docs/known-data.md for what is known about each address.

#include "hooks/vehicle.h"
#include "hooks/native_entry.h"

#include "base/log.h"
#include "debug/state_capture.h"
#include "guest.h"
#include "schema/car_physics.h"

namespace {

constexpr float kMetersPerSecondToKmh = 3.6f;

}  // namespace

float Vehicle_GetSpeedKmh(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0128 Vehicle::GetSpeedKmh");
  float speed = schema::Vehicle(vehicle).speed();  // meters per second
  return speed * kMetersPerSecondToKmh;
}

// Guest entry points. These keep the generated names so the game's calls and
// the function table reach them; they only unpack registers and forward.

// Vehicle_GetSpeedKmh: r3 = vehicle, returns f1.
ITC_NATIVE_ENTRY_ALWAYS(820C0128) {
  
  ctx.fpscr.disableFlushMode();
  debug::StateCapture_OnVehicleSeen(ctx.r3.u32);
  ctx.f1.f64 = double(Vehicle_GetSpeedKmh(ctx.r3.u32));
}
