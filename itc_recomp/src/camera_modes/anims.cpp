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
#include "memory/heap.h"
#include "models/xpr.h"
#include "ui_movies/game.h"

namespace camera_modes {

using namespace steps;

namespace {

constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kHalf = 0x82001BE4;  // 0.5
constexpr uint32_t kTwo = 0x820021E8;   // 2.0
constexpr uint32_t kRadiansPerDegree = 0x82002948;
constexpr uint32_t kFilmScale = 0x82002D0C;   // EvalCamera's length scale
constexpr uint32_t kZoomScale = 0x82002C30;   // EvalCamera's zoom numerator
constexpr uint32_t kFirstTurn = 0x82001FB4;   // CameraAnim::Matrix's first RotateY

constexpr uint32_t kEndKind = 14;
constexpr uint32_t kObjectBytes = 340;
constexpr uint32_t kBlockBytes = 64;
constexpr uint32_t kBlocks = 80;       // an object's blocks
constexpr uint32_t kBlockCount = 336;  // their count
constexpr uint32_t kBlockData = 60;    // a block's data

// VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE): the layer's pages.
uint32_t VirtualAlloc(uint32_t size) { return memory::AllocPages(size, 4096); }

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
  return Lfs(base, guest::LoadU32(base, block + kBlockData) + (time << 2));
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
  if (!memory::IsPages(object)) {
    return false;
  }
  const uint32_t count = guest::LoadU32(base, object + kBlockCount);
  if (count > 64) {
    return false;
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t block = guest::LoadU32(base, object + kBlocks + 4 * i);
    if (!memory::IsPages(block)) {
      return false;
    }
    const uint32_t data = guest::LoadU32(base, block + kBlockData);
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
  FixPair(base, key + 8, curves);
  FixPair(base, key + 16, curves);
  FixGroup(base, key + 24, curves);
  FixGroup(base, key + 120, curves);
}

void FixKeys(uint8_t* base, uint32_t path, uint32_t keys, uint32_t curves) {
  ITC_NATIVE_RUNNING("sub_820AC000 CameraPath::FixKeys");
  const uint32_t first = guest::LoadU32(base, path + 16) * 216 + keys;
  guest::StoreU32(base, path + 16, first);
  for (uint32_t i = 0, at = 0; i < guest::LoadU32(base, path + 8); i++, at += 216) {
    FixKey(base, guest::LoadU32(base, path + 16) + at, curves);
  }
}

void BlockFree(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_8214FFF0 CameraAnimBlock::Free");
  const uint32_t data = guest::LoadU32(base, block + kBlockData);
  if (data != 0) {
    memory::FreePages(data);
  }
  guest::StoreU32(base, block + kBlockData, 0);
}

void AnimFree(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82150098 CameraAnim::Free");
  while (guest::LoadU32(base, object + kBlockCount) != 0) {
    const uint32_t count = guest::LoadU32(base, object + kBlockCount) - 1;
    guest::StoreU32(base, object + kBlockCount, count);
    const uint32_t block = guest::LoadU32(base, object + kBlocks + (count << 2));
    BlockFree(base, block);
    memory::FreePages(block);
  }
}

// --- The camera animations' files ---

int64_t AnimsLoad(uint8_t* base, uint32_t id, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82150B10 CameraAnims::Load");
  const uint32_t size = archive::AssetSize(base, id);
  const uint32_t record = VirtualAlloc(4);
  const uint32_t buffer = VirtualAlloc(size);
  guest::StoreU32(base, record, id);
  return load(id, buffer, kAnimsOnLoaded, record);
}

uint32_t AnimParseObject(uint8_t* base, uint32_t object, const GuestCopy& xmemcpy) {
  ITC_NATIVE_RUNNING("sub_82150100 CameraAnim::ParseObject");
  const uint32_t copy = VirtualAlloc(kObjectBytes);
  const double zero = Lfs(base, kZero);
  guest::StoreU32(base, copy + 0, 0);
  guest::StoreU32(base, copy + 4, 0);
  guest::StoreU32(base, copy + 8, 0);
  guest::StoreU32(base, copy + 12, 0);
  for (const uint32_t at : {20u, 20u, 16u, 32u, 32u, 28u, 40u, 44u}) {
    Stfs(base, copy + at, zero);
  }
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU32(base, copy + 48 + 4 * i, 0);
  }
  guest::StoreU32(base, copy + kBlockCount, 0);
  for (uint32_t at = 0; at < 40; at += 4) {
    guest::StoreU32(base, copy + at, guest::LoadU32(base, object + at));
  }
  Stfs(base, copy + 40, Lfs(base, object + 40));
  Stfs(base, copy + 44, Lfs(base, object + 44));
  uint32_t from = object + guest::LoadU32(base, object + 4);
  if (guest::LoadU32(base, copy + 12) == 0) {
    return copy;
  }
  uint32_t n = 0;
  do {
    const uint32_t block = VirtualAlloc(kBlockBytes);
    Stfs(base, block + 12, zero);
    guest::StoreU32(base, block + 0, 0);
    Stfs(base, block + 16, zero);
    guest::StoreU32(base, block + 4, 0);
    guest::StoreU32(base, block + 8, 0);
    guest::StoreU32(base, block + kBlockData, 0);
    for (uint32_t i = 0; i < 10; i++) {
      guest::StoreU32(base, block + 20 + 4 * i, 0);
    }
    guest::StoreU32(base, block + 0, guest::LoadU32(base, from + 0));
    guest::StoreU32(base, block + 4, guest::LoadU32(base, from + 4));
    const uint32_t bytes = guest::LoadU32(base, from + 8);
    guest::StoreU32(base, block + 8, bytes);
    Stfs(base, block + 12, Lfs(base, from + 12));
    Stfs(base, block + 16, Lfs(base, from + 16));
    guest::StoreU32(base, block + kBlockData, VirtualAlloc(bytes));
    c_runtime::XMemSet(base, guest::LoadU32(base, block + kBlockData), 0,
                       guest::LoadU32(base, block + 8));
    {
      const uint32_t to = guest::LoadU32(base, block + kBlockData);
      const uint32_t source = guest::LoadU32(base, from + 4) + from;
      const uint32_t size = guest::LoadU32(base, block + 8);
      if (!c_runtime::XMemCpy(base, to, source, size)) {
        xmemcpy(to, source, size);
      }
    }
    const uint32_t count = guest::LoadU32(base, copy + kBlockCount);
    guest::StoreU32(base, copy + kBlockCount, count + 1);
    guest::StoreU32(base, copy + kBlocks + (count << 2), block);
    from = guest::LoadU32(base, from + 4) + from + guest::LoadU32(base, from + 8);
    n++;
  } while (n < guest::LoadU32(base, copy + 12));
  return copy;
}

bool AnimsOnLoaded(uint8_t* base, uint32_t data, uint32_t record, uint32_t frame,
                   const GuestCopy& memcpy, const GuestCopy& xmemcpy) {
  if (!memory::IsPages(record) || !memory::IsPages(data)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82150878 CameraAnims::OnLoaded");
  guest::StoreU32(base, frame, frame + 672);
  guest::StoreU32(base, frame + 80, 0);
  const uint32_t id = guest::LoadU32(base, record);
  guest::StoreU32(base, frame + 340, 0);
  uint32_t object = data;
  uint32_t count = 0;
  for (;;) {
    const uint32_t parsed = AnimParseObject(base, object, xmemcpy);
    guest::StoreU32(base, frame + 608, count + 1);
    guest::StoreU32(base, frame + 352 + (count << 2), parsed);
    if (int32_t(guest::LoadU32(base, object)) == int32_t(kEndKind)) {
      break;
    }
    count = guest::LoadU32(base, frame + 608);
    object = guest::LoadU32(base, object + 8) + guest::LoadU32(base, object + 4) + object;
  }
  guest::StoreU32(base, frame + 80, id);
  Copy(base, frame + 84, frame + 352, 260, memcpy);
  const uint32_t n = guest::LoadU32(base, kAnimCount);
  guest::StoreU32(base, kAnimCount, n + 1);
  Copy(base, n * kAnimBytes + kAnims, frame + 80, kAnimBytes, memcpy);
  memory::FreePages(record);
  memory::FreePages(data);
  return true;
}

bool AnimsFreeable(uint8_t* base) {
  const uint32_t entries = guest::LoadU32(base, kAnimCount);
  if (entries > 64) {
    return false;
  }
  for (uint32_t e = 0; e < entries; e++) {
    const uint32_t entry = kAnims + kAnimBytes * e;
    const uint32_t count = guest::LoadU32(base, entry + 260);
    if (count > 64) {
      return false;
    }
    for (uint32_t i = 0; i < count; i++) {
      if (!ObjectFreeable(base, guest::LoadU32(base, entry + 4 + 4 * i))) {
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
  for (uint32_t e = guest::LoadU32(base, kAnimCount); e != 0;
       e = guest::LoadU32(base, kAnimCount)) {
    e--;
    guest::StoreU32(base, kAnimCount, e);
    const uint32_t entry = e * kAnimBytes + kAnims;
    uint32_t count = guest::LoadU32(base, entry + 260);
    r3 = memcpy_r3;
    while (count != 0) {
      count--;
      const uint32_t object = guest::LoadU32(base, entry + 4 + (count << 2));
      AnimFree(base, object);
      memory::FreePages(object);
      r3 = 1;
    }
  }
  return r3;
}

std::optional<double> AnimEvalCamera(uint8_t* base, uint32_t object, uint32_t out,
                                     uint32_t time) {
  const double zero = Lfs(base, kZero);
  double x = zero, y = zero, z = zero, roll = zero, height = zero, length = zero;
  const uint32_t count = guest::LoadU32(base, object + kBlockCount);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t block = guest::LoadU32(base, object + kBlocks + 4 * i);
    switch (guest::LoadU32(base, block)) {
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
  Stfs(base, out + 32, roll);
  Stfs(base, out + 36, Fdivs(Lfs(base, kZoomScale), tangent));
  Stfs(base, out + 16, x);
  Stfs(base, out + 20, y);
  Stfs(base, out + 24, z);
  Stfs(base, out + 28, Lfs(base, kOne));
  return t;
}

void AnimEvalTarget(uint8_t* base, uint32_t object, uint32_t out, uint32_t time) {
  ITC_NATIVE_RUNNING("sub_821504E0 CameraAnim::EvalTarget");
  const double zero = Lfs(base, kZero);
  double x = zero, y = zero, z = zero;
  const uint32_t count = guest::LoadU32(base, object + kBlockCount);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t block = guest::LoadU32(base, object + kBlocks + 4 * i);
    switch (guest::LoadU32(base, block)) {
      case 0: x = Value(base, block, time); break;
      case 1: y = Value(base, block, time); break;
      case 2: z = Fneg(Value(base, block, time)); break;
      default: break;
    }
  }
  Stfs(base, out + 0, x);
  Stfs(base, out + 4, y);
  Stfs(base, out + 8, z);
  Stfs(base, out + 12, Lfs(base, kOne));
}

void AnimMatrix(uint8_t* base, uint32_t object, uint32_t out, uint32_t time, uint32_t frame,
                const GuestCopy& memcpy) {
  ITC_NATIVE_RUNNING("sub_82150590 CameraAnim::Matrix");
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
  const uint32_t count = guest::LoadU32(base, object + kBlockCount);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t block = guest::LoadU32(base, object + kBlocks + 4 * i);
    switch (guest::LoadU32(base, block)) {
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
  Stfs(base, out + 48, x);
  Stfs(base, out + 52, y);
  Stfs(base, out + 56, z);
}

uint32_t AnimsEvaluate(uint8_t* base, uint32_t out, uint32_t id, uint32_t time,
                       const std::function<void(uint32_t object)>& camera_original,
                       std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_82150960 CameraAnims::Evaluate");
  const uint32_t objects = AnimsFind(base, id);
  if (objects == 0) {
    return 0;
  }
  for (uint32_t i = 0; i < guest::LoadU32(base, objects + 256); i++) {
    const uint32_t object = guest::LoadU32(base, objects + 4 * i);
    const int32_t kind = int32_t(guest::LoadU32(base, object));
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
    if (int32_t(guest::LoadU32(base, object)) == 7) {
      AnimEvalTarget(base, object, out, time);
    }
  }
  return 1;
}

double AnimsDuration(uint8_t* base, uint32_t id, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_82150A98 CameraAnims::Duration");
  uint32_t slot = AnimsFind(base, id);
  const uint32_t count = guest::LoadU32(base, slot + 256);
  for (uint32_t i = 0; i < count; i++, slot += 4) {
    const uint32_t object = guest::LoadU32(base, slot);
    const int32_t kind = int32_t(guest::LoadU32(base, object));
    if (kind == int32_t(kEndKind)) {
      break;
    }
    if (kind == 1 || kind == 6) {
      *r3 = slot;
      return Lfs(base, guest::LoadU32(base, object + kBlocks) + 16);
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
  const uint32_t count = guest::LoadU32(base, objects + 256);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t object = guest::LoadU32(base, objects + 4 * i);
    const int32_t kind = int32_t(guest::LoadU32(base, object));
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
  const int32_t state = int32_t(guest::LoadU32(base, buffer + 24));
  if (state != 0 && state != 3) {
    return std::nullopt;
  }
  const uint32_t id = guest::LoadU32(base, guest::LoadU32(base, (index << 2) + kPathAssets));
  game_modes::Buffer28Free(base, buffer);
  game_modes::Buffer28Reset(base, buffer);
  guest::StoreU32(base, buffer + 12, index);
  ITC_NATIVE_RUNNING("sub_82152D80 CameraPath::Load");
  guest::StoreU32(base, buffer + 24, 1);
  const uint32_t data = VirtualAlloc(archive::AssetSize(base, id));
  guest::StoreU32(base, buffer + 20, data);
  const int64_t serial = load(id, data, kPathOnLoaded, buffer);
  guest::StoreU32(base, buffer + 16, uint32_t(serial));
  guest::StoreU32(base, buffer + 24, 2);
  return serial;
}

void PathOnLoaded(uint8_t* base, uint32_t data, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152078 CameraPath::OnLoaded");
  const uint32_t paths = data + 16;
  const uint32_t keys = guest::LoadU32(base, data + 0) * 20 + paths;
  const uint32_t curves = guest::LoadU32(base, data + 4) * 216 + keys;
  for (uint32_t i = 0, path = paths; i < guest::LoadU32(base, data + 0); i++, path += 20) {
    FixKeys(base, path, keys, curves);
  }
  guest::StoreU32(base, buffer + 4, paths);
  const uint32_t count = guest::LoadU32(base, data + 0);
  guest::StoreU32(base, buffer + 24, 3);
  guest::StoreU32(base, buffer + 8, count * 20 + paths - 20);
}

// --- The rival logo ---

void RivalLogoConstruct(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82211848 RivalLogo::RivalLogo");
  guest::StoreU32(base, entry + 8, 0);
  guest::StoreU32(base, entry + 0, 0xFFFFFFFFu);
  guest::StoreU32(base, entry + 4, 2);
  guest::StoreU32(base, entry + 12, 0);
  guest::StoreU32(base, entry + 16, 0);
  ITC_NATIVE_RUNNING("sub_82211708 RivalLogo::InitXpr");
  const uint32_t holder = entry + 20;
  guest::StoreU32(base, holder + 0, 0xFFFFFFFFu);
  if (holder + 4 != 0) {
    models::XprConstruct(base, holder + 4);
  }
  guest::StoreU32(base, holder + 28, 2);
  guest::StoreU32(base, holder + 56, 0);
  guest::StoreU32(base, holder + 60, 0);
  guest::StoreU32(base, holder + 64, 3);
  guest::StoreU32(base, holder + 68, 4);
}

uint32_t RivalLogoPlay(uint8_t* base, uint32_t entry, uint32_t key_slot) {
  ITC_NATIVE_RUNNING("sub_82211AB0 RivalLogo::Play");
  if ((guest::LoadU32(base, entry + 4) & 0x4) == 0) {
    return entry;
  }
  const uint32_t loaded = ui_movies::IsLoaded(base, guest::LoadU32(base, entry + 8));
  const uint32_t logo = guest::LoadU32(base, entry + 0);
  if (logo != 0 ? loaded == 0 : (guest::LoadU32(base, entry + 48) & 0x10) == 0) {
    return loaded;
  }
  guest::StoreU32(base, key_slot, guest::LoadU32(base, entry + 8));
  const uint32_t movie =
      guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key_slot));
  if (logo == 0) {
    for (uint32_t i = 0; i < 2; i++) {
      const uint32_t old = ui_movies::SwapTexture(
          base, movie, guest::LoadU32(base, entry + 84 + 4 * i), entry + 52 + 12 * i);
      guest::StoreU32(base, entry + 76 + 4 * i, old);
    }
  }
  const uint32_t r3 =
      ui_movies::PlayNamed(base, movie, guest::LoadU32(base, entry + 12), 0, 0, 0, 0);
  const uint32_t flags = guest::LoadU32(base, entry + 4) & ~0x4u;
  guest::StoreU32(base, entry + 4, flags);
  guest::StoreU32(base, entry + 4, flags | 0x8);
  return r3;
}

}  // namespace camera_modes
