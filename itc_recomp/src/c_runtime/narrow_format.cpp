// The C runtime's printf into a string (sprintf, _vsnprintf, _vsnprintf_l
// and _output_l) as plain C++: see format.h. A step-by-step translation of
// _output_l (sub_825AF130) for a string stream, as wide_format.cpp is of
// _woutput_l.

#include "c_runtime/format.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"

#include <algorithm>
#include <vector>

namespace c_runtime {

namespace {

// _output_l's tables and the C runtime's data it reads.
constexpr uint32_t kStates = 0x820734D8;      // character classes, then the states
constexpr uint32_t kNarrowNull = 0x820734CC;  // [] = "(null)"
constexpr uint32_t kWideNull = 0x820734D0;    // [] = L"(null)"
constexpr uint32_t kFloatTable = 0x8266BA5C;  // the float conversions (+24 _cfltcvt_l)
constexpr uint32_t kCfltcvt = 0x825AEB20;     // _cfltcvt_l
// _output_l's frame: 1,328 bytes.
constexpr uint32_t kFrameBytes = 1328;

// The flags (as the C runtime's output.c).
constexpr uint32_t kSign = 0x1, kSignSpace = 0x2, kLeft = 0x4, kLeadZero = 0x8, kLong = 0x10,
                   kShort = 0x20, kSigned = 0x40, kAlternate = 0x80, kNegative = 0x100,
                   kForceOctal = 0x200, kWide = 0x800, kLongLong = 0x1000, kI64 = 0x8000;

// A guest range [begin, end).
struct Range {
  uint64_t begin, end;
};

bool Overlaps(const Range& a, const Range& b) {
  return a.begin < b.end && b.begin < a.end;
}

// _output_l on a string stream (flags 66: +0 the pointer, +4 the room, +8
// the buffer) whose pointer and buffer are `buffer` and room `room`, its
// frame at `frame` (the original's r1: the text buffer at +144, the double
// at +128, the prefix at +88, the numbers' digits down from +655, all dead
// stack below the caller's). A dry run writes the frame but not the output,
// notes the guest bytes it reads, and says whether the real run needs
// nothing native code can't do.
class NarrowOutput {
 public:
  NarrowOutput(uint8_t* base, uint32_t buffer, int32_t room, uint32_t format, uint32_t args,
               uint32_t frame, bool dry)
      : base_(base),
        out_(buffer),
        counts_only_(buffer == 0),
        room_(room),
        format_(format),
        args_(args),
        args_start_(args),
        frame_(frame),
        dry_(dry) {}

  // False (dry runs only) where the original must run. `*count` the
  // characters written (as _output_l's r3, a word).
  bool Run(int32_t* count);
  int32_t room() const { return room_; }
  uint32_t pointer() const { return out_; }
  // The guest bytes the run read outside its frame: the format, the
  // arguments, the strings.
  const std::vector<Range>& reads() const { return reads_; }

 private:
  uint8_t U8(uint32_t a) { return guest::LoadU8(base_, a); }
  uint16_t U16(uint32_t a) { return guest::LoadU16(base_, a); }
  uint32_t U32(uint32_t a) { return guest::LoadU32(base_, a); }
  // The next argument's slot (8 bytes, aligned): its address.
  uint32_t NextSlot() {
    args_ = ((args_ + 7) & ~7u) + 8;
    return args_ - 8;
  }
  void Read(uint64_t begin, uint64_t end) {
    if (dry_ && end > begin) {
      reads_.push_back({begin, end});
    }
  }
  // write_char (sub_825AF000) to the string stream: with no buffer only
  // counted; else the room taken, the byte stored and the pointer and the
  // count moved on. A dry run fails where the room runs out (_flsbuf).
  bool Write(uint32_t character) {
    if (counts_only_) {
      count_ = int32_t(uint32_t(count_) + 1);
      return true;
    }
    if (room_ - 1 < 0) {
      return false;
    }
    room_--;
    if (!dry_) {
      guest::StoreU8(base_, out_, uint8_t(character));
    }
    out_++;
    count_ = int32_t(uint32_t(count_) + 1);
    return true;
  }
  // write_multi_char (sub_825AF098): with no buffer the length added (even
  // a negative one); else write_char for each byte while the length lasts.
  bool WriteMulti(uint32_t string, int32_t length) {
    if (counts_only_) {
      count_ = int32_t(uint32_t(count_) + uint32_t(length));
      return true;
    }
    for (; length > 0; length--) {
      if (!Write(U8(string))) {
        return false;
      }
      string++;
    }
    return true;
  }

