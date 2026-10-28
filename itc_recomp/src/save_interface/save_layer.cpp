// The front end's save layer natively (save_layer.h,
// docs/functions/save-interface.md).

#include "save_interface/save_layer.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "native/buffers.h"
#include "strings_and_font/strings_and_font.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "save_interface/save_interface.h"

#include <cstring>

namespace save_interface {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// memcpy (sub_825A7410) of a record: its plain C++, or the same bytes copied
// one by one where it refuses (ranges that may share memory: never for
// records apart and the stack copy).
void Copy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size) {
  if (native::Buffers_Memcpy(base, destination, source, size, 0x8228F89C)) {
    return;
  }
  for (uint32_t i = 0; i < size; i++) {
    guest::StoreU8(base, destination + i, guest::LoadU8(base, source + i));
  }
}

// memset (sub_825A71D0) with 0: the same bytes.
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(base, address, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

template <typename Call>
auto OnPlayersSlot(uint8_t* base, uint32_t player, Call call) -> decltype(call(0u)) {
  const auto slot = SlotOfController(base, ControllerOf(base, player));
  if (!slot) {
    return std::nullopt;
  }
  return call(*slot);
}

}  // namespace

uint32_t ControllerOf(uint8_t* base, uint32_t player) {
  return Load(base, kControllers + (player << 2));
}

std::optional<uint32_t> SlotOfController(uint8_t* base, uint32_t controller) {
  const auto count = InterfaceUserCount(base);
  if (!count) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8228F780 SaveSlots::OfController");
  for (int32_t i = 0; i < int32_t(*count); i++) {
    const auto user = InterfaceUserAt(base, uint32_t(i));
    if (!user) {
      return std::nullopt;
    }
    if (int32_t(uint32_t(*user)) == int32_t(controller)) {
      return uint32_t(i);
    }
  }
  return *count;
}

std::optional<uint32_t> PlayerRequestDevice(uint8_t* base, uint32_t player) {
  return OnPlayersSlot(base, player, [&](uint32_t slot) -> std::optional<uint32_t> {
    const auto r3 = InterfaceRequestDevice(base, slot);
    if (r3) {
      ITC_NATIVE_RUNNING("sub_8228F9B0 SaveSlots::RequestDevice");
    }
    return r3;
  });
}

std::optional<uint32_t> PlayerSlotState(uint8_t* base, uint32_t player) {
  return OnPlayersSlot(base, player, [&](uint32_t slot) -> std::optional<uint32_t> {
    const auto r3 = InterfaceSlotState(base, slot);
    if (r3) {
      ITC_NATIVE_RUNNING("sub_8228F9E8 SaveSlots::State");
    }
    return r3;
  });
}

std::optional<uint32_t> PlayerDevice(uint8_t* base, uint32_t player) {
  return OnPlayersSlot(base, player, [&](uint32_t slot) -> std::optional<uint32_t> {
    const auto r3 = InterfaceDevice(base, slot);
    if (r3) {
      ITC_NATIVE_RUNNING("sub_8228FA90 SaveSlots::Device");
    }
    return r3;
  });
}

std::optional<uint64_t> PlayerPending(uint8_t* base, uint32_t player) {
  return OnPlayersSlot(base, player, [&](uint32_t slot) -> std::optional<uint64_t> {
    const auto r3 = InterfacePending(base, slot);
    if (r3) {
      ITC_NATIVE_RUNNING("sub_8228FAC8 SaveSlots::Pending");
    }
    return r3;
  });
}

std::optional<uint32_t> SaveBuffersCreate(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8228F6B0 SaveBuffers::Create");
  if (Load(base, kSaveBuffers) != 0) {
    return std::nullopt;
  }
  // VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE) each.
  const uint32_t header = memory::AllocPages(24, 4096);
  Store(base, kSaveBuffers, header);
  for (uint32_t offset = 0; offset < 24; offset += 4) {
    Store(base, header + offset, 0);
  }
  const uint32_t game_data = memory::AllocPages(kGameDataBytes, 4096);
  Store(base, Load(base, kSaveBuffers) + 0, game_data);
  const uint32_t replay = memory::AllocPages(kReplayBytes, 4096);
  Store(base, Load(base, kSaveBuffers) + 4, replay);
  const uint32_t record = memory::AllocPages(kReplayRecordBytes, 4096);
  Store(base, Load(base, kSaveBuffers) + 8, record);
  return record;
}

