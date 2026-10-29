// The car sounds' voice holders natively (car_sounds.h,
// docs/functions/car-sounds.md): XAudio's entry points through their plain
// C++ (src/xaudio/voices.h), each in a frame below the caller's, with the
// registers set as the original sets them before the call. Strict floating
// point (CMakeLists.txt), as the original's single-precision steps.

#include "car_sounds/car_sounds.h"

#include "base/log.h"
#include "guest.h"

#include <initializer_list>

namespace car_sounds {

namespace {

namespace xa = xaudio;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
double LoadF(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
// A single-precision step's result, as the register holds it.
double Single(double value) { return double(float(value)); }

xa::Regs Args(std::initializer_list<uint64_t> r) {
  xa::Regs regs;
  for (const uint64_t value : r) {
    regs.r[regs.count++] = value;
  }
  return regs;
}
// The registers the original sets before calling a function whose plain C++
// runs here instead.
void SetRegs(const GuestCall& call, xa::Regs regs) { call(0, 0, regs); }

// The API entry points' frames (their originals' sizes).
constexpr uint32_t kApiFrame = 112;

// The voice in `slot` (the original's (slot + 1) x 4 from the holder).
uint32_t VoiceAt(uint8_t* base, uint32_t holder, uint32_t slot) {
  return Load(base, holder + ((slot + 1) << 2));
}

// The constants.
constexpr uint32_t kZero = 0x82000A34;           // 0.0
constexpr uint32_t kPitchBias = 0x8200801C;
constexpr uint32_t kPitchScale = 0x82008020;
constexpr uint32_t kPitchScaleR9 = 0x82008024;
constexpr uint32_t kPitchFloorR7 = 0x82002A24;
constexpr uint32_t kPitchFloor = 0x82001BE0;
constexpr uint32_t kOne = 0x82007FC8;            // 1.0

// The 12 holders PauseAll and ResumeAll walk, in their order.
constexpr uint32_t kHolders[] = {408 + 44, 408 + 128, 952, 584, 676, 736,
                                 840,      792,       888, 1016, 1072, 1136};

}  // namespace

uint32_t SetPitch(uint8_t* base, uint32_t holder, uint32_t slot, double f1, uint32_t r7,
                  uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D158 CarSound::SetPitch");
  const double floor = LoadF(base, r7 != 0 ? kPitchFloorR7 : kPitchFloor);
  if (f1 == LoadF(base, kZero)) {
    f1 = floor;
  } else {
    f1 = Single(Single(f1 - LoadF(base, kPitchBias)) * LoadF(base, kPitchScale));
    if (f1 < floor) {
      f1 = floor;
    }
  }
  const uint32_t voice = VoiceAt(base, holder, slot);
  xa::Regs regs = Args({voice});
  regs.f1_set = true;
  regs.f1 = f1;
  SetRegs(call, regs);
  // The tail call's frame where the original's callee makes it.
  xa::FrameBelow(base, sp, kApiFrame);
  return xa::ApiSetFrequencyRatio(base, voice, f1, call);
}

uint32_t SetPitchScaled(uint8_t* base, uint32_t holder, uint32_t slot, double f1, double f3,
                        uint32_t r7, uint32_t r9, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D080 CarSound::SetPitchScaled");
  double f0;
  if (f1 == LoadF(base, kZero)) {
    f0 = LoadF(base, r7 != 0 ? kPitchFloorR7 : kPitchFloor);
  } else {
    double f12 = Single(f1 - LoadF(base, kPitchBias));
    double f13 = Single(double(int64_t(int32_t(r9))));
    f13 = Single(f13 * LoadF(base, kPitchScaleR9));
    f12 = Single(f12 * f13);
    f0 = Single(f12 * LoadF(base, kPitchScale));
    f13 = r7 != 0 ? LoadF(base, kPitchFloorR7) : Single(f13 * LoadF(base, kPitchFloor));
    if (f0 < f13) {
      f0 = f13;
    }
  }
  const double ratio = Single(Single(f0 + f3) - LoadF(base, kOne));
  const uint32_t voice = VoiceAt(base, holder, slot);
  xa::Regs regs = Args({voice});
  regs.f1_set = true;
  regs.f1 = ratio;
  SetRegs(call, regs);
  xa::FrameBelow(base, frame, kApiFrame);
  return xa::ApiSetFrequencyRatio(base, voice, ratio, call);
}

uint32_t IsPlaying(uint8_t* base, uint32_t holder, uint32_t slot, uint32_t frame,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D1D8 CarSound::IsPlaying");
  if (int32_t(slot) == -1) {
    return 0;
  }
  const uint32_t voice = VoiceAt(base, holder, slot);
  if (voice == 0) {
    return 0;
  }
  SetRegs(call, Args({voice, frame + 80}));
  xa::FrameBelow(base, frame, kApiFrame);
  xa::ApiGetState(base, voice, frame + 80, 0, 0, 0, call);
  return (guest::LoadU8(base, frame + 80) & 1) != 0 ? 1 : 0;
}

uint32_t UpdateVoices(uint8_t* base, uint32_t holder, uint64_t output, uint32_t frame,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D378 CarSound::UpdateVoices");
  Store(base, frame + 136, uint32_t(output >> 32));
  Store(base, frame + 140, uint32_t(output));
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t at = holder + 4 + 4 * i;
    uint32_t voice = Load(base, at);
    r3 = voice;
    if (voice == 0) {
      continue;
    }
    SetRegs(call, Args({voice, frame + 80}));
    xa::FrameBelow(base, frame, kApiFrame);
    r3 = xa::ApiGetState(base, voice, frame + 80, 0, 0, 0, call);
    const uint8_t state = guest::LoadU8(base, frame + 80);
    if ((state & 4) != 0) {
      voice = Load(base, at);
      SetRegs(call, Args({voice, 0}));
      xa::FrameBelow(base, frame, kApiFrame);
      xa::ApiStop(base, voice, 0, 0, 0, 0, call);
      voice = Load(base, at);
      SetRegs(call, Args({voice}));
      xa::FrameBelow(base, frame, kApiFrame);
      r3 = xa::ApiRelease(base, voice, 0, 0, 0, 0, call);
      Store(base, at, 0);
    } else if ((state & 1) != 0) {
      voice = Load(base, at);
      SetRegs(call, Args({voice, frame + 136}));
      xa::FrameBelow(base, frame, kApiFrame);
      r3 = xa::ApiSetOutputMatrix(base, voice, frame + 136, 0, 0, 0, call);
    }
  }
  return r3;
}

uint32_t Clear(uint8_t* base, uint32_t holder, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D320 CarSound::Clear");
  uint32_t r3 = holder;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t at = holder + 4 + 4 * i;
    uint32_t voice = Load(base, at);
    if (voice == 0) {
      // The original's r3: the slot's word, 0.
      r3 = 0;
      continue;
    }
    SetRegs(call, Args({voice, 0}));
    xa::FrameBelow(base, frame, kApiFrame);
    xa::ApiStop(base, voice, 0, 0, 0, 0, call);
    voice = Load(base, at);
    SetRegs(call, Args({voice}));
    xa::FrameBelow(base, frame, kApiFrame);
    r3 = xa::ApiRelease(base, voice, 0, 0, 0, 0, call);
    Store(base, at, 0);
  }
  Store(base, holder + 20, 0);
  return r3;
}

