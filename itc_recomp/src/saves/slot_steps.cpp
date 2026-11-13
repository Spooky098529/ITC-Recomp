// The save slots' steps on our own saves/ folder, as plain C++ (slot_steps.h).

#include "saves/slot_steps.h"

#include "guest.h"

#include "base/log.h"
#include "save_interface/save_interface.h"
#include "saves/saves.h"

#include <string>
#include <vector>

namespace saves::slot {

namespace {

constexpr uint32_t kDevice = 1;
// The device's free bytes, as the SDK answered (the game only checks there's
// room for the save's 188,416).
constexpr uint64_t kFreeBytes = 3221225472;
constexpr uint32_t kSavedGame = 1;  // the content type
constexpr const char* kContentName = "SystemData.bin";
constexpr const char* kSaveFile = "TXR_DATA_1";
constexpr uint32_t kContentDataBytes = 308;
constexpr uint32_t kOverlappedBytes = 28;

// Win32 errors the steps' overlappeds hold.
constexpr uint32_t kFileNotFound = 2;
constexpr uint32_t kNoMoreFiles = 1627;
constexpr uint32_t kIncomplete = 996;
constexpr uint32_t kPending = 997;

void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, address + i, 0);
  }
}

void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

// An overlapped finished: its status (a Win32 error, 0 for done) and
// information.
void Finish(uint8_t* base, uint32_t overlapped, uint32_t status, uint32_t information = 0) {
  guest::StoreU32(base, overlapped, status);
  guest::StoreU32(base, overlapped + 4, information);
}

// What XGetOverlappedResult (sub_82295C98) answers without waiting: 996 while
// pending, else the status (the information to `result` when given).
uint32_t OverlappedResult(uint8_t* base, uint32_t overlapped, uint32_t result = 0) {
  uint32_t status = guest::LoadU32(base, overlapped);
  if (status == kPending) {
    return kIncomplete;
  }
  if (result) {
    guest::StoreU32(base, result, guest::LoadU32(base, overlapped + 4));
  }
  return status;
}

std::string GuestString(uint8_t* base, uint32_t address, uint32_t max) {
  std::string s;
  for (uint32_t i = 0; i < max; i++) {
    char c = char(guest::LoadU8(base, address + i));
    if (!c) {
      break;
    }
    s += c;
  }
  return s;
}

// The save slot's content data (+164) for its device: the display name
// (+504, UTF-16, wcscpy) and the file name (+760, strcpy), as
// SaveSlot::Create and SaveSlot::Delete build it.
void BuildContentData(uint8_t* base, uint32_t slot) {
  const uint32_t data = slot + 164;
  Clear(base, data, kContentDataBytes);
  for (uint32_t i = 0;; i++) {
    uint8_t c = guest::LoadU8(base, slot + 760 + i);
    guest::StoreU8(base, data + 264 + i, c);
    if (!c) {
      break;
    }
  }
  for (uint32_t i = 0;; i += 2) {
    uint16_t c = guest::LoadU16(base, slot + 504 + i);
    guest::StoreU16(base, data + 8 + i, c);
    if (!c) {
      break;
    }
  }
  guest::StoreU32(base, data, guest::LoadU32(base, slot + 36));
  guest::StoreU32(base, data + 4, kSavedGame);
}

// The one save, as an enumerated XCONTENT_DATA at `address`: the device, the
// type, "GAME DATA   " and the file's date (the display name the game writes),
// and SystemData.bin.
void StoreSaveContentData(uint8_t* base, uint32_t address) {
  Clear(base, address, kContentDataBytes);
  guest::StoreU32(base, address, kDevice);
  guest::StoreU32(base, address + 4, kSavedGame);
  std::string display = "GAME DATA   " + saves::WrittenAt(kSaveFile).value_or("");
  for (uint32_t i = 0; i < display.size() && i < 127; i++) {
    guest::StoreU16(base, address + 8 + 2 * i, uint8_t(display[i]));
  }
  for (uint32_t i = 0; kContentName[i]; i++) {
    guest::StoreU8(base, address + 264 + i, uint8_t(kContentName[i]));
  }
}

