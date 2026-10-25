#include "object_attributes.h"

#include <cstring>

namespace helpers {

namespace {

constexpr uint32_t kNamedObjectsRoot = 0xFFFFFFFC;  // -4
constexpr uint32_t kOpenIf = 0x80;

}  // namespace

AnsiString AnsiString_Init(const char* text, uint32_t address) {
  if (!text) {
    return {0, 0, 0};
  }
  uint16_t length = uint16_t(std::strlen(text));
  return {length, uint16_t(length + 1), address};
}

ObjectAttributes NamedObjectAttributes(uint32_t name_address) {
  return {kNamedObjectsRoot, name_address, kOpenIf};
}

}  // namespace helpers
