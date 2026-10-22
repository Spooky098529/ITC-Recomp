// The C runtime's number parsing under atof natively (parse.h), step for step
// as the translated code: the same state machine, the same 96-bit integer
// steps, the same rounding.

#include "c_runtime/parse.h"

#include "base/log.h"

#include "guest.h"

namespace c_runtime {

namespace {

using u128 = unsigned __int128;
constexpr u128 kMask96 = (u128(1) << 96) - 1;

// The globals the parsing reads (docs/known-data.md).
constexpr uint32_t kDecimalPoint = 0x8266C024;  // a pointer to a pointer to the character
constexpr uint32_t kPow10Positive = 0x8266C4E8;  // 12-byte powers of ten, less 96
constexpr uint32_t kPow10Negative = 0x8266C648;

// Big-endian bytes standing in for a function's stack frame: offsets are
// from its r1 (in __strgtold12_l 80 the product, 96 the number, 112 a
// rounded-down table entry, the multiplication's last carry at 78; in
// $I10_OUTPUT 88 the number, 104 and 120 the products, 136 a tenth, 152 a
// table entry or the number's copy).
struct Frame {
  uint8_t b[200] = {};

  uint16_t U16(uint32_t o) const { return uint16_t(b[o] << 8 | b[o + 1]); }
  uint32_t U32(uint32_t o) const { return uint32_t(U16(o)) << 16 | U16(o + 2); }
  void SetU16(uint32_t o, uint32_t v) {
    b[o] = uint8_t(v >> 8);
    b[o + 1] = uint8_t(v);
  }
  void SetU32(uint32_t o, uint32_t v) {
    SetU16(o, v >> 16);
    SetU16(o + 2, v);
  }
};

// The 96 bits at `o` shifted by one.
void Shl96(Frame& f, uint32_t o) {
  const uint32_t w0 = f.U32(o), w1 = f.U32(o + 4), w2 = f.U32(o + 8);
  f.SetU32(o, w0 << 1 | w1 >> 31);
  f.SetU32(o + 4, w1 << 1 | w2 >> 31);
  f.SetU32(o + 8, w2 << 1);
}

void Shr96(Frame& f, uint32_t o) {
  const uint32_t w0 = f.U32(o), w1 = f.U32(o + 4), w2 = f.U32(o + 8);
  f.SetU32(o + 8, w2 >> 1 | w1 << 31);
  f.SetU32(o + 4, w1 >> 1 | w0 << 31);
  f.SetU32(o, w0 >> 1);
}

// __ld12mul, inlined in __strgtold12_l and $I10_OUTPUT: the number at `x`
// times the one at `y`, the product built at `r`, the result back at `x`.
void Ld12Mul(Frame& f, uint32_t x, uint32_t r, uint32_t y) {
  f.SetU32(r + 8, 0);
  f.SetU32(r + 4, 0);
  f.SetU32(r, 0);
  uint32_t sticky = 0;
  const uint32_t yw = f.U16(y), xw = f.U16(x);
  const uint32_t sign = (xw ^ yw) & 0x8000;
  const uint32_t ex = xw & 0x7FFF, ey = yw & 0x7FFF;
  uint32_t sum = (ex + ey) & 0xFFFF;
  auto infinity = [&] {
    f.SetU32(x, sign != 0 ? 0xFFFF8000u : 0x7FFF8000u);
    f.SetU32(x + 8, 0);
    f.SetU32(x + 4, 0);
  };
  auto zero = [&] {
    f.SetU32(x, 0);
    f.SetU32(x + 8, 0);
    f.SetU32(x + 4, 0);
  };
  if (ex >= 0x7FFF || ey >= 0x7FFF || sum > 49149) {
    infinity();
    return;
  }
  if (sum <= 16319) {
    zero();
    return;
  }
  if (ex == 0) {
    sum = (sum + 1) & 0xFFFF;
    if ((f.U32(x) & 0x7FFFFFFF) == 0 && f.U32(x + 4) == 0 && f.U32(x + 8) == 0) {
      f.SetU16(x, 0);
      return;
    }
  }
  if (ey == 0) {
    sum = (sum + 1) & 0xFFFF;
    if ((f.U32(y) & 0x7FFFFFFF) == 0 && f.U32(y + 4) == 0 && f.U32(y + 8) == 0) {
      zero();
      return;
    }
  }
  // The mantissas' halfwords multiplied into the product, column by column.
  uint32_t column = r + 6;
  for (int32_t count = 5, i = 0; count > 0; count--, column -= 2, i++) {
    uint32_t from_x = x + 10 - 2 * i, from_y = y + 2;
    for (int32_t n = count; n > 0; n--, from_x -= 2, from_y += 2) {
      const uint32_t product = uint32_t(f.U16(from_x)) * f.U16(from_y);
      const uint32_t old = f.U32(column + 2);
      const uint32_t total = old + product;
      f.SetU32(column + 2, total);
      if (total < old || total < product) {
        f.SetU16(column, f.U16(column) + 1);
      }
    }
  }
  uint32_t e = (sum + 65536 - 16382) & 0xFFFF;
  if (int16_t(e) > 0) {
    while ((f.U32(r) & 0x80000000u) == 0) {
      Shl96(f, r);
      e = (e + 0xFFFF) & 0xFFFF;
      if (int16_t(e) <= 0) {
        break;
      }
    }
  }
  if (int16_t(e) <= 0) {
    e = (e + 0xFFFF) & 0xFFFF;
    if (int16_t(e) < 0) {
      do {
        if ((f.U16(r + 10) & 1) != 0) {
          sticky++;
        }
        Shr96(f, r);
        e = (e + 1) & 0xFFFF;
      } while (int16_t(e) < 0);
      if (sticky != 0) {
        f.SetU16(r + 10, f.U16(r + 10) | 1);
      }
    }
  }
  // Rounded to nearest, ties to even, on the top 80 bits.
  if (f.U16(r + 10) > 0x8000 || (f.U32(r + 8) & 0x1FFFF) == 0x18000) {
    const uint32_t low = uint32_t(f.U16(r + 6)) << 16 | f.U16(r + 8);
    if (low != 0xFFFFFFFFu) {
      f.SetU16(r + 8, low + 1);
      f.SetU16(r + 6, (low + 1) >> 16);
    } else {
      f.SetU16(r + 8, 0);
      f.SetU16(r + 6, 0);
      const uint32_t high = uint32_t(f.U16(r + 2)) << 16 | f.U16(r + 4);
      if (high != 0xFFFFFFFFu) {
        f.SetU16(r + 4, high + 1);
        f.SetU16(r + 2, (high + 1) >> 16);
      } else {
        f.SetU16(r + 4, 0);
        f.SetU16(r + 2, 0);
        const uint32_t top = f.U16(r);
        if (top != 0xFFFF) {
          f.SetU16(r, top + 1);
        } else {
          f.SetU16(r, 0x8000);
          e = (e + 1) & 0xFFFF;
        }
      }
    }
  }
  if (e >= 0x7FFF) {
    infinity();
    return;
  }
  f.SetU16(x + 10, f.U16(r + 8));
  f.SetU16(x + 8, f.U16(r + 6));
  f.SetU16(x, sign | e);
  f.SetU16(x + 6, f.U16(r + 4));
  f.SetU16(x + 4, f.U16(r + 2));
  f.SetU16(x + 2, f.U16(r));
}

LongDouble12Full FromFrame(const Frame& f, uint32_t o) {
  return {{f.U16(o), f.U32(o + 2), f.U32(o + 6)}, f.U16(o + 10)};
}

// __multtenpow12, inlined: the number at `x` times 10^`power` (|power| at
// most 5200 or so), by the tables three bits of the power at a time, each
// entry copied to `y` (less one in +6 to +9 when +10 is 0x8000 or more),
// the products built at `r`.
void MultTenPow(uint8_t* base, Frame& f, uint32_t x, uint32_t r, uint32_t y, int32_t power) {
  uint32_t table = kPow10Positive;
  int32_t n = power;
  if (n < 0) {
    table = kPow10Negative;
    n = -n;
  }
  while (n != 0) {
    table += 84;
    const uint32_t index = uint32_t(n) & 7;
    n >>= 3;
    if (index == 0) {
      continue;
    }
    const uint32_t entry = table + index * 12;
    for (uint32_t i = 0; i < 12; i++) {
      f.b[y + i] = guest::LoadU8(base, entry + i);
    }
    if (f.U16(y + 10) >= 0x8000) {
      const uint32_t m = (uint32_t(f.U16(y + 6)) << 16 | f.U16(y + 8)) - 1;
      f.SetU16(y + 8, m);
      f.SetU16(y + 6, m >> 16);
    }
    Ld12Mul(f, x, r, y);
  }
}

}  // namespace

bool Mtold12(const uint8_t* digits, uint32_t count, LongDouble12Full* out) {
  ITC_NATIVE_RUNNING("sub_825B7F10 __mtold12");
  u128 v = 0;
  for (uint32_t i = 0; i < count; i++) {
    v = (v * 10 + uint32_t(int32_t(int8_t(digits[i])))) & kMask96;
  }
  uint32_t e = 16462;
  while ((v >> 64) == 0) {
    if (v == 0) {
      return false;  // the original shifts forever
    }
    v = (v << 16) & kMask96;
    e = (e + 65536 - 16) & 0xFFFF;
  }
  while (((v >> 64) & 0x8000) == 0) {
    v = (v << 1) & kMask96;
    if (v == 0) {
      return false;
    }
    e = (e + 65535) & 0xFFFF;
  }
  const uint32_t w0 = uint32_t(v >> 64), w1 = uint32_t(v >> 32), w2 = uint32_t(v);
  out->value = {uint16_t(e), (w0 & 0xFFFF) << 16 | w1 >> 16, (w1 & 0xFFFF) << 16 | w2 >> 16};
  out->extra = uint16_t(w2);
  return true;
}

bool Strgtold12(uint8_t* base, uint32_t string, uint32_t mult12, uint32_t scale, uint32_t decpt,
                uint32_t implicit_e, Strgtold12Result* result) {
  ITC_NATIVE_RUNNING("sub_825B36B8 __strgtold12_l");
  uint8_t buffer[25];
  uint32_t length = 0;     // digits kept (r6)
  uint32_t man_sign = 0;   // r18
  int32_t exp_sign = 1;    // r27
  bool found_digit = false, found_point = false, found_exponent = false;
  uint32_t exponent = 0;   // r29
  uint32_t man_exp = 0;    // r31: digits dropped or after the point
  uint32_t state = 0;
  uint32_t reset = string;  // r5: where parsing ends when what follows fails
  uint32_t p = string;
  for (;;) {
    const int8_t c = int8_t(guest::LoadU8(base, p));
    if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
      break;
    }
    p++;
  }
  const uint32_t point_pointer = guest::LoadU32(base, kDecimalPoint);
  auto point = [&] {
    return int32_t(int8_t(guest::LoadU8(base, guest::LoadU32(base, point_pointer))));
  };
  auto nonzero_digit = [](int32_t c) { return c >= '1' && c <= '9'; };
  // After digits: d, D, e or E starts the exponent, anything else ends.
  // Returns the next state, or 0 when parsing ends (p moved back).
  auto exponent_letter = [&](int32_t c) -> uint32_t {
    if (c <= 'C') {
      p--;
      return 0;
    }
    if (c <= 'E' || uint32_t(c - 100) <= 1) {
      return 6;
    }
    p--;
    return 0;
  };
  bool done = false;
  while (!done) {
    uint8_t byte = guest::LoadU8(base, p);
    p++;
    int32_t c = int8_t(byte);
    switch (state) {
      case 0:
        if (nonzero_digit(c)) {
          state = 3;
          p--;
        } else if (c == point()) {
          state = 5;
        } else if (c == '+') {
          state = 2;
          man_sign = 0;
        } else if (c == '-') {
          state = 2;
          man_sign = 0x8000;
        } else if (c == '0') {
          state = 1;
        } else {
          p--;
          done = true;
        }
        break;
      case 1:
        found_digit = true;
        if (nonzero_digit(c)) {
          state = 3;
          p--;
        } else if (c == point()) {
          state = 4;
        } else if (c == '+' || c == '-') {
          p--;
          state = 11;
        } else if (c == '0') {
          state = 1;
        } else {
          state = exponent_letter(c);
          done = state == 0;
        }
        break;
      case 2:
        if (nonzero_digit(c)) {
          state = 3;
          p--;
        } else if (c == point()) {
          state = 5;
        } else if (c == '0') {
          state = 1;
        } else {
          p = reset;
          done = true;
        }
        break;
      case 3:
      case 4: {
        found_digit = true;
        const bool after_point = state == 4;
        if (after_point) {
          found_point = true;
          if (length == 0 && c == '0') {
            do {
              byte = guest::LoadU8(base, p);
              man_exp--;
              p++;
            } while (byte == '0');
            c = int8_t(byte);
          }
        }
        while (c >= '0' && c <= '9') {
          if (length < 25) {
            buffer[length++] = uint8_t(c - '0');
            if (after_point) {
              man_exp--;
            }
          } else if (!after_point) {
            man_exp++;
          }
          byte = guest::LoadU8(base, p);
          p++;
          c = int8_t(byte);
        }
        if (!after_point && c == point()) {
          state = 4;
        } else if (c == '+' || c == '-') {
          p--;
          state = 11;
        } else {
          state = exponent_letter(c);
          done = state == 0;
        }
        break;
      }
      case 5:
        found_point = true;
        if (c >= '0' && c <= '9') {
          state = 4;
          p--;
        } else {
          p = reset;
          done = true;
        }
        break;
      case 6:
      case 7:
        if (state == 6) {
          reset = p - 2;
        }
        if (nonzero_digit(c)) {
          state = 9;
          p--;
        } else if (state == 6 && c == '+') {
          state = 7;
        } else if (state == 6 && c == '-') {
          state = 7;
          exp_sign = -1;
        } else if (c == '0') {
          state = 8;
        } else {
          p = reset;
          done = true;
        }
        break;
      case 8:
        found_exponent = true;
        if (c == '0') {
          do {
            byte = guest::LoadU8(base, p);
            p++;
          } while (byte == '0');
          c = int8_t(byte);
        }
        if (nonzero_digit(c)) {
          state = 9;
          p--;
        } else {
          p--;
          done = true;
        }
        break;
      case 9: {
        found_exponent = true;
        int32_t value = 0;
        while (c >= '0' && c <= '9') {
          value = value * 10 + c - '0';
          if (value > 5200) {
            value = 5201;
            break;
          }
          byte = guest::LoadU8(base, p);
          p++;
          c = int8_t(byte);
        }
        exponent = uint32_t(value);
        while (c >= '0' && c <= '9') {
          byte = guest::LoadU8(base, p);
          p++;
          c = int8_t(byte);
        }
        p--;
        done = true;
        break;
      }
      case 11:
        if (implicit_e == 0) {
          p--;
          done = true;
          break;
        }
        reset = p - 1;
        if (c == '+') {
          state = 7;
        } else if (c == '-') {
          state = 7;
          exp_sign = -1;
        } else {
          p = reset;
          done = true;
        }
        break;
      default:  // 10
        done = true;
        break;
    }
  }