std::optional<uint32_t> SaveBuffersFreeReplayList(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8228F7E8 SaveBuffers::FreeReplayList");
  const uint32_t records = Load(base, Load(base, kSaveBuffers) + 12);
  if (records == 0) {
    return std::nullopt;
  }
  // VirtualFree(records, 0, MEM_RELEASE).
  const uint32_t r3 = memory::FreePages(records) ? 1 : 0;
  Store(base, Load(base, kSaveBuffers) + 12, 0);
  return r3;
}

bool SaveBuffersDestroyNative(uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  const uint32_t header = Load(base, kSaveBuffers);
  if (!memory::IsPages(header)) {
    return false;
  }
  for (uint32_t offset : {0u, 4u, 8u, 12u}) {
    const uint32_t buffer = Load(base, header + offset);
    if (buffer != 0 && !memory::IsPages(buffer)) {
      return false;
    }
  }
  return true;
}

uint32_t SaveBuffersDestroy(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8228F8F8 SaveBuffers::Destroy");
  SaveBuffersFreeReplayList(base);
  for (uint32_t offset : {0u, 4u, 8u}) {
    const uint32_t buffer = Load(base, Load(base, kSaveBuffers) + offset);
    if (buffer != 0) {
      memory::FreePages(buffer);  // VirtualFree(buffer, 0, MEM_RELEASE)
      Store(base, Load(base, kSaveBuffers) + offset, 0);
    }
  }
  const uint32_t r3 = memory::FreePages(Load(base, kSaveBuffers)) ? 1 : 0;
  Store(base, kSaveBuffers, 0);
  return r3;
}

std::optional<uint32_t> ReplaysSort(uint8_t* base, uint32_t records, uint32_t temp) {
  ITC_NATIVE_RUNNING("sub_8228F840 Replays::Sort");
  uint32_t header = Load(base, kSaveBuffers);
  int32_t count = int32_t(Load(base, header + 20));
  std::optional<uint32_t> r3;
  if (!(count - 1 > 0)) {
    return r3;
  }
  for (int32_t i = 1;; i++) {
    const uint32_t first = records + 36 * uint32_t(i - 1);
    for (int32_t j = i; j < count; j++) {
      const uint32_t other = records + 36 * uint32_t(j);
      if (int32_t(Load(base, first + 28)) > int32_t(Load(base, other + 28))) {
        Copy(base, temp, first, 36);
        Copy(base, first, other, 36);
        Copy(base, other, temp, 36);
        r3 = other;
        header = Load(base, kSaveBuffers);
      }
      count = int32_t(Load(base, header + 20));
    }
    count = int32_t(Load(base, header + 20));
    if (!(i < count - 1)) {
      return r3;
    }
  }
}

void PlayTimeAdd(uint8_t* base, uint32_t out, uint32_t stats) {
  ITC_NATIVE_RUNNING("sub_821109B8 PlayTime::Add");
  const uint32_t frames = guest::LoadU8(base, stats + 6038);
  const uint32_t hours = Load(base, stats + 6032);
  const uint32_t minutes = guest::LoadU8(base, stats + 6036);
  const uint32_t seconds = guest::LoadU8(base, stats + 6037);
  const uint32_t now = Load(base, kPlayTimeFrames);
  const uint32_t last = Load(base, stats + 6040);
  Store(base, stats + 6040, now);
  const uint32_t d = now - last;
  uint32_t f = d - (d / 30) * 30 + frames;
  uint32_t h = d / 108000 + hours;
  uint32_t m = (d - (d / 108000) * 108000) / 1800 + minutes;
  uint32_t s = (d - (d / 1800) * 1800) / 30 + seconds;
  if (f >= 30) {
    const uint32_t q = f / 30;
    s += q;
    f -= 30 * q;
  }
  if (s >= 60) {
    const uint32_t q = s / 60;
    m += q;
    s -= 60 * q;
  }
  if (m >= 60) {
    const uint32_t q = m / 60;
    h += q;
    m -= 60 * q;
  }
  guest::StoreU8(base, stats + 6037, uint8_t(s));
  guest::StoreU8(base, stats + 6036, uint8_t(m));
  guest::StoreU8(base, stats + 6038, uint8_t(f));
  const uint32_t word = Load(base, stats + 6036);
  Store(base, stats + 6032, h);
  Store(base, out + 0, h);
  Store(base, out + 4, word);
}

