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
uint64_t SetBar(uint8_t* base, uint32_t sub_scene, uint64_t value, uint32_t sp) {
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  const int64_t wide = int64_t(int32_t(uint32_t(value)));
  Store(base, frame + 80, uint32_t(uint64_t(wide) >> 32));
  Store(base, frame + 84, uint32_t(uint64_t(wide)));
  const float bar = float(double(wide));
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, 0);
  guest::StoreFloat(base, node + 16, bar);
  return node;
}

}  // namespace

uint64_t SetKindBar(uint8_t* base, uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82182158 CarDealer::SetKindBar");
  return SetBar(base, 8, value, sp);
}

uint64_t SetColorBar(uint8_t* base, uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82182770 CarDealer::SetColorBar");
  return SetBar(base, 26, value, sp);
}

uint64_t SetScrollArrows(uint8_t* base, uint64_t scroll, uint64_t count, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82182DD0 CarDealer::SetScrollArrows");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  const uint32_t up = ui_movies::FindNode(base, movie, 7, 3);
  const uint32_t down = ui_movies::FindNode(base, movie, 7, 4);
  const int32_t s = int32_t(uint32_t(scroll));
  if (s == 0) {
    Store(base, up + 80, 0);
    Store(base, down + 80, 1);
  } else {
    Store(base, up + 80, 1);
    Store(base, down + 80, int32_t(uint32_t(s + 4)) == int32_t(uint32_t(count)) ? 0 : 1);
  }
  return down;
}

uint64_t SetHeader(uint8_t* base, uint32_t a, uint32_t b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82182C28 CarDealer::SetHeader");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  const uint32_t first = ui_movies::FindNode(base, movie, 2, 1);
  const uint32_t second = ui_movies::FindNode(base, movie, 2, 2);
  Store(base, first + 108, a);
  Store(base, second + 108, b);
  return second;
}

uint64_t SetPrompts(uint8_t* base, const uint32_t args[6], uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82182B30 CarDealer::SetPrompts");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  // The pairs' first values at +88 (r3, r5, r7), their second at +104 (r4,
  // r6, r8), stored in the original's order.
  Store(base, frame + 112, args[5]);
  Store(base, frame + 92, args[2]);
  Store(base, frame + 96, args[4]);
  Store(base, frame + 104, args[1]);
  Store(base, frame + 108, args[3]);
  Store(base, frame + 88, args[0]);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t prompt = ui_movies::FindNode(base, movie, 16, i);
    const uint32_t first = ui_movies::FindNode(base, movie, 16, Load(base, prompt + 100));
    r3 = ui_movies::FindNode(base, movie, 16, Load(base, prompt + 100) + 1);
    const uint32_t a = Load(base, frame + 88 + 4 * i);
    if (a != 0xFFFFFFFFu) {
      const uint32_t b = Load(base, frame + 104 + 4 * i);
      if (b != 0xFFFFFFFFu) {
        Store(base, first + 80, 1);
        Store(base, r3 + 80, 1);
        Store(base, first + 108, a);
        Store(base, r3 + 108, b);
        continue;
      }
    }
    Store(base, first + 80, 0);
    Store(base, r3 + 80, 0);
  }
  return r3;
}

uint64_t HelpLineSet(uint8_t* base, uint64_t id, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199B38 HelpLine::Set");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  calls.Call(kSetText, frame, 0x82199B7C, movie, 17, 0, id);
  calls.Call(kScrollText, frame, 0x82199B90, movie, 17, 0, 1);
  return calls.Call(kScrollReset, frame, 0x82199BA0, movie, 17, 0);
}

}  // namespace car_dealer
