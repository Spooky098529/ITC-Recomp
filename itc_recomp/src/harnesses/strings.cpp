// The string functions (src/hooks/strings.cpp) against the recompiled ones:
// MultiByteToWideChar, WideCharToMultiByte and swprintf.

#include "harness.h"

#include "base/log.h"

namespace harness {

void Strings(Context& h) {
  uint8_t* base = h.base;
  const uint32_t data = h.data;
  Random& r = h.random;

  // The conversions: random text (early terminators, characters above 255
  // for the wide one), lengths (-1, short, negative), rooms (0 to measure,
  // too small, negative) and code pages (0, 1252, UTF-8).
  for (int wide = 0; wide < 2; wide++) {
    Results results(wide ? "WideCharToMultiByte (sub_82296720)" : "MultiByteToWideChar (sub_82296648)");
    for (int i = 0; i < 3000; i++) {
      uint32_t chars = r.Below(40);
      std::vector<uint16_t> text(chars);
      for (auto& ch : text) {
        ch = uint16_t(wide ? (r.Below(3) ? 1 + r.Below(255) : 1 + r.Below(0xFFFE)) : 1 + r.Below(255));
      }
      if (r.Below(4) == 0 && chars) {
        text[r.Below(chars)] = 0;
      }
      int32_t length = r.Below(3) == 0 ? -1 : int32_t(r.Below(chars + 2)) - (r.Below(10) == 0 ? 3 : 0);
      int32_t room = r.Below(4) == 0 ? 0 : int32_t(r.Below(chars + 4)) - (r.Below(10) == 0 ? 2 : 0);
      uint32_t code_page = r.Below(10) == 0 ? 65001 : (r.Below(2) ? 0 : 1252);
      uint32_t used = r.Below(2) ? data + 400 : 0;
      Both(h, results,
           [&] {
             for (uint32_t k = 0; k < chars; k++) {
               if (wide) {
                 guest::StoreU16(base, data + 2 * k, text[k]);
               } else {
                 guest::StoreU8(base, data + k, uint8_t(text[k]));
               }
             }
             if (wide) {
               guest::StoreU16(base, data + 2 * chars, 0);
             } else {
               guest::StoreU8(base, data + chars, 0);
             }
           },
           [&](PPCContext& t) {
             t.r3.u64 = code_page;
             t.r4.u64 = 0;
             t.r5.u64 = data;
             t.r6.s64 = length;
             t.r7.u64 = data + 128;
             t.r8.s64 = room;
             t.r9.u64 = 0;
             t.r10.u64 = used;
           },
           wide ? __imp__sub_82296720 : __imp__sub_82296648, wide ? sub_82296720 : sub_82296648,
           data + 128, 300,
           fmt::format("code page {} length {} room {} characters {}", code_page, length, room, chars));
    }
    results.Report();
  }

  // swprintf: the game's one format (0x82009218), then made-up ones with
  // no conversions but "%%".
  {
    Results results("swprintf (sub_82296870)");
    for (int i = 0; i < 200; i++) {
      const uint32_t format = i == 0 ? 0x82009218 : data + 256;
      std::vector<uint16_t> text;
      const uint32_t chars = r.Below(60);
      for (uint32_t k = 0; k < chars; k++) {
        uint16_t ch = uint16_t(1 + r.Below(0xFFFE));
        if (ch == '%') {
          ch = 'x';
        }
        if (r.Below(20) == 0 && k + 1 < chars) {
          text.push_back('%');
          k++;
          ch = '%';
        }
        text.push_back(ch);
      }
      Both(h, results,
           [&] {
             if (i == 0) {
               return;
             }
             for (uint32_t k = 0; k < text.size(); k++) {
               guest::StoreU16(base, data + 256 + 2 * k, text[k]);
             }
             guest::StoreU16(base, data + 256 + 2 * uint32_t(text.size()), 0);
           },
           [&](PPCContext& t) {
             t.r3.u64 = data;
             t.r4.u64 = format;
           },
           __imp__sub_82296870, sub_82296870, data, 250,
           i == 0 ? std::string("the game's format") : fmt::format("{} characters", text.size()));
    }
    results.Report();
  }
}

}  // namespace harness
