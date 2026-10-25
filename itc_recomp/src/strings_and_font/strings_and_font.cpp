// The string table and the font as plain C++: see strings_and_font.h.

#include "strings_and_font/strings_and_font.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "c_runtime/strings.h"
#include "game_data/data_file.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/xpr.h"

#include "strings_and_font/steps.h"

#include <bit>
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

bool Swprintf(uint8_t* base, uint32_t buffer, uint32_t format, uint32_t args, uint32_t frame,
              int32_t* count) {
  ITC_NATIVE_RUNNING("sub_8210A490 swprintf");
  return c_runtime::Vswprintf(base, buffer, format, args, frame, count);
}

namespace {

// swprintf (natively) of `format` with one argument from a frame at
// `frame` (the caller's r1): the argument in the caller's parameter area
// (+32), as the original's stores it.
void SwprintfOne(uint8_t* base, uint32_t buffer, uint32_t format, uint64_t argument,
                 uint32_t frame) {
  guest::StoreU32(base, frame + 32, uint32_t(argument >> 32));
  guest::StoreU32(base, frame + 36, uint32_t(argument));
  int32_t count;
  Swprintf(base, buffer, format, frame + 32, frame - 96 - 144 - 2336, &count);
}

constexpr uint32_t kGroupedFormat = 0x82007E9C;  // L",%03d"
constexpr uint32_t kPlainFormat = 0x82007E94;    // L"%d"
constexpr uint32_t kMinusFormat = 0x82007E8C;    // L"-%s"

void FormatGrouped(uint8_t* base, int32_t value, uint32_t out, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8210A680 Strings::FormatGrouped");
  uint64_t v = uint64_t(int64_t(value));
  const bool negative = value < 0;
  if (negative) {
    v = 0 - v;
  }
  const uint32_t buffer = frame + 96;
  const int32_t magnitude = int32_t(uint32_t(v));
  const double thousands =
      Single(ToSingle(magnitude) * Lfs(base, 0x820029D8));  // x 0.001
  if (thousands < Lfs(base, 0x82000A30)) {                  // below 1.0
    SwprintfOne(base, buffer, kPlainFormat, v, frame);
  } else {
    const int32_t rest = int32_t(uint32_t(v) - uint32_t(magnitude / 1000) * 1000);
    FormatGrouped(base, int32_t(uint32_t(v) - uint32_t(rest)) / 1000, out, frame - 640);
    SwprintfOne(base, buffer, kGroupedFormat, uint64_t(int64_t(rest)), frame);
  }
  c_runtime::Wcscat(base, out, buffer);
  if (negative) {
    SwprintfOne(base, buffer, kMinusFormat, out, frame);
    c_runtime::Wcscpy(base, out, buffer);
  }
}

}  // namespace

bool StringsFormatGrouped(uint8_t* base, int32_t value, uint32_t out, uint32_t frame) {
  if (value < 0) {
    // "-%s" of `out` after the digits: no 0xFFFF in it, and with the sign,
    // the digits (11 at most) and the terminator within the 256 units.
    const uint32_t length = c_runtime::Wcslen(base, out);
    if (length > 240) {
      return false;
    }
    for (uint32_t k = 0; k < length; k++) {
      if (guest::LoadU16(base, out + 2 * k) == 0xFFFF) {
        return false;
      }
    }
  }
  FormatGrouped(base, value, out, frame);
  return true;
}

namespace {

constexpr uint32_t kFormatFrame = 4800;
constexpr uint32_t kIntFormat = 0x82007EA8;     // L"%%%dd"
constexpr uint32_t kFloatFormat = 0x82007EB4;   // L"%%.%df"
constexpr uint32_t kZeroedFormat = 0x82007EC4;  // L"%%0%dd"
constexpr uint32_t kSwprintf = 0x8210A490;
constexpr uint32_t kFormatGroupedFunction = 0x8210A680;
constexpr uint32_t kFontNumber = 0x82108A70;

// swprintf of `format` with one argument from FormatV's frame (its
// parameter area at +32 written as the original's stores): natively, else
// its adapter through `calls`.
void FormatSwprintf(uint8_t* base, const FormatCalls& calls, uint32_t frame, uint32_t buffer,
                    uint32_t format, uint64_t argument, uint32_t lr) {
  guest::StoreU32(base, frame + 32, uint32_t(argument >> 32));
  guest::StoreU32(base, frame + 36, uint32_t(argument));
  int32_t count;
  if (!Swprintf(base, buffer, format, frame + 32, frame - 96 - 144 - 2336, &count)) {
    calls.call(calls.user, kSwprintf, buffer, format, argument, frame, lr);
  }
}

// memset(buffer, 0, 4096).
void ClearBuffer(uint8_t* base, uint32_t buffer) {
  for (uint32_t at = 0; at < 4096; at += 4) {
    guest::StoreU32(base, buffer + at, 0);
  }
}

// The next 8-byte slot of the va_list at `args`: its address, `args` moved
// past it.
uint32_t NextSlot(uint32_t& args) {
  const uint32_t slot = (args + 7) & ~7u;
  args = slot + 8;
  return slot;
}

}  // namespace

