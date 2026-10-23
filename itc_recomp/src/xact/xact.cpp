#include "xact/xact.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "memory/c_runtime_heap.h"

#include <bit>
#include <cstring>
#include <tuple>
#include <utility>

namespace xact {

namespace {

constexpr uint32_t kOne = 0x82000A30;   // the game's float 1.0
constexpr uint32_t kZero = 0x82000A34;  // the game's float 0.0

// The vtables the constructors write.
constexpr uint32_t kVariableVtable = 0x820753A4;
constexpr uint32_t kListHeadVtable = 0x82075344;
constexpr uint32_t kInterfaceBaseVtable = 0x82075860;
constexpr uint32_t kListenerBaseVtable = 0x8203DF00;

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
constexpr uint32_t kSoundBankAddListener = 0x825C7858;  // the bank's +4 interface's slot +0

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

uint32_t CueGetStateInternal(uint8_t* base, uint32_t cue) {
  ITC_NATIVE_RUNNING("sub_825C4D90 XACTCue::GetStateInternal");
  const uint32_t sound = guest::LoadU32(base, cue + 52);
  if (sound == 0) {
    return 0x20;  // XACT_CUESTATE_STOPPED
  }
  const uint32_t state = guest::LoadU32(base, sound + 80) - 1;
  uint32_t flags = state <= 5 ? 1u << state : 0;
  if (guest::LoadU8(base, sound + 52) & 0x20) {
    flags |= 0x40;  // XACT_CUESTATE_PAUSED
  }
  return flags;
}

namespace {

// ForgetWaveBank once its bank's slot +64 is known to be GetHeader.
uint32_t ForgetWaveBank(uint8_t* base, uint32_t bank, uint32_t wave_bank) {
  ITC_NATIVE_RUNNING("sub_825C72B0 XACTSoundBank::ForgetWaveBank");
  if (guest::LoadU32(base, bank + 244) == 0) {
    return bank;
  }
  const uint32_t header = SoundBankGetHeader(base, bank);
  const uint32_t count = guest::LoadU8(base, header + 17);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t array = guest::LoadU32(base, bank + 244);
    if (guest::LoadU32(base, array + 4 * i) == wave_bank) {
      guest::StoreU32(base, array + 4 * i, 0);
      break;
    }
  }
  return header;
}

}  // namespace

std::optional<uint32_t> SoundBankForgetWaveBank(uint8_t* base, uint32_t bank,
                                                uint32_t wave_bank) {
  if (guest::LoadU32(base, bank + 244) != 0 && !SlotIs(base, bank, 64, kGetHeader)) {
    return std::nullopt;
  }
  return ForgetWaveBank(base, bank, wave_bank);
}

std::optional<uint32_t> EngineForgetWaveBank(uint8_t* base, uint32_t engine, uint32_t wave_bank) {
  const uint32_t end = engine + 44;
  auto bank_of = [](uint32_t node) { return node != 0 ? node - 12 : 0; };
  for (uint32_t node = guest::LoadU32(base, engine + 48); node != end;
       node = guest::LoadU32(base, node + 4)) {
    const uint32_t bank = bank_of(node);
    if (guest::LoadU32(base, bank + 244) != 0 && !SlotIs(base, bank, 64, kGetHeader)) {
      return std::nullopt;
    }
  }
  ITC_NATIVE_RUNNING("sub_825C0408 XACTEngine::ForgetWaveBank");
  uint32_t r3 = engine;
  for (uint32_t node = guest::LoadU32(base, engine + 48); node != end;
       node = guest::LoadU32(base, node + 4)) {
    r3 = ForgetWaveBank(base, bank_of(node), wave_bank);
  }
  return r3;
}

bool SoundSetTrackParameter(uint8_t* base, uint32_t sound) {
  if (guest::LoadU32(base, sound + 40) != sound + 36) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_825CA980 XACTSound::SetTrackParameter");
  return true;
}

std::optional<uint32_t> FaderStep(uint8_t* base, uint32_t fader) {
  const uint32_t state = guest::LoadU32(base, fader + 16);
  if (state == 1 || state == 2) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825CC1D0 XACTFader::Step");
  return 0;
}

void VariableConstruct(uint8_t* base, uint32_t variable) {
  ITC_NATIVE_RUNNING("sub_825C20B0 XACTVariable::XACTVariable");
  CopyFloat(base, variable + 20, kZero);
  guest::StoreU32(base, variable + 4, variable);
  guest::StoreU32(base, variable + 8, variable);
  guest::StoreU32(base, variable + 0, kVariableVtable);
  guest::StoreU32(base, variable + 12, 0);
  guest::StoreU32(base, variable + 16, 0);
  NodePoolConstruct(base, variable + 24);
  guest::StoreU32(base, variable + 240, 0);
  guest::StoreU32(base, variable + 248, 0);
  guest::StoreU16(base, variable + 244, 0xFFFF);
}

uint32_t NodePoolPushBack(uint8_t* base, uint32_t pool, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_825C60C0 XACTNodePool::PushBack");
  const uint32_t node = NodePoolAlloc(base, pool);
  if (node == 0) {
    return 0;
  }
  guest::StoreU32(base, node + 0, value);
  guest::StoreU32(base, node + 4, 0);
  guest::StoreU32(base, node + 8, guest::LoadU32(base, pool + 208));
  const uint32_t tail = guest::LoadU32(base, pool + 208);
  if (tail != 0) {
    guest::StoreU32(base, tail + 4, node);
  } else {
    guest::StoreU32(base, pool + 204, node);
  }
  const uint32_t count = guest::LoadU32(base, pool + 212);
  guest::StoreU32(base, pool + 208, node);
  guest::StoreU32(base, pool + 212, count + 1);
  return node;
}

uint32_t CueAddListener(uint8_t* base, uint32_t self, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_825C6140 XACTCue::AddListener");
  return NodePoolPushBack(base, self + 72, listener);
}

uint32_t SoundBankAddListener(uint8_t* base, uint32_t self, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_825C7858 XACTSoundBank::AddListener");
  return NodePoolPushBack(base, self + 24, listener);
}

namespace {

// The listener lists' removal: the node holding `listener` in the used
// list whose head is at `head`, freed to `pool`. r3 as the originals leave
// it.
uint32_t RemoveListener(uint8_t* base, uint32_t self, uint32_t head, uint32_t pool,
                        uint32_t listener) {
  for (uint32_t node = guest::LoadU32(base, head); node != 0;
       node = guest::LoadU32(base, node + 4)) {
    if (guest::LoadU32(base, node) == listener) {
      NodePoolFree(base, pool, node);
      return pool;
    }
  }
  return self;
}

}  // namespace

uint32_t CueRemoveListener(uint8_t* base, uint32_t self, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_825C6040 XACTCue::RemoveListener");
  return RemoveListener(base, self, self + 276, self + 72, listener);
}

uint32_t SoundBankRemoveListener(uint8_t* base, uint32_t self, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_825C7758 XACTSoundBank::RemoveListener");
  return RemoveListener(base, self, self + 228, self + 24, listener);
}

void SoundBankBaseConstruct(uint8_t* base, uint32_t bank, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_825C76A0 XACTSoundBankBase::XACTSoundBankBase");
  const uint32_t node = bank + 12;
  guest::StoreU32(base, bank + 4, kInterfaceBaseVtable);
  guest::StoreU32(base, bank + 8, kListenerBaseVtable);
  guest::StoreU32(base, node + 4, node);
  guest::StoreU32(base, node + 0, kListHeadVtable);
  guest::StoreU32(base, node + 8, node);
  guest::StoreU32(base, bank + 0, 0x82075A30);
  guest::StoreU32(base, bank + 4, 0x82075A28);
  guest::StoreU32(base, bank + 8, 0x82075A24);
  guest::StoreU32(base, node + 0, 0x82075A84);
  guest::StoreU32(base, bank + 24, engine);
  NodePoolConstruct(base, bank + 28);
  guest::StoreU32(base, bank + 244, 0);
  guest::StoreU32(base, bank + 248, 0);
}

void SoundBankConstruct(uint8_t* base, uint32_t bank, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_825C79B8 XACTSoundBank::XACTSoundBank");
  SoundBankBaseConstruct(base, bank, engine);
  guest::StoreU32(base, bank + 0, 0x82075A90);
  guest::StoreU32(base, bank + 4, 0x82075A28);
  guest::StoreU32(base, bank + 8, 0x82075A24);
  guest::StoreU32(base, bank + 12, 0x82075A88);
  for (uint32_t offset = 252; offset <= 264; offset += 4) {
    guest::StoreU32(base, bank + offset, 0);
  }
}

void WaveBankRootConstruct(uint8_t* base, uint32_t bank, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_825CC620 XACTWaveBankRoot::XACTWaveBankRoot");
  const uint32_t node = bank + 8;
  guest::StoreU32(base, bank + 4, kInterfaceBaseVtable);
  guest::StoreU32(base, node + 0, kListHeadVtable);
  guest::StoreU32(base, node + 4, node);
  guest::StoreU32(base, node + 8, node);
  guest::StoreU32(base, bank + 0, 0x82076024);
  guest::StoreU32(base, bank + 4, 0x8207601C);
  guest::StoreU32(base, node + 0, 0x82076018);
  guest::StoreU32(base, bank + 20, engine);
  NodePoolConstruct(base, bank + 24);
  const uint32_t waves = bank + 240;
  guest::StoreU32(base, waves + 0, kListHeadVtable);
  guest::StoreU32(base, waves + 4, waves);
  guest::StoreU32(base, waves + 8, waves);
  guest::StoreU32(base, bank + 252, 0);
  guest::StoreU32(base, bank + 256, 0);
}

void WaveBankBaseConstruct(uint8_t* base, uint32_t bank, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_825CEBE0 XACTWaveBankBase::XACTWaveBankBase");
  WaveBankRootConstruct(base, bank, engine);
  guest::StoreU32(base, bank + 0, 0x82076140);
  guest::StoreU32(base, bank + 4, 0x8207601C);
  guest::StoreU32(base, bank + 8, 0x8207613C);
}

void WaveBankConstruct(uint8_t* base, uint32_t bank, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_825C8310 XACTWaveBank::XACTWaveBank");
  WaveBankBaseConstruct(base, bank, engine);
  guest::StoreU32(base, bank + 0, 0x82075AE8);
  guest::StoreU32(base, bank + 4, 0x8207601C);
  guest::StoreU32(base, bank + 8, 0x82075AE4);
  for (uint32_t offset = 260; offset <= 276; offset += 4) {
    guest::StoreU32(base, bank + offset, 0);
  }
  guest::StoreU32(base, bank + 288, 0);
}

bool CueBaseConstruct(uint8_t* base, uint32_t cue, uint32_t engine, uint32_t bank,
                      uint32_t record, uint32_t index) {
  // The bank's +4 interface's slot +0 (read before anything is written, as
  // the cue's stores can't reach the bank's vtables but for a cue built
  // over them).
  if (!SlotIs(base, bank + 4, 0, kSoundBankAddListener) ||
      c_runtime::MayShare(cue, 352, bank + 4, 4)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_825C5E58 XACTCueBase::XACTCueBase");
  const uint32_t listener = cue + 8;
  const uint32_t node = cue + 12;
  const uint32_t fader = cue + 24;
  guest::StoreU32(base, cue + 4, kInterfaceBaseVtable);
  guest::StoreU32(base, listener, kListenerBaseVtable);
  guest::StoreU32(base, node + 4, node);
  guest::StoreU32(base, node + 0, kListHeadVtable);
  guest::StoreU32(base, node + 8, node);
  FaderConstruct(base, fader);
  guest::StoreU32(base, cue + 48, record);
  guest::StoreU32(base, cue + 56, engine);
  guest::StoreU32(base, cue + 52, 0);
  guest::StoreU32(base, cue + 0, 0x82075878);
  guest::StoreU32(base, cue + 4, 0x8207586C);
  guest::StoreU32(base, listener, 0x820760AC);
  guest::StoreU32(base, node, 0x82075868);
  guest::StoreU32(base, fader, 0x8207609C);
  guest::StoreU32(base, cue + 60, 0);
  guest::StoreU32(base, cue + 64, 0);
  guest::StoreU16(base, cue + 68, 0);
  guest::StoreU32(base, cue + 72, 0);
  NodePoolConstruct(base, cue + 76);
  guest::StoreU32(base, cue + 296, bank);
  guest::StoreU32(base, cue + 292, 0);
  guest::StoreU32(base, cue + 300, 0);
  guest::StoreU16(base, cue + 304, uint16_t(index));
  guest::StoreU32(base, cue + 316, 0);
  guest::StoreU32(base, cue + 320, 0);
  const uint32_t variables = cue + 324;
  guest::StoreU32(base, variables + 0, kListHeadVtable);
  guest::StoreU32(base, variables + 4, variables);
  guest::StoreU32(base, variables + 8, variables);
  for (uint32_t offset : {336u, 348u, 308u, 312u, 340u, 344u}) {
    guest::StoreU32(base, cue + offset, 0);
  }
  SoundBankAddListener(base, guest::LoadU32(base, cue + 296) + 4, listener);
  return true;
}

bool CueConstruct(uint8_t* base, uint32_t cue, uint32_t engine, uint32_t bank, uint32_t record,
                  uint32_t index) {
  if (!CueBaseConstruct(base, cue, engine, bank, record, index)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_825C67E8 XACTCue::XACTCue");
  guest::StoreU32(base, cue + 0, 0x82075908);
  guest::StoreU32(base, cue + 4, 0x8207586C);
  guest::StoreU32(base, cue + 8, 0x820760AC);
  guest::StoreU32(base, cue + 12, 0x82075904);
  guest::StoreU32(base, cue + 24, 0x8207609C);
  return true;
}

std::optional<uint32_t> WaveEntryGetLoopStartBytes(uint8_t* base, uint32_t entry, uint32_t out) {
  const auto bytes = WaveEntrySamplesToBytes(base, entry, guest::LoadU32(base, entry + 28), out);
  if (bytes) {
    ITC_NATIVE_RUNNING("sub_825CF210 XACTWaveEntry::GetLoopStartBytes");
  }
  return bytes;
}

std::optional<uint32_t> WaveEntryGetLoopLengthBytes(uint8_t* base, uint32_t entry) {
  const uint32_t start = guest::LoadU32(base, entry + 28);
  auto end = WaveEntrySamplesToBytes(base, entry, guest::LoadU32(base, entry + 32) + start, 0);
  if (!end) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825CF288 XACTWaveEntry::GetLoopLengthBytes");
  uint32_t last = *end;
  if ((guest::LoadU32(base, entry + 16) & 3) == 1) {
    // XMA: one 64 KB block more, capped at the play length.
    last += 0x10000;
    const uint32_t play = guest::LoadU32(base, entry + 24);
    if (last > play) {
      last = play;
    }
  }
  return last - *WaveEntrySamplesToBytes(base, entry, guest::LoadU32(base, entry + 28), 0);
}

namespace {

constexpr uint32_t kOutOfMemory = 0x8007000E;  // E_OUTOFMEMORY

// memset (sub_825A71D0) as its adapter does it: the bytes set host part by
// host part.
void Zero(uint8_t* base, uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(base, address, bytes, [](uint8_t* host, uint32_t, uint32_t count) {
    std::memset(host, 0, count);
  });
}

// InitInstanceCounts once its bank's slot +64 is known to be GetHeader.
uint32_t InitInstanceCounts(uint8_t* base, uint32_t bank) {
  ITC_NATIVE_RUNNING("sub_825C7448 XACTSoundBank::InitInstanceCounts");
  if (*SoundBankGetCueCount(base, bank) == 0) {
    return 0;
  }
  const uint32_t bytes = (*SoundBankGetCueCount(base, bank) << 2) & 0x3FFFC;
  const uint32_t counts = memory::crt::XMemAlloc(base, bytes, kPoolAttributes);
  if (counts == 0) {
    return kOutOfMemory;
  }
  Zero(base, counts, (*SoundBankGetCueCount(base, bank) << 2) & 0x3FFFC);
  guest::StoreU32(base, bank + 248, counts);
  return 0;
}

}  // namespace

std::optional<uint32_t> SoundBankInitInstanceCounts(uint8_t* base, uint32_t bank) {
  if (!SlotIs(base, bank, 64, kGetHeader)) {
    return std::nullopt;
  }
  return InitInstanceCounts(base, bank);
}

std::optional<uint32_t> SoundBankLoad(uint8_t* base, uint32_t bank, uint32_t file, uint32_t size,
                                      uint32_t flags, uint32_t attributes) {
  if (!SlotIs(base, bank, 64, kGetHeader)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C7E40 XACTSoundBank::Load");
  guest::StoreU32(base, bank + 252, flags);
  guest::StoreU32(base, bank + 256, attributes);
  guest::StoreU32(base, bank + 264, size);
  guest::StoreU32(base, bank + 260, file);
  auto wave_banks = [&] { return guest::LoadU8(base, SoundBankGetHeader(base, bank) + 17); };
  if (wave_banks() != 0) {
    const uint32_t array = memory::crt::XMemAlloc(base, uint32_t(wave_banks()) << 2,
                                                  kPoolAttributes);
    guest::StoreU32(base, bank + 244, array);
    if (array == 0) {
      return kOutOfMemory;
    }
    for (uint32_t i = 0; i < wave_banks(); i++) {
      guest::StoreU32(base, guest::LoadU32(base, bank + 244) + 4 * i, 0);
    }
  }
  return InitInstanceCounts(base, bank);
}

// --- Batch 6: the global settings' accessors and their users ---

namespace {

// The global settings' slots (vtable 0x820753A8, read from the loaded
// image) the accessors call.
constexpr uint32_t kSettingsGetCategoryRecord = 0x825C23C8;  // +4
constexpr uint32_t kSettingsGetVariableRecord = 0x825C2428;  // +8
constexpr uint32_t kSettingsGetParent = 0x825C24E8;          // +24
constexpr uint32_t kSettingsGetVolumeByte = 0x825C2518;      // +28
constexpr uint32_t kSettingsGetCategoryFlag = 0x825C2618;    // +32
constexpr uint32_t kSettingsGetBuffer = 0x825C2200;          // +44
constexpr uint32_t kSettingsGetNameBuckets = 0x825C2208;     // +48
constexpr uint32_t kGainTable = 0x82075418;  // a category volume byte's gain

// Whether the settings' slots a function reaches are the native ones: the
// buffer (+44); a category's record (+4, and the buffer); its parent (+24,
// and the record).
bool BufferSlot(uint8_t* base, uint32_t settings) {
  return SlotIs(base, settings, 44, kSettingsGetBuffer);
}
bool RecordSlots(uint8_t* base, uint32_t settings) {
  return SlotIs(base, settings, 4, kSettingsGetCategoryRecord) && BufferSlot(base, settings);
}
bool ParentSlots(uint8_t* base, uint32_t settings) {
  return SlotIs(base, settings, 24, kSettingsGetParent) && RecordSlots(base, settings);
}

// The settings at the engine's +124 (0 for none).
uint32_t EngineSettings(uint8_t* base, uint32_t engine) {
  return guest::LoadU32(base, engine + 124);
}

// A category's record and parent once RecordSlots and ParentSlots hold.
uint32_t CategoryRecord(uint8_t* base, uint32_t settings, uint32_t index) {
  return *SettingsGetCategoryRecord(base, settings, index);
}
uint32_t Parent(uint8_t* base, uint32_t settings, uint32_t index) {
  return *SettingsGetParentCategory(base, settings, index);
}

// AddInstance and RemoveInstance once ParentSlots holds: `delta` added to
// the live instance count (+104) of the category and each parent. r3 as
// the original leaves it: the settings for none, else the last parent.
uint32_t AddToInstances(uint8_t* base, uint32_t settings, uint32_t index, uint32_t delta) {
  uint32_t r3 = settings;
  uint32_t c = index & 0xFFFF;
  while (c != 0xFFFF) {
    const uint32_t at = guest::LoadU32(base, settings + 104) + (c << 2);
    guest::StoreU32(base, at, guest::LoadU32(base, at) + delta);
    r3 = Parent(base, settings, c);
    c = r3 & 0xFFFF;
  }
  return r3;
}

}  // namespace

uint32_t SettingsGetBuffer(uint8_t* base, uint32_t settings) {
  ITC_NATIVE_RUNNING("sub_825C2200 XACTGlobalSettings::GetBuffer");
  return guest::LoadU32(base, settings + 132);
}

std::optional<uint32_t> SettingsGetNameBuckets(uint8_t* base, uint32_t settings) {
  if (!BufferSlot(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C2208 XACTGlobalSettings::GetNameBuckets");
  const uint32_t buffer = SettingsGetBuffer(base, settings);
  return guest::LoadU32(base, buffer + 31) + buffer;
}

std::optional<uint32_t> SettingsGetNameEntry(uint8_t* base, uint32_t settings, uint32_t index) {
  if (!BufferSlot(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C2268 XACTGlobalSettings::GetNameEntry");
  const uint32_t buffer = SettingsGetBuffer(base, settings);
  return 6 * (index & 0xFFFF) + guest::LoadU32(base, buffer + 35) + buffer;
}

std::optional<uint32_t> SettingsGetPointer(uint8_t* base, uint32_t settings, uint32_t offset) {
  if (!BufferSlot(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C22C8 XACTGlobalSettings::GetPointer");
  return SettingsGetBuffer(base, settings) + offset;
}

std::optional<uint32_t> SettingsGetCategoryRecord(uint8_t* base, uint32_t settings,
                                                  uint32_t index) {
  if (!BufferSlot(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C23C8 XACTGlobalSettings::GetCategoryRecord");
  const uint32_t buffer = SettingsGetBuffer(base, settings);
  return 10 * (index & 0xFFFF) + guest::LoadU32(base, buffer + 23) + buffer;
}

std::optional<uint32_t> SettingsGetVariableRecord(uint8_t* base, uint32_t settings,
                                                  uint32_t index) {
  if (!BufferSlot(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C2428 XACTGlobalSettings::GetVariableRecord");
  const uint32_t buffer = SettingsGetBuffer(base, settings);
  return 13 * (index & 0xFFFF) + guest::LoadU32(base, buffer + 27) + buffer;
}

std::optional<uint32_t> SettingsGetMaxInstances(uint8_t* base, uint32_t settings,
                                                uint32_t index) {
  if (!RecordSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C2480 XACTGlobalSettings::GetMaxInstances");
  return guest::LoadU8(base, CategoryRecord(base, settings, index));
}

std::optional<uint32_t> SettingsGetInstanceBehavior(uint8_t* base, uint32_t settings,
                                                    uint32_t index) {
  if (!RecordSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C24B0 XACTGlobalSettings::GetInstanceBehavior");
  return (guest::LoadU8(base, CategoryRecord(base, settings, index) + 5) >> 3) & 7;
}

std::optional<uint32_t> SettingsGetParentCategory(uint8_t* base, uint32_t settings,
                                                  uint32_t index) {
  if (!RecordSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C24E8 XACTGlobalSettings::GetParentCategory");
  return guest::LoadU16(base, CategoryRecord(base, settings, index) + 6);
}

std::optional<uint32_t> SettingsGetVolumeByte(uint8_t* base, uint32_t settings, uint32_t index) {
  if (!RecordSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C2518 XACTGlobalSettings::GetVolumeByte");
  return guest::LoadU8(base, CategoryRecord(base, settings, index) + 8);
}

std::optional<uint32_t> SettingsGetNameEntryIndex(uint8_t* base, uint32_t settings,
                                                  uint32_t entry) {
  if (!SlotIs(base, settings, 48, kSettingsGetNameBuckets) || !BufferSlot(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C2548 XACTGlobalSettings::GetNameEntryIndex");
  const uint32_t buckets = *SettingsGetNameBuckets(base, settings);
  const uint32_t count = guest::LoadU16(base, SettingsGetBuffer(base, settings) + 13);
  const uint32_t bytes = entry - (count << 1) - buckets;
  // The division by 6 as the original's multiply.
  return (uint32_t((uint64_t(bytes) * 0xAAAAAAABu) >> 32) >> 2) & 0xFFFF;
}

std::optional<uint32_t> SettingsGetCategoryFlag(uint8_t* base, uint32_t settings,
                                                uint32_t index) {
  if (!RecordSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C2618 XACTGlobalSettings::GetCategoryFlag");
  return guest::LoadU8(base, CategoryRecord(base, settings, index) + 9) & 1;
}

std::optional<uint32_t> EngineGetCategoryFlag(uint8_t* base, uint32_t engine, uint32_t index) {
  const uint32_t settings = EngineSettings(base, engine);
  if (settings != 0 &&
      (!SlotIs(base, settings, 32, kSettingsGetCategoryFlag) || !RecordSlots(base, settings))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C0708 XACTEngine::GetCategoryFlag");
  return settings == 0 ? 0 : *SettingsGetCategoryFlag(base, settings, index);
}

std::optional<uint32_t> SettingsIsInCategory(uint8_t* base, uint32_t settings, uint32_t category,
                                             uint32_t ancestor) {
  if (!ParentSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C3548 XACTGlobalSettings::IsInCategory");
  for (uint32_t c = category & 0xFFFF; c != 0xFFFF; c = Parent(base, settings, c) & 0xFFFF) {
    if (c == (ancestor & 0xFFFF)) {
      return 1;
    }
  }
  return 0;
}

std::optional<uint32_t> SettingsAddInstance(uint8_t* base, uint32_t settings, uint32_t index) {
  if (!ParentSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C3F90 XACTGlobalSettings::AddInstance");
  return AddToInstances(base, settings, index, 1);
}

std::optional<uint32_t> SettingsRemoveInstance(uint8_t* base, uint32_t settings, uint32_t index) {
  if (!ParentSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C4000 XACTGlobalSettings::RemoveInstance");
  return AddToInstances(base, settings, index, 0xFFFFFFFFu);
}

std::optional<uint32_t> EngineAddInstance(uint8_t* base, uint32_t engine, uint32_t index) {
  const uint32_t settings = EngineSettings(base, engine);
  if (settings != 0 && !ParentSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C0598 XACTEngine::AddInstance");
  return settings == 0 ? 0 : *SettingsAddInstance(base, settings, index);
}

std::optional<uint32_t> EngineRemoveInstance(uint8_t* base, uint32_t engine, uint32_t index) {
  const uint32_t settings = EngineSettings(base, engine);
  if (settings != 0 && !ParentSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C05B0 XACTEngine::RemoveInstance");
  return settings == 0 ? 0 : *SettingsRemoveInstance(base, settings, index);
}

std::optional<uint32_t> SettingsSetCategoryVolume(uint8_t* base, uint32_t settings,
                                                  uint32_t index, double volume) {
  if (!SlotIs(base, settings, 28, kSettingsGetVolumeByte) || !RecordSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C4070 XACTGlobalSettings::SetCategoryVolume");
  const uint32_t byte = *SettingsGetVolumeByte(base, settings, index);
  const float gain = guest::LoadFloat(base, kGainTable + ((byte << 2) & 0x3FC));
  guest::StoreFloat(base, guest::LoadU32(base, settings + 120) + ((index << 2) & 0x3FFFC),
                    float(double(gain) * volume));
  return byte;
}

std::optional<uint32_t> EngineGetVariableRecord(uint8_t* base, uint32_t engine, uint32_t index) {
  const uint32_t settings = EngineSettings(base, engine);
  if (settings != 0 && (!BufferSlot(base, settings) ||
                        !SlotIs(base, settings, 8, kSettingsGetVariableRecord))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C0748 XACTEngine::GetVariableRecord");
  if (settings == 0 ||
      (index & 0xFFFF) >= guest::LoadU16(base, SettingsGetBuffer(base, settings) + 11)) {
    return 0;
  }
  return SettingsGetVariableRecord(base, settings, index);
}

std::optional<CategoryVolume> EngineGetCategoryVolume(uint8_t* base, uint32_t engine,
                                                      uint32_t index) {
  const uint32_t settings = EngineSettings(base, engine);
  if (settings != 0 && !ParentSlots(base, settings)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C04D8 XACTEngine::GetCategoryVolume");
  double volume = double(guest::LoadFloat(base, kOne));
  if (settings == 0) {
    return CategoryVolume{volume, engine};
  }
  const uint32_t c = index & 0xFFFF;
  const auto word = [&](uint32_t field) {
    return guest::LoadU32(base, guest::LoadU32(base, settings + field) + (c << 2));
  };
  if (word(112) != 0 || (guest::LoadU32(base, settings + 116) != 0 && word(108) == 0)) {
    return CategoryVolume{double(guest::LoadFloat(base, kZero)), engine};
  }
  uint32_t r3 = engine;
  for (uint32_t at = c; at != 0xFFFF; at = r3 & 0xFFFF) {
    const float gain =
        guest::LoadFloat(base, guest::LoadU32(base, settings + 120) + (at << 2));
    volume = double(float(double(gain) * volume));
    r3 = Parent(base, settings, at);
  }
  return CategoryVolume{volume, r3};
}

// --- Batch 7: the wave entries' leaves, the wave banks' listeners and the
// sound banks' wave bank lookup, the variable's destructor ---

uint32_t WaveEntryHasLoopRegion(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_825CEE10 XACTWaveEntry::HasLoopRegion");
  return guest::LoadU32(base, entry + 32) != 0 ? 1 : 0;
}

uint32_t WaveEntryGetFormatTag(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_825CEE28 XACTWaveEntry::GetFormatTag");
  const uint32_t tag = guest::LoadU32(base, entry + 16) & 3;
  return tag < 1 ? 0 : tag == 1 ? 1 : 3;
}

uint32_t WaveEntryMsToSamples(uint8_t* base, uint32_t entry, uint32_t ms) {
  ITC_NATIVE_RUNNING("sub_825CEEA0 XACTWaveEntry::MsToSamples");
  const uint64_t rate = (guest::LoadU32(base, entry + 16) >> 5) & 0x3FFFF;
  return uint32_t(rate * ms / 1000);
}

uint32_t WaveEntryGetPlayLength(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82099BA0 XACTWaveEntry::GetPlayLength");
  return guest::LoadU32(base, entry + 24);
}

uint32_t WaveEntryGetLoopStart(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_820A8958 XACTWaveEntry::GetLoopStart");
  return guest::LoadU32(base, entry + 28);
}

uint32_t WaveEntryGetLoopLength(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82379D50 XACTWaveEntry::GetLoopLength");
  return guest::LoadU32(base, entry + 32);
}

double SoundGetPitch(uint8_t* base, uint32_t sound) {
  ITC_NATIVE_RUNNING("sub_825CAC48 XACTSound::GetPitch");
  return double(float(double(guest::LoadFloat(base, sound + 120)) +
                      double(guest::LoadFloat(base, sound + 92))));
}

uint32_t WaveBankAddListener(uint8_t* base, uint32_t self, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_825C8690 XACTWaveBank::AddListener");
  return NodePoolPushBack(base, self + 20, listener);
}

uint32_t WaveBankRemoveListener(uint8_t* base, uint32_t self, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_825C8540 XACTWaveBank::RemoveListener");
  return RemoveListener(base, self, self + 224, self + 20, listener);
}

namespace {

constexpr uint32_t kWaveBankGetName = 0x825C85B8;     // the wave bank's slot +24
constexpr uint32_t kGetWaveBankNames = 0x825C7B70;    // the sound bank's slot +80
constexpr uint32_t kWaveBankAddListener = 0x825C8690;  // its +4 interface's slot +0

}  // namespace

std::optional<uint32_t> EngineFindWaveBank(uint8_t* base, uint32_t engine, uint32_t name) {
  const uint32_t end = engine + 56;
  // Every prepared bank's slot +24 first (the walk changes nothing, but
  // the decline comes before any work).
  for (uint32_t node = guest::LoadU32(base, engine + 60); node != end;
       node = guest::LoadU32(base, node + 4)) {
    const uint32_t bank = node != 0 ? node - 8 : 0;
    if (guest::LoadU32(base, bank + 252) != 0 && !SlotIs(base, bank, 24, kWaveBankGetName)) {
      return std::nullopt;
    }
  }
  ITC_NATIVE_RUNNING("sub_825C0360 XACTEngine::FindWaveBank");
  for (uint32_t node = guest::LoadU32(base, engine + 60); node != end;
       node = guest::LoadU32(base, node + 4)) {
    const uint32_t bank = node != 0 ? node - 8 : 0;
    if (guest::LoadU32(base, bank + 252) == 0) {
      continue;
    }
    uint32_t a = WaveBankGetName(base, bank), b = name;
    for (;; a++, b++) {
      const uint8_t c = guest::LoadU8(base, a);
      if (c == 0 || c != guest::LoadU8(base, b)) {
        break;
      }
    }
    if (guest::LoadU8(base, a) == guest::LoadU8(base, b)) {
      return bank;
    }
  }
  return 0;
}

std::optional<uint32_t> SoundBankGetWaveBank(uint8_t* base, uint32_t bank, uint32_t index) {
  const uint32_t i = index & 0xFF;
  const uint32_t slot = guest::LoadU32(base, bank + 244) + (i << 2);
  const uint32_t cached = guest::LoadU32(base, slot);
  std::optional<uint32_t> found;
  if (cached == 0) {
    if (!SlotIs(base, bank, 80, kGetWaveBankNames)) {
      return std::nullopt;
    }
    const auto names = SoundBankGetWaveBankNames(base, bank);
    if (!names) {
      return std::nullopt;
    }
    found = EngineFindWaveBank(base, guest::LoadU32(base, bank + 24), *names + (i << 6));
    if (!found || (*found != 0 && !SlotIs(base, *found + 4, 0, kWaveBankAddListener))) {
      return std::nullopt;
    }
  }
  ITC_NATIVE_RUNNING("sub_825C7218 XACTSoundBank::GetWaveBank");
  if (cached != 0) {
    return cached;
  }
  if (*found != 0) {
    WaveBankAddListener(base, *found + 4, bank + 8);
    guest::StoreU32(base, guest::LoadU32(base, bank + 244) + (i << 2), *found);
  }
  return *found;
}

uint32_t VariableDestruct(uint8_t* base, uint32_t variable) {
  ITC_NATIVE_RUNNING("sub_825C2128 XACTVariable::~XACTVariable");
  guest::StoreU32(base, variable, kVariableVtable);
  const uint32_t pool = variable + 24;
  // Every used node back on the free list, head first.
  for (uint32_t node = guest::LoadU32(base, pool + 204); node != 0;) {
    const uint32_t next = guest::LoadU32(base, node + 4);
    guest::StoreU32(base, node + 4, guest::LoadU32(base, pool));
    guest::StoreU32(base, pool, node);
    guest::StoreU32(base, pool + 204, next);
    node = next;
  }
  guest::StoreU32(base, pool + 212, 0);
  guest::StoreU32(base, pool + 208, 0);
  const uint32_t r3 = NodePoolFreeChunks(base, pool);
  guest::StoreU32(base, variable, kListHeadVtable);
  return r3;
}

uint32_t VariableDeletingDestruct(uint8_t* base, uint32_t variable, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_825C3EE0 XACTVariable::DeletingDestructor");
  if (flags & 2) {
    const uint32_t block = variable - 4;
    const uint32_t count = guest::LoadU32(base, block);
    uint32_t at = variable + count * 252;
    for (int32_t i = int32_t(count) - 1; i >= 0; i--) {
      at -= 252;
      VariableDestruct(base, at);
    }
    if (flags & 1) {
      memory::crt::XMemFree(block, 0x20840051);
    }
    return block;
  }
  VariableDestruct(base, variable);
  if (flags & 1) {
    memory::crt::XMemFree(variable, 0x20848026);
  }
  return variable;
}

// --- Batch 8: the sound's volume, the cue's variables, the sounds' track
// records and RPC set-up where nothing is to do ---

std::optional<CategoryVolume> SoundGetVolume(uint8_t* base, uint32_t sound) {
  const uint32_t category = guest::LoadU16(base, guest::LoadU32(base, sound + 28) + 1);
  const auto volume = EngineGetCategoryVolume(base, guest::LoadU32(base, sound + 32), category);
  if (!volume) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825CB5E8 XACTSound::GetVolume");
  const auto at = [&](uint32_t address) { return double(guest::LoadFloat(base, address)); };
  double f0 = at(guest::LoadU32(base, sound + 48) + 28);  // the cue's fader gain
  f0 = double(float(f0 * at(sound + 88)));
  f0 = double(float(f0 * volume->f1));
  f0 = double(float(f0 * at(sound + 112)));
  return CategoryVolume{double(float(f0 * at(sound + 8))), volume->r3};
}

namespace {

constexpr uint32_t kCueVariableAttributes = 0x20848026;  // a cue's local variable
constexpr uint32_t kInvalidVariableIndex = 0x8AC7000A;   // XACTENGINE_E_INVALIDVARIABLEINDEX

}  // namespace

std::optional<uint32_t> CueGetVariable(uint8_t* base, uint32_t cue, uint32_t index, uint32_t out) {
  uint32_t variable = CueFindVariable(base, cue, index);
  std::optional<uint32_t> record;
  if (variable == 0) {
    record = EngineGetVariableRecord(base, guest::LoadU32(base, cue + 56), index);
    if (!record) {
      return std::nullopt;
    }
  }
  ITC_NATIVE_RUNNING("sub_825C5960 XACTCue::GetVariable");
  uint32_t result = 0;
  if (variable == 0) {
    if (*record == 0 || (guest::LoadU8(base, *record) & 4) == 0) {
      result = kInvalidVariableIndex;
    } else {
      variable = memory::crt::XMemAlloc(base, 252, kCueVariableAttributes);
      if (variable == 0) {
        result = kOutOfMemory;
      } else {
        VariableConstruct(base, variable);
        VariableInit(base, variable, guest::LoadU32(base, cue + 56), cue, index, *record, 0);
        // Linked at the tail of the cue's list (+324, its tail at +332).
        const uint32_t list = cue + 324;
        guest::StoreU32(base, variable + 4, list);
        guest::StoreU32(base, variable + 8, guest::LoadU32(base, list + 8));
        guest::StoreU32(base, guest::LoadU32(base, list + 8) + 4, variable);
        guest::StoreU32(base, list + 8, variable);
      }
    }
  }
  if (out != 0) {
    guest::StoreU32(base, out, variable);
  }
  return result;
}

std::optional<uint32_t> CueMakeInstanceVariable(uint8_t* base, uint32_t cue) {
  if (!CueGetVariable(base, cue, 0, 0)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C5E28 XACTCue::MakeInstanceVariable");
  return 0;
}

std::optional<uint32_t> SoundGetTrackRecord(uint8_t* base, uint32_t sound, uint32_t index) {
  const uint32_t record = guest::LoadU32(base, sound + 28);
  if ((guest::LoadU8(base, record) & 0x1E) != 0) {
    return std::nullopt;  // the optional sections (never in play)
  }
  ITC_NATIVE_RUNNING("sub_825CC9C8 XACTComplexSound::GetTrackRecord");
  return record + 10 + 5 * (index & 0xFFFF);
}

std::optional<uint32_t> SoundSetUpRpcs(uint8_t* base, uint32_t sound) {
  if ((guest::LoadU8(base, guest::LoadU32(base, sound + 28)) & 2) != 0) {
    return std::nullopt;  // RPCs to set up (never in play)
  }
  ITC_NATIVE_RUNNING("sub_825CB470 XACTSound::SetUpRpcs");
  return 0;
}

// --- Batch 9: loading the global settings ---

namespace {

constexpr uint32_t kGlobalsAttributes = 0x20840051;  // the array of global variables

// MakeState once the slots it reaches are known to be the native ones.
uint32_t MakeState(uint8_t* base, uint32_t settings) {
  ITC_NATIVE_RUNNING("sub_825C3B68 XACTGlobalSettings::MakeState");
  const auto buffer = [&] { return SettingsGetBuffer(base, settings); };
  const uint32_t variables = guest::LoadU16(base, buffer() + 11);
  const auto category_bytes = [&] { return uint32_t(guest::LoadU16(base, buffer() + 9)) << 2; };
  const uint32_t counts = memory::crt::XMemAlloc(base, category_bytes(), kPoolAttributes);
  if (counts == 0) {
    return kOutOfMemory;
  }
  uint32_t volumes = 0, states = 0, flags = 0, globals = 0;
  bool failed = (volumes = memory::crt::XMemAlloc(base, category_bytes(), kPoolAttributes)) == 0 ||
                (states = memory::crt::XMemAlloc(base, category_bytes(), kPoolAttributes)) == 0 ||
                (flags = memory::crt::XMemAlloc(base, category_bytes(), kPoolAttributes)) == 0;
  if (!failed && variables != 0) {
    const uint32_t block = memory::crt::XMemAlloc(base, 252 * variables + 4, kGlobalsAttributes);
    if (block == 0) {
      failed = true;
    } else {
      globals = block + 4;
      guest::StoreU32(base, block, variables);
      for (uint32_t v = 0; v < variables; v++) {
        VariableConstruct(base, globals + 252 * v);
      }
    }
  }
  if (failed) {
    memory::crt::XMemFree(counts, kPoolAttributes);
    for (uint32_t block : {volumes, states, flags}) {
      if (block != 0) {
        memory::crt::XMemFree(block, kPoolAttributes);
      }
    }
    return kOutOfMemory;
  }
  Zero(base, counts, category_bytes());
  guest::StoreU32(base, settings + 104, counts);
  Zero(base, states, category_bytes());
  guest::StoreU32(base, settings + 112, states);
  Zero(base, flags, category_bytes());
  guest::StoreU32(base, settings + 108, flags);
  // Each category's volume from the gain table by its volume byte.
  for (uint32_t c = 0; c < guest::LoadU16(base, buffer() + 9); c++) {
    const uint32_t byte = *SettingsGetVolumeByte(base, settings, c);
    CopyFloat(base, volumes + (c << 2), kGainTable + ((byte << 2) & 0x3FC));
  }
  guest::StoreU32(base, settings + 120, volumes);
  for (uint32_t v = 0; v < variables; v++) {
    const uint32_t record = *SettingsGetVariableRecord(base, settings, v);
    VariableInit(base, globals + 252 * v, guest::LoadU32(base, settings + 4), 0, v, record, 0);
  }
  guest::StoreU32(base, settings + 124, globals);
  return 0;
}

}  // namespace

std::optional<uint32_t> SettingsLoad(uint8_t* base, uint32_t settings, uint32_t buffer,
                                     uint32_t size, uint32_t flags, uint32_t attributes) {
  if (!BufferSlot(base, settings) || !SlotIs(base, settings, 28, kSettingsGetVolumeByte) ||
      !RecordSlots(base, settings) || !SlotIs(base, settings, 8, kSettingsGetVariableRecord)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_825C26E0 XACTGlobalSettings::Load");
  guest::StoreU32(base, settings + 144, flags);
  guest::StoreU32(base, settings + 136, attributes);
  guest::StoreU32(base, settings + 140, size);
  guest::StoreU32(base, settings + 132, buffer);
  const auto header = [&] { return SettingsGetBuffer(base, settings); };
  if (guest::LoadU16(base, header() + 17) != 0) {
    // The RPCs on DSP parameters (parameter above 2) counted into +100.
    uint32_t step = guest::LoadU32(base, header() + 55);
    guest::StoreU16(base, settings + 100, 0);
    uint32_t rpc = buffer;
    for (uint32_t i = 0;; i = (i + 1) & 0xFFFF) {
      rpc += step;
      if (i >= guest::LoadU16(base, header() + 17)) {
        break;
      }
      const uint32_t points = guest::LoadU8(base, rpc + 2);
      step = points * 9 + 5;
      if (guest::LoadU16(base, rpc + 3) > 2) {
        guest::StoreU16(base, settings + 100, uint16_t(guest::LoadU16(base, settings + 100) + 1));
      }
    }
  }
  return MakeState(base, settings);
}

std::optional<uint32_t> SettingsSetUpDspPreset(uint8_t* base, uint32_t settings) {
  if (!BufferSlot(base, settings)) {
    return std::nullopt;
  }
  const bool presets = guest::LoadU16(base, SettingsGetBuffer(base, settings) + 19) != 0;
  const bool made = guest::LoadU32(base, settings + 8) != 0;
  if (presets != made) {
    return std::nullopt;  // the renderer makes or releases the effect (never in play)
  }
  ITC_NATIVE_RUNNING("sub_825C27C0 XACTGlobalSettings::SetUpDspPreset");
  return 0;
}

std::optional<uint32_t> SettingsSetUpRpcs(uint8_t* base, uint32_t settings) {
  if (guest::LoadU16(base, settings + 100) != 0) {
    return std::nullopt;  // RPCs on DSP parameters (never in play)
  }
  ITC_NATIVE_RUNNING("sub_825C2910 XACTGlobalSettings::SetUpRpcs");
  return 0;
}

}  // namespace xact
