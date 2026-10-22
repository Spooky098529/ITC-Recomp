// The C runtime's string and character helpers natively (strings.h).

#include "c_runtime/strings.h"

#include "base/log.h"

#include "guest.h"

#include <cstring>
#include <iterator>

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
  ITC_NATIVE_RUNNING("sub_825A9DD8 strcpy_s");
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
  ITC_NATIVE_RUNNING("sub_825AC6B0 __ascii_strnicmp");
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

int32_t ToUpperStep(int32_t c) {
  return uint32_t(c - 'a') <= 25 ? -32 : 0;
}

int32_t ToLowerStep(int32_t c) {
  return uint32_t(c - 'A') <= 25 ? 32 : 0;
}

uint32_t CTypeC(uint8_t* base, uint32_t c, uint32_t mask) {
  const uint32_t table = guest::LoadU32(base, kCLocale + 200);
  return guest::LoadU16(base, table + 2 * c) & mask;
}

uint32_t IsLeadByte(uint8_t* base, uint32_t c) {
  const uint32_t locale = guest::LoadU32(base, kCurrentLocale);
  const uint32_t table = guest::LoadU32(base, locale + 200);
  return guest::LoadU16(base, table + 2 * (c & 0xFF)) & 0x8000;
}

bool StrToXl(uint8_t* base, uint32_t string, int32_t radix, uint32_t flags, bool want_end,
             uint32_t* end, uint64_t* result) {
  ITC_NATIVE_RUNNING("sub_825A82C8 strtoxl");
  if (string == 0 || (radix != 0 && (radix < 2 || radix > 36))) {
    return false;
  }
  const uint32_t locale = guest::LoadU32(base, kCurrentLocale);
  const uint32_t table = guest::LoadU32(base, locale + 200);
  auto type = [&](uint8_t c) { return guest::LoadU16(base, table + 2 * uint32_t(c)); };
  uint32_t next = string + 1;  // the byte after `c`
  uint8_t c = guest::LoadU8(base, string);
  for (;;) {
    if (int32_t(guest::LoadU32(base, locale + 172)) > 1) {
      return false;  // multibyte: sub_825AFD98, never seen in play
    }
    if ((type(c) & 8) == 0) {
      break;
    }
    c = guest::LoadU8(base, next++);
  }
  if (c == '-') {
    flags |= 2;
    c = guest::LoadU8(base, next++);
  } else if (c == '+') {
    c = guest::LoadU8(base, next++);
  }
  bool check_prefix = false;
  if (radix == 0) {
    if (c != '0') {
      radix = 10;
    } else {
      const uint8_t x = guest::LoadU8(base, next);
      if (x == 'x' || x == 'X') {
        radix = 16;
        check_prefix = true;
      } else {
        radix = 8;
      }
    }
  } else if (radix == 16) {
    check_prefix = true;
  }
  if (check_prefix && c == '0') {
    const uint8_t x = guest::LoadU8(base, next);
    if (x == 'x' || x == 'X') {
      c = guest::LoadU8(base, next + 1);
      next += 2;
    }
  }
  const uint32_t unsigned_radix = uint32_t(radix);
  const uint32_t limit = 0xFFFFFFFFu / unsigned_radix;
  uint32_t value = 0;
  for (;;) {
    const uint16_t t = type(c);
    int32_t digit;
    if (t & 4) {
      digit = int32_t(int8_t(c)) - '0';
    } else if (t & 0x103) {
      digit = int32_t(int8_t(c));
      if (digit >= 'a' && digit <= 'z') {
        digit -= 32;
      }
      digit -= 55;
    } else {
      break;
    }
    if (!(uint32_t(digit) < unsigned_radix)) {
      break;
    }
    flags |= 8;
    if (value < limit ||
        (value == limit && uint32_t(digit) <= 0xFFFFFFFFu - limit * unsigned_radix)) {
      value = value * unsigned_radix + uint32_t(digit);
    } else {
      flags |= 4;  // overflow: the rest of the digits are read without adding
      if (!want_end) {
        break;
      }
    }
    c = guest::LoadU8(base, next++);
  }
  next--;
  if ((flags & 8) == 0) {
    *end = string;
    *result = 0;
    return true;
  }
  if ((flags & 4) != 0) {
    return false;  // ERANGE through _errno, never seen in play
  }
  if ((flags & 1) == 0) {
    if ((flags & 2) != 0 ? value > 0x80000000u : value > 0x7FFFFFFFu) {
      return false;
    }
  }
  *end = next;
  *result = (flags & 2) != 0 ? 0 - uint64_t(value) : uint64_t(value);
  return true;
}

namespace {

// The guest's views of host memory and where each starts in the memory behind
// them (map_info in the SDK's src/system/xmemory.cpp): 0x7F000000 and the
// views from 0xA0000000 show the physical memory (from 0xE0000000 offset by
// 4 KB), 0x80000000 and 0x90000000 both the xex.
struct View {
  uint32_t start;
  uint64_t target;
};
constexpr View kViews[] = {
    {0x00000000, 0x000000000}, {0x40000000, 0x040000000}, {0x7F000000, 0x100000000},
    {0x80000000, 0x080000000}, {0x90000000, 0x080000000}, {0xA0000000, 0x100000000},
    {0xC0000000, 0x100000000}, {0xE0000000, 0x100001000},
};

// Where `size` bytes at `address` start in the memory behind the views;
// false when they cross into the next view (or past 4 GB). `view` is the
// view's index, when asked for.
bool Behind(uint32_t address, uint32_t size, uint64_t* start, size_t* view = nullptr) {
  size_t i = std::size(kViews) - 1;
  while (address < kViews[i].start) {
    i--;
  }
  const uint64_t end = i + 1 < std::size(kViews) ? kViews[i + 1].start : 0x100000000ull;
  if (uint64_t(address) + size > end) {
    return false;
  }
  *start = kViews[i].target + (address - kViews[i].start);
  if (view) {
    *view = i;
  }
  return true;
}

}  // namespace

bool InOneView(uint32_t address, uint32_t size) {
  uint64_t start;
  return Behind(address, size, &start);
}

bool MayShare(uint32_t a, uint32_t a_size, uint32_t b, uint32_t b_size) {
  uint64_t from_a, from_b;
  if (!Behind(a, a_size, &from_a) || !Behind(b, b_size, &from_b)) {
    return true;
  }
  return from_a < from_b + b_size && from_b < from_a + a_size;
}

bool Memcpy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size) {
  if (size == 0) {
    return true;
  }
  uint64_t to, from;
  if (!Behind(destination, size, &to) || !Behind(source, size, &from)) {
    return false;
  }
  if (to < from + size && from < to + size) {
    return false;  // overlapping: the original's loop order decides
  }
  std::memcpy(REX_RAW_ADDR(destination), REX_RAW_ADDR(source), size);
  return true;
}

bool MemmoveDown(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size) {
  if (size == 0) {
    return true;
  }
  uint64_t to, from;
  size_t to_view, from_view;
  if (!Behind(destination, size, &to, &to_view) || !Behind(source, size, &from, &from_view)) {
    return false;
  }
  if (to < from + size && from < to + size) {
    if (to_view != from_view) {
      return false;  // the same memory through two views: the original decides
    }
    // One view: the guest's order is the host's, and the original's backward
    // copy gives what memmove does.
    std::memmove(REX_RAW_ADDR(destination), REX_RAW_ADDR(source), size);
    return true;
  }
  std::memcpy(REX_RAW_ADDR(destination), REX_RAW_ADDR(source), size);
  return true;
}

}  // namespace c_runtime