uint32_t StringsFormatV(uint8_t* base, const FormatCalls& calls, uint32_t stack, uint32_t table,
                        uint32_t out, uint32_t id, uint32_t args) {
  ITC_NATIVE_RUNNING("sub_8210A990 Strings::FormatV");
  const uint32_t frame = stack - kFormatFrame;
  guest::StoreU32(base, frame, stack);  // the back chain, as stwu
  const uint32_t buffer = frame + 608;
  const uint32_t format = frame + 96;
  uint32_t text = StringsGet(base, table, id & 0xFFFF).value_or(table);
  ClearBuffer(base, buffer);
  for (uint32_t c; (c = guest::LoadU16(base, text)) != 0; text += 2) {
    uint32_t append = buffer;
    if (c == 0xFFD7) {
      const uint32_t slot = NextSlot(args);
      const uint32_t inner = guest::LoadU16(base, slot + 6);
      ClearBuffer(base, buffer);
      append = StringsFormatV(base, calls, frame, table, buffer, inner, args);
    } else if (c > 0xFFEB && c <= 0xFFF5) {
      const uint32_t slot = NextSlot(args);
      const uint32_t value = guest::LoadU32(base, slot + 4);
      FormatSwprintf(base, calls, frame, format, kIntFormat, (0xFFF5 - c) & 0xFFFF, 0x8210AA78);
      FormatSwprintf(base, calls, frame, buffer, format, value, 0x8210AA88);
    } else if (c > 0xFFE1 && c <= 0xFFEB) {
      const uint32_t slot = NextSlot(args);
      const double value =
          std::bit_cast<double>((uint64_t(guest::LoadU32(base, slot)) << 32) |
                                guest::LoadU32(base, slot + 4));
      FormatSwprintf(base, calls, frame, format, kFloatFormat, (0xFFEB - c) & 0xFFFF, 0x8210AAD0);
      const double rounded = double(float(value));
      FormatSwprintf(base, calls, frame, buffer, format, std::bit_cast<uint64_t>(rounded),
                     0x8210AAE8);
    } else if (c > 0xFFD7 && c <= 0xFFE1) {
      const uint32_t slot = NextSlot(args);
      const uint32_t value = guest::LoadU32(base, slot + 4);
      FormatSwprintf(base, calls, frame, format, kZeroedFormat, (0xFFE1 - c) & 0xFFFF, 0x8210AA78);
      FormatSwprintf(base, calls, frame, buffer, format, value, 0x8210AA88);
    } else if (c == 0xFFD3) {
      const uint32_t slot = NextSlot(args);
      ClearBuffer(base, buffer);
      const uint32_t value = guest::LoadU32(base, slot + 4);
      if (!StringsFormatGrouped(base, int32_t(value), buffer, frame - 640)) {
        calls.call(calls.user, kFormatGroupedFunction, table, value, buffer, frame, 0x8210AB4C);
      }
    } else if (c == 0xFFD2) {
      const uint32_t slot = NextSlot(args);
      ClearBuffer(base, buffer);
      calls.call(calls.user, kFontNumber, guest::LoadU32(base, kFont), buffer,
                 guest::LoadU32(base, slot + 4), frame, 0x8210AB84);
    } else {
      guest::StoreU16(base, frame + 80, uint16_t(c));
      guest::StoreU16(base, frame + 82, 0);
      append = frame + 80;
    }
    c_runtime::Wcscat(base, out, append);
  }
  return out;
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

std::optional<uint32_t> StringsGet(uint8_t* base, uint32_t table, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210A800 Strings::Get");
  constexpr uint32_t kBounds = 0x82007E18;
  for (uint32_t i = 0; i < 17; i++) {
    if (int32_t(id) < int32_t(guest::LoadU32(base, kBounds + 4 + 4 * i))) {
      const uint32_t bound = guest::LoadU32(base, kBounds + 4 * i);
      return game_data::GetEntry(base, guest::LoadU32(base, table + 4 * i), id - bound, false);
    }
  }
  return std::nullopt;
}

std::optional<uint32_t> ReleaseXprSlot(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_82108860 XPR::Release (a slot)");
  const uint32_t xpr = guest::LoadU32(base, slot);
  if (xpr == 0) {
    return slot;
  }
  if (!memory::IsHeapBlock(xpr) || !models::XprDestruct(base, xpr)) {
    return std::nullopt;
  }
  memory::crt::OperatorDelete(xpr);
  guest::StoreU32(base, slot, 0);
  return xpr;
}

}  // namespace strings_and_font
