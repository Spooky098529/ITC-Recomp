// The pause menu's data (PauseMenuData, race-screens.toml), in host memory
// since E5 step 3 (2026-11-22): its 40 fields are consecutive 32-bit words,
// which the original reaches by address (a field's address plus 4 x an
// index, and runs of words copied). These read and write the host struct's
// word at a byte offset, as those addresses did.

#pragma once

#include "schema/race_screens.h"

#include <cstdint>
#include <cstring>

namespace race_screens {

static_assert(sizeof(schema::PauseMenuDataData) == schema::PauseMenuData::kSize,
              "PauseMenuData's fields are its 40 words, no padding");

// The fields' byte offsets the original reaches by address.
inline constexpr uint32_t kPauseHudOption0 = 0;
inline constexpr uint32_t kPauseMusicVolumeSaved = 16;
inline constexpr uint32_t kPauseAdjustSaved0 = 24;
inline constexpr uint32_t kPauseHudEdit0 = 36;
inline constexpr uint32_t kPauseMusicVolume = 52;
inline constexpr uint32_t kPauseAdjust0 = 60;
inline constexpr uint32_t kPauseHudDefault0 = 72;
inline constexpr uint32_t kPauseMusicVolumeDefault = 88;
inline constexpr uint32_t kPauseAdjustDefault0 = 96;
inline constexpr uint32_t kPauseLastItemOk = 144;

inline uint32_t PauseWord(const schema::PauseMenuData& data, uint32_t offset) {
  uint32_t v;
  std::memcpy(&v, reinterpret_cast<const uint8_t*>(data.data()) + offset, 4);
  return v;
}

inline void SetPauseWord(const schema::PauseMenuData& data, uint32_t offset, uint32_t v) {
  std::memcpy(reinterpret_cast<uint8_t*>(data.data()) + offset, &v, 4);
}

// `count` words copied from `from` to `to` (byte offsets), one at a time.
inline void CopyPauseWords(const schema::PauseMenuData& data, uint32_t to, uint32_t from,
                           uint32_t count) {
  for (uint32_t k = 0; k < 4 * count; k += 4) {
    SetPauseWord(data, to + k, PauseWord(data, from + k));
  }
}

}  // namespace race_screens
