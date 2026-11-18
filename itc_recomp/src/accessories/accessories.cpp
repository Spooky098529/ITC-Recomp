// The interior accessories (docs/functions/accessories.md): see
// accessories.h.

#include "accessories/accessories.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "game_data/data_file.h"
#include "guest.h"
#include "matrix_math/matrix_math.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/part_model.h"
#include "models/texture_provider.h"
#include "native/buffers.h"
#include "schema/accessories.h"

#include <cstring>

namespace accessories {

namespace {

using schema::Accessories;
using schema::AccessoriesGlobals;
using schema::Accessory;
using schema::AccessoryEntry;
using schema::AccessoryFile;
using schema::AccessoryFloat3;
using schema::AccessoryMatrix;
using schema::AccessoryNodeLocal;
using schema::AccessoryNodeRecord;
using schema::AccessorySkeleton;

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
  const AccessoryFile f(base, file);
  f.set_asset_id(0xFFFFFFFFu);
  f.set_vtable(kFileVtable);
  f.set_serial(0xFFFFFFFFu);
  f.set_state(0);
  f.set_data(0);
  f.set_size(0);
  f.set_owned(1);
  f.set_callback(0);
  f.set_callback_context(0);
}

void FileSetData(uint8_t* base, uint32_t file, uint32_t data, uint32_t owned) {
  ITC_NATIVE_RUNNING("sub_820F0BF0 AccessoryFile::SetData");
  const AccessoryFile f(base, file);
  f.set_data(data);
  f.set_owned(owned);
  f.set_size(0);
  f.set_asset_id(0xFFFFFFFFu);
  f.set_serial(0xFFFFFFFFu);
  f.set_state(2);
  f.set_callback(0);
  f.set_callback_context(0);
}

int64_t FileLoad(uint8_t* base, uint32_t file, uint32_t id, uint32_t buffer, uint32_t callback,
                 uint32_t context, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820F0C90 AccessoryFile::Load");
  const AccessoryFile f(base, file);
  f.set_asset_id(id);
  const uint32_t size = archive::AssetSize(base, id);
  f.set_size(size);
  if (buffer != 0) {
    f.set_data(buffer);
    f.set_owned(0);
  } else {
    f.set_data(VirtualAlloc(size));
    f.set_owned(1);
  }
  const uint32_t asset = f.asset_id();
  const int64_t serial = load(asset, f.data(), kFileOnArchiveDone, file);
  f.set_state(1);
  f.set_callback(callback);
  f.set_callback_context(context);
  f.set_serial(uint32_t(serial));
  return serial;
}

Callback FileOnArchiveDone(uint8_t* base, uint32_t serial, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0C20 AccessoryFile::OnArchiveDone");
  const AccessoryFile f(base, file);
  if (serial != f.serial()) {
    return {0, 0};
  }
  f.set_state(2);
  const uint32_t function = f.callback();
  if (function == 0) {
    return {0, 0};
  }
  return {function, f.callback_context()};
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
  const AccessoryFile f(base, file);
  if (f.owned() == 0 || f.state() != 2) {
    return true;
  }
  const uint32_t block = f.data();
  return block == 0 || memory::IsPages(block);
}

std::optional<uint32_t> FileReset(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0D30 AccessoryFile::Reset");
  const AccessoryFile f(base, file);
  std::optional<uint32_t> r3;
  if (f.owned() != 0) {
    const uint32_t state = f.state();
    if (state == 1) {
      r3 = archive::Cancel(base, f.serial());
    } else if (state == 2) {
      r3 = FileFreeData(base, f.data_address());
      Require(r3.has_value(), "AccessoryFile::FreeData");
    }
  }
  f.set_state(0);
  f.set_asset_id(0xFFFFFFFFu);
  f.set_serial(0xFFFFFFFFu);
  f.set_data(0);
  f.set_size(0);
  f.set_owned(1);
  f.set_callback(0);
  f.set_callback_context(0);
  return r3;
}

// --- The set ---

void SetBaseConstruct(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F07B8 AccessoriesBase::AccessoriesBase");
  const Accessories s(base, set);
  s.set_vtable(kSetBaseVtable);
  s.set_entry_0(0u);
  s.set_entry_1(0u);
  s.set_entry_2(0u);
}

uint32_t SetConstruct(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F0E60 Accessories::Accessories");
  SetBaseConstruct(base, set);
  const Accessories s(base, set);
  const float one = AccessoriesGlobals::one(base);
  s.set_vtable(kSetVtable);
  s.set_scale_w(one);
  s.set_scale_x(one);
  s.set_scale_y(one);
  s.set_scale_z(one);
  s.set_scale_w(one);
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
  if (AccessoriesGlobals::set(base)) {
    return std::nullopt;
  }
  const uint32_t set = SetNew(base);
  AccessoriesGlobals::set_set(base, set);
  return set;
}

void SetScale(uint8_t* base, uint32_t set, uint32_t scale) {
  ITC_NATIVE_RUNNING("sub_820F0F00 Accessories::SetScale");
  const Accessories s(base, set);
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t entry = Load(base, s.entry_0_address() + 4 * i);
    if (entry != 0) {
      Copy16(base, AccessoryEntry(base, entry).accessory_scale_xy_address(), scale);
    }
  }
  Copy16(base, s.scale_xy_address(), scale);
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
  if (!AccessoriesGlobals::set(base)) {
    // lis and addi: sign-extended
    return uint64_t(int64_t(int32_t(AccessoriesGlobals::set_address())));
  }
  return SetFree(base, AccessoriesGlobals::set_address(), sp, destroy);
}

