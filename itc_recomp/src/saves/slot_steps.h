// The save slots' steps on our own saves/ folder (saves.h), as plain C++
// native callers can call (the save devices' slot handlers,
// src/save_interface/slot_handlers.h): the steps that started Xbox calls (the
// device selector, the device's data, the content enumeration, create, open,
// delete and close, the thumbnail, and the save file's CreateFile, ReadFile
// and WriteFile) do their work at once and mark the step's overlapped done,
// so the waits pass on their next poll without a kernel call. One device,
// room enough, one save (SystemData.bin holding TXR_DATA_1). No file or event
// handles are made. Their adapters, for recompiled callers, are in
// src/hooks/saves.cpp (B4, no flag). See B4 in docs/rexglue-removal.md and
// docs/functions/files-and-saves.md; the slot's fields are in
// docs/known-data.md.
//
// `slot` is a save slot's address (1,000 bytes), `owner` the save devices.
// Each returns the r3 the step answers (sign-extended by the adapters).

#pragma once

#include <cstdint>

namespace saves::slot {

// sub_820A45C0 SaveSlot::CheckDevice: the device (+36), never removed.
uint32_t CheckDevice(uint8_t* base, uint32_t slot);
// sub_820A48B8 SaveSlot::WaitDeviceSelector (the slot handler for state 4):
// 4 while the selector (+8) is pending; else, with a device: +960 = 0, +964
// = 1, its free bytes at +40; without: +960 = 0, +40 = 0. Returns 3.
int32_t WaitDeviceSelector(uint8_t* base, uint32_t slot);
// sub_820A4960 SaveSlot::ShowDeviceSelector: no dialog: the device chosen at
// once (+36), the selector's overlapped (+8) done. Returns 1.
int32_t ShowDeviceSelector(uint8_t* base, uint32_t slot);
// sub_820A49D0 SaveSlot::Enumerate (+68 of the owner the count asked for):
// -2 without a device; else the saves found into the buffer (+80), the
// enumeration's overlapped (+48) done with the count. Returns 1.
int32_t Enumerate(uint8_t* base, uint32_t owner, uint32_t slot);
// sub_820A4A88 SaveSlot::WaitEnumerate: the count found (+92, +984; 0 for
// 1627) and 1; 0 while pending, -6 for an error.
int32_t WaitEnumerate(uint8_t* base, uint32_t slot);
// sub_820A4B18 SaveSlot::OpenItem: `item` the index in the enumeration
// buffer. -2 without a device, -5 for an item with none; else the open
// (+100) done (found when the save's file exists) and 1.
int32_t OpenItem(uint8_t* base, uint32_t slot, uint32_t item);
// sub_820A4B98 SaveSlot::WaitOpenItem: 1 when the open (+100) is done (the
// close has nothing to do), 0 while pending, -7 for an error.
int32_t WaitOpenItem(uint8_t* base, uint32_t slot);
// sub_820A4C58 SaveSlot::Create: `mode` 1 the save (create always), else
// open existing; 3 checks the space (XContentCalculateSize's plain C++ of
// the owner's +60 and +64) too. -2 without a device, -10 without room; else
// the content data at +164 and the create (+136) done, and 1.
int32_t Create(uint8_t* base, uint32_t owner, uint32_t slot, uint32_t mode);
// sub_820A4D68 SaveSlot::WriteFile: once the create (+136) is done (0
// pending, -7 an error), the data (+932, +936 bytes) written as the file the
// path at +802 names (-3 if that fails); the file overlapped (+472) done.
// Returns 1.
int32_t WriteFile(uint8_t* base, uint32_t slot);
// sub_820A4EF8 SaveSlot::ReadFile: the same for reading into +932.
int32_t ReadFile(uint8_t* base, uint32_t slot);
// sub_820A5110 SaveSlot::SetThumbnail: -2 without a device; else its step
// (+136) done (no thumbnail kept) and 1.
int32_t SetThumbnail(uint8_t* base, uint32_t slot);
// sub_820A51E0 SaveSlot::Delete: -2 without a device; else the content data
// at +164, the save's file deleted, its step (+136) done and 1.
int32_t Delete(uint8_t* base, uint32_t slot);
// sub_82295BF0 XContentClose: nothing to close: `overlapped` done and the
// SDK's answer (997; 0 without one).
uint32_t ContentClose(uint8_t* base, uint32_t overlapped);

}  // namespace saves::slot
