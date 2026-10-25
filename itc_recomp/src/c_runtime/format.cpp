// The C runtime's long double conversions and printf's character output
// natively (format.h), step for step as the translated code.

#include "c_runtime/format.h"

#include "base/log.h"

#include "c_runtime/parse.h"
#include "c_runtime/strings.h"

#include "guest.h"

#include <algorithm>
#include <climits>

namespace c_runtime {

LongDouble12 DoubleToLd12(uint64_t bits) {
  ITC_NATIVE_RUNNING("sub_825B6378 ___dtold");
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
  ITC_NATIVE_RUNNING("sub_825B69B8 _ld12tod");
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
  ITC_NATIVE_RUNNING("sub_825B7A80 _putwc_nolock");
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
  ITC_NATIVE_RUNNING("sub_825B1368 wide write_char");
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

bool GuestText::Reach(uint64_t at) {
  constexpr uint64_t kLimit = 1u << 20;
  if (!ok_ || at >= kLimit || address_ + at > 0xFFFFFFFFull) {
    ok_ = false;
    return false;
  }
  if (at >= bytes_.size()) {
    const size_t size = std::max<size_t>(at + 1, bytes_.size() * 2);
    bytes_.resize(size);
    known_.resize(size);
    written_.resize(size);
  }
  return true;
}

uint8_t GuestText::Get(uint64_t at) {
  if (!Reach(at)) {
    return 0;
  }
  if (!known_[at]) {
    bytes_[at] = guest::LoadU8(base_, uint32_t(address_ + at));
    known_[at] = 1;
  }
  return bytes_[at];
}

void GuestText::Set(uint64_t at, uint8_t value) {
  if (!Reach(at)) {
    return;
  }
  bytes_[at] = value;
  known_[at] = 1;
  written_[at] = 1;
  end_ = std::max(end_, at + 1);
}

uint32_t GuestText::Length(uint64_t at) {
  uint32_t length = 0;
  while (ok_ && Get(at + length) != 0) {
    length++;
  }
  return length;
}

void GuestText::Move(uint64_t to, uint64_t from, uint32_t count) {
  if (count == 0 || !Reach(std::max(to, from) + count - 1)) {
    return;
  }
  if (to > from) {
    for (uint64_t i = count; i-- > 0;) {
      Set(to + i, Get(from + i));
    }
  } else if (to < from) {
    for (uint64_t i = 0; i < count; i++) {
      Set(to + i, Get(from + i));
    }
  }
}

void GuestText::Fill(uint64_t at, uint8_t value, uint32_t count) {
  if (count == 0 || !Reach(at + count - 1)) {
    return;
  }
  for (uint64_t i = 0; i < count; i++) {
    Set(at + i, value);
  }
}

bool GuestText::Commit() {
  if (!ok_) {
    return false;
  }
  if (end_ == 0) {
    return true;
  }
  if (!InOneView(address_, uint32_t(end_)) || address_ + end_ > 0xA0000000ull) {
    return false;
  }
  for (uint64_t i = 0; i < end_; i++) {
    if (written_[i]) {
      guest::StoreU8(base_, uint32_t(address_ + i), bytes_[i]);
    }
  }
  return true;
}

int32_t FpToStr(GuestText& text, uint64_t at, uint32_t size, int32_t digits, int32_t* exponent,
                const std::vector<uint8_t>& mantissa) {
  ITC_NATIVE_RUNNING("sub_825B61E8 _fptostr");
  const uint32_t whole = digits > 0 ? uint32_t(digits) : 0;
  if (size == 0 || size <= whole + 1) {
    return -1;  // EINVAL, ERANGE: _errno and _invalid_parameter
  }
  const auto digit = [&](size_t i) { return i < mantissa.size() ? mantissa[i] : uint8_t(0); };
  text.Set(at, '0');
  size_t next = 0;
  uint64_t p = at + 1;
  int32_t left = digits;
  while (left > 0 && text.ok()) {
    uint8_t c = digit(next);
    if (c != 0) {
      next++;
    } else {
      c = '0';
    }
    text.Set(p++, c);
    left--;
  }
  text.Set(p, 0);
  if (left >= 0 && int8_t(digit(next)) >= '5') {  // the original's extsb: signed
    p--;
    while (text.ok() && text.Get(p) == '9') {
      text.Set(p, '0');
      p--;
    }
    text.Set(p, uint8_t(text.Get(p) + 1));
  }
  if (text.Get(at) == '1') {
    *exponent = int32_t(uint32_t(*exponent) + 1);
  } else {
    text.Move(at, at + 1, text.Length(at + 1) + 1);
  }
  return 0;
}

bool Cftof2(GuestText& text, int32_t precision, int32_t sign, int32_t exponent, bool g_format,
            uint8_t point) {
  ITC_NATIVE_RUNNING("sub_825AE708 _cftof2_l");
  const int32_t before = int32_t(uint32_t(exponent) - 1);
  if (g_format && before == precision) {
    const int64_t at = int64_t(int32_t(uint32_t(sign == kMinus) + uint32_t(before)));
    if (at < 0) {
      return false;
    }
    text.Set(uint64_t(at), '0');
    text.Set(uint64_t(at) + 1, 0);
  }
  uint64_t p = 0;
  if (sign == kMinus) {
    text.Set(0, uint8_t(sign));
    p = 1;
  }
  uint64_t q;
  if (exponent <= 0) {
    text.Move(p + 1, p, text.Length(p) + 1);
    text.Set(p, '0');
    q = p + 1;
  } else {
    q = p + uint64_t(exponent);
  }
  if (precision > 0) {
    text.Move(q + 1, q, text.Length(q) + 1);
    text.Set(q, point);
    if (exponent < 0) {
      if (exponent == INT32_MIN) {
        return false;
      }
      int32_t zeros = -exponent;
      if (!g_format && precision < zeros) {
        zeros = precision;
      }
      if (zeros != 0) {
        text.Move(q + 1 + uint64_t(zeros), q + 1, text.Length(q + 1) + 1);
      }
      text.Fill(q + 1, '0', uint32_t(zeros));
    }
  }
  return text.ok();
}

uint32_t DecimalPointAddress(uint8_t* base, uint32_t* words) {
  constexpr uint32_t kLocaleData = 0x8266C024;
  const uint32_t first = guest::LoadU32(base, kLocaleData);
  const uint32_t point = guest::LoadU32(base, first);
  if (words) {
    words[0] = kLocaleData;
    words[1] = first;
    words[2] = point;
  }
  return point;
}

bool Cftof(uint8_t* base, uint64_t bits, uint32_t buffer, uint32_t size, int32_t precision) {
  ITC_NATIVE_RUNNING("sub_825AE8E0 _cftof_l");
  // _fltout2: $I10_OUTPUT's digits copied by strcpy_s into 22 bytes.
  FloatOut out;
  if (!I10Output(base, DoubleToLd12(bits), 17, 0, &out)) {
    return false;
  }
  uint32_t length = 0;
  while (length < 22 && out.bytes[4 + length] != 0) {
    length++;
  }
  if (length >= 22) {
    return false;
  }
  const int32_t sign = int32_t(int8_t(out.bytes[2]));
  int32_t exponent = int32_t(int16_t(out.bytes[0] << 8 | out.bytes[1]));
  const std::vector<uint8_t> mantissa(out.bytes + 4, out.bytes + 4 + length);
  if (buffer == 0 || size == 0) {
    return false;  // EINVAL
  }
  const uint32_t minus = sign == kMinus ? 1 : 0;
  const uint32_t room = size == 0xFFFFFFFFu ? 0xFFFFFFFFu : size - minus;
  GuestText text(base, buffer);
  if (FpToStr(text, minus, room, int32_t(uint32_t(exponent) + uint32_t(precision)), &exponent,
              mantissa) != 0) {
    return false;
  }
  uint32_t words[3];
  const uint32_t point = DecimalPointAddress(base, words);
  if (!Cftof2(text, precision, sign, exponent, false, guest::LoadU8(base, point))) {
    return false;
  }
  const uint32_t end = uint32_t(text.WrittenEnd());
  if (MayShare(words[0], 4, buffer, end) || MayShare(words[1], 4, buffer, end) ||
      MayShare(point, 1, buffer, end)) {
    return false;
  }
  return text.Commit();
}

}  // namespace c_runtime
