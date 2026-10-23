#include "xact/xact.h"

#include "guest.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"

#include <bit>

namespace xact {

namespace {

constexpr uint32_t kOne = 0x82000A30;  // the game's float 1.0

constexpr uint32_t kSettingsVtable = 0x820753A8;
constexpr uint32_t kSettingsBaseVtable = 0x82075818;
constexpr uint32_t kFaderVtable = 0x8207600C;

constexpr uint32_t kPoolAttributes = 0x20840001;  // XMemAlloc's for the pools' chunks
constexpr uint32_t kChunkBytes = 196;             // a next-chunk word, 16 nodes
constexpr uint32_t kNodesPerChunk = 16;
constexpr uint32_t kNodeBytes = 12;

// A float moved as lfs and stfs move it (single to double and back).
void CopyFloat(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreFloat(base, to, float(double(guest::LoadFloat(base, from))));
}

}  // namespace

bool HashName(uint8_t* base, uint32_t name, uint32_t table_size, uint32_t* hash) {
  ITC_NATIVE_RUNNING("sub_825BFC50 XACTHashName");
  const uint32_t size = table_size & 0xFFFF;
  if (size == 0) {
    return false;
  }
  uint32_t h = 0;
  for (uint32_t at = name;; at++) {
    const uint8_t c = guest::LoadU8(base, at);
    if (c == 0) {
      break;
    }
    const uint32_t low = h & 0xFFFF;
    h = ((low + (low << 1)) & 0xFFFF) + ((h >> 1) & 0x7FFF) +
        (uint32_t(int32_t(int8_t(c))) & 0xFFFF);
  }
  *hash = (h & 0xFFFF) % size;
  return true;
}

void NodePoolConstruct(uint8_t* base, uint32_t pool) {
  ITC_NATIVE_RUNNING("sub_825C2060 XACTNodePool::XACTNodePool");
  const uint32_t chunk = pool + 8;
  guest::StoreU32(base, pool + 4, chunk);
  guest::StoreU32(base, chunk, 0);
  guest::StoreU32(base, pool, 0);
  // The inline chunk's nodes pushed on the free list in order.
  for (uint32_t i = 0; i < kNodesPerChunk; i++) {
    const uint32_t node = guest::LoadU32(base, pool + 4) + 4 + kNodeBytes * i;
    guest::StoreU32(base, node + 4, guest::LoadU32(base, pool));
    guest::StoreU32(base, pool, node);
  }
  guest::StoreU32(base, pool + 204, 0);
  guest::StoreU32(base, pool + 208, 0);
  guest::StoreU32(base, pool + 212, 0);
}

void NodePoolFree(uint8_t* base, uint32_t pool, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_825C2C60 XACTNodePool::Free");
  if (node == guest::LoadU32(base, pool + 204)) {
    guest::StoreU32(base, pool + 204, guest::LoadU32(base, node + 4));
  } else {
    guest::StoreU32(base, guest::LoadU32(base, node + 8) + 4, guest::LoadU32(base, node + 4));
  }
  if (node == guest::LoadU32(base, pool + 208)) {
    guest::StoreU32(base, pool + 208, guest::LoadU32(base, node + 8));
  } else {
    guest::StoreU32(base, guest::LoadU32(base, node + 4) + 8, guest::LoadU32(base, node + 8));
  }
  guest::StoreU32(base, node + 4, guest::LoadU32(base, pool));
  guest::StoreU32(base, pool, node);
  guest::StoreU32(base, pool + 212, guest::LoadU32(base, pool + 212) - 1);
}

uint32_t NodePoolAlloc(uint8_t* base, uint32_t pool) {
  ITC_NATIVE_RUNNING("sub_825C2CC8 XACTNodePool::Alloc");
  uint32_t node = guest::LoadU32(base, pool);
  if (node == 0) {
    const uint32_t chunk = memory::crt::XMemAlloc(base, kChunkBytes, kPoolAttributes);
    if (chunk != 0) {
      for (uint32_t i = 0; i < kNodesPerChunk; i++) {
        const uint32_t added = chunk + 4 + kNodeBytes * i;
        guest::StoreU32(base, added + 4, guest::LoadU32(base, pool));
        guest::StoreU32(base, pool, added);
      }
      guest::StoreU32(base, chunk, guest::LoadU32(base, pool + 4));
      guest::StoreU32(base, pool + 4, chunk);
    }
    node = guest::LoadU32(base, pool);
    if (node == 0) {
      return 0;
    }
  }
  guest::StoreU32(base, pool, guest::LoadU32(base, node + 4));
  return node;
}

uint32_t NodePoolFreeChunks(uint8_t* base, uint32_t pool) {
  ITC_NATIVE_RUNNING("sub_825C1F30 XACTNodePool::FreeChunks");
  uint32_t r3 = pool;
  if (guest::LoadU32(base, pool + 4) == 0) {
    return r3;
  }
  uint32_t next = 0;
  do {
    const uint32_t chunk = guest::LoadU32(base, pool + 4);
    next = guest::LoadU32(base, chunk);
    r3 = chunk;
    if (chunk != pool + 8) {
      r3 = memory::crt::XMemFree(chunk, kPoolAttributes);
    }
    guest::StoreU32(base, pool + 4, next);
  } while (next != 0);
  return r3;
}

void EngineAddCue(uint8_t* base, uint32_t engine, uint32_t cue) {
  ITC_NATIVE_RUNNING("sub_825C1518 XACTEngine::AddCue");
  const uint32_t node = cue != 0 ? cue + 12 : 0;
  const uint32_t list = engine + 28;
  guest::StoreU32(base, node + 4, list);
  guest::StoreU32(base, node + 8, guest::LoadU32(base, list + 8));
  guest::StoreU32(base, guest::LoadU32(base, list + 8) + 4, node);
  guest::StoreU32(base, list + 8, node);
}

void GlobalSettingsBaseConstruct(uint8_t* base, uint32_t settings, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_825C3508 XACTGlobalSettingsBase::XACTGlobalSettingsBase");
  guest::StoreU32(base, settings + 4, engine);
  guest::StoreU32(base, settings + 0, kSettingsBaseVtable);
  guest::StoreU32(base, settings + 8, 0);
  guest::StoreU16(base, settings + 100, 0);
  for (uint32_t offset = 104; offset <= 128; offset += 4) {
    guest::StoreU32(base, settings + offset, 0);
  }
}

void GlobalSettingsConstruct(uint8_t* base, uint32_t settings, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_825C21B0 XACTGlobalSettings::XACTGlobalSettings");
  GlobalSettingsBaseConstruct(base, settings, engine);
  guest::StoreU32(base, settings + 0, kSettingsVtable);
  for (uint32_t offset = 132; offset <= 144; offset += 4) {
    guest::StoreU32(base, settings + offset, 0);
  }
}

void VariableInit(uint8_t* base, uint32_t variable, uint32_t engine, uint32_t cue,
                  uint32_t index, uint32_t record, uint32_t r8) {
  ITC_NATIVE_RUNNING("sub_825C1DE0 XACTVariable::Init");
  guest::StoreU32(base, variable + 12, engine);
  guest::StoreU32(base, variable + 16, record);
  const uint32_t value = record + 1;  // the initial value (unaligned)
  const uint32_t bits = guest::LoadU32(base, value);
  guest::StoreU32(base, variable + 240, r8);
  guest::StoreU16(base, variable + 244, uint16_t(index));
  guest::StoreU32(base, variable + 248, cue);
  guest::StoreFloat(base, variable + 20, float(double(std::bit_cast<float>(bits))));
}

void CueDetachSound(uint8_t* base, uint32_t cue) {
  ITC_NATIVE_RUNNING("sub_825C4FC0 XACTCue::DetachSound");
  guest::StoreU32(base, cue + 52, 0);
  guest::StoreU32(base, cue + 40, 0);
}

uint32_t CueFindVariable(uint8_t* base, uint32_t cue, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825C5758 XACTCue::FindVariable");
  const uint32_t end = cue + 324;
  const uint16_t wanted = uint16_t(index);
  for (uint32_t v = guest::LoadU32(base, cue + 328); v != end; v = guest::LoadU32(base, v + 4)) {
    if (guest::LoadU16(base, v + 244) == wanted) {
      return v;
    }
  }
  return 0;
}

void FaderConstruct(uint8_t* base, uint32_t fader) {
  ITC_NATIVE_RUNNING("sub_825CC0A0 XACTFader::XACTFader");
  guest::StoreU32(base, fader + 0, kFaderVtable);
  CopyFloat(base, fader + 4, kOne);
  CopyFloat(base, fader + 20, kOne);
  guest::StoreU16(base, fader + 8, 0);
  guest::StoreU32(base, fader + 12, 0);
  guest::StoreU32(base, fader + 16, 0);
}

void FaderDestruct(uint8_t* base, uint32_t fader) {
  ITC_NATIVE_RUNNING("sub_825CC0D0 XACTFader::~XACTFader");
  guest::StoreU32(base, fader + 0, kFaderVtable);
}

}  // namespace xact
