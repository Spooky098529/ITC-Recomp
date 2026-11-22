#include "race_hud/race_hud.h"

#include "base/log.h"
#include "guest.h"
#include "schema/camera_modes.h"
#include "schema/race_hud_object.h"
#include "ui_movies/game.h"

namespace race_hud {

HudMovie RunMovie(uint8_t* base, uint32_t hud_address) {
  ITC_NATIVE_RUNNING("sub_822141E8 RaceHud::RunMovie");
  const schema::RaceHud hud(base, hud_address);
  switch (hud.mode()) {
    case 0:
      return {hud.any_bar_low() != 0 ? HudCall::kRun : HudCall::kDraw,
              hud.racers_movie().address};
    case 1:
      return {HudCall::kRunTimer, hud.timer().address};
    case 2:
    case 4:
      return {HudCall::kRun, hud.splits().address};
    case 3:
      return {HudCall::kState3, 0};
    default:
      return {};
  }
}

std::optional<uint32_t> RivalLogoMovie(uint8_t* base, uint32_t entry_address, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82211A28 RivalLogo::Draw");
  const schema::RivalLogo entry(base, entry_address);
  if ((entry.flags() & 8) == 0) {
    return std::nullopt;
  }
  guest::StoreU32(base, frame + 80, entry.movie());
  return guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

void RivalLogoFinished(uint8_t* base, uint32_t entry_address) {
  const schema::RivalLogo entry(base, entry_address);
  entry.set_flags(entry.flags() | 16);
}

}  // namespace race_hud
