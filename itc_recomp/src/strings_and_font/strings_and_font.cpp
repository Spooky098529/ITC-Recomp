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
#include "schema/strings_and_font.h"

#include "strings_and_font/steps.h"

#include <bit>
#include <cmath>

namespace strings_and_font {

using namespace steps;

namespace {

using G = schema::StringsAndFontGlobals;
using schema::Font;
using schema::FontDrawParams;
using schema::FontPage;
using schema::Strings;

// The string table's file i: its pointer, loaded flag and count of loads.
// The table is a host object (E5 step 5): its words read and written through
// its view (by offset); the flag's address goes to Archive::Load.
constexpr uint32_t kFilesOffset = 0, kFlagsOffset = 68, kCountsOffset = 4236;
uint32_t FlagSlot(const Strings& t, uint32_t i) { return t.loaded_flags_address() + 4 * i; }
uint32_t File(const Strings& t, uint32_t i) { return uint32_t(t.load_at(kFilesOffset + 4 * i, 4)); }
void SetFile(const Strings& t, uint32_t i, uint32_t v) { t.store_at(kFilesOffset + 4 * i, 4, v); }
uint32_t Flag(const Strings& t, uint32_t i) { return uint32_t(t.load_at(kFlagsOffset + 4 * i, 4)); }
void SetFlag(const Strings& t, uint32_t i, uint32_t v) { t.store_at(kFlagsOffset + 4 * i, 4, v); }
uint32_t Count(const Strings& t, uint32_t i) { return uint32_t(t.load_at(kCountsOffset + 4 * i, 4)); }
void SetCount(const Strings& t, uint32_t i, uint32_t v) { t.store_at(kCountsOffset + 4 * i, 4, v); }

// The font's words by offset (a host object, E5 step 5): its pages (+20 + 4i)
// and the pages' files (+40 + 4i).
constexpr uint32_t kPagesOffset = 20, kPageFilesOffset = 40;
uint32_t FontWord(const Font& f, uint32_t offset) { return uint32_t(f.load_at(offset, 4)); }

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

uint32_t StringsInit() {
  ITC_NATIVE_RUNNING("sub_8210A860 Strings::Init");
  if (G::strings().address != 0) {
    return 0;
  }
  // VirtualAlloc's 4,304 bytes: the table in host memory (E5 step 5).
  const Strings table = Strings::New();
  G::set_strings(table);
  for (uint32_t i = 0; i < kStringFiles; i++) {
    SetFile(table, i, 0);
    SetCount(table, i, 0);
  }
  table.set_files_asked(0);
  return table.address;
}

void StringsOnLoaded(uint32_t flag) {
  ITC_NATIVE_RUNNING("sub_8210A850 Strings::OnLoaded");
  schema::StringsLoadedFlag(flag).set_loaded(1);
}

bool StringsRelease(uint32_t table_address, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_8210A538 Strings::Release");
  const Strings table(table_address);
  // The files it would free, checked before anything is written.
  for (uint32_t i = 0; i < kStringFiles; i++) {
    if (!(mask & (1u << i))) {
      continue;
    }
    const uint32_t file = File(table, i);
    if (file != 0 && Count(table, i) == 1 &&
        !memory::IsPages(file)) {
      return false;
    }
  }
  for (uint32_t i = 0; i < kStringFiles; i++) {
    const uint32_t bit = 1u << i;
    if (!(bit & mask)) {
      continue;
    }
    const uint32_t file = File(table, i);
    if (file == 0) {
      continue;
    }
    const uint32_t count = Count(table, i) - 1;
    SetCount(table, i, count);
    if (count != 0) {
      continue;
    }
    memory::FreePages(file);  // a StringFile blob: freed through its handle
    SetFile(table, i, 0);
    SetFlag(table, i, 0);
    table.set_files_asked(table.files_asked() & ~bit);
  }
  return true;
}

uint32_t FontIsLoaded(uint32_t font_address) {
  ITC_NATIVE_RUNNING("sub_821081A0 Font::IsLoaded");
  const Font font(font_address);
  const uint32_t mask = font.pages_asked();
  uint32_t asked = 0;
  for (uint32_t i = 0; i < 4; i++) {
    if (!(mask & (1u << i))) {
      continue;
    }
    asked++;
    const FontPage page(FontWord(font, kPagesOffset + 4 * i));
    if (page.loaded() == 0) {
      return 0;
    }
  }
  return asked != 0 ? 1 : 0;
}

uint32_t StringsIsLoaded(uint32_t table_address) {
  ITC_NATIVE_RUNNING("sub_8210A768 Strings::IsLoaded");
  const Strings table(table_address);
  const uint32_t mask = table.files_asked();
  bool all = true;
  uint32_t asked = 0;
  for (uint32_t i = 0; i < kStringFiles; i++) {
    const uint32_t bit = (1u << i) & mask;
    if (bit != 0 && Flag(table, i) == 0) {
      all = false;
    }
    if (bit != 0) {
      asked++;
    }
  }
  if (asked == 0 || !all) {
    return 0;
  }
  return FontIsLoaded(G::font().address) != 0 ? 1 : 0;
}

uint32_t TextIsLoaded() {
  ITC_NATIVE_RUNNING("sub_82211C90 Text::IsLoaded");
  if (StringsIsLoaded(G::strings().address) == 0) {
    return 0;
  }
  return FontIsLoaded(G::font().address) != 0 ? 1 : 0;
}

bool Swprintf(uint32_t buffer, uint32_t format, uint32_t args, uint32_t frame,
              int32_t* count) {
  ITC_NATIVE_RUNNING("sub_8210A490 swprintf");
  return c_runtime::Vswprintf(buffer, format, args, frame, count);
}

namespace {

// swprintf (natively) of `format` with one argument from a frame at
// `frame` (the caller's r1): the argument in the caller's parameter area
// (+32), as the original's stores it.
void SwprintfOne(uint32_t buffer, uint32_t format, uint64_t argument,
                 uint32_t frame) {
  guest::StoreU32(frame + 32, uint32_t(argument >> 32));
  guest::StoreU32(frame + 36, uint32_t(argument));
  int32_t count;
  Swprintf(buffer, format, frame + 32, frame - 96 - 144 - 2336, &count);
}


void FormatGrouped(int32_t value, uint32_t out, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8210A680 Strings::FormatGrouped");
  uint64_t v = uint64_t(int64_t(value));
  const bool negative = value < 0;
  if (negative) {
    v = 0 - v;
  }
  const uint32_t buffer = frame + 96;
  const int32_t magnitude = int32_t(uint32_t(v));
  const double thousands =
      Single(ToSingle(magnitude) * Lfs(G::thousandth_address()));
  if (thousands < Lfs(G::one_address())) {
    SwprintfOne(buffer, G::plain_format_address(), v, frame);
  } else {
    const int32_t rest = int32_t(uint32_t(v) - uint32_t(magnitude / 1000) * 1000);
    FormatGrouped(int32_t(uint32_t(v) - uint32_t(rest)) / 1000, out, frame - 640);
    SwprintfOne(buffer, G::grouped_format_address(), uint64_t(int64_t(rest)), frame);
  }
  c_runtime::Wcscat(out, buffer);
  if (negative) {
    SwprintfOne(buffer, G::minus_format_address(), out, frame);
    c_runtime::Wcscpy(out, buffer);
  }
}

}  // namespace

bool StringsFormatGrouped(int32_t value, uint32_t out, uint32_t frame) {
  if (value < 0) {
    // "-%s" of `out` after the digits: no 0xFFFF in it, and with the sign,
    // the digits (11 at most) and the terminator within the 256 units.
    const uint32_t length = c_runtime::Wcslen(out);
    if (length > 240) {
      return false;
    }
    for (uint32_t k = 0; k < length; k++) {
      if (guest::LoadU16(out + 2 * k) == 0xFFFF) {
        return false;
      }
    }
  }
  FormatGrouped(value, out, frame);
  return true;
}

namespace {

constexpr uint32_t kFormatFrame = 4800;
constexpr uint32_t kSwprintf = 0x8210A490;
constexpr uint32_t kFormatGroupedFunction = 0x8210A680;
constexpr uint32_t kFontNumber = 0x82108A70;

// swprintf of `format` with one argument from FormatV's frame (its
// parameter area at +32 written as the original's stores): natively, else
// its adapter through `calls`.
void FormatSwprintf(const FormatCalls& calls, uint32_t frame, uint32_t buffer,
                    uint32_t format, uint64_t argument, uint32_t lr) {
  guest::StoreU32(frame + 32, uint32_t(argument >> 32));
  guest::StoreU32(frame + 36, uint32_t(argument));
  int32_t count;
  if (!Swprintf(buffer, format, frame + 32, frame - 96 - 144 - 2336, &count)) {
    calls.call(calls.user, kSwprintf, buffer, format, argument, frame, lr);
  }
}

// memset(buffer, 0, 4096).
void ClearBuffer(uint32_t buffer) {
  for (uint32_t at = 0; at < 4096; at += 4) {
    guest::StoreU32(buffer + at, 0);
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

namespace {

// What differs between FormatV's loop and Format's: the frame's size, the
// return addresses of the guest calls (the swprintf pairs, FormatGrouped's,
// the font number's) and whether 0xFFD2 is a case.
struct FormatSite {
  uint32_t frame_bytes;
  uint32_t int_lr[2];
  uint32_t double_lr[2];
  uint32_t grouped_lr;
  bool font_number;
};
constexpr FormatSite kFormatV = {4800, {0x8210AA78, 0x8210AA88}, {0x8210AAD0, 0x8210AAE8},
                                 0x8210AB4C, true};
constexpr FormatSite kFormat = {4784, {0x8210ACC4, 0x8210ACD4}, {0x8210AD1C, 0x8210AD34},
                                0x8210AD90, false};

// The loop both share over `text`, from their frame at `frame`.
uint32_t FormatText(const FormatCalls& calls, uint32_t frame, uint32_t table,
                    uint32_t out, uint32_t text, uint32_t args, const FormatSite& site) {
  const uint32_t buffer = frame + 608;
  const uint32_t format = frame + 96;
  for (uint32_t c; (c = schema::FontText(text).unit()) != 0; text += 2) {
    uint32_t append = buffer;
    if (c == 0xFFD7) {
      const uint32_t slot = NextSlot(args);
      const uint32_t inner = guest::LoadU16(slot + 6);
      ClearBuffer(buffer);
      append = StringsFormatV(calls, frame, table, buffer, inner, args);
    } else if (c > 0xFFEB && c <= 0xFFF5) {
      const uint32_t slot = NextSlot(args);
      const uint32_t value = guest::LoadU32(slot + 4);
      FormatSwprintf(calls, frame, format, G::int_format_address(), (0xFFF5 - c) & 0xFFFF,
                     site.int_lr[0]);
      FormatSwprintf(calls, frame, buffer, format, value, site.int_lr[1]);
    } else if (c > 0xFFE1 && c <= 0xFFEB) {
      const uint32_t slot = NextSlot(args);
      const double value =
          std::bit_cast<double>((uint64_t(guest::LoadU32(slot)) << 32) |
                                guest::LoadU32(slot + 4));
      FormatSwprintf(calls, frame, format, G::float_format_address(), (0xFFEB - c) & 0xFFFF,
                     site.double_lr[0]);
      const double rounded = double(float(value));
      FormatSwprintf(calls, frame, buffer, format, std::bit_cast<uint64_t>(rounded),
                     site.double_lr[1]);
    } else if (c > 0xFFD7 && c <= 0xFFE1) {
      const uint32_t slot = NextSlot(args);
      const uint32_t value = guest::LoadU32(slot + 4);
      FormatSwprintf(calls, frame, format, G::zeroed_format_address(), (0xFFE1 - c) & 0xFFFF,
                     site.int_lr[0]);
      FormatSwprintf(calls, frame, buffer, format, value, site.int_lr[1]);
    } else if (c == 0xFFD3) {
      const uint32_t slot = NextSlot(args);
      ClearBuffer(buffer);
      const uint32_t value = guest::LoadU32(slot + 4);
      if (!StringsFormatGrouped(int32_t(value), buffer, frame - 640)) {
        calls.call(calls.user, kFormatGroupedFunction, table, value, buffer, frame,
                   site.grouped_lr);
      }
    } else if (c == 0xFFD2 && site.font_number) {
      const uint32_t slot = NextSlot(args);
      ClearBuffer(buffer);
      calls.call(calls.user, kFontNumber, G::font().address, buffer,
                 guest::LoadU32(slot + 4), frame, 0x8210AB84);
    } else {
      guest::StoreU16(frame + 80, uint16_t(c));
      guest::StoreU16(frame + 82, 0);
      append = frame + 80;
    }
    c_runtime::Wcscat(out, append);
  }
  return out;
}

}  // namespace

uint32_t StringsFormatV(const FormatCalls& calls, uint32_t stack, uint32_t table,
                        uint32_t out, uint32_t id, uint32_t args) {
  ITC_NATIVE_RUNNING("sub_8210A990 Strings::FormatV");
  const uint32_t frame = stack - kFormatV.frame_bytes;
  guest::StoreU32(frame, stack);  // the back chain, as stwu
  const uint32_t text = StringsGet(table, id & 0xFFFF).value_or(table);
  ClearBuffer(frame + 608);
  return FormatText(calls, frame, table, out, text, args, kFormatV);
}

uint32_t StringsFormat(const FormatCalls& calls, uint32_t stack, uint32_t table,
                       uint32_t out, uint32_t id, const uint64_t varargs[5]) {
  ITC_NATIVE_RUNNING("sub_8210ABC0 Strings::Format");
  // r6 to r10 into the caller's parameter area, the va_list from there.
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(stack + 40 + 8 * i, uint32_t(varargs[i] >> 32));
    guest::StoreU32(stack + 44 + 8 * i, uint32_t(varargs[i]));
  }
  const uint32_t frame = stack - kFormat.frame_bytes;
  guest::StoreU32(frame, stack);  // the back chain, as stwu
  const uint32_t text = StringsGet(table, id & 0xFFFF).value_or(table);
  ClearBuffer(frame + 608);
  const uint32_t args = stack + 40;
  guest::StoreU32(frame + 80, args);  // the va_list, as the original keeps it
  return FormatText(calls, frame, table, out, text, args, kFormat);
}

uint32_t StringsPageCount(uint32_t text) {
  ITC_NATIVE_RUNNING("sub_8210A5C0 Strings::PageCount");
  uint32_t count = 0;
  for (uint32_t c; (c = schema::FontText(text).unit()) != 0; text += 2) {
    if (c == kPageBreak) {
      count++;
    }
  }
  return count;
}

uint32_t StringsCopyPage(uint32_t out, uint32_t text, int32_t page, int32_t skip) {
  ITC_NATIVE_RUNNING("sub_8210A5E8 Strings::CopyPage");
  while (page > 0) {
    const uint32_t c = schema::FontText(text).unit();
    if (c == 0) {
      break;
    }
    if (c == kPageBreak) {
      page--;
    }
    text += 2;
  }
  for (uint32_t c; (c = schema::FontText(text).unit()) != 0; text += 2) {
    if (c == kPageBreak) {
      break;
    }
    if (skip <= 0) {
      // wcscat(out, {c, 0}).
      const uint32_t end = out + 2 * c_runtime::Wcslen(out);
      guest::StoreU16(end, uint16_t(c));
      guest::StoreU16(end + 2, 0);
    }
    if (schema::FontText(text).unit() == '\n') {
      skip--;
    }
  }
  return out;
}

void InitShaderEntries() {
  ITC_NATIVE_RUNNING("sub_825D9590 (the font's pixel shader entries)");
  // The stores in the original's order (each length after the next source).
  uint32_t lengths[4];
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t entry = G::shader_entries_address() + 12 * i;
    const uint32_t source = guest::LoadU32(G::pixel_shader_sources_address() + 4 * i);
    if (i > 0) {
      guest::StoreU32(entry - 4, 0);
    }
    guest::StoreU32(entry, source);
    if (i > 0) {
      guest::StoreU32(entry - 8, lengths[i - 1]);
    }
    uint32_t end = source;
    while (guest::LoadU8(end) != 0) {
      end++;
    }
    lengths[i] = end - source;
  }
  guest::StoreU32(G::shader_entries_address() + 44, 0);
  guest::StoreU32(G::shader_entries_address() + 40, lengths[3]);
}

void FontSetParams(uint32_t font_address, uint32_t params_address) {
  ITC_NATIVE_RUNNING("sub_82108368 Font::SetParams");
  const Font font(font_address);
  const FontDrawParams params(params_address);
  font.set_params(params);
  const double one = Lfd(G::one_double_address());
  const int32_t x = params.x();
  const double scale_x = double(params.scale_x()) - one;
  const int32_t origin_x = params.origin_x();
  font.set_start_x(StartCoordinate(scale_x, x, origin_x));
  const int32_t y = params.y();
  const double scale_y = double(params.scale_y()) - one;
  const int32_t origin_y = params.origin_y();
  font.set_start_y(StartCoordinate(scale_y, y, origin_y));
  font.set_letter_spacing(float(ToSingle(params.letter_spacing())));
  font.set_line_spacing(float(ToSingle(params.line_spacing())));
}

std::optional<uint32_t> StringsGet(uint32_t table, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210A800 Strings::Get");
  const uint32_t bounds = G::string_bounds_address();
  for (uint32_t i = 0; i < 17; i++) {
    if (int32_t(id) < int32_t(guest::LoadU32(bounds + 4 + 4 * i))) {
      const uint32_t bound = guest::LoadU32(bounds + 4 * i);
      return game_data::GetEntry(File(Strings(table), i),
                                 id - bound, false);
    }
  }
  return std::nullopt;
}

std::optional<uint32_t> ReleaseXprSlot(uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_82108860 XPR::Release (a slot)");
  const uint32_t xpr = guest::LoadU32(slot);
  if (xpr == 0) {
    return slot;
  }
  if (!memory::IsHeapBlock(xpr) || !models::XprDestruct(xpr)) {
    return std::nullopt;
  }
  memory::crt::OperatorDelete(xpr);
  guest::StoreU32(slot, 0);
  return xpr;
}

// --- The loads ---

FontLoadResult FontLoad(uint32_t font_address, uint32_t mask,
                        const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82109498 Font::Load");
  const Font font(font_address);
  FontLoadResult result{false, std::nullopt};
  font.set_pages_asked(mask | font.pages_asked());
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t file_slot = kPageFilesOffset + 4 * i;
    const uint32_t page_slot = kPagesOffset + 4 * i;
    const uint32_t id_slot = G::font_page_ids_address() + 4 * i;
    if ((mask & (1u << i)) == 0 || FontWord(font, file_slot) != 0) {
      continue;
    }
    // operator new's 28 bytes: the page in host memory (E5 step 5).
    font.store_at(page_slot, 4, FontPage::New().address);
    const uint32_t asset = guest::LoadU32(id_slot);
    // The page's file stays guest pages: its XPR's texture is read by the
    // renderer (E5 step 7).
    font.store_at(file_slot, 4, memory::AllocPages(archive::AssetSize(asset), 4096));
    FontPage(FontWord(font, page_slot)).set_loaded(0);
    result.last = load(guest::LoadU32(id_slot), FontWord(font, file_slot),
                       kFontPageLoaded, FontWord(font, page_slot));
  }
  return result;
}

LastLoad StringsLoad(uint32_t table_address, uint32_t mask,
                     const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_8210A8D0 Strings::Load");
  const Strings table(table_address);
  LastLoad last;
  table.set_files_asked(mask | table.files_asked());
  for (uint32_t i = 0; i < kStringFiles; i++) {
    if ((mask & (1u << i)) == 0) {
      continue;
    }
    const uint32_t flag = FlagSlot(table, i);
    const uint32_t id_slot = G::string_file_ids_address() + 4 * i;
    SetCount(table, i, Count(table, i) + 1);
    if (File(table, i) != 0) {
      continue;
    }
    schema::StringsLoadedFlag(flag).set_loaded(0);
    // VirtualAlloc's pages: the file in PC memory, a blob (E5 step 5).
    const uint32_t size = archive::AssetSize(guest::LoadU32(id_slot));
    const uint32_t buffer = size != 0 ? schema::StringFile::New(size).address : 0;
    SetFile(table, i, buffer);
    last = load(guest::LoadU32(id_slot), buffer, kStringsLoaded, flag);
  }
  return last;
}

int64_t TextLoad(const archive::LoadCall& load,
                 const FontFallback& font_fallback) {
  ITC_NATIVE_RUNNING("sub_8210ADF8 Text::Load");
  const uint32_t font = G::font().address;
  if (FontLoad(font, 15, load).handed_back) {
    font_fallback(font, 15);
  }
  const uint32_t table = G::strings().address;
  const LastLoad last = StringsLoad(table, 0x8001, load);
  return last ? *last : int64_t(table);
}

}  // namespace strings_and_font
