// The light billboards' recording (docs/functions/sprites.md):
// Billboards::RecordState (sub_8209C1A0), Billboards::Record (sub_8209CC38)
// and Billboards::Draw (sub_8209CDB0), step for step as the translated code
// (built with strict floats, CMakeLists.txt). Each keeps the original's frame
// (its back chain written as stwu does, its temporaries at the original's
// offsets: frame interpolation notes the multiply by the camera pass's
// projection on RecordState's stack) and runs its guest calls from it with
// the original's return addresses. The vertices are built inside frame
// interpolation's camera blend (interpolation::BlendState_BuildBillboardVertices,
// as the vertex builder's adapter does), and Record runs inside the checks its
// diagnostics' wrap puts around the original (src/debug/frame_state_diff.cpp).

#include "sprites/billboards.h"

#include "car_scene_draw/car_scene_draw.h"
#include "car_scene_draw/ops.h"
#include "guest.h"

#include "base/log.h"
#include "command_list/command_list.h"
#include "debug/frame_state_diff.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "native/buffers.h"

#include <initializer_list>

namespace sprites {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kStateFrame = 368;
constexpr uint32_t kRecordFrame = 160;
constexpr uint32_t kDrawFrame = 96;
constexpr uint32_t kVerticesFrame = 304;  // Billboards::BuildVertices's

constexpr uint32_t kRenderApp = 0x826BABA0;   // the render manager
constexpr uint32_t kAlphaRef = 0x8266D090;    // the 1024 alpha mode's reference
constexpr uint32_t kVertexShader = 0x826A50A0;
constexpr uint32_t kPixelShader = 0x826A50A4;

constexpr uint32_t kOpenChunk = 0x8258C9D0;
constexpr uint32_t kAllocateSlots = 0x8258CAA0;
constexpr uint32_t kAllocateBytes = 0x8258CB98;
constexpr uint32_t kMemcpyReturn = 0x8209C53C;
constexpr uint32_t kMultiplyReturn1 = 0x8209C59C;
constexpr uint32_t kMultiplyReturn2 = 0x8209C5AC;

// Flush-to-zero off, as the callees' plain C++ expects at entry.
void FlushOff() { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); }

class Writer {
 public:
  Writer(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls, uint32_t f, uint32_t list)
      : base_(base), calls_(calls), f_(f), list_(list) {}

  // CommandList::AllocateSlots (one slot), the original where the list is
  // full (it prints its message and returns 0).
  uint32_t Slot(uint32_t lr) {
    const uint32_t first = command_list::AllocateSlots(base_, list_, 1);
    if (first != 0) {
      return first;
    }
    return uint32_t(Call(kAllocateSlots, {list_, 1}, lr));
  }
  // A slot's words +0, +4 (and +8 for three).
  uint32_t Slot(uint32_t lr, std::initializer_list<uint32_t> words) {
    const uint32_t slot = Slot(lr);
    uint32_t at = slot;
    for (const uint32_t w : words) {
      Store(base_, at, w);
      at += 4;
    }
    return slot;
  }
  uint64_t Call(uint32_t function, std::initializer_list<uint64_t> regs, uint32_t lr,
                std::initializer_list<double> fregs = {}) {
    const uint64_t r3 = calls_.call(function, regs.begin(), int(regs.size()), fregs.begin(),
                                    int(fregs.size()), f_, lr);
    FlushOff();
    return r3;
  }
  uint8_t* base() const { return base_; }
  uint32_t list() const { return list_; }

 private:
  uint8_t* base_;
  const car_scene_draw::SceneDrawCalls& calls_;
  uint32_t f_;
  uint32_t list_;
};

}  // namespace

