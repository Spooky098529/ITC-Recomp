// The post effects' records natively (post_effects.h,
// docs/functions/post-effects.md): ColorFilter::Record,
// TargetFilter::Record, MotionBlur::Record and the glow's star streaks
// (Glow::RecordStar and RecordStarPass; their math, the vector unit's
// steps included, as the translated code does it: this file builds with
// -ffp-model=strict and -ffp-contract=off). Step for step as the recompiled functions: their
// frames where the originals' are (with the words they write there), the
// render manager's getters, CameraPasses::RenderTarget (its diagnostics'
// wrap) and the render queue as guest calls with the originals' return
// addresses, the command list and the recording helpers as plain C++. The
// whole record is declined (nothing done but the getters) when the list
// hasn't room for all its slots or a chunk is open, where the original
// prints its messages: left to it.

#include "post_effects/post_effects.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "car_scene_draw/ops.h"
#include "command_list/command_list.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "models/model.h"
#include "native/buffers.h"
#include "scene_target/scene_target.h"

#include <bit>
#include <initializer_list>

namespace post_effects {

namespace {

constexpr uint32_t kRenderApp = 0x826BABA0;  // the render manager
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kRenderTarget = 0x8209F7A8;  // CameraPasses::RenderTarget (wrapped)
constexpr uint32_t kOne = 0x8200882C;           // 1.0
constexpr uint32_t kZero = 0x82000A34;          // 0.0

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
// std of a zero-extended word (its high word 0).
void StoreDoubleword(uint8_t* base, uint32_t address, uint32_t low) {
  Store(base, address, 0);
  Store(base, address + 4, low);
}

using Fpscr = rex::platform::FPSCRPlatform;

// tan as the original's call gives it: nearest rounding (its _ctrlfp), or
// false where c_runtime::Tan can't stand in.
bool Tangent(double x, double* result) {
  return (Fpscr::getcsr() & Fpscr::RoundMaskVal) == 0 && c_runtime::Tan(x, result);
}

// A record's guest calls and its list, from its frame.
class Recorder {
 public:
  Recorder(uint8_t* base, const RecordCalls& calls, uint32_t frame)
      : base_(base), calls_(calls), frame_(frame) {}

  uint64_t Call(uint32_t function, std::initializer_list<uint64_t> regs, uint32_t lr,
                std::initializer_list<double> fregs = {}) {
    return calls_.call(function, regs.begin(), int(regs.size()), fregs.begin(),
                       int(fregs.size()), frame_, lr);
  }
  uint32_t VtableCall(uint32_t object, uint32_t byte, uint32_t lr,
                      std::initializer_list<uint64_t> more = {}, double f1 = 0.0,
                      bool with_f1 = false) {
    uint64_t regs[4] = {object, 0, 0, 0};
    int count = 1;
    for (const uint64_t r : more) {
      regs[count++] = r;
    }
    const uint32_t function = Load(base_, Load(base_, object) + byte);
    return uint32_t(calls_.call(function, regs, count, &f1, with_f1 ? 1 : 0, frame_, lr));
  }
  // The render manager's slot 8, then `byte` of its answer: the command
  // list (4) or the render queue (8).
  uint32_t Get(uint32_t byte, uint32_t lr1, uint32_t lr2) {
    const uint32_t object = VtableCall(Load(base_, kRenderApp), 32, lr1);
    return VtableCall(object, byte, lr2);
  }
  // Whether the record may go on natively: no chunk open, room for `slots`.
  bool Ready(uint32_t list, uint32_t slots) const {
    const uint32_t used = Load(base_, list + 12);
    return guest::LoadU8(base_, list + 52) == 0 &&
           Load(base_, command_list::kOpenChunks) == 0 && used + slots >= used &&
           used + slots < Load(base_, list + 8);
  }
  // Whether the list has room for `slots` (a record inside a chunk already
  // open: only the room).
  bool Room(uint32_t list, uint64_t slots) const {
    const uint64_t used = Load(base_, list + 12);
    return used + slots < Load(base_, list + 8);
  }
  // The chunk handed to the render queue (its slot 4: r4 = the chunk, r5 =
  // the key, f1); its r3.
  uint32_t Submit(uint32_t chunk, uint64_t key, double f1, uint32_t lr1, uint32_t lr2,
                  uint32_t lr3) {
    const uint32_t queue = Get(8, lr1, lr2);
    return VtableCall(queue, 16, lr3, {chunk, key}, f1, true);
  }