// --- The entry and the accessory ---

bool AccessoryConstructible(uint8_t* base) {
  return models::CanAllocatePointers(base, models::kTextureProviderSlots);
}

uint32_t SkeletonConstruct(uint8_t* base, uint32_t skeleton, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820F03F0 AccessorySkeleton::AccessorySkeleton");
  const uint32_t frame = sp - 224;
  const AccessorySkeleton s(base, skeleton);
  model_animation::SkeletonBaseConstruct(base, skeleton);
  s.set_vtable_2(kListNodeVtable);
  s.set_vtable(kSkeletonVtable);
  s.set_vtable_2(kSkeletonVtable2);
  FileConstruct(base, s.file_address());
  const float one = AccessoriesGlobals::one(base);
  const float zero = AccessoriesGlobals::zero(base);
  // The job (+216): its functor's vtable (+220) set twice, its flags.
  s.set_job_functor_vtable(kJobFunctorFirst);
  s.set_unknown_233(1);
  s.set_job_vtable(kJobVtable);
  s.set_job_functor_vtable(kJobFunctor);
  Store(base, frame + 80, 0);
  Store(base, frame + 84, 0);
  s.set_unknown_224(0);
  s.set_unknown_228(0);
  s.set_unknown_232(0);
  s.set_unknown_234(0);
  s.set_job_method(Load(base, frame + 80));
  s.set_job_method_extra(Load(base, frame + 84));
  s.set_job_delegate_vtable(kJobDelegate);
  s.set_job_target(0u);
  s.set_matrix_row0_w(one);
  s.set_matrix_row1_w(one);
  s.set_matrix_row2_w(one);
  s.set_matrix_row3_w(one);
  s.set_last_row0_w(one);
  s.set_last_row1_w(one);
  s.set_last_row2_w(one);
  s.set_last_row3_w(one);
  s.set_movement_w(one);
  s.set_swing_w(one);
  s.set_swinging(1);
  s.set_angle(zero);
  s.set_angle_speed(zero);
  s.set_set_up(0);
  s.set_swing_node(0);
  StoreIdentity(base, frame + 96, 1.0);
  Require(native::Buffers_Memcpy(base, s.matrix_address(), frame + 96, 64, 0x820F0544), "memcpy");
  StoreIdentity(base, frame + 96, 1.0);
  Require(native::Buffers_Memcpy(base, s.last_matrix_address(), frame + 96, 64, 0x820F0588),
          "memcpy");
  s.set_movement_z(zero);
  s.set_movement_y(zero);
  s.set_movement_x(zero);
  s.set_movement_w(one);
  s.set_swing_z(zero);
  s.set_swing_y(zero);
  s.set_swing_x(zero);
  s.set_swing_w(one);
  return skeleton;
}