uint32_t RecordBillboardState(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls,
                              uint32_t stack, uint32_t set, uint32_t list, uint32_t material) {
  ITC_NATIVE_RUNNING("sub_8209C1A0 Billboards::RecordState");
  const uint32_t f = stack - kStateFrame;
  Store(base, f, stack);  // the back chain, as stwu
  Writer w(base, calls, f, list);

  // The alpha mode's render states.
  const int32_t alpha = int32_t(Load(base, material + 0));
  if (alpha == 16 || alpha == 32 || alpha == 64 || alpha == 256) {
    w.Slot(0x8209C260, {1, 1, 0});
    w.Slot(0x8209C27C, {56, 0});
  } else if (alpha == 1024) {
    Stfs(base, f + 80, Lfs(base, kAlphaRef));
    w.Slot(0x8209C20C, {1, 2, 0});
    w.Slot(0x8209C228, {56, 0});
    w.Slot(0x8209C244, {208, Load(base, f + 80)});
  }

  // The blend mode's.
  switch (int32_t(Load(base, material + 4))) {
    case 1:
      w.Slot(0x8209C45C, {1, 1, 0});
      w.Slot(0x8209C478, {96, 0});
      break;
    case 2:
      w.Slot(0x8209C40C, {1, 2, 0});
      w.Slot(0x8209C428, {96, 1});
      w.Slot(0x8209C444, {100, 64});
      break;
    case 4:
      w.Slot(0x8209C384, {1, 4, 0});
      w.Slot(0x8209C3A0, {60, 1});
      w.Slot(0x8209C3BC, {80, 0});
      w.Slot(0x8209C3D8, {76, 1});
      w.Slot(0x8209C3F4, {72, 1});
      break;
    case 8:
      w.Slot(0x8209C338, {1, 2, 0});
      w.Slot(0x8209C354, {76, 4});
      w.Slot(0x8209C370, {72, 0});
      break;
    case 16:
      w.Slot(0x8209C2C8, {1, 3, 0});
      w.Slot(0x8209C2E4, {80, 4});
      w.Slot(0x8209C300, {76, 1});
      w.Slot(0x8209C31C, {72, 6});
      break;
    default:
      break;
  }
  w.Slot(0x8209C490, {1, 2, 0});
  w.Slot(0x8209C4AC, {48, 0});
  w.Slot(0x8209C4C8, {40, 1});

  // The identity at +96 (its w column's 1.0 stores overwritten by the
  // vector stores, as the original's), the camera pass's view at +224.
  const double one = Lfs(base, kOne);
  for (uint32_t at : {108u, 124u, 140u, 156u}) {
    Stfs(base, f + at, one);
  }
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t col = 0; col < 4; col++) {
      Store(base, f + 96 + 16 * row + 4 * col, row == col ? 0x3F800000u : 0u);
    }
  }
  const uint32_t pass = Load(base, set);
  if (!native::Buffers_Memcpy(base, f + 224, pass + 16, 64, kMemcpyReturn)) {
    calls.memcpy(f + 224, pass + 16, 64, f, kMemcpyReturn);
  }
  FlushOff();

  // Command 7, vertex c0 x4: the identity x (the view x the pass's
  // projection at +80), both multiplies noted for frame interpolation.
  w.Slot(0x8209C548, {7, 4, 0x10004});
  uint32_t matrix = command_list::AllocateBytes(base, list, 64, false);
  if (matrix == 0) {
    matrix = uint32_t(w.Call(kAllocateBytes, {list, 64, 0}, 0x8209C574));
  }
  for (uint32_t at : {172u, 188u, 204u, 220u}) {
    Stfs(base, f + at, one);
  }
  const uint32_t projection = Load(base, set) + 80;  // read again, as the original
  matrix_math::Multiply(base, f + 160, f + 224, projection);
  interpolation::BlendState_OnMultiply(base, f + 160, f + 224, projection, kMultiplyReturn1);
  matrix_math::Multiply(base, matrix, f + 96, f + 160);
  interpolation::BlendState_OnMultiply(base, matrix, f + 96, f + 160, kMultiplyReturn2);
  FlushOff();

  // The shaders and the texture (or none in stages 0 to 3).
  w.Slot(0x8209C5B8, {8, 1, 0});
  w.Slot(0x8209C5D8, {Load(base, kVertexShader)});
  w.Slot(0x8209C5F4, {9, 1, 0});
  w.Slot(0x8209C614, {Load(base, kPixelShader)});
  if (Load(base, material + 12) != 0) {
    w.Slot(0x8209C638, {4, 1, 0});
    return w.Slot(0x8209C654, {0, Load(base, material + 12)});
  }
  w.Slot(0x8209C668, {4, 4, 0});
  w.Slot(0x8209C684, {0, 0});
  w.Slot(0x8209C69C, {1, 0});
  w.Slot(0x8209C6B4, {2, 0});
  return w.Slot(0x8209C6CC, {3, 0});
}

