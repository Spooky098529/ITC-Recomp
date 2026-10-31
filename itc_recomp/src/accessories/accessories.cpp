// The interior accessories (docs/functions/accessories.md): see
// accessories.h.

#include "accessories/accessories.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"

#include <cstring>

namespace accessories {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// lfs: a single widened to a double, as the translation does.
double Lfs(uint8_t* base, uint32_t address) {
  const uint32_t bits = Load(base, address);
  float value;
  std::memcpy(&value, &bits, 4);
  return double(value);
}
// stfs: a double rounded to single.
void Stfs(uint8_t* base, uint32_t address, double value) {
  const float single = float(value);
  uint32_t bits;
  std::memcpy(&bits, &single, 4);
  Store(base, address, bits);
}

// VirtualAlloc(0, size, 0x801000, 4) through the allocation layer (the
// adapters check it routes there).
uint32_t VirtualAlloc(uint32_t size) {
  return memory::AllocPages(size, 4096);
}

// ld and std of two doublewords: 16 bytes copied, the first eight read
// before they're stored, then the second.
void Copy16(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t half = 0; half < 16; half += 8) {
    const uint32_t high = Load(base, from + half), low = Load(base, from + half + 4);
    Store(base, to + half, high);
    Store(base, to + half + 4, low);
  }
}

}  // namespace

// --- The file ---

void FileConstruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0C50 AccessoryFile::AccessoryFile");
  Store(base, file + 4, 0xFFFFFFFFu);
  Store(base, file + 0, kFileVtable);
  Store(base, file + 8, 0xFFFFFFFFu);
  Store(base, file + 12, 0);
  Store(base, file + 16, 0);
  Store(base, file + 20, 0);
  Store(base, file + 24, 1);
  Store(base, file + 28, 0);
  Store(base, file + 32, 0);
}

void FileSetData(uint8_t* base, uint32_t file, uint32_t data, uint32_t owned) {
  ITC_NATIVE_RUNNING("sub_820F0BF0 AccessoryFile::SetData");
  Store(base, file + 16, data);
  Store(base, file + 24, owned);
  Store(base, file + 20, 0);
  Store(base, file + 4, 0xFFFFFFFFu);
  Store(base, file + 8, 0xFFFFFFFFu);
  Store(base, file + 12, 2);
  Store(base, file + 28, 0);
  Store(base, file + 32, 0);
}

int64_t FileLoad(uint8_t* base, uint32_t file, uint32_t id, uint32_t buffer, uint32_t callback,
                 uint32_t context, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820F0C90 AccessoryFile::Load");
  Store(base, file + 4, id);
  const uint32_t size = archive::AssetSize(base, id);
  Store(base, file + 20, size);
  if (buffer != 0) {
    Store(base, file + 16, buffer);
    Store(base, file + 24, 0);
  } else {
    Store(base, file + 16, VirtualAlloc(size));
    Store(base, file + 24, 1);
  }
  const int64_t serial = load(Load(base, file + 4), Load(base, file + 16), kFileOnArchiveDone, file);
  Store(base, file + 12, 1);
  Store(base, file + 28, callback);
  Store(base, file + 32, context);
  Store(base, file + 8, uint32_t(serial));
  return serial;
}

Callback FileOnArchiveDone(uint8_t* base, uint32_t serial, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0C20 AccessoryFile::OnArchiveDone");
  if (serial != Load(base, file + 8)) {
    return {0, 0};
  }
  Store(base, file + 12, 2);
  const uint32_t function = Load(base, file + 28);
  if (function == 0) {
    return {0, 0};
  }
  return {function, Load(base, file + 32)};
}

std::optional<uint32_t> FileFreeData(uint8_t* base, uint32_t slot) {
  const uint32_t block = Load(base, slot);
  if (block != 0 && !memory::IsPages(block)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820F08D0 AccessoryFile::FreeData");
  if (block == 0) {
    return 0;
  }
  const uint32_t r3 = memory::FreePages(block) ? 1 : 0;
  Store(base, slot, 0);
  return r3;
}

// --- The set ---

void SetBaseConstruct(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F07B8 AccessoriesBase::AccessoriesBase");
  Store(base, set + 0, kSetBaseVtable);
  Store(base, set + 4, 0);
  Store(base, set + 8, 0);
  Store(base, set + 12, 0);
}

uint32_t SetConstruct(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F0E60 Accessories::Accessories");
  SetBaseConstruct(base, set);
  const double one = Lfs(base, kOne);
  Store(base, set + 0, kSetVtable);
  for (uint32_t offset : {28u, 16u, 20u, 24u, 28u}) {
    Stfs(base, set + offset, one);
  }
  return set;
}

uint32_t SetNew(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820F0F48 Accessories::New");
  const uint32_t set = VirtualAlloc(kSetBytes);
  if (set == 0) {
    return 0;
  }
  return SetConstruct(base, set);
}

std::optional<uint32_t> SetCreate(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820F1038 Accessories::Create");
  if (Load(base, kSet) != 0) {
    return std::nullopt;
  }
  const uint32_t set = SetNew(base);
  Store(base, kSet, set);
  return set;
}

void SetScale(uint8_t* base, uint32_t set, uint32_t scale) {
  ITC_NATIVE_RUNNING("sub_820F0F00 Accessories::SetScale");
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t entry = Load(base, set + 4 + 4 * i);
    if (entry != 0) {
      Copy16(base, entry + 1072, scale);
    }
  }
  Copy16(base, set + 16, scale);
}

}  // namespace accessories
