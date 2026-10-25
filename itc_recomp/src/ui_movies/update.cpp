// UiMovie::Update's own work (sub_8211D2F8) as plain C++: see game.h. Float
// work is the translated code's step for step, flush-to-zero off (the
// adapter's job). Built with -ffp-model=strict and -ffp-contract=off
// (CMakeLists.txt).

#include "ui_movies/game.h"

#include "base/log.h"
#include "strings_and_font/steps.h"

namespace ui_movies {

using namespace strings_and_font::steps;

namespace {
constexpr uint32_t kStep = 0x820021E8;  // the time a frame (f30)
constexpr uint32_t kZero = 0x82000A34;  // 0.0 (f31)
}  // namespace

void UpdateBegin(uint8_t* base, uint32_t local) {
  ITC_NATIVE_RUNNING("sub_8211D2F8 UiMovie::Update");
  for (uint32_t k = 0; k < 32; k++) {
    guest::StoreU16(base, local + 4 * k + 2, uint16_t(k));
    guest::StoreU16(base, local + 4 * k, uint16_t(k + 1));
  }
  guest::StoreU16(base, local + 128, 0);
  guest::StoreU16(base, local + 136, 0);
  guest::StoreU16(base, local + 124, 0xFFFF);
  guest::StoreU16(base, local + 132, 0xFFFF);
  guest::StoreU16(base, local + 134, 0xFFFF);
  guest::StoreU16(base, local + 130, 0xFFFF);
}

void StepQueuedTime(uint8_t* base, uint32_t slot) {
  const bool reversed = guest::LoadU32(base, slot + 28) != 0;
  double time = Lfs(base, slot + 20);
  const double step = Lfs(base, kStep), zero = Lfs(base, kZero);
  time = reversed ? Single(time - step) : Single(time + step);
  if (time < zero) {
    time = zero;
  } else {
    const double length = Lfs(base, guest::LoadU32(base, slot + 16) + 12);
    if (time > length) {
      time = length;
    }
  }
  Stfs(base, slot + 20, time);
}

void StepLoopingTime(uint8_t* base, uint32_t slot) {
  const double step = Lfs(base, kStep), zero = Lfs(base, kZero);
  double time = Single(Lfs(base, slot + 20) + step);
  const uint32_t animation = guest::LoadU32(base, slot + 16);
  if (time < zero) {
    time = Lfs(base, animation + 12);
  } else {
    const double length = Lfs(base, animation + 12);
    if (time > length) {
      time = zero;
    }
  }
  Stfs(base, slot + 20, time);
}

void UnlinkQueueFirst(uint8_t* base, uint32_t list) {
  const uint16_t first = guest::LoadU16(base, list + 132);
  const uint32_t link = list + (uint32_t(int32_t(int16_t(first))) << 2);
  const uint16_t next = guest::LoadU16(base, link);
  guest::StoreU16(base, link, first);
  guest::StoreU16(base, list + 132, next);
  const uint16_t unlinked = guest::LoadU16(base, link);
  const uint16_t free = guest::LoadU16(base, list + 128);
  guest::StoreU16(base, list + 128, unlinked);
  guest::StoreU16(base, link, free);
  guest::StoreU16(base, list + 136, uint16_t(guest::LoadU16(base, list + 136) - 1));
}

void UnlinkLocalFirst(uint8_t* base, uint32_t list, uint32_t link, uint16_t first) {
  const uint16_t next = guest::LoadU16(base, link);
  guest::StoreU16(base, link, first);
  const uint16_t free = guest::LoadU16(base, list + 128);
  guest::StoreU16(base, list + 132, next);
  const uint16_t unlinked = guest::LoadU16(base, link);
  guest::StoreU16(base, list + 128, unlinked);
  guest::StoreU16(base, link, free);
  guest::StoreU16(base, list + 136, uint16_t(guest::LoadU16(base, list + 136) - 1));
}

}  // namespace ui_movies