uint64_t RecordBillboards(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls,
                          uint32_t stack, uint32_t set, uint32_t material) {
  ITC_NATIVE_RUNNING("sub_8209CC38 Billboards::Record");
  const uint32_t f = stack - kRecordFrame;
  Store(base, f, stack);  // the back chain, as stwu
  auto vtable_call = [&](uint32_t object, uint32_t slot, uint32_t lr,
                         std::initializer_list<uint64_t> more = {},
                         std::initializer_list<double> fregs = {}) {
    uint64_t regs[4] = {object, 0, 0, 0};
    int count = 1;
    for (const uint64_t r : more) {
      regs[count++] = r;
    }
    const uint32_t function = Load(base, Load(base, object) + 4 * slot);
    const uint64_t r3 =
        calls.call(function, regs, count, fregs.begin(), int(fregs.size()), f, lr);
    FlushOff();
    return r3;
  };
  // The render manager's command list (its slot 8, then slot 1).
  const uint32_t object = uint32_t(vtable_call(Load(base, kRenderApp), 8, 0x8209CC68));
  const uint32_t list = uint32_t(vtable_call(object, 1, 0x8209CC78));
  Writer w(base, calls, f, list);
  if (command_list::OpenChunk(base, list) == command_list::OpenResult::kNested) {
    w.Call(kOpenChunk, {list}, 0x8209CC80);
  }
  const uint32_t chunk = w.Slot(0x8209CC8C);
  guest::StoreU16(base, chunk + 2, 5);
  Store(base, chunk + 4, 0);

  RecordBillboardState(base, calls, f, set, list, material);
  FlushOff();
  // BuildVertices' frame's back chain, as its adapter makes it (its slots'
  // calls run from it).
  Store(base, f - kVerticesFrame, f);
  struct Slots {
    Writer* w;
    uint8_t* base;
    uint32_t list;
  } user{&w, base, list};
  const SlotAllocator slots{
      [](void* u, uint32_t count, uint32_t return_address) -> uint32_t {
        auto& s = *static_cast<Slots*>(u);
        const uint32_t first = command_list::AllocateSlots(s.base, s.list, count);
        if (first != 0) {
          return first;
        }
        return uint32_t(s.w->Call(kAllocateSlots, {s.list, count}, return_address));
      },
      &user};
  const uint32_t counted =
      interpolation::BlendState_BuildBillboardVertices(base, set, material, slots);
  FlushOff();

  // Three render states (alpha reference 0.0, alpha test off) and command
  // 36.
  const double zero = Lfs(base, kZero);
  Stfs(base, f + 80, zero);
  w.Slot(0x8209CCDC, {1, 2, 0});
  w.Slot(0x8209CD00, {208, Load(base, f + 80)});
  w.Slot(0x8209CD20, {48, 1});
  w.Slot(0x8209CD3C, {36, 0, 0});
  command_list::CloseChunk(base, list);
  if (counted == 0) {
    return list;  // CloseChunk's r3
  }
  // Handed to the render queue (slot 8, its slot 2, then slot 4: the chunk,
  // the set's layer, depth 0.0).
  const uint32_t object2 = uint32_t(vtable_call(Load(base, kRenderApp), 8, 0x8209CD70));
  const uint32_t queue = uint32_t(vtable_call(object2, 2, 0x8209CD80));
  return vtable_call(queue, 4, 0x8209CD9C, {chunk, Load(base, set + 4)}, {zero});
}

uint64_t DrawBillboards(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls,
                        uint32_t stack, uint32_t set, uint32_t material) {
  ITC_NATIVE_RUNNING("sub_8209CDB0 Billboards::Draw");
  const uint32_t f = stack - kDrawFrame;
  Store(base, f, stack);  // the back chain, as stwu
  if (Load(base, material + 32) == 0) {
    return 0;
  }
  // Record inside its wrap's checks (--debug_hide, --watch_lights,
  // --find_per_frame_state).
  if (!debug::IsHidden(0x8209CC38)) {
    debug::WatchLights(base, material);
    debug::FrameStateDiff_Before(base);
    FlushOff();
    RecordBillboards(base, calls, f, set, material);
    FlushOff();
    debug::FrameStateDiff_After(base, "draw step part", 0x8209CC38);
  }
  return 1;
}

}  // namespace sprites