uint32_t Pause(uint8_t* base, uint32_t holder, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D240 CarSound::Pause");
  uint32_t r3 = holder;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t at = holder + 4 + 4 * i;
    uint32_t voice = Load(base, at);
    Store(base, at + 20, 0);
    r3 = voice;
    if (voice == 0) {
      continue;
    }
    SetRegs(call, Args({voice, frame + 80}));
    xa::FrameBelow(base, frame, kApiFrame);
    r3 = xa::ApiGetState(base, voice, frame + 80, 0, 0, 0, call);
    if ((guest::LoadU8(base, frame + 80) & 1) == 0) {
      continue;
    }
    voice = Load(base, at);
    Store(base, at + 20, 1);
    SetRegs(call, Args({voice, 0}));
    xa::FrameBelow(base, frame, kApiFrame);
    r3 = xa::ApiStop(base, voice, 0, 0, 0, 0, call);
  }
  return r3;
}

uint32_t Resume(uint8_t* base, uint32_t holder, const Thread& thread, uint32_t frame,
                const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D2C0 CarSound::Resume");
  uint32_t r3 = holder;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t at = holder + 4 + 4 * i;
    const uint32_t voice = Load(base, at);
    r3 = voice;
    if (voice == 0 || Load(base, at + 20) == 0) {
      continue;
    }
    SetRegs(call, Args({voice, 0}));
    xa::FrameBelow(base, frame, kApiFrame);
    r3 = xa::ApiStart(base, voice, 0, thread, call);
  }
  return r3;
}

uint32_t PauseAll(uint8_t* base, uint32_t sound, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E2B8 CarSound::PauseAll");
  uint32_t r3 = 0;
  for (const uint32_t offset : kHolders) {
    SetRegs(call, Args({sound + offset}));
    r3 = Pause(base, sound + offset, xa::FrameBelow(base, frame, 112), call);
  }
  return r3;
}

uint32_t ResumeAll(uint8_t* base, uint32_t sound, const Thread& thread, uint32_t frame,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E350 CarSound::ResumeAll");
  uint32_t r3 = 0;
  for (const uint32_t offset : kHolders) {
    SetRegs(call, Args({sound + offset}));
    r3 = Resume(base, sound + offset, thread, xa::FrameBelow(base, frame, 112), call);
  }
  return r3;
}

}  // namespace car_sounds
