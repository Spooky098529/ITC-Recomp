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
#include "schema/career.h"
#include "schema/files_and_saves.h"
#include "schema/options_load_save.h"
#include "schema/save_interface.h"

#include <cstring>
#include <utility>

namespace save_interface {

namespace {

using G = schema::SaveInterfaceGlobals;

// A kept replay's bytes (type10-mode.toml's KeptReplay, 0x13C8F0): the
// replay buffer's size as a blob (SaveBuffers::Create).
constexpr uint32_t kKeptReplayBytes = 0x13C8F0;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// memcpy (sub_825A7410) of a record: its plain C++, or the same bytes copied
// one by one where it refuses (ranges that may share memory: never for
// records apart and the stack copy).
void Copy(uint32_t destination, uint32_t source, uint32_t size) {
  if (native::Buffers_Memcpy(destination, source, size, 0x8228F89C)) {
    return;
  }
  for (uint32_t i = 0; i < size; i++) {
    guest::StoreU8(destination + i, guest::LoadU8(source + i));
  }
}

// memset (sub_825A71D0) with 0: the same bytes.
void Clear(uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(address, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

template <typename Call>
auto OnPlayersSlot(uint32_t player, Call call) -> decltype(call(0u)) {
  const auto slot = SlotOfController(ControllerOf(player));
  if (!slot) {
    return std::nullopt;
  }
  return call(*slot);
}

}  // namespace

uint32_t ControllerOf(uint32_t player) {
  return Load(G::controllers_address() + (player << 2));
}

std::optional<uint32_t> SlotOfController(uint32_t controller) {
  const auto count = InterfaceUserCount();
  if (!count) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8228F780 SaveSlots::OfController");
  for (int32_t i = 0; i < int32_t(*count); i++) {
    const auto user = InterfaceUserAt(uint32_t(i));
    if (!user) {
      return std::nullopt;
    }
    if (int32_t(uint32_t(*user)) == int32_t(controller)) {
      return uint32_t(i);
    }
  }
  return *count;
}

std::optional<uint32_t> PlayerRequestDevice(uint32_t player) {
  return OnPlayersSlot(player, [&](uint32_t slot) -> std::optional<uint32_t> {
    const auto r3 = InterfaceRequestDevice(slot);
    if (r3) {
      ITC_NATIVE_RUNNING("sub_8228F9B0 SaveSlots::RequestDevice");
    }
    return r3;
  });
}

std::optional<uint32_t> PlayerSlotState(uint32_t player) {
  return OnPlayersSlot(player, [&](uint32_t slot) -> std::optional<uint32_t> {
    const auto r3 = InterfaceSlotState(slot);
    if (r3) {
      ITC_NATIVE_RUNNING("sub_8228F9E8 SaveSlots::State");
    }
    return r3;
  });
}

std::optional<uint32_t> PlayerDevice(uint32_t player) {
  return OnPlayersSlot(player, [&](uint32_t slot) -> std::optional<uint32_t> {
    const auto r3 = InterfaceDevice(slot);
    if (r3) {
      ITC_NATIVE_RUNNING("sub_8228FA90 SaveSlots::Device");
    }
    return r3;
  });
}

std::optional<uint64_t> PlayerPending(uint32_t player) {
  return OnPlayersSlot(player, [&](uint32_t slot) -> std::optional<uint64_t> {
    const auto r3 = InterfacePending(slot);
    if (r3) {
      ITC_NATIVE_RUNNING("sub_8228FAC8 SaveSlots::Pending");
    }
    return r3;
  });
}

std::optional<uint32_t> SaveBuffersCreate() {
  ITC_NATIVE_RUNNING("sub_8228F6B0 SaveBuffers::Create");
  if (G::save_buffers().address != 0) {
    return std::nullopt;
  }
  // VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE) each, in PC memory
  // (E5 step 5): the header a host object, its buffers blobs (the game's
  // bytes, saved and loaded by address).
  const schema::SaveBuffers header = schema::SaveBuffers::New();
  G::set_save_buffers(header);
  header.set_game_data(0);
  header.set_replay(0);
  header.set_replay_record(0u);
  header.set_records(0u);
  header.set_entries(0u);
  header.set_count(0);
  const uint32_t game_data = schema::OptionsLoadedGameData::New(kGameDataBytes).address;
  G::save_buffers().set_game_data(game_data);
  // A kept replay's size, not the original's kReplayBytes: Replay::CopyFrom
  // copies a whole kept replay (KeptReplay::kSize) into and out of it, past
  // the original's buffer, which a blob of that size would refuse
  // (save-interface.toml's SaveReplayData). The saves still write and read
  // kReplayBytes.
  const uint32_t replay = schema::SaveReplayData::New(kKeptReplayBytes).address;
  G::save_buffers().set_replay(replay);
  const uint32_t record = schema::SaveReplayRecord::New(kReplayRecordBytes).address;
  G::save_buffers().set_replay_record(record);
  return record;
}

std::optional<uint32_t> SaveBuffersFreeReplayList() {
  ITC_NATIVE_RUNNING("sub_8228F7E8 SaveBuffers::FreeReplayList");
  const uint32_t records = G::save_buffers().records().address;
  if (records == 0) {
    return std::nullopt;
  }
  // VirtualFree(records, 0, MEM_RELEASE).
  const uint32_t r3 = memory::FreePages(records) ? 1 : 0;
  G::save_buffers().set_records(0u);
  return r3;
}

bool SaveBuffersDestroyNative() {
  if (!hooks::NativeHeapActive()) {
    return false;
  }
  const schema::SaveBuffers header = G::save_buffers();
  if (!memory::IsPages(header.address)) {
    return false;
  }
  for (uint32_t buffer : {header.game_data(), header.replay(), header.replay_record().address,
                          header.records().address}) {
    if (buffer != 0 && !memory::IsPages(buffer)) {
      return false;
    }
  }
  return true;
}

uint32_t SaveBuffersDestroy() {
  ITC_NATIVE_RUNNING("sub_8228F8F8 SaveBuffers::Destroy");
  SaveBuffersFreeReplayList();
  // +0, +4 and +8 in turn, each read through [0x828161F0] again.
  using Get = uint32_t (*)(const schema::SaveBuffers&);
  using Set = void (*)(const schema::SaveBuffers&);
  static constexpr std::pair<Get, Set> kFields[] = {
      {[](const schema::SaveBuffers& h) { return h.game_data(); },
       [](const schema::SaveBuffers& h) { h.set_game_data(0); }},
      {[](const schema::SaveBuffers& h) { return h.replay(); },
       [](const schema::SaveBuffers& h) { h.set_replay(0); }},
      {[](const schema::SaveBuffers& h) { return h.replay_record().address; },
       [](const schema::SaveBuffers& h) { h.set_replay_record(0u); }}};
  for (const auto& [get, set] : kFields) {
    const uint32_t buffer = get(G::save_buffers());
    if (buffer != 0) {
      memory::FreePages(buffer);  // VirtualFree(buffer, 0, MEM_RELEASE)
      set(G::save_buffers());
    }
  }
  const uint32_t r3 = memory::FreePages(G::save_buffers().address) ? 1 : 0;
  G::set_save_buffers(0u);
  return r3;
}

std::optional<uint32_t> ReplaysSort(uint32_t records, uint32_t temp) {
  ITC_NATIVE_RUNNING("sub_8228F840 Replays::Sort");
  schema::SaveBuffers header = G::save_buffers();
  int32_t count = int32_t(header.count());
  std::optional<uint32_t> r3;
  if (!(count - 1 > 0)) {
    return r3;
  }
  for (int32_t i = 1;; i++) {
    const uint32_t first = records + 36 * uint32_t(i - 1);
    for (int32_t j = i; j < count; j++) {
      const uint32_t other = records + 36 * uint32_t(j);
      if (int32_t(schema::SaveReplayRecord(first).number()) >
          int32_t(schema::SaveReplayRecord(other).number())) {
        Copy(temp, first, 36);
        Copy(first, other, 36);
        Copy(other, temp, 36);
        r3 = other;
        header = G::save_buffers();
      }
      count = int32_t(header.count());
    }
    count = int32_t(header.count());
    if (!(i < count - 1)) {
      return r3;
    }
  }
}

void PlayTimeAdd(uint32_t out, uint32_t stats) {
  ITC_NATIVE_RUNNING("sub_821109B8 PlayTime::Add");
  // career.toml's CareerStats: +6032 the hours, +6036 to +6038 the minutes,
  // seconds and frames, +6040 (a measure fault there) the last mark.
  const schema::CareerStats st(stats);
  const uint32_t frames = st.unknown_6038();
  const uint32_t hours = st.unknown_6032();
  const uint32_t minutes = st.unknown_6036();
  const uint32_t seconds = st.unknown_6037();
  const uint32_t now = G::play_time_frames();
  const uint32_t last = st.fault_6040();
  st.set_fault_6040(now);
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
  st.set_unknown_6037(uint8_t(s));
  st.set_unknown_6036(uint8_t(m));
  st.set_unknown_6038(uint8_t(f));
  // The word at +6036 (the minutes, seconds, frames and the next byte).
  const uint32_t word = Load(st.unknown_6036_address());
  st.set_unknown_6032(h);
  Store(out + 0, h);
  Store(out + 4, word);
}

std::optional<uint64_t> GameDataSave(uint32_t frame, const GameDataCalls& calls) {
  const auto slot = SlotOfController(ControllerOf(0));
  if (!slot || !InterfaceRequestReady(false)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8228FB00 GameData::Save");
  const uint32_t name = frame + 96;
  const uint32_t time = frame + 80;
  Clear(name, 256);
  PlayTimeUpdate(vehicle_accessors::SaveDataOf(0), frame - 96 + 80);
  calls.local_time(time);
  // swprintf(name, format, L"GAME DATA", year, month, day, hour, minute):
  // r5 to r10 in the caller's parameter area (frame + 32), as the
  // original's stores them.
  const std::array<uint64_t, 8> regs = {
      name,
      uint64_t(int64_t(int32_t(kGameDataFormat))),
      uint64_t(int64_t(int32_t(kGameDataTitle))),
      guest::LoadU16(time + 0),
      guest::LoadU16(time + 2),
      guest::LoadU16(time + 6),
      guest::LoadU16(time + 8),
      guest::LoadU16(time + 10)};
  for (uint32_t k = 0; k < 6; k++) {
    guest::StoreU32(frame + 32 + 8 * k, uint32_t(regs[2 + k] >> 32));
    guest::StoreU32(frame + 36 + 8 * k, uint32_t(regs[2 + k]));
  }
  int32_t count = 0;
  if (!strings_and_font::Swprintf(name, kGameDataFormat, frame + 32,
                                  frame - 96 - 144 - 2336, &count)) {
    calls.swprintf(regs);
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  return InterfaceSave(*slot,
                       Request{name, kGameDataFile, kGameDataPath, save, kGameDataBytes, 0, 0});
}

std::optional<uint64_t> GameDataLoad(uint32_t player) {
  const auto slot = SlotOfController(ControllerOf(player));
  if (!slot || !InterfaceRequestReady(true)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8228FBC0 GameData::Load");
  Clear(G::save_buffers().game_data(), kGameDataBytes);
  const uint32_t buffer = G::save_buffers().game_data();
  return InterfaceLoad(*slot,
                       Request{kLoadDisplayName, kGameDataFile, kGameDataPath, buffer,
                               kGameDataBytes, 0, 0});
}

std::optional<uint32_t> GameDataCheck(uint32_t player) {
  const auto slot = SlotOfController(ControllerOf(player));
  if (!slot) {
    return std::nullopt;
  }
  const auto count = InterfaceSaveCount(*slot);
  const auto state = InterfaceSlotState(*slot);
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
  const auto pending = InterfacePending(*slot);
  if (!pending) {
    return std::nullopt;
  }
  if (uint32_t(*pending) != 0) {
    ITC_NATIVE_RUNNING("sub_8228FC50 GameData::Check");
    return 4;
  }
  const auto free = InterfaceFreeBytes(*slot);
  if (!free) {
    return std::nullopt;
  }
  const uint32_t size = uint32_t(ContentSize(kGameDataBytes, 0));
  for (int32_t i = 0; i < int32_t(*count); i++) {
    const auto entry = InterfaceSaveAt(*slot, uint64_t(i));
    if (!entry) {
      return std::nullopt;
    }
    // strcmp of the file name (XCONTENT_DATA +264) with "SystemData.bin".
    uint32_t a = schema::XContentData(uint32_t(*entry)).file_name_address(),
             b = kGameDataFile;
    int32_t diff;
    for (;;) {
      const uint32_t c = guest::LoadU8(a);
      diff = int32_t(c - guest::LoadU8(b));
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

namespace {

bool StartsWithRep(uint32_t name) {
  return guest::LoadU8(name + 0) == 'r' && guest::LoadU8(name + 1) == 'e' &&
         guest::LoadU8(name + 2) == 'p';
}

// Player 0's slot, as the replays' functions find it each time.
std::optional<uint32_t> ReplaySlot() {
  return SlotOfController(ControllerOf(0));
}

}  // namespace

std::optional<uint32_t> ReplaysCount(uint32_t out) {
  const auto slot = ReplaySlot();
  if (!slot) {
    return std::nullopt;
  }
  const auto state = InterfaceSlotState(*slot);
  const auto count = InterfaceSaveCount(*slot);
  const auto pending = InterfacePending(*slot);
  if (!state || !count || !pending || !InterfaceSaveAt(*slot, 0)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_822902A0 Replays::Count");
  Store(out, 0);
  if (int32_t(*state) == 2) {
    return 3;
  }
  if (int32_t(*state) != 3 || uint32_t(*pending) != 0) {
    return 4;
  }
  for (int32_t i = 0; i < int32_t(*count); i++) {
    const uint32_t name =
        schema::XContentData(uint32_t(*InterfaceSaveAt(*ReplaySlot(), uint64_t(i))))
            .file_name_address();
    if (StartsWithRep(name)) {
      Store(out, Load(out) + 1);
    }
  }
  return 7;
}

std::optional<uint32_t> ReplaysReadEntry(uint32_t index, uint32_t, uint32_t) {
  const auto slot = ReplaySlot();
  if (!slot) {
    return std::nullopt;
  }
  const auto state = InterfaceSlotState(*slot);
  const auto pending = InterfacePending(*slot);
  const auto save = InterfaceSaveAt(*slot, index);
  if (!state || !pending || !save) {
    return std::nullopt;
  }
  if (int32_t(*state) == 2) {
    ITC_NATIVE_RUNNING("sub_822900E0 Replays::ReadEntry");
    return 3;
  }
  if (int32_t(*state) != 3 || uint32_t(*pending) != 0) {
    ITC_NATIVE_RUNNING("sub_822900E0 Replays::ReadEntry");
    return 4;
  }
  if (StartsWithRep(schema::XContentData(uint32_t(*save)).file_name_address())) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_822900E0 Replays::ReadEntry");
  return 5;
}

bool ReplaysBuildListNative() {
  if (!hooks::NativeHeapActive() || !native::Buffers_NativeMemcpy()) {
    return false;
  }
  const schema::SaveBuffers header = G::save_buffers();
  for (uint32_t list : {header.records().address, header.entries().address}) {
    if (list != 0 && !memory::IsPages(list)) {
      return false;
    }
  }
  return true;
}

std::optional<uint32_t> ReplaysBuildList(
    uint32_t add_empty, uint32_t frame,
    const std::function<uint32_t(uint32_t index, uint32_t record, uint32_t entry)>& read_entry) {
  const auto counted = ReplaysCount(frame + 80);
  if (!counted) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82290378 Replays::BuildList");
  if (*counted != 7) {
    return *counted;
  }
  schema::SaveBuffers header = G::save_buffers();
  if (const uint32_t records = header.records().address; records != 0) {
    memory::FreePages(records);  // VirtualFree(records, 0, MEM_RELEASE)
    header = G::save_buffers();
  }
  if (const uint32_t entries = header.entries().address; entries != 0) {
    memory::FreePages(entries);
  }
  uint32_t count = Load(frame + 80);
  uint32_t records, entries;
  // VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE) each, as blobs in PC
  // memory (E5 step 5); 0 for no bytes, as VirtualAlloc refuses them.
  const auto new_records = [&](uint32_t n) {
    const uint32_t bytes = n * schema::SaveReplayRecord::kSize;
    return bytes == 0 ? 0u : schema::SaveReplayRecord::New(bytes).address;
  };
  const auto new_entries = [&](uint32_t n) {
    const uint32_t bytes = n * schema::SaveReplayEntry::kSize;
    return bytes == 0 ? 0u : schema::SaveReplayEntry::New(bytes).address;
  };
  if (int32_t(count) < 99 && add_empty != 0) {
    count++;
    G::save_buffers().set_records(new_records(count));
    G::save_buffers().set_entries(new_entries(count));
    G::save_buffers().records().set_number(0);
    G::save_buffers().entries().set_number(0);
    header = G::save_buffers();
    records = header.records().address + schema::SaveReplayRecord::kSize;
    entries = header.entries().address + schema::SaveReplayEntry::kSize;
  } else {
    G::save_buffers().set_records(new_records(count));
    G::save_buffers().set_entries(new_entries(count));
    header = G::save_buffers();
    records = header.records().address;
    entries = header.entries().address;
  }
  header.set_count(count);
  const auto saves = [&] { return *InterfaceSaveCount(*ReplaySlot()); };
  if (saves() != 0) {
    uint32_t index = 0;
    do {
      const auto native = ReplaysReadEntry(index, records, entries);
      const uint32_t answer = native ? *native : read_entry(index, records, entries);
      if (int32_t(answer) >= 3) {
        if (int32_t(answer) <= 4) {
          return answer;
        }
        if (answer == 6) {
          records += 36;
          entries += 304;
        }
      }
      index++;
    } while (index < saves());
  }
  ReplaysSort(G::save_buffers().records().address, frame - 176 + 80);
  return 7;
}

uint32_t PlayTimeUpdate(uint32_t save, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_82112838 PlayTime::Update");
  PlayTimeAdd(out, schema::CareerSave(save).stats_address());
  return out;
}

}  // namespace save_interface
