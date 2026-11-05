// The post effects' records natively (post_effects.h,
// docs/functions/post-effects.md): ColorFilter::Record and
// TargetFilter::Record. Step for step as the recompiled functions: their
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
#include "command_list/command_list.h"
#include "models/model.h"

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
  // The chunk handed to the render queue (its slot 4: r4 = the chunk, r5 =
  // the key, f1); its r3.
  uint32_t Submit(uint32_t chunk, uint32_t key, double f1, uint32_t lr1, uint32_t lr2,
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

}  // namespace post_effects
