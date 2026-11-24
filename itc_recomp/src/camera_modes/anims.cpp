// The camera animations' and camera paths' files and the rival logo's
// pieces (docs/functions/camera-modes.md): the loads, the parse, the frees,
// the evaluation and the paths' fix-ups. Store for store as the recompiled
// functions.

#include "camera_modes/camera_modes.h"

#include "camera_modes/steps.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "memory/handles.h"
#include "memory/heap.h"
#include "models/xpr.h"

#include <cstring>
#include "ui_movies/game.h"
#include "schema/camera_modes.h"
#include "schema/game_modes.h"

namespace camera_modes {

using namespace steps;

namespace {

using G = schema::CameraModesGlobals;

constexpr uint32_t kOne = G::one_address();    // 1.0
constexpr uint32_t kZero = G::zero_address();  // 0.0
constexpr uint32_t kHalf = G::half_address();  // 0.5
constexpr uint32_t kTwo = G::two_address();    // 2.0
constexpr uint32_t kRadiansPerDegree = G::radians_per_degree_address();
constexpr uint32_t kFilmScale = G::film_scale_address();  // EvalCamera's length scale
constexpr uint32_t kZoomScale = G::zoom_scale_address();  // EvalCamera's zoom numerator
constexpr uint32_t kFirstTurn = G::first_turn_address();  // CameraAnim::Matrix's first RotateY

constexpr uint32_t kEndKind = 14;

// VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE)'s buffer as a blob (E5
// step 5: PC memory in the guest's byte order, memory::NewBlob) of the
// schema type `T`; 0 for 0 bytes, as the layer's pages gave. The frees
// (memory::FreePages, the game's VirtualFree) take its handle.
template <typename T>
uint32_t Blob(uint8_t* base, uint32_t size) {
  return size == 0 ? 0 : T::New(base, size).address;
}

// memcpy, or the guest's where the native layer refuses.
void Copy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size,
          const GuestCopy& guest_copy) {
  if (!c_runtime::Memcpy(base, destination, source, size)) {
    guest_copy(destination, source, size);
  }
}

// A block's value at frame `time`: the float [+60][time] (the index
// shifted as rlwinm's, 32 bits).
double Value(uint8_t* base, uint32_t block, uint32_t time) {
  const schema::CameraAnimBlock b(base, block);
  return Lfs(base, b.values() + (time << 2));
}

// A camera animation is a host object (E5 step 4), laid out at the guest
// offsets: a word at `offset`, and its block list (+80, up to 64 handles).
void SetAnimWord(const schema::CameraAnim& anim, uint32_t offset, uint32_t value) {
  std::memcpy(reinterpret_cast<uint8_t*>(anim.data()) + offset, &value, sizeof value);
}
uint32_t AnimBlock(const schema::CameraAnim& anim, uint32_t index) {
  uint32_t value;
  std::memcpy(&value, anim.blocks() + 4 * index, sizeof value);
  return value;
}
void SetAnimBlock(const schema::CameraAnim& anim, uint32_t index, uint32_t block) {
  std::memcpy(anim.blocks() + 4 * index, &block, sizeof block);
}

// A (kind, value) pair: kind 2 made kind 1 with value x 24 + `curves`.
void FixPair(uint8_t* base, uint32_t pair, uint32_t curves) {
  if (int32_t(guest::LoadU32(base, pair)) != 2) {
    return;
  }
  const uint32_t value = guest::LoadU32(base, pair + 4);
  guest::StoreU32(base, pair, 1);
  guest::StoreU32(base, pair + 4, value * 24 + curves);
}

bool ObjectFreeable(uint8_t* base, uint32_t object) {
  const schema::CameraAnim anim(base, object);
  if (!memory::IsHandle(object)) {  // a host object (E5 step 4)
    return false;
  }
  const uint32_t count = anim.blocks_used();
  if (count > 64) {
    return false;
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t block = AnimBlock(anim, i);
    if (!memory::IsHandle(block)) {  // host objects (E5 step 3)
      return false;
    }
    const uint32_t data = schema::CameraAnimBlock(base, block).values();
    if (data != 0 && !memory::IsPages(data)) {
      return false;
    }
  }
  return true;
}

}  // namespace

