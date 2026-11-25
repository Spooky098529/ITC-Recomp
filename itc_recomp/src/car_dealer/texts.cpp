// The car dealer's movie 4 setters (docs/functions/car-dealer.md): see
// car_dealer.h.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace car_dealer {

namespace {

// SetKindBar and SetColorBar: node (sub_scene, 0) +16 = the value as a
// float, the 64-bit integer the conversion went through left at +80.
uint64_t SetBar(uint32_t sub_scene, uint64_t value, uint32_t sp) {
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(4, frame + 80);
  const int64_t wide = int64_t(int32_t(uint32_t(value)));
  Store(frame + 80, uint32_t(uint64_t(wide) >> 32));
  Store(frame + 84, uint32_t(uint64_t(wide)));
  const float bar = float(double(wide));
  const uint32_t node = ui_movies::FindNode(movie, sub_scene, 0);
  Node(node).set_y(bar);
  return node;
}

}  // namespace

uint64_t SetKindBar(uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82182158 CarDealer::SetKindBar");
  return SetBar(8, value, sp);
}

uint64_t SetColorBar(uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82182770 CarDealer::SetColorBar");
  return SetBar(26, value, sp);
}

uint64_t SetScrollArrows(uint64_t scroll, uint64_t count, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82182DD0 CarDealer::SetScrollArrows");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(4, frame + 80);
  const uint32_t up = ui_movies::FindNode(movie, 7, 3);
  const uint32_t down = ui_movies::FindNode(movie, 7, 4);
  const int32_t s = int32_t(uint32_t(scroll));
  if (s == 0) {
    Node(up).set_visible(0);
    Node(down).set_visible(1);
  } else {
    Node(up).set_visible(1);
    Node(down).set_visible(int32_t(uint32_t(s + 4)) == int32_t(uint32_t(count)) ? 0 : 1);
  }
  return down;
}

uint64_t SetHeader(uint32_t a, uint32_t b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82182C28 CarDealer::SetHeader");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(4, frame + 80);
  const uint32_t first = ui_movies::FindNode(movie, 2, 1);
  const uint32_t second = ui_movies::FindNode(movie, 2, 2);
  Node(first).set_texture_entry(a);
  Node(second).set_texture_entry(b);
  return second;
}

uint64_t SetPrompts(const uint32_t args[6], uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82182B30 CarDealer::SetPrompts");
  const uint32_t frame = Frame(sp, 176);
  const uint32_t movie = FindMovie(4, frame + 80);
  // The pairs' first values at +88 (r3, r5, r7), their second at +104 (r4,
  // r6, r8), stored in the original's order.
  Store(frame + 112, args[5]);
  Store(frame + 92, args[2]);
  Store(frame + 96, args[4]);
  Store(frame + 104, args[1]);
  Store(frame + 108, args[3]);
  Store(frame + 88, args[0]);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t prompt = ui_movies::FindNode(movie, 16, i);
    // The prompt's +100 read as a word (its first child index), twice.
    const Node group(prompt);
    const uint32_t first = ui_movies::FindNode(movie, 16, Load(group.width_address()));
    r3 = ui_movies::FindNode(movie, 16, Load(group.width_address()) + 1);
    const uint32_t a = Load(frame + 88 + 4 * i);
    if (a != 0xFFFFFFFFu) {
      const uint32_t b = Load(frame + 104 + 4 * i);
      if (b != 0xFFFFFFFFu) {
        Node(first).set_visible(1);
        Node(r3).set_visible(1);
        Node(first).set_texture_entry(a);
        Node(r3).set_texture_entry(b);
        continue;
      }
    }
    Node(first).set_visible(0);
    Node(r3).set_visible(0);
  }
  return r3;
}

uint64_t HelpLineSet(uint64_t id, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199B38 HelpLine::Set");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(4, frame + 80);
  calls.Call(kSetText, frame, 0x82199B7C, movie, 17, 0, id);
  calls.Call(kScrollText, frame, 0x82199B90, movie, 17, 0, 1);
  return calls.Call(kScrollReset, frame, 0x82199BA0, movie, 17, 0);
}

}  // namespace car_dealer
