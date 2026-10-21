// The save slots' steps on our own saves/ folder (src/saves/saves.h): the
// steps that started Xbox calls (the device selector, the device's data, the
// content enumeration, create, open, delete, close and the thumbnail, and the
// save file's CreateFile, ReadFile and WriteFile) do their work at once and
// mark the step's overlapped done, so the game's waits, left recompiled, pass
// on their next poll without a kernel call. One device, room enough, one save
// (SystemData.bin holding TXR_DATA_1). No file or event handles are made, so
// the close step's wait (sub_820A5070) has none to close. See B4 in
// docs/rexglue-removal.md and docs/functions/files-and-saves.md; the slot's
// fields are in docs/known-data.md.

#include "guest.h"

#include "saves/saves.h"

#include "base/log.h"

#include <string>

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

// SaveSlot::CheckDevice: r3 = slot. The device (+36) is never removed; returns it.
extern "C" REX_FUNC(sub_820A45C0) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = guest::LoadU32(base, ctx.r3.u32 + 36);
}

// SaveSlot::WaitDeviceSelector: r4 = slot. 4 while the selector (+8) is
// pending; else, with a device: +960 = 0, +964 = 1, its free bytes at +40;
// without: +960 = 0, +40 = 0. Returns 3.
extern "C" REX_FUNC(sub_820A48B8) {
  REX_FUNC_PROLOGUE();
  const uint32_t slot = ctx.r4.u32;
  if (guest::LoadU32(base, slot + 8) == kPending) {
    ctx.r3.u64 = 4;
    return;
  }
  uint32_t result = OverlappedResult(base, slot + 8);
  if (guest::LoadU32(base, slot + 36) && result == 0) {
    guest::StoreU32(base, slot + 960, 0);
    guest::StoreU32(base, slot + 964, 1);
    StoreU64(base, slot + 40, kFreeBytes);
  } else if (result == kIncomplete) {
    ctx.r3.u64 = 4;
    return;
  } else {
    guest::StoreU32(base, slot + 960, 0);
    StoreU64(base, slot + 40, 0);
  }
  ctx.r3.u64 = 3;
}

// SaveSlot::ShowDeviceSelector: r3 = owner, r4 = slot. No dialog: the device
// chosen at once (+36), the selector's overlapped (+8) done. Returns 1.
extern "C" REX_FUNC(sub_820A4960) {
  REX_FUNC_PROLOGUE();
  const uint32_t slot = ctx.r4.u32;
  Clear(base, slot + 8, kOverlappedBytes);
  guest::StoreU32(base, slot + 36, kDevice);
  Finish(base, slot + 8, 0);
  ctx.r3.u64 = 1;
}