  result->end = p;
  auto fill = [&](uint32_t sign_exponent, uint32_t high, uint32_t low, uint32_t flags) {
    result->value = {{uint16_t(sign_exponent | man_sign), high, low}, 0};
    result->flags = flags;
  };
  if (!found_digit) {
    fill(0, 0, 0, 4);
    return true;
  }
  if (length > 24) {
    if (int8_t(buffer[23]) >= 5) {
      buffer[23]++;
    }
    length = 24;
    man_exp++;
  }
  if (length == 0) {
    fill(0, 0, 0, 0);
    return true;
  }
  while (buffer[length - 1] == 0) {  // trailing zeros
    length--;
    man_exp++;
    if (length == 0) {
      return false;  // the original reads below its buffer
    }
  }
  Frame f;
  LongDouble12Full number;
  if (!Mtold12(buffer, length, &number)) {
    return false;
  }
  f.SetU16(96, number.value.sign_exponent);
  f.SetU32(98, number.value.mantissa_high);
  f.SetU32(102, number.value.mantissa_low);
  f.SetU16(106, number.extra);
  if (exp_sign < 0) {
    exponent = 0 - exponent;
  }
  uint32_t total = man_exp + exponent;
  if (!found_exponent) {
    total += scale;
  }
  if (!found_point) {
    total -= decpt;
  }
  if (int32_t(total) > 5200) {
    fill(0x7FFF, 0x80000000u, 0, 2);
    return true;
  }
  if (int32_t(total) < -5200) {
    fill(0, 0, 0, 1);
    return true;
  }
  if (total != 0) {
    if (mult12 == 0) {
      f.SetU16(106, 0);
    }
    MultTenPow(base, f, 96, 80, 112, int32_t(total));
  }
  result->value = FromFrame(f, 96);
  result->value.value.sign_exponent = uint16_t(result->value.value.sign_exponent | man_sign);
  result->flags = 0;
  return true;
}

