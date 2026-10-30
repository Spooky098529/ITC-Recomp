// The light billboards' shaders and vertex declaration natively (start_up.h,
// docs/functions/app-start-up.md).

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "graphics/video_output.h"
#include "guest.h"

namespace app_start_up {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Strlen(uint8_t* base, uint32_t string) {
  uint32_t length = 0;
  while (guest::LoadU8(base, string + length) != 0) {
    length++;
  }
  return length;
}

constexpr uint32_t kRingIndex = 0x826A5098;
constexpr uint32_t kRingCount = 0x826A509C;
constexpr uint32_t kVertexShader = 0x826A50A0;
constexpr uint32_t kPixelShader = 0x826A50A4;
constexpr uint32_t kDeclaration = 0x826A50A8;
constexpr uint32_t kVertexSource = 0x82600A54;  // a pointer to the HLSL
constexpr uint32_t kPixelSource = 0x82600A60;   // a pointer to the HLSL
constexpr uint32_t kElements = 0x82600A64;
constexpr uint32_t kEntry = 0x82001BCC;          // "main"
constexpr uint32_t kVertexProfile = 0x82001BD4;  // "vs_3_0"
constexpr uint32_t kPixelProfile = 0x82001BC4;   // "ps_3_0"
constexpr uint32_t kGlowScale = 0x8266D090;
constexpr uint32_t kZero = 0x82000A34;

}  // namespace

void BillboardsInit(uint8_t* base, uint32_t frame, const BillboardCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209BE88 Billboards::Init");
  namespace vo = graphics::video_output;
  const uint32_t ring = Load(base, kRingIndex);
  Store(base, kRingCount, 0);
  Store(base, kRingIndex, ring == 0 ? 1 : 0);

  // The two shaders: compiled, made under the device lock, their buffers
  // released.
  struct Shader {
    uint32_t source;
    uint32_t profile;
    uint32_t buffer;  // frame offsets
    uint32_t errors;
    uint32_t lock;
    uint32_t global;
  };
  const Shader shaders[2] = {
      {Load(base, kVertexSource), kVertexProfile, 96, 104, 108, kVertexShader},
      {Load(base, kPixelSource), kPixelProfile, 100, 112, 116, kPixelShader},
  };
  for (int i = 0; i < 2; i++) {
    const Shader& s = shaders[i];
    Store(base, frame + 92, 0);
    Store(base, frame + 84, frame + s.errors);
    const uint32_t result = calls.compile(i, s.source, Strlen(base, s.source), kEntry, s.profile,
                                          frame + s.buffer);
    if (int32_t(result) < 0) {
      if (const uint32_t errors = Load(base, frame + s.errors); errors != 0) {
        calls.release(2 * i, errors);
      }
    }
    Store(base, frame + s.lock, vo::LockDevice(base, calls.video_output(2 * i)));
    const uint32_t data = calls.buffer_data(i, Load(base, frame + s.buffer));
    Store(base, s.global, i == 0 ? calls.vertex_shader(data) : calls.pixel_shader(data));
    vo::UnlockDevice(base, calls.video_output(2 * i + 1), frame + s.lock);
    calls.release(2 * i + 1, Load(base, frame + s.buffer));
  }

  Store(base, frame + 116, vo::LockDevice(base, calls.video_output(4)));
  const auto declaration = CreateVertexDeclaration(base, kElements, calls.memcpy_call);
  Store(base, kDeclaration, declaration.value_or(0));
  vo::UnlockDevice(base, calls.video_output(5), frame + 116);
  Store(base, kGlowScale, Load(base, kZero));
}

}  // namespace app_start_up
