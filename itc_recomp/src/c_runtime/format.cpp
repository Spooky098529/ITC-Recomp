// The C runtime's long double conversions and printf's character output
// natively (format.h), step for step as the translated code.

#include "c_runtime/format.h"

#include "guest.h"

namespace c_runtime {

LongDouble12 DoubleToLd12(uint64_t bits) {
  const uint32_t high = uint32_t(bits >> 32);
  const uint32_t low = uint32_t(bits);
  const uint16_t sign = uint16_t((high >> 16) & 0x8000);
  const uint32_t exponent = (high >> 20) & 0x7FF;
  const uint32_t mantissa = high & 0xFFFFF;
  uint32_t biased;
  uint32_t leading = 0x80000000u;  // the explicit leading bit
  if (exponent == 0) {
    if (mantissa == 0 && low == 0) {
      return {sign, 0, 0};
    }
    biased = 15361;  // a denormal: normalized below
    leading = 0;
  } else if (exponent == 2047) {
    biased = 32767;
  } else {
    biased = exponent + 15360;
  }
  uint32_t mantissa_high = (mantissa << 11) | (low >> 21) | leading;
  uint32_t mantissa_low = low << 11;
  while ((mantissa_high & 0x80000000u) == 0) {
    biased = (biased + 0xFFFF) & 0xFFFF;
    mantissa_high = (mantissa_high << 1) | (mantissa_low >> 31);
    mantissa_low <<= 1;
  }
  return {uint16_t(sign | biased), mantissa_high, mantissa_low};
}

namespace {

// The double's format descriptor at 0x8266C514 (read from guest memory):
// max_exp, min_exp, precision, exponent width, size in bits, bias.
constexpr int32_t kMaxExp = 1024;
constexpr int32_t kMinExp = -1023;
constexpr int32_t kPrecision = 53;
constexpr int32_t kExponentWidth = 11;
constexpr int32_t kBias = 1023;

// slw: a shift of 32 or more gives 0.
uint32_t Slw(uint32_t value, uint32_t count) {
  return count >= 32 ? 0 : value << count;
}

// _RoundMan: keeps `precision` bits of the 96-bit mantissa, adding one at
// the last kept bit when the next is set and any after it is (_IncMan);
// returns the carry out of the top word.
bool RoundMan(uint32_t man[3], int32_t precision) {
  const int32_t round_word = precision / 32;
  const int32_t shift = 31 - (precision - round_word * 32);
  bool carry = false;
  if ((man[round_word] & (1u << shift)) != 0) {
    bool tail = (man[round_word] & ~(0xFFFFFFFFu << shift)) != 0;
    for (int32_t i = round_word + 1; !tail && i < 3; i++) {
      tail = man[i] != 0;
    }
    if (tail) {
      const int32_t last = precision - 1;
      const int32_t word = last / 32;
      const uint32_t bit = 1u << (31 - (last - word * 32));
      const uint32_t old = man[word];
      const uint32_t sum = old + bit;
      carry = sum < old || sum < bit;
      man[word] = sum;
      for (int32_t i = word - 1; i >= 0 && carry; i--) {
        const uint32_t value = man[i] + 1;
        carry = value < man[i] || value < 1;
        man[i] = value;
      }
    }
  }
  man[round_word] &= 0xFFFFFFFFu << shift;
  for (int32_t i = round_word + 1; i < 3; i++) {
    man[i] = 0;
  }
  return carry;
}

// _ShrMan: the 96-bit mantissa shifted right by `count` (0 or more): bits
// within each word first, then whole words.
void ShrMan(uint32_t man[3], int32_t count) {
  const int32_t words = count / 32;
  const uint32_t bits = uint32_t(count - words * 32);
  const uint32_t low_mask = ~(0xFFFFFFFFu << bits);
  uint32_t carry = 0;
  for (int32_t i = 0; i < 3; i++) {
    const uint32_t value = man[i];
    man[i] = (value >> bits) | carry;
    carry = Slw(value & low_mask, 32 - bits);
  }
  for (int32_t i = 2; i >= 0; i--) {
    man[i] = i >= words ? man[i - words] : 0;
  }
}

}  // namespace

uint64_t Ld12ToDouble(const LongDouble12& value, uint16_t extra, int32_t* status) {
  uint32_t man[3] = {value.mantissa_high, value.mantissa_low, uint32_t(extra) << 16};
  const bool negative = (value.sign_exponent & 0x8000) != 0;
  int32_t exponent = int32_t(value.sign_exponent & 0x7FFF) - 16383;
  int32_t biased = 0;
  if (exponent == -16383) {
    // A zero, or a denormal long double: flushed to zero.
    *status = 0;
    if (man[0] != 0 || man[1] != 0 || man[2] != 0) {
      man[0] = man[1] = man[2] = 0;
      *status = 2;
    }
  } else {
    const uint32_t saved[3] = {man[0], man[1], man[2]};
    const int32_t unrounded = exponent;
    if (RoundMan(man, kPrecision)) {
      exponent++;
    }
    if (exponent < kMinExp - kPrecision) {
      man[0] = man[1] = man[2] = 0;  // underflow to zero
      *status = 2;
    } else if (exponent <= kMinExp) {
      // A denormal: the unrounded mantissa shifted by the unrounded
      // exponent's distance, rounded again, and room made for the exponent.
      man[0] = saved[0];
      man[1] = saved[1];
      man[2] = saved[2];
      ShrMan(man, kMinExp - unrounded);
      RoundMan(man, kPrecision);
      ShrMan(man, kExponentWidth + 1);
      *status = 2;
    } else if (exponent >= kMaxExp) {
      man[0] = 0x80000000u;  // overflow: infinity
      man[1] = man[2] = 0;
      ShrMan(man, kExponentWidth);
      biased = kBias + kMaxExp;
      *status = 1;
    } else {
      man[0] &= 0x7FFFFFFFu;  // the leading bit left out
      ShrMan(man, kExponentWidth);
      biased = kBias + exponent;
      *status = 0;
    }
  }
  const uint32_t high = Slw(uint32_t(biased), 31 - kExponentWidth) |
                        (negative ? 0x80000000u : 0) | man[0];
  return (uint64_t(high) << 32) | man[1];
}

int32_t WriteChar(uint8_t* base, uint32_t character, uint32_t stream, uint32_t count) {
  if ((guest::LoadU32(base, stream + 12) & 0x40) != 0 && guest::LoadU32(base, stream + 8) == 0) {
    guest::StoreU32(base, count, guest::LoadU32(base, count) + 1);
    return 0;
  }
  const uint32_t room = guest::LoadU32(base, stream + 4) - 1;
  if (int32_t(room) < 0) {
    return -1;  // _flsbuf
  }
  guest::StoreU32(base, stream + 4, room);
  guest::StoreU8(base, guest::LoadU32(base, stream), uint8_t(character));
  guest::StoreU32(base, stream, guest::LoadU32(base, stream) + 1);
  guest::StoreU32(base, count, guest::LoadU32(base, count) + 1);
  return 1;
}

namespace {

// Whether [a, a + a_bytes) and [b, b + b_bytes) share a guest address.
bool Overlap(uint32_t a, uint32_t a_bytes, uint32_t b, uint32_t b_bytes) {
  return uint64_t(a) < uint64_t(b) + b_bytes && uint64_t(b) < uint64_t(a) + a_bytes;
}

// A string stream with no buffer: write_char and its wide one only count.
bool CountsOnly(uint8_t* base, uint32_t stream) {
  return (guest::LoadU32(base, stream + 12) & 0x40) != 0 && guest::LoadU32(base, stream + 8) == 0;
}

}  // namespace

bool WriteMultiChar(uint8_t* base, uint32_t string, int32_t length, uint32_t stream,
                    uint32_t count, int32_t* last) {
  *last = -1;
  if (CountsOnly(base, stream)) {
    guest::StoreU32(base, count, guest::LoadU32(base, count) + uint32_t(length));
    return true;
  }
  if (length <= 0) {
    return true;
  }
  const uint32_t room = guest::LoadU32(base, stream + 4);
  const uint32_t pointer = guest::LoadU32(base, stream);
  const uint32_t counted = guest::LoadU32(base, count);
  if (Overlap(pointer, uint32_t(length), stream, 16) ||
      Overlap(pointer, uint32_t(length), count, 4) || Overlap(count, 4, stream, 16)) {
    return false;
  }
  for (uint32_t k = 0; k < uint32_t(length); k++) {
    if (int32_t(room - 1 - k) < 0 || counted + k + 1 == 0xFFFFFFFFu) {
      return false;
    }
  }
  for (uint32_t k = 0; k < uint32_t(length); k++) {
    *last = guest::LoadU8(base, string + k);
    WriteChar(base, uint32_t(*last), stream, count);
  }
  return true;
}

bool PutwcString(uint8_t* base, uint16_t unit, uint32_t stream) {
  if (stream == kStreams + 32 || stream == kStreams + 64) {
    return false;  // stdout and stderr: the console
  }
  if ((guest::LoadU32(base, stream + 12) & 0x40) == 0) {
    return false;  // a file stream
  }
  const uint32_t room = guest::LoadU32(base, stream + 4) - 2;
  if (int32_t(room) < 0) {
    return false;  // _flswbuf
  }
  guest::StoreU32(base, stream + 4, room);
  guest::StoreU16(base, guest::LoadU32(base, stream), unit);
  guest::StoreU32(base, stream, guest::LoadU32(base, stream) + 2);
  return true;
}

bool WriteWideChar(uint8_t* base, uint16_t unit, uint32_t stream, uint32_t count) {
  if (CountsOnly(base, stream)) {
    guest::StoreU32(base, count, guest::LoadU32(base, count) + 1);
    return true;
  }
  if (!PutwcString(base, unit, stream)) {
    return false;
  }
  guest::StoreU32(base, count, unit == 0xFFFF ? 0xFFFFFFFFu : guest::LoadU32(base, count) + 1);
  return true;
}

}  // namespace c_runtime
