// The mode manager's tick services as plain C++
// (docs/functions/mode-tick.md); see mode_tick.h.

#include "mode_tick/mode_tick.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/last_error.h"
#include "vehicle_accessors/vehicle_accessors.h"

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

namespace {

void StoreChecks(uint8_t* base, uint32_t value) {
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(base, kChecks + 4 * i, value);
  }
}

// The checks +16, +20 and +24 set (the others cleared before).
void SetChecks16To24(uint8_t* base) {
  guest::StoreU32(base, kChecks + 16, 1);
  guest::StoreU32(base, kChecks + 20, 1);
  guest::StoreU32(base, kChecks + 24, 1);
}

// All seven set, in the order type 1's path stores them.
void SetAllChecks(uint8_t* base) {
  for (uint32_t offset : {16u, 20u, 24u, 0u, 4u, 8u, 12u}) {
    guest::StoreU32(base, kChecks + offset, 1);
  }
}

// The race data (`[0x826C5B2C]`), its stage list and the word +0xA3AF0
// (the human cars' arrangement: 6 is two).
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRaceStages = 0x59620;
constexpr uint32_t kRaceHumans = 0xA3AF0;
// The boot movies' first id (Boot::StartMovie): 10 is the UBI logo.
constexpr uint32_t kBootMovie = 0x826C5BAC;

}  // namespace

bool ModesSetChecks(uint8_t* base, uint32_t type, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82106868 Modes::SetChecks");
  switch (type) {
    case 1: {
      const uint32_t stage = frame + 80;
      if (!vehicle_accessors::StagesCopy(base, stage,
                                         guest::LoadU32(base, kRaceData) + kRaceStages,
                                         0xFFFFFFFF)) {
        return false;
      }
      const uint32_t humans = guest::LoadU32(base, guest::LoadU32(base, kRaceData) + kRaceHumans);
      if (humans == 0) {
        SetAllChecks(base);
      } else if (humans == 6) {
        StoreChecks(base, 0);
        if (int32_t(guest::LoadU32(base, stage)) == 1) {
          guest::StoreU32(base, kChecks + 20, 1);
          guest::StoreU32(base, kChecks + 24, 1);
        }
      } else {
        StoreChecks(base, 0);
        SetChecks16To24(base);
      }
      return true;
    }
    case 3:
      if (int32_t(guest::LoadU32(base, kBootMovie)) == 10) {
        SetAllChecks(base);
      } else {
        StoreChecks(base, 0);
        SetChecks16To24(base);
      }
      return true;
    case 4:
      SetAllChecks(base);
      return true;
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 12:
      StoreChecks(base, 0);
      SetChecks16To24(base);
      return true;
    default:  // 0, 11, above 12 and below 0
      StoreChecks(base, 1);
      return true;
  }
}

void ModesSetSceneTargetMode(uint8_t* base, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_82106740 Modes::SetSceneTargetMode");
  uint32_t mode = 0;
  switch (type) {
    case 1: mode = 1; break;
    case 3: mode = 3; break;
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
    case 12: mode = 4; break;
    default: break;  // 4, 9, 10, 11 and any other
  }
  guest::StoreU32(base, guest::LoadU32(base, 0x8266D08C) + 28, mode);
}

bool PresenceSetForMode(uint8_t* base, uint32_t pcr, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_822934B8 Presence::SetForMode");
  if (type == 1) {
    return false;
  }
  last_error::Set(base, pcr, 0);
  return true;
}

bool SignInWatchCheckIdle(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8209ED58 SignInWatch::CheckIdle");
  auto prompt = [&](uint32_t reason) {
    guest::StoreU32(base, kWatchState, 1);
    guest::StoreU32(base, kWatchReason, reason);
  };
  if (int32_t(guest::LoadU32(base, kOtherUserSignedIn)) == 1) {
    guest::StoreU32(base, kWatchReason, 0);
    guest::StoreU32(base, kWatchState, 1);
    return true;
  }
  if (int32_t(guest::LoadU32(base, kUserSignedOut)) == 1) {
    guest::StoreU32(base, kWatchReason, 1);
    guest::StoreU32(base, kWatchState, 1);
    return true;
  }
  if (guest::LoadU32(base, kChecks + 8) == 0 && int32_t(guest::LoadU32(base, 0x82816118)) == 1) {
    prompt(2);
    return true;
  }
  // No network session: r8 0 in the original.
  bool session = true;
  if (guest::LoadU32(base, 0x826C7824) != 0) {
    const uint32_t begin = guest::LoadU32(base, 0x826C7308 + 4);
    const uint32_t end = guest::LoadU32(base, 0x826C7308 + 8);
    if (begin > end || (end - begin) % 384 != 0) {
      return false;
    }
    session = begin != end;
  } else {
    session = guest::LoadU32(base, 0x826C7878) == 0;
  }
  if (guest::LoadU32(base, kChecks + 20) == 0 && !session) {
    prompt(5);
  }
  if (guest::LoadU32(base, kChecks + 16) != 0) {
    return true;
  }
  const uint32_t shift = guest::LoadU32(base, kPlayerUsers) & 0x3F;
  const uint32_t bit = shift >= 32 ? 0 : 1u << shift;
  if ((bit & guest::LoadU32(base, 0x826C795C)) != 0) {
    return true;
  }
  prompt(4);
  return true;
}

}  // namespace mode_tick