// --- The pieces with no adapters (only their callers' plain C++ calls them) ---

void FixTriple(uint8_t* base, uint32_t triple, uint32_t curves) {
  ITC_NATIVE_RUNNING("sub_820ABE68 CameraPath::FixTriple");
  FixPair(base, triple + 0, curves);
  FixPair(base, triple + 8, curves);
  FixPair(base, triple + 16, curves);
}

void FixGroup(uint8_t* base, uint32_t group, uint32_t curves) {
  ITC_NATIVE_RUNNING("sub_820ABED8 CameraPath::FixGroup");
  FixTriple(base, group, curves);
  FixTriple(base, group + 24, curves);
  FixPair(base, group + 48, curves);
  FixTriple(base, group + 56, curves);
  FixPair(base, group + 80, curves);
  FixPair(base, group + 88, curves);
}

void FixKey(uint8_t* base, uint32_t key, uint32_t curves) {
  ITC_NATIVE_RUNNING("sub_820ABF80 CameraPath::FixKey");
  const schema::CameraPathKey k(base, key);
  FixPair(base, k.fov_kind_address(), curves);
  FixPair(base, k.zoom_kind_address(), curves);
  FixGroup(base, k.eye_position_x_kind_address(), curves);
  FixGroup(base, k.target_position_x_kind_address(), curves);
}

void FixKeys(uint8_t* base, uint32_t path, uint32_t keys, uint32_t curves) {
  ITC_NATIVE_RUNNING("sub_820AC000 CameraPath::FixKeys");
  const schema::CameraPathRecord p(base, path);
  const uint32_t first = p.keys() * 216 + keys;
  p.set_keys(first);
  for (uint32_t i = 0, at = 0; i < p.key_count(); i++, at += 216) {
    FixKey(base, p.keys() + at, curves);
  }
}

void BlockFree(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_8214FFF0 CameraAnimBlock::Free");
  const schema::CameraAnimBlock b(base, block);
  const uint32_t data = b.values();
  if (data != 0) {
    memory::FreePages(data);
  }
  b.set_values(0);
}

void AnimFree(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82150098 CameraAnim::Free");
  const schema::CameraAnim anim(base, object);
  while (anim.blocks_used() != 0) {
    const uint32_t count = anim.blocks_used() - 1;
    anim.set_blocks_used(count);
    const uint32_t block = AnimBlock(anim, count);
    BlockFree(base, block);
    // A host object (E5 step 3): deleted in place of the VirtualFree at
    // 0x821500E8.
    schema::CameraAnimBlock(base, block).Delete();
  }
}

// --- The camera animations' files ---

int64_t AnimsLoad(uint8_t* base, uint32_t id, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82150B10 CameraAnims::Load");
  const uint32_t size = archive::AssetSize(base, id);
  // The record lives in host memory (E5 step 3): the original's
  // VirtualAlloc(4) at 0x82150B3C; the archive keeps its handle as the
  // callback's context.
  const schema::CameraAnimLoad record = schema::CameraAnimLoad::New(base);
  // The file's buffer, a blob (E5 step 5) in place of the VirtualAlloc
  // returning at 0x82150B54; CameraAnims::OnLoaded frees it.
  const uint32_t buffer = Blob<schema::CameraAnimsFile>(base, size);
  record.set_id(id);
  return load(id, buffer, kAnimsOnLoaded, record.address);
}

