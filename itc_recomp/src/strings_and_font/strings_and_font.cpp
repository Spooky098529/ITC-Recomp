// The string table and the font as plain C++: see strings_and_font.h.

#include "strings_and_font/strings_and_font.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/heap.h"

#include "strings_and_font/steps.h"

#include <cmath>

namespace strings_and_font {

using namespace steps;

namespace {

constexpr uint32_t kPageBreak = 0xFFD1;

// The start point's coordinate: (int)(position - (scale - 1) * origin), the
// subtraction fused as the original's fnmsub, back to a float.
float StartCoordinate(double scale_less_one, int32_t position, int32_t origin) {
  const double p = ToSingle(position);
  const double o = ToSingle(origin);
  const double value = -std::fma(scale_less_one, o, -p);
  return float(ToSingle(Fctiwz(value)));
}

}  // namespace

uint32_t StringsInit(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8210A860 Strings::Init");
  if (guest::LoadU32(base, kStrings) != 0) {
    return 0;
  }
  const uint32_t table = memory::AllocPages(kStringsBytes, 4096);
  guest::StoreU32(base, kStrings, table);
  for (uint32_t i = 0; i < kStringFiles; i++) {
    guest::StoreU32(base, table + 4 * i, 0);
    guest::StoreU32(base, table + 4236 + 4 * i, 0);
  }
  guest::StoreU32(base, table + 136, 0);
  return table;
}

void StringsOnLoaded(uint8_t* base, uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_8210A850 Strings::OnLoaded");
  guest::StoreU32(base, flag, 1);
}

bool StringsRelease(uint8_t* base, uint32_t table, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_8210A538 Strings::Release");
  // The files it would free, checked before anything is written.
  for (uint32_t i = 0; i < kStringFiles; i++) {
    if (!(mask & (1u << i))) {
      continue;
    }
    const uint32_t file = guest::LoadU32(base, table + 4 * i);
    if (file != 0 && guest::LoadU32(base, table + 4236 + 4 * i) == 1 &&
        !memory::IsPages(file)) {
      return false;
    }
  }
  for (uint32_t i = 0; i < kStringFiles; i++) {
    const uint32_t bit = 1u << i;
    if (!(bit & mask)) {
      continue;
    }
    const uint32_t file = guest::LoadU32(base, table + 4 * i);
    if (file == 0) {
      continue;
    }
    const uint32_t count = guest::LoadU32(base, table + 4236 + 4 * i) - 1;
    guest::StoreU32(base, table + 4236 + 4 * i, count);
    if (count != 0) {
      continue;
    }
    memory::FreePages(file);
    guest::StoreU32(base, table + 4 * i, 0);
    guest::StoreU32(base, table + 68 + 4 * i, 0);
    guest::StoreU32(base, table + 136, guest::LoadU32(base, table + 136) & ~bit);
  }
  return true;
}

uint32_t FontIsLoaded(uint8_t* base, uint32_t font) {
  ITC_NATIVE_RUNNING("sub_821081A0 Font::IsLoaded");
  const uint32_t mask = guest::LoadU32(base, font + 12);
  uint32_t asked = 0;
  for (uint32_t i = 0; i < 4; i++) {
    if (!(mask & (1u << i))) {
      continue;
    }
    asked++;
    const uint32_t page = guest::LoadU32(base, font + 20 + 4 * i);
    if (guest::LoadU32(base, page + 24) == 0) {
      return 0;
    }
  }
  return asked != 0 ? 1 : 0;
}

uint32_t StringsIsLoaded(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_8210A768 Strings::IsLoaded");
  const uint32_t mask = guest::LoadU32(base, table + 136);
  bool all = true;
  uint32_t asked = 0;
  for (uint32_t i = 0; i < kStringFiles; i++) {
    const uint32_t bit = (1u << i) & mask;
    if (bit != 0 && guest::LoadU32(base, table + 68 + 4 * i) == 0) {
      all = false;
    }
    if (bit != 0) {
      asked++;
    }
  }
  if (asked == 0 || !all) {
    return 0;
  }
  return FontIsLoaded(base, guest::LoadU32(base, kFont)) != 0 ? 1 : 0;
}

uint32_t TextIsLoaded(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82211C90 Text::IsLoaded");
  if (StringsIsLoaded(base, guest::LoadU32(base, kStrings)) == 0) {
    return 0;
  }
  return FontIsLoaded(base, guest::LoadU32(base, kFont)) != 0 ? 1 : 0;
}

uint32_t StringsPageCount(uint8_t* base, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_8210A5C0 Strings::PageCount");
  uint32_t count = 0;
  for (uint32_t c; (c = guest::LoadU16(base, text)) != 0; text += 2) {
    if (c == kPageBreak) {
      count++;
    }
  }
  return count;
}

uint32_t StringsCopyPage(uint8_t* base, uint32_t out, uint32_t text, int32_t page, int32_t skip) {
  ITC_NATIVE_RUNNING("sub_8210A5E8 Strings::CopyPage");
  while (page > 0) {
    const uint32_t c = guest::LoadU16(base, text);
    if (c == 0) {
      break;
    }
    if (c == kPageBreak) {
      page--;
    }
    text += 2;
  }
  for (uint32_t c; (c = guest::LoadU16(base, text)) != 0; text += 2) {
    if (c == kPageBreak) {
      break;
    }
    if (skip <= 0) {
      // wcscat(out, {c, 0}).
      const uint32_t end = out + 2 * c_runtime::Wcslen(base, out);
      guest::StoreU16(base, end, uint16_t(c));
      guest::StoreU16(base, end + 2, 0);
    }
    if (guest::LoadU16(base, text) == '\n') {
      skip--;
    }
  }
  return out;
}

void InitShaderEntries(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D9590 (the font's pixel shader entries)");
  // The stores in the original's order (each length after the next source).
  uint32_t lengths[4];
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t entry = kShaderEntries + 12 * i;
    const uint32_t source = guest::LoadU32(base, 0x8260333C + 4 * i);
    if (i > 0) {
      guest::StoreU32(base, entry - 4, 0);
    }
    guest::StoreU32(base, entry, source);
    if (i > 0) {
      guest::StoreU32(base, entry - 8, lengths[i - 1]);
    }
    uint32_t end = source;
    while (guest::LoadU8(base, end) != 0) {
      end++;
    }
    lengths[i] = end - source;
  }
  guest::StoreU32(base, kShaderEntries + 44, 0);
  guest::StoreU32(base, kShaderEntries + 40, lengths[3]);
}

void FontSetParams(uint8_t* base, uint32_t font, uint32_t params) {
  ITC_NATIVE_RUNNING("sub_82108368 Font::SetParams");
  guest::StoreU32(base, font + 16, params);
  const double one = Lfd(base, 0x82002250);
  const int32_t x = int32_t(guest::LoadU32(base, params + 0));
  const double scale_x = double(guest::LoadFloat(base, params + 28)) - one;
  const int32_t origin_x = int32_t(guest::LoadU32(base, params + 8));
  guest::StoreFloat(base, font + 56, StartCoordinate(scale_x, x, origin_x));
  const int32_t y = int32_t(guest::LoadU32(base, params + 4));
  const double scale_y = double(guest::LoadFloat(base, params + 32)) - one;
  const int32_t origin_y = int32_t(guest::LoadU32(base, params + 12));
  guest::StoreFloat(base, font + 60, StartCoordinate(scale_y, y, origin_y));
  guest::StoreFloat(base, font + 16448,
                    float(ToSingle(int32_t(guest::LoadU32(base, params + 84)))));
  guest::StoreFloat(base, font + 16452,
                    float(ToSingle(int32_t(guest::LoadU32(base, params + 88)))));
}

}  // namespace strings_and_font