uint32_t AccessoryConstruct(uint8_t* base, uint32_t accessory, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820EF258 Accessory::Accessory");
  const uint32_t frame = sp - 96;
  const Accessory a(base, accessory);
  Require(models::PartModelConstruct(base, accessory), "PartModel::PartModel");
  a.set_vtable(kAccessoryVtable);
  FileConstruct(base, a.model_file_address());
  FileConstruct(base, a.xpr_file_address());
  SkeletonConstruct(base, a.skeleton_address(), frame);
  const float one = AccessoriesGlobals::one(base);
  a.set_anchor_w(one);
  a.set_angle_w(one);
  a.set_scale_w(one);
  a.set_anchor_w(one);
  const float zero = AccessoriesGlobals::zero(base);
  a.set_anchor_z(zero);
  a.set_anchor_y(zero);
  a.set_anchor_x(zero);
  a.set_angle_z(zero);
  a.set_angle_y(zero);
  a.set_angle_x(zero);
  a.set_angle_w(one);
  a.set_scale_x(one);
  a.set_scale_y(one);
  a.set_scale_z(one);
  a.set_scale_w(one);
  a.set_visible(1);
  a.set_texture_index(0);
  a.set_loaded(0);
  return accessory;
}

uint32_t EntryNew(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820F0920 AccessoryEntry::New");
  const uint32_t entry = VirtualAlloc(kEntryBytes);
  if (entry == 0) {
    return 0;
  }
  const AccessoryEntry e(base, entry);
  e.set_count(1);
  FileConstruct(base, e.file_address());
  AccessoryConstruct(base, e.accessory_address(), sp - 96);
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
         FileDestructible(base, AccessorySkeleton(base, skeleton).file_address());
}

bool AccessoryDestructible(uint8_t* base, uint32_t accessory) {
  const Accessory a(base, accessory);
  return SkeletonDestructible(base, a.skeleton_address()) &&
         FileDestructible(base, a.xpr_file_address()) &&
         FileDestructible(base, a.model_file_address()) &&
         models::ModelMembersDestructible(base, accessory) &&
         models::PendingFreesHaveRoom(base, models::ModelReleaseNodes(base, accessory));
}

bool EntryDestroyable(uint8_t* base, uint32_t slot) {
  const uint32_t entry = Load(base, slot);
  const AccessoryEntry e(base, entry);
  return entry == 0 || (memory::IsPages(entry) && AccessoryDestructible(base, e.accessory_address()) &&
                        FileDestructible(base, e.file_address()));
}

uint32_t FileDestruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0DB8 AccessoryFile::~AccessoryFile");
  const AccessoryFile f(base, file);
  f.set_vtable(kFileVtable);
  const uint32_t r3 = FileReset(base, file).value_or(file);
  f.set_vtable(kListNodeVtable);
  return r3;
}

uint32_t SkeletonDestruct(uint8_t* base, uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_820EFFD8 AccessorySkeleton::~AccessorySkeleton");
  const AccessorySkeleton s(base, skeleton);
  s.set_vtable(kSkeletonVtable);
  s.set_vtable_2(kSkeletonVtable2);
  s.set_job_delegate_vtable(kJobDelegateBase);
  s.set_job_functor_vtable(kJobFunctorBase);
  s.set_job_functor_vtable(kJobFunctorFirst);
  s.set_job_vtable(kJobBaseVtable);
  FileDestruct(base, s.file_address());
  s.set_vtable_2(kListNodeVtable);
  Require(model_animation::SkeletonBaseDestruct(base, skeleton), "SkeletonBase::~SkeletonBase");
  return skeleton;
}

uint32_t AccessoryDestruct(uint8_t* base, uint32_t accessory) {
  ITC_NATIVE_RUNNING("sub_820EF310 Accessory::~Accessory");
  const Accessory a(base, accessory);
  a.set_vtable(kAccessoryVtable);
  SkeletonDestruct(base, a.skeleton_address());
  FileDestruct(base, a.xpr_file_address());
  FileDestruct(base, a.model_file_address());
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
  const AccessoryEntry e(base, entry);
  AccessoryDestruct(base, e.accessory_address());
  FileDestruct(base, e.file_address());
  const auto r3 = FileFreeData(base, slot);
  Require(r3.has_value(), "AccessoryFile::FreeData");
  return *r3;
}

bool SetReleasable(uint8_t* base, uint32_t set) {
  const Accessories s(base, set);
  uint32_t entries[3];
  for (uint32_t i = 0; i < 3; i++) {
    entries[i] = Load(base, s.entry_0_address() + 4 * i);
    for (uint32_t k = 0; k < i; k++) {
      if (entries[i] != 0 && entries[i] == entries[k]) {
        return false;
      }
    }
    if (entries[i] != 0 && AccessoryEntry(base, entries[i]).count() - 1 == 0 &&
        !EntryDestroyable(base, s.entry_0_address() + 4 * i)) {
      return false;
    }
  }
  return true;
}

