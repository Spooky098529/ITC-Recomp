// The mode manager's tick services as plain C++
// (docs/functions/mode-tick.md); see mode_tick.h.

#include "mode_tick/mode_tick.h"

#include "base/log.h"
#include "guest.h"
#include "c_runtime/strings.h"
#include "command_list/command_list.h"
#include "helpers/compare_string.h"
#include "hooks/last_error.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <climits>
#include <cmath>

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

namespace {

// fctiwz's answer as the translated code computes it (a NaN 0x80000000, at
// or above INT_MAX INT_MAX, else truncated: below INT_MIN 0x80000000).
uint32_t ToInt(double value) {
  if (std::isnan(value)) {
    return 0x80000000u;
  }
  if (value >= double(INT_MAX)) {
    return uint32_t(INT_MAX);
  }
  if (value <= double(INT_MIN) - 1.0) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(value));
}

// 255.0 / duration × count, as the original's fdivs and fmuls (single
// rounding each), truncated.
uint32_t FadeAlpha(uint8_t* base, int32_t duration, int32_t count) {
  const double scale = double(guest::LoadFloat(base, 0x82000C44));  // 255.0
  const double d = double(float(double(duration)));
  const double c = double(float(double(count)));
  return ToInt(double(float(double(float(scale / d)) * c)));
}

// A layer's counts stepped: the in count (the alpha down from 255), then
// the out count (the alpha up to 255, held there at 0).
void StepFade(uint8_t* base, uint32_t layer) {
  const uint32_t word = 4 * layer;
  int32_t count = int32_t(guest::LoadU32(base, kFadeInCounts + word));
  if (count > 0) {
    const int32_t duration = int32_t(guest::LoadU32(base, kFadeInDurations + word));
    count--;
    guest::StoreU32(base, kFadeInCounts + word, uint32_t(count));
    guest::StoreU32(base, kFadeAlphas + word, FadeAlpha(base, duration, count));
  }
  count = int32_t(guest::LoadU32(base, kFadeOutCounts + word));
  if (count > 0) {
    count--;
    guest::StoreU32(base, kFadeOutCounts + word, uint32_t(count));
    if (count == 0) {
      guest::StoreU32(base, kFadeAlphas + word, 255);
    } else {
      const int32_t duration = int32_t(guest::LoadU32(base, kFadeOutDurations + word));
      guest::StoreU32(base, kFadeAlphas + word,
                      FadeAlpha(base, duration, int32_t(uint32_t(duration) - uint32_t(count))));
    }
  }
}

// The layer's color rectangle at `rect`, in the original's store order (its
// matrix rows are vector stores, at 16-byte aligned addresses, over the
// 1.0s it stores at +12, +28, +44 and +60 first).
void BuildFadeRect(uint8_t* base, uint32_t rect, uint32_t layer) {
  const uint32_t one = guest::LoadU32(base, 0x82000A30);  // 1.0
  for (uint32_t row = 0; row < 4; row++) {
    guest::StoreU32(base, rect + 16 * row + 12, one);
  }
  const uint32_t color = kFadeColors + 12 * layer;
  const uint32_t red = guest::LoadU32(base, color);
  for (uint32_t row = 0; row < 4; row++) {
    const uint32_t at = (rect + 16 * row) & ~0xFu;
    for (uint32_t i = 0; i < 4; i++) {
      guest::StoreU32(base, at + 4 * i, i == row ? 0x3F800000u : 0);
    }
  }
  const uint32_t green = guest::LoadU32(base, color + 4);
  const uint32_t blue = guest::LoadU32(base, color + 8);
  guest::StoreU32(base, rect + 64, 0);
  guest::StoreU32(base, rect + 68, 0);
  guest::StoreU32(base, rect + 80, 0);
  guest::StoreU32(base, rect + 84, 0);
  guest::StoreU32(base, rect + 72, 2);
  guest::StoreU32(base, rect + 96, red);
  guest::StoreU32(base, rect + 100, green);
  guest::StoreU32(base, rect + 104, blue);
  guest::StoreU32(base, rect + 88, 1280);
  guest::StoreU32(base, rect + 92, 720);
  guest::StoreU32(base, rect + 108, guest::LoadU32(base, kFadeAlphas + 4 * layer));
}