uint32_t AnimParseObject(uint8_t* base, uint32_t object, const GuestCopy& xmemcpy) {
  ITC_NATIVE_RUNNING("sub_82150100 CameraAnim::ParseObject");
  const schema::CameraAnimFileObject file(base, object);
  // A host object (E5 step 4) in place of the VirtualAlloc(340) at
  // 0x82150128; CameraAnims::OnLoaded lists its handle.
  const schema::CameraAnim anim = schema::CameraAnim::New(base);
  const uint32_t copy = anim.address;
  const double zero = Lfs(base, kZero);
  schema::CameraAnim(base, copy).set_kind(0);
  schema::CameraAnim(base, copy).set_header_size(0);
  schema::CameraAnim(base, copy).set_unknown_8(0);
  schema::CameraAnim(base, copy).set_block_count(0);
  for (const uint32_t at : {20u, 20u, 16u, 32u, 32u, 28u, 40u, 44u}) {
    SetAnimWord(anim, at, Bits(zero));
  }
  for (uint32_t i = 0; i < 8; i++) {
    SetAnimWord(anim, 48 + 4 * i, 0);
  }
  schema::CameraAnim(base, copy).set_blocks_used(0);
  for (uint32_t at = 0; at < 40; at += 4) {
    SetAnimWord(anim, at, guest::LoadU32(base, object + at));
  }
  schema::CameraAnim(base, copy).set_unknown_40(float(Lfs(base, object + 40)));
  schema::CameraAnim(base, copy).set_unknown_44(float(Lfs(base, object + 44)));
  uint32_t from = object + file.header_size();
  if (schema::CameraAnim(base, copy).block_count() == 0) {
    return copy;
  }
  uint32_t n = 0;
  do {
    // A host object (E5 step 3) in place of the VirtualAlloc(64) at
    // 0x82150220; the anim's list holds its handle.
    const uint32_t block = schema::CameraAnimBlock::New(base).address;
    schema::CameraAnimBlock(base, block).set_unknown_12(float(zero));
    schema::CameraAnimBlock(base, block).set_channel(0);
    schema::CameraAnimBlock(base, block).set_duration(float(zero));
    schema::CameraAnimBlock(base, block).set_header_size(0);
    schema::CameraAnimBlock(base, block).set_data_size(0);
    schema::CameraAnimBlock(base, block).set_values(0);
    std::memset(schema::CameraAnimBlock(base, block).unknown_20(), 0, 40);
    const schema::CameraAnimFileBlock source_block(base, from);
    schema::CameraAnimBlock(base, block).set_channel(source_block.channel());
    schema::CameraAnimBlock(base, block).set_header_size(source_block.header_size());
    const uint32_t bytes = source_block.data_size();
    schema::CameraAnimBlock(base, block).set_data_size(bytes);
    schema::CameraAnimBlock(base, block).set_unknown_12(source_block.unknown_12());
    schema::CameraAnimBlock(base, block).set_duration(source_block.duration());
    // A blob (E5 step 5) in place of the VirtualAlloc returning at
    // 0x82150294; CameraAnimBlock::Free frees it.
    schema::CameraAnimBlock(base, block).set_values(
        Blob<schema::CameraAnimValues>(base, bytes));
    c_runtime::XMemSet(base, schema::CameraAnimBlock(base, block).values(), 0,
                       schema::CameraAnimBlock(base, block).data_size());
    {
      const uint32_t to = schema::CameraAnimBlock(base, block).values();
      const uint32_t source = schema::CameraAnimFileBlock(base, from).header_size() + from;
      const uint32_t size = schema::CameraAnimBlock(base, block).data_size();
      if (!c_runtime::XMemCpy(base, to, source, size)) {
        xmemcpy(to, source, size);
      }
    }
    const uint32_t count = schema::CameraAnim(base, copy).blocks_used();
    schema::CameraAnim(base, copy).set_blocks_used(count + 1);
    SetAnimBlock(anim, count, block);
    from = schema::CameraAnimFileBlock(base, from).header_size() + from +
           schema::CameraAnimFileBlock(base, from).data_size();
    n++;
  } while (n < schema::CameraAnim(base, copy).block_count());
  return copy;
}

