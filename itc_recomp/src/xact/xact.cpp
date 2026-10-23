#include "xact/xact.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "memory/c_runtime_heap.h"

#include <bit>
#include <tuple>
#include <utility>

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

// The functions the vtables' slots name, where the native callers run
// only while the slot is that function.
constexpr uint32_t kGetHeader = 0x825C7AA8;     // the sound bank's slot +64
constexpr uint32_t kGetEntry = 0x825C8398;      // the wave bank's slot +16
constexpr uint32_t kGetSeekTable = 0x825C8580;  // the wave bank's slot +20

// Whether the object's vtable slot at `offset` is `function`.
bool SlotIs(uint8_t* base, uint32_t object, uint32_t offset, uint32_t function) {
  return guest::LoadU32(base, guest::LoadU32(base, object) + offset) == function;
}

// Whether memcpy (sub_825A7410) copies these natively with nothing for its
// override to record: a destination outside the physical views (the copies
// it records for the renderer), each range in one view, the two not sharing
// memory.
bool CopyFits(uint32_t destination, uint32_t source, uint32_t size) {
  return destination < 0xA0000000u &&
         c_runtime::InOneView(destination, size) && c_runtime::InOneView(source, size) &&
         !c_runtime::MayShare(destination, size, source, size);
}

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

uint32_t SoundBankGetHeader(uint8_t* base, uint32_t bank) {
  ITC_NATIVE_RUNNING("sub_825C7AA8 XACTSoundBank::GetHeader");
  return guest::LoadU32(base, bank + 260);
}

std::optional<uint32_t> SoundBankHeader(uint8_t* base, uint32_t bank) {
  if (!SlotIs(base, bank, 64, kGetHeader)) {
    return std::nullopt;
  }
  return SoundBankGetHeader(base, bank);
}

std::optional<uint32_t> SoundBankGetSimpleCues(uint8_t* base, uint32_t bank) {
  const auto header = SoundBankHeader(base, bank);
  if (!header) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C7AB0 XACTSoundBank::GetSimpleCues");
  return guest::LoadU32(base, *header + 26) + *header;
}

std::optional<uint32_t> SoundBankGetWaveBankNames(uint8_t* base, uint32_t bank) {
  const auto header = SoundBankHeader(base, bank);
  if (!header) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C7B70 XACTSoundBank::GetWaveBankNames");
  return guest::LoadU32(base, *header + 50) + *header;
}

std::optional<uint32_t> SoundBankGetPointer(uint8_t* base, uint32_t bank, uint32_t offset) {
  const auto header = SoundBankHeader(base, bank);
  if (!header) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C7C90 XACTSoundBank::GetPointer");
  return *header + offset;
}

std::optional<uint32_t> SoundBankGetCueCount(uint8_t* base, uint32_t bank) {
  const auto header = SoundBankHeader(base, bank);
  if (!header) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C6928 XACTSoundBank::GetCueCount");
  return (guest::LoadU16(base, *header + 9) + guest::LoadU16(base, *header + 11)) & 0xFFFF;
}

