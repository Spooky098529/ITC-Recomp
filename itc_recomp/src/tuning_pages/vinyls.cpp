// The vinyls' records between the screen and the selected garage car
// (docs/functions/tuning-pages.md): see tuning_pages.h. Single-precision
// steps as the originals' (each product rounded to a float); needs
// flush-to-zero off, as the originals' lfs leaves it.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/car_makers.h"
#include "schema/tuning_screen.h"
#include "strings_and_font/steps.h"
#include "tuning_screen/tuning_screen.h"

#include <bit>

namespace tuning_pages {

namespace {

// The screen (tuning-screen.toml's TuningScreen).
schema::TuningScreen Screen(uint32_t s) { return schema::TuningScreen(s); }

using G = schema::TuningPagesGlobals;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
// lfs: a float made a double.
double Lfs(uint32_t address) {
  return double(std::bit_cast<float>(Load(address)));
}
// stfs: a double rounded to a float.
void Stfs(uint32_t address, double value) {
  Store(address, std::bit_cast<uint32_t>(float(value)));
}
// fmuls: the product rounded to a float.
double Fmuls(double a, double b) { return double(float(a * b)); }
// An int made a float (std, lfd, fcfid, frsp); the doubleword the original
// passes it through on the stack is a C++ value since E5 step 6.
double IntToFloat(int32_t value) {
  const uint64_t word = uint64_t(int64_t(value));
  return double(float(double(int64_t(word))));
}

constexpr uint32_t kMemcpy = 0x825A7410;

}  // namespace

void StoreVinyls(uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82203F20 TuningScreen::StoreVinyls");
  const uint32_t s = uint32_t(screen);
  const uint32_t c = tuning_screen::SelectedGarageCar(s) + 212;
  // The three ids and the conversions' doublewords, below `sp` in the
  // original (a leaf): C++ values since E5 step 6 (only read here).
  (void)sp;
  const uint32_t ids[3] = {0, 1, 2};
  const double scale = double(G::store_scale());
  const double minus = double(G::minus_one());
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t n = ids[k];
    const uint32_t from = uint32_t(uint64_t(n) * 24 + screen);
    const uint32_t r = c + 96 + 32 * k;  // the first group's record
    const uint32_t r2 = r + 96;          // the second's
    schema::CarMakersCarDecal(r).set_image(Load((n + 7792) * 4 + s));
    schema::CarMakersCarDecal(r).set_x(float(IntToFloat(int32_t(Load(from + 30880)))));
    schema::CarMakersCarDecal(r).set_y(float(IntToFloat(int32_t(Load(from + 30884)))));
    schema::CarMakersCarDecal(r).set_angle(float(Fmuls(Lfs((n + 1287) * 24 + s), scale)));
    const int32_t flag = int32_t(Load(Screen(s).vinyl_layers_0_address() + 16 + 24 * k));
    double angle = Lfs(from + 30892);
    if (flag != 0) {
      angle = Fmuls(angle, minus);
    }
    schema::CarMakersCarDecal(r).set_flag_scale(float(angle));
    schema::CarMakersCarDecal(r).set_scale(float(Lfs(from + 30892)));
    schema::CarMakersCarDecal(r2).set_image(Load((n + 7795) * 4 + s));
    schema::CarMakersCarDecal(r2).set_x(float(IntToFloat(int32_t(Load(from + 30952)))));
    schema::CarMakersCarDecal(r2).set_y(float(IntToFloat(int32_t(Load(from + 30956)))));
    schema::CarMakersCarDecal(r2).set_angle(float(Fmuls(Lfs((n + 1290) * 24 + s), scale)));
    const int32_t flag2 = int32_t(Load(Screen(s).vinyl_layers_1_address() + 16 + 24 * k));
    double angle2 = Lfs(from + 30964);
    if (flag2 != 0) {
      angle2 = Fmuls(angle2, minus);
    }
    schema::CarMakersCarDecal(r2).set_flag_scale(float(angle2));
    schema::CarMakersCarDecal(r2).set_scale(float(Lfs(from + 30964)));
  }
}