// The file a read or write step opens: the path at +802 ("TXR360:\TXR_DATA_1").
std::string SlotFileName(uint8_t* base, uint32_t slot) {
  std::string path = GuestString(base, slot + 802, 128);
  auto name = saves::FileName(path);
  if (!name) {
    ITC_LOG_WARN("saves: not a save file: \"{}\"", path);
    return {};
  }
  return *name;
}

// The read and write steps' file overlapped (+472, 20 bytes cleared, its event
// +488 none) done with `bytes`, and no file or event handle (+496, +492).
void FinishFileStep(uint8_t* base, uint32_t slot, uint32_t bytes) {
  Clear(base, slot + 472, 20);
  Finish(base, slot + 472, 0, bytes);
  guest::StoreU32(base, slot + 492, 0);
  guest::StoreU32(base, slot + 496, 0xFFFFFFFF);
  guest::StoreU32(base, slot + 500, bytes);
}

// A read or write step's start: the content's create (+136) has to be done.
// 0 to poll again, -7 for an error (no handles), else 1 to go on.
int32_t WaitForCreate(uint8_t* base, uint32_t slot) {
  uint32_t result = OverlappedResult(base, slot + 136);
  if (result == 0) {
    return 1;
  }
  if (result == kIncomplete) {
    return 0;
  }
  guest::StoreU32(base, slot + 492, 0);
  guest::StoreU32(base, slot + 496, 0xFFFFFFFF);
  return -7;
}

}  // namespace

uint32_t CheckDevice(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A45C0 SaveSlot::CheckDevice");
  return guest::LoadU32(base, slot + 36);
}

int32_t WaitDeviceSelector(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A48B8 SaveSlot::WaitDeviceSelector");
  if (guest::LoadU32(base, slot + 8) == kPending) {
    return 4;
  }
  uint32_t result = OverlappedResult(base, slot + 8);
  if (guest::LoadU32(base, slot + 36) && result == 0) {
    guest::StoreU32(base, slot + 960, 0);
    guest::StoreU32(base, slot + 964, 1);
    StoreU64(base, slot + 40, kFreeBytes);
  } else if (result == kIncomplete) {
    return 4;
  } else {
    guest::StoreU32(base, slot + 960, 0);
    StoreU64(base, slot + 40, 0);
  }
  return 3;
}

int32_t ShowDeviceSelector(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A4960 SaveSlot::ShowDeviceSelector");
  Clear(base, slot + 8, kOverlappedBytes);
  guest::StoreU32(base, slot + 36, kDevice);
  Finish(base, slot + 8, 0);
  return 1;
}

int32_t Enumerate(uint8_t* base, uint32_t owner, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A49D0 SaveSlot::Enumerate");
  if (!guest::LoadU32(base, slot + 36)) {
    return -2;
  }
  const uint32_t count = guest::LoadU32(base, owner + 68);
  guest::StoreU32(base, slot + 88, count * kContentDataBytes);
  guest::StoreU32(base, slot + 76, 0);
  Clear(base, slot + 48, kOverlappedBytes);
  uint32_t found = 0;
  if (count && saves::Exists(kSaveFile)) {
    StoreSaveContentData(base, guest::LoadU32(base, slot + 80));
    found = 1;
  }
  Finish(base, slot + 48, 0, found);
  return 1;
}

int32_t WaitEnumerate(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A4A88 SaveSlot::WaitEnumerate");
  uint32_t result = OverlappedResult(base, slot + 48, slot + 92);
  if (result == 0) {
    guest::StoreU32(base, slot + 984, guest::LoadU32(base, slot + 92));
  } else if (result == kNoMoreFiles) {
    guest::StoreU32(base, slot + 984, 0);
  } else {
    return result == kIncomplete ? 0 : -6;
  }
  return 1;
}

int32_t OpenItem(uint8_t* base, uint32_t slot, uint32_t item_index) {
  ITC_NATIVE_RUNNING("sub_820A4B18 SaveSlot::OpenItem");
  if (!guest::LoadU32(base, slot + 36)) {
    return -2;
  }
  const uint32_t item = guest::LoadU32(base, slot + 80) + item_index * kContentDataBytes;
  Clear(base, slot + 100, kOverlappedBytes);
  if (!guest::LoadU32(base, item)) {
    return -5;
  }
  Finish(base, slot + 100, saves::Exists(kSaveFile) ? 0 : kFileNotFound);
  return 1;
}

