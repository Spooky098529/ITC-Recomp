// The XPR object (see xpr.h), store for store as the recompiled functions.

#include "models/xpr.h"

#include "base/log.h"
#include "guest.h"
#include "graphics/resources.h"
#include "models/memory_blocks.h"
#include "native/buffers.h"
#include "schema/models.h"

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
  const schema::XPR x(base, xpr);
  x.set_header_block(0u);
  x.set_header_size(0u);
  x.set_data_block(0u);
  x.set_data_size(0u);
  x.set_records(0u);
  x.set_record_count(0u);
}

std::optional<uint32_t> XprRelease(uint8_t* base, uint32_t xpr) {
  ITC_NATIVE_RUNNING("sub_825970A8 XPR::Release");
  // Room for both blocks' nodes, checked before anything changes.
  if (!PendingFreesHaveRoom(base, 2)) {
    return std::nullopt;
  }
  const schema::XPR x(base, xpr);
  const uint32_t header = x.header_block();
  if (header != 0) {
    Require(FreeLater(base, header), "Memory::FreeLater");
  }
  const uint32_t data = x.data_block();
  x.set_header_block(0u);
  x.set_header_size(0u);
  uint32_t r3 = data;
  if (data != 0) {
    Require(FreePhysicalLater(base, data, kXprDataAttributes), "Memory::FreePhysicalLater");
    r3 = kMemoryBlocksLock;
  }
  x.set_data_block(0u);
  x.set_data_size(0u);
  x.set_records(0u);
  x.set_record_count(0u);
  return r3;
}

uint64_t XprGetTexture(uint8_t* base, uint32_t xpr, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82597118 XPR::GetTexture");
  const schema::XPR x(base, xpr);
  if (index >= x.record_count()) {
    index = 0;
  }
  const schema::XprRecord record(base, x.records().address + (index << 4));
  return uint64_t(record.header_offset()) + x.header_block();
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
  const char* host = reinterpret_cast<const char*>(guest::Pointer(base, text));
  ITC_LOG_WARN("XPR::Create: the game's debug message: {}", TrimmedMessage(host));
}

}  // namespace

std::optional<uint32_t> XprCreate(uint8_t* base, uint32_t xpr, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_82596F68 XPR::Create");
  if (!native::Buffers_NativeMemcpy()) {
    return std::nullopt;
  }
  const schema::XPR x(base, xpr);
  const schema::Xpr2File f(base, file);
  if (f.magic() != kXpr2) {
    DebugMessage(base, 0x8206CE20);
    return kNotXpr2;
  }
  const uint32_t contents = file + schema::Xpr2File::kSize;
  x.set_header_size(f.header_size());
  const uint32_t data_bytes = f.data_size();
  const uint32_t header_bytes = x.header_size();
  x.set_data_size(data_bytes);
  x.set_header_block(AllocVirtual(base, header_bytes));
  const uint32_t data = AllocPhysical(base, x.data_size(), kXprDataAllocation);
  x.set_data_block(data);
  if (data == 0) {
    DebugMessage(base, 0x8206CDFC);
  }
  Require(native::Buffers_Memcpy(base, x.header_block(), contents, x.header_size(), 0x82597000),
          "memcpy");
  Require(native::Buffers_Memcpy(base, x.data_block(), x.header_size() + contents,
                                 x.data_size(), 0x82597014),
          "memcpy");
  const uint32_t header = x.header_block();
  // The header block's +0: the record count (an XprRecord's +0 in the schema,
  // which joins the block with its records).
  x.set_record_count(schema::XprRecord(base, header).type());
  x.set_records(header + 4);
  for (uint32_t i = 0; i < x.record_count(); i++) {
    schema::XprRecord record(base, x.records().address + schema::XprRecord::kSize * i);
    record.set_name(record.name() + x.header_block());
    record = schema::XprRecord(base, x.records().address + schema::XprRecord::kSize * i);
    if ((record.type() & 0xFFFF0000u) == kTextureTag) {
      const uint32_t to = x.data_block();
      RelocateTexture(base, record.header_offset() + x.header_block(), to, to);
    }
  }
  return 0;
}

}  // namespace models
