// The game's container files (game_data/data_file.h).

#include "game_data/data_file.h"

#include "base/log.h"
#include "guest.h"
#include "schema/files_and_saves.h"

#include <atomic>

namespace game_data {

namespace {

uint32_t LoadLittle(uint8_t* base, uint32_t address) {
  return __builtin_bswap32(guest::LoadU32(base, address));
}

}  // namespace

uint32_t GetEntry(uint8_t* base, uint32_t container, uint32_t index, bool skip_empty) {
  ITC_NATIVE_RUNNING("sub_82294730 DataFile::GetEntry");
  const schema::DataFile file(base, container);
  if (index >= LoadLittle(base, file.count_address())) {
    return 0;
  }
  const uint32_t entry = container + LoadLittle(base, file.offsets_address() + 4 * index);
  if (skip_empty && guest::LoadU32(base, entry) == 0) {
    return 0;
  }
  return entry;
}

}  // namespace game_data