int32_t WaitOpenItem(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A4B98 SaveSlot::WaitOpenItem");
  uint32_t result = OverlappedResult(base, slot + 100);
  return result == 0 ? 1 : result == kIncomplete ? 0 : -7;
}

int32_t Create(uint8_t* base, uint32_t owner, uint32_t slot, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_820A4C58 SaveSlot::Create");
  if (!guest::LoadU32(base, slot + 36)) {
    return -2;
  }
  if (mode == 3) {
    // XContentCalculateSize (game code, sub_82295C18's plain C++) of the
    // owner's +60 and +64.
    const uint64_t size = save_interface::ContentSize(guest::LoadU32(base, owner + 60),
                                                      guest::LoadU32(base, owner + 64));
    if (kFreeBytes < size) {
      return -10;
    }
  }
  Clear(base, slot + 136, kOverlappedBytes);
  BuildContentData(base, slot);
  const bool create_always = mode == 1;
  Finish(base, slot + 136, create_always || saves::Exists(kSaveFile) ? 0 : kFileNotFound);
  return 1;
}

int32_t WriteFile(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A4D68 SaveSlot::WriteFile");
  if (int32_t waited = WaitForCreate(base, slot); waited != 1) {
    return waited;
  }
  const uint32_t data = guest::LoadU32(base, slot + 932), size = guest::LoadU32(base, slot + 936);
  std::string name = SlotFileName(base, slot);
  std::vector<uint8_t> bytes(size);
  for (uint32_t i = 0; i < size; i++) {
    bytes[i] = guest::LoadU8(base, data + i);
  }
  if (name.empty() || !saves::Write(name, bytes.data(), size)) {
    ITC_LOG_WARN("saves: can't write the save \"{}\" ({} bytes)", name, size);
    guest::StoreU32(base, slot + 492, 0);
    guest::StoreU32(base, slot + 496, 0xFFFFFFFF);
    return -3;
  }
  FinishFileStep(base, slot, size);
  return 1;
}

int32_t ReadFile(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A4EF8 SaveSlot::ReadFile");
  if (int32_t waited = WaitForCreate(base, slot); waited != 1) {
    return waited;
  }
  const uint32_t data = guest::LoadU32(base, slot + 932), size = guest::LoadU32(base, slot + 936);
  std::string name = SlotFileName(base, slot);
  auto bytes = name.empty() ? std::nullopt : saves::Read(name, size);
  if (!bytes) {
    ITC_LOG_WARN("saves: can't read the save \"{}\"", name);
    guest::StoreU32(base, slot + 492, 0);
    guest::StoreU32(base, slot + 496, 0xFFFFFFFF);
    return -3;
  }
  for (uint32_t i = 0; i < bytes->size(); i++) {
    guest::StoreU8(base, data + i, (*bytes)[i]);
  }
  FinishFileStep(base, slot, uint32_t(bytes->size()));
  return 1;
}

int32_t SetThumbnail(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5110 SaveSlot::SetThumbnail");
  if (!guest::LoadU32(base, slot + 36)) {
    return -2;
  }
  Clear(base, slot + 136, kOverlappedBytes);
  Finish(base, slot + 136, 0);
  return 1;
}

int32_t Delete(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A51E0 SaveSlot::Delete");
  if (!guest::LoadU32(base, slot + 36)) {
    return -2;
  }
  Clear(base, slot + 136, kOverlappedBytes);
  BuildContentData(base, slot);
  bool deleted = saves::Delete(kSaveFile);
  Finish(base, slot + 136, deleted ? 0 : kFileNotFound);
  return 1;
}

uint32_t ContentClose(uint8_t* base, uint32_t overlapped) {
  ITC_NATIVE_RUNNING("sub_82295BF0 XContentClose");
  if (overlapped) {
    Finish(base, overlapped, 0);
    return kPending;
  }
  return 0;
}

}  // namespace saves::slot