  uint8_t* base_;
  const RecordCalls& calls_;
  uint32_t frame_;
};

// --- The list's steps (Ready checked: none fails) ---

uint32_t Slot(uint8_t* base, uint32_t list) {
  return command_list::AllocateSlots(base, list, 1);
}
// A slot with [a, b, c] (+12 left as it was).
uint32_t Slot3(uint8_t* base, uint32_t list, uint32_t a, uint32_t b, uint32_t c) {
  const uint32_t s = Slot(base, list);
  Store(base, s + 0, a);
  Store(base, s + 4, b);
  Store(base, s + 8, c);
  return s;
}
// A slot with [a, b] (+8, +12 left as they were).
uint32_t Slot2(uint8_t* base, uint32_t list, uint32_t a, uint32_t b) {
  const uint32_t s = Slot(base, list);
  Store(base, s + 0, a);
  Store(base, s + 4, b);
  return s;
}
// The chunk's header: [+2] = 5 (a halfword), [+4] = 0.
uint32_t Header(uint8_t* base, uint32_t list) {
  const uint32_t s = Slot(base, list);
  guest::StoreU16(base, s + 2, 5);
  Store(base, s + 4, 0);
  return s;
}
// The vertex declaration: command 10, then its slot.
void Declaration(uint8_t* base, uint32_t list) {
  Slot3(base, list, 10, 1, 0);
  Store(base, Slot(base, list), Load(base, kVertexDeclaration));
}
// Sampler 0's texture: command 4, then [0, texture].
void Texture(uint8_t* base, uint32_t list, uint32_t texture) {
  Slot3(base, list, 4, 1, 0);
  Slot2(base, list, 0, texture);
}
// Command 2 with sampler 0's three states (20 = 0, 16 = 0, 24 = 2).
void SamplerStates(uint8_t* base, uint32_t list) {
  Slot3(base, list, 2, 3, 0);
  Slot3(base, list, 0, 20, 0);
  Slot3(base, list, 0, 16, 0);
  Slot3(base, list, 0, 24, 2);
}
// The shared shader pair `pair`: commands 8 and 9, each with its slot.
void ShaderPair(uint8_t* base, uint32_t list, uint32_t pair) {
  Slot3(base, list, 8, 1, 0);
  Store(base, Slot(base, list), models::SharedVertexShader(base, pair));
  Slot3(base, list, 9, 1, 0);
  Store(base, Slot(base, list), models::SharedPixelShader(base, pair));
}
// Command 19, then its slot (a quad: [8, 3, 40] as a word and two
// halfwords) with PostEffect::AddQuad's vertices in +8.
void Quad(uint8_t* base, uint32_t list, uint32_t effect, double value, bool at_view) {
  Slot3(base, list, 19, 1, 0);
  const uint32_t s = Slot(base, list);
  Store(base, s + 0, 8);
  guest::StoreU16(base, s + 4, 3);
  guest::StoreU16(base, s + 6, 40);
  Store(base, s + 8,
        AddQuad(base, effect, Load(base, effect + 16), Load(base, effect + 20), value, at_view));
}
// Command 19 and a quad at a quarter of the effect's size (+16 >> 2, +20
// >> 2), not at the view.
void QuarterQuad(uint8_t* base, uint32_t list, uint32_t effect, double value) {
  Slot3(base, list, 19, 1, 0);
  const uint32_t s = Slot(base, list);
  Store(base, s + 0, 8);
  guest::StoreU16(base, s + 4, 3);
  guest::StoreU16(base, s + 6, 40);
  Store(base, s + 8,
        AddQuad(base, effect, Load(base, effect + 16) >> 2, Load(base, effect + 20) >> 2, value,
                false));
}
// The texel size from the effect's size, its words as the original's
// frame keeps them (width at `width_at`, height at `height_at`).
void TexelSize(uint8_t* base, uint32_t list, uint32_t effect, uint32_t width_at,
               uint32_t height_at) {
  const uint32_t height = Load(base, effect + 20);
  const uint32_t width = Load(base, effect + 16);
  StoreDoubleword(base, height_at, height);
  StoreDoubleword(base, width_at, width);
  RecordTexelSize(base, list, double(float(double(int64_t(width)))),
                  double(float(double(int64_t(height)))));
}

}  // namespace

std::optional<uint32_t> ColorFilterRecord(uint8_t* base, uint32_t stack, uint32_t filter,
                                          const RecordCalls& calls) {
  const uint32_t frame = stack - 224;
  Store(base, frame, stack);  // the back chain, as stwu
  Recorder r(base, calls, frame);
  const uint32_t list = r.Get(4, 0x82135220, 0x82135230);
  if (!r.Ready(list, 75)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_821351F0 ColorFilter::Record");
  command_list::OpenChunk(base, list);
  const uint32_t header = Header(base, list);
  RecordResolve(base, filter, list, Load(base, Load(base, filter + 8064) + 24), 0, 0, true);
  Declaration(base, list);
  RecordBeginStates(base, list);
  Texture(base, list, Load(base, Load(base, filter + 8064) + 24));
  SamplerStates(base, list);
  // The strength (+8112) clamped to the two constants (a NaN stays), times
  // tan(the constant at 0x82002CD8).
  double strength = Lfs(base, filter + 8112);
  const double high = Lfs(base, 0x82002A30);
  const double low = Lfs(base, 0x82008898);
  if (strength > high) {
    strength = high;
  } else if (strength < low) {
    strength = low;
  }
  const double angle = std::bit_cast<double>(uint64_t(Load(base, 0x82002CD8)) << 32 |
                                             Load(base, 0x82002CDC));
  double tangent;
  if (!Tangent(angle, &tangent)) {
    tangent = calls.tan(angle, frame, 0x821353B8);
  }
  const double scale = double(float(tangent));
  const uint32_t c = frame + 96;
  TexelSize(base, list, filter, c, frame + 80);
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  // c37: the inverses of +8080 to +8088.
  Stfs(base, c + 0, double(float(one / Lfs(base, filter + 8080))));
  Stfs(base, c + 4, double(float(one / Lfs(base, filter + 8084))));
  Stfs(base, c + 8, double(float(one / Lfs(base, filter + 8088))));
  Stfs(base, c + 12, zero);
  RecordPixelConstants(base, list, c, 37, 1);
  // c38: +8120, (strength + 1) times the tangent, +8116, 0.
  const double sum = double(float(strength + one));
  Stfs(base, c + 0, Lfs(base, filter + 8120));
  Stfs(base, c + 8, Lfs(base, filter + 8116));
  Stfs(base, c + 12, zero);
  Stfs(base, c + 4, double(float(sum * scale)));
  RecordPixelConstants(base, list, c, 38, 1);
  // c39: +8096 to +8104, 0.
  Stfs(base, c + 0, Lfs(base, filter + 8096));
  Stfs(base, c + 4, Lfs(base, filter + 8100));
  Stfs(base, c + 8, Lfs(base, filter + 8104));
  Stfs(base, c + 12, zero);
  RecordPixelConstants(base, list, c, 39, 1);
  RecordPixelBool(base, list, Load(base, filter + 8068), 0);
  RecordPixelBool(base, list, Load(base, filter + 8072), 1);
  Slot3(base, list, 1, 1, 0);
  Slot2(base, list, 60, 0);
  Slot3(base, list, 3, 1, 0);
  Slot2(base, list, 0, 0x10001);
  ShaderPair(base, list, 31);
  Quad(base, list, filter, one, true);
  RecordEndStates(base, list);
  Slot3(base, list, 36, 0, 0);
  command_list::CloseChunk(base, list);
  const uint32_t r3 =
      r.Submit(header, Load(base, filter + 8), zero, 0x82135670, 0x82135680, 0x8213569C);
  Store(base, filter + 40, 0);
  return r3;
}

std::optional<uint32_t> TargetFilterRecord(uint8_t* base, uint32_t stack, uint32_t filter,
                                           const RecordCalls& calls) {
  const uint32_t frame = stack - 224;
  Store(base, frame, stack);  // the back chain, as stwu
  const uint32_t flags = Load(base, filter + 32);
  uint32_t id;
  if ((flags & 1) != 0) {
    if (Load(base, kCameraPasses + 12 * 592 + 588) == 0) {
      return filter;
    }
    id = 4;
  } else if ((flags & 2) != 0) {
    if (Load(base, kCameraPasses + 11 * 592 + 588) == 0) {
      return filter;
    }
    id = 3;
  } else {
    return filter;
  }
  Recorder r(base, calls, frame);
  const uint32_t target = uint32_t(r.Call(kRenderTarget, {id}, 0x82135BB0));
  if (target == 0) {
    return 0;
  }
  const uint32_t list = r.Get(4, 0x82135BD4, 0x82135BE4);
  if (!r.Ready(list, 91)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82135B50 TargetFilter::Record");
  const bool second = (flags & 2) != 0;
  command_list::OpenChunk(base, list);
  const uint32_t header = Header(base, list);
  RecordBeginStates(base, list);
  Declaration(base, list);
  SamplerStates(base, list);
  const double one = Lfs(base, kOne);
  // Drawn into +8064 and resolved into the target, twice (pairs 28 and 29
  // for bit 1, 24 and 25 for bit 0).
  for (uint32_t pass = 0; pass < 2; pass++) {
    RecordTargets(base, list, Load(base, filter + 8064), 0);
    if (pass == 0) {
      TexelSize(base, list, filter, frame + 88, frame + 80);
    } else {
      TexelSize(base, list, filter, frame + 80, frame + 88);
    }
    Texture(base, list, target);
    ShaderPair(base, list, (second ? 28 : 24) + pass);
    Quad(base, list, filter, one, false);
    RecordResolve(base, filter, list, target, 0, 0, false);
  }
  RecordEndStates(base, list);
  Slot3(base, list, 36, 0, 0);
  command_list::CloseChunk(base, list);
  const uint32_t r3 =
      r.Submit(header, Load(base, filter + 8), Lfs(base, kZero), 0x821360EC, 0x821360FC,
               0x8213611C);
  Store(base, filter + 40, 0);
  return r3;
}

std::optional<uint32_t> MotionBlurRecord(uint8_t* base, uint32_t stack, uint32_t blur,
                                         const RecordCalls& calls) {
  const uint32_t frame = stack - 288;
  Store(base, frame, stack);  // the back chain, as stwu
  Recorder r(base, calls, frame);
  // The list before every step, as the original fetches it.
  uint32_t list = r.Get(4, 0x82136420, 0x82136430);
  if (!r.Ready(list, 131)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_821363F0 MotionBlur::Record");
  // --- The upload chunk: vertex c100 x4, the previous view-projection ---
  const uint32_t upload = Slot(base, list);  // its header, before the chunk opens
  list = r.Get(4, 0x82136454, 0x82136464);
  command_list::OpenChunk(base, list);
  list = r.Get(4, 0x8213647C, 0x8213648C);
  Header(base, list);
  list = r.Get(4, 0x821364B8, 0x821364C8);
  Slot3(base, list, 7, 4, 0x16404);
  list = r.Get(4, 0x82136500, 0x82136510);
  const uint32_t matrix = command_list::AllocateSlots(base, list, 4);
  // This tick's view-projection on a logic pass (+48 set) with history
  // (+44 clear), else the previous tick's (frame interpolation notes the
  // copy).
  const bool logic = Load(base, blur + 48) != 0 && Load(base, blur + 44) == 0;
  interpolation::BlendState_MotionBlurCopy(base, blur, matrix, blur + (logic ? 8080 : 8144));
  list = r.Get(4, 0x82136550, 0x82136560);
  Slot3(base, list, 36, 0, 0);
  list = r.Get(4, 0x8213658C, 0x8213659C);
  command_list::CloseChunk(base, list);
  // On a logic pass with its camera pass active: the pair moved on (+8144 =
  // +8080, +8080 = the pass's view-projection at +144).
  if (logic) {
    const uint32_t pass = kCameraPasses + Load(base, blur + 36) * 592;
    if (Load(base, pass + 588) != 0) {
      matrix_math::Copy(base, blur + 8144, blur + 8080);
      interpolation::BlendState_OnCopy(base, blur + 8144, blur + 8080, 0x821365E8);
      matrix_math::Copy(base, blur + 8080, pass + 144);
      interpolation::BlendState_OnCopy(base, blur + 8080, pass + 144, 0x821365F4);
    }
  }
  // --- The blur's chunk ---
  list = r.Get(4, 0x82136608, 0x82136618);
  command_list::OpenChunk(base, list);
  const uint32_t draw = Header(base, list);
  // The velocities drawn into +8072 (with +8076's depth): shader pair 32
  // and 280 bytes of constants.
  RecordTargets(base, list, Load(base, blur + 8072), Load(base, blur + 8076));
  Slot3(base, list, 1, 3, 0);
  Slot2(base, list, 40, 1);
  Slot2(base, list, 48, 1);
  Slot2(base, list, 44, 6);
  Slot3(base, list, 26, 1, 0);
  const uint32_t bytes = command_list::AllocateBytes(base, list, 280, false);
  const double zero = Lfs(base, kZero);
  Stfs(base, bytes + 272, zero);
  Store(base, bytes + 276, 0);
  Store(base, bytes + 268, 0x80808000);
  Store(base, bytes + 4, 0);
  Store(base, bytes + 8, 49);
  Store(base, bytes + 0, 0);
  ShaderPair(base, list, 32);
  Slot3(base, list, 36, 0, 0);
  // Resolved into +8064's texture; the frame into +8068's.
  const uint32_t velocities = Header(base, list);
  RecordResolve(base, blur, list, Load(base, Load(base, blur + 8064) + 24), 0, 0, false);
  Slot3(base, list, 36, 0, 0);
  const uint32_t scene = Header(base, list);
  RecordResolve(base, blur, list, Load(base, Load(base, blur + 8068) + 24), 0, 0xFFFFFFFDu,
                true);
  // The blur over the view: shader pair 33 with the two textures.
  Declaration(base, list);
  RecordBeginStates(base, list);
  Slot3(base, list, 1, 5, 0);
  Slot2(base, list, 60, 1);
  Slot2(base, list, 308, 1);
  Slot2(base, list, 312, 1);
  Slot2(base, list, 316, 1);
  Slot2(base, list, 320, 1);
  Slot3(base, list, 4, 2, 0);
  Slot2(base, list, 0, Load(base, Load(base, blur + 8068) + 24));
  Slot2(base, list, 1, Load(base, Load(base, blur + 8064) + 24));
  Slot3(base, list, 2, 8, 0);
  Slot3(base, list, 0, 0, 2);
  Slot3(base, list, 0, 4, 2);
  Slot3(base, list, 0, 20, 1);
  Slot3(base, list, 0, 16, 1);
  Slot3(base, list, 0, 24, 2);
  Slot3(base, list, 1, 20, 1);
  Slot3(base, list, 1, 16, 1);
  Slot3(base, list, 1, 24, 2);
  // c40: +8208 to +8216 and the constant at 0x82008828.
  const uint32_t c = frame + 80;
  Stfs(base, c + 12, Lfs(base, 0x82008828));
  Stfs(base, c + 0, Lfs(base, blur + 8208));
  Stfs(base, c + 4, Lfs(base, blur + 8212));
  Stfs(base, c + 8, Lfs(base, blur + 8216));
  RecordPixelConstants(base, list, c, 40, 1);
  // c41: ((1 / width)^2 x +8220) x +8220, 0, 0, 0.
  const double scale = Lfs(base, blur + 8220);
  const uint32_t width = Load(base, blur + 16);
  Stfs(base, frame + 100, zero);
  Stfs(base, frame + 104, zero);
  Stfs(base, frame + 108, zero);
  StoreDoubleword(base, frame + 80, width);
  const double one = Lfs(base, kOne);
  const double w = double(float(double(int64_t(width))));
  const double a = double(float(one / w));
  const double b = double(float(one / w));
  double v = double(float(a * b));
  v = double(float(v * scale));
  v = double(float(v * scale));
  Stfs(base, frame + 96, v);
  RecordPixelConstants(base, list, frame + 96, 41, 1);
  // The texel size (the frame's +80 the height, then the width).
  const uint32_t height = Load(base, blur + 20);
  StoreDoubleword(base, frame + 80, height);
  StoreDoubleword(base, frame + 80, width);
  RecordTexelSize(base, list, double(float(double(int64_t(width)))),
                  double(float(double(int64_t(height)))));
  Slot3(base, list, 3, 1, 0);
  Slot2(base, list, 0, 0x10706);
  ShaderPair(base, list, 33);
  Quad(base, list, blur, one, true);
  RecordEndStates(base, list);
  Slot3(base, list, 36, 0, 0);
  command_list::CloseChunk(base, list);
  // The four chunks to the render queue: the upload and the velocities
  // with the key +12 - 2, the velocities' resolve with +12, the blur with
  // +8.
  // (+12 - 2 as the original's 64-bit subtraction from the word.)
  r.Submit(upload, uint64_t(Load(base, blur + 12)) - 2, zero, 0x82136CC0, 0x82136CD0,
           0x82136CF0);
  r.Submit(draw, uint64_t(Load(base, blur + 12)) - 2, zero, 0x82136D04, 0x82136D14, 0x82136D34);
  r.Submit(velocities, Load(base, blur + 12), zero, 0x82136D48, 0x82136D58, 0x82136D74);
  const uint32_t r3 =
      r.Submit(scene, Load(base, blur + 8), zero, 0x82136D88, 0x82136D98, 0x82136DB4);
  Store(base, blur + 40, 0);
  return r3;
}

// --- The glow's star streaks ---

namespace {

namespace ops = car_scene_draw::ops;

constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kStarLines = 64;  // the scan's bound (the game's star: 4)

double Fmuls(double a, double b) { return double(float(a * b)); }
double Fadds(double a, double b) { return double(float(a + b)); }
double Fsubs(double a, double b) { return double(float(a - b)); }
double Fdivs(double a, double b) { return double(float(a / b)); }
double Fmadds(double a, double b, double c) { return double(float(std::fma(a, b, c))); }
double Frsp(double a) { return double(float(a)); }
double Fabs(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) & ~0x8000000000000000ull);
}
// fcfid of a zero-extended word, rounded to single.
double WordToSingle(uint32_t value) { return Frsp(double(int64_t(value))); }
double Lfd(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(uint64_t(Load(base, address)) << 32 | Load(base, address + 4));
}

// memcpy (sub_825A7410) of `bytes` from `from` to `to`, natively where its
// override copies (else the original from `sp` with `lr`).
void Memcpy(uint8_t* base, const RecordCalls& calls, uint32_t to, uint32_t from, uint32_t bytes,
            uint32_t sp, uint32_t lr) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    const uint64_t regs[3] = {to, from, bytes};
    calls.call(kMemcpy, regs, 3, nullptr, 0, sp, lr);
  }
}

}  // namespace

