// The car sounds' parts natively (car_sounds.h, docs/functions/car-sounds.md):
// the hits, the parts at +736, +840 and +888, the wind, the pass by, the
// engine with its two layers, the tires with their layer, the roll, the horn
// and the turbo, each from CarSound::Update every tick. Their callees' plain C++ called directly
// (PlayOnce, IsPlaying, UpdateVoices, XAudio's SetVolume and Stop, the
// vehicle's accessors), each in a frame below the caller's, with the
// registers set as the original sets them before the call. Strict floating
// point (CMakeLists.txt), as the original's single-precision steps.

#include "car_sounds/car_sounds.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "guest.h"
#include "hooks/vehicle.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cmath>
#include <cstdint>
#include <initializer_list>

namespace car_sounds {

namespace {

namespace xa = xaudio;
namespace va = vehicle_accessors;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
double LoadF(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
// stfs: the register rounded to single.
void StoreF(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
// A single-precision step's result, as the register holds it.
double Single(double value) { return double(float(value)); }
// fmadds: the fused product and sum, rounded once to single.
double Fmadds(double a, double b, double c) { return double(float(std::fma(a, b, c))); }

// fctidz's low word, as the translation leaves it (stfiwx).
uint32_t FctidzWord(double value) {
  if (std::isnan(value)) {
    return 0;
  }
  if (value > 9223372036854775808.0) {
    return 0xFFFFFFFFu;  // LLONG_MAX
  }
  if (value >= 9223372036854775808.0 || value < -9223372036854775808.0) {  // indefinite
    return 0;
  }
  return uint32_t(uint64_t(int64_t(value)));
}
// fctidz's whole word, as the translation leaves it (stfd).
uint64_t Fctidz64(double value) {
  if (std::isnan(value) || value >= 9223372036854775808.0 ||
      value < -9223372036854775808.0) {
    return value > 9223372036854775808.0 ? uint64_t(INT64_MAX) : 0x8000000000000000ull;
  }
  return uint64_t(int64_t(value));
}
// fctiwz's low word, as the translation leaves it (stfiwx).
uint32_t FctiwzWord(double value) {
  if (std::isnan(value)) {
    return 0x80000000u;
  }
  if (value >= double(INT32_MAX)) {
    return uint32_t(INT32_MAX);
  }
  if (value <= -2147483649.0) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(value));
}

xa::Regs Args(std::initializer_list<uint64_t> r) {
  xa::Regs regs;
  for (const uint64_t value : r) {
    regs.r[regs.count++] = value;
  }
  return regs;
}
xa::Regs ArgsF1(std::initializer_list<uint64_t> r, double f1) {
  xa::Regs regs = Args(r);
  regs.f1_set = true;
  regs.f1 = f1;
  return regs;
}
void SetRegs(const GuestCall& call, xa::Regs regs) { call(0, 0, regs); }
// f1 as a leaf's plain C++ leaves it (its result), for the guest calls after.
void SetF1(const GuestCall& call, double f1) { SetRegs(call, ArgsF1({}, f1)); }

// The voice in `slot` (the original's (slot + 1) x 4 from the holder).
uint32_t VoiceAt(uint8_t* base, uint32_t holder, uint32_t slot) {
  return Load(base, holder + ((slot + 1) << 2));
}

// The frames (their originals' sizes).
constexpr uint32_t kApiFrame = 112;
constexpr uint32_t kPlayOnceFrame = 128;
constexpr uint32_t kIsPlayingFrame = 112;
constexpr uint32_t kUpdateVoicesFrame = 112;

// The constants.
constexpr uint32_t kZero = 0x82000A34;        // 0.0
constexpr uint32_t kOne = 0x82007FC8;         // 1.0
constexpr uint32_t kVolume = 0x82007FEC;      // the one-shots' volume scale
constexpr uint32_t kPitch = 0x8200803C;       // the one-shots' pitch
constexpr uint32_t kLevelScale = 0x82008054;  // strength to the wave's level
constexpr uint32_t kLevelMax = 0x820021E8;
constexpr uint32_t kAmountScale = 0x82008050;
constexpr uint32_t kVolumeSlope = 0x8200804C;
constexpr uint32_t kVolumeBase = 0x820029CC;
constexpr uint32_t kVolumeScale = 0x82008028;
constexpr uint32_t kHitHigh = 0x82002A0C;
constexpr uint32_t kHitLow = 0x82001FC0;
constexpr uint32_t kCounterScale = 0x82008000;

// PlayOnce (r3 the holder, r4 its view +40, r5 the wave, r8 `loop`, f1 the
// volume, f2 the pitch) in a frame below `frame`.
uint32_t Play(uint8_t* base, uint32_t holder, uint32_t wave, uint32_t loop, double volume,
              double pitch, const Voices& voices, uint32_t frame, const GuestCall& call) {
  const uint32_t view = Load(base, holder + 40);
  SetRegs(call, ArgsF1({holder, view, wave}, volume));
  return PlayOnce(base, holder, view, wave, loop, volume, pitch, voices,
                  xa::FrameBelow(base, frame, kPlayOnceFrame), call);
}

uint32_t Update(uint8_t* base, uint32_t holder, uint64_t output, uint32_t frame,
                const GuestCall& call) {
  SetRegs(call, Args({holder, output}));
  return UpdateVoices(base, holder, output, xa::FrameBelow(base, frame, kUpdateVoicesFrame), call);
}

uint32_t Playing(uint8_t* base, uint32_t holder, uint32_t slot, uint32_t frame,
                 const GuestCall& call) {
  SetRegs(call, Args({holder, slot}));
  return IsPlaying(base, holder, slot, xa::FrameBelow(base, frame, kIsPlayingFrame), call);
}

uint32_t SetVolume(uint8_t* base, uint32_t voice, double volume, uint32_t frame,
                   const GuestCall& call) {
  SetRegs(call, ArgsF1({voice}, volume));
  xa::FrameBelow(base, frame, kApiFrame);
  return xa::ApiSetVolume(base, voice, volume, call);
}

uint32_t StopVoice(uint8_t* base, uint32_t voice, uint32_t frame, const GuestCall& call) {
  SetRegs(call, Args({voice, 0}));
  xa::FrameBelow(base, frame, kApiFrame);
  return xa::ApiStop(base, voice, 0, 0, 0, 0, call);
}

// The hits' counter (+48 the ticks left) for a wave played at `level`
// (the fctidz word): +48 the level + 1 (as a 64-bit word at the frame's +80,
// back through fcfid) times [0x82008000].
void HitCounters(uint8_t* base, uint32_t part, uint32_t level, uint32_t frame) {
  const uint64_t next = uint64_t(level) + 1;
  Store64(base, frame + 80, next & 0xFFFFFFFFu);
  const double count = Single(double(int64_t(next & 0xFFFFFFFFu)));
  Store(base, part + 48, FctidzWord(Single(count * LoadF(base, kCounterScale))));
}

}  // namespace

uint32_t HitImpact(uint8_t* base, uint32_t part, uint32_t vehicle, const Voices& voices,
                   uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E778 HitSounds::Impact");
  if ((Load(base, vehicle + 6152) & 1) == 0) {
    return part;
  }
  uint32_t level = 0;   // r29
  uint64_t wave = 0;    // r28
  double strength = LoadF(base, kZero);  // f31
  Store(base, frame + 80, 0);
  uint32_t r3 = Load(base, vehicle + 340);
  if (r3 != 0) {
    SetRegs(call, Args({r3, 0x00FFFFFF}));
    r3 = va::CollisionObjectFirstContact(base, r3, 0x00FFFFFF);
    if (r3 != 0) {
      SetRegs(call, Args({r3}));
      strength = double(va::ContactStrength(base, r3));
      SetF1(call, strength);
      double f0 = Single(strength * LoadF(base, kLevelScale));
      const double max = LoadF(base, kLevelMax);
      if (f0 > max) {
        f0 = max;
      }
      level = FctidzWord(f0);
      Store(base, frame + 80, level);
      wave = uint64_t(level) + 7;
    }
  }
  double f0 = Single(strength * LoadF(base, kAmountScale));
  double amount = LoadF(base, kOne);  // f11
  if (!(f0 > amount)) {
    amount = f0;
  }
  const double high = LoadF(base, kHitHigh);
  f0 = Single(Fmadds(amount, LoadF(base, kVolumeSlope), LoadF(base, kVolumeBase)) *
              LoadF(base, kVolumeScale));
  const auto play = [&] {
    const double volume = Single(f0 * LoadF(base, kVolume));
    r3 = Play(base, part, uint32_t(wave), 0, volume, LoadF(base, kPitch), voices, frame, call);
    HitCounters(base, part, level, frame);
    Store(base, part + 60, FctiwzWord(strength));
    Store(base, part + 64, 0);
  };
  if (strength > high && int32_t(Load(base, part + 64)) == -1) {
    play();
  } else if (strength > LoadF(base, kHitLow) && int32_t(Load(base, part + 64)) >= 0) {
    play();
  } else if (strength > high) {
    Store(base, part + 64, 0);
  }
  return r3;
}

uint32_t HitScrape(uint8_t* base, uint32_t part, uint32_t vehicle, const Voices& voices,
                   uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E938 HitSounds::Scrape");
  const uint32_t flags = Load(base, vehicle + 6152);
  if ((flags & 1) != 0 || (flags & 2) == 0) {
    // Out of contact: the looping voice (+56) stopped once it has played
    // more than two ticks since (+52).
    uint32_t r3 = Playing(base, part, Load(base, part + 56), frame, call);
    if (r3 == 0) {
      return r3;
    }
    const uint32_t ticks = Load(base, part + 52) + 1;
    Store(base, part + 52, ticks);
    if (ticks <= 2) {
      return r3;
    }
    r3 = StopVoice(base, VoiceAt(base, part, Load(base, part + 56)), frame, call);
    Store(base, part + 56, 0xFFFFFFFFu);
    return r3;
  }
  double amount = LoadF(base, kZero);  // f27
  uint32_t r3 = Load(base, vehicle + 340);
  if (r3 != 0) {
    SetRegs(call, Args({r3, 0xFF000000}));
    r3 = va::CollisionObjectFirstContact(base, r3, 0xFF000000);
    if (r3 != 0) {
      SetRegs(call, Args({r3}));
      const double strength = double(va::ContactStrength(base, r3));
      r3 = vehicle;
      SetRegs(call, ArgsF1({vehicle}, strength));
      const double speed = double(Vehicle_GetSpeedKmh(base, vehicle));
      SetF1(call, speed);
      amount = Single(Single(speed * strength) * LoadF(base, 0x8200805C));
    }
  }
  const double scaled = Single(amount * LoadF(base, kAmountScale));  // f26
  const double one = LoadF(base, kOne);                              // f25
  const double slope = LoadF(base, kVolumeSlope);                    // f30
  const double pitch = LoadF(base, kPitch);                          // f23
  const double level_max = LoadF(base, kLevelMax);                   // f28
  const double vbase = LoadF(base, kVolumeBase);                     // f29
  const double low = LoadF(base, kHitLow);                           // f24
  const double clamped = scaled > one ? one : scaled;
  double volume = Single(Fmadds(clamped, slope, vbase) * LoadF(base, kVolumeScale));  // f31
  if (amount > LoadF(base, 0x82001FD8) && amount < low) {
    if (Playing(base, part, Load(base, part + 56), frame, call) != 0) {
      const double f0 = Single((scaled > one ? one : scaled) * level_max);
      const uint32_t voice = VoiceAt(base, part, Load(base, part + 56));
      volume = Single(Fmadds(f0, slope, vbase) * LoadF(base, 0x82008058));
      r3 = SetVolume(base, voice, Single(volume * LoadF(base, kVolume)), frame, call);
    } else {
      r3 = Play(base, part, 10, 1, Single(volume * LoadF(base, kVolume)), pitch, voices, frame,
                call);
      Store(base, part + 56, r3);
    }
    Store(base, part + 48, 4);
    Store(base, part + 52, 0);
  }
  if (amount > low) {
    double f0 = Single(amount * LoadF(base, kLevelScale));
    if (f0 > level_max) {
      f0 = level_max;
    }
    const uint32_t level = FctidzWord(f0);
    Store(base, frame + 80, level);
    r3 = Play(base, part, level + 7, 0, Single(volume * LoadF(base, kVolume)), pitch, voices, frame,
              call);
    HitCounters(base, part, level, frame);
  }
  return r3;
}

uint32_t HitWave9(uint8_t* base, uint32_t part, uint32_t vehicle, const Voices& voices,
                  uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210EB90 HitSounds::Wave9");
  if ((Load(base, vehicle + 6152) & 4) == 0) {
    return part;
  }
  const uint32_t r3 =
      Play(base, part, 9, 0, LoadF(base, kVolume), LoadF(base, kPitch), voices, frame, call);
  Store(base, part + 48, 32);
  return r3;
}

uint32_t HitSoundsUpdate(uint8_t* base, uint32_t part, uint64_t vehicle, uint64_t output,
                         const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210FD78 HitSounds::Update");
  const uint32_t ticks = Load(base, part + 48);
  if (ticks == 0) {
    const uint32_t v = uint32_t(vehicle);
    SetRegs(call, Args({part, vehicle, output}));
    HitImpact(base, part, v, voices, xa::FrameBelow(base, frame, 144), call);
    SetRegs(call, Args({part, vehicle}));
    HitScrape(base, part, v, voices, xa::FrameBelow(base, frame, 192), call);
    SetRegs(call, Args({part, vehicle}));
    HitWave9(base, part, v, voices, xa::FrameBelow(base, frame, 96), call);
  } else {
    Store(base, part + 48, ticks - 1);
  }
  int32_t since = int32_t(Load(base, part + 64));
  if (since >= 0) {
    Store(base, part + 64, uint32_t(since + 1));
  }
  since = int32_t(Load(base, part + 64));
  if (since > 32) {
    Store(base, part + 64, 0xFFFFFFFFu);
  }
  return Update(base, part, output, frame, call);
}

uint32_t Part736Update(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                       const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210EBF0 CarSound::Part736Update");
  if (Load(base, vehicle + 2252) == 0) {
    return part;
  }
  if (guest::LoadU8(base, vehicle + 2948) != 0) {
    const uint32_t specs = Load(base, vehicle + 5712);
    double scale;
    if (guest::LoadU8(base, specs + 2019) == 1) {
      scale = LoadF(base, 0x82007FC8);
    } else {
      const uint32_t kind = Load(base, specs + 1656);
      if (kind < 2) {
        scale = LoadF(base, 0x82007FD4);
      } else if (kind < 5) {
        scale = LoadF(base, 0x8200295C);
      } else {
        scale = LoadF(base, 0x82007FC8);
      }
    }
    if (Load(base, part + 48) == 0) {
      const double f13 = Single(LoadF(base, vehicle + 2952) * scale);
      const uint32_t slot = Play(base, part, 4, 0, Single(f13 * LoadF(base, 0x82002C50)),
                                 LoadF(base, kPitch), voices, frame, call);
      Store(base, part + 52, slot);
      Store(base, part + 48, 32);
    }
  } else {
    const uint32_t ticks = Load(base, part + 48);
    if (ticks != 0) {
      Store(base, part + 48, ticks - 1);
    }
  }
  return Update(base, part, output, frame, call);
}

uint32_t Part840Update(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                       const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210ECF0 CarSound::Part840Update");
  const double pitch = LoadF(base, kPitch);
  if (guest::LoadU8(base, vehicle + 2504) != 0) {
    Play(base, part, 6, 0, LoadF(base, kVolume), pitch, voices, frame, call);
  }
  if (guest::LoadU8(base, vehicle + 2960) != 0 && guest::LoadU8(base, part + 45) == 0) {
    Play(base, part, 18, 0, LoadF(base, kVolume), pitch, voices, frame, call);
    guest::StoreU8(base, part + 45, 2);
  }
  const uint8_t left = uint8_t(guest::LoadU8(base, part + 45) - 1);
  guest::StoreU8(base, part + 45, left);
  if (int8_t(left) < 0) {
    guest::StoreU8(base, part + 45, 0);
  }
  return Update(base, part, output, frame, call);
}

uint32_t Part888Update(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                       const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210EEE0 CarSound::Part888Update");
  const double full = LoadF(base, 0x82002C50);  // f31
  const double zero = LoadF(base, kZero);       // f30
  const double rest = LoadF(base, 0x82001BE0);  // f28
  const double step = LoadF(base, kOne);        // f29
  if (int32_t(Load(base, part + 48)) == 0) {
    if (guest::LoadU8(base, vehicle + 2424) != 0) {
      if (LoadF(base, part + 60) == rest) {
        StoreF(base, part + 60, LoadF(base, 0x82007FB8));
      }
      const uint32_t slot = Play(base, part, 17, 1, full, LoadF(base, kPitch), voices, frame, call);
      StoreF(base, part + 56, rest);
      Store(base, part + 52, slot);
      Store(base, part + 48, 1);
    }
  } else {
    double f0 = Single(LoadF(base, part + 60) - step);
    StoreF(base, part + 60, f0);
    if (f0 < zero) {
      StoreF(base, part + 60, zero);
    }
    const double f13 = Single(LoadF(base, part + 60) * LoadF(base, 0x820029EC));
    const uint32_t voice = VoiceAt(base, part, Load(base, part + 52));
    SetVolume(base, voice, Fmadds(f13, LoadF(base, 0x82007FD8), full), frame, call);
    if (guest::LoadU8(base, vehicle + 2424) == 0 && LoadF(base, part + 56) == rest) {
      StoreF(base, part + 56, LoadF(base, 0x820029D0));
    }
  }
  double f0 = LoadF(base, part + 56);
  if (!(f0 == rest)) {
    f0 = Single(f0 - step);
    StoreF(base, part + 56, f0);
    const double f13 = Single(f0 * LoadF(base, 0x82008060));
    const uint32_t voice = VoiceAt(base, part, Load(base, part + 52));
    SetVolume(base, voice, Single(f13 * full), frame, call);
    if (LoadF(base, part + 56) == zero) {
      if (Playing(base, part, Load(base, part + 52), frame, call) != 0) {
        StopVoice(base, VoiceAt(base, part, Load(base, part + 52)), frame, call);
      }
      StoreF(base, part + 56, rest);
      StoreF(base, part + 60, rest);
      Store(base, part + 48, 0);
    }
  }
  return Update(base, part, output, frame, call);
}

uint32_t WindSoundUpdate(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                         const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210EDB0 WindSound::Update");
  // The four halfwords at the vehicle's +5410 (96 bytes apart), bit 0.
  bool any = false;
  for (uint32_t i = 0; i < 4; i++) {
    if ((guest::LoadU16(base, vehicle + 5410 + 96 * i) & 1) != 0) {
      any = true;
      break;
    }
  }
  if (any) {
    if (int32_t(Load(base, part + 48)) == 0) {
      SetRegs(call, Args({vehicle}));
      const double first = double(Vehicle_GetSpeedKmh(base, vehicle));
      SetF1(call, first);
      if (first > LoadF(base, 0x8200257C)) {
        SetRegs(call, Args({vehicle}));
        const double speed = double(Vehicle_GetSpeedKmh(base, vehicle));
        SetF1(call, speed);
        double scale = LoadF(base, 0x820021E4);  // f12
        double f0 = Single(speed * LoadF(base, 0x82002A08));
        const double one = LoadF(base, kOne);
        if (f0 > one) {
          f0 = one;
        }
        const uint32_t view_mode = Load(base, Load(base, Load(base, 0x826C2CE8)) + 1860);
        if (int32_t(view_mode) == 1) {
          scale = LoadF(base, 0x820021F4);
        }
        const double f13 = Single(scale * f0);
        Play(base, part, 5, 0, Single(f13 * LoadF(base, 0x82001FBC)), LoadF(base, kPitch), voices,
             frame, call);
        Store(base, part + 48, 1);
      }
    }
  } else if (int32_t(Load(base, part + 48)) == 1) {
    Store(base, part + 48, 0);
  }
  return Update(base, part, output, frame, call);
}

uint32_t PassBySoundUpdate(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                           uint32_t passed, const Voices& voices, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210F088 PassBySound::Update");
  if (vehicle == 0) {
    return part;
  }
  if ((Load(base, vehicle + 6148) & 0x81) == 0 && int32_t(passed) != 0) {
    Play(base, part, 15, 0, LoadF(base, 0x82002A10), LoadF(base, kPitch), voices, frame, call);
  }
  return Update(base, part, output, frame, call);
}

// --- The engine (sub_8210E470 and its two layers) ---

namespace {

constexpr uint32_t kLayerUpdateFrame = 272;
constexpr uint32_t kPitchedFrame = 144;
constexpr uint32_t kSetPitchScaledFrame = 96;

constexpr uint32_t kSpeedCap = 0x82007FC4;     // the engine's speed's cap
constexpr uint32_t kCurveEnd = 0x82001BE0;     // a curve's end (x at most it)
constexpr uint32_t kLayerVolume = 0x82008028;  // the table's word to a volume
constexpr uint32_t kLayerGain = 0x82007FD4;
constexpr uint32_t kEngineRest = 0x82003384;  // the engine's +172 without a vehicle

double LoadDouble(uint8_t* base, uint32_t address) {
  const uint64_t bits = (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
  return std::bit_cast<double>(bits);
}

}  // namespace

uint32_t EngineLayerUpdate(uint8_t* base, uint32_t layer, double speed, double volume,
                           double pitch, double gain, uint64_t output, const Voices& voices,
                           uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D478 EngineLayer::Update");
  const uint32_t table = Load(base, layer + 4);  // r25
  uint32_t out = layer + 20;                     // r22
  if (table == 0) {
    return 0;
  }
  {
    const double cap = LoadF(base, kSpeedCap);
    if (speed > cap) {
      speed = cap;
    }
  }
  uint32_t entry = table + 4;  // r29
  if (!(int32_t(Load(base, entry)) > -1)) {
    return 0;
  }
  const uint32_t holder = layer + 44;          // r26
  uint32_t slot_at = layer + 48;               // r30
  const double scale = LoadF(base, kLayerVolume);  // f26
  const double end = LoadF(base, kCurveEnd);       // f29
  const double zero = LoadF(base, kZero);          // f27
  uint32_t slot = 0;                               // r28
  do {
    const uint32_t second = entry + 72;  // r31
    double amount = zero;                // f31
    double rate = zero;                  // f30
    // The first curve (entry + 8): the volume's amount at the speed, stored
    // at the layer's next float.
    {
      uint32_t r11 = second - 64, r10 = r11;
      bool found = false;
      for (;;) {
        const double x = LoadF(base, r11);
        if (!(x > end)) {
          break;
        }
        if (!(speed > x)) {
          found = true;
          break;
        }
        r10 = r11;
        r11 += 8;
      }
      if (found) {
        if (r11 == r10) {
          amount = LoadF(base, r11 + 4);
        } else {
          const double x0 = LoadF(base, r10), x1 = LoadF(base, r11);
          const double dx = Single(speed - x0);
          const double span = Single(x1 - x0);
          const double y0 = LoadF(base, r10 + 4), y1 = LoadF(base, r11 + 4);
          const double dy = Single(y1 - y0);
          amount = Fmadds(Single(dx / span), dy, y0);
        }
        StoreF(base, out, amount);
        out += 4;
      }
      if (!(LoadF(base, r11) > end)) {
        amount = LoadF(base, r11 - 4);
        StoreF(base, out, amount);
        out += 4;
      }
    }
    // The second curve (entry + 72): the pitch at the speed.
    {
      uint32_t r11 = second, r10 = second;
      double x = LoadF(base, second);
      bool found = false;
      while (x > end) {
        if (!(speed > x)) {
          found = true;
          break;
        }
        r10 = r11;
        r11 += 8;
        x = LoadF(base, r11);
      }
      if (found) {
        if (r11 == r10) {
          rate = LoadF(base, r11 + 4);
        } else {
          const double x0 = LoadF(base, r10), x1 = LoadF(base, r11);
          const double dx = Single(speed - x0);
          const double span = Single(x1 - x0);
          const double y0 = LoadF(base, r10 + 4), y1 = LoadF(base, r11 + 4);
          const double dy = Single(y1 - y0);
          rate = Fmadds(Single(dx / span), dy, y0);
        }
      }
      if (!(LoadF(base, r11) > end)) {
        rate = LoadF(base, r11 - 4);
      }
    }
    // Started when it has an amount and isn't playing.
    uint32_t voice = Load(base, slot_at);
    SetRegs(call, Args({voice, frame + 80}));
    xa::FrameBelow(base, frame, kApiFrame);
    xa::ApiGetState(base, voice, frame + 80, 0, 0, 0, call);
    if (amount > zero && (guest::LoadU8(base, frame + 80) & 1) == 0) {
      voice = Load(base, slot_at);
      SetRegs(call, Args({voice, 0}));
      xa::FrameBelow(base, frame, kApiFrame);
      xa::ApiStart(base, voice, 0, voices.thread, call);
    }
    // The volume: the table's word (as fcfid gives it, through the frame's
    // +88) times the scale, the amount, f2, f5 and the gain.
    const uint32_t word = Load(base, table);
    voice = Load(base, slot_at);
    Store64(base, frame + 88, word);
    double level = Single(double(int64_t(uint64_t(word))));
    level = Single(level * scale);
    level = Single(level * amount);
    level = Single(level * volume);
    level = Single(Single(level * gain) * LoadF(base, kLayerGain));
    SetVolume(base, voice, level, frame, call);
    // The pitch (SetPitchScaled: r7 0, r9 the entry's +4, f3 `pitch`; the
    // original's f2, [0x8200295C], isn't read by it).
    SetRegs(call, ArgsF1({holder, slot}, rate));
    SetPitchScaled(base, holder, slot, rate, pitch, 0, Load(base, entry + 4),
                   xa::FrameBelow(base, frame, kSetPitchScaledFrame), call);
    // The output map: the whole r7 at the frame's +96.
    voice = Load(base, slot_at);
    Store64(base, frame + 96, output);
    SetRegs(call, Args({voice, frame + 96}));
    xa::FrameBelow(base, frame, kApiFrame);
    xa::ApiSetOutputMatrix(base, voice, frame + 96, 0, 0, 0, call);
    entry += 136;
    slot++;
    slot_at += 4;
  } while (int32_t(Load(base, entry)) > -1);
  return 0;
}

uint32_t EngineLayerUpdatePitched(uint8_t* base, uint32_t engine, double speed, double f2,
                                  double f3, double f4, uint64_t output, const Voices& voices,
                                  uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D718 EngineLayer::UpdatePitched");
  constexpr uint32_t k = 0x82007FC4;  // r31: the constants from it
  const double threshold = LoadF(base, k + 28);
  double f0;
  if (speed > threshold) {
    f0 = LoadF(base, k + 0);
    if (!(speed > f0)) {
      f0 = speed;
    }
    const double x = Single(Single(f0 - threshold) * LoadF(base, 0x82008030));
    const double y = LoadDouble(base, 0x820056D8);
    const double curve = Single(c_runtime::Pow(x, y));
    f0 = Single(-std::fma(curve, LoadF(base, k + 36), -LoadF(base, k + 28)));
    const double cap = LoadF(base, k + 0);
    if (f0 > cap) {
      f0 = cap;
    }
  } else {
    f0 = speed;
  }
  const double scaled = Single(f0 * LoadF(base, 0x8200802C));
  const double mix = Fmadds(f2, LoadF(base, 0x82002A3C), LoadF(base, k + 20));
  const double volume = Single(scaled * mix);
  const uint32_t layer = engine + 84;
  SetRegs(call, ArgsF1({layer}, speed));
  return EngineLayerUpdate(base, layer, speed, volume, f3, f4, output, voices,
                           xa::FrameBelow(base, frame, kLayerUpdateFrame), call);
}

uint32_t EngineSoundUpdate(uint8_t* base, uint32_t engine, uint32_t vehicle, uint64_t output,
                           double f1, double f2, const Voices& voices, uint32_t frame,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E470 EngineSound::Update");
  constexpr uint32_t k = 0x82007FC8;  // r29
  double speed;                       // f30
  if (vehicle != 0) {
    const double top = LoadF(base, Load(base, vehicle + 272) + 1280);
    SetRegs(call, Args({vehicle}));
    const double value = double(va::VehicleFloat2224(base, vehicle));
    SetF1(call, value);
    speed = Single(Single(value / top) * LoadF(base, k - 4));
  } else {
    const double rise = Single(LoadF(base, engine + 172) + LoadF(base, 0x82001FD8));
    StoreF(base, engine + 172, rise);
    if (rise > LoadF(base, 0x82008048)) {
      StoreF(base, engine + 172, LoadF(base, kEngineRest));
    }
    speed = LoadF(base, engine + 172);
  }
  const double one = LoadF(base, k);       // f10
  const double zero = LoadF(base, kZero);  // f11
  double load;                             // f31
  if (vehicle != 0) {
    double target;  // f12
    if (guest::LoadU8(base, vehicle + 2248) != 0) {
      target = zero;
    } else if (LoadF(base, vehicle + 2268) > zero && int32_t(Load(base, vehicle + 2256)) != 0) {
      target = zero;
    } else if (int32_t(Load(base, vehicle + 2252)) == -1) {
      target = LoadF(base, vehicle + 288);
    } else {
      target = LoadF(base, vehicle + 284);
    }
    const double last = LoadF(base, engine + 168);
    load = Fmadds(Single(target - last), LoadF(base, 0x82002968), last);
  } else {
    load = one;
  }
  SetRegs(call, ArgsF1({engine}, speed));
  EngineLayerUpdate(base, engine, speed, Fmadds(load, zero, one), f1, f2, output, voices,
                    xa::FrameBelow(base, frame, kLayerUpdateFrame), call);
  SetRegs(call, ArgsF1({engine}, speed));
  const uint32_t r3 = EngineLayerUpdatePitched(base, engine, speed, load, f1, f2, output, voices,
                                               xa::FrameBelow(base, frame, kPitchedFrame), call);
  StoreF(base, engine + 168, load);
  return r3;
}

// --- The tires (sub_8210E5C0 with its layer) and the roll (sub_8210DB60) ---

namespace {

constexpr uint32_t kTireLayerFrame = 160;
constexpr uint32_t kSlipWheelValueFrame = 240;
constexpr uint32_t kTireVolume = 0x82007FEC;    // the tires' and roll's volume scale
constexpr uint32_t kTirePitch = 0x82008038;     // added to the tires' pitch word
constexpr uint32_t kTireRampScale = 0x820029EC; // +8 x 10 to the ramp's length
constexpr uint32_t kTireRampShort = 0x82001FD8; // the ramp below 10 ticks
constexpr uint32_t kTireRampMax = 0x82008034;   // the ramp's cap
constexpr uint32_t kSlipSpeed = 0x82002BCC;     // SlipWheelValue's f1
constexpr uint32_t kSlipThreshold = 0x82007FF0; // MaxWheelSlip's f1
constexpr uint32_t kRollScale = 0x82008044;     // the speed squared to the volume
constexpr uint32_t kRollPitchScale = 0x82008040;
constexpr uint32_t kRollPitch = 0x8200803C;     // added to the roll's pitch word

// The pitch from a word converted through the frame's +80 (or +88): the
// 64-bit word stored, fcfid, frsp, then + `add` (fadds).
double PitchOfWord(uint8_t* base, uint32_t at, int64_t word, uint32_t add) {
  Store64(base, at, uint64_t(word));
  return Single(Single(double(word)) + LoadF(base, add));
}

}  // namespace

uint32_t TireLayerUpdate(uint8_t* base, uint32_t part, uint32_t layer, uint32_t active,
                         uint64_t output, double scale, const Voices& voices, uint32_t frame,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D890 TireLayer::Update");
  const uint32_t holder = part + 24;
  uint32_t r3;
  if (active != 0) {  // r26
    // Playing (started first if not): the volume +80 x f1, the pitch from
    // the half-word +76, the output; +9 = 0, +8 up by one.
    const uint32_t playing = Playing(base, holder, Load(base, layer), frame, call);
    uint32_t voice = VoiceAt(base, holder, Load(base, layer));
    if (playing == 0) {
      SetRegs(call, Args({voice, 0}));
      xa::FrameBelow(base, frame, kApiFrame);
      xa::ApiStart(base, voice, 0, voices.thread, call);
      voice = VoiceAt(base, holder, Load(base, layer));
    }
    SetVolume(base, voice,
              Single(Single(LoadF(base, part + 80) * scale) * LoadF(base, kTireVolume)), frame,
              call);
    const int64_t word = int16_t(guest::LoadU16(base, part + 76));
    const double pitch = PitchOfWord(base, frame + 80, word, kTirePitch);
    const uint32_t slot = Load(base, layer);
    SetRegs(call, ArgsF1({holder, slot}, pitch));
    SetPitch(base, holder, slot, pitch, 0, frame, call);
    voice = VoiceAt(base, holder, Load(base, layer));
    Store64(base, frame + 80, output);
    SetRegs(call, Args({voice, frame + 80}));
    xa::FrameBelow(base, frame, kApiFrame);
    r3 = xa::ApiSetOutputMatrix(base, voice, frame + 80, 0, 0, 0, call);
    guest::StoreU8(base, layer + 9, 0);
    guest::StoreU8(base, layer + 8, uint8_t(guest::LoadU8(base, layer + 8) + 1));
    Store(base, layer + 4, active);
    return r3;
  }
  r3 = Playing(base, holder, Load(base, layer), frame, call);
  if (r3 == 0) {
    Store(base, layer + 4, active);
    return r3;
  }
  if (Load(base, layer + 4) != 0) {
    // Just stopped slipping: the ramp's length +10 from the ticks it played
    // (+8 x 10 x [0x820029EC], fctidz's low byte; under 10, [0x82001FD8];
    // else that byte as a float, at most [0x82008034], fctidz's low byte).
    const int64_t ticks = int32_t(uint32_t(guest::LoadU8(base, layer + 8)) * 10);
    Store64(base, frame + 80, uint64_t(ticks));
    double f0 = Single(Single(double(ticks)) * LoadF(base, kTireRampScale));
    uint64_t word = Fctidz64(f0);
    Store64(base, frame + 80, word);
    const uint8_t length = uint8_t(word);
    guest::StoreU8(base, layer + 10, length);
    if (length < 10) {
      f0 = LoadF(base, kTireRampShort);
    } else {
      Store64(base, frame + 80, length);
      f0 = Single(double(int64_t(length)));
      const double cap = LoadF(base, kTireRampMax);
      if (f0 > cap) {
        f0 = cap;
      }
    }
    word = Fctidz64(f0);
    Store64(base, frame + 80, word);
    guest::StoreU8(base, layer + 10, uint8_t(word));
  }
  // The ramp: the pitch half-way from +76 toward +9 x 128 (at most 16383),
  // while +9 is below +10; then stopped, +8 and +9 cleared. +9 up by one.
  const uint32_t step = guest::LoadU8(base, layer + 9);
  int32_t target = int32_t(step << 7);
  if (target > 16383) {
    target = 16383;
  }
  const uint32_t length = guest::LoadU8(base, layer + 10);
  const uint16_t now = guest::LoadU16(base, part + 76);
  const int32_t diff = int32_t(uint32_t(int64_t(int16_t(target)) - int64_t(int16_t(now))));
  const int64_t mid = int16_t(uint16_t(uint32_t(diff / 2) + now));
  if (step < length) {
    uint32_t voice = VoiceAt(base, holder, Load(base, layer));
    SetVolume(base, voice, Single(LoadF(base, part + 80) * LoadF(base, kTireVolume)), frame,
              call);
    const uint32_t slot = Load(base, layer);
    const double pitch = PitchOfWord(base, frame + 80, mid, kTirePitch);
    SetRegs(call, ArgsF1({holder, slot}, pitch));
    SetPitch(base, holder, slot, pitch, 0, frame, call);
    voice = VoiceAt(base, holder, Load(base, layer));
    Store64(base, frame + 80, output);
    SetRegs(call, Args({voice, frame + 80}));
    xa::FrameBelow(base, frame, kApiFrame);
    r3 = xa::ApiSetOutputMatrix(base, voice, frame + 80, 0, 0, 0, call);
  } else {
    r3 = StopVoice(base, VoiceAt(base, holder, Load(base, layer)), frame, call);
    guest::StoreU8(base, layer + 8, 0);
    guest::StoreU8(base, layer + 9, 0);
  }
  guest::StoreU8(base, layer + 9, uint8_t(guest::LoadU8(base, layer + 9) + 1));
  Store(base, layer + 4, active);
  return r3;
}

uint32_t TireSoundUpdate(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                         const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E5C0 TireSound::Update");
  const double speed = LoadF(base, kSlipSpeed);
  SetRegs(call, ArgsF1({vehicle}, speed));
  const double slip = vehicle::VehicleSlipWheelValue(
      base, vehicle, speed, xa::FrameBelow(base, frame, kSlipWheelValueFrame));  // f29
  SetF1(call, slip);
  uint32_t grounded = 0;  // r9: the wheels whose +433 is set
  for (uint32_t i = 0; i < 4; i++) {
    if (guest::LoadU8(base, vehicle + 433 + 368 * i) != 0) {
      grounded++;
    }
  }
  const double zero = LoadF(base, kZero);  // f31
  const double one = LoadF(base, kOne);    // f30
  uint32_t active = 0;                     // r29
  bool slipping = false;
  if (grounded != 0) {
    const double threshold = LoadF(base, kSlipThreshold);
    SetRegs(call, ArgsF1({vehicle}, threshold));
    const double most = double(va::VehicleMaxWheelSlip(base, vehicle, threshold));
    SetF1(call, most);
    if (most != zero) {  // fcmpu: a NaN slips
      active = 1;
      SetRegs(call, ArgsF1({part, vehicle}, slip));
      TireSoundAmount(base, part, slip);
      guest::StoreU16(base, part + 70, uint16_t(guest::LoadU16(base, part + 70) + 1));
      StoreF(base, part + 72, Single(LoadF(base, part + 72) + one));
      slipping = true;
    }
  } else {
    // No wheel down: both voices (+28, +32) stopped when playing (their
    // states at the frame's +80 and +81).
    for (uint32_t i = 0; i < 2; i++) {
      const uint32_t voice = Load(base, part + 28 + 4 * i);
      if (voice == 0) {
        continue;
      }
      SetRegs(call, Args({voice, frame + 80 + i}));
      xa::FrameBelow(base, frame, kApiFrame);
      xa::ApiGetState(base, voice, frame + 80 + i, 0, 0, 0, call);
      if ((guest::LoadU8(base, frame + 80 + i) & 1) != 0) {
        StopVoice(base, Load(base, part + 28 + 4 * i), frame, call);
      }
    }
  }
  if (!slipping) {
    // Neither slot (+0, +12) playing: +80, +72 = 0.0, +70, +76 = 0.
    const uint32_t holder = part + 24;
    if (Playing(base, holder, Load(base, part + 0), frame, call) == 0 &&
        Playing(base, holder, Load(base, part + 12), frame, call) == 0) {
      StoreF(base, part + 80, zero);
      guest::StoreU16(base, part + 70, uint16_t(active));
      StoreF(base, part + 72, zero);
      guest::StoreU16(base, part + 76, uint16_t(active));
    }
  }
  SetRegs(call, ArgsF1({part, part, active}, one));
  return TireLayerUpdate(base, part, part, active, output, one, voices,
                         xa::FrameBelow(base, frame, kTireLayerFrame), call);
}

uint32_t RollSoundUpdate(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                         const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210DB60 RollSound::Update");
  SetRegs(call, Args({vehicle}));
  const double speed = double(Vehicle_GetSpeedKmh(base, vehicle));
  SetF1(call, speed);
  double f0 = Single(Single(speed * speed) * LoadF(base, kRollScale));
  const double cap = LoadF(base, kOne);
  if (f0 > cap) {
    f0 = cap;
  }
  const double volume = Single(f0 * LoadF(base, kTireVolume));  // f31
  Store(base, frame + 88, FctiwzWord(Single(volume * LoadF(base, kRollPitchScale))));
  const double zero = LoadF(base, kZero);
  uint32_t voice = Load(base, part + 4);
  uint32_t r3 = voice;
  bool playing = false;  // r29
  if (voice != 0) {
    SetRegs(call, Args({voice, frame + 80}));
    xa::FrameBelow(base, frame, kApiFrame);
    r3 = xa::ApiGetState(base, voice, frame + 80, 0, 0, 0, call);
    playing = (guest::LoadU8(base, frame + 80) & 1) != 0;
  }
  if (playing) {
    if (volume == zero) {
      return StopVoice(base, Load(base, part + 4), frame, call);
    }
  } else {
    if (volume == zero) {
      return r3;
    }
    voice = Load(base, part + 4);
    SetRegs(call, Args({voice, 0}));
    xa::FrameBelow(base, frame, kApiFrame);
    xa::ApiStart(base, voice, 0, voices.thread, call);
  }
  // The pitch from the word at +88 (as the GetState above may have left it),
  // then the volume x [0x82007FEC] again and the output.
  const int64_t word = int32_t(Load(base, frame + 88));
  const double pitch = PitchOfWord(base, frame + 88, word, kRollPitch);
  SetRegs(call, ArgsF1({part, 0}, pitch));
  SetPitch(base, part, 0, pitch, 0, frame, call);
  voice = Load(base, part + 4);
  SetVolume(base, voice, Single(volume * LoadF(base, kTireVolume)), frame, call);
  voice = Load(base, part + 4);
  Store64(base, frame + 88, output);
  SetRegs(call, Args({voice, frame + 88}));
  xa::FrameBelow(base, frame, kApiFrame);
  return xa::ApiSetOutputMatrix(base, voice, frame + 88, 0, 0, 0, call);
}

// --- The horn (sub_8210F118) and the turbo (sub_8210F4E8) ---

namespace {

constexpr uint32_t kRaceData = 0x826C5B2C;     // the race data: its +0xA3AF4 the race mode
constexpr uint32_t kRaceMode = 0xA3AF4;
constexpr uint32_t kCarInputs = 0x826C5B30;    // the racers' 20-byte input records' pointers
constexpr uint32_t kSideWord = 0x826C7824;     // 0 picks input record 1 in mode 8
constexpr uint32_t kOtherInput = 0x826C62CC;   // the other 20-byte input record (mode 8)
constexpr uint32_t kHornButton = 0x20000;      // the input word's bit (probably the horn)
constexpr uint32_t kHornVolume = 0x82007FD8;
constexpr uint32_t kHornPitchScale = 1600;     // SetPitchScaled's r9
constexpr uint32_t kTurboStep = 0x82008068;    // +59 x it added to the volume +60
constexpr uint32_t kTurboVolumeMax = 0x82008064;
constexpr uint32_t kTurboOff = 0x82002A2C;     // the boost below it stops the voice
constexpr uint32_t kTurboRise = 0x820021EC;    // the boost's rise to the pitch word
constexpr uint32_t kTurboRound = 0x820029CC;   // added before fctiwz

// fctiwz's whole 64-bit result as the translation leaves it (stfd): the
// truncated int32 sign-extended, 0x80000000 for a NaN, INT32_MAX at or above
// it, INT32_MIN (sign-extended) below its range.
uint64_t Fctiwz64(double value) {
  if (std::isnan(value)) {
    return 0x80000000u;
  }
  if (value >= double(INT32_MAX)) {
    return uint64_t(INT32_MAX);
  }
  if (value <= -2147483649.0) {
    return uint64_t(int64_t(INT32_MIN));
  }
  return uint64_t(int64_t(int32_t(value)));
}

// One horn: the 20-byte input record at `record` copied to the frame's +80;
// with its first word's button bit, the looping wave 11 + `horn` played
// (slot at +`slot_at`, +`flag_at` = 1) or kept pitched; without it, the
// voice stopped when +`flag_at` is 1 and it plays (+`flag_at` = 0).
void HornUpdate(uint8_t* base, uint32_t part, uint32_t record, uint32_t horn, uint32_t flag_at,
                uint32_t slot_at, double volume, double pitch, double f3, const Voices& voices,
                uint32_t frame, const GuestCall& call) {
  for (uint32_t i = 0; i < 20; i += 4) {
    Store(base, frame + 80 + i, Load(base, record + i));
  }
  const uint32_t flag = Load(base, part + flag_at);
  if ((Load(base, frame + 80) & kHornButton) != 0) {
    uint32_t slot;
    if (flag == 0) {
      slot = Play(base, part, horn + 11, 1, volume, pitch, voices, frame, call);
      Store(base, part + slot_at, slot);
    } else {
      slot = Load(base, part + slot_at);
    }
    SetRegs(call, ArgsF1({part, slot}, pitch));
    SetPitchScaled(base, part, slot, pitch, f3, 0, kHornPitchScale,
                   xa::FrameBelow(base, frame, kSetPitchScaledFrame), call);
    if (flag == 0) {
      Store(base, part + flag_at, 1);
    }
  } else if (flag == 1) {
    if (Playing(base, part, Load(base, part + slot_at), frame, call) != 0) {
      StopVoice(base, VoiceAt(base, part, Load(base, part + slot_at)), frame, call);
      Store(base, part + flag_at, 0);
    }
  }
}

}  // namespace

uint32_t HornSoundUpdate(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                         double f1, const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210F118 HornSound::Update");
  if (vehicle == 0) {
    return part;
  }
  const uint32_t mode = Load(base, Load(base, kRaceData) + kRaceMode);
  const uint32_t horn = Load(base, Load(base, vehicle + 5712) + 1796) & 0xFF;
  if (int32_t(mode) == 8) {
    const double pitch = LoadF(base, kPitch);       // f31
    const double volume = LoadF(base, kHornVolume);  // f28
    const uint32_t side = Load(base, kSideWord) == 0 ? 1 : 0;  // r30
    const uint32_t flags = Load(base, vehicle + 6148);
    const bool first = (flags & 1) != 0, second = (flags & 0x80) != 0;
    if (side == 0 ? first : second) {
      HornUpdate(base, part, Load(base, kCarInputs + 4 * side), horn, 56, 48, volume, pitch, f1,
                 voices, frame, call);
    }
    if (side == 0 ? second : first) {
      HornUpdate(base, part, kOtherInput, horn, 60, 52, volume, pitch, f1, voices, frame, call);
    }
  } else {
    HornUpdate(base, part, Load(base, kCarInputs), horn, 56, 48, LoadF(base, kHornVolume),
               LoadF(base, kPitch), f1, voices, frame, call);
  }
  return Update(base, part, output, frame, call);
}

uint32_t TurboSoundUpdate(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                          const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210F4E8 TurboSound::Update");
  const double boost = LoadF(base, vehicle + 2944);  // f30
  // The volume +60 up by +59 x [0x82008068], at most [0x82008064] (the
  // byte through the frame's +80).
  {
    const uint32_t ramp = guest::LoadU8(base, part + 59);
    Store64(base, frame + 80, ramp);
    double f0 = Fmadds(Single(double(int64_t(ramp))), LoadF(base, kTurboStep),
                       LoadF(base, part + 60));
    const double max = LoadF(base, kTurboVolumeMax);
    if (f0 > max) {
      f0 = max;
    }
    StoreF(base, part + 60, f0);
  }
  const double one = LoadF(base, kOne);    // f29
  const double zero = LoadF(base, kZero);  // f31
  const uint8_t wait = guest::LoadU8(base, part + 58);
  const auto word_pitch = [&](int64_t word) {
    Store64(base, frame + 80, uint64_t(word));
    return Single(double(word));
  };
  if (wait != 0) {
    guest::StoreU8(base, part + 58, uint8_t(wait + 255));
  } else if (Playing(base, part, Load(base, part + 48), frame, call) == 0 && boost > one &&
             boost > LoadF(base, part + 52)) {
    // Spooling up: the looping wave 3 at the volume +60, its pitch from +56.
    const double pitch = word_pitch(int64_t(int16_t(guest::LoadU16(base, part + 56))) + 8192);
    const uint32_t slot =
        Play(base, part, 3, 1, LoadF(base, part + 60), pitch, voices, frame, call);
    StoreF(base, part + 60, zero);
    Store(base, part + 48, slot);
    guest::StoreU8(base, part + 59, 0);
    guest::StoreU8(base, part + 58, 16);
  } else if (boost < LoadF(base, kTurboOff)) {
    if (Playing(base, part, Load(base, part + 48), frame, call) != 0) {
      StopVoice(base, VoiceAt(base, part, Load(base, part + 48)), frame, call);
      StoreF(base, part + 60, zero);
      guest::StoreU8(base, part + 59, 0);
    }
  }
  const uint32_t playing = Playing(base, part, Load(base, part + 48), frame, call);
  const int32_t word = int16_t(guest::LoadU16(base, part + 56));
  if (playing != 0) {
    if (word < 3072) {
      const double rise = Single(boost - LoadF(base, part + 52));
      if (!(rise < zero)) {
        // +56 up by the rise x [0x820021EC] (+ [0x820029CC], fctiwz's low
        // half-word through the frame's +80), at most 3072.
        const double now = word_pitch(word);
        const double f0 =
            Single(Fmadds(rise, LoadF(base, kTurboRise), now) + LoadF(base, kTurboRound));
        const uint64_t bits = Fctiwz64(f0);
        Store64(base, frame + 80, bits);
        const uint16_t next = uint16_t(bits);
        guest::StoreU16(base, part + 56, int16_t(next) > 3072 ? 3072 : next);
      }
    }
    const uint32_t slot = Load(base, part + 48);
    const double pitch =
        word_pitch(int64_t(int16_t(guest::LoadU16(base, part + 56))) + 8192);
    SetRegs(call, ArgsF1({part, slot}, pitch));
    SetPitch(base, part, slot, pitch, 0, frame, call);
    SetVolume(base, VoiceAt(base, part, Load(base, part + 48)), LoadF(base, part + 60), frame,
              call);
    guest::StoreU8(base, part + 59, uint8_t(guest::LoadU8(base, part + 59) + 1));
  } else {
    // Not playing: the pitch word +56 back down by 255 a tick, to 0.
    const int32_t down = word - 255;
    guest::StoreU16(base, part + 56, down >= 0 ? uint16_t(down) : 0);
  }
  StoreF(base, part + 52, boost);
  return Update(base, part, output, frame, call);
}

}  // namespace car_sounds
