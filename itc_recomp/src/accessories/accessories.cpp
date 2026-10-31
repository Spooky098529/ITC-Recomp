// The interior accessories (docs/functions/accessories.md): see
// accessories.h.

#include "accessories/accessories.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/part_model.h"
#include "models/texture_provider.h"
#include "native/buffers.h"

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

// A callee whose checks the adapter made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Accessories: {} refused after its checks", what);
    std::abort();
  }
}

// The identity's four rows as the originals' stvx leave them at `rows`
// (16-byte aligned down, as stvx).
void StoreIdentity(uint8_t* base, uint32_t rows, double one) {
  const uint32_t at = rows & ~0xFu;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      if (row == column) {
        Stfs(base, at + 16 * row + 4 * column, one);
      } else {
        Store(base, at + 16 * row + 4 * column, 0);
      }
    }
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

bool FileResetNative(uint8_t* base, uint32_t file) {
  if (Load(base, file + 24) == 0 || Load(base, file + 12) != 2) {
    return true;
  }
  const uint32_t block = Load(base, file + 16);
  return block == 0 || memory::IsPages(block);
}

std::optional<uint32_t> FileReset(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0D30 AccessoryFile::Reset");
  std::optional<uint32_t> r3;
  if (Load(base, file + 24) != 0) {
    const uint32_t state = Load(base, file + 12);
    if (state == 1) {
      r3 = archive::Cancel(base, Load(base, file + 8));
    } else if (state == 2) {
      r3 = FileFreeData(base, file + 16);
      Require(r3.has_value(), "AccessoryFile::FreeData");
    }
  }
  Store(base, file + 12, 0);
  Store(base, file + 4, 0xFFFFFFFFu);
  Store(base, file + 8, 0xFFFFFFFFu);
  Store(base, file + 16, 0);
  Store(base, file + 20, 0);
  Store(base, file + 24, 1);
  Store(base, file + 28, 0);
  Store(base, file + 32, 0);
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

uint32_t SetFree(uint8_t* base, uint32_t slot, uint32_t sp,
                 const std::function<void(uint32_t set, uint32_t frame)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820F0F90 Accessories::Free");
  const uint32_t set = Load(base, slot);
  if (set == 0) {
    return 0;
  }
  const uint32_t frame = sp - 96;
  Store(base, frame, sp);  // the back chain, for the destructor's guest call
  destroy(set, frame);
  const auto r3 = FileFreeData(base, slot);
  Require(r3.has_value(), "AccessoryFile::FreeData");
  return *r3;
}

uint64_t SetDestroy(uint8_t* base, uint32_t sp,
                    const std::function<void(uint32_t set, uint32_t frame)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820F1078 Accessories::Destroy");
  if (Load(base, kSet) == 0) {
    return uint64_t(int64_t(int32_t(kSet)));  // lis and addi: sign-extended
  }
  return SetFree(base, kSet, sp, destroy);
}

// --- The entry and the accessory ---

bool AccessoryConstructible(uint8_t* base) {
  return models::CanAllocatePointers(base, models::kTextureProviderSlots);
}

uint32_t SkeletonConstruct(uint8_t* base, uint32_t skeleton, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820F03F0 AccessorySkeleton::AccessorySkeleton");
  const uint32_t frame = sp - 224;
  model_animation::SkeletonBaseConstruct(base, skeleton);
  Store(base, skeleton + 176, kListNodeVtable);
  Store(base, skeleton + 0, kSkeletonVtable);
  Store(base, skeleton + 176, kSkeletonVtable2);
  FileConstruct(base, skeleton + 180);
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  // The job (+216): its functor's vtable (+220) set twice, its flags.
  const uint32_t job = skeleton + 216;
  Store(base, job + 4, kJobFunctorFirst);
  guest::StoreU8(base, job + 17, 1);
  Store(base, job + 0, kJobVtable);
  Store(base, job + 4, kJobFunctor);
  Store(base, frame + 80, 0);
  Store(base, frame + 84, 0);
  Store(base, job + 8, 0);
  Store(base, job + 12, 0);
  guest::StoreU8(base, job + 16, 0);
  guest::StoreU8(base, job + 18, 0);
  Store(base, job + 32, Load(base, frame + 80));
  Store(base, job + 36, Load(base, frame + 84));
  Store(base, job + 24, kJobDelegate);
  Store(base, job + 28, 0);
  const uint32_t matrix = skeleton + 288, last = skeleton + 352;
  for (uint32_t offset : {12u, 28u, 44u, 60u}) {
    Stfs(base, matrix + offset, one);
  }
  for (uint32_t offset : {12u, 28u, 44u, 60u}) {
    Stfs(base, last + offset, one);
  }
  Stfs(base, skeleton + 428, one);
  Stfs(base, skeleton + 444, one);
  Store(base, skeleton + 264, 1);
  Stfs(base, skeleton + 268, zero);
  Stfs(base, skeleton + 272, zero);
  Store(base, skeleton + 260, 0);
  Store(base, skeleton + 256, 0);
  StoreIdentity(base, frame + 96, 1.0);
  Require(native::Buffers_Memcpy(base, matrix, frame + 96, 64, 0x820F0544), "memcpy");
  StoreIdentity(base, frame + 96, 1.0);
  Require(native::Buffers_Memcpy(base, last, frame + 96, 64, 0x820F0588), "memcpy");
  for (uint32_t offset : {424u, 420u, 416u}) {
    Stfs(base, skeleton + offset, zero);
  }
  Stfs(base, skeleton + 428, one);
  for (uint32_t offset : {440u, 436u, 432u}) {
    Stfs(base, skeleton + offset, zero);
  }
  Stfs(base, skeleton + 444, one);
  return skeleton;
}

uint32_t AccessoryConstruct(uint8_t* base, uint32_t accessory, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820EF258 Accessory::Accessory");
  const uint32_t frame = sp - 96;
  Require(models::PartModelConstruct(base, accessory), "PartModel::PartModel");
  Store(base, accessory + 0, kAccessoryVtable);
  FileConstruct(base, accessory + 464);
  FileConstruct(base, accessory + 500);
  SkeletonConstruct(base, accessory + 544, frame);
  const double one = Lfs(base, kOne);
  for (uint32_t offset : {1004u, 1020u, 1036u, 1004u}) {
    Stfs(base, accessory + offset, one);
  }
  const double zero = Lfs(base, kZero);
  for (uint32_t offset : {1000u, 996u, 992u, 1016u, 1012u, 1008u}) {
    Stfs(base, accessory + offset, zero);
  }
  for (uint32_t offset : {1020u, 1024u, 1028u, 1032u, 1036u}) {
    Stfs(base, accessory + offset, one);
  }
  Store(base, accessory + 1044, 1);
  Store(base, accessory + 1040, 0);
  Store(base, accessory + 1048, 0);
  return accessory;
}

uint32_t EntryNew(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820F0920 AccessoryEntry::New");
  const uint32_t entry = VirtualAlloc(kEntryBytes);
  if (entry == 0) {
    return 0;
  }
  Store(base, entry + 0, 1);
  FileConstruct(base, entry + 4);
  AccessoryConstruct(base, entry + 48, sp - 96);
  return entry;
}

// --- The destructors ---

constexpr uint32_t kJobDelegateBase = 0x82000ABC;  // the functors' base
constexpr uint32_t kJobFunctorBase = 0x82000BA8;
constexpr uint32_t kJobBaseVtable = 0x82000EB8;

bool FileDestructible(uint8_t* base, uint32_t file) {
  return FileResetNative(base, file);
}

bool SkeletonDestructible(uint8_t* base, uint32_t skeleton) {
  return model_animation::SkeletonBaseDestructible(base, skeleton) &&
         FileDestructible(base, skeleton + 180);
}

bool AccessoryDestructible(uint8_t* base, uint32_t accessory) {
  return SkeletonDestructible(base, accessory + 544) && FileDestructible(base, accessory + 500) &&
         FileDestructible(base, accessory + 464) &&
         models::ModelMembersDestructible(base, accessory) &&
         models::PendingFreesHaveRoom(base, models::ModelReleaseNodes(base, accessory));
}

bool EntryDestroyable(uint8_t* base, uint32_t slot) {
  const uint32_t entry = Load(base, slot);
  return entry == 0 || (memory::IsPages(entry) && AccessoryDestructible(base, entry + 48) &&
                        FileDestructible(base, entry + 4));
}

uint32_t FileDestruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0DB8 AccessoryFile::~AccessoryFile");
  Store(base, file + 0, kFileVtable);
  const uint32_t r3 = FileReset(base, file).value_or(file);
  Store(base, file + 0, kListNodeVtable);
  return r3;
}

