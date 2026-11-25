// The text's set-up natively: Text::Init, Font::Init, Font::Font and
// Font::CreateShaders (start_up.h, docs/functions/strings-and-font.md).

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "graphics/video_output.h"
#include "guest.h"
#include "memory/heap.h"
#include "schema/strings_and_font.h"

namespace app_start_up {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
uint32_t Strlen(uint32_t string) {
  uint32_t length = 0;
  while (guest::LoadU8(string + length) != 0) {
    length++;
  }
  return length;
}

constexpr uint32_t kFontPointer = 0x826C5C00;
constexpr uint32_t kFontElements = 0x826032F8;
constexpr uint32_t kFontVertexSource = 0x82603338;  // a pointer to the HLSL
constexpr uint32_t kFontPixelEntries = 0x826C5C04;  // 4 x 12 bytes: source, length, 0
constexpr uint32_t kVertexProfile = 0x82001BD4;     // "vs_3_0"
constexpr uint32_t kPixelProfile = 0x82007C6C;      // "ps_2_0"

// The ten colors at +16528 (three bytes each), as the original stores them.
constexpr uint8_t kColors[30] = {255, 234, 0,   44,  242, 242, 82,  240, 105, 198,
                                 66,  14,  0,   255, 0,   127, 127, 127, 0,   0,
                                 0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

}  // namespace

uint32_t FontCreateShaders(uint32_t font, uint32_t frame, const FontCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821088B8 Font::CreateShaders");
  const auto declaration = CreateVertexDeclaration(kFontElements, calls.memcpy_call);
  Store(font + 16560, declaration.value_or(0));

  const uint32_t source = Load(kFontVertexSource);
  Store(frame + 84, frame + 100);
  Store(frame + 92, 0);
  calls.compile(0, source, Strlen(source), kVertexProfile, frame + 96);
  uint32_t data = calls.buffer_data(0, Load(frame + 96));
  Store(font + 16564, calls.vertex_shader(data));
  uint32_t r3 = calls.release(0, Load(frame + 96));

  for (int i = 0; i < 4; i++) {
    const uint32_t entry = kFontPixelEntries + 12 * i;
    Store(frame + 92, 0);
    Store(frame + 84, frame + 100);
    calls.compile(1 + i, Load(entry), Load(entry + 4), kPixelProfile, frame + 96);
    data = calls.buffer_data(1 + i, Load(frame + 96));
    Store(font + 16568 + 4 * i, calls.pixel_shader(i, data));
    r3 = calls.release(1 + i, Load(frame + 96));
  }
  return r3;
}

uint32_t FontConstruct(uint32_t font, uint32_t frame, uint32_t shaders_frame,
                       const FontCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82109228 Font::Font");
  namespace vo = graphics::video_output;
  Store(font + 0, calls.video_output());
  Store(font + 4, calls.render_object(0));
  Store(font + 8, calls.render_object(1));
  Store(frame + 88, vo::LockDevice(Load(font + 0)));
  FontCreateShaders(font, shaders_frame, calls);
  vo::UnlockDevice(Load(font + 0), frame + 88);
  for (uint32_t i = 0; i < 4; i++) {
    Store(font + 20 + 4 * i, 0);
    Store(font + 40 + 4 * i, 0);
  }
  // The five bytes the original keeps on its stack first (234, 0, 44, 242,
  // 242), then the colors.
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU8(frame + 80 + i, kColors[1 + i]);
  }
  for (uint32_t i = 0; i < 30; i++) {
    guest::StoreU8(font + 16528 + i, kColors[i]);
  }
  Store(font + 12, 0);
  return 14;
}

uint32_t TextInit(uint32_t font_frame, uint32_t shaders_frame,
                  const FontCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8210ADD0 Text::Init");
  // Font::Init (sub_82109A00): VirtualAlloc(0, 16592, MEM_COMMIT,
  // PAGE_READWRITE), then Font::Font.
  {
    ITC_NATIVE_RUNNING("sub_82109A00 Font::Init");
    // The font in host memory (E5 step 5): Font::New for VirtualAlloc's pages.
    const uint32_t font = schema::Font::New().address;
    Store(kFontPointer, font);
    FontConstruct(font, font_frame, shaders_frame, calls);
  }
  return calls.strings_init();
}

}  // namespace app_start_up
