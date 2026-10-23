// The C runtime's wide printf into a string (vswprintf, _vswprintf_l and
// _woutput_l) as plain C++: see format.h.

#include "c_runtime/format.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"

namespace c_runtime {

namespace {

// _woutput_l's tables and the C runtime's data it reads.
constexpr uint32_t kStates = 0x820734D8;      // character classes, then the states
constexpr uint32_t kNarrowNull = 0x820734CC;  // [] = "(null)"
constexpr uint32_t kWideNull = 0x820734D0;    // [] = L"(null)"
constexpr uint32_t kLocale = 0x8266BC28;      // [] = the current locale (+172 MB_CUR_MAX)
constexpr uint32_t kFloatTable = 0x8266BA5C;  // the float conversions (+24 _cfltcvt_l)
constexpr uint32_t kCfltcvt = 0x825AEB20;     // _cfltcvt_l

// The flags (as the C runtime's output.c).
constexpr uint32_t kSign = 0x1, kSignSpace = 0x2, kLeft = 0x4, kLeadZero = 0x8, kLong = 0x10,
                   kShort = 0x20, kSigned = 0x40, kAlternate = 0x80, kNegative = 0x100,
                   kForceOctal = 0x200, kWide = 0x800, kLongLong = 0x1000, kI64 = 0x8000;

// _woutput_l on a string stream of INT_MAX bytes at `buffer`, its frame at
// `frame` (the original's r1: the text buffer at +128, the double at +112,
// the prefix at +88, mbtowc's characters at +84 and +86, all dead stack
// below the caller's). A dry run writes the frame but not the output, and
// says whether the real run needs nothing native code can't do.
class WideOutput {
 public:
  WideOutput(uint8_t* base, uint32_t buffer, uint32_t format, uint32_t args, uint32_t frame,
             bool dry)
      : base_(base), out_(buffer), format_(format), args_(args), frame_(frame), dry_(dry) {}

  // False (dry runs only) where the original must run. `*count` the
  // characters written, or -1.
  bool Run(int32_t* count);
  // The bytes left in the stream (its +4) after the run.
  int32_t room() const { return room_; }
  uint32_t pointer() const { return out_; }

 private:
  uint16_t U16(uint32_t a) { return guest::LoadU16(base_, a); }
  uint8_t U8(uint32_t a) { return guest::LoadU8(base_, a); }
  uint32_t U32(uint32_t a) { return guest::LoadU32(base_, a); }
  // The next argument's slot (8 bytes, aligned): its address.
  uint32_t NextSlot() {
    args_ = ((args_ + 7) & ~7u) + 8;
    return args_ - 8;
  }
  uint32_t MbCurMax() { return U32(U32(kLocale) + 172); }
  // write_char (sub_825B1368) to the string stream: _putwc_nolock's room
  // taken first (always enough for INT_MAX bytes), the unit stored, the
  // count -1 for 0xFFFF (its failure value) or moved on by one. A dry run
  // fails on 0xFFFF (the original's error paths read errno) and only counts.
  bool Write(uint32_t unit) {
    unit &= 0xFFFF;
    if (unit == 0xFFFF || room_ < 2) {
      return false;
    }
    room_ -= 2;
    if (!dry_) {
      guest::StoreU16(base_, out_, uint16_t(unit));
    }
    out_ += 2;
    count_ = count_ + 1;
    return true;
  }

