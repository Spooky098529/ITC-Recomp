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

void Strncpy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t count) {
  if (count == 0) {
    return;
  }
  for (;;) {
    const uint8_t byte = guest::LoadU8(base, source++);
    guest::StoreU8(base, destination++, byte);
    if (byte == 0) {
      break;
    }
    if (--count == 0) {
      return;
    }
  }
  // The terminator took one of the bytes left.
  for (uint32_t i = 1; i < count; i++) {
    guest::StoreU8(base, destination++, 0);
  }
}

bool StrcpyS(uint8_t* base, uint32_t destination, uint32_t size, uint32_t source) {
  if (destination == 0 || size == 0 || source == 0) {
    return false;
  }
  uint32_t length = 0;  // fits when the terminator is within the first `size` bytes
  while (guest::LoadU8(base, source + length) != 0) {
    if (++length == size) {
      return false;
    }
  }
  if (destination > source && destination - source <= length) {
    return false;
  }
  for (uint32_t i = 0; i <= length; i++) {
    guest::StoreU8(base, destination + i, guest::LoadU8(base, source + i));
  }
  return true;
}

int32_t Strncmp(uint8_t* base, uint32_t a, uint32_t b, int32_t count) {
  if (count <= 0) {
    return 0;
  }
  for (uint32_t left = uint32_t(count);; a++, b++) {
    const uint8_t x = guest::LoadU8(base, a);
    const int32_t difference = int32_t(x) - int32_t(guest::LoadU8(base, b));
    if (--left == 0 || x == 0 || difference != 0) {
      return difference;
    }
  }
}

namespace {

int32_t FoldAscii(int32_t c) {
  return uint32_t(c - 'A') <= 25 ? c + 32 : c;
}

}  // namespace

int32_t Stricmp(uint8_t* base, uint32_t a, uint32_t b) {
  for (;; a++, b++) {
    const int32_t y = guest::LoadU8(base, b);
    const int32_t x = guest::LoadU8(base, a);
    if (y == 0) {
      return x - y;
    }
    if (x == y) {
      continue;
    }
    const int32_t difference = FoldAscii(x) - FoldAscii(y);
    if (difference != 0) {
      return difference;
    }
  }
}

int32_t AsciiStrnicmp(uint8_t* base, uint32_t a, uint32_t b, uint32_t count) {
  if (count == 0) {
    return 0;
  }
  for (;; a++, b++) {
    const int32_t x = FoldAscii(guest::LoadU8(base, a));
    const int32_t y = FoldAscii(guest::LoadU8(base, b));
    if (--count == 0 || x == 0 || x != y) {
      return x - y;
    }
  }
}

int64_t Strstr(uint8_t* base, uint32_t string, uint32_t find) {
  if (guest::LoadU8(base, find) == 0) {
    return 0;
  }
  for (uint32_t at = 0; guest::LoadU8(base, string + at) != 0; at++) {
    uint32_t k = 0;
    while (guest::LoadU8(base, find + k) != 0 &&
           guest::LoadU8(base, string + at + k) == guest::LoadU8(base, find + k)) {
      k++;
      if (guest::LoadU8(base, string + at + k) == 0) {
        break;
      }
    }
    if (guest::LoadU8(base, find + k) == 0) {
      return at;
    }
  }
  return -1;
}

void CopyForward(uint8_t* base, uint32_t destination, uint32_t source, uint32_t count) {
  while (count != 0 && (destination & 3) != 0) {
    guest::StoreU8(base, destination++, guest::LoadU8(base, source++));
    count--;
  }
  for (uint32_t words = count >> 2; words != 0; words--) {
    // Read whole before it's written, as the original's word (or four
    // bytes, from an unaligned source) is.
    const uint32_t word = guest::LoadU32(base, source);
    guest::StoreU32(base, destination, word);
    source += 4;
    destination += 4;
  }
  for (uint32_t rest = count & 3; rest != 0; rest--) {
    guest::StoreU8(base, destination++, guest::LoadU8(base, source++));
  }
}

}  // namespace c_runtime