// SaveSlot::Enumerate: r3 = owner (+68 the count asked for), r4 = slot. -2
// without a device. The buffer's size (+88, 308 bytes an item), no enumerator
// (+76), and the saves found into the buffer (+80): the one save when its
// file exists. The enumeration's overlapped (+48) done with the count.
// Returns 1.
extern "C" REX_FUNC(sub_820A49D0) {
  REX_FUNC_PROLOGUE();
  const uint32_t owner = ctx.r3.u32, slot = ctx.r4.u32;
  if (!guest::LoadU32(base, slot + 36)) {
    ctx.r3.u64 = uint64_t(-2);
    return;
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
  ctx.r3.u64 = 1;
}

// The enumeration's wait: r4 = slot. The count found (+92, +984; 0 for 1627);
// 0 while pending, -6 for an error. No enumerator to close. Returns 1.
extern "C" REX_FUNC(sub_820A4A88) {
  REX_FUNC_PROLOGUE();
  const uint32_t slot = ctx.r4.u32;
  uint32_t result = OverlappedResult(base, slot + 48, slot + 92);
  if (result == 0) {
    guest::StoreU32(base, slot + 984, guest::LoadU32(base, slot + 92));
  } else if (result == kNoMoreFiles) {
    guest::StoreU32(base, slot + 984, 0);
  } else {
    ctx.r3.u64 = result == kIncomplete ? 0 : uint64_t(-6);
    return;
  }
  ctx.r3.u64 = 1;
}

// SaveSlot::OpenItem: r3 = owner, r4 = slot, r5 = the item's index in the
// enumeration buffer (+80). -2 without a device, -5 for an item with none.
// The open (+100) done: found when the save's file exists. Returns 1.
extern "C" REX_FUNC(sub_820A4B18) {
  REX_FUNC_PROLOGUE();
  const uint32_t slot = ctx.r4.u32;
  if (!guest::LoadU32(base, slot + 36)) {
    ctx.r3.u64 = uint64_t(-2);
    return;
  }
  const uint32_t item = guest::LoadU32(base, slot + 80) + ctx.r5.u32 * kContentDataBytes;
  Clear(base, slot + 100, kOverlappedBytes);
  if (!guest::LoadU32(base, item)) {
    ctx.r3.u64 = uint64_t(-5);
    return;
  }
  Finish(base, slot + 100, saves::Exists(kSaveFile) ? 0 : kFileNotFound);
  ctx.r3.u64 = 1;
}

// SaveSlot::WaitOpenItem: r3 = owner, r4 = slot. When the open (+100) is
// done, closing the content has nothing to do (+100 stays done); 1. 0 while
// pending, -7 for an error.
extern "C" REX_FUNC(sub_820A4B98) {
  REX_FUNC_PROLOGUE();
  uint32_t result = OverlappedResult(base, ctx.r4.u32 + 100);
  ctx.r3.u64 = result == 0 ? 1 : result == kIncomplete ? 0 : uint64_t(-7);
}

// SaveSlot::Create: r3 = owner, r4 = slot, r5 = mode (1 the save: create
// always; else open existing; 3 checks the space too). -2 without a device,
// -10 without room. The content data built at +164 as the game does; the
// create (+136) done: open existing finds the save only when its file exists.
// Returns 1.
extern "C" REX_FUNC(sub_820A4C58) {
  REX_FUNC_PROLOGUE();
  const uint32_t owner = ctx.r3.u32, slot = ctx.r4.u32, mode = ctx.r5.u32;
  if (!guest::LoadU32(base, slot + 36)) {
    ctx.r3.u64 = uint64_t(-2);
    return;
  }
  if (mode == 3) {
    // XContentCalculateSize (game code) of the owner's +60 and +64.
    ctx.r3.u64 = guest::LoadU32(base, owner + 60);
    ctx.r4.u64 = guest::LoadU32(base, owner + 64);
    sub_82295C18(ctx, base);
    if (kFreeBytes < ctx.r3.u64) {
      ctx.r3.u64 = uint64_t(-10);
      return;
    }
  }
  Clear(base, slot + 136, kOverlappedBytes);
  BuildContentData(base, slot);
  const bool create_always = mode == 1;
  Finish(base, slot + 136, create_always || saves::Exists(kSaveFile) ? 0 : kFileNotFound);
  ctx.r3.u64 = 1;
}

// SaveSlot::WriteFile: r4 = slot. Once the create (+136) is done, writes the
// data (+932, +936 bytes) as the file the path at +802 names; -3 if that
// fails. The file overlapped (+472) done. Returns 1.
extern "C" REX_FUNC(sub_820A4D68) {
  REX_FUNC_PROLOGUE();
  const uint32_t slot = ctx.r4.u32;
  if (int32_t waited = WaitForCreate(base, slot); waited != 1) {
    ctx.r3.u64 = uint64_t(int64_t(waited));
    return;
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
    ctx.r3.u64 = uint64_t(-3);
    return;
  }
  FinishFileStep(base, slot, size);
  ctx.r3.u64 = 1;
}

// SaveSlot::ReadFile: r4 = slot. Once the create (+136) is done, reads the
// file the path at +802 names into +932 (at most +936 bytes); -3 if it can't
// be read. The file overlapped (+472) done with the bytes read. Returns 1.
extern "C" REX_FUNC(sub_820A4EF8) {
  REX_FUNC_PROLOGUE();
  const uint32_t slot = ctx.r4.u32;
  if (int32_t waited = WaitForCreate(base, slot); waited != 1) {
    ctx.r3.u64 = uint64_t(int64_t(waited));
    return;
  }
  const uint32_t data = guest::LoadU32(base, slot + 932), size = guest::LoadU32(base, slot + 936);
  std::string name = SlotFileName(base, slot);
  auto bytes = name.empty() ? std::nullopt : saves::Read(name, size);
  if (!bytes) {
    ITC_LOG_WARN("saves: can't read the save \"{}\"", name);
    guest::StoreU32(base, slot + 492, 0);
    guest::StoreU32(base, slot + 496, 0xFFFFFFFF);
    ctx.r3.u64 = uint64_t(-3);
    return;
  }
  for (uint32_t i = 0; i < bytes->size(); i++) {
    guest::StoreU8(base, data + i, (*bytes)[i]);
  }
  FinishFileStep(base, slot, uint32_t(bytes->size()));
  ctx.r3.u64 = 1;
}

// SaveSlot::SetThumbnail: r4 = slot. -2 without a device. No thumbnail kept;
// its step (+136) done. Returns 1.
extern "C" REX_FUNC(sub_820A5110) {
  REX_FUNC_PROLOGUE();
  const uint32_t slot = ctx.r4.u32;
  if (!guest::LoadU32(base, slot + 36)) {
    ctx.r3.u64 = uint64_t(-2);
    return;
  }
  Clear(base, slot + 136, kOverlappedBytes);
  Finish(base, slot + 136, 0);
  ctx.r3.u64 = 1;
}

// SaveSlot::Delete: r4 = slot. -2 without a device. The content data built at
// +164 as the game does, the save's file deleted; its step (+136) done.
// Returns 1.
extern "C" REX_FUNC(sub_820A51E0) {
  REX_FUNC_PROLOGUE();
  const uint32_t slot = ctx.r4.u32;
  if (!guest::LoadU32(base, slot + 36)) {
    ctx.r3.u64 = uint64_t(-2);
    return;
  }
  Clear(base, slot + 136, kOverlappedBytes);
  BuildContentData(base, slot);
  bool deleted = saves::Delete(kSaveFile);
  Finish(base, slot + 136, deleted ? 0 : kFileNotFound);
  ctx.r3.u64 = 1;
}

// XContentClose: r3 = the content's root name, r4 = overlapped. Nothing to
// close: the overlapped done, and the SDK's answer (997; 0 without one).
// Called inline by the save and load steps (sub_820A5BE8, sub_820A5E68).
extern "C" REX_FUNC(sub_82295BF0) {
  REX_FUNC_PROLOGUE();
  if (ctx.r4.u32) {
    Finish(base, ctx.r4.u32, 0);
    ctx.r3.u64 = kPending;
  } else {
    ctx.r3.u64 = 0;
  }
}