std::optional<uint32_t> SetReleaseAll(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F0AD8 Accessories::ReleaseAll");
  const Accessories s(base, set);
  std::optional<uint32_t> r3;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = s.entry_0_address() + 4 * i;
    const uint32_t entry = Load(base, slot);
    if (entry == 0) {
      continue;
    }
    const AccessoryEntry e(base, entry);
    e.set_count(e.count() - 1);
    if (AccessoryEntry(base, Load(base, slot)).count() == 0) {
      r3 = EntryDestroy(base, slot);
    }
  }
  return r3;
}

namespace {

uint32_t ReleaseSlot(uint32_t set, int32_t index) {
  return set + ((uint32_t(index + 1) << 2) & 0xFFFFFFFCu);
}

}  // namespace

bool SetReleaseNative(uint8_t* base, uint32_t set, int32_t index) {
  if (index == 3) {
    return true;
  }
  const uint32_t slot = ReleaseSlot(set, index);
  const uint32_t entry = Load(base, slot);
  return entry == 0 || AccessoryEntry(base, entry).count() - 1 != 0 ||
         EntryDestroyable(base, slot);
}

std::optional<uint32_t> SetRelease(uint8_t* base, uint32_t set, int32_t index) {
  ITC_NATIVE_RUNNING("sub_820F0A90 Accessories::Release");
  if (index == 3) {
    return std::nullopt;
  }
  const uint32_t slot = ReleaseSlot(set, index);
  const uint32_t entry = Load(base, slot);
  if (entry == 0) {
    return slot;
  }
  const AccessoryEntry e(base, entry);
  e.set_count(e.count() - 1);
  if (AccessoryEntry(base, Load(base, slot)).count() != 0) {
    return slot;
  }
  return EntryDestroy(base, slot);
}

// --- The skeleton's set-up ---

uint32_t SkeletonReset(uint8_t* base, uint32_t skeleton, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820EFB30 AccessorySkeleton::Reset");
  const uint32_t frame = sp - 192;
  const AccessorySkeleton s(base, skeleton);
  for (uint32_t i = 0; int32_t(i) < int32_t(s.node_file().node_count()); i++) {
    const AccessoryNodeLocal entry(base, s.locals().address + 144 * i);
    // Three rows (ld and std of two doublewords each, each row read first).
    Copy16(base, entry.row0_xy_address(), s.rows_0().address + 16 * i);
    Copy16(base, entry.row1_xy_address(), s.rows_1().address + 16 * i);
    Copy16(base, entry.row2_xy_address(), s.rows_2().address + 16 * i);
    const AccessoryNodeRecord node(base, s.nodes().address + 160 * i);
    if (node.flags() & 0x40) {
      Copy16(base, entry.row3_xy_address(), node.rest_xy_address());
    }
  }
  const float zero = AccessoriesGlobals::zero(base);
  s.set_angle(zero);
  s.set_angle_speed(zero);
  StoreIdentity(base, frame + 80, 1.0);
  Require(native::Buffers_Memcpy(base, s.matrix_address(), frame + 80, 64, 0x820EFC60), "memcpy");
  StoreIdentity(base, frame + 80, 1.0);
  Require(native::Buffers_Memcpy(base, s.last_matrix_address(), frame + 80, 64, 0x820EFCA4),
          "memcpy");
  s.set_movement_z(zero);
  s.set_movement_y(zero);
  s.set_movement_x(zero);
  const float one = AccessoriesGlobals::one(base);
  s.set_movement_w(one);
  s.set_swing_z(zero);
  s.set_swing_y(zero);
  s.set_swing_x(zero);
  s.set_swing_w(one);
  s.set_stepped(0);
  return s.last_matrix_address();
}

