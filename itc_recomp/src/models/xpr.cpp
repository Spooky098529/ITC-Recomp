// The XPR object (see xpr.h), store for store as the recompiled functions.

#include "models/xpr.h"

#include "base/log.h"
#include "guest.h"
#include "graphics/resources.h"
#include "models/memory_blocks.h"
#include "native/buffers.h"

#include <cstdlib>
#include <cstring>
#include <string>

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
    ITC_LOG_ERROR("XPR: {} refused after its checks", what);
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

uint32_t RelocateResource(uint8_t* base, uint32_t resource, uint32_t address) {
  ITC_NATIVE_RUNNING("sub_8235A9F8 Resource::Relocate");
  const uint32_t type = graphics::ResourceType(base, resource);
  const int32_t kind = int32_t(type);
  if (kind == 1) {
    const uint32_t word = Load(base, resource + 24);
    Store(base, resource + 24, (((word & ~3u) + address) & ~3u) | (word & 3u));
  } else if (kind == 2 || kind == 8) {
    Store(base, resource + 24, Load(base, resource + 24) + address);
  } else if (kind == 3 || (kind > 8 && type - 17 <= 3)) {
    const uint32_t mip = (Load(base, resource + 48) & 0xFFFFF000u) != 0 ? address : 0;
    return RelocateTexture(base, resource, address, mip);
  }
  return type;
}

namespace {

constexpr uint32_t kXpr2 = 0x58505232;      // 'XPR2'
constexpr uint32_t kTextureTag = 0x54580000;  // 'TX'
constexpr uint32_t kNotXpr2 = 0x80004005;

// The message without its line end.
std::string TrimmedMessage(const char* host) {
  std::string text(host, strnlen(host, 256));
  while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
    text.pop_back();
  }
  return text;
}

void DebugMessage(uint8_t* base, uint32_t text) {
  const char* host = reinterpret_cast<const char*>(REX_RAW_ADDR(text));
  ITC_LOG_WARN("XPR::Create: the game's debug message: {}", TrimmedMessage(host));
}

}  // namespace

std::optional<uint32_t> XprCreate(uint8_t* base, uint32_t xpr, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_82596F68 XPR::Create");
  if (!native::Buffers_NativeMemcpy()) {
    return std::nullopt;
  }
  if (Load(base, file) != kXpr2) {
    DebugMessage(base, 0x8206CE20);
    return kNotXpr2;
  }
  const uint32_t contents = file + 12;
  Store(base, xpr + 4, Load(base, file + 4));
  const uint32_t data_bytes = Load(base, file + 8);
  const uint32_t header_bytes = Load(base, xpr + 4);
  Store(base, xpr + 12, data_bytes);
  Store(base, xpr + 0, AllocVirtual(base, header_bytes));
  const uint32_t data = AllocPhysical(base, Load(base, xpr + 12), kXprDataAllocation);
  Store(base, xpr + 8, data);
  if (data == 0) {
    DebugMessage(base, 0x8206CDFC);
  }
  Require(native::Buffers_Memcpy(base, Load(base, xpr + 0), contents, Load(base, xpr + 4),
                                 0x82597000),
          "memcpy");
  Require(native::Buffers_Memcpy(base, Load(base, xpr + 8), Load(base, xpr + 4) + contents,
                                 Load(base, xpr + 12), 0x82597014),
          "memcpy");
  const uint32_t header = Load(base, xpr + 0);
  Store(base, xpr + 20, Load(base, header));
  Store(base, xpr + 16, header + 4);
  for (uint32_t i = 0; i < Load(base, xpr + 20); i++) {
    uint32_t record = Load(base, xpr + 16) + 16 * i;
    Store(base, record + 12, Load(base, record + 12) + Load(base, xpr + 0));
    record = Load(base, xpr + 16) + 16 * i;
    if ((Load(base, record) & 0xFFFF0000u) == kTextureTag) {
      const uint32_t to = Load(base, xpr + 8);
      RelocateTexture(base, Load(base, record + 4) + Load(base, xpr + 0), to, to);
    }
  }
  return 0;
}

}  // namespace models