uint32_t SkeletonDestruct(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_820EFFD8 AccessorySkeleton::~AccessorySkeleton");
  const uint32_t job = skeleton + 216;
  Store(base, skeleton + 0, kSkeletonVtable);
  Store(base, skeleton + 176, kSkeletonVtable2);
  Store(base, job + 24, kJobDelegateBase);
  Store(base, job + 4, kJobFunctorBase);
  Store(base, job + 4, kJobFunctorFirst);
  Store(base, job + 0, kJobBaseVtable);
  FileDestruct(base, skeleton + 180);
  Store(base, skeleton + 176, kListNodeVtable);
  Require(model_animation::SkeletonBaseDestruct(base, skeleton), "SkeletonBase::~SkeletonBase");
  return skeleton;
}

uint32_t AccessoryDestruct(uint8_t* base, uint32_t accessory) {
  ITC_NATIVE_RUNNING("sub_820EF310 Accessory::~Accessory");
  Store(base, accessory + 0, kAccessoryVtable);
  SkeletonDestruct(base, accessory + 544);
  FileDestruct(base, accessory + 500);
  FileDestruct(base, accessory + 464);
  const auto r3 = models::PartModelDestruct(base, accessory);
  Require(r3.has_value(), "PartModel::~PartModel");
  return *r3;
}