bool Fltin2(uint8_t* base, uint32_t string, FltinResult* result) {
  ITC_NATIVE_RUNNING("sub_825AFDC8 _fltin2");
  Strgtold12Result parsed;
  if (!Strgtold12(base, string, 0, 0, 0, 0, &parsed)) {
    return false;
  }
  uint32_t flags = 0;
  uint64_t bits = 0;
  if ((parsed.flags & 4) != 0) {
    flags = 512;
  } else {
    int32_t status;
    bits = Ld12ToDouble(parsed.value.value, parsed.value.extra, &status);
    if ((parsed.flags & 2) != 0 || status == 1) {
      flags = 128;
    }
    if ((parsed.flags & 1) != 0 || status == 2) {
      flags |= 256;
    }
  }
  result->flags = flags;
  result->used = parsed.end - string;
  result->bits = bits;
  return true;
}

bool AtofL(uint8_t* base, uint32_t string, uint64_t* bits) {
  ITC_NATIVE_RUNNING("sub_825AB0C0 _atof_l");
  if (string == 0) {
    return false;  // EINVAL through _errno
  }
  constexpr uint32_t kCurrentLocalePointer = 0x8266BC28;
  const uint32_t locale = guest::LoadU32(base, kCurrentLocalePointer);
  for (;;) {
    if (int32_t(guest::LoadU32(base, locale + 172)) > 1) {
      return false;  // multibyte: sub_825AFD98
    }
    const uint32_t table = guest::LoadU32(base, locale + 200);
    const uint32_t c = guest::LoadU8(base, string);
    if ((guest::LoadU16(base, table + 2 * c) & 8) == 0) {
      break;
    }
    string++;
  }
  FltinResult parsed;
  if (!Fltin2(base, string, &parsed)) {
    return false;
  }
  *bits = parsed.bits;
  return true;
}

