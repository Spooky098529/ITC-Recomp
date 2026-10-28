// The archive streaming's functions as plain C++: see archive.h.

#include "archive/archive.h"

#include "base/log.h"
#include "guest.h"

namespace archive {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Toc(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8209D868 Archive::Toc");
  return LoadU32(base, id < kFirstCrsId ? kBuildToc : kCrsToc);
}

uint32_t AssetSize(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820A0678 Archive::AssetSize");
  const uint32_t index = id < kFirstCrsId ? id : id - kFirstCrsId;
  const uint32_t entry = Toc(base, id) + ((index + 1) << 4);
  const uint32_t unpacked = LoadU32(base, entry + 8);
  return unpacked != 0 ? unpacked : LoadU32(base, entry + 4);
}

}  // namespace archive
