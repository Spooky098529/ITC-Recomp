// The app's start-up natively (start_up.h, docs/functions/app-start-up.md).

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/app_start_up.h"
#include "schema/movies.h"

#include <algorithm>

namespace app_start_up {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }

constexpr uint32_t kDeclarationAttributes = 0x24800000;

}  // namespace

using Globals = schema::AppStartUpGlobals;

void ReadErrorSetApp(uint32_t interface) {
  ITC_NATIVE_RUNNING("sub_8209AC50 ReadError::SetApp");
  Globals::set_read_error_app(interface);
}

void SetAppInterface84(uint32_t interface) {
  ITC_NATIVE_RUNNING("sub_8209AC30 SetAppInterface84");
  Globals::set_app_interface_84(interface);
}

void SetRenderApp(uint32_t interface) {
  ITC_NATIVE_RUNNING("sub_8209F580 SetRenderApp");
  Globals::set_render_app(interface);
}

void ClearWords8110() {
  ITC_NATIVE_RUNNING("sub_8225FB70 ClearWords8110");
  Globals::set_word_82816110(0);
  Globals::set_word_82816114(0);
}

std::optional<uint32_t> MoviesSet(uint32_t interface) {
  const schema::AppInterface16 app(interface);
  if (Load(app.vtable() + 44) != kInterfaceMovie) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A4360 Movies::Set");
  const uint32_t movie = app.movie();
  schema::MoviesGlobals::set_movie_class(movie);
  return movie;
}

std::optional<uint32_t> CreateVertexDeclaration(uint32_t elements,
                                                const MemcpyCall& memcpy_call) {
  uint32_t count = 0;
  uint32_t highest = 0;
  uint8_t map[16] = {};
  for (uint32_t element = elements;; element += 12) {
    const uint32_t stream = schema::VertexElement(element).stream();
    if (stream == 255) {
      break;
    }
    if (stream > 15) {
      return std::nullopt;
    }
    highest = std::max(highest, stream);
    map[stream] = 255;
    count++;
  }
  ITC_NATIVE_RUNNING("sub_822A8980 CreateVertexDeclaration");
  const uint32_t bytes = count * 12;
  const uint32_t declaration = memory::crt::XMemAlloc(bytes + 56, kDeclarationAttributes);
  if (declaration == 0) {
    return 0;
  }
  for (uint32_t i = 0; i < 56; i++) {
    guest::StoreU8(declaration + i, 0);
  }
  const schema::VertexDeclaration made(declaration);
  made.set_element_count(count);
  made.set_highest_stream(highest);
  made.set_unknown_48(0);
  made.set_reference_count(1);
  made.set_common(0x00100005);
  made.set_unknown_20(0xFFFF0000);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU8(made.stream_map_address() + i, map[i]);
  }
  if (!c_runtime::Memcpy(declaration + 52, elements, bytes)) {
    memcpy_call(declaration + 52, elements, bytes);
  }
  return declaration;
}

uint32_t AppStartUp(uint32_t interface, uint32_t frame, const StartUpCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82106548 App::StartUp");
  // GlobalMemoryStatus(frame + 80) (Dropped) left out.
  ReadErrorSetApp(interface);
  SetAppInterface84(interface);
  SetRenderApp(interface);
  calls.call(0x8209B378, interface, std::nullopt, 0x82106584);  // SceneTarget::Create
  const uint32_t pool = calls.virtual_call(16, 0x82106598);
  calls.call(0x8209E210, pool, 0xC80000, 0x821065A0);         // Archive::Init
  calls.call(0x8209F430, interface, 1, 0x821065AC);           // ActionMappers::Init
  calls.call(0x8209F9B8, std::nullopt, std::nullopt, 0x821065B0);  // CameraPasses::Init
  calls.call(0x8209CDE8, std::nullopt, std::nullopt, 0x821065B4);  // ScreenFades::Init
  uint32_t movie;
  if (const auto answer = MoviesSet(interface)) {
    movie = *answer;
  } else {
    movie = calls.call(0x820A4360, interface, std::nullopt, 0x821065BC);  // Movies::Set
  }
  calls.call(0x8209BE88, movie, std::nullopt, 0x821065C0);  // Billboards::Init
  const uint32_t music = calls.virtual_call(48, 0x821065D4);
  calls.call(0x82110258, music, std::nullopt, 0x821065D8);      // SoundGlobals::Init
  calls.call(0x8209FFB8, interface, std::nullopt, 0x821065E0);  // SaveInterface::Init
  calls.call(0x8214E190, interface, std::nullopt, 0x821065E8);  // Network::SetUp
  calls.call(0x8210ADD0, std::nullopt, std::nullopt, 0x821065EC);  // Text::Init
  ClearWords8110();
  calls.call(0x8209F5E0, std::nullopt, std::nullopt, 0x821065F4);  // Players::ResetControllers
  calls.call(0x8209D730, std::nullopt, std::nullopt, 0x821065F8);  // Archive::WaitIdle
  // GlobalMemoryStatus(frame + 80) again (Dropped): r3 its buffer.
  return frame + 80;
}

}  // namespace app_start_up