// A slot's three words (+0, +4, +8) in the order given.
struct SlotWrite {
  uint32_t offset, value;
};
void WriteSlot(uint8_t* base, uint32_t slot, std::initializer_list<SlotWrite> writes) {
  for (const SlotWrite& w : writes) {
    guest::StoreU32(base, slot + w.offset, w.value);
  }
}

// Whether `slots` more slots can be allocated on `list` without the
// original's messages: no chunk open (on it or another list), and room
// (the count not wrapping).
bool ChunkFits(uint8_t* base, uint32_t list, uint32_t slots) {
  if (guest::LoadU8(base, list + 52) != 0 || guest::LoadU32(base, command_list::kOpenChunks) != 0) {
    return false;
  }
  const uint32_t used = guest::LoadU32(base, list + 12);
  return used + slots >= used && used + slots < guest::LoadU32(base, list + 8);
}

}  // namespace

uint32_t ScreenFadesUpdate(uint8_t* base, uint32_t frame,
                           const std::function<void(uint32_t key, uint32_t rect)>& draw) {
  ITC_NATIVE_RUNNING("sub_8209D130 ScreenFades::Update");
  const uint32_t rect = frame + kFadeRect;
  for (uint32_t layer = 0; layer < 2; layer++) {
    if (layer == 1 || (guest::LoadU32(base, kFadeInCounts + 4) == 0 &&
                       guest::LoadU32(base, kFadeOutCounts + 4) == 0)) {
      StepFade(base, layer);
    }
    BuildFadeRect(base, rect, layer);
    if (int32_t(guest::LoadU32(base, kFadeAlphas + 4 * layer)) > 0) {
      draw(guest::LoadU32(base, kFadeKeys + 4 * layer), rect);
    }
  }
  if ((guest::LoadU32(base, kFadeOutCounts) | guest::LoadU32(base, kFadeInCounts)) != 0) {
    return 1;
  }
  return (guest::LoadU32(base, kFadeOutCounts + 4) | guest::LoadU32(base, kFadeInCounts + 4)) != 0
             ? 1
             : 0;
}

bool ScissorChunksFit(uint8_t* base, uint32_t list) { return ChunkFits(base, list, 8); }

uint32_t RecordScissorChunk(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8209CE68 ScreenFades::RecordScissor");
  command_list::OpenChunk(base, list);
  auto slot = [&] { return command_list::AllocateSlots(base, list, 1); };
  const uint32_t chunk = slot();
  guest::StoreU16(base, chunk + 2, 5);
  guest::StoreU32(base, chunk + 4, 0);
  WriteSlot(base, slot(), {{8, 0}, {0, 31}, {4, 1}});
  const uint32_t s = slot();
  guest::StoreU16(base, s + 0, 0);
  guest::StoreU16(base, s + 2, 0);
  guest::StoreU16(base, s + 4, 1280);
  guest::StoreU16(base, s + 6, 720);
  WriteSlot(base, slot(), {{4, 0}, {0, 36}, {8, 0}});
  command_list::CloseChunk(base, list);
  return chunk;
}

uint32_t RecordScissorEndChunk(uint8_t* base, uint32_t list) {
  command_list::OpenChunk(base, list);
  auto slot = [&] { return command_list::AllocateSlots(base, list, 1); };
  const uint32_t chunk = slot();
  guest::StoreU16(base, chunk + 2, 5);
  guest::StoreU32(base, chunk + 4, 0);
  WriteSlot(base, slot(), {{0, 1}, {4, 1}, {8, 0}});
  WriteSlot(base, slot(), {{4, 0}, {0, 200}});
  WriteSlot(base, slot(), {{0, 36}, {4, 0}, {8, 0}});
  command_list::CloseChunk(base, list);
  return chunk;
}

