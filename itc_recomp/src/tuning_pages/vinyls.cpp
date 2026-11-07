// The vinyls' records between the screen and the selected garage car
// (docs/functions/tuning-pages.md): see tuning_pages.h. Single-precision
// steps as the originals' (each product rounded to a float); needs
// flush-to-zero off, as the originals' lfs leaves it.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "strings_and_font/steps.h"
#include "tuning_screen/tuning_screen.h"

#include <bit>

namespace tuning_pages {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// lfs: a float made a double.
double Lfs(uint8_t* base, uint32_t address) {
  return double(std::bit_cast<float>(Load(base, address)));
}
// stfs: a double rounded to a float.
void Stfs(uint8_t* base, uint32_t address, double value) {
  Store(base, address, std::bit_cast<uint32_t>(float(value)));
}
// fmuls: the product rounded to a float.
double Fmuls(double a, double b) { return double(float(a * b)); }
// An int made a float (std, lfd, fcfid, frsp): the doubleword at `at`.
double IntToFloat(uint8_t* base, uint32_t at, int32_t value) {
  const uint64_t word = uint64_t(int64_t(value));
  Store(base, at, uint32_t(word >> 32));
  Store(base, at + 4, uint32_t(word));
  return double(float(double(int64_t(word))));
}

constexpr uint32_t kMinusOne = 0x82001BE0;     // probably -1.0
constexpr uint32_t kZero = 0x82000A34;         // 0.0
constexpr uint32_t kStoreScale = 0x82002948;   // StoreVinyls' scale for +16
constexpr uint32_t kLoadScale = 0x8200294C;    // LoadVinyls'
constexpr uint32_t kMemcpy = 0x825A7410;

}  // namespace

void StoreVinyls(uint8_t* base, uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82203F20 TuningScreen::StoreVinyls");
  const uint32_t s = uint32_t(screen);
  const uint32_t c = tuning_screen::SelectedGarageCar(base, s) + 212;
  Store(base, sp - 16, 0);
  Store(base, sp - 12, 1);
  Store(base, sp - 8, 2);
  const double scale = Lfs(base, kStoreScale);
  const double minus = Lfs(base, kMinusOne);
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t n = Load(base, sp - 16 + 4 * k);
    const uint32_t from = uint32_t(uint64_t(n) * 24 + screen);
    const uint32_t r = c + 96 + 32 * k;  // the first group's record
    const uint32_t r2 = r + 96;          // the second's
    Store(base, r + 24, Load(base, (n + 7792) * 4 + s));
    Stfs(base, r + 8, IntToFloat(base, sp - 48, int32_t(Load(base, from + 30880))));
    Stfs(base, r + 12, IntToFloat(base, sp - 40, int32_t(Load(base, from + 30884))));
    Stfs(base, r + 16, Fmuls(Lfs(base, (n + 1287) * 24 + s), scale));
    const int32_t flag = int32_t(Load(base, s + 30896 + 24 * k));
    double angle = Lfs(base, from + 30892);
    if (flag != 0) {
      angle = Fmuls(angle, minus);
    }
    Stfs(base, r + 0, angle);
    Stfs(base, r + 4, Lfs(base, from + 30892));
    Store(base, r2 + 24, Load(base, (n + 7795) * 4 + s));
    Stfs(base, r2 + 8, IntToFloat(base, sp - 32, int32_t(Load(base, from + 30952))));
    Stfs(base, r2 + 12, IntToFloat(base, sp - 24, int32_t(Load(base, from + 30956))));
    Stfs(base, r2 + 16, Fmuls(Lfs(base, (n + 1290) * 24 + s), scale));
    const int32_t flag2 = int32_t(Load(base, s + 30968 + 24 * k));
    double angle2 = Lfs(base, from + 30964);
    if (flag2 != 0) {
      angle2 = Fmuls(angle2, minus);
    }
    Stfs(base, r2 + 0, angle2);
    Stfs(base, r2 + 4, Lfs(base, from + 30964));
  }
}

uint64_t LoadVinyls(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                    uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822040A8 TuningScreen::LoadVinyls");
  namespace steps = strings_and_font::steps;
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = sp - 144;
  Store(base, frame, sp);
  const uint32_t c = tuning_screen::SelectedGarageCar(base, s) + 212;
  Store(base, frame + 88, 2);
  Store(base, frame + 80, 0);
  Store(base, frame + 84, 1);
  const double minus = Lfs(base, kMinusOne);
  const double zero = Lfs(base, kZero);
  const double scale = Lfs(base, kLoadScale);
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t n = Load(base, frame + 80 + 4 * k);
    const uint32_t r = (n << 5) + c + 96;  // the record (r10 + 96)
    const uint32_t to = s + 30880 + 24 * k;
    Store(base, s + 31168 + 4 * k, Load(base, r + 24));
    Store(base, to + 0, uint32_t(steps::Fctiwz(Lfs(base, r + 8))));
    Store(base, to + 4, uint32_t(steps::Fctiwz(Lfs(base, r + 12))));
    Stfs(base, to + 8, Fmuls(Lfs(base, r + 16), scale));
    // The flag: the record's +0 (read as the k-th) below 0.0; the angle +4.
    const bool negative = Lfs(base, c + 96 + 32 * k) < zero;
    const double first = Lfs(base, ((n + 3) << 5) + c);
    Stfs(base, to + 12, negative ? Fmuls(first, minus) : first);  // overwritten below
    Store(base, to + 16, negative ? 1 : 0);
    Stfs(base, to + 12, Lfs(base, r + 4));
    const uint32_t r2 = r + 96;
    const uint32_t to2 = s + 30952 + 24 * k;
    Store(base, s + 31180 + 4 * k, Load(base, r2 + 24));
    Store(base, to2 + 0, uint32_t(steps::Fctiwz(Lfs(base, r2 + 8))));
    Store(base, to2 + 4, uint32_t(steps::Fctiwz(Lfs(base, r2 + 12))));
    Stfs(base, to2 + 8, Fmuls(Lfs(base, r2 + 16), scale));
    const bool negative2 = Lfs(base, c + 192 + 32 * k) < zero;
    const double first2 = Lfs(base, ((n + 6) << 5) + c);
    Stfs(base, to2 + 12, negative2 ? Fmuls(first2, minus) : first2);
    Store(base, to2 + 16, negative2 ? 1 : 0);
    Stfs(base, to2 + 12, Lfs(base, r2 + 4));
  }
  // The six ids' saved copies.
  for (uint32_t j = 0; j < 6; j += 3) {
    for (uint32_t k = 0; k < 3; k++) {
      const uint32_t n = Load(base, frame + 80 + 4 * k);
      Store(base, s + 31192 + 4 * (j + k), Load(base, (n + j + 7792) * 4 + s));
    }
  }
  // The six records' saved copies.
  uint64_t r3 = screen;
  for (uint32_t j = 0; j < 6; j += 3) {
    for (uint32_t k = 0; k < 3; k++) {
      const uint32_t n = Load(base, frame + 80 + 4 * k);
      const uint64_t destination = screen + 31024 + 24 * (j + k);
      const uint32_t source = uint32_t(uint64_t(j + n) * 24 + screen) + 30880;
      if (native::Buffers_Memcpy(base, uint32_t(destination), source, 24, 0x822042B4)) {
        r3 = destination;
      } else {
        r3 = call(kMemcpy, {destination, source, 24}, frame, 0x822042B4);
      }
    }
  }
  return r3;
}

}  // namespace tuning_pages