std::optional<uint32_t> SoundBankFirstEvent(uint8_t* base, uint32_t bank, uint32_t iterator) {
  if (!SlotIs(base, bank, 64, kGetHeader)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C7F30 XACTSoundBank::FirstEvent");
  const uint32_t offset = guest::LoadU32(base, iterator + 0);
  guest::StoreU32(base, iterator + 4, offset + 1);
  const uint32_t header = SoundBankGetHeader(base, bank);
  const uint32_t cursor = guest::LoadU32(base, iterator + 4);
  const uint8_t count = guest::LoadU8(base, header + offset);
  guest::StoreU8(base, iterator + 9, 0);
  guest::StoreU8(base, iterator + 8, count);
  return SoundBankGetHeader(base, bank) + cursor;
}

std::optional<uint32_t> SoundBankNextEvent(uint8_t* base, uint32_t bank, uint32_t iterator) {
  if (!SlotIs(base, bank, 64, kGetHeader)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C7F98 XACTSoundBank::NextEvent");
  const uint32_t header = SoundBankGetHeader(base, bank);
  const uint32_t at = guest::LoadU32(base, iterator + 4);
  const uint32_t type = guest::LoadU32(base, header + at) & 0x1F;
  // The fixed sizes by type, and for the variable ones (a 16-bit count n
  // at +2 past the fixed part): the fixed part, and n's record size.
  static constexpr uint8_t kFixed[21] = {8,  12, 16, 0, 19, 23, 0,  22, 22, 11, 45,
                                         0,  42, 18, 13, 22, 26, 26, 15, 11, 0};
  if (type <= 20) {
    uint32_t cursor = 0;
    uint32_t fixed = 0, record = 0;
    switch (type) {
      case 3: fixed = 9; record = 5; break;
      case 6: fixed = 16; record = 5; break;
      case 11: fixed = 41; record = 6; break;
      case 20: fixed = 7; record = 6; break;
      default: break;
    }
    if (record == 0) {
      cursor = guest::LoadU32(base, iterator + 4) + kFixed[type];
    } else {
      const uint32_t moved = guest::LoadU32(base, iterator + 4) + fixed;
      guest::StoreU32(base, iterator + 4, moved);
      const uint32_t n = guest::LoadU16(base, SoundBankGetHeader(base, bank) + moved + 2);
      cursor = n * record + guest::LoadU32(base, iterator + 4) + 8;
    }
    guest::StoreU32(base, iterator + 4, cursor);
  }
  const uint8_t index = uint8_t(guest::LoadU8(base, iterator + 9) + 1);
  const uint8_t count = guest::LoadU8(base, iterator + 8);
  guest::StoreU8(base, iterator + 9, index);
  if (index == count) {
    return 0;
  }
  const uint32_t cursor = guest::LoadU32(base, iterator + 4);
  return SoundBankGetHeader(base, bank) + cursor;
}

std::optional<uint32_t> SoundBankGetCueRecord(uint8_t* base, uint32_t bank, uint32_t index) {
  const auto header = SoundBankHeader(base, bank);
  if (!header) {
    return std::nullopt;
  }
  const uint32_t i = index & 0xFFFF;
  if (i >= guest::LoadU16(base, *header + 9)) {
    return std::nullopt;  // a complex cue: sub_825C7B10 is still the original's
  }
  ITC_NATIVE_RUNNING("sub_825C8230 XACTSoundBank::GetCueRecord");
  return *SoundBankGetSimpleCues(base, bank) + 5 * i;
}

uint32_t WaveBankGetEntry(uint8_t* base, uint32_t bank, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825C8398 XACTWaveBank::GetEntry");
  return guest::LoadU32(base, bank + 272) + 24 * (index & 0xFFFF);
}

uint32_t WaveBankGetSeekTable(uint8_t* base, uint32_t bank, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_825C8580 XACTWaveBank::GetSeekTable");
  const uint32_t segment = guest::LoadU32(base, bank + 288);
  const uint32_t offset = guest::LoadU32(base, segment + 4 * (index & 0xFFFF));
  if (offset == 0xFFFFFFFFu) {
    return 0;
  }
  const uint32_t count = guest::LoadU32(base, guest::LoadU32(base, bank + 284) + 4);
  return (count << 2) + segment + offset;
}

uint32_t WaveBankGetName(uint8_t* base, uint32_t bank) {
  ITC_NATIVE_RUNNING("sub_825C85B8 XACTWaveBank::GetName");
  return guest::LoadU32(base, bank + 284) + 8;
}

uint32_t WaveBankGetEntryCount(uint8_t* base, uint32_t bank) {
  ITC_NATIVE_RUNNING("sub_825C85E8 XACTWaveBank::GetEntryCount");
  return guest::LoadU32(base, guest::LoadU32(base, bank + 284) + 4);
}

bool WaveBankGetWaveFormat(uint8_t* base, uint32_t bank, uint32_t index, uint32_t out) {
  if (!SlotIs(base, bank, 16, kGetEntry)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_825CC320 XACTWaveBank::GetWaveFormat");
  const uint32_t format = WaveBankGetEntry(base, bank, index) + 4;
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU8(base, out + i, guest::LoadU8(base, format + i));
  }
  return true;
}

void WaveEntryConstruct(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_825CED88 XACTWaveEntry::XACTWaveEntry");
  guest::StoreU32(base, entry, 0);
}

bool WaveEntrySet(uint8_t* base, uint32_t entry, uint32_t index, uint32_t bank) {
  if (!SlotIs(base, bank, 16, kGetEntry)) {
    return false;
  }
  // The entry's address doesn't depend on the stores before the copy
  // (GetEntry reads the bank's +272).
  const uint32_t source = guest::LoadU32(base, bank + 272) + 24 * (index & 0xFFFF);
  if (!CopyFits(entry + 12, source, 24) || c_runtime::MayShare(entry + 4, 8, bank + 272, 4)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_825CED98 XACTWaveEntry::Set");
  guest::StoreU16(base, entry + 4, uint16_t(index));
  guest::StoreU32(base, entry + 8, bank);
  const uint32_t from = WaveBankGetEntry(base, bank, index);
  c_runtime::Memcpy(base, entry + 12, from, 24);
  guest::StoreU32(base, entry + 0, 1);
  return true;
}

uint32_t WaveEntryGetLoopRegion(uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_825CEDF8 XACTWaveEntry::GetLoopRegion");
  return entry + 28;
}

uint32_t WaveEntryGetDuration(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_825CEE00 XACTWaveEntry::GetDuration");
  return guest::LoadU32(base, entry + 12) >> 4;
}

bool WaveEntrySamplesToMs(uint8_t* base, uint32_t entry, uint32_t samples, uint32_t* ms) {
  const uint32_t rate = (guest::LoadU32(base, entry + 16) >> 5) & 0x3FFFF;
  if (rate == 0) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_825CEE70 XACTWaveEntry::SamplesToMs");
  *ms = uint32_t(uint64_t(samples) * 1000 / rate);
  return true;
}

namespace {

// The bytes a block (XMA: 64 KB) or a sample frame (PCM: the sample size x
// the channels) by the format word; 0 for other tags.
uint32_t BlockBytes(uint32_t format) {
  switch (format & 3) {
    case 0: {
      const uint32_t bits = (format & 0x80000000u) ? 16 : 8;
      return (bits >> 3) * ((format >> 2) & 7);
    }
    case 1:
      return 0x10000;
    default:
      return 0;
  }
}

}  // namespace

namespace {

// FindXmaBlock's search: the block's byte offset and the samples into it,
// rounded to 128.
std::pair<uint32_t, uint32_t> XmaBlock(uint8_t* base, uint32_t entry, uint32_t position,
                                       uint32_t seek_table) {
  ITC_NATIVE_RUNNING("sub_825CEF50 XACTWaveEntry::FindXmaBlock");
  const uint32_t table = seek_table + 4;
  uint32_t high = guest::LoadU32(base, seek_table) - 1;
  uint32_t low = 1;
  uint32_t middle = 0;
  if (position >= guest::LoadU32(base, table)) {
    // A binary search for the block whose end is past the position and
    // whose start (the previous block's end) isn't.
    while (low <= high) {
      middle = (high + low) >> 1;
      const uint32_t at = table + (middle << 2);
      if (position >= guest::LoadU32(base, at - 4) && position < guest::LoadU32(base, at)) {
        break;
      }
      if (position < guest::LoadU32(base, at)) {
        high = middle - 1;
      } else {
        low = middle + 1;
      }
    }
  }
  const uint32_t offset = BlockBytes(guest::LoadU32(base, entry + 16)) * middle;
  if (middle != 0) {
    position -= guest::LoadU32(base, table + (middle << 2) - 4);
  }
  return {offset, position > 128 ? position & ~0x7Fu : (position + 127) & ~0x7Fu};
}

}  // namespace

void WaveEntryFindXmaBlock(uint8_t* base, uint32_t entry, uint32_t position, uint32_t seek_table,
                           uint32_t block_offset, uint32_t into) {
  guest::StoreU32(base, block_offset, 0);
  guest::StoreU32(base, into, position);
  const auto [offset, samples] = XmaBlock(base, entry, position, seek_table);
  guest::StoreU32(base, block_offset, offset);
  guest::StoreU32(base, into, samples);
}

std::optional<uint32_t> WaveEntrySamplesToBytes(uint8_t* base, uint32_t entry, uint32_t position,
                                                uint32_t out) {
  const uint32_t format = guest::LoadU32(base, entry + 16);
  uint32_t result = 0, into = 0;
  switch (format & 3) {
    case 0:
      result = BlockBytes(format) * position;
      break;
    case 1: {
      const uint32_t bank = guest::LoadU32(base, entry + 8);
      if (!SlotIs(base, bank, 20, kGetSeekTable)) {
        return std::nullopt;
      }
      const uint32_t seek = WaveBankGetSeekTable(base, bank, guest::LoadU16(base, entry + 4));
      // FindXmaBlock's two words go to the original's stack slots: host
      // locals here.
      std::tie(result, into) = XmaBlock(base, entry, position, seek);
      break;
    }
    default:
      break;
  }
  ITC_NATIVE_RUNNING("sub_825CF080 XACTWaveEntry::SamplesToBytes");
  if (out != 0) {
    guest::StoreU32(base, out, into);
  }
  return result;
}

}  // namespace xact
