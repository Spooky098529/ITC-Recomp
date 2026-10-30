// The app's start-up natively (start_up.h, docs/functions/app-start-up.md).

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

#include <algorithm>

namespace app_start_up {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t VirtualTarget(uint8_t* base, uint32_t object, uint32_t offset) {
  return Load(base, Load(base, object) + offset);
}

constexpr uint32_t kDeclarationAttributes = 0x24800000;

}  // namespace

void ReadErrorSetApp(uint8_t* base, uint32_t interface) {
  ITC_NATIVE_RUNNING("sub_8209AC50 ReadError::SetApp");
  Store(base, kReadErrorApp, interface);
}

void SetAppInterface84(uint8_t* base, uint32_t interface) {
  ITC_NATIVE_RUNNING("sub_8209AC30 SetAppInterface84");
  Store(base, kAppInterface84, interface);
}

void SetRenderApp(uint8_t* base, uint32_t interface) {
  ITC_NATIVE_RUNNING("sub_8209F580 SetRenderApp");
  Store(base, kRenderApp, interface);
}

void ClearWords8110(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8225FB70 ClearWords8110");
  Store(base, kWords8110, 0);
  Store(base, kWords8114, 0);
}

std::optional<uint32_t> MoviesSet(uint8_t* base, uint32_t interface) {
  if (VirtualTarget(base, interface, 44) != kInterfaceMovie) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A4360 Movies::Set");
  const uint32_t movie = Load(base, interface + 112);
  Store(base, kMovieClass, movie);
  return movie;
}

std::optional<uint32_t> CreateVertexDeclaration(uint8_t* base, uint32_t elements,
                                                const MemcpyCall& memcpy_call) {
  uint32_t count = 0;
  uint32_t highest = 0;
  uint8_t map[16] = {};
  for (uint32_t element = elements;; element += 12) {
    const uint32_t stream = guest::LoadU16(base, element);
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
  const uint32_t declaration = memory::crt::XMemAlloc(base, bytes + 56, kDeclarationAttributes);
  if (declaration == 0) {
    return 0;
  }
  for (uint32_t i = 0; i < 56; i++) {
    guest::StoreU8(base, declaration + i, 0);
  }
  Store(base, declaration + 24, count);
  Store(base, declaration + 28, highest);
  Store(base, declaration + 48, 0);
  Store(base, declaration + 4, 1);
  Store(base, declaration + 0, 0x00100005);
  Store(base, declaration + 20, 0xFFFF0000);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU8(base, declaration + 32 + i, map[i]);
  }
  if (!c_runtime::Memcpy(base, declaration + 52, elements, bytes)) {
    memcpy_call(declaration + 52, elements, bytes);
  }
  return declaration;
}

}  // namespace app_start_up