std::optional<uint32_t> SkeletonSetUp(uint8_t* base, uint32_t skeleton, uint32_t sp) {
  const AccessorySkeleton s(base, skeleton);
  if (s.set_up() != 0 || s.file_state() != 2) {
    ITC_NATIVE_RUNNING("sub_820F0070 AccessorySkeleton::SetUp");
    return s.set_up();
  }
  if (!model_animation::SkeletonSetUp(base, skeleton, s.file_data(), 0, 1)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820F0070 AccessorySkeleton::SetUp");
  if (int32_t(s.node_file().node_count()) > 0) {
    for (uint32_t i = 0; int32_t(i) < int32_t(s.node_file().node_count()); i++) {
      const AccessoryNodeRecord node(base, s.nodes().address + 160 * i);
      if (c_runtime::Strstr(base, node.name_address(), kSwingNodeName) >= 0) {
        s.set_swing_node(i);
        break;
      }
    }
    s.set_set_up(1);
    SkeletonReset(base, skeleton, sp - 128);
  }
  return s.set_up();
}

uint32_t SetBaseDestruct(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F0B58 AccessoriesBase::~AccessoriesBase");
  const Accessories s(base, set);
  s.set_vtable(kSetBaseVtable);
  const uint32_t r3 = SetReleaseAll(base, set).value_or(set);
  s.set_vtable(kListNodeVtable);
  return r3;
}

uint32_t SetDestruct(uint8_t* base, uint32_t set, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820F0FE0 Accessories::~Accessories");
  Accessories(base, set).set_vtable(kSetVtable);
  SetBaseDestruct(base, set);
  if (flags & 1) {
    Require(memory::crt::OperatorDelete(set), "operator delete");
  }
  return set;
}

// --- The draw's helpers ---

uint64_t AccessorySetTexture(
    uint8_t* base, uint32_t accessory, uint32_t index, int32_t shape, uint32_t sp,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set) {
  ITC_NATIVE_RUNNING("sub_820EF568 Accessory::SetTexture");
  const Accessory a(base, accessory);
  a.set_texture_index(index);
  uint64_t r3 = accessory;
  if (a.loaded() == 0) {
    return r3;
  }
  const uint32_t frame = sp - 160;
  Store(base, frame, sp);  // the back chain, for the slot's guest call
  r3 = texture_set(a.textures_vtable_address(), frame);  // the provider at +80
  if (!(Load(base, uint32_t(r3) + 20) > index)) {
    return r3;
  }
  uint32_t first = uint32_t(shape), count = 1;
  if (shape < 0) {
    count = a.shape_count();
    first = 0;
  }
  for (uint32_t s = first; s < count; s++) {
    for (uint32_t g = 0; g < 2; g++) {
      const int32_t group = int32_t(g == 0 ? AccessoriesGlobals::material_group_0(base)
                                            : AccessoriesGlobals::material_group_1(base));
      const uint32_t materials = models::MaterialCount(base, accessory, s, group);
      r3 = materials;
      for (int32_t m = 0; m < int32_t(materials); m++) {
        const uint32_t material = models::Material(base, accessory, s, uint32_t(m), group);
        r3 = material;
        for (uint32_t k = 0; k < 8; k++) {
          Store(base, material + 176 + 4 * k, index);
          Store(base, material + 144 + 4 * k, 0);
        }
      }
    }
  }
  return r3;
}

uint32_t FromScaleRotationTranslation(uint8_t* base, uint32_t out, uint32_t angles,
                                      uint32_t position, uint32_t scale, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820EF660 Matrix::FromScaleRotationTranslation");
  const uint32_t frame = sp - 208;
  StoreIdentity(base, frame + 96, 1.0);
  Require(native::Buffers_Memcpy(base, out, frame + 96, 64, 0x820EF6C0), "memcpy");
  const AccessoryMatrix m(base, out);
  const AccessoryFloat3 s(base, scale), a(base, angles), p(base, position);
  Stfs(base, m.row0_x_address(), Lfs(base, s.x_address()));
  Stfs(base, m.row1_y_address(), Lfs(base, s.y_address()));
  Stfs(base, m.row2_z_address(), Lfs(base, s.z_address()));
  const uint32_t angle = frame + 80, turn = frame - 80;
  Stfs(base, angle, Lfs(base, a.x_address()));
  matrix_math::RotateX(base, out, out, angle, turn);
  Stfs(base, angle, Lfs(base, a.y_address()));
  matrix_math::RotateY(base, out, out, angle, turn);
  Stfs(base, angle, Lfs(base, a.z_address()));
  matrix_math::RotateZ(base, out, out, angle, turn);
  Stfs(base, m.translation_x_address(), Lfs(base, p.x_address()));
  Stfs(base, m.translation_y_address(), Lfs(base, p.y_address()));
  Stfs(base, m.translation_z_address(), Lfs(base, p.z_address()));
  return out;
}

// --- The load's end ---

bool Finishable(uint8_t* base, uint32_t accessory) {
  const Accessory a(base, accessory);
  return FileResetNative(base, a.model_file_address()) &&
         FileResetNative(base, a.xpr_file_address());
}

uint32_t AccessoryFinish(uint8_t* base, uint32_t accessory, uint32_t release, uint32_t sp,
                         const FinishCalls& calls, bool* ran_load) {
  ITC_NATIVE_RUNNING("sub_820EF368 Accessory::Finish");
  if (ran_load != nullptr) {
    *ran_load = false;
  }
  const Accessory a(base, accessory);
  if (a.loaded() != 0 || a.model_file_state() != 2) {
    return a.loaded();
  }
  const uint32_t frame = sp - 112;
  Store(base, frame, sp);  // the back chain, for the guest calls
  const uint32_t file = a.model_file_data(), xpr = a.xpr_file_data();
  if (!models::ModelLoad(base, accessory, file, xpr, calls.model_load)) {
    calls.load(accessory, file, xpr, frame);
  }
  if (ran_load != nullptr) {
    *ran_load = true;
  }
  if (release != 0) {
    FileReset(base, a.model_file_address());
    FileReset(base, a.xpr_file_address());
  }
  const uint32_t skeleton = a.skeleton_address();
  const auto set_up = SkeletonSetUp(base, skeleton, frame);
  const uint32_t r3 = set_up ? *set_up : calls.set_up(skeleton, frame);
  if (r3 != 0) {
    const AccessorySkeleton s(base, skeleton);
    const uint32_t rows = s.rows_0().address;
    Copy16(base, a.anchor_xy_address(), rows + ((s.swing_node() << 4) & 0xFFFFFFF0u));
    a.set_loaded(1);
  }
  return a.loaded();
}

uint64_t EntryOnLoaded(
    uint8_t* base, uint32_t entry, uint32_t sp, const FinishCalls& calls,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set,
    bool* ran_load) {
  ITC_NATIVE_RUNNING("sub_820F0820 AccessoryEntry::OnLoaded");
  const uint32_t frame = sp - 112;
  const AccessoryEntry e(base, entry);
  const uint32_t data = e.file_data();
  const uint32_t accessory = e.accessory_address();
  const Accessory a(base, accessory);
  FileSetData(base, a.model_file_address(), game_data::GetEntry(base, data, 0, false), 0);
  FileSetData(base, a.xpr_file_address(), game_data::GetEntry(base, data, 1, false), 0);
  FileSetData(base, AccessorySkeleton(base, a.skeleton_address()).file_address(),
              game_data::GetEntry(base, data, 2, false), 0);
  Require(Finishable(base, accessory), "Accessory::Finish");
  AccessoryFinish(base, accessory, 1, frame, calls, ran_load);
  return AccessorySetTexture(base, accessory, a.texture_index(), -1, frame, texture_set);
}

// --- The acquisition ---

uint32_t SetAcquireEntry(
    uint8_t* base, uint32_t set, int32_t index, uint32_t texture, uint32_t sp,
    const archive::LoadCall& load,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set) {
  ITC_NATIVE_RUNNING("sub_820F09E0 Accessories::AcquireEntry");
  if (index == 3) {
    return 0;
  }
  const uint32_t frame = sp - 128;
  const uint32_t slot = set + ((uint32_t(index + 1) << 2) & 0xFFFFFFFCu);
  const uint32_t entry = Load(base, slot);
  if (entry == 0) {
    const uint32_t made = EntryNew(base, frame);
    Store(base, slot, made);
    if (made == 0) {
      return 0;
    }
    AccessorySetTexture(base, AccessoryEntry(base, made).accessory_address(), texture, -1, frame,
                        texture_set);
    const uint32_t stored = Load(base, slot);
    FileLoad(base, AccessoryEntry(base, stored).file_address(), uint32_t(index) + 23008, 0,
             kEntryOnLoaded, stored, load);
  } else {
    const AccessoryEntry e(base, entry);
    e.set_count(e.count() + 1);
    AccessorySetTexture(base, AccessoryEntry(base, Load(base, slot)).accessory_address(), texture,
                        -1, frame, texture_set);
  }
  return AccessoryEntry(base, Load(base, slot)).accessory_address();
}

uint32_t SetAcquire(
    uint8_t* base, uint32_t set, int32_t index, uint32_t texture, uint32_t sp,
    const archive::LoadCall& load,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set) {
  ITC_NATIVE_RUNNING("sub_820F0EB8 Accessories::Acquire");
  const uint32_t accessory = SetAcquireEntry(base, set, index, texture, sp - 96, load, texture_set);
  if (accessory != 0) {
    Copy16(base, Accessory(base, accessory).scale_xy_address(),
           Accessories(base, set).scale_xy_address());
  }
  return accessory;
}

}  // namespace accessories
