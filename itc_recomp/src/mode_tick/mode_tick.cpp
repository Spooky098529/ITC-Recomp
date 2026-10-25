// The mode manager's tick services as plain C++
// (docs/functions/mode-tick.md); see mode_tick.h.

#include "mode_tick/mode_tick.h"

#include "base/log.h"
#include "guest.h"

namespace mode_tick {

void BillboardsFlipRing(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8209BD18 Billboards::FlipRing");
  const uint32_t half = guest::LoadU32(base, kRingHalf);
  guest::StoreU32(base, kRingCount, 0);
  guest::StoreU32(base, kRingHalf, half == 0 ? 1 : 0);
}

void ScreenFadesInit(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8209CDE8 ScreenFades::Init");
  for (uint32_t layer = 0; layer < 2; layer++) {
    for (uint32_t words : {kFadeAlphas, kFadeOutDurations, kFadeInDurations, kFadeOutCounts,
                           kFadeInCounts}) {
      guest::StoreU32(base, words + 4 * layer, 0);
    }
  }
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(base, kFadeColors + 4 * i, 0);
  }
}

void ScreenFadesFadeIn(uint8_t* base, uint32_t passes, uint32_t red, uint32_t green,
                       uint32_t blue, uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_8209D038 ScreenFades::FadeIn");
  const uint32_t word = 4 * layer, color = kFadeColors + 12 * layer;
  guest::StoreU32(base, kFadeInDurations + word, passes);
  guest::StoreU32(base, kFadeInCounts + word, passes);
  guest::StoreU32(base, kFadeOutCounts + word, 0);
  guest::StoreU32(base, kFadeAlphas + word, 0);
  guest::StoreU32(base, color, red);
  guest::StoreU32(base, color + 4, green);
  guest::StoreU32(base, color + 8, blue);
  // The other layer: 1 for layer 0, else 0 (any other value).
  if (layer == 0) {
    guest::StoreU32(base, kFadeInCounts + 4, 0);
    guest::StoreU32(base, kFadeOutCounts + 4, 0);
    guest::StoreU32(base, kFadeAlphas + 4, 0);
  } else {
    guest::StoreU32(base, kFadeInCounts, 0);
    guest::StoreU32(base, kFadeAlphas, 0);
    guest::StoreU32(base, kFadeOutCounts, 0);
  }
}

void ScreenFadesFadeOut(uint8_t* base, uint32_t passes, uint32_t red, uint32_t green,
                        uint32_t blue, uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_8209D0C8 ScreenFades::FadeOut");
  const uint32_t word = 4 * layer, color = kFadeColors + 12 * layer;
  guest::StoreU32(base, color, red);
  guest::StoreU32(base, kFadeOutCounts + word, passes);
  guest::StoreU32(base, kFadeOutDurations + word, passes);
  guest::StoreU32(base, kFadeInCounts + word, 0);
  guest::StoreU32(base, kFadeAlphas + word, 0);
  guest::StoreU32(base, color + 4, green);
  guest::StoreU32(base, color + 8, blue);
}

int32_t PlayersSignedInSlot(uint8_t* base, uint32_t player) {
  ITC_NATIVE_RUNNING("sub_8209F658 Players::SignedInSlot");
  const uint32_t mask = guest::LoadU32(base, kSignedInMask);
  int32_t slot = 0;
  for (uint32_t user = 0; user < 4; user++) {
    if ((mask & (1u << user)) == 0) {
      continue;
    }
    if (user == guest::LoadU32(base, kPlayerUsers + 4 * player)) {
      return slot;
    }
    slot++;
  }
  return -1;
}

void ActionMappersSetBlocked(uint8_t* base, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8209F318 ActionMappers::SetBlocked");
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, kActionMapperEntries + 8 * i + 4, value);
  }
}

}  // namespace mode_tick