uint32_t GlowRecordStarPass(uint8_t* base, uint32_t stack, uint32_t glow, uint32_t texture,
                            uint32_t samples, double attenuation, double step, double dx,
                            double dy, uint32_t colors, uint32_t pass, uint32_t dest,
                            const RecordCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82134060 Glow::RecordStarPass");
  const uint32_t frame = stack - 768;
  Store(base, frame, stack);  // the back chain, as stwu
  Recorder r(base, calls, frame);
  const uint32_t list = r.Get(4, 0x821340B0, 0x821340C0);
  // The two blocks' w words 1.0: the offsets at +112, the weights at +368.
  const double one = Lfs(base, kOne);
  for (uint32_t i = 0; i < 16; i++) {
    Stfs(base, frame + 124 + 16 * i, one);
  }
  for (uint32_t i = 0; i < 16; i++) {
    Stfs(base, frame + 380 + 16 * i, one);
  }
  if (samples != 0) {
    const double scale = Lfs(base, 0x82008834);
    Stfs(base, frame + 108, one);
    StoreDoubleword(base, frame + 80, pass);
    const double zero = Lfs(base, kZero);
    const double limit = Lfs(base, 0x82002A2C);
    const double passes = Fadds(WordToSingle(pass), one);
    for (uint32_t i = 0; i < samples; i++) {
      StoreDoubleword(base, frame + 80, i);
      const double index = WordToSingle(i);
      const double weight = Frsp(c_runtime::Pow(attenuation, Fmuls(index, step)));
      const uint32_t color = colors + 16 * i;
      const double cr = Lfs(base, color + 0);
      const double cg = Lfs(base, color + 4);
      const double x = Fmuls(index, dx);
      const double cb = Lfs(base, color + 8);
      const double y = Fmuls(index, dy);
      const uint32_t offset = frame + 112 + 16 * i;
      const uint32_t at = frame + 368 + 16 * i;
      Stfs(base, offset + 0, x);
      double wr = Fmuls(cr, weight);
      Stfs(base, offset + 4, y);
      double wg = Fmuls(cg, weight);
      double wb = Fmuls(cb, weight);
      wr = Fmuls(wr, passes);
      wg = Fmuls(wg, passes);
      wb = Fmuls(wb, passes);
      Stfs(base, frame + 96, Fmuls(wr, scale));
      Stfs(base, frame + 100, Fmuls(wg, scale));
      Stfs(base, frame + 104, Fmuls(wb, scale));
      ops::Copy16(base, at, frame + 96);
      // Past the limit: the sample's offset and weight zeroed (times 0.0).
      if (!(Fabs(x) < limit) || !(Fabs(y) < limit)) {
        Stfs(base, at + 0, Fmuls(Lfs(base, at + 0), zero));
        Stfs(base, offset + 0, zero);
        Stfs(base, offset + 4, zero);
        Stfs(base, at + 8, Fmuls(Lfs(base, at + 8), zero));
        Stfs(base, at + 4, Fmuls(Lfs(base, at + 4), zero));
      }
    }
  }
  // c0 and c16 (pixel floats, 16 registers each), copied from the frame.
  Slot3(base, list, 7, 16, 0x01010010);
  Memcpy(base, calls, command_list::AllocateSlots(base, list, 16), frame + 112, 256, frame,
         0x82134280);
  Slot3(base, list, 7, 16, 0x01011010);
  Memcpy(base, calls, command_list::AllocateSlots(base, list, 16), frame + 368, 256, frame,
         0x821342BC);
  Texture(base, list, texture);
  ShaderPair(base, list, 17);
  QuarterQuad(base, list, glow, Lfs(base, 0x82008828));
  return RecordResolve(base, glow, list, dest, 256, 0xFFFFFFFDu, false).value_or(0);
}

