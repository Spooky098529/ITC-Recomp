// CarSound::Update3D natively (car_sounds.h, docs/functions/car-sounds.md).
// Built with strict floating point and no contraction (CMakeLists.txt), as
// x3daudio.cpp, so every rounding below is the one written and the vector
// unit's flush to zero stays around its four operations.

#include "car_sounds/car_sounds.h"

#include "base/log.h"
#include "guest.h"
#include "schema/car_sounds.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

#include <bit>
#include <cmath>

namespace car_sounds {

namespace {

using Fpscr = ::base::FloatMode;

// The globals and constants Update3D reads.
constexpr uint32_t kRaceData = 0x826C5B2C;      // the race data: +0xA3AF0, +0xA3AF4
constexpr uint32_t kRaceKind = 0xA3AF0;         // 4: the camera pass may be the second
constexpr uint32_t kRaceMode = 0xA3AF4;         // 8: the side word below picks the car
constexpr uint32_t kViewports = 0x826C2CE8;     // +0 viewport 0 (+1860 its view mode), +8 the second
constexpr uint32_t kCameraPasses = 0x826BAC00;  // 592 bytes each
constexpr uint32_t kSideWords = 0x826C7820;     // +4: which of the vehicle's +6148 bits
constexpr uint32_t kVolume = schema::CarSoundsGlobals::effects_volume_address();  // f30
constexpr uint32_t kZero = 0x82000A34;          // f31
constexpr uint32_t kTicks = 0x82001FD8;         // the velocities' first factor (splat)
constexpr uint32_t kVelocityScale = 0x820021E4; // their second
constexpr uint32_t kOwnShare = 0x82002A10;      // the output map's share for the car at the camera
constexpr uint32_t kCloseSquared = 0x82002568;  // the passing check's squared distance
constexpr uint32_t kDopplerDrop = 0x8200806C;   // the Doppler factor's fall
constexpr uint32_t kDopplerOne = 0x82007FC8;    // the Doppler factor it falls through
constexpr uint64_t kHandle =  // lis/addi: sign-extended
    0xFFFFFFFF00000000ull | schema::CarSoundsGlobals::x3daudio_address();
constexpr uint64_t kSideWords64 = 0xFFFFFFFF826C7820ull;
constexpr uint32_t kCalculateReturn = 0x8210FA64;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// lfs: a guest float widened.
double Lfs(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
// stfs: a double rounded to a guest float.
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
// lfs then stfs of one value: its bits (the translated code's float to
// double and back folds away, signaling NaNs included).
uint32_t Word(uint8_t* base, uint32_t address) { return Load(base, address); }
double Fmuls(double a, double b) { return double(float(a * b)); }
double Fsubs(double a, double b) { return double(float(a - b)); }
double Fmadds(double a, double b, double c) { return double(float(std::fma(a, b, c))); }
void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
uint64_t DoubleBits(double value) { return std::bit_cast<uint64_t>(value); }

// lvx128 and stvx as the translation does them: the 16 bytes at the address
// rounded down, word 0 in the top lane.
simde__m128 LoadLanes(uint8_t* base, uint32_t address) {
  const uint32_t a = address & ~0xFu;
  return simde_mm_castsi128_ps(
      simde_mm_set_epi32(int(Load(base, a + 0)), int(Load(base, a + 4)), int(Load(base, a + 8)),
                         int(Load(base, a + 12))));
}
void StoreLanes(uint8_t* base, uint32_t address, simde__m128 v) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  const uint32_t a = address & ~0xFu;
  Store(base, a + 0, lanes[3]);
  Store(base, a + 4, lanes[2]);
  Store(base, a + 8, lanes[1]);
  Store(base, a + 12, lanes[0]);
}
// stvx of a register as the SDK's context held it (host byte order: guest byte j
// is host byte 15 - j).
void StoreRegister(uint8_t* base, uint32_t address, const uint8_t (&v)[16]) {
  const uint32_t a = address & ~0xFu;
  for (uint32_t j = 0; j < 16; j++) {
    guest::StoreU8(base, a + j, v[15 - j]);
  }
}
// lvlx then vspltw 0: the bytes from the address to its 16-byte block's end
// (zero past it), word 0 in every lane.
simde__m128 SplatLeft(uint8_t* base, uint32_t address) {
  const uint32_t offset = address & 0xF;
  uint32_t word = 0;
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t byte = offset + k < 16 ? guest::LoadU8(base, address + k) : 0;
    word |= byte << (24 - 8 * k);
  }
  return simde_mm_castsi128_ps(simde_mm_set1_epi32(int(word)));
}

// vsubfp128 and vmulfp128: flush to zero on (the translated code's
// enableFlushModeUnconditional), the operands in its order.
simde__m128 VectorSub(simde__m128 a, simde__m128 b) {
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  vector_library::PinInMode(a, b);
  simde__m128 r = simde_mm_sub_ps(a, b);
  vector_library::PinInMode(r);
  Fpscr::setcsr(csr);
  return r;
}
simde__m128 VectorMul(simde__m128 a, simde__m128 b) {
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  vector_library::PinInMode(a, b);
  simde__m128 r = simde_mm_mul_ps(a, b);
  vector_library::PinInMode(r);
  Fpscr::setcsr(csr);
  return r;
}

}  // namespace

