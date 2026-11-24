// CarSound::Update3D natively (car_sounds.h, docs/functions/car-sounds.md).
// Built with strict floating point and no contraction (CMakeLists.txt), as
// x3daudio.cpp, so every rounding below is the one written and the vector
// unit's flush to zero stays around its four operations.

#include "car_sounds/car_sounds.h"

#include "base/log.h"
#include "guest.h"
#include "schema/camera_modes.h"
#include "schema/cameras.h"
#include "schema/car_physics.h"
#include "schema/car_sounds.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/vehicle_accessors.h"
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
// The race data's kind (4: the camera pass may be the second) and mode (8:
// the human flag picks the car); the viewports (the first's view mode, the
// split word picks the second camera pass); the human flag (0x826C7820's
// +4): which of the vehicle's car flags.
constexpr uint32_t kCameraPasses = schema::CamerasGlobals::passes_address();  // 592 bytes each
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
constexpr uint64_t kSideWords64 =  // lis/addi: sign-extended
    0xFFFFFFFF00000000ull | schema::RaceWorldGlobals::start_object_address();
constexpr uint32_t kCalculateReturn = 0x8210FA64;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// lfs: a guest float widened.
double Lfs(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
// lfs then stfs of one value: its bits (the translated code's float to
// double and back folds away, signaling NaNs included).
uint32_t Word(uint8_t* base, uint32_t address) { return Load(base, address); }
double Fmuls(double a, double b) { return double(float(a * b)); }
double Fsubs(double a, double b) { return double(float(a - b)); }
double Fmadds(double a, double b, double c) { return double(float(std::fma(a, b, c))); }
uint64_t DoubleBits(double value) { return std::bit_cast<uint64_t>(value); }

// The car sound's words (in host memory since E5 step 4): read and written
// through its view at their offsets, as the guest's bytes give them.
uint32_t WordIn(const schema::CarSound& cs, uint32_t address) {
  return uint32_t(cs.load_at(address - cs.address, 4));
}
void StoreIn(const schema::CarSound& cs, uint32_t address, uint32_t value) {
  cs.store_at(address - cs.address, 4, value);
}
void StoreU8In(const schema::CarSound& cs, uint32_t address, uint8_t value) {
  cs.store_at(address - cs.address, 1, value);
}
double LfsIn(const schema::CarSound& cs, uint32_t address) {
  return double(std::bit_cast<float>(WordIn(cs, address)));
}
void StfsIn(const schema::CarSound& cs, uint32_t address, double value) {
  StoreIn(cs, address, std::bit_cast<uint32_t>(float(value)));
}
// LoadLanes over the car sound's words.
simde__m128 LoadLanesIn(const schema::CarSound& cs, uint32_t address) {
  const uint32_t a = address & ~0xFu;
  return simde_mm_castsi128_ps(simde_mm_set_epi32(int(WordIn(cs, a + 0)), int(WordIn(cs, a + 4)),
                                                  int(WordIn(cs, a + 8)), int(WordIn(cs, a + 12))));
}
// lvlx then vspltw 0 of `value` as the original stored it at `address`
// (a frame slot, a value since E5 step 6): its bytes up to the address's
// 16-byte block's end (zero past it), word 0 in every lane.
simde__m128 SplatLeft(uint32_t value, uint32_t address) {
  const uint32_t offset = address & 0xF;
  uint32_t word = 0;
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t byte = offset + k < 16 ? (value >> (24 - 8 * k)) & 0xFF : 0;
    word |= byte << (24 - 8 * k);
  }
  return simde_mm_castsi128_ps(simde_mm_set1_epi32(int(word)));
}
// A lane of `v` as stvx stored it (word 0 the top lane), read back by lfs.
double LaneSingle(simde__m128 v, uint32_t word) {
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), simde_mm_castps_si128(v));
  return double(std::bit_cast<float>(lanes[3 - word]));
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
  // The back chain of the original's 288-byte frame, below which
  // X3DAudioCalculate makes its own (E5 step 6: the prologue's saves, read
  // by nothing, are no longer stored; the frame's own slots, +80 to +136,
  // are values).
  const uint32_t frame = sp - 288;
  Store(base, frame, sp);

  // The race data's kind and the viewports pick the camera pass.
  const schema::RaceData race = schema::RaceStatesGlobals::race_data(base);
  const uint32_t kind = race.kind();
  const schema::Viewports viewports = schema::CameraModesGlobals::viewports(base);
  uint64_t camera64 = 0xFFFFFFFF00000000ull | kCameraPasses;
  if (kind == 4 && viewports.split() != 0) {
    camera64 += 592;
  }
  const schema::CameraPass camera(base, uint32_t(camera64));
  const schema::Viewport viewport = viewports.first();
  const uint32_t emitter_at = cs.emitter_position_x_address();    // r24
  const uint32_t listener_at = cs.listener_position_x_address();  // r26
  const double volume = Lfs(base, kVolume);  // f30
  const uint32_t view_mode = viewport.view_mode();  // r23
  const simde__m128 old_emitter = LoadLanesIn(cs, emitter_at);   // v127
  const simde__m128 old_listener = LoadLanesIn(cs, listener_at); // v126
  const uint32_t zero_word = Word(base, kZero);
  const double zero = Lfs(base, kZero);  // f31
  const uint32_t mode = race.race_mode();  // r29
  const bool own_view = int32_t(view_mode) == 1 && uint32_t(r4) == 1;

  // Whether the car is the one at the camera (its emitter put there, and its
  // output map a fixed share): in mode 8 by the side word's bit of the
  // vehicle's +6148 (0x1 when it's set, 0x80 when it's 0).
  auto at_camera = [&] {
    if (mode == 8) {
      const bool side_zero = schema::RaceWorldGlobals::human_flag(base) == 0;
      if (!own_view) {
        return false;
      }
      if (!side_zero && (schema::Vehicle(base, vehicle).car_flags() & 0x1) != 0) {
        return true;
      }
      return side_zero && ((schema::Vehicle(base, vehicle).car_flags() >> 7) & 1) != 0;
    }
    return own_view;
  };

  // The emitter's position (the original's frame +96 to +104): the camera
  // pass's eye, or the body's position (Vec4::Copy's x, y and z).
  uint32_t ex, ey, ez;
  if (at_camera()) {
    ex = Word(base, camera.eye_x_address());
    ey = Word(base, camera.eye_x_address() + 4);
    ez = Word(base, camera.eye_x_address() + 8);
  } else {
    const uint32_t body = cs.vehicle().body<schema::View>().address;
    const schema::VectorLibVector position(base, vehicle_accessors::RigidBodyPosition(body));
    ex = position.x_bits();
    ey = position.y_bits();
    ez = position.z_bits();
  }

  // The emitter's position (+368), the listener's (+384) the camera pass's
  // +400.
  StoreIn(cs, cs.emitter_position_z_address(), ez);
  StoreIn(cs, emitter_at, ex);
  StoreIn(cs, cs.emitter_position_y_address(), ey);
  StoreIn(cs, listener_at, Word(base, camera.eye_x_address()));
  StoreIn(cs, cs.listener_position_y_address(), Word(base, camera.eye_y_address()));
  StoreIn(cs, cs.listener_position_z_address(), Word(base, camera.eye_z_address()));
  const simde__m128 listener_moved = VectorSub(LoadLanesIn(cs, listener_at), old_listener);
  // The listener's front: the camera pass's +464 and +472, y 0.
  StoreIn(cs, l.front_x_address(), Word(base, camera.forward_x_address()));
  StoreIn(cs, l.front_y_address(), zero_word);
  const uint32_t front_z = Word(base, camera.forward_z_address());
  // The factor splat from the frame's +80, as the original stored it there.
  const uint32_t ticks = Word(base, kTicks);
  StoreIn(cs, l.front_z_address(), front_z);
  const simde__m128 ticks_a = SplatLeft(ticks, frame + 80);
  const simde__m128 emitter_moved = VectorSub(LoadLanesIn(cs, emitter_at), old_emitter);
  const simde__m128 ticks_b = SplatLeft(ticks, frame + 80);
  const uint32_t lx = WordIn(cs, listener_at);
  const simde__m128 listener_velocity = VectorMul(listener_moved, ticks_a);
  // The listener's position (+28) and the emitter's (+80).
  StoreIn(cs, e.position_z_address(), ez);
  StoreIn(cs, l.position_x_address(), lx);
  const uint32_t ly = WordIn(cs, cs.listener_position_y_address());
  const uint32_t lz = WordIn(cs, cs.listener_position_z_address());
  StoreIn(cs, l.position_y_address(), ly);
  StoreIn(cs, l.position_z_address(), lz);
  StoreIn(cs, e.position_x_address(), ex);
  StoreIn(cs, e.position_y_address(), ey);
  const simde__m128 emitter_velocity = VectorMul(emitter_moved, ticks_b);
  // The velocities (listener +40, emitter +92; the original's frame +112
  // and +128, 16-byte aligned as the stack pointer is) times the second
  // factor.
  const double scale = Lfs(base, kVelocityScale);
  l.set_velocity_x(float(Fmuls(LaneSingle(listener_velocity, 0), scale)));
  l.set_velocity_y(float(Fmuls(LaneSingle(listener_velocity, 1), scale)));
  l.set_velocity_z(float(Fmuls(LaneSingle(listener_velocity, 2), scale)));
  e.set_velocity_x(float(Fmuls(LaneSingle(emitter_velocity, 0), scale)));
  e.set_velocity_y(float(Fmuls(LaneSingle(emitter_velocity, 1), scale)));
  e.set_velocity_z(float(Fmuls(LaneSingle(emitter_velocity, 2), scale)));

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
      StfsIn(cs, cs.output_map_address() + 8 * i + 4, share);
      StoreU8In(cs, cs.output_map_address() + 8 * i, uint8_t(i));
    }
  } else {
    for (uint32_t i = 0; i < 6; i++) {
      StoreU8In(cs, cs.output_map_address() + 8 * i, uint8_t(i));
      const uint32_t matrix = dsp.matrix().address;
      StfsIn(cs, cs.output_map_address() + 8 * i + 4, Fmuls(Lfs(base, matrix + 4 * i), volume));
    }
  }
  cs.set_output_map_pointer(cs.output_map_address());
  cs.set_sends_0(cs.output_map_count_address());
  cs.set_output_map_count(6);
  cs.set_sends_0_kind(0);
  // The second map at +224: the reverb level (+200) times the volume.
  for (uint32_t i = 0; i < 6; i++) {
    StoreU8In(cs, cs.reverb_map_address() + 8 * i, uint8_t(i));
    StfsIn(cs, cs.reverb_map_address() + 8 * i + 4, Fmuls(double(dsp.reverb_level()), volume));
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
    const double lx2 = LfsIn(cs, listener_at);
    const double dx = Fsubs(lx2, LfsIn(cs, emitter_at));
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
  StoreIn(cs, cs.last_doppler_address(), WordIn(cs, dsp.doppler_factor_address()));
  return r3;
}

}  // namespace car_sounds
