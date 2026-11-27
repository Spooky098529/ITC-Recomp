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
#include "schema/material_passes.h"

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
using schema::MaterialRecord;

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}

// lfs: a single widened to a double, as the translation does.
double Lfs(uint32_t address) {
  const uint32_t bits = Load(address);
  float value;
  std::memcpy(&value, &bits, 4);
  return double(value);
}
// stfs: a double rounded to single.
void Stfs(uint32_t address, double value) {
  const float single = float(value);
  uint32_t bits;
  std::memcpy(&bits, &single, 4);
  Store(address, bits);
}

// VirtualAlloc(0, size, 0x801000, 4) through the allocation layer (the
// adapters check it routes there).
uint32_t VirtualAlloc(uint32_t size) {
  return memory::AllocPages(size, 4096);
}

// ld and std of two doublewords: 16 bytes copied, the first eight read
// before they're stored, then the second.
void Copy16(uint32_t to, uint32_t from) {
  for (uint32_t half = 0; half < 16; half += 8) {
    const uint32_t high = Load(from + half), low = Load(from + half + 4);
    Store(to + half, high);
    Store(to + half + 4, low);
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
void StoreIdentity(uint32_t rows, double one) {
  const uint32_t at = rows & ~0xFu;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      if (row == column) {
        Stfs(at + 16 * row + 4 * column, one);
      } else {
        Store(at + 16 * row + 4 * column, 0);
      }
    }
  }
}

}  // namespace

// --- The file ---