uint64_t Update3D(uint8_t* base, uint64_t sound64, uint64_t r4, uint64_t vehicle64, uint32_t sp,
                  const Update3DSaves& s, const x3daudio::TanCall& tan) {
  ITC_NATIVE_RUNNING("sub_8210F820 CarSound::Update3D");
  const uint32_t sound = uint32_t(sound64);
  const uint32_t vehicle = uint32_t(vehicle64);
  const schema::CarSound cs(base, sound);
  const schema::X3DAudioListener l(base, cs.listener_address());
  const schema::X3DAudioEmitter e(base, cs.emitter_address());
  const schema::X3DAudioDspSettings dsp(base, cs.dsp_address());
  // The prologue: __savegprlr_22 (r22 to r31 at sp - 88 to - 16, lr at
  // - 8), f30 and f31 at - 104 and - 96, v126 and v127 at - 144 and - 128
  // (rounded down to 16), the back chain of the 288-byte frame.
  const uint64_t gprs[10] = {s.r22, s.r23, s.r24, s.r25, s.r26,
                             s.r27, s.r28, s.r29, s.r30, s.r31};
  for (uint32_t i = 0; i < 10; i++) {
    StoreU64(base, sp - 88 + 8 * i, gprs[i]);
  }
  Store(base, sp - 8, s.lr);
  StoreU64(base, sp - 104, s.f30);
  StoreU64(base, sp - 96, s.f31);
  StoreRegister(base, sp - 144, s.v126);
  StoreRegister(base, sp - 128, s.v127);
  const uint32_t frame = sp - 288;
  Store(base, frame, sp);

  // The race data's kind and the viewports pick the camera pass.
  const uint32_t race = Load(base, kRaceData);
  const uint32_t kind = Load(base, race + kRaceKind);
  const uint32_t viewports = Load(base, kViewports);
  uint64_t camera64 = 0xFFFFFFFF00000000ull | kCameraPasses;
  if (kind == 4 && Load(base, viewports + 8) != 0) {
    camera64 += 592;
  }
  const uint32_t camera = uint32_t(camera64);
  const uint32_t viewport = Load(base, viewports + 0);
  const uint32_t emitter_at = cs.emitter_position_x_address();    // r24
  const uint32_t listener_at = cs.listener_position_x_address();  // r26
  const double volume = Lfs(base, kVolume);  // f30
  const uint32_t view_mode = Load(base, viewport + 1860);  // r23
  const simde__m128 old_emitter = LoadLanes(base, emitter_at);   // v127
  const simde__m128 old_listener = LoadLanes(base, listener_at); // v126
  const uint32_t zero_word = Word(base, kZero);
  const double zero = Lfs(base, kZero);  // f31
  Store(base, frame + 108, zero_word);
  const uint32_t mode = Load(base, race + kRaceMode);  // r29
  const bool own_view = int32_t(view_mode) == 1 && uint32_t(r4) == 1;

  // Whether the car is the one at the camera (its emitter put there, and its
  // output map a fixed share): in mode 8 by the side word's bit of the
  // vehicle's +6148 (0x1 when it's set, 0x80 when it's 0).
  auto at_camera = [&] {
    if (mode == 8) {
      const bool side_zero = Load(base, kSideWords + 4) == 0;
      if (!own_view) {
        return false;
      }
      if (!side_zero && (Load(base, vehicle + 6148) & 0x1) != 0) {
        return true;
      }
      return side_zero && ((Load(base, vehicle + 6148) >> 7) & 1) != 0;
    }
    return own_view;
  };

  if (at_camera()) {
    for (uint32_t i = 0; i < 12; i += 4) {
      Store(base, frame + 96 + i, Word(base, camera + 400 + i));
    }
  } else {
    const uint32_t body = Load(base, cs.vehicle<schema::View>().address + 336);
    vector_library::Vec4Copy(base, frame + 96, vehicle_accessors::RigidBodyPosition(body));
  }

  // The emitter's position (+368) from the frame, the listener's (+384) the
  // camera pass's +400.
  const uint32_t ez = Word(base, frame + 104);
  const uint32_t ex = Word(base, frame + 96);
  const uint32_t ey = Word(base, frame + 100);
  Store(base, cs.emitter_position_z_address(), ez);
  Store(base, emitter_at, ex);
  Store(base, cs.emitter_position_y_address(), ey);
  Store(base, listener_at, Word(base, camera + 400));
  Store(base, cs.listener_position_y_address(), Word(base, camera + 404));
  Store(base, cs.listener_position_z_address(), Word(base, camera + 408));
  const simde__m128 listener_moved = VectorSub(LoadLanes(base, listener_at), old_listener);
  // The listener's front: the camera pass's +464 and +472, y 0.
  Store(base, l.front_x_address(), Word(base, camera + 464));
  Store(base, l.front_y_address(), zero_word);
  const uint32_t front_z = Word(base, camera + 472);
  const uint32_t ticks = Word(base, kTicks);
  Store(base, frame + 80, ticks);
  Store(base, l.front_z_address(), front_z);
  const simde__m128 ticks_a = SplatLeft(base, frame + 80);
  const simde__m128 emitter_moved = VectorSub(LoadLanes(base, emitter_at), old_emitter);
  Store(base, frame + 80, ticks);
  const simde__m128 ticks_b = SplatLeft(base, frame + 80);
  const uint32_t lx = Word(base, listener_at);
  const simde__m128 listener_velocity = VectorMul(listener_moved, ticks_a);
  // The listener's position (+28) and the emitter's (+80).
  Store(base, e.position_z_address(), ez);
  Store(base, l.position_x_address(), lx);
  const uint32_t ly = Word(base, cs.listener_position_y_address());
  const uint32_t lz = Word(base, cs.listener_position_z_address());
  Store(base, l.position_y_address(), ly);
  Store(base, l.position_z_address(), lz);
  Store(base, e.position_x_address(), ex);
  Store(base, e.position_y_address(), ey);
  const simde__m128 emitter_velocity = VectorMul(emitter_moved, ticks_b);
  StoreLanes(base, frame + 112, listener_velocity);
  StoreLanes(base, frame + 128, emitter_velocity);
  // The velocities (listener +40, emitter +92) times the second factor.
  const double scale = Lfs(base, kVelocityScale);
  l.set_velocity_x(float(Fmuls(Lfs(base, frame + 112), scale)));
  l.set_velocity_y(float(Fmuls(Lfs(base, frame + 116), scale)));
  l.set_velocity_z(float(Fmuls(Lfs(base, frame + 120), scale)));
  e.set_velocity_x(float(Fmuls(Lfs(base, frame + 128), scale)));
  e.set_velocity_y(float(Fmuls(Lfs(base, frame + 132), scale)));
  e.set_velocity_z(float(Fmuls(Lfs(base, frame + 136), scale)));

  // X3DAudioCalculate (flags 0x3D: the matrix, both low-pass filters, the
  // reverb, Doppler) into the DSP settings at +176, its frame below this
  // one; never empty for these flags.
  const auto whole = [&](uint32_t at) { return sound64 + (at - sound); };
  const x3daudio::CalculateArgs args{kHandle, whole(cs.listener_address()),
                                     whole(cs.emitter_address()), 61, whole(cs.dsp_address())};
  const x3daudio::CalculateSaves saves{kCalculateReturn,
                                       whole(listener_at),
                                       kSideWords64,
                                       whole(cs.dsp_address()),
                                       uint64_t(mode),
                                       camera64,
                                       sound64,
                                       s.f23,
                                       s.f24,
                                       s.f25,
                                       s.f26,
                                       s.f27,
                                       s.f28,
                                       s.f29,
                                       DoubleBits(volume),
                                       DoubleBits(zero)};
  const uint64_t r3 = x3daudio::Calculate(base, args, frame, saves, tan).value_or(0);

  // The output map at +272: six {channel, gain} pairs, the matrix's
  // coefficients times the volume, or the fixed share for the car at the
  // camera.
  if (at_camera()) {
    const double share = Fmuls(volume, Lfs(base, kOwnShare));
    for (uint32_t i = 0; i < 6; i++) {
      Stfs(base, cs.output_map_address() + 8 * i + 4, share);
      guest::StoreU8(base, cs.output_map_address() + 8 * i, uint8_t(i));
    }
  } else {
    for (uint32_t i = 0; i < 6; i++) {
      guest::StoreU8(base, cs.output_map_address() + 8 * i, uint8_t(i));
      const uint32_t matrix = dsp.matrix().address;
      Stfs(base, cs.output_map_address() + 8 * i + 4, Fmuls(Lfs(base, matrix + 4 * i), volume));
    }
  }
  cs.set_output_map_pointer(cs.output_map_address());
  cs.set_sends_0(cs.output_map_count_address());
  cs.set_output_map_count(6);
  cs.set_sends_0_kind(0);
  // The second map at +224: the reverb level (+200) times the volume.
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU8(base, cs.reverb_map_address() + 8 * i, uint8_t(i));
    Stfs(base, cs.reverb_map_address() + 8 * i + 4, Fmuls(double(dsp.reverb_level()), volume));
  }
  cs.set_output_sends(cs.sends_0_kind_address());
  cs.set_reverb_map_count(6);
  cs.set_reverb_map_pointer(cs.reverb_map_address());
  cs.set_sends_1_kind(1);
  cs.set_output_count(2);
  cs.set_passed_close(0);
  cs.set_sends_1(cs.reverb_map_count_address());

  // Passed close (+1260): view mode 1 for another car, within the squared
  // distance, with the Doppler factor (+204) fallen through 1 since the last
  // tick (+1256).
  if (int32_t(view_mode) == 1 && uint32_t(r4) != 1) {
    const double dy = Fsubs(double(cs.listener_position_y()), double(cs.emitter_position_y()));
    const double lx2 = Lfs(base, listener_at);
    const double dx = Fsubs(lx2, Lfs(base, emitter_at));
    const double ez2 = double(cs.emitter_position_z());
    const double dz = Fsubs(double(cs.listener_position_z()), ez2);
    double d2 = Fmuls(dy, dy);
    d2 = Fmadds(dx, dx, d2);
    const double close = Lfs(base, kCloseSquared);
    d2 = Fmadds(dz, dz, d2);
    if (d2 < close) {
      const double last = double(cs.last_doppler());
      const double now = double(dsp.doppler_factor());
      const double fall = Fsubs(last, now);
      if (fall > Lfs(base, kDopplerDrop)) {
        const double one = Lfs(base, kDopplerOne);
        if (last > one && now < one) {
          cs.set_passed_close(1);
        }
      }
    }
  }
  Store(base, cs.last_doppler_address(), Word(base, dsp.doppler_factor_address()));
  return r3;
}

}  // namespace car_sounds
