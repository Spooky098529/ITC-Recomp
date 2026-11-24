// The post effects' records natively (post_effects.h,
// docs/functions/post-effects.md): ColorFilter::Record,
// TargetFilter::Record, MotionBlur::Record and the glow's star streaks
// (Glow::RecordStar and RecordStarPass; their math, the vector unit's
// steps included, as the translated code does it: this file builds with
// -ffp-model=strict and -ffp-contract=off). Step for step as the recompiled functions: their
// frames where the originals' are (with the words handed on there: the
// constants' blocks, the star's colors, offsets, weights and matrices; the
// words only the port reads are host values, E5 step 6), the
// render manager's getters, CameraPasses::RenderTarget (its diagnostics'
// wrap) and the render queue as guest calls with the originals' return
// addresses, the command list and the recording helpers as plain C++. The
// whole record is declined (nothing done but the getters) when the list
// hasn't room for all its slots or a chunk is open, where the original
// prints its messages: left to it.

#include "base/float_mode.h"
#include "post_effects/post_effects.h"

#include "guest.h"

#include "app_start_up/start_up.h"
#include "base/log.h"
#include "c_runtime/math.h"
#include "car_scene_draw/ops.h"
#include "command_list/command_list.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "models/model.h"
#include "native/buffers.h"
#include "scene_target/scene_target.h"
#include "schema/cameras.h"
#include "schema/command_list.h"
#include "schema/graphics_resources.h"
#include "schema/post_effects.h"

#include <bit>
#include <initializer_list>

