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

namespace {

// The upper 20 bits moved by `by`, the low 12 kept.
uint32_t Moved(uint32_t word, uint32_t by) {
  return (((word & 0xFFFFF000u) + by) & 0xFFFFF000u) | (word & 0xFFFu);
}

}  // namespace

uint32_t RelocateTexture(uint8_t* base, uint32_t header, uint32_t base_address,
                         uint32_t mip_address) {
  ITC_NATIVE_RUNNING("sub_8235A990 TextureHeader::Relocate");
  Store(base, header + 0x20, Moved(Load(base, header + 0x20), base_address));
  const uint32_t levels = ((Load(base, header + 44) >> 6) & 0xF) + 1;
  if (levels > 1) {
    Store(base, header + 0x30, Moved(Load(base, header + 0x30), mip_address));
  }
  return levels;
}

}  // namespace models
