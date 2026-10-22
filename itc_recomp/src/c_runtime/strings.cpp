// The C runtime's string and character helpers natively (strings.h).

#include "c_runtime/strings.h"

#include "guest.h"

namespace c_runtime {

uint32_t Wcslen(uint8_t* base, uint32_t string) {
  uint32_t n = 0;
  while (guest::LoadU16(base, string + 2 * n) != 0) {
    n++;
  }
  return n;
}

void Wcscpy(uint8_t* base, uint32_t destination, uint32_t source) {
  for (;;) {
    const uint16_t unit = guest::LoadU16(base, source);
    guest::StoreU16(base, destination, unit);
    if (unit == 0) {
      return;
    }
    source += 2;
    destination += 2;
  }
}

void Wcscat(uint8_t* base, uint32_t destination, uint32_t source) {
  while (guest::LoadU16(base, destination) != 0) {
    destination += 2;
  }
  Wcscpy(base, destination, source);
}

int32_t Wcscmp(uint8_t* base, uint32_t a, uint32_t b) {
  for (;;) {
    const uint16_t x = guest::LoadU16(base, a);
    const uint16_t y = guest::LoadU16(base, b);
    if (x != y) {
      return x < y ? -1 : 1;
    }
    if (y == 0) {
      return 0;
    }
    a += 2;
    b += 2;
  }
}

int32_t Mbtowc(uint8_t* base, uint32_t wide, uint32_t bytes, uint32_t count) {
  if (bytes == 0 || count == 0) {
    return 0;
  }
  const uint8_t byte = guest::LoadU8(base, bytes);
  if (wide != 0) {
    guest::StoreU16(base, wide, byte);
  }
  return byte != 0 ? 1 : 0;
}

}  // namespace c_runtime