bool AnimsOnLoaded(uint8_t* base, uint32_t data, uint32_t record, uint32_t frame,
                   const GuestCopy& memcpy, const GuestCopy& xmemcpy) {
  // The record is a host object's handle (E5 step 3, CameraAnims::Load).
  if (!memory::IsHandle(record) || !memory::IsPages(data)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82150878 CameraAnims::OnLoaded");
  guest::StoreU32(base, frame, frame + 672);
  guest::StoreU32(base, frame + 80, 0);
  const uint32_t id = schema::CameraAnimLoad(base, record).id();
  guest::StoreU32(base, frame + 340, 0);
  uint32_t object = data;
  uint32_t count = 0;
  for (;;) {
    const uint32_t parsed = AnimParseObject(base, object, xmemcpy);
    guest::StoreU32(base, frame + 608, count + 1);
    guest::StoreU32(base, frame + 352 + (count << 2), parsed);
    if (int32_t(schema::CameraAnimFileObject(base, object).kind()) == int32_t(kEndKind)) {
      break;
    }
    count = guest::LoadU32(base, frame + 608);
    object = schema::CameraAnimFileObject(base, object).data_size() +
             schema::CameraAnimFileObject(base, object).header_size() + object;
  }
  guest::StoreU32(base, frame + 80, id);
  Copy(base, frame + 84, frame + 352, 260, memcpy);
  const uint32_t n = G::anim_count(base);
  G::set_anim_count(base, n + 1);
  Copy(base, n * schema::CameraAnimEntry::kSize + G::anims_address(), frame + 80,
       schema::CameraAnimEntry::kSize, memcpy);
  schema::CameraAnimLoad(base, record).Delete();  // the original's VirtualFree at 0x82150940
  memory::FreePages(data);
  return true;
}

bool AnimsFreeable(uint8_t* base) {
  const uint32_t entries = G::anim_count(base);
  if (entries > 64) {
    return false;
  }
  for (uint32_t e = 0; e < entries; e++) {
    const uint32_t entry = G::anims_address() + schema::CameraAnimEntry::kSize * e;
    const uint32_t count = schema::CameraAnimEntry(base, entry).count();
    if (count > 64) {
      return false;
    }
    for (uint32_t i = 0; i < count; i++) {
      if (!ObjectFreeable(
              base, guest::LoadU32(
                        base, schema::CameraAnimEntry(base, entry).objects_address() + 4 * i))) {
        return false;
      }
    }
  }
  return true;
}

std::optional<uint32_t> AnimsFreeAll(uint8_t* base, uint32_t memcpy_r3) {
  ITC_NATIVE_RUNNING("sub_821507D8 CameraAnims::FreeAll");
  std::optional<uint32_t> r3;
  // The original copies each entry to its stack (memcpy) and counts the
  // copy's objects down; the entry itself isn't written.
  for (uint32_t e = G::anim_count(base); e != 0; e = G::anim_count(base)) {
    e--;
    G::set_anim_count(base, e);
    const uint32_t entry = e * schema::CameraAnimEntry::kSize + G::anims_address();
    uint32_t count = schema::CameraAnimEntry(base, entry).count();
    r3 = memcpy_r3;
    while (count != 0) {
      count--;
      const uint32_t object = guest::LoadU32(
          base, schema::CameraAnimEntry(base, entry).objects_address() + (count << 2));
      AnimFree(base, object);
      memory::FreePages(object);
      r3 = 1;
    }
  }
  return r3;
}

std::optional<double> AnimEvalCamera(uint8_t* base, uint32_t object, uint32_t out,
                                     uint32_t time) {
  const schema::CameraAnim anim(base, object);
  const schema::CameraAnimOut to(base, out);
  const double zero = Lfs(base, kZero);
  double x = zero, y = zero, z = zero, roll = zero, height = zero, length = zero;
  const uint32_t count = anim.blocks_used();
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t block = AnimBlock(anim, i);
    switch (schema::CameraAnimBlock(base, block).channel()) {
      case 0: x = Value(base, block, time); break;
      case 1: y = Value(base, block, time); break;
      case 2: z = Fneg(Value(base, block, time)); break;
      case 101: height = Value(base, block, time); break;
      case 102: length = Value(base, block, time); break;
      case 109: roll = Fneg(Value(base, block, time)); break;
      default: break;
    }
  }
  const double a = Fmuls(length, Lfs(base, kFilmScale));
  const double two = Lfs(base, kTwo);
  const double b = Fmuls(height, two);
  const double angle = Single(c_runtime::Atan(Fdivs(a, b)));
  const double half = Fmuls(Fmuls(angle, two), Lfs(base, kHalf));
  // tan (sub_825A8A50) as its adapter gives it with nearest rounding set.
  double t;
  if ((Fpscr::getcsr() & Fpscr::RoundMaskVal) != 0 || !c_runtime::Tan(half, &t)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82150350 CameraAnim::EvalCamera");
  const double tangent = Single(t);
  to.set_roll(float(roll));
  to.set_zoom(float(Fdivs(Lfs(base, kZoomScale), tangent)));
  to.set_x(float(x));
  to.set_y(float(y));
  to.set_z(float(z));
  to.set_w(float(Lfs(base, kOne)));
  return t;
}

void AnimEvalTarget(uint8_t* base, uint32_t object, uint32_t out, uint32_t time) {
  ITC_NATIVE_RUNNING("sub_821504E0 CameraAnim::EvalTarget");
  const schema::CameraAnim anim(base, object);
  const schema::CameraVec4 to(base, out);
  const double zero = Lfs(base, kZero);
  double x = zero, y = zero, z = zero;
  const uint32_t count = anim.blocks_used();
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t block = AnimBlock(anim, i);
    switch (schema::CameraAnimBlock(base, block).channel()) {
      case 0: x = Value(base, block, time); break;
      case 1: y = Value(base, block, time); break;
      case 2: z = Fneg(Value(base, block, time)); break;
      default: break;
    }
  }
  to.set_x(float(x));
  to.set_y(float(y));
  to.set_z(float(z));
  to.set_w(float(Lfs(base, kOne)));
}

