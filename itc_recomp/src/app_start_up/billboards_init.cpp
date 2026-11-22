// The light billboards' shaders and vertex declaration natively (start_up.h,
// docs/functions/app-start-up.md).

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "graphics/video_output.h"
#include "guest.h"
#include "schema/app_start_up.h"

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

using Globals = schema::AppStartUpGlobals;

constexpr uint32_t kEntry = 0x82001BCC;          // "main"
constexpr uint32_t kVertexProfile = 0x82001BD4;  // "vs_3_0"
constexpr uint32_t kPixelProfile = 0x82001BC4;   // "ps_3_0"

}  // namespace

void BillboardsInit(uint8_t* base, uint32_t frame, const BillboardCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209BE88 Billboards::Init");
  namespace vo = graphics::video_output;
  const uint32_t ring = Globals::billboard_ring_index(base);
  Globals::set_billboard_vertex_count(base, 0);
  Globals::set_billboard_ring_index(base, ring == 0 ? 1 : 0);

  // The two shaders: compiled, made under the device lock, their buffers
  // released.
  struct Shader {
    uint32_t source;
    uint32_t profile;
    uint32_t buffer;  // frame offsets
    uint32_t errors;
    uint32_t lock;
    uint32_t global;  // the shader's word
  };
  const Shader shaders[2] = {
      {Globals::billboard_vertex_source(base).address, kVertexProfile, 96, 104, 108,
       Globals::billboard_vertex_shader_address()},
      {Globals::billboard_pixel_source(base).address, kPixelProfile, 100, 112, 116,
       Globals::billboard_pixel_shader_address()},
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
  const auto declaration =
      CreateVertexDeclaration(base, Globals::billboard_elements_address(), calls.memcpy_call);
  Globals::set_billboard_declaration(base, declaration.value_or(0));
  vo::UnlockDevice(base, calls.video_output(5), frame + 116);
  Globals::set_billboard_float_8266D090(base, Globals::float_zero(base));
}

}  // namespace app_start_up
