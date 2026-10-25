// UiMovie::ApplyAnimation (sub_825A08E0) as plain C++: see ui_movies.h.
// Step for step as the translated code: each single-precision operation is
// the double operation rounded to float (R below), the fmadds std::fma
// rounded to float. Built with -ffp-model=strict and -ffp-contract=off
// (CMakeLists.txt), as keys.cpp.

#include "ui_movies/ui_movies.h"

#include "base/log.h"

#include <bit>
#include <cmath>

namespace ui_movies {

namespace {

constexpr uint32_t kOne = 0x82000A30;   // f28: 1.0
constexpr uint32_t kZero = 0x82000A34;  // f30

double R(double x) { return double(float(x)); }

double LoadFloat(uint8_t* base, uint32_t address) {
  return double(std::bit_cast<float>(guest::LoadU32(base, address)));
}

// The tracks of one node track list on one child: each track's keys around
// the time and its t (the original's loc_825A0974 to loc_825A0AF4).
void ApplyTracks(uint8_t* base, uint32_t animation, uint32_t list, uint32_t child, double time,
                 double one, double zero, uint32_t frame, const TrackCall& apply) {
  for (uint32_t i = 0; i < guest::LoadU32(base, list + 12); i++) {
    const uint32_t track = NodeTrack(list, i);
    const uint32_t keys = KeyList(base, animation, guest::LoadU32(base, track + 4));
    const double local = R(time - LoadFloat(base, track + 8));
    if (local < zero) {
      continue;
    }
    uint32_t k0 = 0, k1 = 0;
    // The keys around the time: k[i] at or before the local time, and the
    // animation's time (not the local one, as the original compares) before
    // k[i + 1].
    bool found = false;
    if (guest::LoadU32(base, keys + 12) - 1 != 0) {
      for (uint32_t k = 0;;) {
        if (!(LoadFloat(base, Key(keys, k) + 4) > local) &&
            time < LoadFloat(base, Key(keys, k + 1) + 4)) {
          k0 = Key(keys, k);
          k1 = Key(keys, k + 1);
          found = k0 != 0;
          break;
        }
        k++;
        if (!(k < guest::LoadU32(base, keys + 12) - 1)) {
          break;
        }
      }
    }
    double start = zero;  // f13: k0's time
    if (found) {
      start = LoadFloat(base, k0 + 4);
    } else if (time < LoadFloat(base, Key(keys, 0) + 4)) {
      // Before the first key: none, then it.
      k0 = 0;
      k1 = Key(keys, 0);
    } else if (!(LoadFloat(base, Key(keys, guest::LoadU32(base, keys + 12) - 1) + 4) > time)) {
      // At or after the last key: it, then none.
      k1 = 0;
      k0 = Key(keys, guest::LoadU32(base, keys + 12) - 1);
      if (k0 != 0) {
        start = LoadFloat(base, k0 + 4);
      }
    }
    // The end: k1's time, or the animation's length with no k1.
    double t = R((k1 != 0 ? LoadFloat(base, k1 + 4) : LoadFloat(base, animation + 12)) - start);
    if (t == zero) {
      t = zero;
    } else {
      t = R(R(local - start) / t);
      if (t < zero) {
        t = zero;
      } else if (t > one) {
        t = one;
      }
    }
    apply.call(apply.user, frame, child, track, t, k0, k1);
  }
}

}  // namespace

uint32_t ApplyAnimation(uint8_t* base, uint32_t node, uint32_t animation, double time,
                        uint32_t loop, uint32_t frame, const TrackCall& apply) {
  ITC_NATIVE_RUNNING("sub_825A08E0 UiMovie::ApplyAnimation");
  const double one = LoadFloat(base, kOne);
  const double zero = LoadFloat(base, kZero);
  for (uint32_t l = 0; l < guest::LoadU32(base, animation + 36); l++) {
    const uint32_t list = NodeTrackList(base, animation, l);
    for (uint32_t c = 0; c < guest::LoadU32(base, node + 112); c++) {
      const uint32_t child = Child(base, node, c);
      if (guest::LoadU32(base, child + 8) != guest::LoadU32(base, list + 8)) {
        continue;
      }
      ApplyTracks(base, animation, list, child, time, one, zero, frame, apply);
    }
  }
  for (uint32_t s = 0; s < guest::LoadU32(base, animation + 52); s++) {
    const uint32_t sub = SubAnimation(base, animation, s);
    if (time < LoadFloat(base, sub + 24)) {
      continue;
    }
    // The time within the sub-animation's range (+24, +28), clamped above
    // at 1, mapped onto +32..+36.
    const double end = LoadFloat(base, sub + 28);
    double f0;
    if (R(end - LoadFloat(base, sub + 24)) == zero) {
      f0 = zero;
    } else {
      const double f13 =
          R(R(time - LoadFloat(base, sub + 24)) / R(end - LoadFloat(base, sub + 24)));
      if (f13 > one) {
        f0 = one;
      } else {
        f0 = R(R(time - LoadFloat(base, sub + 24)) / R(end - LoadFloat(base, sub + 24)));
      }
    }
    const double m0 = LoadFloat(base, sub + 32);
    const double mapped = R(std::fma(R(LoadFloat(base, sub + 36) - m0), f0, m0));
    const uint32_t id = guest::LoadU32(base, sub + 8);
    if (id == 0) {
      ApplyAnimation(base, node, guest::LoadU32(base, sub + 16), mapped, loop, frame - 224,
                     apply);
      continue;
    }
    for (uint32_t c = 0; c < guest::LoadU32(base, node + 112); c++) {
      const uint32_t child = Child(base, node, c);
      if (guest::LoadU32(base, child + 8) == id) {
        ApplyAnimation(base, child, guest::LoadU32(base, sub + 16), mapped, loop, frame - 224,
                       apply);
      }
    }
  }
  if (int32_t(loop) != 0) {
    return time > zero ? 1 : 0;
  }
  return time < LoadFloat(base, animation + 12) ? 1 : 0;
}

}  // namespace ui_movies