void AnimMatrix(uint8_t* base, uint32_t object, uint32_t out, uint32_t time, uint32_t frame,
                const GuestCopy& memcpy) {
  ITC_NATIVE_RUNNING("sub_82150590 CameraAnim::Matrix");
  const schema::CameraAnim anim(base, object);
  const schema::CameraMatrix to(base, out);
  auto identity = [&](uint32_t at) {
    for (uint32_t row = 0; row < 4; row++) {
      for (uint32_t column = 0; column < 4; column++) {
        guest::StoreU32(base, at + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
      }
    }
  };
  identity(frame + 96);
  Copy(base, out, frame + 96, 64, memcpy);
  const double zero = Lfs(base, kZero);
  double x = zero, y = zero, z = zero, turn_x = zero, turn_y = zero, turn_z = zero;
  const uint32_t count = anim.blocks_used();
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t block = AnimBlock(anim, i);
    switch (schema::CameraAnimBlock(base, block).channel()) {
      case 0: x = Value(base, block, time); break;
      case 1: y = Value(base, block, time); break;
      case 2: z = Fneg(Value(base, block, time)); break;
      case 3: turn_x = Value(base, block, time); break;
      case 4: turn_y = Value(base, block, time); break;
      case 5: turn_z = Fneg(Value(base, block, time)); break;
      default: break;
    }
  }
  const uint32_t m = frame + 160, angle = frame + 80, turn = frame - 80;
  identity(frame + 96);
  for (uint32_t i = 0; i < 64; i += 4) {
    guest::StoreU32(base, m + i, guest::LoadU32(base, frame + 96 + i));
  }
  Stfs(base, angle, Lfs(base, kFirstTurn));
  matrix_math::RotateY(base, m, m, angle, turn);
  const double degrees = Lfs(base, kRadiansPerDegree);
  Stfs(base, angle, Fmuls(Fneg(turn_x), degrees));
  matrix_math::RotateX(base, m, m, angle, turn);
  Stfs(base, angle, Fmuls(Fneg(turn_y), degrees));
  matrix_math::RotateY(base, m, m, angle, turn);
  Stfs(base, angle, Fmuls(Fneg(turn_z), degrees));
  matrix_math::RotateZ(base, m, m, angle, turn);
  matrix_math::Multiply(base, out, out, m);
  interpolation::BlendState_OnMultiply(base, out, out, m, 0x821507B0);
  to.set_m30(float(x));
  to.set_m31(float(y));
  to.set_m32(float(z));
}

uint32_t AnimsEvaluate(uint8_t* base, uint32_t out, uint32_t id, uint32_t time,
                       const std::function<void(uint32_t object)>& camera_original,
                       std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_82150960 CameraAnims::Evaluate");
  const uint32_t objects = AnimsFind(base, id);
  if (objects == 0) {
    return 0;
  }
  for (uint32_t i = 0; i < schema::CameraAnimObjects(base, objects).count(); i++) {
    const uint32_t object = guest::LoadU32(base, objects + 4 * i);
    const int32_t kind = int32_t(schema::CameraAnim(base, object).kind());
    if (kind == int32_t(kEndKind)) {
      break;
    }
    if (kind == 6) {
      if (const auto t = AnimEvalCamera(base, object, out, time)) {
        *f1 = *t;
      } else {
        camera_original(object);
        *f1 = std::nullopt;
      }
    }
    if (int32_t(schema::CameraAnim(base, object).kind()) == 7) {
      AnimEvalTarget(base, object, out, time);
    }
  }
  return 1;
}