  uint8_t* base_;
  uint32_t out_;
  bool counts_only_;
  int32_t room_;
  uint32_t format_, args_, args_start_, frame_;
  bool dry_;
  int32_t count_ = 0;
  std::vector<Range> reads_;
};

bool NarrowOutput::Run(int32_t* result) {
  if (!dry_) {
    ITC_NATIVE_RUNNING("sub_825AF130 _output_l");
  }
  const uint32_t f = frame_;
  const uint32_t format_start = format_;
  uint32_t c = U8(format_);
  uint32_t flags = 0, width = 0, prefix_length = 0, hex_add = 0, state = 0;
  int32_t precision = 0;
  bool wide_text = false, no_output = false;
  uint32_t text_length = 0, text = 0;
  if (c != 0) {
    for (;;) {
      format_++;
      if (count_ < 0) {
        break;
      }
      const int32_t sc = int32_t(int8_t(c));
      const uint32_t cls = (sc >= 32 && sc <= 120) ? (U8(kStates + uint32_t(sc) - 32) & 0xF) : 0;
      state = uint32_t(int32_t(int8_t(U8(kStates + cls * 8 + state))) >> 4);
      bool output = false;  // the type's output stage (5AFA88)
      bool normal = false;  // the character itself (5AF538)
      switch (state) {
        case 0:
          normal = true;
          break;
        case 1:  // '%'
          flags = 0;
          precision = -1;
          wide_text = false;
          width = 0;
          prefix_length = 0;
          no_output = false;
          break;
        case 2:  // a flag
          if (sc == ' ') flags |= kSignSpace;
          else if (sc == '#') flags |= kAlternate;
          else if (sc == '+') flags |= kSign;
          else if (sc == '-') flags |= kLeft;
          else if (sc == '0') flags |= kLeadZero;
          break;
        case 3:  // the width
          if (sc == '*') {
            width = U32(NextSlot() + 4);
            if (int32_t(width) < 0) {
              flags |= kLeft;
              width = 0u - width;
            }
          } else {
            width = width * 10 + uint32_t(sc) - 48;
          }
          break;
        case 4:  // '.'
          precision = 0;
          break;
        case 5:  // the precision
          if (sc == '*') {
            precision = int32_t(U32(NextSlot() + 4));
            if (precision < 0) {
              precision = -1;
            }
          } else {
            precision = int32_t(uint32_t(precision) * 10 + uint32_t(sc) - 48);
          }
          break;
        case 6:  // the size
          if (sc == 'I') {
            const int32_t next = int8_t(U8(format_));
            if (next == '6' && U8(format_ + 1) == '4') {
              format_ += 2;
              flags |= kI64;
            } else if (next == '3' && U8(format_ + 1) == '2') {
              format_ += 2;
              flags &= ~kI64;
            } else if (next != 'd' && next != 'i' && next != 'o' && next != 'u' && next != 'x' &&
                       next != 'X') {
              state = 0;
              normal = true;
            }
          } else if (sc == 'h') {
            flags |= kShort;
          } else if (sc == 'l') {
            if (U8(format_) == 'l') {
              format_++;
              flags |= kLongLong;
            } else {
              flags |= kLong;
            }
          } else if (sc == 'w') {
            flags |= kWide;
          }
          break;
        case 7: {  // the type
          output = true;
          uint32_t radix = 0;
          int32_t type = sc;
          switch (sc) {
            case 'C':
              if (!(flags & (kLong | kShort | kWide))) flags |= kWide;
              [[fallthrough]];
            case 'c': {
              const uint32_t slot = NextSlot();
              if (flags & (kLong | kWide)) {
                return false;  // wctomb_s (sub_825B6890, never in play): the original
              }
              text_length = 1;
              guest::StoreU32(base_, f + 84, 1);
              guest::StoreU8(base_, f + 144, uint8_t(U32(slot + 4)));
              text = f + 144;
              break;
            }
            case 'Z': {
              const uint32_t string = U32(NextSlot() + 4);
              if (string != 0) {
                Read(string, uint64_t(string) + 8);
              }
              if (string == 0 || U32(string + 4) == 0) {
                text = U32(kNarrowNull);
                uint32_t p = text;
                while (U8(p) != 0) p++;
                Read(text, uint64_t(p) + 1);
                text_length = p - text;
              } else {
                text = U32(string + 4);
                const int32_t bytes = int16_t(U16(string));
                if (flags & kWide) {
                  wide_text = true;
                  text_length = uint32_t(bytes / 2);
                } else {
                  wide_text = false;
                  text_length = uint32_t(bytes);
                }
              }
              break;
            }
            case 'S':
              if (!(flags & (kLong | kShort | kWide))) flags |= kWide;
              [[fallthrough]];
            case 's': {
              int32_t most = precision == -1 ? 0x7FFFFFFF : precision;
              text = U32(NextSlot() + 4);
              if (flags & (kLong | kWide)) {
                if (text == 0) text = U32(kWideNull);
                wide_text = true;
                uint32_t p = text;
                for (; most != 0; most--) {
                  const uint32_t unit = U16(p);
                  if (unit == 0) {
                    Read(p, uint64_t(p) + 2);
                    break;
                  }
                  p += 2;
                }
                Read(text, p);
                text_length = uint32_t(int32_t(p - text) >> 1);
              } else {
                if (text == 0) text = U32(kNarrowNull);
                uint32_t p = text;
                for (; most != 0; most--) {
                  if (U8(p) == 0) {
                    Read(p, uint64_t(p) + 1);
                    break;
                  }
                  p++;
                }
                Read(text, p);
                text_length = p - text;
              }
              break;
            }
            case 'A': case 'E': case 'G':  // the caps flag (the original's; not native)
              type = int8_t(uint8_t(sc + 32));
              [[fallthrough]];
            case 'a': case 'e': case 'f': case 'g': {
              flags |= kSigned;
              text = f + 144;
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
              guest::StoreU32(base_, f + 128, uint32_t(bits >> 32));
              guest::StoreU32(base_, f + 132, uint32_t(bits));
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
              hex_add = sc == 'x' ? 39 : 7;
              radix = 16;
              if (flags & kAlternate) {
                guest::StoreU8(base_, f + 89, uint8_t(hex_add + 81));
                guest::StoreU8(base_, f + 88, '0');
                prefix_length = 2;
              }
              break;
            case 'n':
              return false;  // %n (_get_printf_count_output, never in play): the original
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
            uint32_t p = f + 655;
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
            text_length = (f + 655) - p;
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
        wide_text = false;
        if (IsLeadByte(base_, c & 0xFF) != 0) {
          if (!Write(c)) return false;
          c = U8(format_);
          format_++;
          if (c == 0) {
            return false;  // a lead byte at the end: EINVAL, the original
          }
        }
        if (!Write(c)) return false;
      }
      if (output && !no_output) {
        if (flags & kSigned) {
          uint32_t sign = 0;
          if (flags & kNegative) sign = '-';
          else if (flags & kSign) sign = '+';
          else if (flags & kSignSpace) sign = ' ';
          if (sign != 0) {
            guest::StoreU8(base_, f + 88, uint8_t(sign));
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
        if (!WriteMulti(f + 88, int32_t(prefix_length))) return false;
        if ((flags & kLeadZero) && !(flags & kLeft)) {
          for (int32_t k = padding; k > 0; k--) {
            if (!Write('0')) return false;
            if (count_ == -1) break;
          }
        }
        if (wide_text && int32_t(text_length) > 0) {
          return false;  // wctomb_s (sub_825B6890, never in play): the original
        }
        if (text < f || text >= f + kFrameBytes) {
          Read(text, uint64_t(text) + std::max(int32_t(text_length), 0));
        }
        if (!WriteMulti(text, int32_t(text_length))) return false;
        if (count_ >= 0 && (flags & kLeft)) {
          for (int32_t k = padding; k > 0; k--) {
            if (!Write(' ')) return false;
            if (count_ == -1) break;
          }
        }
      }
      c = U8(format_);
      if (c == 0) {
        break;
      }
    }
  }
  Read(format_start, uint64_t(format_) + 1);
  Read(args_start_, args_);
  *result = count_;
  return true;
}

// _output_l over a string stream, then the terminator: a dry run first, the
// real one only where it needs nothing the original alone does, and where
// what it writes (with the terminator) reads none of its own inputs or its
// frame (the original would read its own writes there).
bool Format(uint8_t* base, uint32_t buffer, int32_t room, uint32_t format, uint32_t args,
            uint32_t frame, int32_t* count) {
  {
    NarrowOutput dry(base, buffer, room, format, args, frame, true);
    int32_t ignored;
    if (!dry.Run(&ignored)) {
      return false;
    }
    if (buffer != 0) {
      if (dry.room() - 1 < 0) {
        return false;  // no room for the terminator: _flsbuf, the original
      }
      const Range written{buffer, uint64_t(dry.pointer()) + 1};
      if (Overlaps(written, {frame, uint64_t(frame) + kFrameBytes})) {
        return false;
      }
      for (const Range& read : dry.reads()) {
        if (Overlaps(written, read)) {
          return false;
        }
      }
    }
  }
  NarrowOutput output(base, buffer, room, format, args, frame, false);
  output.Run(count);
  if (buffer != 0) {
    guest::StoreU8(base, output.pointer(), 0);
  }
  return true;
}

}  // namespace

bool Sprintf(uint8_t* base, uint32_t buffer, uint32_t format, uint32_t args, uint32_t frame,
             int32_t* count) {
  if (buffer == 0 || format == 0) {
    return false;  // EINVAL through _invalid_parameter: the original
  }
  if (!Format(base, buffer, 0x7FFFFFFF, format, args, frame, count)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_825A93D0 sprintf");
  return true;
}

bool Vsnprintf(uint8_t* base, uint32_t buffer, uint32_t size, uint32_t format, uint32_t args,
               uint32_t frame, int32_t* count) {
  if (format == 0 || (size != 0 && buffer == 0)) {
    return false;  // EINVAL through _invalid_parameter: the original
  }
  const int32_t room = size > 0x7FFFFFFFu ? 0x7FFFFFFF : int32_t(size);
  if (!Format(base, buffer, room, format, args, frame, count)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_825AB010 _vsnprintf");
  ITC_NATIVE_RUNNING("sub_825AAF18 _vsnprintf_l");
  return true;
}

}  // namespace c_runtime