std::optional<uint32_t> ColorRectRecordFlat(uint8_t* base, uint32_t rect) {
  ITC_NATIVE_RUNNING("sub_8217A370 ColorRect::RecordFlat");
  const uint32_t list = guest::LoadU32(base, kColorRectLists + 4);
  const bool blend = int32_t(guest::LoadU32(base, rect + 108)) > 0;
  // The header, the blend states (6 or 4), 13, 6, 5, the vertices (6), 15,
  // a slot left as it is and 36.
  if (!ChunkFits(base, list, blend ? 19 : 17)) {
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  auto slot = [&] { return command_list::AllocateSlots(base, list, 1); };
  const uint32_t chunk = slot();
  guest::StoreU16(base, chunk + 2, 5);
  guest::StoreU32(base, chunk + 4, 0);
  uint32_t last;
  if (blend) {
    WriteSlot(base, slot(), {{4, 5}, {0, 1}, {8, 0}});
    WriteSlot(base, slot(), {{0, 56}, {4, 6}});
    WriteSlot(base, slot(), {{4, 0}, {0, 96}});
    WriteSlot(base, slot(), {{4, 1}, {0, 60}});
    WriteSlot(base, slot(), {{4, 6}, {0, 72}});
    last = slot();
    WriteSlot(base, last, {{4, 7}, {0, 76}});
  } else {
    WriteSlot(base, slot(), {{8, 0}, {4, 3}, {0, 1}});
    WriteSlot(base, slot(), {{0, 56}, {4, 6}});
    WriteSlot(base, slot(), {{4, 0}, {0, 96}});
    last = slot();
    WriteSlot(base, last, {{4, 0}, {0, 60}});
  }
  WriteSlot(base, slot(), {{4, 1}, {0, 13}, {8, 0}});
  WriteSlot(base, slot(), {{0, 6}, {4, 1}, {8, 0}});
  WriteSlot(base, slot(), {{0, 5}, {4, 4}, {8, 0}});
  uint32_t vertex = command_list::AllocateVertices(base, list, 4, false);
  const uint32_t z = guest::LoadU32(base, 0x82000A30);  // 1.0
  const int32_t x = int32_t(guest::LoadU32(base, rect + 80));
  const int32_t y = int32_t(guest::LoadU32(base, rect + 84));
  const int32_t right = int32_t(guest::LoadU32(base, rect + 88) + uint32_t(x));
  const int32_t bottom = int32_t(guest::LoadU32(base, rect + 92) + uint32_t(y));
  const float corners[4][2] = {{float(x), float(y)},
                               {float(right), float(y)},
                               {float(x), float(bottom)},
                               {float(right), float(bottom)}};
  for (const auto& corner : corners) {
    guest::StoreFloat(base, vertex + 0, corner[0]);
    guest::StoreFloat(base, vertex + 4, corner[1]);
    guest::StoreU32(base, vertex + 8, z);
    const uint32_t color = (guest::LoadU32(base, rect + 108) << 24) |
                           (uint32_t(guest::LoadU8(base, rect + 99)) << 16) |
                           (uint32_t(guest::LoadU8(base, rect + 103)) << 8) |
                           guest::LoadU8(base, rect + 107);
    guest::StoreU32(base, vertex + 12, color);
    vertex += 24;
  }
  WriteSlot(base, slot(), {{4, 1}, {0, 15}, {8, 0}});
  slot();
  WriteSlot(base, slot(), {{4, 0}, {0, 36}, {8, 0}});
  command_list::CloseChunk(base, list);
  return chunk;
}

MusicIds MusicPlay(uint8_t* base, uint32_t pcr, uint32_t frame, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_821063B0 Music::Play");
  // The names (bgm_tmp_01 to bgm_tmp_17 and BGM_SYSTEM, in the image) in the
  // order the original's table holds them, then its terminator.
  static constexpr uint32_t kNames[19] = {
      0x82006AD8, 0x82006ACC, 0x82006AC0, 0x82006AB4, 0x82006AA8, 0x82006A9C, 0x82006A90,
      0x82006A84, 0x82006A78, 0x82006AE4, 0x82006A6C, 0x82006A60, 0x82006A54, 0x82006A48,
      0x82006A3C, 0x82006A30, 0x82006A24, 0x82006A18, 0};
  // The table's stores as the original's: +132 first, then the rest.
  guest::StoreU32(base, frame + 132, kNames[9]);
  for (uint32_t i = 0; i < 19; i++) {
    if (i != 9) {
      guest::StoreU32(base, frame + 96 + 4 * i, kNames[i]);
    }
  }
  uint32_t index = 0;
  while (uint32_t(helpers::Lstrcmp(base, pcr, name, kNames[index])) != 0) {
    index++;
    if (guest::LoadU32(base, frame + 96 + 4 * index) == 0) {
      index = 99;
      break;
    }
  }
  if (index == 99) {
    guest::StoreU32(base, frame + 80, 22887);
    guest::StoreU32(base, frame + 84, 22888);
    return {frame + 84, frame + 80};
  }
  guest::StoreU32(base, frame + 84, 22972 + index);
  guest::StoreU32(base, frame + 80, 22990 + index);
  return {frame + 80, frame + 84};
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

bool SignInWatchCheckIdleTraps(uint8_t* base) {
  if (guest::LoadU32(base, 0x826C7824) == 0) {
    return false;
  }
  const uint32_t begin = guest::LoadU32(base, 0x826C7308 + 4);
  const uint32_t end = guest::LoadU32(base, 0x826C7308 + 8);
  return begin > end || (end - begin) % 384 != 0;
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
    if (SignInWatchCheckIdleTraps(base)) {
      return false;
    }
    session = guest::LoadU32(base, 0x826C7308 + 4) != guest::LoadU32(base, 0x826C7308 + 8);
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

namespace {

// The player's name (player 0's slot among the signed-in users) copied to
// `to` (wcscpy), or 32 bytes of zeros when not signed in (the frame's are
// four doubleword stores, the last name's a memset: the same bytes).
void CopyPlayerName(uint8_t* base, uint32_t to) {
  if (PlayersSignedInSlot(base, 0) >= 0) {
    const uint32_t slot = uint32_t(PlayersSignedInSlot(base, 0));
    c_runtime::Wcscpy(base, to, kUserNames + (slot << 5));
  } else {
    for (uint32_t i = 0; i < 32; i += 4) {
      guest::StoreU32(base, to + i, 0);
    }
  }
}

}  // namespace

bool SignInWatchUpdate(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8209EEC8 SignInWatch::Update");
  const uint32_t state = guest::LoadU32(base, kWatchState);
  if ((state >= 1 && state <= 9) || (state == 0 && SignInWatchCheckIdleTraps(base))) {
    return false;
  }
  const uint32_t shift = guest::LoadU32(base, kPlayerUsers) & 0x3F;
  const uint32_t user = shift >= 32 ? 0 : 1u << shift;
  const uint32_t name = frame + 80;
  CopyPlayerName(base, name);
  const uint32_t last_mask = guest::LoadU32(base, kLastMask);
  const bool was_in = (last_mask & user) == user;
  if (!was_in && last_mask != guest::LoadU32(base, kSignedInMask)) {
    if (guest::LoadU32(base, kLastCount) > guest::LoadU32(base, kSignedInCount)) {
      guest::StoreU32(base, kUserSignedOut, 1);
    } else {
      guest::StoreU32(base, kOtherUserSignedIn, 1);
    }
  }
  if (was_in && ((user & guest::LoadU32(base, kSignedInMask)) == 0 ||
                 c_runtime::Wcscmp(base, kLastName, name) != 0)) {
    guest::StoreU32(base, kUserSignedOut, 1);
  }
  if (int32_t(guest::LoadU32(base, kChecks)) == 1) {
    guest::StoreU32(base, kOtherUserSignedIn, 0);
  }
  if (int32_t(guest::LoadU32(base, kChecks + 4)) == 1) {
    guest::StoreU32(base, kUserSignedOut, 0);
  }
  if (state == 0) {
    SignInWatchCheckIdle(base);
  }
  // States 3 to 9 run sub_822910F0 here: CheckIdle sets 1 at most, and a
  // state above 9 isn't among them.
  guest::StoreU32(base, kLastCount, guest::LoadU32(base, kSignedInCount));
  guest::StoreU32(base, kLastMask, guest::LoadU32(base, kSignedInMask));
  CopyPlayerName(base, kLastName);
  return true;
}

int32_t SignInWatchStaticInit(uint8_t* base, const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D9518 SignInWatch::StaticInit");
  guest::StoreU32(base, 0x826BAB3C, 0);
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(base, 0x826BAB40 + 4 * i, 0);
  }
  guest::StoreU32(base, 0x826BAB70, 0);
  // memset (sub_825A71D0): the same bytes.
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU8(base, 0x826BAB1C + i, 0);
  }
  return c_runtime::AtexitFrom(base, 0x825DAF08, call);
}

}  // namespace mode_tick