double AnimsDuration(uint8_t* base, uint32_t id, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_82150A98 CameraAnims::Duration");
  uint32_t slot = AnimsFind(base, id);
  const uint32_t count = schema::CameraAnimObjects(base, slot).count();
  for (uint32_t i = 0; i < count; i++, slot += 4) {
    const uint32_t object = guest::LoadU32(base, slot);
    const int32_t kind = int32_t(schema::CameraAnim(base, object).kind());
    if (kind == int32_t(kEndKind)) {
      break;
    }
    if (kind == 1 || kind == 6) {
      *r3 = slot;
      return double(schema::CameraAnimBlock(base, schema::CameraAnim(base, object).first_block_bits()).duration());
    }
  }
  *r3 = slot;
  return Lfs(base, kZero);
}

uint32_t AnimsObjectMatrix(uint8_t* base, uint32_t out, uint32_t id, uint32_t time,
                           uint32_t frame, const GuestCopy& memcpy) {
  ITC_NATIVE_RUNNING("sub_82150A10 CameraAnims::ObjectMatrix");
  const uint32_t objects = AnimsFind(base, id);
  if (objects == 0) {
    return 0;
  }
  const uint32_t count = schema::CameraAnimObjects(base, objects).count();
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t object = guest::LoadU32(base, objects + 4 * i);
    const int32_t kind = int32_t(schema::CameraAnim(base, object).kind());
    if (kind == int32_t(kEndKind)) {
      break;
    }
    if (kind == 1) {
      AnimMatrix(base, object, out, time, frame, memcpy);
      break;
    }
  }
  return 1;
}

// --- The camera paths ---

std::optional<int64_t> PathRequest(uint8_t* base, uint32_t buffer, uint32_t index,
                                   const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82152E08 CameraPath::Request");
  if (int32_t(index) == -1) {
    return std::nullopt;
  }
  const schema::CameraPathBuffer b(base, buffer);
  const int32_t state = int32_t(b.state());
  if (state != 0 && state != 3) {
    return std::nullopt;
  }
  const uint32_t id = guest::LoadU32(base, guest::LoadU32(base, (index << 2) + kPathAssets));
  game_modes::Buffer28Free(base, buffer);
  game_modes::Buffer28Reset(base, buffer);
  b.set_index(index);
  ITC_NATIVE_RUNNING("sub_82152D80 CameraPath::Load");
  b.set_state(1);
  // The file, a blob (E5 step 5) in place of the VirtualAlloc returning at
  // 0x82152DC0; Buffer28::Free frees it.
  const uint32_t data = Blob<schema::CameraPathFile>(base, archive::AssetSize(base, id));
  b.set_block(data);
  const int64_t serial = load(id, data, kPathOnLoaded, buffer);
  b.set_serial(uint32_t(serial));
  b.set_state(2);
  return serial;
}

void PathOnLoaded(uint8_t* base, uint32_t data, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152078 CameraPath::OnLoaded");
  const schema::CameraPathFile file(base, data);
  const uint32_t paths = data + schema::CameraPathFile::kSize;
  const uint32_t keys = file.path_count() * schema::CameraPathRecord::kSize + paths;
  const uint32_t curves = file.key_count() * schema::CameraPathKey::kSize + keys;
  for (uint32_t i = 0, path = paths; i < file.path_count(); i++, path += schema::CameraPathRecord::kSize) {
    FixKeys(base, path, keys, curves);
  }
  const schema::CameraPathBuffer b(base, buffer);
  b.set_first_path(paths);
  const uint32_t count = file.path_count();
  b.set_state(3);
  b.set_last_path(count * schema::CameraPathRecord::kSize + paths - schema::CameraPathRecord::kSize);
}

// --- The rival logo ---