namespace post_effects {

namespace {

using G = schema::PostEffectsGlobals;
using schema::PostEffect;
using schema::RenderTexture;

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
using Fpscr = ::base::FloatMode;

// tan as the original's call gives it: nearest rounding (its _ctrlfp), or
// false where c_runtime::Tan can't stand in.
bool Tangent(double x, double* result) {
  return (Fpscr::getcsr() & Fpscr::RoundMaskVal) == 0 && c_runtime::Tan(x, result);
}

// A render texture's header (its +24), as the commands take it.
uint32_t HeaderOf(const RenderTexture& texture) {
  return texture.header().address;
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
  // (Slot 8's plain C++ called directly where it's the game's,
  // app_start_up::RenderManagerScene.)
  uint32_t Get(uint32_t byte, uint32_t lr1, uint32_t lr2) {
    const uint32_t manager = G::render_manager(base_).address;
    const auto scene = app_start_up::RenderManagerScene(base_, manager);
    const uint32_t object = scene ? *scene : VtableCall(manager, 32, lr1);
    return VtableCall(object, byte, lr2);
  }
  // Whether the record may go on natively: no chunk open, room for `slots`.
  bool Ready(uint32_t list, uint32_t slots) const {
    const schema::CommandList commands(base_, list);
    const uint32_t used = commands.used();
    return commands.chunk_open() == 0 &&
           schema::CommandListGlobals::open_chunks(base_) == 0 && used + slots >= used &&
           used + slots < commands.capacity();
  }
  // Whether the list has room for `slots` (a record inside a chunk already
  // open: only the room).
  bool Room(uint32_t list, uint64_t slots) const {
    const schema::CommandList commands(base_, list);
    const uint64_t used = commands.used();
    return used + slots < commands.capacity();
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
  Store(base, Slot(base, list), G::vertex_declaration<schema::View>(base).address);
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
  const PostEffect e(base, effect);
  Store(base, s + 8, AddQuad(base, effect, e.width(), e.height(), value, at_view));
}
// Command 19 and a quad at a quarter of the effect's size (+16 >> 2, +20
// >> 2), not at the view.
void QuarterQuad(uint8_t* base, uint32_t list, uint32_t effect, double value) {
  Slot3(base, list, 19, 1, 0);
  const uint32_t s = Slot(base, list);
  Store(base, s + 0, 8);
  guest::StoreU16(base, s + 4, 3);
  guest::StoreU16(base, s + 6, 40);
  const PostEffect e(base, effect);
  Store(base, s + 8, AddQuad(base, effect, e.width() >> 2, e.height() >> 2, value, false));
}
// The texel size from the effect's size. (The original keeps the two words
// in its frame for the conversion; nothing reads them there: not written,
// E5 step 6.)
void TexelSize(uint8_t* base, uint32_t list, uint32_t effect) {
  const PostEffect e(base, effect);
  const uint32_t height = e.height();
  const uint32_t width = e.width();
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
  const schema::ColorFilter f(base, filter);
  command_list::OpenChunk(base, list);
  const uint32_t header = Header(base, list);
  RecordResolve(base, filter, list, HeaderOf(f.frame_texture()), 0, 0, true);
  Declaration(base, list);
  RecordBeginStates(base, list);
  Texture(base, list, HeaderOf(f.frame_texture()));
  SamplerStates(base, list);
  // The strength (+8112) clamped to the two constants (a NaN stays), times
  // tan(the constant at 0x82002CD8).
  double strength = double(f.strength());
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
  TexelSize(base, list, filter);
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  // c37: the inverses of +8080 to +8088.
  Stfs(base, c + 0, double(float(one / double(f.color0_r()))));
  Stfs(base, c + 4, double(float(one / double(f.color0_g()))));
  Stfs(base, c + 8, double(float(one / double(f.color0_b()))));
  Stfs(base, c + 12, zero);
  RecordPixelConstants(base, list, c, 37, 1);
  // c38: +8120, (strength + 1) times the tangent, +8116, 0.
  const double sum = double(float(strength + one));
  Stfs(base, c + 0, double(f.c38_x()));
  Stfs(base, c + 8, double(f.c38_z()));
  Stfs(base, c + 12, zero);
  Stfs(base, c + 4, double(float(sum * scale)));
  RecordPixelConstants(base, list, c, 38, 1);
  // c39: +8096 to +8104, 0.
  Stfs(base, c + 0, double(f.color1_r()));
  Stfs(base, c + 4, double(f.color1_g()));
  Stfs(base, c + 8, double(f.color1_b()));
  Stfs(base, c + 12, zero);
  RecordPixelConstants(base, list, c, 39, 1);
  RecordPixelBool(base, list, f.bool_0(), 0);
  RecordPixelBool(base, list, f.bool_1(), 1);
  Slot3(base, list, 1, 1, 0);
  Slot2(base, list, 60, 0);
  Slot3(base, list, 3, 1, 0);
  Slot2(base, list, 0, 0x10001);
  ShaderPair(base, list, 31);
  Quad(base, list, filter, one, true);
  RecordEndStates(base, list);
  Slot3(base, list, 36, 0, 0);
  command_list::CloseChunk(base, list);
  const uint32_t r3 = r.Submit(header, f.sort_key(), zero, 0x82135670, 0x82135680, 0x8213569C);
  f.set_reset(0);
  return r3;
}

std::optional<uint32_t> TargetFilterRecord(uint8_t* base, uint32_t stack, uint32_t filter,
                                           const RecordCalls& calls) {
  const uint32_t frame = stack - 224;
  Store(base, frame, stack);  // the back chain, as stwu
  const schema::TargetFilter f(base, filter);
  const uint32_t flags = f.flags();
  uint32_t id;
  if ((flags & 1) != 0) {
    if (schema::CameraPass(base, G::camera_passes_address() + 12 * schema::CameraPass::kSize)
            .enabled() == 0) {
      return filter;
    }
    id = 4;
  } else if ((flags & 2) != 0) {
    if (schema::CameraPass(base, G::camera_passes_address() + 11 * schema::CameraPass::kSize)
            .enabled() == 0) {
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
    RecordTargets(base, list, f.surface().address, 0);
    TexelSize(base, list, filter);
    Texture(base, list, target);
    ShaderPair(base, list, (second ? 28 : 24) + pass);
    Quad(base, list, filter, one, false);
    RecordResolve(base, filter, list, target, 0, 0, false);
  }
  RecordEndStates(base, list);
  Slot3(base, list, 36, 0, 0);
  command_list::CloseChunk(base, list);
  const uint32_t r3 =
      r.Submit(header, f.sort_key(), Lfs(base, kZero), 0x821360EC, 0x821360FC, 0x8213611C);
  f.set_reset(0);
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
  const schema::MotionBlur mb(base, blur);
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
  const bool logic = mb.updated() != 0 && mb.no_history() == 0;
  interpolation::BlendState_MotionBlurCopy(
      base, blur, matrix,
      logic ? mb.view_projection_address() : mb.previous_view_projection_address());
  list = r.Get(4, 0x82136550, 0x82136560);
  Slot3(base, list, 36, 0, 0);
  list = r.Get(4, 0x8213658C, 0x8213659C);
  command_list::CloseChunk(base, list);
  // On a logic pass with its camera pass active: the pair moved on (+8144 =
  // +8080, +8080 = the pass's view-projection at +144).
  if (logic) {
    const schema::CameraPass pass(
        base, G::camera_passes_address() + mb.camera_pass() * schema::CameraPass::kSize);
    if (pass.enabled() != 0) {
      const uint32_t current = mb.view_projection_address();
      const uint32_t previous = mb.previous_view_projection_address();
      matrix_math::Copy(base, previous, current);
      interpolation::BlendState_OnCopy(base, previous, current, 0x821365E8);
      matrix_math::Copy(base, current, pass.view_projection_address());
      interpolation::BlendState_OnCopy(base, current, pass.view_projection_address(), 0x821365F4);
    }
  }
  // --- The blur's chunk ---
  list = r.Get(4, 0x82136608, 0x82136618);
  command_list::OpenChunk(base, list);
  const uint32_t draw = Header(base, list);
  // The velocities drawn into +8072 (with +8076's depth): shader pair 32
  // and 280 bytes of constants.
  RecordTargets(base, list, mb.velocity_surface().address, mb.depth_surface().address);
  Slot3(base, list, 1, 3, 0);
  Slot2(base, list, 40, 1);
  Slot2(base, list, 48, 1);
  Slot2(base, list, 44, 6);
  Slot3(base, list, 26, 1, 0);
  const schema::ClearItem clear(base, command_list::AllocateBytes(base, list, 280, false));
  const double zero = Lfs(base, kZero);
  clear.set_depth(float(zero));
  clear.set_stencil(0);
  clear.set_color(0x80808000);
  clear.set_rects(0);
  clear.set_flags(49);
  clear.set_rect_count(0);
  ShaderPair(base, list, 32);
  Slot3(base, list, 36, 0, 0);
  // Resolved into +8064's texture; the frame into +8068's.
  const uint32_t velocities = Header(base, list);
  RecordResolve(base, blur, list, HeaderOf(mb.velocity_texture()), 0, 0, false);
  Slot3(base, list, 36, 0, 0);
  const uint32_t scene = Header(base, list);
  RecordResolve(base, blur, list, HeaderOf(mb.frame_texture()), 0, 0xFFFFFFFDu, true);
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
  Slot2(base, list, 0, HeaderOf(mb.frame_texture()));
  Slot2(base, list, 1, HeaderOf(mb.velocity_texture()));
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
  Stfs(base, c + 0, double(mb.c40_x()));
  Stfs(base, c + 4, double(mb.c40_y()));
  Stfs(base, c + 8, double(mb.c40_z()));
  RecordPixelConstants(base, list, c, 40, 1);
  // c41: ((1 / width)^2 x +8220) x +8220, 0, 0, 0.
  const double scale = double(mb.scale());
  const uint32_t width = mb.width();
  Stfs(base, frame + 100, zero);
  Stfs(base, frame + 104, zero);
  Stfs(base, frame + 108, zero);
  const double one = Lfs(base, kOne);
  const double w = double(float(double(int64_t(width))));
  const double a = double(float(one / w));
  const double b = double(float(one / w));
  double v = double(float(a * b));
  v = double(float(v * scale));
  v = double(float(v * scale));
  Stfs(base, frame + 96, v);
  RecordPixelConstants(base, list, frame + 96, 41, 1);
  // The texel size. (The original converts the height and width through
  // its frame's +80, and the width above: not written, E5 step 6.)
  const uint32_t height = mb.height();
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
  r.Submit(upload, uint64_t(mb.second_sort_key()) - 2, zero, 0x82136CC0, 0x82136CD0,
           0x82136CF0);
  r.Submit(draw, uint64_t(mb.second_sort_key()) - 2, zero, 0x82136D04, 0x82136D14, 0x82136D34);
  r.Submit(velocities, mb.second_sort_key(), zero, 0x82136D48, 0x82136D58, 0x82136D74);
  const uint32_t r3 = r.Submit(scene, mb.sort_key(), zero, 0x82136D88, 0x82136D98, 0x82136DB4);
  mb.set_reset(0);
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
    const double zero = Lfs(base, kZero);
    const double limit = Lfs(base, 0x82002A2C);
    const double passes = Fadds(WordToSingle(pass), one);
    for (uint32_t i = 0; i < samples; i++) {
      const double index = WordToSingle(i);
      const double weight = Frsp(c_runtime::Pow(attenuation, Fmuls(index, step)));
      const schema::GlowStarColor color(base, colors + 16 * i);
      const double cr = double(color.r());
      const double cg = double(color.g());
      const double x = Fmuls(index, dx);
      const double cb = double(color.b());
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
      // The weight (with +12's 1.0) straight into the block: the original
      // builds it at its frame's +96 and copies it (E5 step 6).
      Stfs(base, at + 0, Fmuls(wr, scale));
      Stfs(base, at + 4, Fmuls(wg, scale));
      Stfs(base, at + 8, Fmuls(wb, scale));
      Stfs(base, at + 12, one);
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
  const schema::Glow g(base, glow);
  const uint32_t lines = g.star_line_count();
  if (lines > kStarLines) {
    return std::nullopt;
  }
  uint64_t slots = 80;
  for (uint32_t d = 0; d < lines; d++) {
    const uint32_t passes = schema::StarLine(base, g.star_lines_address() + 16 * d).passes();
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
  const schema::Glow g(base, glow);
  const uint32_t lines = g.star_line_count();
  ITC_NATIVE_RUNNING("sub_82136FA8 Glow::RecordStar");
  Slot3(base, list, 1, 1, 0);
  Slot2(base, list, 60, 0);
  Slot3(base, list, 26, 1, 0);
  const schema::ClearItem clear(base, command_list::AllocateBytes(base, list, 280, false));
  clear.set_flags(1);
  clear.set_stencil(0);
  clear.set_rects(0);
  clear.set_rect_count(0);
  const double zero = Lfs(base, kZero);
  clear.set_depth(float(zero));
  clear.set_color(0xFF000000u);
  const double turn = Frsp(c_runtime::Atan(Lfd(base, 0x82008920)));
  const double one = Lfs(base, kOne);
  // --- The 16 colors at +208: two passes of the star's eight ---
  for (uint32_t i = 0; i < 16; i++) {
    Stfs(base, frame + 220 + 16 * i, one);
  }
  const uint32_t width = g.width();
  const double aberration = double(g.glare_chromatic_aberration());
  const uint32_t height = g.height();
  const double fraction = Lfs(base, 0x82008834);
  const double w = WordToSingle(width);
  const double toward = Lfs(base, 0x82008918);
  const double h = WordToSingle(height);
  uint32_t at = frame + 216;
  for (uint32_t p = 1; p <= 2; p++) {
    const double t = Fmuls(WordToSingle(p), fraction);
    for (uint32_t k = 0; k < 8; k++) {
      Stfs(base, at + 4, one);
      // The star color read in place (the original copies it to its
      // frame's +112 and writes the blend back there, read by nothing: E5
      // step 6).
      const uint32_t color = g.star_colors_address() + 16 * k;
      double cr = Lfs(base, color);
      double cg = Lfs(base, color + 4);
      const double dr = Fsubs(toward, cr);
      double cb = Lfs(base, color + 8);
      const double dg = Fsubs(toward, cg);
      const double db = Fsubs(toward, cb);
      cr = Fmadds(dr, t, cr);
      cg = Fmadds(dg, t, cg);
      cb = Fmadds(db, t, cb);
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
  const schema::CameraPass camera(
      base, G::camera_passes_address() + g.camera_pass() * schema::CameraPass::kSize);
  double angle = zero;
  if (camera.enabled() != 0) {
    Stfs(base, frame + 156, one);
    Stfs(base, frame + 172, one);
    Stfs(base, frame + 188, one);
    Stfs(base, frame + 204, one);
    Stfs(base, frame + 128, zero);
    Stfs(base, frame + 132, zero);
    Stfs(base, frame + 136, one);
    Stfs(base, frame + 140, zero);
    matrix_math::Inverse(base, frame + 144, camera.view_address());
    matrix_math::Vec4Transform(base, frame + 128, frame + 144, frame + 128);
    Stfs(base, frame + 132, zero);
    const simde__m128 v = ops::Lvx(base, frame + 128);
    // The two axes, the normalized vector and the two dot products as
    // host vectors (the original builds and stores them in its frame's
    // +96, +112 and +128 for itself: E5 step 6), the words as stfs makes
    // them.
    const uint32_t one_bits = std::bit_cast<uint32_t>(float(one));
    const uint32_t zero_bits = std::bit_cast<uint32_t>(float(zero));
    const simde__m128 x_axis = ops::VectorOf(one_bits, zero_bits, zero_bits, zero_bits);
    const simde__m128 z_axis = ops::VectorOf(zero_bits, zero_bits, one_bits, zero_bits);
    simde__m128 x;
    simde__m128 z_dot;
    {
      ops::FlushToZero ftz;
      const simde__m128 dot = ops::Dot3(v, v);
      const simde__m128 n = ops::Normalized(v, dot);
      x = ops::Dot3(n, x_axis);
      z_dot = ops::Dot3(n, z_axis);
    }
    const double cosine = double(std::bit_cast<float>(ops::WordOf(x, 0)));
    const double arc = c_runtime::Acos(cosine);
    const double z = double(std::bit_cast<float>(ops::WordOf(z_dot, 0)));
    angle = Frsp(arc);
    if (z < zero) {
      angle = Fsubs(Lfs(base, 0x82001FA8), angle);
    }
  }
  // --- The lines' passes ---
  const double rotation =
      Fadds(Fadds(double(g.glare_star_inclination()), double(g.star_inclination())), angle);
  if (g.star_line_count() > 0) {
    const double size = Fadds(h, w);
    const double texel_x = Fdivs(one, w);
    const double texel_y = Fdivs(one, h);
    const double first_step =
        Fmuls(Fdivs(Fmuls(Fadds(turn, Lfs(base, 0x82001FBC)), Lfs(base, 0x82008914)), size),
              Lfs(base, 0x820029D4));
    uint32_t line = g.star_lines_address();
    uint32_t target = g.line_texture_0_address();
    for (uint32_t d = 0; d < g.star_line_count(); d++, line += 16, target += 4) {
      // The line's words read in place (the original copies its 16 bytes
      // to its frame's +96 first: E5 step 6).
      const double length = double(g.star_step());
      const double attenuation = double(g.star_attenuation());
      const double a = Fadds(Lfs(base, line + 12), rotation);
      double dx = Fmuls(Fmuls(Frsp(c_runtime::Sin(a)), texel_x), length);
      const double cosine = c_runtime::Cos(a);
      const uint32_t passes = Load(base, line);
      double step = first_step;
      double dy = Fmuls(Fmuls(Frsp(cosine), texel_y), length);
      for (uint32_t p = 0; p < passes; p++) {
        const uint32_t source = p == 0 ? g.history().address : Load(base, target);
        const uint32_t texture = HeaderOf(RenderTexture(base, source));
        const uint32_t dest = HeaderOf(RenderTexture(base, Load(base, target)));
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
    Slot2(base, list, s,
          HeaderOf(RenderTexture(base, Load(base, g.line_texture_0_address() + 4 * s))));
  }
  ShaderPair(base, list, 18);
  QuarterQuad(base, list, glow, Lfs(base, 0x82008828));
  RecordResolve(base, glow, list, HeaderOf(g.star_texture()), 0, 0, false);
  Texture(base, list, HeaderOf(g.star_texture()));
  Slot3(base, list, 2, 3, 0);
  Slot3(base, list, 0, 20, 1);
  Slot3(base, list, 0, 16, 1);
  Slot3(base, list, 0, 24, 2);
  ShaderPair(base, list, 23);
  const uint32_t quarter_h = g.height() >> 2;
  const uint32_t quarter_w = g.width() >> 2;
  RecordTexelSize(base, list, WordToSingle(quarter_w), WordToSingle(quarter_h));
  QuarterQuad(base, list, glow, one);
  RecordResolve(base, glow, list, HeaderOf(g.star_texture()), 0, 0, false);
  ShaderPair(base, list, 22);
  QuarterQuad(base, list, glow, one);
  RecordResolve(base, glow, list, HeaderOf(g.star_texture()), 0, 0, false);
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
  const schema::Glow g(base, glow);
  command_list::OpenChunk(base, list);
  const uint32_t header = Header(base, list);
  RecordBeginStates(base, list);
  // The frame resolved into +8352's texture (by the view).
  RecordResolve(base, glow, list, HeaderOf(g.frame_texture()), 0, 0xFFFFFFFDu, true);
  Slot3(base, list, 1, 1, 0);
  Slot2(base, list, 60, 0);
  Slot3(base, list, 3, 1, 0);
  Slot2(base, list, 0, 0x10001);
  // c34: +8444, +8448, 0, 0 (the bright pass).
  const double zero = Lfs(base, kZero);
  const uint32_t c = frame + 96;
  Stfs(base, c + 8, zero);
  Stfs(base, c + 12, zero);
  Stfs(base, c + 0, double(g.c34_x()));
  Stfs(base, c + 4, double(g.c34_y()));
  RecordPixelConstants(base, list, c, 34, 1);
  // Drawn into +8412 with shader pair 10 twice, c33 from +8440, then
  // +8436, with +8452, +8456 and +8476.
  RecordTargets(base, list, g.quarter_surface().address, 0);
  Declaration(base, list);
  Texture(base, list, HeaderOf(g.frame_texture()));
  Slot3(base, list, 2, 6, 0);
  for (uint32_t sampler = 0; sampler < 2; sampler++) {
    Slot3(base, list, sampler, 20, 1);
    Slot3(base, list, sampler, 16, 1);
    Slot3(base, list, sampler, 24, 2);
  }
  const double quarter = Lfs(base, 0x82008828);
  for (uint32_t pass = 0; pass < 2; pass++) {
    if (pass == 1) {
      Texture(base, list, HeaderOf(g.frame_texture()));
    }
    ShaderPair(base, list, 10);
    TexelSize(base, list, glow);
    Stfs(base, c + 0, double(pass == 0 ? g.c33_x_first() : g.c33_x_second()));
    Stfs(base, c + 4, double(g.strength()));
    Stfs(base, c + 8, double(g.c33_z()));
    Stfs(base, c + 12, double(g.c33_w()));
    RecordPixelConstants(base, list, c, 33, 1);
    QuarterQuad(base, list, glow, quarter);
  }
  // The history (+8408) blended over it with shader pair 7, weighted by
  // +8460 (or 1 - 0 when +40, the reset, is set).
  Texture(base, list, HeaderOf(g.history()));
  Slot3(base, list, 1, 5, 0);
  for (const uint32_t state : {60u, 308u, 312u, 316u, 320u}) {
    Slot2(base, list, state, 1);
  }
  Slot3(base, list, 3, 1, 0);
  Slot2(base, list, 0, 0x10706);
  ShaderPair(base, list, 7);
  const uint32_t vertices =
      AddQuad(base, glow, g.width() >> 2, g.height() >> 2, Lfs(base, kOne), false);
  const bool reset = g.reset() != 0;
  const schema::PostEffectQuad quad(base, vertices);
  quad.set_v0_r(float(quarter));
  quad.set_v0_g(float(quarter));
  quad.set_v0_b(float(quarter));
  quad.set_v1_r(float(quarter));
  quad.set_v1_g(float(quarter));
  quad.set_v1_b(float(quarter));
  // The alphas: the history's weight, read for each.
  quad.set_v0_a(float(reset ? zero : double(g.history_weight())));
  quad.set_v1_a(float(reset ? zero : double(g.history_weight())));
  quad.set_v2_a(float(reset ? zero : double(g.history_weight())));
  quad.set_v2_b(float(quarter));
  quad.set_v2_g(float(quarter));
  quad.set_v2_r(float(quarter));
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
  if (g.updated() != 0 && g.no_history() == 0) {
    RecordResolve(base, glow, list, HeaderOf(g.history()), 0, 0xFFFFFFFDu, false);
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
  Slot2(base, list, 0, HeaderOf(g.frame_texture()));
  Slot2(base, list, 1, HeaderOf(g.final_texture_1()));
  Slot2(base, list, 2, HeaderOf(g.star_texture()));
  ShaderPair(base, list, 21);
  Quad(base, list, glow, quarter, true);
  RecordEndStates(base, list);
  Slot3(base, list, 36, 0, 0);
  command_list::CloseChunk(base, list);
  const uint32_t r3 = r.Submit(header, g.sort_key(), zero, 0x821384E0, 0x821384F0, 0x8213850C);
  g.set_reset(0);
  return r3;
}

}  // namespace post_effects
