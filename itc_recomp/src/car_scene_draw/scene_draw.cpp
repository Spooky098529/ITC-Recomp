// CarScene::Draw (sub_820C5DE8, docs/functions/car-scene-draw.md), the car
// scene's draw, step for step as the translated code (see car_scene_draw.h):
// its 1,696-byte frame with the original's temporaries at their offsets
// (frame interpolation pairs the matrices it builds by guest address), its
// callees' plain C++ inside the checks their diagnostics' wraps put around
// the originals, and the command list chunks recorded with the original's
// calls (the render manager's getters before every command list call, as
// the original makes them).
//
// Part 4a: the set-up and both paths' records. Part 4b: the pass loop and
// the end (marked below).

#include "car_scene_draw/car_scene_draw.h"

#include "car_scene_draw/ops.h"
#include "guest.h"

#include "base/log.h"
#include "cameras/cameras.h"
#include "command_list/command_list.h"
#include "debug/frame_state_diff.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "models/xpr.h"
#include "native/buffers.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>
#include <initializer_list>

namespace car_scene_draw {

namespace {

using namespace ops;

constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kPassSize = 592;
constexpr uint32_t kRenderApp = 0x826BABA0;   // the render manager
constexpr uint32_t kSharedTexture = 0x8296B8F0;
constexpr uint32_t kLightDirection = 0x8296B950;  // the scene lights' first vector
constexpr uint32_t kRecords = 0x82002D40;  // the three records, 60 bytes each (w0 to w14)
constexpr uint32_t kRecordSize = 60;
constexpr uint32_t kLightX = 0x82001BE8;   // the second path's light's x

// The originals' wrapped callees (src/debug/frame_state_diff.cpp).
constexpr uint32_t kXprGetTexture = 0x82597118;
constexpr uint32_t kDisable = 0x820A0958;
constexpr uint32_t kEnableForView = 0x820AA1A8;
constexpr uint32_t kRecordBegins = 0x8209F8A0;
constexpr uint32_t kRecordEnds = 0x8209F900;
constexpr uint32_t kRenderTarget = 0x8209F7A8;
constexpr uint32_t kPlayerModel = 0x820C4FB8;
constexpr uint32_t kListAt = 0x82593FC0;
constexpr uint32_t kSetLight = 0x820C4EB8;
constexpr uint32_t kBeam = 0x820DAAC8;
constexpr uint32_t kBuildOrtho = 0x82597518;
constexpr uint32_t kBuild = 0x82597610;
constexpr uint32_t kFit = 0x82598768;
constexpr uint32_t kBeamShape = 0x820DAB10;
constexpr uint32_t kBeamColor = 0x820DAA38;
constexpr uint32_t kStagesCopy = 0x820FFD88;  // its adapter's wrap does the same checks
constexpr uint32_t kCarDrawUpdate = 0x820DAC68;
// The command list's (their adapters run the originals where the plain C++
// declines).
constexpr uint32_t kOpenChunk = 0x8258C9D0;
constexpr uint32_t kAllocateSlots = 0x8258CAA0;

int32_t LoadS(uint8_t* base, uint32_t address) { return int32_t(Load(base, address)); }

// Flush-to-zero off, as the callees' plain C++ expects at entry.
void FlushOff() { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); }

// The doubleword at `from` to `to` (an ld/std pair).
void CopyDoubleword(uint8_t* base, uint32_t to, uint32_t from) {
  uint64_t word;
  std::memcpy(&word, base + from, 8);
  std::memcpy(base + to, &word, 8);
}

// std of a 64-bit value (big-endian, high word first).
void StoreDoubleword(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

uint32_t Pass(uint8_t* base, uint32_t index) { return kCameraPasses + index * kPassSize; }

// A wrapped callee's call: nothing when --debug_hide names it (r3 then
// `r3`, the argument the original set), else `body` between
// FrameStateDiff_Before and After, flush-to-zero off around it. Returns r3.
template <typename Body>
uint64_t Wrapped(uint8_t* base, uint32_t function, uint64_t r3, Body body) {
  if (debug::IsHidden(function)) {
    return r3;
  }
  debug::FrameStateDiff_Before(base);
  FlushOff();
  const uint64_t left = body();
  FlushOff();
  debug::FrameStateDiff_After(base, "draw step part", function);
  return left;
}

// The root's work, with its frame and calls.
class SceneDraw {
 public:
  SceneDraw(uint8_t* base, const SceneDrawCalls& calls, uint32_t f, uint32_t scene)
      : base_(base), calls_(calls), f_(f), scene_(scene) {}

  void SetUp(uint32_t matrix);
  // Each returns the original's r24 at loc_820C7C50 (Car::DrawUpdate's
  // callees get it): the last record's chunk where it was drawn, else its
  // scene model (first path) or its camera pass (second path).
  uint64_t FirstPath();
  uint64_t SecondPath();
  // Part 4b: the pass descriptions' loop (loc_820C7C50) and the end.
  void PassLoop(bool second_path, uint64_t r24);
  void End();
  // The other cars' headlights for model i (the second path with the
  // scene's +5052 0): the original's r24 after (the last model looked at).
  uint64_t OtherHeadlights(uint32_t i, uint32_t lights);
  // The description's keys and mask around Car::DrawUpdate(model).
  void DrawModel(uint32_t i, uint32_t p, uint32_t model, uint64_t r24);
  uint32_t list_at_(uint32_t n) {
    const uint32_t list = scene_ + 5028;
    return uint32_t(Wrapped(base_, kListAt, list, [&] {
      return uint64_t(vehicle_accessors::ListAt(base_, list, n));
    }));
  }

 private:
  // --- Guest calls ---
  uint64_t Call(uint32_t function, std::initializer_list<uint64_t> regs, uint32_t lr,
                std::initializer_list<double> fregs = {}) {
    const uint64_t r3 = calls_.call(function, regs.begin(), int(regs.size()), fregs.begin(),
                                    int(fregs.size()), f_, lr);
    FlushOff();
    return r3;
  }
  // A call through slot `slot` of `object`'s vtable (r3 = object).
  uint32_t VtableCall(uint32_t object, uint32_t slot, uint32_t lr,
                      std::initializer_list<uint64_t> more = {}) {
    uint64_t regs[4] = {object, 0, 0, 0};
    int count = 1;
    for (const uint64_t r : more) {
      regs[count++] = r;
    }
    const uint32_t function = Load(base_, Load(base_, object) + 4 * slot);
    const uint64_t r3 = calls_.call(function, regs, count, nullptr, 0, f_, lr);
    FlushOff();
    return uint32_t(r3);
  }
  // The render manager's getters (slot 8, then `slot` of its answer) with
  // the return addresses x and x + 16: the command list (1) or the render
  // queue (2).
  uint32_t Get(uint32_t slot, uint32_t x) {
    const uint32_t object = VtableCall(Load(base_, kRenderApp), 8, x);
    return VtableCall(object, slot, x + 16);
  }

  // --- The command list (each step's getters at x, x + 16; the call at
  // x + 0x14, or x + 0x18 after AllocateSlots's li r4) ---
  void Open(uint32_t x) {
    const uint32_t list = Get(1, x);
    if (command_list::OpenChunk(base_, list) == command_list::OpenResult::kNested) {
      Call(kOpenChunk, {list}, x + 0x14);
    }
  }
  uint32_t Allocate(uint32_t count, uint32_t x) {
    const uint32_t list = Get(1, x);
    const uint32_t first = command_list::AllocateSlots(base_, list, count);
    if (first != 0) {
      return first;
    }
    // A full list: the original prints its message and returns 0 (the
    // root then writes at 0, as the original does).
    return uint32_t(Call(kAllocateSlots, {list, count}, x + 0x18));
  }
  void Close(uint32_t x) { command_list::CloseChunk(base_, Get(1, x)); }
  // The chunk to the render queue (its slot 4: r4 = the chunk, r5 = the
  // key, f1 = 0.0), the getters at x and x + 16, the call at x + 0x2C.
  void Submit(uint32_t chunk, uint32_t key, uint32_t x) {
    const uint32_t queue = Get(2, x);
    const uint32_t function = Load(base_, Load(base_, queue) + 16);
    Call(function, {queue, chunk, key}, x + 0x2C, {zero_});
  }
  // One slot's three words.
  void Command(uint32_t x, uint32_t w0, uint32_t w1, uint32_t w2) {
    const uint32_t slot = Allocate(1, x);
    Store(base_, slot + 0, w0);
    Store(base_, slot + 4, w1);
    Store(base_, slot + 8, w2);
  }

  // --- Callees ---
  void Memcpy(uint32_t to, uint32_t from, uint32_t bytes, uint32_t lr) {
    if (!native::Buffers_Memcpy(base_, to, from, bytes, lr)) {
      calls_.memcpy(to, from, bytes, f_, lr);
    }
    FlushOff();
  }
  void CopyMatrix(uint32_t to, uint32_t from, uint32_t lr) {
    matrix_math::Copy(base_, to, from);
    interpolation::BlendState_OnCopy(base_, to, from, lr);
    FlushOff();
  }
  void Transform(uint32_t out, uint32_t m, uint32_t v) {
    debug::Vec4Transform(base_, out, m, v);
    FlushOff();
  }
  uint32_t RenderTarget(uint32_t id) {
    return uint32_t(Wrapped(base_, kRenderTarget, id,
                            [&] { return uint64_t(cameras::RenderTarget(base_, id)); }));
  }
  uint64_t XprTexture(uint32_t xpr) {
    return Wrapped(base_, kXprGetTexture, xpr,
                   [&] { return models::XprGetTexture(base_, xpr, 0); });
  }
  // The record's scene model: List::At(scene + 5028, PlayerModel(w9)).
  uint32_t RecordModel(uint32_t index) {
    const uint64_t player = Wrapped(base_, kPlayerModel, index, [&] {
      return uint64_t(CarScenePlayerModel(base_, index));
    });
    const uint32_t list = scene_ + 5028;
    return uint32_t(Wrapped(base_, kListAt, list, [&] {
      return uint64_t(vehicle_accessors::ListAt(base_, list, uint32_t(player)));
    }));
  }
  void SetLight(uint32_t direction, double distance) {
    const uint32_t pass = Load(base_, f_ + 1716) + 5072;
    Wrapped(base_, kSetLight, pass, [&] {
      return uint64_t(ScenePassSetLight(base_, pass, direction, distance, f_, calls_.memcpy));
    });
  }
  // The direction at `at` scaled to length 1 in place (lvx, the normalize,
  // stvx).
  void NormalizeInPlace(uint32_t at) {
    const simde__m128 v = Lvx(base_, at);
    FlushToZero flush;
    Stvx(base_, at, Normalized(v, Dot3(v, v)));
  }
  void Fit(uint32_t out, uint32_t pass, uint32_t direction, double high, double low,
           double far_cut, double min_distance, double distance_scale, uint32_t lr);

  // --- The projected texture chunks ---
  struct ChunkSteps {
    uint32_t open, header, matrix_command, matrix, states, state20, state16, state24, texture,
        sampler;
    uint32_t pixel_command, pixel;  // 0: no pixel constant
    uint32_t end, close, submit;
    uint32_t copy_lr, render_target_lr;
    uint32_t sampler_spill;  // the frame word the sampler slot is kept in (0: none)
  };
  // A header, command 7 vertex c<reg> x4 = the matrix, command 2 with the
  // sampler's states 20, 16 = `state` and 24 = 2, command 4 with the sampler
  // = RenderTarget(`target`), then `pixel(slot)` after command 7 pixel
  // c<pixel_reg> when steps.pixel isn't 0, command 36; closed and handed to
  // the queue with `key`. Returns the chunk (the original's r24 after it).
  template <typename Pixel>
  uint32_t ProjectionChunk(const ChunkSteps& steps, uint32_t reg, uint32_t matrix, uint32_t sampler,
                       uint32_t state, uint32_t target, uint32_t pixel_reg, Pixel pixel,
                       uint32_t key);
  // The pass's size (+504, +508) as floats, 0.0, 0.0 (through `temp` and
  // the frame's +240 and +256, as fcfid's), copied to the slot.
  void SizeConstant(uint32_t slot, uint32_t pass, uint32_t temp);
  // The direction at +304 (0, 1, 0) with w = the height at `height`.
  void HeightConstant(uint32_t slot, uint32_t height);

  // --- The second path's records ---
  void HeadlightProjector(uint32_t record, uint32_t model);
  void SharedTextures(uint32_t record);
  uint32_t Projections(uint32_t record, uint32_t model, uint32_t pass);
  void LightArrays();

  uint8_t* base_;
  const SceneDrawCalls& calls_;
  uint32_t f_;
  uint32_t scene_;
  double one_ = 0;
  double zero_ = 0;
};

void SceneDraw::Fit(uint32_t out, uint32_t pass, uint32_t direction, double high, double low,
                    double far_cut, double min_distance, double distance_scale, uint32_t lr) {
  if (TextureProjectionFitDeclines(base_, pass)) {
    // The original, through its wrap.
    Call(kFit, {out, pass, direction}, lr, {high, low, far_cut, min_distance, distance_scale});
    return;
  }
  Wrapped(base_, kFit, out, [&] {
    uint32_t r3 = 0;
    if (!TextureProjectionFit(base_, out, pass, direction, high, low, far_cut, min_distance,
                              distance_scale, f_, calls_.push_back, &r3)) {
      ITC_LOG_ERROR("car_scene_draw: TextureProjection::Fit declined after its check");
    }
    return uint64_t(r3);
  });
}

template <typename Pixel>
uint32_t SceneDraw::ProjectionChunk(const ChunkSteps& s, uint32_t reg, uint32_t matrix,
                                uint32_t sampler, uint32_t state, uint32_t target,
                                uint32_t pixel_reg, Pixel pixel, uint32_t key) {
  Open(s.open);
  const uint32_t chunk = Allocate(1, s.header);
  guest::StoreU16(base_, chunk + 2, 5);
  Store(base_, chunk + 4, 0);
  Command(s.matrix_command, 7, 4, 0x10004u | (reg << 8));
  CopyMatrix(Allocate(4, s.matrix), matrix, s.copy_lr);
  Command(s.states, 2, 3, 0);
  Command(s.state20, sampler, 20, state);
  Command(s.state16, sampler, 16, state);
  Command(s.state24, sampler, 24, 2);
  Command(s.texture, 4, 1, 0);
  {
    const uint32_t slot = Allocate(1, s.sampler);
    if (s.sampler_spill != 0) {
      Store(base_, f_ + s.sampler_spill, slot);
    }
    Store(base_, slot + 0, sampler);
    Store(base_, slot + 4, RenderTarget(target));
  }
  if (s.pixel != 0) {
    Command(s.pixel_command, 7, 1, 0x01010001u | (pixel_reg << 8));
    pixel(Allocate(1, s.pixel));
  }
  Command(s.end, 36, 0, 0);
  Close(s.close);
  Submit(chunk, key, s.submit);
  return chunk;
}

void SceneDraw::SizeConstant(uint32_t slot, uint32_t pass, uint32_t temp) {
  Stfs(base_, temp + 8, zero_);
  Stfs(base_, temp + 12, zero_);
  const int32_t width = LoadS(base_, pass + 504);
  const int32_t height = LoadS(base_, pass + 508);
  StoreDoubleword(base_, f_ + 240, uint64_t(int64_t(width)));
  StoreDoubleword(base_, f_ + 256, uint64_t(int64_t(height)));
  Stfs(base_, temp + 0, Frsp(double(int64_t(width))));
  Stfs(base_, temp + 4, Frsp(double(int64_t(height))));
  Copy16(base_, slot, temp);
}

void SceneDraw::HeightConstant(uint32_t slot, uint32_t height) {
  Copy16Interleaved(base_, slot, f_ + 304);
  Stfs(base_, slot + 12, Lfs(base_, height));
}

// --- The set-up ---

void SceneDraw::SetUp(uint32_t matrix) {
  // Pass 0's view-projection kept (put back at the end), then with a matrix
  // the matrix times it for the draw.
  for (uint32_t i = 0; i < 64; i += 8) {
    CopyDoubleword(base_, f_ + 1328 + i, kCameraPasses + 144 + i);
  }
  Store(base_, f_ + 88, kCameraPasses);
  one_ = Lfs(base_, kOne);
  if (matrix != 0) {
    Stfs(base_, f_ + 1020, one_);
    Stfs(base_, f_ + 1036, one_);
    Stfs(base_, f_ + 1052, one_);
    Stfs(base_, f_ + 1068, one_);
    matrix_math::Multiply(base_, f_ + 1008, matrix, kCameraPasses + 144);
    interpolation::BlendState_OnMultiply(base_, f_ + 1008, matrix, kCameraPasses + 144,
                                         0x820C5E7C);
    FlushOff();
    CopyMatrix(kCameraPasses + 144, f_ + 1008, 0x820C5E88);
  }
  Store(base_, f_ + 192, 0);
  Store(base_, f_ + 228, 0x8296BB60);
  Store(base_, f_ + 224, 0x8296BAE0);
}

// --- Path +4 = 0 ---

uint64_t SceneDraw::FirstPath() {
  // The shared assets' texture when slot 0 is done.
  uint32_t texture = 0;
  if (LoadS(base_, scene_ + 84) == 3) {
    texture = uint32_t(XprTexture(scene_ + 88));
  }
  Store(base_, kSharedTexture, texture);
  // The camera passes: all 21 disabled, then the view's, the begins and
  // ends recorded.
  for (uint32_t i = 0; i < 21; i++) {
    const uint32_t pass = Pass(base_, i);
    Wrapped(base_, kDisable, pass, [&] {
      cameras::Disable(base_, pass);
      return uint64_t(pass);
    });
  }
  const uint64_t enabled = Wrapped(base_, kEnableForView, kCameraPasses + 20 * kPassSize, [&] {
    return uint64_t(cameras::EnableForView(base_));
  });
  const uint64_t begins = Call(kRecordBegins, {enabled}, 0x820C5F04);
  Call(kRecordEnds, {begins}, 0x820C5F08);

  // The light's direction: 0x8296B950's first vector, normalized, at +128.
  Stfs(base_, f_ + 140, one_);
  Copy16(base_, f_ + 128, kLightDirection);
  Store(base_, f_ + 80, 3);
  const double far_plane = Lfs(base_, 0x82000C34);
  const double size = Lfs(base_, 0x820021F4);
  const double distance = Lfs(base_, 0x820021E4);
  const double light_distance = Lfs(base_, 0x82002FB0);
  NormalizeInPlace(f_ + 128);
  zero_ = Lfs(base_, kZero);

  static constexpr ChunkSteps kSteps = {
      0x820C611C, 0x820C6144, 0x820C617C, 0x820C61BC, 0x820C61F0, 0x820C6228,
      0x820C6260, 0x820C6298, 0x820C62D0, 0x820C6308, 0x820C6348, 0x820C6388,
      0x820C6410, 0x820C6448, 0x820C6470, 0x820C61DC, 0x820C6330, 0};
  uint64_t r24 = 0;
  for (uint32_t r = 0; r < 3; r++) {
    const uint32_t record = kRecords + r * kRecordSize;
    const uint32_t pass = Pass(base_, Load(base_, record + 0));
    const uint32_t model = RecordModel(Load(base_, record + 36));
    r24 = model;
    if (LoadS(base_, pass + 588) != 0 && model != 0) {
      SetLight(f_ + 128, light_distance);
      // The model's position (its matrix's (0, 0, 0, 1)) at +272.
      Stfs(base_, f_ + 272, zero_);
      Stfs(base_, f_ + 276, zero_);
      Stfs(base_, f_ + 280, zero_);
      Stfs(base_, f_ + 284, one_);
      Memcpy(f_ + 528, model + 4304, 64, 0x820C6040);
      Transform(f_ + 272, f_ + 528, f_ + 272);
      // The eye (+592): the position plus the direction times `distance`;
      // up (0, 1, 0) at +496.
      const double dx = Fmuls(Lfs(base_, f_ + 128), distance);
      const double dy = Fmuls(Lfs(base_, f_ + 132), distance);
      const double dz = Fmuls(Lfs(base_, f_ + 136), distance);
      const double px = Lfs(base_, f_ + 272);
      Stfs(base_, f_ + 124, one_);
      Stfs(base_, f_ + 604, one_);
      Stfs(base_, f_ + 700, one_);
      Stfs(base_, f_ + 716, one_);
      Stfs(base_, f_ + 732, one_);
      Stfs(base_, f_ + 748, one_);
      Stfs(base_, f_ + 496, zero_);
      Stfs(base_, f_ + 112, Fadds(dx, px));
      Stfs(base_, f_ + 116, Fadds(dy, Lfs(base_, f_ + 276)));
      Stfs(base_, f_ + 120, Fadds(dz, Lfs(base_, f_ + 280)));
      {
        uint32_t w[4];
        for (int i = 0; i < 4; i++) {
          w[i] = Load(base_, f_ + 112 + 4 * i);
        }
        Stfs(base_, f_ + 500, one_);
        Stfs(base_, f_ + 504, zero_);
        Stfs(base_, f_ + 508, one_);
        for (int i = 0; i < 4; i++) {
          Store(base_, f_ + 592 + 4 * i, w[i]);
        }
      }
      const uint32_t out = f_ + 688, eye = f_ + 592, target = f_ + 272, up = f_ + 496;
      Wrapped(base_, kBuildOrtho, out, [&] {
        ProjectorBuildOrtho(base_, out, eye, target, up, size, size, zero_, far_plane, f_);
        return uint64_t(out);
      });
      const uint32_t light_pass = Pass(base_, Load(base_, record + 16));  // w4
      CopyMatrix(light_pass + 144, out, 0x820C6108);
      r24 = ProjectionChunk(
          kSteps, 14, out, 7, 0, Load(base_, record + 32), 77,
          [&](uint32_t slot) { SizeConstant(slot, light_pass, f_ + 144); },
          Load(base_, record + 56));
    }
    Store(base_, f_ + 80, 2 - r);
  }
  return r24;
}

// --- Path +4 != 0 ---

void SceneDraw::LightArrays() {
  const uint32_t a = f_ + 144, b = f_ + 112;
  auto clear = [&] {
    for (uint32_t i = 0; i < 16; i += 4) {
      Stfs(base_, a + i, zero_);
    }
    for (uint32_t i = 0; i < 16; i += 4) {
      Stfs(base_, b + i, zero_);
    }
  };
  // The headlight projector slots (8 at 0x8296BAE0 and 8 at 0x8296BB60),
  // then the three vectors at 0x8296B980 and 0x8296B9B0, cleared.
  auto fill = [&](uint32_t to_a, uint32_t to_b, uint32_t bytes) {
    for (uint32_t i = 0; i < bytes; i += 16) {
      CopyDoubleword(base_, to_a + i, a);
      CopyDoubleword(base_, to_b + i, b);
      CopyDoubleword(base_, to_a + i + 8, a + 8);
      CopyDoubleword(base_, to_b + i + 8, b + 8);
    }
  };
  clear();
  fill(0x8296BAE0, 0x8296BB60, 128);
  clear();
  fill(0x8296B980, 0x8296B9B0, 48);

  // The scene lights: 0x8296B950 = the direction, 0x8296B980 = ([0x82001BE4]
  // x3, 1.0), 0x8296B9B0 = ([0x8200296C] x3, 1.0), 0x8296B990, 0x8296B9A0,
  // 0x8296B9C0 and 0x8296B9D0 = (0, 0, 0, 1); 0x8265ED00 = the scene's +5280.
  Stfs(base_, f_ + 156, one_);
  Stfs(base_, f_ + 124, one_);
  Stfs(base_, f_ + 128, zero_);
  Stfs(base_, f_ + 132, zero_);
  Stfs(base_, f_ + 136, zero_);
  Stfs(base_, f_ + 140, one_);
  const double ambient = Lfs(base_, 0x82001BE4);
  Stfs(base_, f_ + 144, ambient);
  Stfs(base_, f_ + 148, ambient);
  Stfs(base_, f_ + 152, ambient);
  const double diffuse = Lfs(base_, 0x8200296C);
  {
    uint32_t w[4];
    for (int i = 0; i < 4; i++) {
      w[i] = Load(base_, f_ + 144 + 4 * i);
    }
    Stfs(base_, f_ + 112, diffuse);
    Stfs(base_, f_ + 116, diffuse);
    Stfs(base_, f_ + 120, diffuse);
    for (int i = 0; i < 4; i++) {
      Store(base_, 0x8296B980 + 4 * i, w[i]);
    }
  }
  // The (0, 0, 0, 1)s at +240, +256 and +208 (the light's direction at +176
  // read first).
  uint32_t direction[4], specular[4], position[4];
  for (int i = 0; i < 4; i++) {
    direction[i] = Load(base_, f_ + 176 + 4 * i);
    specular[i] = Load(base_, f_ + 112 + 4 * i);
    position[i] = Load(base_, f_ + 128 + 4 * i);
  }
  for (const uint32_t at : {f_ + 240u, f_ + 256u}) {
    Stfs(base_, at + 0, zero_);
    Stfs(base_, at + 4, zero_);
    Stfs(base_, at + 8, zero_);
    Stfs(base_, at + 12, one_);
  }
  for (int i = 0; i < 4; i++) {
    Store(base_, kLightDirection + 4 * i, direction[i]);
  }
  uint32_t w240[4], w256[4];
  for (int i = 0; i < 4; i++) {
    w240[i] = Load(base_, f_ + 240 + 4 * i);
    w256[i] = Load(base_, f_ + 256 + 4 * i);
  }
  Stfs(base_, f_ + 208, zero_);
  Stfs(base_, f_ + 212, zero_);
  for (int i = 0; i < 4; i++) {
    Store(base_, 0x8296B9B0 + 4 * i, specular[i]);
    Store(base_, 0x8296B990 + 4 * i, position[i]);
    Store(base_, 0x8296B9C0 + 4 * i, w240[i]);
    Store(base_, 0x8296B9A0 + 4 * i, w256[i]);
  }
  Stfs(base_, f_ + 216, zero_);
  Stfs(base_, f_ + 220, one_);
  for (uint32_t i = 0; i < 16; i += 8) {
    CopyDoubleword(base_, 0x8296B9D0 + i, f_ + 208 + i);
  }
  for (uint32_t i = 0; i < 16; i += 8) {
    CopyDoubleword(base_, 0x8265ED00 + i, scene_ + 5280 + i);
  }
}

void SceneDraw::HeadlightProjector(uint32_t record, uint32_t model) {
  const uint32_t beam = f_ + 400;
  Wrapped(base_, kBeam, beam, [&] {
    SceneModelBeam(base_, beam, model);
    return uint64_t(beam);
  });
  const uint32_t scene = Load(base_, f_ + 1716);
  Stfs(base_, f_ + 444, one_);
  Stfs(base_, f_ + 476, one_);
  Copy16(base_, f_ + 480, scene + 5376);
  // The model's matrix at +528; the eye (+432) = the scene's +5376 through
  // it, then its last row cleared: the beam (+464) and up (+384, (0, 1, 0))
  // turned by it.
  Memcpy(f_ + 528, model + 4304, 64, 0x820C690C);
  Stfs(base_, f_ + 384, zero_);
  Stfs(base_, f_ + 388, one_);
  Stfs(base_, f_ + 392, zero_);
  Stfs(base_, f_ + 396, zero_);
  Transform(f_ + 432, f_ + 528, f_ + 480);
  Stfs(base_, f_ + 588, zero_);
  Stfs(base_, f_ + 584, zero_);
  Stfs(base_, f_ + 580, zero_);
  Stfs(base_, f_ + 576, zero_);
  Transform(f_ + 464, f_ + 528, beam);
  Transform(f_ + 384, f_ + 528, f_ + 384);
  // Minus the beam (+128), normalized (+112); the target (+512) = the eye
  // less it.
  for (uint32_t i = 0; i < 16; i += 4) {
    Stfs(base_, f_ + 128 + i, Fneg(Lfs(base_, f_ + 464 + i)));
  }
  const double fov = Lfs(base_, scene + 5436);
  Stfs(base_, f_ + 828, one_);
  Stfs(base_, f_ + 844, one_);
  Stfs(base_, f_ + 860, one_);
  {
    const simde__m128 v = Lvx(base_, f_ + 128);
    Stfs(base_, f_ + 876, one_);
    Stfs(base_, f_ + 524, one_);
    FlushToZero flush;
    Stvx(base_, f_ + 112, Normalized(v, Dot3(v, v)));
  }
  for (uint32_t i = 0; i < 12; i += 4) {
    Stfs(base_, f_ + 512 + i, Fsubs(Lfs(base_, f_ + 432 + i), Lfs(base_, f_ + 112 + i)));
  }
  const uint32_t out = f_ + 816, eye = f_ + 432, target = f_ + 512, up = f_ + 384;
  const double far_plane = Lfs(base_, 0x82002FA8);
  if (PerspectiveDeclines(base_, fov)) {
    Call(kBuild, {out, eye, target, up}, 0x820C6A2C, {fov, one_, one_, far_plane});
  } else {
    Wrapped(base_, kBuild, out, [&] {
      if (!ProjectorBuild(base_, out, eye, target, up, fov, one_, one_, far_plane, f_)) {
        ITC_LOG_ERROR("car_scene_draw: Projector::Build declined after its check");
      }
      return uint64_t(out);
    });
  }
  const uint32_t pass = Pass(base_, Load(base_, record + 4));  // w1
  CopyMatrix(pass + 144, out, 0x820C6A48);
  static constexpr ChunkSteps kSteps = {
      0x820C6A5C, 0x820C6A84, 0x820C6ABC, 0x820C6AFC, 0x820C6B30, 0x820C6B68,
      0x820C6BA4, 0x820C6BE0, 0x820C6C1C, 0x820C6C54, 0x820C6C9C, 0x820C6CDC,
      0x820C6D68, 0x820C6DA0, 0x820C6DC8, 0x820C6B1C, 0x820C6C80, 96};
  ProjectionChunk(
      kSteps, 87, out, 8, 0, Load(base_, record + 20), 77,
      [&](uint32_t slot) { SizeConstant(slot, pass, f_ + 208); }, Load(base_, record + 40));
}

void SceneDraw::SharedTextures(uint32_t record) {
  Open(0x820C6E08);
  const uint32_t chunk = Allocate(1, 0x820C6E30);
  const uint32_t provider = Load(base_, f_ + 1716) + 432;
  Store(base_, f_ + 96, 5);
  guest::StoreU16(base_, chunk + 2, 5);
  Store(base_, f_ + 80, provider);
  Store(base_, chunk + 4, 0);
  Store(base_, f_ + 84, 11);
  // Samplers 11 to 15 (the frame's +84; the count left at +96): states 20,
  // 16 and 24 = 1, and the provider's texture where there is one.
  uint32_t left;
  do {
    const uint32_t sampler = Load(base_, f_ + 84);
    Command(0x820C6E80, 2, 3, 0);
    Command(0x820C6EB8, sampler, 20, 1);
    Command(0x820C6EF4, sampler, 16, 1);
    Command(0x820C6F30, sampler, 24, 1);
    const uint32_t textures = Load(base_, f_ + 80);
    if (textures != 0 && VtableCall(textures, 3, 0x820C6F7C, {0}) != 0) {
      const uint32_t xpr = VtableCall(Load(base_, f_ + 80), 3, 0x820C6F9C, {0});
      if (uint32_t(XprTexture(xpr)) != 0) {
        Command(0x820C6FC0, 4, 1, 0);
        const uint32_t slot = Allocate(1, 0x820C6FF8);
        Store(base_, f_ + 92, slot);
        Store(base_, slot + 0, Load(base_, f_ + 84));
        const uint32_t again = VtableCall(Load(base_, f_ + 80), 3, 0x820C7038, {0});
        Store(base_, Load(base_, f_ + 92) + 4, uint32_t(XprTexture(again)));
      }
    }
    left = Load(base_, f_ + 96) - 1;
    Store(base_, f_ + 96, left);
    Store(base_, f_ + 84, Load(base_, f_ + 84) + 1);
  } while (left != 0);
  Command(0x820C7078, 36, 0, 0);
  Close(0x820C70B0);
  Submit(chunk, Load(base_, record + 44), 0x820C70D8);
}

uint32_t SceneDraw::Projections(uint32_t record, uint32_t model, uint32_t pass) {
  const double scale = Lfs(base_, 0x82002FAC);
  const double band = Lfs(base_, 0x820021F4);
  const double raise = Lfs(base_, 0x82001FD8);
  const double cut1 = Lfs(base_, 0x82002C38);
  const double far1 = Lfs(base_, 0x82002204);
  const double cut2 = Lfs(base_, 0x82002A0C);
  const double least2 = Lfs(base_, 0x82002CAC);
  const double far2 = Lfs(base_, 0x82002A1C);
  const double cut3 = Lfs(base_, 0x82000D44);
  const double least3 = Lfs(base_, 0x82002674);
  const double far3 = Lfs(base_, 0x8200222C);
  // The model's position (its matrix's (0, 0, 0, 1)) at `at`, through its
  // matrix copied to `matrix`.
  auto position = [&](uint32_t at, uint32_t matrix, uint32_t lr) {
    Stfs(base_, at + 0, zero_);
    Stfs(base_, at + 4, zero_);
    Stfs(base_, at + 8, zero_);
    Stfs(base_, at + 12, one_);
    Memcpy(matrix, model + 4304, 64, lr);
    Transform(at, matrix, at);
  };
  auto ones = [&](uint32_t out) {
    for (uint32_t i = 12; i < 64; i += 16) {
      Stfs(base_, out + i, one_);
    }
  };
  // (3) Straight down between the height + [0x82001FD8] and the height -
  // [0x820021F4] into pass w2.
  {
    position(f_ + 352, f_ + 1200, 0x820C7124);
    const double ratio = Fdivs(scale, Lfs(base_, pass + 368));
    const double height = Lfs(base_, f_ + 356);
    ones(f_ + 752);
    const double low = Fsubs(height, band);
    const double high = Fadds(height, raise);
    const double least = Fmuls(ratio, band);
    const double cut = Fmuls(ratio, cut1);
    Fit(f_ + 752, pass, f_ + 304, high, low, cut, least, far1, 0x820C7174);
    CopyMatrix(Pass(base_, Load(base_, record + 8)) + 144, f_ + 752, 0x820C7190);
    static constexpr ChunkSteps kSteps = {
        0x820C71A4, 0x820C71CC, 0x820C7204, 0x820C7244, 0x820C7278, 0x820C72B0,
        0x820C72E8, 0x820C7320, 0x820C7358, 0x820C7390, 0x820C73D4, 0x820C7414,
        0x820C745C, 0x820C7494, 0x820C74BC, 0x820C7264, 0x820C73B8, 92};
    ProjectionChunk(
        kSteps, 70, f_ + 752, 9, 1, Load(base_, record + 24), 68,
        [&](uint32_t slot) { HeightConstant(slot, f_ + 356); }, Load(base_, record + 48));
  }
  // (4) Between the height ± [0x820021F4] into pass w3.
  {
    position(f_ + 320, f_ + 1264, 0x820C7508);
    const double ratio = Fdivs(scale, Lfs(base_, pass + 368));
    ones(f_ + 944);
    const double least = Fmuls(ratio, least2);
    const double cut = Fmuls(ratio, cut2);
    const double height = Lfs(base_, f_ + 324);
    const double low = Fsubs(height, band);
    const double high = Fadds(height, band);
    Fit(f_ + 944, pass, f_ + 304, high, low, cut, least, far2, 0x820C7558);
    CopyMatrix(Pass(base_, Load(base_, record + 12)) + 144, f_ + 944, 0x820C7574);
    static constexpr ChunkSteps kSteps = {
        0x820C7588, 0x820C75B0, 0x820C75E8, 0x820C7628, 0x820C765C, 0x820C7694,
        0x820C76CC, 0x820C7704, 0x820C773C, 0x820C7774, 0x820C77B8, 0x820C77F8,
        0x820C7840, 0x820C7878, 0x820C78A0, 0x820C7648, 0x820C779C, 92};
    ProjectionChunk(
        kSteps, 74, f_ + 944, 10, 1, Load(base_, record + 28), 69,
        [&](uint32_t slot) { HeightConstant(slot, f_ + 324); }, Load(base_, record + 52));
  }
  // (5) Along the light (+176) into pass w4, the description's light set
  // first.
  {
    SetLight(f_ + 176, far1);
    position(f_ + 416, f_ + 1120, 0x820C7900);
    const double ratio = Fdivs(scale, Lfs(base_, pass + 368));
    ones(f_ + 880);
    const double least = Fmuls(ratio, least3);
    const double cut = Fmuls(ratio, cut3);
    const double height = Lfs(base_, f_ + 420);
    const double low = Fsubs(height, band);
    const double high = Fadds(height, band);
    Fit(f_ + 880, pass, f_ + 176, high, low, cut, least, far3, 0x820C7950);
    CopyMatrix(Pass(base_, Load(base_, record + 16)) + 144, f_ + 880, 0x820C796C);
    static constexpr ChunkSteps kSteps = {
        0x820C7980, 0x820C79A8, 0x820C79E0, 0x820C7A20, 0x820C7A54, 0x820C7A8C,
        0x820C7AC4, 0x820C7AFC, 0x820C7B34, 0x820C7B6C, 0,          0,
        0x820C7BAC, 0x820C7BE4, 0x820C7C0C, 0x820C7A40, 0x820C7B94, 0};
    return ProjectionChunk(
        kSteps, 14, f_ + 880, 7, 0, Load(base_, record + 32), 0, [](uint32_t) {},
        Load(base_, record + 56));
  }
}

uint64_t SceneDraw::SecondPath() {
  Store(base_, f_ + 192, 1);
  // [0x8296B8F0]: the shared assets' texture when the scene's +5044 is set,
  // else render target 1.
  uint32_t texture;
  if (LoadS(base_, scene_ + 5044) != 0) {
    texture = uint32_t(XprTexture(scene_ + 88));
  } else {
    texture = RenderTarget(1);
  }
  Stfs(base_, f_ + 180, one_);
  Stfs(base_, f_ + 308, one_);
  Store(base_, kSharedTexture, texture);
  // The light's direction at +176: ([0x82001BE8], 1.0, 0.0, 0.0) normalized,
  // or with the scene's +5052 set 0x8296B950's first vector normalized; the
  // fits' direction (0, 1, 0, 0) at +304.
  Stfs(base_, f_ + 176, Lfs(base_, kLightX));
  zero_ = Lfs(base_, kZero);
  const bool parking = LoadS(base_, scene_ + 5052) != 0;
  Stfs(base_, f_ + 184, zero_);
  Stfs(base_, f_ + 188, zero_);
  Stfs(base_, f_ + 304, zero_);
  Stfs(base_, f_ + 312, zero_);
  Stfs(base_, f_ + 316, zero_);
  NormalizeInPlace(f_ + 176);
  if (parking) {
    Copy16(base_, f_ + 176, kLightDirection);
    NormalizeInPlace(f_ + 176);
  } else {
    LightArrays();
  }

  Store(base_, f_ + 160, 3);
  uint64_t r24 = 0;
  for (uint32_t r = 0; r < 3; r++) {
    const uint32_t record = kRecords + r * kRecordSize;
    const uint32_t pass = Load(base_, f_ + 88) + Load(base_, record + 0) * kPassSize;
    r24 = pass;
    const uint32_t model = RecordModel(Load(base_, record + 36));
    if (LoadS(base_, pass + 588) != 0 && model != 0) {
      HeadlightProjector(record, model);
      SharedTextures(record);
      r24 = Projections(record, model, pass);
    }
    Store(base_, f_ + 160, 2 - r);
  }
  return r24;
}


// --- Both paths: the pass descriptions (loc_820C7C50) ---

constexpr uint32_t kQueueKeyA = 0x8296B90C;  // the render queue's keys
constexpr uint32_t kQueueKeyB = 0x8296B900;
constexpr uint32_t kQueueKeyC = 0x8296B908;
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kViewports = 0x826C2CE8;

void SceneDraw::PassLoop(bool second_path, uint64_t r24) {
  const uint32_t count = Load(base_, scene_ + 5036);
  Store(base_, f_ + 96, count);
  Store(base_, kQueueKeyA, Load(base_, 0x82601854));
  Store(base_, kQueueKeyB, Load(base_, 0x82601858));
  uint32_t descriptions = second_path ? 4 : 1;  // r26
  // The player's model p (CarScene::PlayerModel(0) inlined).
  uint32_t p = 0;
  Store(base_, f_ + 160, p);
  {
    const uint32_t race = Load(base_, kRaceData);
    const int32_t kind = LoadS(base_, race + 0xA3AF0);
    const uint32_t word = Load(base_, race + 0x59614);
    if (kind == 6 && word == 2) {
      p = Load(base_, 0x826C7820 + 4) == 0 ? 1 : 0;
      Store(base_, f_ + 160, p);
    }
  }
  // The frame's words the original keeps its addresses in (left there).
  uint32_t offset = 0;  // r14
  Store(base_, f_ + 80, descriptions);
  Store(base_, f_ + 84, offset);
  Store(base_, f_ + 240, 0x8296BA60);
  Store(base_, f_ + 256, 0x8296BC60);
  Store(base_, f_ + 288, 0x8296B9E0);
  Store(base_, f_ + 336, 0x8296BBE0);
  Store(base_, f_ + 340, 0x8265ECA0);
  Store(base_, f_ + 344, 0x8265ECE0);
  Store(base_, f_ + 368, 0x82002EF8);
  Store(base_, f_ + 348, 0x82002F38);
  Store(base_, f_ + 92, 0x82002DF8);
  const uint32_t description = scene_ + 5072;  // r20
  const uint32_t lights = scene_ + 5248;  // r22

  // The description's camera pass, or 0 when it isn't active (the
  // original's add. and +588 checks, made again at each use).
  auto active_pass = [&]() -> uint32_t {
    const uint32_t pass = Load(base_, description + 0) * kPassSize + Load(base_, f_ + 88);
    if (pass == 0 || LoadS(base_, pass + 588) == 0) {
      return 0;
    }
    return pass;
  };
  auto list_at = [&](uint32_t n) { return list_at_(n); };
  // Viewport v's view mode is 2 (probably the cockpit).
  auto cockpit_view = [&](uint32_t v) {
    const uint32_t viewport = Load(base_, Load(base_, kViewports) + 4 * v);
    return viewport != 0 && LoadS(base_, viewport + 1860) == 2;
  };
  // List::At(n)'s +4296 = value when it exists (two List::At calls, as the
  // original).
  auto show = [&](uint32_t n, uint32_t value) {
    if (list_at(n) != 0) {
      Store(base_, list_at(n) + 4296, value);
    }
  };

  do {
    uint32_t table;
    if (second_path) {
      table = Load(base_, f_ + 92);
    } else {
      table = LoadS(base_, 0x826032DC) == 8 ? Load(base_, f_ + 348) : Load(base_, f_ + 368);
    }
    Memcpy(description, offset + table, 64, 0x820C7D9C);
    if (active_pass() != 0) {
      // The pass's eye to 0x8265ECE0 and its view to 0x8265ECA0.
      Copy16(base_, Load(base_, f_ + 344), active_pass() + 400);
      CopyMatrix(Load(base_, f_ + 340), active_pass() + 16, 0x820C7E28);
      if (second_path) {
        // The player's and model 1's +4296 by the pass's type.
        const uint32_t type = Load(base_, active_pass() + 584);
        if (type == 0) {
          const uint32_t stage = f_ + 1120;
          const uint32_t stages = Load(base_, kRaceData) + 0x59620;
          if (!debug::IsHidden(kStagesCopy)) {
            debug::FrameStateDiff_Before(base_);
            FlushOff();
            const bool copied = vehicle_accessors::StagesCopy(base_, stage, stages, 0xFFFFFFFF);
            FlushOff();
            debug::FrameStateDiff_After(base_, "draw step part", kStagesCopy);
            if (!copied) {
              // The original through its adapter (never in play: the copy
              // goes to the frame).
              Call(kStagesCopy, {stage, stages, 0xFFFFFFFFFFFFFFFFull}, 0x820C7F54);
            }
          }
          const uint32_t race = Load(base_, kRaceData);
          if ((LoadS(base_, race + 0xA3AF0) != 6 || LoadS(base_, stage) == 1) &&
              (Load(base_, race + 0x59AE4) & 0x80) == 0 && list_at(p) != 0) {
            const uint32_t value = cockpit_view(0) ? 0 : 1;
            Store(base_, list_at(p) + 4296, value);
          }
        } else if (type == 1) {
          show(1, 1);
          if (cockpit_view(0)) {
            show(p, 0);
          }
        } else if (type == 2) {
          show(p, 1);
          if (cockpit_view(1)) {
            show(1, 0);
          }
        } else if (type == 3) {
          show(p, 0);
        }
      }
      const int32_t models = LoadS(base_, f_ + 96);  // r28
      for (uint32_t i = 0; int32_t(i) < models; i++) {
        const uint32_t model = list_at(i);  // r18
        if (LoadS(base_, model + 16) != 3 || Load(base_, model + 5048) == 0) {
          continue;
        }
        if (second_path && LoadS(base_, scene_ + 5052) == 0) {
          r24 = OtherHeadlights(i, lights);
        }
        DrawModel(i, p, model, r24);
      }
    }
    descriptions--;
    offset += 64;
    Store(base_, f_ + 80, descriptions);
    Store(base_, f_ + 84, offset);
  } while (descriptions != 0);
}

uint64_t SceneDraw::OtherHeadlights(uint32_t i, uint32_t lights) {
  // Slots 0 to 7 of 0x8296BAE0 and 0x8296BB60 cleared (from the frame's
  // +208 and +144).
  for (const uint32_t at : {f_ + 208u, f_ + 212u, f_ + 216u, f_ + 220u, f_ + 144u, f_ + 148u,
                            f_ + 152u, f_ + 156u}) {
    Stfs(base_, at, zero_);
  }
  for (uint32_t k = 0; k < 128; k += 16) {
    const uint32_t a = Load(base_, f_ + 224) + k, b = Load(base_, f_ + 228) + k;
    CopyDoubleword(base_, a, f_ + 208);
    CopyDoubleword(base_, b, f_ + 144);
    CopyDoubleword(base_, a + 8, f_ + 216);
    CopyDoubleword(base_, b + 8, f_ + 152);
  }
  Stfs(base_, f_ + 620, one_);
  Stfs(base_, f_ + 460, one_);
  Copy16(base_, f_ + 672, lights + 128);  // the scene's +5376
  uint32_t slot = 2;  // r23 (the root's r25)
  uint32_t j = 0;     // r24
  const int32_t models = LoadS(base_, f_ + 96);  // r28 (at least 1 here)
  for (; int32_t(j) < models; j++) {
    const uint32_t other = list_at_(j);
    const uint32_t beam = f_ + 624;
    Wrapped(base_, kBeam, beam, [&] {
      SceneModelBeam(base_, beam, other);
      return uint64_t(beam);
    });
    if (j == i || other == 0 || LoadS(base_, other + 16) != 3 ||
        Load(base_, other + 5048) == 0 || LoadS(base_, other + 32) == 3) {
      continue;
    }
    const uint32_t at = slot << 4;
    if (int32_t(slot++) >= 8) {
      return j;
    }
    // Its position (the scene's +5376 through its matrix) at +608, its beam
    // through it at +448, the beam's minus (+128) normalized at +480.
    Memcpy(f_ + 528, other + 4304, 64, 0x820C8134);
    Transform(f_ + 608, f_ + 528, f_ + 672);
    Stfs(base_, f_ + 588, zero_);
    Stfs(base_, f_ + 584, zero_);
    Stfs(base_, f_ + 580, zero_);
    Stfs(base_, f_ + 576, zero_);
    Transform(f_ + 448, f_ + 528, beam);
    for (uint32_t k = 0; k < 16; k += 4) {
      Stfs(base_, f_ + 128 + k, Fneg(Lfs(base_, f_ + 448 + k)));
    }
    Copy16(base_, f_ + 656, lights + 192);  // the scene's +5440
    {
      const simde__m128 v = Lvx(base_, f_ + 128);
      FlushToZero flush;
      Stvx(base_, f_ + 480, Normalized(v, Dot3(v, v)));
    }
    // BeamShape's vector at +640.
    const uint32_t shape = uint32_t(Wrapped(base_, kBeamShape, f_ + 1088, [&] {
      SceneModelBeamShape(base_, f_ + 1088, other);
      return uint64_t(f_ + 1088);
    }));
    Copy16Interleaved(base_, f_ + 640, shape);
    // The two colors times the scene's +5248 to +5256 (+112) and +5264 to
    // +5272 (+400).
    const uint32_t color1 = uint32_t(Wrapped(base_, kBeamColor, f_ + 1072, [&] {
      return uint64_t(SceneModelBeamColor(base_, f_ + 1072, other, f_));
    }));
    {
      const double s0 = Lfs(base_, lights + 0);
      const double s1 = Lfs(base_, lights + 4);
      const double s2 = Lfs(base_, lights + 8);
      Stfs(base_, f_ + 124, one_);
      Stfs(base_, f_ + 112, Fmuls(s0, Lfs(base_, color1 + 0)));
      const double c1 = Lfs(base_, color1 + 4);
      const double c2 = Lfs(base_, color1 + 8);
      const double y = Fmuls(c1, s1);
      const double z = Fmuls(c2, s2);
      Stfs(base_, f_ + 116, y);
      Stfs(base_, f_ + 120, z);
    }
    const uint32_t color2 = uint32_t(Wrapped(base_, kBeamColor, f_ + 1104, [&] {
      return uint64_t(SceneModelBeamColor(base_, f_ + 1104, other, f_));
    }));
    {
      Stfs(base_, f_ + 412, one_);
      const double c0 = Lfs(base_, color2 + 0);
      Stfs(base_, f_ + 400, Fmuls(c0, Lfs(base_, lights + 16)));
      const double c1 = Lfs(base_, color2 + 4);
      const double s1 = Lfs(base_, lights + 20);
      const double c2 = Lfs(base_, color2 + 8);
      Stfs(base_, f_ + 404, Fmuls(c1, s1));
      Stfs(base_, f_ + 408, Fmuls(c2, Lfs(base_, lights + 24)));
    }
    // Slot `slot` of the six arrays: 0x8296BBE0 the direction, 0x8296B9E0
    // the position, 0x8296BAE0 the first color, 0x8296BB60 the second,
    // 0x8296BC60 the scene's +5440, 0x8296BA60 the shape.
    Copy16(base_, at + Load(base_, f_ + 336), f_ + 480);
    Copy16(base_, at + Load(base_, f_ + 288), f_ + 608);
    Copy16(base_, at + Load(base_, f_ + 224), f_ + 112);
    Copy16(base_, at + Load(base_, f_ + 228), f_ + 400);
    Copy16(base_, at + Load(base_, f_ + 256), f_ + 656);
    Copy16(base_, at + Load(base_, f_ + 240), f_ + 640);
  }
  return j;
}

void SceneDraw::DrawModel(uint32_t i, uint32_t p, uint32_t model, uint64_t r24) {
  const uint32_t description = scene_ + 5072;
  // The render queue's keys: the description's +64 to +72 keep them, they
  // take its +20 to +28.
  const uint32_t key_b = Load(base_, description + 24);
  const uint32_t key_a = Load(base_, description + 20);
  Store(base_, description + 64, Load(base_, kQueueKeyA));
  Store(base_, description + 68, Load(base_, kQueueKeyB));
  const uint32_t key_c = Load(base_, kQueueKeyC);
  Store(base_, kQueueKeyB, key_b);
  Store(base_, kQueueKeyA, key_a);
  Store(base_, description + 72, key_c);
  Store(base_, kQueueKeyC, Load(base_, description + 28));
  // The mask (+5132, the description's +60) for the call, by the pass's
  // type (pass 0's address, 584, read when the pass isn't active, as the
  // original).
  uint32_t pass = Load(base_, description + 0) * kPassSize + Load(base_, f_ + 88);
  const uint32_t saved = Load(base_, scene_ + 5132);
  uint32_t mask = saved;
  if (pass == 0 || LoadS(base_, pass + 588) == 0) {
    pass = 0;
  }
  const uint32_t type = Load(base_, pass + 584);
  if (type == 0) {
    if (p == i) {
      mask &= ~0x4u;
    }
    if (LoadS(base_, scene_ + 5052) != 0) {
      mask &= ~0x8u;
      mask &= ~0xE0u;
    }
  } else if (type == 1) {
    if (p == i) {
      mask &= ~0x4u;
    }
  } else if (type == 2) {
    if (i == 1) {
      mask &= ~0x4u;
    }
  }
  Store(base_, scene_ + 5132, mask);
  Wrapped(base_, kCarDrawUpdate, model, [&] {
    calls_.car_draw_update(model, r24, f_, 0x820C840C);
    return uint64_t(0);
  });
  const uint32_t key = Load(base_, scene_ + 5144);
  Store(base_, scene_ + 5132, saved);
  Store(base_, kQueueKeyC, key);
  const uint32_t b = Load(base_, description + 68);
  const uint32_t a = Load(base_, description + 64);
  Store(base_, kQueueKeyB, b);
  Store(base_, kQueueKeyA, a);
}

void SceneDraw::End() {
  CopyMatrix(Load(base_, f_ + 88) + 144, f_ + 1328, 0x820C8460);
}

}  // namespace

uint32_t CarSceneDraw(uint8_t* base, const SceneDrawCalls& calls, uint32_t stack, uint32_t scene,
                      uint32_t matrix) {
  ITC_NATIVE_RUNNING("sub_820C5DE8 CarScene::Draw");
  const uint32_t f = stack - kSceneDrawFrame;
  Store(base, f, stack);  // the back chain, as stwu
  const int32_t state = LoadS(base, scene + 0);
  Store(base, f + 1716, scene);
  if (state == 4) {
    return scene;  // r3 as it came
  }
  SceneDraw draw(base, calls, f, scene);
  draw.SetUp(matrix);
  SceneDrawFrame frame;
  frame.f = f;
  frame.scene = scene;
  frame.second_path = LoadS(base, scene + 4) != 0;
  const uint64_t r24 = frame.second_path ? draw.SecondPath() : draw.FirstPath();
  // Both paths: the pass descriptions, then pass 0's view-projection put
  // back.
  draw.PassLoop(frame.second_path, r24);
  draw.End();
  FlushOff();
  // r3 as Matrix::Copy leaves it: its destination, pass 0's view-projection.
  return Load(base, f + 88) + 144;
}

}  // namespace car_scene_draw