uint32_t EntryDestroy(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820F0988 AccessoryEntry::Destroy");
  const uint32_t entry = Load(base, slot);
  if (entry == 0) {
    return slot;
  }
  AccessoryDestruct(base, entry + 48);
  FileDestruct(base, entry + 4);
  const auto r3 = FileFreeData(base, slot);
  Require(r3.has_value(), "AccessoryFile::FreeData");
  return *r3;
}

bool SetReleasable(uint8_t* base, uint32_t set) {
  uint32_t entries[3];
  for (uint32_t i = 0; i < 3; i++) {
    entries[i] = Load(base, set + 4 + 4 * i);
    for (uint32_t k = 0; k < i; k++) {
      if (entries[i] != 0 && entries[i] == entries[k]) {
        return false;
      }
    }
    if (entries[i] != 0 && Load(base, entries[i]) - 1 == 0 &&
        !EntryDestroyable(base, set + 4 + 4 * i)) {
      return false;
    }
  }
  return true;
}

std::optional<uint32_t> SetReleaseAll(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F0AD8 Accessories::ReleaseAll");
  std::optional<uint32_t> r3;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = set + 4 + 4 * i;
    const uint32_t entry = Load(base, slot);
    if (entry == 0) {
      continue;
    }
    Store(base, entry, Load(base, entry) - 1);
    if (Load(base, Load(base, slot)) == 0) {
      r3 = EntryDestroy(base, slot);
    }
  }
  return r3;
}

// --- The skeleton's set-up ---

uint32_t SkeletonReset(uint8_t* base, uint32_t skeleton, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820EFB30 AccessorySkeleton::Reset");
  const uint32_t frame = sp - 192;
  for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, Load(base, skeleton + 80) + 4)); i++) {
    const uint32_t entry = Load(base, skeleton + 136) + 144 * i;
    // Three rows (ld and std of two doublewords each, each row read first).
    for (uint32_t row = 0; row < 3; row++) {
      Copy16(base, entry + 16 * row, Load(base, skeleton + 88 + 4 * row) + 16 * i);
    }
    const uint32_t node = Load(base, skeleton + 84) + 160 * i;
    if (Load(base, node + 132) & 0x40) {
      Copy16(base, entry + 48, node + 64);
    }
  }
  const double zero = Lfs(base, kZero);
  Stfs(base, skeleton + 268, zero);
  Stfs(base, skeleton + 272, zero);
  StoreIdentity(base, frame + 80, 1.0);
  Require(native::Buffers_Memcpy(base, skeleton + 288, frame + 80, 64, 0x820EFC60), "memcpy");
  StoreIdentity(base, frame + 80, 1.0);
  Require(native::Buffers_Memcpy(base, skeleton + 352, frame + 80, 64, 0x820EFCA4), "memcpy");
  for (uint32_t offset : {424u, 420u, 416u}) {
    Stfs(base, skeleton + offset, zero);
  }
  const double one = Lfs(base, kOne);
  Stfs(base, skeleton + 428, one);
  for (uint32_t offset : {440u, 436u, 432u}) {
    Stfs(base, skeleton + offset, zero);
  }
  Stfs(base, skeleton + 444, one);
  Store(base, skeleton + 152, 0);
  return skeleton + 352;
}

std::optional<uint32_t> SkeletonSetUp(uint8_t* base, uint32_t skeleton, uint32_t sp) {
  if (Load(base, skeleton + 260) != 0 || Load(base, skeleton + 192) != 2) {
    ITC_NATIVE_RUNNING("sub_820F0070 AccessorySkeleton::SetUp");
    return Load(base, skeleton + 260);
  }
  if (!model_animation::SkeletonSetUp(base, skeleton, Load(base, skeleton + 196), 0, 1)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820F0070 AccessorySkeleton::SetUp");
  const uint32_t view = skeleton + 80;
  if (int32_t(Load(base, Load(base, view) + 4)) > 0) {
    for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, Load(base, view) + 4)); i++) {
      if (c_runtime::Strstr(base, Load(base, skeleton + 84) + 160 * i, kSwingNodeName) >= 0) {
        Store(base, skeleton + 256, i);
        break;
      }
    }
    Store(base, skeleton + 260, 1);
    SkeletonReset(base, skeleton, sp - 128);
  }
  return Load(base, skeleton + 260);
}

}  // namespace accessories