  uint8_t* base_;
  uint32_t out_, format_, args_, frame_;
  bool dry_;
  int32_t room_ = 0x7FFFFFFF;
  int32_t count_ = 0;
};

bool WideOutput::Run(int32_t* result) {
  if (!dry_) {
    ITC_NATIVE_RUNNING("sub_825B13D0 _woutput_l");
  }
  const uint32_t f = frame_;
  uint32_t c = U16(format_);
  uint32_t flags = 0, width = 0, prefix_length = 0, hex_add = 0, state = 0;
  int32_t precision = 0;
  bool wide_text = false, no_output = false;
  uint32_t text_length = 0, text = kStates;
  if (c == 0) {
    *result = 0;
    return true;
  }
  for (;;) {
    format_ += 2;
    if (count_ < 0) {
      break;
    }
    const uint32_t cls = (c >= 32 && c <= 120) ? (U8(kStates + c - 32) & 0xF) : 0;
    state = uint32_t(int32_t(int8_t(U8(kStates + cls * 8 + state))) >> 4);
    bool output = false;  // the type's output stage (1C14)
    bool normal = false;  // the character itself
    switch (state) {
      case 0:
        normal = true;
        break;
      case 1:  // '%'
        no_output = false;
        width = 0;
        prefix_length = 0;
        flags = 0;
        precision = -1;
        wide_text = false;
        break;
      case 2:  // a flag
        if (c == ' ') flags |= kSignSpace;
        else if (c == '#') flags |= kAlternate;
        else if (c == '+') flags |= kSign;
        else if (c == '-') flags |= kLeft;
        else if (c == '0') flags |= kLeadZero;
        break;
      case 3:  // the width
        if (c == '*') {
          width = U32(NextSlot() + 4);
          if (int32_t(width) < 0) {
            flags |= kLeft;
            width = 0u - width;
          }
        } else {
          width = width * 10 + c - 48;
        }
        break;
      case 4:  // '.'
        precision = 0;
        break;
      case 5:  // the precision
        if (c == '*') {
          precision = int32_t(U32(NextSlot() + 4));
          if (precision < 0) {
            precision = -1;
          }
        } else {
          precision = int32_t(uint32_t(precision) * 10 + c - 48);
        }
        break;
      case 6:  // the size
        if (c == 'I') {
          const uint32_t next = U16(format_);
          if (next == '6' && U16(format_ + 2) == '4') {
            format_ += 4;
            flags |= kI64;
          } else if (next == '3' && U16(format_ + 2) == '2') {
            format_ += 4;
            flags &= ~kI64;
          } else if (next != 'd' && next != 'i' && next != 'o' && next != 'u' && next != 'x' &&
                     next != 'X') {
            state = 0;
            normal = true;
          }
        } else if (c == 'h') {
          flags |= kShort;
        } else if (c == 'l') {
          if (U16(format_) == 'l') {
            format_ += 2;
            flags |= kLongLong;
          } else {
            flags |= kLong;
          }
        } else if (c == 'w') {
          flags |= kWide;
        }
        break;
      case 7: {  // the type
        output = true;
        uint32_t type = c;
        uint32_t radix = 0;
        switch (c) {
          case 'C':
            if (!(flags & (kLong | kShort | kWide))) flags |= kShort;
            [[fallthrough]];
          case 'c': {
            text_length = 1;
            wide_text = true;
            const uint32_t value = U32(NextSlot() + 4) & 0xFFFF;
            guest::StoreU16(base_, f + 84, uint16_t(value));
            if (flags & kShort) {
              guest::StoreU8(base_, f + 86, uint8_t(value));
              guest::StoreU8(base_, f + 87, 0);
              if (Mbtowc(base_, f + 128, f + 86, MbCurMax()) < 0) {
                no_output = true;
              }
            } else {
              guest::StoreU16(base_, f + 128, uint16_t(value));
            }
            text = f + 128;
            break;
          }
          case 'S':
            if (!(flags & (kLong | kShort | kWide))) flags |= kShort;
            [[fallthrough]];
          case 's': {
            int32_t most = precision == -1 ? 0x7FFFFFFF : precision;
            text = U32(NextSlot() + 4);
            if (flags & kShort) {
              if (text == 0) text = U32(kNarrowNull);
              uint32_t p = text;
              text_length = 0;
              if (most > 0) {
                while (U8(p) != 0) {
                  if (IsLeadByte(base_, U8(p)) != 0) p++;
                  text_length++;
                  p++;
                  if (!(int32_t(text_length) < most)) break;
                }
              }
            } else {
              if (text == 0) text = U32(kWideNull);
              wide_text = true;
              uint32_t p = text;
              while (most != 0) {
                const uint32_t unit = U16(p);
                most--;
                if (unit == 0) break;
                p += 2;
              }
              text_length = uint32_t(int32_t(p - text) >> 1);
            }
            break;
          }
          case 'E': case 'G': case 'A':  // (the original's caps flag; not native)
            type = c + 32;
            [[fallthrough]];
          case 'e': case 'f': case 'g': case 'a': {
            flags |= kSigned;
            text = f + 128;
            if (precision < 0) {
              precision = 6;
            } else if (precision == 0 && type == 'g') {
              precision = 1;
            } else if (precision > 512) {
              precision = 512;
            }
            // Natively only %f (the conversion table's _cfltcvt_l giving
            // _cftof_l), with no buffer from the heap (precision 163 or
            // less) and no forced point ('#' with precision 0).
            if (type != 'f' || precision > 163 || ((flags & kAlternate) && precision == 0) ||
                U32(kFloatTable + 24) != kCfltcvt) {
              return false;
            }
            const uint32_t slot = NextSlot();
            const uint64_t bits = (uint64_t(U32(slot)) << 32) | U32(slot + 4);
            guest::StoreU32(base_, f + 112, uint32_t(bits >> 32));
            guest::StoreU32(base_, f + 116, uint32_t(bits));
            if (!Cftof(base_, bits, text, 512, precision)) {
              return false;  // (a real run follows a dry one that succeeded)
            }
            if (U8(text) == '-') {
              flags |= kNegative;
              text++;
            }
            uint32_t end = text;
            while (U8(end) != 0) end++;
            text_length = end - text;
            break;
          }
          case 'd': case 'i':
            flags |= kSigned;
            radix = 10;
            break;
          case 'u':
            radix = 10;
            break;
          case 'o':
            radix = 8;
            if (flags & kAlternate) flags |= kForceOctal;
            break;
          case 'p':
            precision = 8;
            [[fallthrough]];
          case 'X':
          case 'x':
            hex_add = c == 'x' ? 39 : 7;
            radix = 16;
            if (flags & kAlternate) {
              guest::StoreU16(base_, f + 90, uint16_t(hex_add + 81));
              guest::StoreU16(base_, f + 88, '0');
              prefix_length = 2;
            }
            break;
          case 'n': case 'Z':
            return false;  // %n (never in play) and counted strings: the original
          default:
            break;  // the last conversion's text again (the original's)
        }
        if (radix != 0) {
          uint64_t value;
          if (flags & (kI64 | kLongLong)) {
            const uint32_t slot = NextSlot();
            value = (uint64_t(U32(slot)) << 32) | U32(slot + 4);
          } else if (flags & kShort) {
            const uint32_t word = U32(NextSlot() + 4);
            value = (flags & kSigned) ? uint64_t(int64_t(int16_t(word))) : uint64_t(word & 0xFFFF);
          } else {
            const uint32_t word = U32(NextSlot() + 4);
            value = (flags & kSigned) ? uint64_t(int64_t(int32_t(word))) : uint64_t(word);
          }
          if ((flags & kSigned) && int64_t(value) < 0) {
            value = 0 - value;
            flags |= kNegative;
          }
          if (!(flags & (kI64 | kLongLong))) {
            value &= 0xFFFFFFFF;
          }
          if (precision < 0) {
            precision = 1;
          } else {
            flags &= ~kLeadZero;
            if (precision > 512) precision = 512;
          }
          if (value == 0) {
            prefix_length = 0;
          }
          uint32_t p = f + 639;
          for (;;) {
            const bool more = precision > 0;
            precision--;
            if (!more && value == 0) break;
            const uint64_t digit = value % radix;
            value /= radix;
            uint32_t ch = uint32_t(digit) + 48;
            if (int32_t(ch) > 57) ch += hex_add;
            guest::StoreU8(base_, p, uint8_t(ch));
            p--;
          }
          text_length = (f + 639) - p;
          text = p + 1;
          if ((flags & kForceOctal) && (text_length == 0 || U8(text) != '0')) {
            text--;
            guest::StoreU8(base_, text, '0');
            text_length++;
          }
        }
        break;
      }
      default:
        break;
    }
    if (normal) {
      wide_text = true;
      if (!Write(c)) return false;
    }
    if (output && !no_output) {
      if (flags & kSigned) {
        uint32_t sign = 0;
        if (flags & kNegative) sign = '-';
        else if (flags & kSign) sign = '+';
        else if (flags & kSignSpace) sign = ' ';
        if (sign != 0) {
          guest::StoreU16(base_, f + 88, uint16_t(sign));
          prefix_length = 1;
        }
      }
      const int32_t padding = int32_t(width - text_length - prefix_length);
      if (!(flags & (kLeft | kLeadZero))) {
        for (int32_t k = padding; k > 0; k--) {
          if (!Write(' ')) return false;
          if (count_ == -1) break;
        }
      }
      for (uint32_t k = 0; k < prefix_length; k++) {
        if (!Write(U16(f + 88 + 2 * k))) return false;
      }
      if ((flags & kLeadZero) && !(flags & kLeft)) {
        for (int32_t k = padding; k > 0; k--) {
          if (!Write('0')) return false;
          if (count_ == -1) break;
        }
      }
      if (!wide_text && int32_t(text_length) > 0) {
        uint32_t p = text;
        for (int32_t left = int32_t(text_length); left > 0;) {
          left--;
          const int32_t used = Mbtowc(base_, f + 84, p, MbCurMax());
          if (used <= 0) {
            count_ = -1;
            break;
          }
          if (!Write(U16(f + 84))) return false;
          p += uint32_t(used);
        }
      } else {
        for (uint32_t k = 0; int32_t(k) < int32_t(text_length); k++) {
          if (!Write(U16(text + 2 * k))) return false;
        }
      }
      if (count_ >= 0 && (flags & kLeft)) {
        for (int32_t k = padding; k > 0; k--) {
          if (!Write(' ')) return false;
          if (count_ == -1) break;
        }
      }
    }
    c = U16(format_);
    if (c == 0) {
      break;
    }
  }
  *result = count_;
  return true;
}

}  // namespace

bool Vswprintf(uint8_t* base, uint32_t buffer, uint32_t format, uint32_t args, uint32_t frame,
               int32_t* count) {
  if (buffer == 0 || format == 0) {
    return false;  // EINVAL through _invalid_parameter: the original
  }
  {
    WideOutput dry(base, buffer, format, args, frame, true);
    int32_t ignored;
    if (!dry.Run(&ignored) || dry.room() < 2) {
      return false;
    }
  }
  ITC_NATIVE_RUNNING("sub_825A9980 _vswprintf_l");
  WideOutput output(base, buffer, format, args, frame, false);
  output.Run(count);
  // _vswprintf_l's terminator: two bytes through the stream's putc.
  guest::StoreU8(base, output.pointer(), 0);
  guest::StoreU8(base, output.pointer() + 1, 0);
  return true;
}

}  // namespace c_runtime