namespace {

// Glow::RecordStar's slots: 80, and 48 for each line's pass; nothing where
// it leaves the star to the original (more than kStarLines lines, a line of
// more than 2 passes).
std::optional<uint64_t> StarSlots(uint8_t* base, uint32_t glow) {
  const uint32_t star = glow + 8112;
  const uint32_t lines = Load(base, star);
  if (lines > kStarLines) {
    return std::nullopt;
  }
  uint64_t slots = 80;
  for (uint32_t d = 0; d < lines; d++) {
    const uint32_t passes = Load(base, star + 4 + 16 * d);
    if (passes > 2) {
      return std::nullopt;
    }
    slots += 48 * uint64_t(passes);
  }
  return slots;
}

}  // namespace

std::optional<uint32_t> GlowRecordStar(uint8_t* base, uint32_t stack, uint32_t glow,
                                       const RecordCalls& calls) {
  const uint32_t frame = stack - 672;
  Store(base, frame, stack);  // the back chain, as stwu
  Recorder r(base, calls, frame);
  const uint32_t list = r.Get(4, 0x82136FD8, 0x82136FE8);
  const std::optional<uint64_t> slots = StarSlots(base, glow);
  if (!slots || !r.Room(list, *slots)) {
    return std::nullopt;
  }
  const uint32_t star = glow + 8112;
  const uint32_t lines = Load(base, star);
  ITC_NATIVE_RUNNING("sub_82136FA8 Glow::RecordStar");
  Slot3(base, list, 1, 1, 0);
  Slot2(base, list, 60, 0);
  Slot3(base, list, 26, 1, 0);
  const uint32_t bytes = command_list::AllocateBytes(base, list, 280, false);
  Store(base, bytes + 8, 1);
  Store(base, bytes + 276, 0);
  Store(base, bytes + 4, 0);
  Store(base, bytes + 0, 0);
  const double zero = Lfs(base, kZero);
  Stfs(base, bytes + 272, zero);
  Store(base, bytes + 268, 0xFF000000u);
  const double turn = Frsp(c_runtime::Atan(Lfd(base, 0x82008920)));
  const double one = Lfs(base, kOne);
  // --- The 16 colors at +208: two passes of the star's eight ---
  for (uint32_t i = 0; i < 16; i++) {
    Stfs(base, frame + 220 + 16 * i, one);
  }
  const uint32_t width = Load(base, glow + 16);
  const double aberration = Lfs(base, glow + 8088);
  const uint32_t height = Load(base, glow + 20);
  const double fraction = Lfs(base, 0x82008834);
  StoreDoubleword(base, frame + 112, width);
  StoreDoubleword(base, frame + 96, height);
  const double w = WordToSingle(width);
  const double toward = Lfs(base, 0x82008918);
  const double h = WordToSingle(height);
  uint32_t at = frame + 216;
  for (uint32_t p = 1; p <= 2; p++) {
    StoreDoubleword(base, frame + 96, p);
    const double t = Fmuls(WordToSingle(p), fraction);
    for (uint32_t k = 0; k < 8; k++) {
      Stfs(base, at + 4, one);
      ops::Copy16(base, frame + 112, star + 112 + 16 * k);
      double cr = Lfs(base, frame + 112);
      double cg = Lfs(base, frame + 116);
      const double dr = Fsubs(toward, cr);
      double cb = Lfs(base, frame + 120);
      const double dg = Fsubs(toward, cg);
      const double db = Fsubs(toward, cb);
      cr = Fmadds(dr, t, cr);
      Stfs(base, frame + 112, cr);
      cg = Fmadds(dg, t, cg);
      Stfs(base, frame + 116, cg);
      cb = Fmadds(db, t, cb);
      Stfs(base, frame + 120, cb);
      cr = Fsubs(cr, toward);
      cg = Fsubs(cg, toward);
      cb = Fsubs(cb, toward);
      Stfs(base, at - 8, Fmadds(cr, aberration, toward));
      Stfs(base, at - 4, Fmadds(cg, aberration, toward));
      Stfs(base, at + 0, Fmadds(cb, aberration, toward));
      at += 16;
    }
  }
  // --- The turn: the view's z axis (the camera pass's inverse view times
  // (0, 0, 1, 0)) with y 0, normalized, its angle to x by acos (from 2 pi
  // when its z is negative) ---
  const uint32_t camera = kCameraPasses + Load(base, glow + 36) * 592;
  double angle = zero;
  if (Load(base, camera + 588) != 0) {
    Stfs(base, frame + 156, one);
    Stfs(base, frame + 172, one);
    Stfs(base, frame + 188, one);
    Stfs(base, frame + 204, one);
    Stfs(base, frame + 128, zero);
    Stfs(base, frame + 132, zero);
    Stfs(base, frame + 136, one);
    Stfs(base, frame + 140, zero);
    matrix_math::Inverse(base, frame + 144, camera + 16);
    matrix_math::Vec4Transform(base, frame + 128, frame + 144, frame + 128);
    Stfs(base, frame + 132, zero);
    Stfs(base, frame + 96, one);
    Stfs(base, frame + 100, zero);
    Stfs(base, frame + 104, zero);
    Stfs(base, frame + 108, zero);
    const simde__m128 v = ops::Lvx(base, frame + 128);
    const simde__m128 x_axis = ops::Lvx(base, frame + 96);
    Stfs(base, frame + 96, zero);
    Stfs(base, frame + 100, zero);
    Stfs(base, frame + 104, one);
    Stfs(base, frame + 108, zero);
    const simde__m128 z_axis = ops::Lvx(base, frame + 96);
    {
      ops::FlushToZero ftz;
      const simde__m128 dot = ops::Dot3(v, v);
      const simde__m128 n = ops::Normalized(v, dot);
      const simde__m128 x = ops::Dot3(n, x_axis);
      ops::Stvx(base, frame + 128, n);
      const simde__m128 z = ops::Dot3(n, z_axis);
      ops::Stvx(base, frame + 96, x);
      ops::Stvx(base, frame + 112, z);
    }
    const double cosine = Lfs(base, frame + 96);
    const double arc = c_runtime::Acos(cosine);
    const double z = Lfs(base, frame + 112);
    angle = Frsp(arc);
    if (z < zero) {
      angle = Fsubs(Lfs(base, 0x82001FA8), angle);
    }
  }
  // --- The lines' passes ---
  const double rotation = Fadds(Fadds(Lfs(base, glow + 8084), Lfs(base, star + 100)), angle);
  if (Load(base, star) > 0) {
    const double size = Fadds(h, w);
    const double texel_x = Fdivs(one, w);
    const double texel_y = Fdivs(one, h);
    const double first_step =
        Fmuls(Fdivs(Fmuls(Fadds(turn, Lfs(base, 0x82001FBC)), Lfs(base, 0x82008914)), size),
              Lfs(base, 0x820029D4));
    uint32_t line = star + 4;
    uint32_t target = glow + 8360;
    for (uint32_t d = 0; d < Load(base, star); d++, line += 16, target += 4) {
      ops::Copy16(base, frame + 96, line);
      const double length = Lfs(base, glow + 8468);
      const double attenuation = Lfs(base, glow + 8472);
      const double a = Fadds(Lfs(base, frame + 108), rotation);
      double dx = Fmuls(Fmuls(Frsp(c_runtime::Sin(a)), texel_x), length);
      const double cosine = c_runtime::Cos(a);
      const uint32_t passes = Load(base, frame + 96);
      double step = first_step;
      double dy = Fmuls(Fmuls(Frsp(cosine), texel_y), length);
      for (uint32_t p = 0; p < passes; p++) {
        const uint32_t source = p == 0 ? Load(base, glow + 8408) : Load(base, target);
        const uint32_t texture = Load(base, source + 24);
        const uint32_t dest = Load(base, Load(base, target) + 24);
        Store(base, frame + 92, dest);
        GlowRecordStarPass(base, frame, glow, texture, 8, attenuation, step, dx, dy,
                           frame + 208 + ((passes - p - 1) << 7), p, dest, calls);
        const double k = Lfs(base, 0x82008828);
        dx = Fmuls(dx, k);
        dy = Fmuls(dy, k);
        step = Fmuls(step, k);
      }
    }
  }
  // --- The lines merged: shader pair 18 over the four line textures into
  // +8384's, then pairs 23 and 22 over it ---
  Slot3(base, list, 2, 5, 0);
  for (uint32_t s = 0; s < 5; s++) {
    Slot3(base, list, s, 16, 1);
  }
  Slot3(base, list, 4, 4, 0);
  for (uint32_t s = 0; s < 4; s++) {
    Slot2(base, list, s, Load(base, Load(base, glow + 8360 + 4 * s) + 24));
  }
  ShaderPair(base, list, 18);
  QuarterQuad(base, list, glow, Lfs(base, 0x82008828));
  RecordResolve(base, glow, list, Load(base, Load(base, glow + 8384) + 24), 0, 0, false);
  Texture(base, list, Load(base, Load(base, glow + 8384) + 24));
  Slot3(base, list, 2, 3, 0);
  Slot3(base, list, 0, 20, 1);
  Slot3(base, list, 0, 16, 1);
  Slot3(base, list, 0, 24, 2);
  ShaderPair(base, list, 23);
  const uint32_t quarter_h = Load(base, glow + 20) >> 2;
  const uint32_t quarter_w = Load(base, glow + 16) >> 2;
  StoreDoubleword(base, frame + 96, quarter_h);
  StoreDoubleword(base, frame + 112, quarter_w);
  RecordTexelSize(base, list, WordToSingle(quarter_w), WordToSingle(quarter_h));
  QuarterQuad(base, list, glow, one);
  RecordResolve(base, glow, list, Load(base, Load(base, glow + 8384) + 24), 0, 0, false);
  ShaderPair(base, list, 22);
  QuarterQuad(base, list, glow, one);
  RecordResolve(base, glow, list, Load(base, Load(base, glow + 8384) + 24), 0, 0, false);
  Slot3(base, list, 1, 1, 0);
  Slot2(base, list, 60, 1);
  Slot3(base, list, 3, 1, 0);
  return Slot2(base, list, 0, 0x10101);
}

