#include "race_hud/race_hud.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace race_hud {

HudMovie RunMovie(uint8_t* base, uint32_t hud) {
  ITC_NATIVE_RUNNING("sub_822141E8 RaceHud::RunMovie");
  switch (guest::LoadU32(base, hud + 448)) {
    case 0:
      return {guest::LoadU32(base, hud + 456) != 0 ? HudCall::kRun : HudCall::kDraw,
              guest::LoadU32(base, hud + 604)};
    case 1:
      return {HudCall::kRunTimer, guest::LoadU32(base, hud + 624)};
    case 2:
    case 4:
      return {HudCall::kRun, guest::LoadU32(base, hud + 632)};
    case 3:
      return {HudCall::kState3, 0};
    default:
      return {};
  }
}

std::optional<uint32_t> RivalLogoMovie(uint8_t* base, uint32_t entry, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82211A28 RivalLogo::Draw");
  if ((guest::LoadU32(base, entry + 4) & 8) == 0) {
    return std::nullopt;
  }
  guest::StoreU32(base, frame + 80, guest::LoadU32(base, entry + 8));
  return guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

void RivalLogoFinished(uint8_t* base, uint32_t entry) {
  guest::StoreU32(base, entry + 4, guest::LoadU32(base, entry + 4) | 16);
}

}  // namespace race_hud
