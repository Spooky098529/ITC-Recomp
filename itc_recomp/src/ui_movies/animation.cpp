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

using G = schema::UiMoviesGlobals;

double R(double x) { return double(float(x)); }

// The tracks of one node track list on one child: each track's keys around
// the time and its t (the original's loc_825A0974 to loc_825A0AF4).
void ApplyTracks(uint32_t animation, uint32_t list, uint32_t child, double time,
                 double one, double zero, uint32_t frame, const TrackCall& apply) {
  const auto key_time = [&](uint32_t key) { return double(schema::UiMovieKey(key).time()); };
  for (uint32_t i = 0; i < schema::UiMovieNodeTrackList(list).track_count(); i++) {
    const schema::UiMovieTrack track(NodeTrack(list, i));
    const uint32_t keys = KeyList(animation, track.key_list());
    const schema::UiMovieKeyList key_list(keys);
    const double local = R(time - double(track.start()));
    if (local < zero) {
      continue;
    }
    uint32_t k0 = 0, k1 = 0;
    // The keys around the time: k[i] at or before the local time, and the
    // animation's time (not the local one, as the original compares) before
    // k[i + 1].
    bool found = false;
    if (key_list.key_count() - 1 != 0) {
      for (uint32_t k = 0;;) {
        if (!(key_time(Key(keys, k)) > local) && time < key_time(Key(keys, k + 1))) {
          k0 = Key(keys, k);
          k1 = Key(keys, k + 1);
          found = k0 != 0;
          break;
        }
        k++;
        if (!(k < key_list.key_count() - 1)) {
          break;
        }
      }
    }
    double start = zero;  // f13: k0's time
    if (found) {
      start = key_time(k0);
    } else if (time < key_time(Key(keys, 0))) {
      // Before the first key: none, then it.
      k0 = 0;
      k1 = Key(keys, 0);
    } else if (!(key_time(Key(keys, key_list.key_count() - 1)) > time)) {
      // At or after the last key: it, then none.
      k1 = 0;
      k0 = Key(keys, key_list.key_count() - 1);
      if (k0 != 0) {
        start = key_time(k0);
      }
    }
    // The end: k1's time, or the animation's length with no k1.
    double t = R((k1 != 0 ? key_time(k1)
                          : double(schema::UiMovieAnimation(animation).length())) -
                 start);
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
    apply.call(apply.user, frame, child, track.address, t, k0, k1);
  }
}

}  // namespace

uint32_t ApplyAnimation(uint32_t node, uint32_t animation, double time,
                        uint32_t loop, uint32_t frame, const TrackCall& apply) {
  ITC_NATIVE_RUNNING("sub_825A08E0 UiMovie::ApplyAnimation");
  const schema::UiMovieAnimation a(animation);
  const double one = double(G::one());
  const double zero = double(G::zero());
  for (uint32_t l = 0; l < a.node_track_list_count(); l++) {
    const uint32_t list = NodeTrackList(animation, l);
    for (uint32_t c = 0; c < schema::UiMovieNode(node).unknown_112(); c++) {
      const uint32_t child = Child(node, c);
      if (schema::UiMovieNode(child).id() !=
          schema::UiMovieNodeTrackList(list).node_id()) {
        continue;
      }
      ApplyTracks(animation, list, child, time, one, zero, frame, apply);
    }
  }
  for (uint32_t s = 0; s < a.sub_animation_count(); s++) {
    const schema::UiMovieSubAnimation sub(SubAnimation(animation, s));
    if (time < double(sub.start())) {
      continue;
    }
    // The time within the sub-animation's range (+24, +28), clamped above
    // at 1, mapped onto +32..+36.
    const double end = double(sub.end());
    double f0;
    if (R(end - double(sub.start())) == zero) {
      f0 = zero;
    } else {
      const double f13 = R(R(time - double(sub.start())) / R(end - double(sub.start())));
      if (f13 > one) {
        f0 = one;
      } else {
        f0 = R(R(time - double(sub.start())) / R(end - double(sub.start())));
      }
    }
    const double m0 = double(sub.map_start());
    const double mapped = R(std::fma(R(double(sub.map_end()) - m0), f0, m0));
    const uint32_t id = sub.node_id();
    if (id == 0) {
      ApplyAnimation(node, sub.animation().address, mapped, loop, frame - 224,
                     apply);
      continue;
    }
    for (uint32_t c = 0; c < schema::UiMovieNode(node).unknown_112(); c++) {
      const uint32_t child = Child(node, c);
      if (schema::UiMovieNode(child).id() == id) {
        ApplyAnimation(child, sub.animation().address, mapped, loop, frame - 224,
                       apply);
      }
    }
  }
  if (int32_t(loop) != 0) {
    return time > zero ? 1 : 0;
  }
  return time < double(a.length()) ? 1 : 0;
}

}  // namespace ui_movies