namespace {

// The strings $I10_OUTPUT copies for the special values (constant data).
constexpr uint32_t kQNan = 0x82074490;  // "1#QNAN"
constexpr uint32_t kInf = 0x82074498;   // "1#INF"
constexpr uint32_t kInd = 0x820744A0;   // "1#IND"
constexpr uint32_t kSNan = 0x820744A8;  // "1#SNAN"

}  // namespace

bool I10Output(uint8_t* base, const LongDouble12& value, int32_t digits, uint32_t flags,
               FloatOut* out) {
  ITC_NATIVE_RUNNING("sub_825B8B48 $I10_OUTPUT");
  for (bool& w : out->written) {
    w = false;
  }
  auto put = [&](uint32_t offset, uint8_t byte) {
    out->bytes[offset] = byte;
    out->written[offset] = true;
  };
  auto put16 = [&](uint32_t offset, uint32_t v) {
    put(offset, uint8_t(v >> 8));
    put(offset + 1, uint8_t(v));
  };
  const uint32_t expn = value.sign_exponent & 0x7FFF;
  const uint32_t sign = value.sign_exponent & 0x8000;
  const uint32_t manhi = value.mantissa_high, manlo = value.mantissa_low;
  const uint8_t sign_char = sign != 0 ? '-' : ' ';
  put(2, sign_char);
  auto zero = [&] {  // "0", exponent 0
    put16(0, 0);
    put(2, sign == 0x8000 ? '-' : ' ');
    put(3, 1);
    put(5, 0);
    put(4, '0');
    out->result = 1;
  };
  if (expn == 0 && manhi == 0 && manlo == 0) {
    zero();
    return true;
  }
  if (expn == 0x7FFF) {
    put16(0, 1);
    uint32_t text;
    uint8_t length;
    if ((manhi != 0x80000000u || manlo != 0) && (manhi & 0x40000000u) == 0) {
      text = kSNan;
      length = 6;
    } else if (sign != 0 && manhi == 0xC0000000u) {
      text = manlo != 0 ? kQNan : kInd;
      length = manlo != 0 ? 6 : 5;
    } else if (manhi == 0x80000000u && manlo == 0) {
      text = kInf;
      length = 5;
    } else {
      text = kQNan;
      length = 6;
    }
    // strcpy_s into the 22 bytes at +4 (the strings fit).
    for (uint32_t i = 0;; i++) {
      const uint8_t c = guest::LoadU8(base, text + i);
      put(4 + i, c);
      if (c == 0) {
        break;
      }
      if (i >= 21) {
        return false;  // too long: strcpy_s's error path
      }
    }
    put(3, length);
    out->result = 0;
    return true;
  }

  Frame f;
  constexpr uint32_t x = 88;
  f.SetU16(x, expn);
  f.SetU32(x + 2, manhi);
  f.SetU32(x + 6, manlo);
  f.SetU16(x + 10, 0);
  // A tenth (0x3FFB CCCC...CC), for the estimate's correction.
  f.b[136] = 0x3F;
  f.b[137] = 0xFB;
  for (uint32_t i = 138; i < 148; i++) {
    f.b[i] = 0xCC;
  }
  const uint32_t estimate = ((manhi >> 24) * 2 + (expn >> 8)) * 77 + expn * 19728 - 0x134312F4u;
  int32_t ir = int16_t(int32_t(estimate) >> 16);
  MultTenPow(base, f, x, 104, 152, -ir);
  if (f.U16(x) >= 0x3FFF) {
    ir = int16_t(ir + 1);
    Ld12Mul(f, x, 120, 136);
  }
  put16(0, uint32_t(ir));
  if ((flags & 1) != 0) {
    digits += ir;
    if (digits <= 0) {
      zero();
      return true;
    }
  }
  if (digits > 21) {
    digits = 21;
  }
  // The mantissa as a fraction below 1: its exponent cleared, the bits put
  // 8 below the top (shifted right more for a number below a half).
  const int32_t unbiased = int32_t(f.U16(x)) - 16382;
  f.SetU16(x, 0);
  u128 v = (u128(f.U32(x)) << 64 | u128(f.U32(x + 4)) << 32 | f.U32(x + 8));
  v = (v << 8) & kMask96;
  if (unbiased < 0) {
    const uint32_t shift = uint32_t(-unbiased) & 0xFF;
    for (uint32_t i = 0; i < shift; i++) {
      v >>= 1;
    }
  }
  if (digits + 1 <= 0) {
    return false;  // the original reads +3 as a digit then
  }
  // One digit more than asked: times ten, the top byte out.
  uint32_t p = 4;
  for (int32_t k = digits + 1; k > 0; k--) {
    v = (v * 10) & kMask96;
    put(p++, uint8_t((v >> 88) + '0'));
    v &= (u128(1) << 88) - 1;
  }
  // Rounded on the last digit, the digits before it carried into.
  uint32_t q = p - 2;
  if (int8_t(out->bytes[p - 1]) >= '5') {
    while (q >= 4 && out->bytes[q] == '9') {
      put(q, '0');
      q--;
    }
    if (q < 4) {
      put16(0, uint32_t(uint16_t(out->bytes[0] << 8 | out->bytes[1]) + 1));
      q++;
    }
    put(q, uint8_t(out->bytes[q] + 1));
  } else {
    while (q >= 4 && out->bytes[q] == '0') {
      q--;
    }
    if (q < 4) {
      put16(0, 0);
      put(2, sign == 0x8000 ? '-' : ' ');
      put(5, 0);
      put(4, '0');
      put(3, 1);
      out->result = 1;
      return true;
    }
  }
  const int8_t length = int8_t(q - 3);
  put(3, uint8_t(length));
  put(uint32_t(int32_t(length) + 4), 0);
  out->result = 1;
  return true;
}

}  // namespace c_runtime