void FileConstruct(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0C50 AccessoryFile::AccessoryFile");
  const AccessoryFile f(file);
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

void FileSetData(uint32_t file, uint32_t data, uint32_t owned) {
  ITC_NATIVE_RUNNING("sub_820F0BF0 AccessoryFile::SetData");
  const AccessoryFile f(file);
  f.set_data(data);
  f.set_owned(owned);
  f.set_size(0);
  f.set_asset_id(0xFFFFFFFFu);
  f.set_serial(0xFFFFFFFFu);
  f.set_state(2);
  f.set_callback(0);
  f.set_callback_context(0);
}

int64_t FileLoad(uint32_t file, uint32_t id, uint32_t buffer, uint32_t callback,
                 uint32_t context, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820F0C90 AccessoryFile::Load");
  const AccessoryFile f(file);
  f.set_asset_id(id);
  const uint32_t size = archive::AssetSize(id);
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

Callback FileOnArchiveDone(uint32_t serial, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0C20 AccessoryFile::OnArchiveDone");
  const AccessoryFile f(file);
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

std::optional<uint32_t> FileFreeData(uint32_t slot) {
  const uint32_t block = Load(slot);
  if (block != 0 && !memory::IsPages(block)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820F08D0 AccessoryFile::FreeData");
  if (block == 0) {
    return 0;
  }
  const uint32_t r3 = memory::FreePages(block) ? 1 : 0;
  Store(slot, 0);
  return r3;
}

bool FileResetNative(uint32_t file) {
  const AccessoryFile f(file);
  if (f.owned() == 0 || f.state() != 2) {
    return true;
  }
  const uint32_t block = f.data();
  return block == 0 || memory::IsPages(block);
}

std::optional<uint32_t> FileReset(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0D30 AccessoryFile::Reset");
  const AccessoryFile f(file);
  std::optional<uint32_t> r3;
  if (f.owned() != 0) {
    const uint32_t state = f.state();
    if (state == 1) {
      r3 = archive::Cancel(f.serial());
    } else if (state == 2) {
      r3 = FileFreeData(f.data_address());
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

void SetBaseConstruct(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F07B8 AccessoriesBase::AccessoriesBase");
  const Accessories s(set);
  s.set_vtable(kSetBaseVtable);
  s.set_entry_0(0u);
  s.set_entry_1(0u);
  s.set_entry_2(0u);
}

uint32_t SetConstruct(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F0E60 Accessories::Accessories");
  SetBaseConstruct(set);
  const Accessories s(set);
  const float one = AccessoriesGlobals::one();
  s.set_vtable(kSetVtable);
  s.set_scale_w(one);
  s.set_scale_x(one);
  s.set_scale_y(one);
  s.set_scale_z(one);
  s.set_scale_w(one);
  return set;
}

uint32_t SetNew() {
  ITC_NATIVE_RUNNING("sub_820F0F48 Accessories::New");
  // In host memory (E5 step 5: a handle, freed by Accessories::Free's
  // VirtualFree through FileFreeData), where the original VirtualAllocs
  // kSetBytes.
  static_assert(Accessories::kSize == kSetBytes);
  const uint32_t set = Accessories::New().address;
  if (set == 0) {
    return 0;
  }
  return SetConstruct(set);
}

std::optional<uint32_t> SetCreate() {
  ITC_NATIVE_RUNNING("sub_820F1038 Accessories::Create");
  if (AccessoriesGlobals::set()) {
    return std::nullopt;
  }
  const uint32_t set = SetNew();
  AccessoriesGlobals::set_set(set);
  return set;
}

void SetScale(uint32_t set, uint32_t scale) {
  ITC_NATIVE_RUNNING("sub_820F0F00 Accessories::SetScale");
  const Accessories s(set);
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t entry = Load(s.entry_0_address() + 4 * i);
    if (entry != 0) {
      Copy16(AccessoryEntry(entry).accessory_scale_xy_address(), scale);
    }
  }
  Copy16(s.scale_xy_address(), scale);
}

uint32_t SetFree(uint32_t slot, uint32_t sp,
                 const std::function<void(uint32_t set, uint32_t frame)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820F0F90 Accessories::Free");
  const uint32_t set = Load(slot);
  if (set == 0) {
    return 0;
  }
  // No guest frame (E5 step 6): the original's 96 bytes held only the back
  // chain; the destructor is called from `sp`.
  destroy(set, sp);
  const auto r3 = FileFreeData(slot);
  Require(r3.has_value(), "AccessoryFile::FreeData");
  return *r3;
}

uint64_t SetDestroy(uint32_t sp,
                    const std::function<void(uint32_t set, uint32_t frame)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820F1078 Accessories::Destroy");
  if (!AccessoriesGlobals::set()) {
    // lis and addi: sign-extended
    return uint64_t(int64_t(int32_t(AccessoriesGlobals::set_address())));
  }
  return SetFree(AccessoriesGlobals::set_address(), sp, destroy);
}

// --- The entry and the accessory ---

bool AccessoryConstructible() {
  return models::CanAllocatePointers(models::kTextureProviderSlots);
}

uint32_t SkeletonConstruct(uint32_t skeleton, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820F03F0 AccessorySkeleton::AccessorySkeleton");
  // The identity at the original frame's +96 stays on the stack (memcpy reads
  // it by address).
  const uint32_t frame = sp - 224;
  const AccessorySkeleton s(skeleton);
  model_animation::SkeletonBaseConstruct(skeleton);
  s.set_vtable_2(kListNodeVtable);
  s.set_vtable(kSkeletonVtable);
  s.set_vtable_2(kSkeletonVtable2);
  FileConstruct(s.file_address());
  const float one = AccessoriesGlobals::one();
  const float zero = AccessoriesGlobals::zero();
  // The job (+216): its functor's vtable (+220) set twice, its flags.
  s.set_job_functor_vtable(kJobFunctorFirst);
  s.set_unknown_233(1);
  s.set_job_vtable(kJobVtable);
  s.set_job_functor_vtable(kJobFunctor);
  // The method's two words, zeroed at the frame's +80 and +84 and read back:
  // C++ locals since E5 step 6 (only read here).
  const uint32_t method[2] = {0, 0};
  s.set_unknown_224(0);
  s.set_unknown_228(0);
  s.set_unknown_232(0);
  s.set_unknown_234(0);
  s.set_job_method(method[0]);
  s.set_job_method_extra(method[1]);
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
  StoreIdentity(frame + 96, 1.0);
  Require(native::Buffers_Memcpy(s.matrix_address(), frame + 96, 64, 0x820F0544), "memcpy");
  StoreIdentity(frame + 96, 1.0);
  Require(native::Buffers_Memcpy(s.last_matrix_address(), frame + 96, 64, 0x820F0588),
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

uint32_t AccessoryConstruct(uint32_t accessory, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820EF258 Accessory::Accessory");
  // No guest frame (E5 step 6): the original's 96 bytes held nothing this
  // port writes; the skeleton's constructor is given `sp`.
  const Accessory a(accessory);
  Require(models::PartModelConstruct(accessory), "PartModel::PartModel");
  a.set_vtable(kAccessoryVtable);
  FileConstruct(a.model_file_address());
  FileConstruct(a.xpr_file_address());
  SkeletonConstruct(a.skeleton_address(), sp);
  const float one = AccessoriesGlobals::one();
  a.set_anchor_w(one);
  a.set_angle_w(one);
  a.set_scale_w(one);
  a.set_anchor_w(one);
  const float zero = AccessoriesGlobals::zero();
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

uint32_t EntryNew(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820F0920 AccessoryEntry::New");
  // In host memory (E5 step 5: a handle, freed by AccessoryEntry::Destroy's
  // VirtualFree through FileFreeData), where the original VirtualAllocs
  // kEntryBytes.
  static_assert(AccessoryEntry::kSize == kEntryBytes);
  const uint32_t entry = AccessoryEntry::New().address;
  if (entry == 0) {
    return 0;
  }
  const AccessoryEntry e(entry);
  e.set_count(1);
  FileConstruct(e.file_address());
  AccessoryConstruct(e.accessory_address(), sp);  // no guest frame (E5 step 6)
  return entry;
}

// --- The destructors ---

constexpr uint32_t kJobDelegateBase = 0x82000ABC;  // the functors' base
constexpr uint32_t kJobFunctorBase = 0x82000BA8;
constexpr uint32_t kJobBaseVtable = 0x82000EB8;

bool FileDestructible(uint32_t file) {
  return FileResetNative(file);
}

bool SkeletonDestructible(uint32_t skeleton) {
  return model_animation::SkeletonBaseDestructible(skeleton) &&
         FileDestructible(AccessorySkeleton(skeleton).file_address());
}

bool AccessoryDestructible(uint32_t accessory) {
  const Accessory a(accessory);
  return SkeletonDestructible(a.skeleton_address()) &&
         FileDestructible(a.xpr_file_address()) &&
         FileDestructible(a.model_file_address()) &&
         models::ModelMembersDestructible(accessory) &&
         models::PendingFreesHaveRoom(models::ModelReleaseNodes(accessory));
}

bool EntryDestroyable(uint32_t slot) {
  const uint32_t entry = Load(slot);
  const AccessoryEntry e(entry);
  return entry == 0 || (memory::IsPages(entry) && AccessoryDestructible(e.accessory_address()) &&
                        FileDestructible(e.file_address()));
}

uint32_t FileDestruct(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820F0DB8 AccessoryFile::~AccessoryFile");
  const AccessoryFile f(file);
  f.set_vtable(kFileVtable);
  const uint32_t r3 = FileReset(file).value_or(file);
  f.set_vtable(kListNodeVtable);
  return r3;
}

uint32_t SkeletonDestruct(uint32_t skeleton) {
  ITC_NATIVE_RUNNING("sub_820EFFD8 AccessorySkeleton::~AccessorySkeleton");
  const AccessorySkeleton s(skeleton);
  s.set_vtable(kSkeletonVtable);
  s.set_vtable_2(kSkeletonVtable2);
  s.set_job_delegate_vtable(kJobDelegateBase);
  s.set_job_functor_vtable(kJobFunctorBase);
  s.set_job_functor_vtable(kJobFunctorFirst);
  s.set_job_vtable(kJobBaseVtable);
  FileDestruct(s.file_address());
  s.set_vtable_2(kListNodeVtable);
  Require(model_animation::SkeletonBaseDestruct(skeleton), "SkeletonBase::~SkeletonBase");
  return skeleton;
}

uint32_t AccessoryDestruct(uint32_t accessory) {
  ITC_NATIVE_RUNNING("sub_820EF310 Accessory::~Accessory");
  const Accessory a(accessory);
  a.set_vtable(kAccessoryVtable);
  SkeletonDestruct(a.skeleton_address());
  FileDestruct(a.xpr_file_address());
  FileDestruct(a.model_file_address());
  const auto r3 = models::PartModelDestruct(accessory);
  Require(r3.has_value(), "PartModel::~PartModel");
  return *r3;
}

uint32_t EntryDestroy(uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820F0988 AccessoryEntry::Destroy");
  const uint32_t entry = Load(slot);
  if (entry == 0) {
    return slot;
  }
  const AccessoryEntry e(entry);
  AccessoryDestruct(e.accessory_address());
  FileDestruct(e.file_address());
  const auto r3 = FileFreeData(slot);
  Require(r3.has_value(), "AccessoryFile::FreeData");
  return *r3;
}

bool SetReleasable(uint32_t set) {
  const Accessories s(set);
  uint32_t entries[3];
  for (uint32_t i = 0; i < 3; i++) {
    entries[i] = Load(s.entry_0_address() + 4 * i);
    for (uint32_t k = 0; k < i; k++) {
      if (entries[i] != 0 && entries[i] == entries[k]) {
        return false;
      }
    }
    if (entries[i] != 0 && AccessoryEntry(entries[i]).count() - 1 == 0 &&
        !EntryDestroyable(s.entry_0_address() + 4 * i)) {
      return false;
    }
  }
  return true;
}

std::optional<uint32_t> SetReleaseAll(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F0AD8 Accessories::ReleaseAll");
  const Accessories s(set);
  std::optional<uint32_t> r3;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = s.entry_0_address() + 4 * i;
    const uint32_t entry = Load(slot);
    if (entry == 0) {
      continue;
    }
    const AccessoryEntry e(entry);
    e.set_count(e.count() - 1);
    if (AccessoryEntry(Load(slot)).count() == 0) {
      r3 = EntryDestroy(slot);
    }
  }
  return r3;
}

namespace {

uint32_t ReleaseSlot(uint32_t set, int32_t index) {
  return set + ((uint32_t(index + 1) << 2) & 0xFFFFFFFCu);
}

}  // namespace

bool SetReleaseNative(uint32_t set, int32_t index) {
  if (index == 3) {
    return true;
  }
  const uint32_t slot = ReleaseSlot(set, index);
  const uint32_t entry = Load(slot);
  return entry == 0 || AccessoryEntry(entry).count() - 1 != 0 ||
         EntryDestroyable(slot);
}

std::optional<uint32_t> SetRelease(uint32_t set, int32_t index) {
  ITC_NATIVE_RUNNING("sub_820F0A90 Accessories::Release");
  if (index == 3) {
    return std::nullopt;
  }
  const uint32_t slot = ReleaseSlot(set, index);
  const uint32_t entry = Load(slot);
  if (entry == 0) {
    return slot;
  }
  const AccessoryEntry e(entry);
  e.set_count(e.count() - 1);
  if (AccessoryEntry(Load(slot)).count() != 0) {
    return slot;
  }
  return EntryDestroy(slot);
}

// --- The skeleton's set-up ---

uint32_t SkeletonReset(uint32_t skeleton, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820EFB30 AccessorySkeleton::Reset");
  const uint32_t frame = sp - 192;
  const AccessorySkeleton s(skeleton);
  for (uint32_t i = 0; int32_t(i) < int32_t(s.node_file().node_count()); i++) {
    const AccessoryNodeLocal entry(s.locals().address + 144 * i);
    // Three rows (ld and std of two doublewords each, each row read first).
    Copy16(entry.row0_xy_address(), s.rows_0().address + 16 * i);
    Copy16(entry.row1_xy_address(), s.rows_1().address + 16 * i);
    Copy16(entry.row2_xy_address(), s.rows_2().address + 16 * i);
    const AccessoryNodeRecord node(s.nodes().address + 160 * i);
    if (node.flags() & 0x40) {
      Copy16(entry.row3_xy_address(), node.rest_xy_address());
    }
  }
  const float zero = AccessoriesGlobals::zero();
  s.set_angle(zero);
  s.set_angle_speed(zero);
  StoreIdentity(frame + 80, 1.0);
  Require(native::Buffers_Memcpy(s.matrix_address(), frame + 80, 64, 0x820EFC60), "memcpy");
  StoreIdentity(frame + 80, 1.0);
  Require(native::Buffers_Memcpy(s.last_matrix_address(), frame + 80, 64, 0x820EFCA4),
          "memcpy");
  s.set_movement_z(zero);
  s.set_movement_y(zero);
  s.set_movement_x(zero);
  const float one = AccessoriesGlobals::one();
  s.set_movement_w(one);
  s.set_swing_z(zero);
  s.set_swing_y(zero);
  s.set_swing_x(zero);
  s.set_swing_w(one);
  s.set_stepped(0);
  return s.last_matrix_address();
}

std::optional<uint32_t> SkeletonSetUp(uint32_t skeleton, uint32_t sp) {
  const AccessorySkeleton s(skeleton);
  if (s.set_up() != 0 || s.file_state() != 2) {
    ITC_NATIVE_RUNNING("sub_820F0070 AccessorySkeleton::SetUp");
    return s.set_up();
  }
  if (!model_animation::SkeletonSetUp(skeleton, s.file_data(), 0, 1)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820F0070 AccessorySkeleton::SetUp");
  if (int32_t(s.node_file().node_count()) > 0) {
    for (uint32_t i = 0; int32_t(i) < int32_t(s.node_file().node_count()); i++) {
      const AccessoryNodeRecord node(s.nodes().address + 160 * i);
      if (c_runtime::Strstr(node.name_address(), kSwingNodeName) >= 0) {
        s.set_swing_node(i);
        break;
      }
    }
    s.set_set_up(1);
    SkeletonReset(skeleton, sp);  // no guest frame (E5 step 6)
  }
  return s.set_up();
}

uint32_t SetBaseDestruct(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820F0B58 AccessoriesBase::~AccessoriesBase");
  const Accessories s(set);
  s.set_vtable(kSetBaseVtable);
  const uint32_t r3 = SetReleaseAll(set).value_or(set);
  s.set_vtable(kListNodeVtable);
  return r3;
}

uint32_t SetDestruct(uint32_t set, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_820F0FE0 Accessories::~Accessories");
  Accessories(set).set_vtable(kSetVtable);
  SetBaseDestruct(set);
  if (flags & 1) {
    Require(memory::crt::OperatorDelete(set), "operator delete");
  }
  return set;
}

// --- The draw's helpers ---

uint64_t AccessorySetTexture(
    uint32_t accessory, uint32_t index, int32_t shape, uint32_t sp,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set) {
  ITC_NATIVE_RUNNING("sub_820EF568 Accessory::SetTexture");
  const Accessory a(accessory);
  a.set_texture_index(index);
  uint64_t r3 = accessory;
  if (a.loaded() == 0) {
    return r3;
  }
  // No guest frame (E5 step 6): the original's 160 bytes held only the back
  // chain; the slot is called from `sp`.
  r3 = texture_set(a.textures_vtable_address(), sp);  // the provider at +80
  if (!(Load(uint32_t(r3) + 20) > index)) {
    return r3;
  }
  uint32_t first = uint32_t(shape), count = 1;
  if (shape < 0) {
    count = a.shape_count();
    first = 0;
  }
  for (uint32_t s = first; s < count; s++) {
    for (uint32_t g = 0; g < 2; g++) {
      const int32_t group = int32_t(g == 0 ? AccessoriesGlobals::material_group_0()
                                            : AccessoriesGlobals::material_group_1());
      const uint32_t materials = models::MaterialCount(accessory, s, group);
      r3 = materials;
      for (int32_t m = 0; m < int32_t(materials); m++) {
        const uint32_t material = models::Material(accessory, s, uint32_t(m), group);
        r3 = material;
        const MaterialRecord record(material);
        for (uint32_t k = 0; k < 8; k++) {
          Store(record.texture_index_address() + 4 * k, index);
          Store(record.xpr_index_address() + 4 * k, 0);
        }
      }
    }
  }
  return r3;
}

uint32_t FromScaleRotationTranslation(uint32_t out, uint32_t angles,
                                      uint32_t position, uint32_t scale, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820EF660 Matrix::FromScaleRotationTranslation");
  const uint32_t frame = sp - 208;
  StoreIdentity(frame + 96, 1.0);
  Require(native::Buffers_Memcpy(out, frame + 96, 64, 0x820EF6C0), "memcpy");
  const AccessoryMatrix m(out);
  const AccessoryFloat3 s(scale), a(angles), p(position);
  Stfs(m.row0_x_address(), Lfs(s.x_address()));
  Stfs(m.row1_y_address(), Lfs(s.y_address()));
  Stfs(m.row2_z_address(), Lfs(s.z_address()));
  const uint32_t angle = frame + 80, turn = frame - 80;
  Stfs(angle, Lfs(a.x_address()));
  matrix_math::RotateX(out, out, angle, turn);
  Stfs(angle, Lfs(a.y_address()));
  matrix_math::RotateY(out, out, angle, turn);
  Stfs(angle, Lfs(a.z_address()));
  matrix_math::RotateZ(out, out, angle, turn);
  Stfs(m.translation_x_address(), Lfs(p.x_address()));
  Stfs(m.translation_y_address(), Lfs(p.y_address()));
  Stfs(m.translation_z_address(), Lfs(p.z_address()));
  return out;
}

// --- The load's end ---

bool Finishable(uint32_t accessory) {
  const Accessory a(accessory);
  return FileResetNative(a.model_file_address()) &&
         FileResetNative(a.xpr_file_address());
}

uint32_t AccessoryFinish(uint32_t accessory, uint32_t release, uint32_t sp,
                         const FinishCalls& calls, bool* ran_load) {
  ITC_NATIVE_RUNNING("sub_820EF368 Accessory::Finish");
  if (ran_load != nullptr) {
    *ran_load = false;
  }
  const Accessory a(accessory);
  if (a.loaded() != 0 || a.model_file_state() != 2) {
    return a.loaded();
  }
  // No guest frame (E5 step 6): the original's 112 bytes held only the back
  // chain; the guest calls are made from `sp`.
  const uint32_t file = a.model_file_data(), xpr = a.xpr_file_data();
  memory::PhysicalTag tag("model vertices: accessories");
  if (!models::ModelLoad(accessory, file, xpr, calls.model_load)) {
    calls.load(accessory, file, xpr, sp);
  }
  if (ran_load != nullptr) {
    *ran_load = true;
  }
  if (release != 0) {
    FileReset(a.model_file_address());
    FileReset(a.xpr_file_address());
  }
  const uint32_t skeleton = a.skeleton_address();
  const auto set_up = SkeletonSetUp(skeleton, sp);
  const uint32_t r3 = set_up ? *set_up : calls.set_up(skeleton, sp);
  if (r3 != 0) {
    const AccessorySkeleton s(skeleton);
    const uint32_t rows = s.rows_0().address;
    Copy16(a.anchor_xy_address(), rows + ((s.swing_node() << 4) & 0xFFFFFFF0u));
    a.set_loaded(1);
  }
  return a.loaded();
}

uint64_t EntryOnLoaded(
    uint32_t entry, uint32_t sp, const FinishCalls& calls,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set,
    bool* ran_load) {
  ITC_NATIVE_RUNNING("sub_820F0820 AccessoryEntry::OnLoaded");
  // No guest frame (E5 step 6): the original's 112 bytes held nothing this
  // port writes; its callees are given `sp`.
  const AccessoryEntry e(entry);
  const uint32_t data = e.file_data();
  const uint32_t accessory = e.accessory_address();
  const Accessory a(accessory);
  FileSetData(a.model_file_address(), game_data::GetEntry(data, 0, false), 0);
  FileSetData(a.xpr_file_address(), game_data::GetEntry(data, 1, false), 0);
  FileSetData(AccessorySkeleton(a.skeleton_address()).file_address(),
              game_data::GetEntry(data, 2, false), 0);
  Require(Finishable(accessory), "Accessory::Finish");
  AccessoryFinish(accessory, 1, sp, calls, ran_load);
  return AccessorySetTexture(accessory, a.texture_index(), -1, sp, texture_set);
}

// --- The acquisition ---

uint32_t SetAcquireEntry(
    uint32_t set, int32_t index, uint32_t texture, uint32_t sp,
    const archive::LoadCall& load,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set) {
  ITC_NATIVE_RUNNING("sub_820F09E0 Accessories::AcquireEntry");
  if (index == 3) {
    return 0;
  }
  // No guest frame (E5 step 6): the original's 128 bytes held nothing this
  // port writes; its callees are given `sp`.
  const uint32_t slot = set + ((uint32_t(index + 1) << 2) & 0xFFFFFFFCu);
  const uint32_t entry = Load(slot);
  if (entry == 0) {
    const uint32_t made = EntryNew(sp);
    Store(slot, made);
    if (made == 0) {
      return 0;
    }
    AccessorySetTexture(AccessoryEntry(made).accessory_address(), texture, -1, sp,
                        texture_set);
    const uint32_t stored = Load(slot);
    FileLoad(AccessoryEntry(stored).file_address(), uint32_t(index) + 23008, 0,
             kEntryOnLoaded, stored, load);
  } else {
    const AccessoryEntry e(entry);
    e.set_count(e.count() + 1);
    AccessorySetTexture(AccessoryEntry(Load(slot)).accessory_address(), texture,
                        -1, sp, texture_set);
  }
  return AccessoryEntry(Load(slot)).accessory_address();
}

uint32_t SetAcquire(
    uint32_t set, int32_t index, uint32_t texture, uint32_t sp,
    const archive::LoadCall& load,
    const std::function<uint64_t(uint32_t provider, uint32_t frame)>& texture_set) {
  ITC_NATIVE_RUNNING("sub_820F0EB8 Accessories::Acquire");
  const uint32_t accessory = SetAcquireEntry(set, index, texture, sp, load, texture_set);
  if (accessory != 0) {
    Copy16(Accessory(accessory).scale_xy_address(),
           Accessories(set).scale_xy_address());
  }
  return accessory;
}

}  // namespace accessories
