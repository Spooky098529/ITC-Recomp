// The game's container files (game_data/data_file.h).

#include "game_data/data_file.h"

#include "base/log.h"
#include "guest.h"
#include "schema/files_and_saves.h"

#include <atomic>

namespace game_data {

namespace {

uint32_t LoadLittle(uint32_t address) {
  return __builtin_bswap32(guest::LoadU32(address));
}

}  // namespace

uint32_t GetEntry(uint32_t container, uint32_t index, bool skip_empty) {
  ITC_NATIVE_RUNNING("sub_82294730 DataFile::GetEntry");
  const schema::DataFile file(container);
  if (index >= LoadLittle(file.count_address())) {
    return 0;
  }
  const uint32_t entry = container + LoadLittle(file.offsets_address() + 4 * index);
  if (skip_empty && guest::LoadU32(entry) == 0) {
    return 0;
  }
  return entry;
}

}  // namespace game_data