std::optional<uint32_t> GlowRecord(uint8_t* base, uint32_t stack, uint32_t glow,
                                   const RecordCalls& calls) {
  const uint32_t frame = stack - 272;
  Store(base, frame, stack);  // the back chain, as stwu
  Recorder r(base, calls, frame);
  const uint32_t list = r.Get(4, 0x82137A04, 0x82137A14);
  // 142 slots of its own (with the history's resolve), and the star's.
  const std::optional<uint64_t> star = StarSlots(base, glow);
  if (!star || !r.Ready(list, 142 + *star)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_821379D8 Glow::Record");
  command_list::OpenChunk(base, list);
  const uint32_t header = Header(base, list);
  RecordBeginStates(base, list);
  // The frame resolved into +8352's texture (by the view).
  RecordResolve(base, glow, list, Load(base, Load(base, glow + 8352) + 24), 0, 0xFFFFFFFDu, true);
  Slot3(base, list, 1, 1, 0);
  Slot2(base, list, 60, 0);
  Slot3(base, list, 3, 1, 0);
  Slot2(base, list, 0, 0x10001);
  // c34: +8444, +8448, 0, 0 (the bright pass).
  const double zero = Lfs(base, kZero);
  const uint32_t c = frame + 96;
  Stfs(base, c + 8, zero);
  Stfs(base, c + 12, zero);
  Stfs(base, c + 0, Lfs(base, glow + 8444));
  Stfs(base, c + 4, Lfs(base, glow + 8448));
  RecordPixelConstants(base, list, c, 34, 1);
  // Drawn into +8412 with shader pair 10 twice, c33 from +8440, then
  // +8436, with +8452, +8456 and +8476.
  RecordTargets(base, list, Load(base, glow + 8412), 0);
  Declaration(base, list);
  Texture(base, list, Load(base, Load(base, glow + 8352) + 24));
  Slot3(base, list, 2, 6, 0);
  for (uint32_t sampler = 0; sampler < 2; sampler++) {
    Slot3(base, list, sampler, 20, 1);
    Slot3(base, list, sampler, 16, 1);
    Slot3(base, list, sampler, 24, 2);
  }
  const double quarter = Lfs(base, 0x82008828);
  for (uint32_t pass = 0; pass < 2; pass++) {
    if (pass == 1) {
      Texture(base, list, Load(base, Load(base, glow + 8352) + 24));
    }
    ShaderPair(base, list, 10);
    if (pass == 0) {
      TexelSize(base, list, glow, frame + 96, frame + 80);
    } else {
      TexelSize(base, list, glow, frame + 80, frame + 96);
    }
    Stfs(base, c + 0, Lfs(base, glow + (pass == 0 ? 8440 : 8436)));
    Stfs(base, c + 4, Lfs(base, glow + 8452));
    Stfs(base, c + 8, Lfs(base, glow + 8456));
    Stfs(base, c + 12, Lfs(base, glow + 8476));
    RecordPixelConstants(base, list, c, 33, 1);
    QuarterQuad(base, list, glow, quarter);
  }
  // The history (+8408) blended over it with shader pair 7, weighted by
  // +8460 (or 1 - 0 when +40, the reset, is set).
  Texture(base, list, Load(base, Load(base, glow + 8408) + 24));
  Slot3(base, list, 1, 5, 0);
  for (const uint32_t state : {60u, 308u, 312u, 316u, 320u}) {
    Slot2(base, list, state, 1);
  }
  Slot3(base, list, 3, 1, 0);
  Slot2(base, list, 0, 0x10706);
  ShaderPair(base, list, 7);
  const uint32_t vertices = AddQuad(base, glow, Load(base, glow + 16) >> 2,
                                    Load(base, glow + 20) >> 2, Lfs(base, kOne), false);
  const bool reset = Load(base, glow + 40) != 0;
  for (const uint32_t at : {16u, 20u, 24u, 56u, 60u, 64u}) {
    Stfs(base, vertices + at, quarter);
  }
  for (const uint32_t at : {28u, 68u, 108u}) {
    Stfs(base, vertices + at, reset ? zero : Lfs(base, glow + 8460));
  }
  for (const uint32_t at : {104u, 100u, 96u}) {
    Stfs(base, vertices + at, quarter);
  }
  Slot3(base, list, 19, 1, 0);
  {
    const uint32_t s = Slot(base, list);
    Store(base, s + 0, 8);
    guest::StoreU16(base, s + 4, 3);
    guest::StoreU16(base, s + 6, 40);
    Store(base, s + 8, vertices);
  }
  Slot3(base, list, 1, 5, 0);
  for (const uint32_t state : {60u, 308u, 312u, 316u, 320u}) {
    Slot2(base, list, state, 0);
  }
  Slot3(base, list, 3, 1, 0);
  Slot2(base, list, 0, 0x10001);
  // The history resolved when +48 is set and +44 isn't.
  if (Load(base, glow + 48) != 0 && Load(base, glow + 44) == 0) {
    RecordResolve(base, glow, list, Load(base, Load(base, glow + 8408) + 24), 0, 0xFFFFFFFDu,
                  false);
  }
  // The star streaks (room checked above), then the scene target's
  // surfaces bound again.
  if (!GlowRecordStar(base, frame, glow, calls)) {
    r.Call(0x82136FA8, {glow}, 0x821382D4);
  }
  scene_target::RecordBoundTargets(base, list);
  Slot3(base, list, 3, 1, 0);
  Slot2(base, list, 0, 0x10001);
  Slot3(base, list, 2, 1, 0);
  Slot3(base, list, 2, 16, 1);
  // Shader pair 21 over the frame with +8352's, +8404's and +8384's
  // textures.
  Slot3(base, list, 4, 3, 0);
  Slot2(base, list, 0, Load(base, Load(base, glow + 8352) + 24));
  Slot2(base, list, 1, Load(base, Load(base, glow + 8404) + 24));
  Slot2(base, list, 2, Load(base, Load(base, glow + 8384) + 24));
  ShaderPair(base, list, 21);
  Quad(base, list, glow, quarter, true);
  RecordEndStates(base, list);
  Slot3(base, list, 36, 0, 0);
  command_list::CloseChunk(base, list);
  const uint32_t r3 =
      r.Submit(header, Load(base, glow + 8), zero, 0x821384E0, 0x821384F0, 0x8213850C);
  Store(base, glow + 40, 0);
  return r3;
}

}  // namespace post_effects