void RivalLogoConstruct(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82211848 RivalLogo::RivalLogo");
  const schema::RivalLogo r(base, entry);
  r.set_movie(0);
  r.set_logo(0xFFFFFFFFu);
  r.set_flags(2);
  r.set_anim(0);
  r.set_asset(0);
  ITC_NATIVE_RUNNING("sub_82211708 RivalLogo::InitXpr");
  // The holder (+20): its +0, its XPR (+4), +28, +56 to +68.
  r.set_holder(0xFFFFFFFFu);
  if (r.xpr_address() != 0) {
    models::XprConstruct(base, r.xpr_address());
  }
  r.set_xpr_flags(2);
  r.set_old_texture0(0);
  r.set_old_texture1(0);
  r.set_texture_name0(3);
  r.set_texture_name1(4);
}

uint32_t RivalLogoPlay(uint8_t* base, uint32_t entry, uint32_t key_slot) {
  ITC_NATIVE_RUNNING("sub_82211AB0 RivalLogo::Play");
  const schema::RivalLogo r(base, entry);
  if ((r.flags() & 0x4) == 0) {
    return entry;
  }
  const uint32_t loaded = ui_movies::IsLoaded(base, r.movie());
  const uint32_t logo = r.logo();
  if (logo != 0 ? loaded == 0 : (r.xpr_flags() & 0x10) == 0) {
    return loaded;
  }
  guest::StoreU32(base, key_slot, r.movie());
  const uint32_t movie =
      guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key_slot));
  if (logo == 0) {
    for (uint32_t i = 0; i < 2; i++) {
      const uint32_t old = ui_movies::SwapTexture(
          base, movie, guest::LoadU32(base, r.texture_name0_address() + 4 * i),
          r.textures_address() + 12 * i);
      guest::StoreU32(base, r.old_texture0_address() + 4 * i, old);
    }
  }
  const uint32_t r3 = ui_movies::PlayNamed(base, movie, r.anim(), 0, 0, 0, 0);
  const uint32_t flags = r.flags() & ~0x4u;
  r.set_flags(flags);
  r.set_flags(flags | 0x8);
  return r3;
}

uint32_t RivalLogoOpen(uint8_t* base, uint32_t entry, uint32_t logo, uint32_t texture,
                       const std::function<uint32_t(uint32_t, uint32_t)>& open,
                       const std::function<uint32_t(uint32_t, uint32_t)>& load_xpr) {
  ITC_NATIVE_RUNNING("sub_822118E0 RivalLogo::Open");
  const schema::RivalLogo r(base, entry);
  if (int32_t(logo) == -1) {
    r.set_flags(1);
    return entry;
  }
  if ((r.flags() & 0x2) == 0) {
    return entry;
  }
  constexpr uint32_t kTextures = 0x8200CEA0;
  const uint32_t record = kTextures - 304 + logo * 12;
  r.set_logo(logo);
  r.set_flags(r.flags() | 4);
  r.set_movie(guest::LoadU32(base, record + 0));
  r.set_anim(guest::LoadU32(base, record + 4));
  const uint32_t asset = guest::LoadU32(base, record + 8);
  r.set_asset(asset);
  uint32_t r3 = open(r.movie(), asset);
  if (r.logo() == 0) {
    r3 = load_xpr(r.holder_address(), guest::LoadU32(base, kTextures + (texture << 2)));
  }
  return r3;
}

uint32_t RivalLogoClose(uint8_t* base, uint32_t entry, uint32_t frame,
                        const RivalLogoCloseCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82211980 RivalLogo::Close");
  const schema::RivalLogo r(base, entry);
  guest::StoreU32(base, frame + 80, r.movie());
  uint32_t r3 = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = guest::LoadU32(base, r3);
  if ((r.flags() & 0x8) != 0) {
    calls.finish_all(movie);
    if (r.logo() == 0) {
      for (uint32_t i = 0; i < 2; i++) {
        ui_movies::SwapTexture(base, movie, guest::LoadU32(base, r.texture_name0_address() + 4 * i),
                               guest::LoadU32(base, r.old_texture0_address() + 4 * i));
      }
      calls.release_xpr(r.holder_address());
    }
    r3 = calls.remove(r.movie());
  }
  r.set_movie(0);
  r.set_logo(0xFFFFFFFFu);
  r.set_flags(1);
  r.set_anim(0);
  r.set_asset(0);
  return r3;
}

}  // namespace camera_modes
