// The game's XAudio render driver object as plain C++ (audio/render_driver.h).

#include "audio/render_driver.h"

#include "guest.h"

#include "base/log.h"
#include "hooks/lifted_call.h"

namespace audio {

uint64_t RenderDriverDestroy(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t driver) {
  ITC_NATIVE_RUNNING("sub_824658C8 AudioRenderDriver::Destroy");
  constexpr uint32_t kVtable = 0x8204AAEC, kVtable4 = 0x8204AAD0, kBaseVtable4 = 0x8204A6B8;
  const uint32_t frame = lifted_call::PushFrame(base, sp, 96);
  const uint32_t handle = guest::LoadU32(base, driver + 16);
  guest::StoreU32(base, driver, kVtable);
  guest::StoreU32(base, driver + 4, kVtable4);
  if (handle != 0) {
    const lifted::Result result = lifted_call::Import(
        base, __imp__XAudioUnregisterRenderDriverClient, frame, r13, {handle});
    if (int32_t(result.r3) >= 0) {
      guest::StoreU32(base, driver + 16, 0);
    }
  }
  guest::StoreU32(base, driver + 4, kBaseVtable4);
  return driver;
}

}  // namespace audio
