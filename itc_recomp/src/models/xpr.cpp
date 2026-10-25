// The XPR object (see xpr.h), store for store as the recompiled functions.

#include "models/xpr.h"

#include "base/log.h"
#include "guest.h"
#include "models/memory_blocks.h"

#include <cstdlib>

namespace models {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// A put-off free refused after the checks.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("XPR::Release: {} refused after its checks", what);
    std::abort();
  }
}

}  // namespace

void XprConstruct(uint8_t* base, uint32_t xpr) {
  ITC_NATIVE_RUNNING("sub_82596E68 XPR::XPR");
  for (uint32_t offset = 0; offset < kXprBytes; offset += 4) {
    Store(base, xpr + offset, 0);
  }
}

std::optional<uint32_t> XprRelease(uint8_t* base, uint32_t xpr) {
  ITC_NATIVE_RUNNING("sub_825970A8 XPR::Release");
  // Room for both blocks' nodes, checked before anything changes.
  if (!PendingFreesHaveRoom(base, 2)) {
    return std::nullopt;
  }
  const uint32_t header = Load(base, xpr + kXprHeader);
  if (header != 0) {
    Require(FreeLater(base, header), "Memory::FreeLater");
  }
  const uint32_t data = Load(base, xpr + kXprData);
  Store(base, xpr + 0, 0);
  Store(base, xpr + 4, 0);
  uint32_t r3 = data;
  if (data != 0) {
    Require(FreePhysicalLater(base, data, kXprDataAttributes), "Memory::FreePhysicalLater");
    r3 = kMemoryBlocksLock;
  }
  for (uint32_t offset = 8; offset < kXprBytes; offset += 4) {
    Store(base, xpr + offset, 0);
  }
  return r3;
}

std::optional<uint32_t> XprDestruct(uint8_t* base, uint32_t xpr) {
  ITC_NATIVE_RUNNING("sub_82597148 XPR::~XPR");
  return XprRelease(base, xpr);
}

}  // namespace models