uint64_t LoadVinyls(const tuning_screen::GuestCall& call, uint64_t screen,
                    uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822040A8 TuningScreen::LoadVinyls");
  namespace steps = strings_and_font::steps;
  const uint32_t s = uint32_t(screen);
  // No guest frame (E5 step 6): the original's 144 bytes held the back chain
  // and the three ids at +80 (a C++ array, only read here); memcpy's guest
  // fallback is called from `sp`.
  const uint32_t c = tuning_screen::SelectedGarageCar(s) + 212;
  const uint32_t ids[3] = {0, 1, 2};
  const double minus = double(G::minus_one());
  const double zero = double(G::zero());
  const double scale = double(G::load_scale());
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t n = ids[k];
    const uint32_t r = (n << 5) + c + 96;  // the record (r10 + 96)
    const uint32_t to = Screen(s).vinyl_layers_0_address() + 24 * k;
    Store(Screen(s).layer_ids_0_address() + 4 * k, schema::CarMakersCarDecal(r).image());
    Store(to + 0, uint32_t(steps::Fctiwz(double(schema::CarMakersCarDecal(r).x()))));
    Store(to + 4, uint32_t(steps::Fctiwz(double(schema::CarMakersCarDecal(r).y()))));
    Stfs(to + 8, Fmuls(double(schema::CarMakersCarDecal(r).angle()), scale));
    // The flag: the record's +0 (read as the k-th) below 0.0; the angle +4.
    const bool negative = double(schema::CarMakersCarDecal(c + 96 + 32 * k).flag_scale()) < zero;
    const double first = Lfs(((n + 3) << 5) + c);
    Stfs(to + 12, negative ? Fmuls(first, minus) : first);  // overwritten below
    Store(to + 16, negative ? 1 : 0);
    Stfs(to + 12, double(schema::CarMakersCarDecal(r).scale()));
    const uint32_t r2 = r + 96;
    const uint32_t to2 = Screen(s).vinyl_layers_1_address() + 24 * k;
    Store(Screen(s).layer_ids_1_address() + 4 * k, schema::CarMakersCarDecal(r2).image());
    Store(to2 + 0, uint32_t(steps::Fctiwz(double(schema::CarMakersCarDecal(r2).x()))));
    Store(to2 + 4, uint32_t(steps::Fctiwz(double(schema::CarMakersCarDecal(r2).y()))));
    Stfs(to2 + 8, Fmuls(double(schema::CarMakersCarDecal(r2).angle()), scale));
    const bool negative2 = double(schema::CarMakersCarDecal(c + 192 + 32 * k).flag_scale()) < zero;
    const double first2 = Lfs(((n + 6) << 5) + c);
    Stfs(to2 + 12, negative2 ? Fmuls(first2, minus) : first2);
    Store(to2 + 16, negative2 ? 1 : 0);
    Stfs(to2 + 12, double(schema::CarMakersCarDecal(r2).scale()));
  }
  // The six ids' saved copies.
  for (uint32_t j = 0; j < 6; j += 3) {
    for (uint32_t k = 0; k < 3; k++) {
      const uint32_t n = ids[k];
      Store(Screen(s).saved_layer_ids_0_address() + 4 * (j + k), Load((n + j + 7792) * 4 + s));
    }
  }
  // The six records' saved copies.
  uint64_t r3 = screen;
  for (uint32_t j = 0; j < 6; j += 3) {
    for (uint32_t k = 0; k < 3; k++) {
      const uint32_t n = ids[k];
      const uint64_t destination = screen + 31024 + 24 * (j + k);
      const uint32_t source = uint32_t(uint64_t(j + n) * 24 + screen) + 30880;
      if (native::Buffers_Memcpy(uint32_t(destination), source, 24, 0x822042B4)) {
        r3 = destination;
      } else {
        r3 = call(kMemcpy, {destination, source, 24}, sp, 0x822042B4);
      }
    }
  }
  return r3;
}

}  // namespace tuning_pages