std::optional<uint64_t> GameDataSave(uint8_t* base, uint32_t frame, const GameDataCalls& calls) {
  const auto slot = SlotOfController(base, ControllerOf(base, 0));
  if (!slot || !InterfaceRequestReady(base, false)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8228FB00 GameData::Save");
  const uint32_t name = frame + 96;
  const uint32_t time = frame + 80;
  Clear(base, name, 256);
  PlayTimeUpdate(base, vehicle_accessors::SaveDataOf(base, 0), frame - 96 + 80);
  calls.local_time(time);
  // swprintf(name, format, L"GAME DATA", year, month, day, hour, minute):
  // r5 to r10 in the caller's parameter area (frame + 32), as the
  // original's stores them.
  const std::array<uint64_t, 8> regs = {
      name,
      uint64_t(int64_t(int32_t(kGameDataFormat))),
      uint64_t(int64_t(int32_t(kGameDataTitle))),
      guest::LoadU16(base, time + 0),
      guest::LoadU16(base, time + 2),
      guest::LoadU16(base, time + 6),
      guest::LoadU16(base, time + 8),
      guest::LoadU16(base, time + 10)};
  for (uint32_t k = 0; k < 6; k++) {
    guest::StoreU32(base, frame + 32 + 8 * k, uint32_t(regs[2 + k] >> 32));
    guest::StoreU32(base, frame + 36 + 8 * k, uint32_t(regs[2 + k]));
  }
  int32_t count = 0;
  if (!strings_and_font::Swprintf(base, name, kGameDataFormat, frame + 32,
                                  frame - 96 - 144 - 2336, &count)) {
    calls.swprintf(regs);
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  return InterfaceSave(base, *slot,
                       Request{name, kGameDataFile, kGameDataPath, save, kGameDataBytes, 0, 0});
}

std::optional<uint64_t> GameDataLoad(uint8_t* base, uint32_t player) {
  const auto slot = SlotOfController(base, ControllerOf(base, player));
  if (!slot || !InterfaceRequestReady(base, true)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8228FBC0 GameData::Load");
  Clear(base, Load(base, Load(base, kSaveBuffers) + 0), kGameDataBytes);
  const uint32_t buffer = Load(base, Load(base, kSaveBuffers) + 0);
  return InterfaceLoad(base, *slot,
                       Request{kLoadDisplayName, kGameDataFile, kGameDataPath, buffer,
                               kGameDataBytes, 0, 0});
}

std::optional<uint32_t> GameDataCheck(uint8_t* base, uint32_t player) {
  const auto slot = SlotOfController(base, ControllerOf(base, player));
  if (!slot) {
    return std::nullopt;
  }
  const auto count = InterfaceSaveCount(base, *slot);
  const auto state = InterfaceSlotState(base, *slot);
  if (!count || !state) {
    return std::nullopt;
  }
  if (int32_t(*state) == 2) {
    ITC_NATIVE_RUNNING("sub_8228FC50 GameData::Check");
    return 3;
  }
  if (int32_t(*state) != 3) {
    ITC_NATIVE_RUNNING("sub_8228FC50 GameData::Check");
    return 4;
  }
  const auto pending = InterfacePending(base, *slot);
  if (!pending) {
    return std::nullopt;
  }
  if (uint32_t(*pending) != 0) {
    ITC_NATIVE_RUNNING("sub_8228FC50 GameData::Check");
    return 4;
  }
  const auto free = InterfaceFreeBytes(base, *slot);
  if (!free) {
    return std::nullopt;
  }
  const uint32_t size = uint32_t(ContentSize(kGameDataBytes, 0));
  for (int32_t i = 0; i < int32_t(*count); i++) {
    const auto entry = InterfaceSaveAt(base, *slot, uint64_t(i));
    if (!entry) {
      return std::nullopt;
    }
    // strcmp of the file name (XCONTENT_DATA +264) with "SystemData.bin".
    uint32_t a = uint32_t(*entry + 264), b = kGameDataFile;
    int32_t diff;
    for (;;) {
      const uint32_t c = guest::LoadU8(base, a);
      diff = int32_t(c - guest::LoadU8(base, b));
      if (c == 0) {
        break;
      }
      a++;
      b++;
      if (diff != 0) {
        break;
      }
    }
    if (diff == 0) {
      ITC_NATIVE_RUNNING("sub_8228FC50 GameData::Check");
      return 1;
    }
  }
  ITC_NATIVE_RUNNING("sub_8228FC50 GameData::Check");
  return *free >= size ? 0 : 2;
}

uint32_t PlayTimeUpdate(uint8_t* base, uint32_t save, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82112838 PlayTime::Update");
  PlayTimeAdd(base, out, save + 48);
  return out;
}

}  // namespace save_interface
