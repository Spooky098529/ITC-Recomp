// The car sounds' voice holders natively (car_sounds.h,
// docs/functions/car-sounds.md): XAudio's entry points through their plain
// C++ (src/xaudio/voices.h), each in a frame below the caller's, with the
// registers set as the original sets them before the call. Strict floating
// point (CMakeLists.txt), as the original's single-precision steps.

#include "car_sounds/car_sounds.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

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
constexpr uint32_t kEngineRest = 0x82003384;

// The 12 holders Stop and Release walk, in their order.
constexpr uint32_t kClearHolders[] = {408 + 44, 408 + 128, 584, 676, 736, 840,
                                      792,      888,       952, 1016, 1072, 1136};

void StoreF(uint8_t* base, uint32_t address, uint32_t constant) {
  guest::StoreFloat(base, address, guest::LoadFloat(base, constant));
}
void StoreB(uint8_t* base, uint32_t address, uint8_t value) { guest::StoreU8(base, address, value); }

// The frames of the plain C++ callees (their originals' sizes).
constexpr uint32_t kAddVoiceFrame = 496;
constexpr uint32_t kCreateFrame = 128;
constexpr uint32_t kSubmitFrame = 128;

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

uint32_t AddVoice(uint8_t* base, uint32_t holder, uint32_t view, uint32_t wave, uint32_t loop,
                  const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210CE38 CarSound::AddVoice");
  const uint32_t f = frame;
  // The six-channel map at +80 (its entries at +96: input 0 to output i,
  // gain 0.0) and the outputs at +144 (two at +160: the null voice and the
  // submix, each with the map).
  for (const uint32_t at : {100u, 108u, 116u, 124u, 132u, 140u}) {
    StoreF(base, f + at, kZero);
  }
  StoreB(base, f + 121, 3);
  StoreB(base, f + 96, 0);
  StoreB(base, f + 97, 0);
  StoreB(base, f + 104, 0);
  StoreB(base, f + 105, 1);
  StoreB(base, f + 129, 4);
  StoreB(base, f + 112, 0);
  StoreB(base, f + 113, 2);
  StoreB(base, f + 120, 0);
  StoreB(base, f + 128, 0);
  StoreB(base, f + 137, 5);
  StoreB(base, f + 136, 0);
  StoreB(base, f + 80, 6);
  StoreB(base, f + 144, 2);
  Store(base, f + 160, 0);
  Store(base, f + 84, f + 96);
  Store(base, f + 148, f + 160);
  Store(base, f + 164, f + 80);
  Store(base, f + 168, Load(base, kSubmix));
  Store(base, f + 172, f + 80);
  uint32_t slot = 0;
  while (Load(base, holder + 4 + 4 * slot) != 0) {
    if (++slot >= 4) {
      return 0xFFFFFFFFu;
    }
  }
  // The voice's description at +304 (92 bytes cleared), wave 0's format.
  for (uint32_t i = 0; i < 92; i += 4) {
    Store(base, f + 304 + i, 0);
  }
  SetRegs(call, Args({view, 0, f + 304}));
  WaveBankFormat(base, view, 0, f + 304);
  StoreB(base, f + 361, 2);
  StoreB(base, f + 362, 6);
  StoreB(base, f + 363, 1);
  const uint32_t out = holder + ((slot + 1) << 2);
  Store(base, out, 0);
  Store(base, f + 376, f + 144);
  SetRegs(call, Args({f + 304, out}));
  xa::ApiCreateSourceVoice(base, f + 304, out, voices.native_heap, voices.thread,
                           xa::FrameBelow(base, f, kCreateFrame), call);
  // The packet at +208: the data, the length (+212), the loop region.
  Store(base, f + 208, 0);
  for (uint32_t i = 0; i < 84; i += 4) {
    Store(base, f + 212 + i, 0);
  }
  SetRegs(call, Args({view, wave, f + 88}));
  WaveBankLength(base, view, wave, f + 88);
  SetRegs(call, Args({view, wave, f + 208}));
  WaveBankData(base, view, wave, f + 208);
  Store(base, f + 176, 0);
  for (uint32_t i = 0; i < 20; i += 4) {
    Store(base, f + 180 + i, 0);
  }
  Store(base, f + 212, Load(base, f + 88));
  SetRegs(call, Args({view, wave, f + 176}));
  if (!WaveBankEntry(base, view, wave, f + 176)) {
    call(0x8210C998, 0x8210CFEC, Args({view, wave, f + 176}));
  }
  if (loop != 0) {
    // The entry's format (+4): bit 31 16-bit samples, bits 2-4 the channels.
    const uint32_t format = Load(base, f + 180);
    const uint32_t bits = (format >> 31) == 1 ? 16 : 8;
    const uint32_t frame_bytes = (bits * ((format >> 2) & 7)) >> 3;
    Store(base, f + 220, frame_bytes * Load(base, f + 192));
    Store(base, f + 216, 0xFFFFFFFFu);
    Store(base, f + 224, frame_bytes * Load(base, f + 196));
  }
  const uint32_t voice = Load(base, out);
  SetRegs(call, Args({voice, f + 208, 1}));
  xa::FrameBelow(base, f, kSubmitFrame);
  xa::ApiSubmitPacket(base, voice, f + 208, 1, 0, 0, call);
  Store(base, holder + 20, Load(base, holder + 20) + 1);
  return slot;
}

uint32_t PlayOnce(uint8_t* base, uint32_t holder, uint32_t view, uint32_t wave, uint32_t loop,
                  double volume, double pitch, const Voices& voices, uint32_t frame,
                  const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E3E8 CarSound::PlayOnce");
  SetRegs(call, Args({holder, view, wave, loop}));
  const uint32_t slot = AddVoice(base, holder, view, wave, loop, voices,
                                 xa::FrameBelow(base, frame, kAddVoiceFrame), call);
  if (int32_t(slot) == -1) {
    return slot;
  }
  uint32_t voice = VoiceAt(base, holder, slot);
  SetRegs(call, Args({voice, 0}));
  xa::FrameBelow(base, frame, kApiFrame);
  xa::ApiStart(base, voice, 0, voices.thread, call);
  voice = VoiceAt(base, holder, slot);
  xa::Regs regs = Args({voice});
  regs.f1_set = true;
  regs.f1 = volume;
  SetRegs(call, regs);
  xa::FrameBelow(base, frame, kApiFrame);
  xa::ApiSetVolume(base, voice, volume, call);
  regs = Args({holder, slot});
  regs.f1_set = true;
  regs.f1 = pitch;
  SetRegs(call, regs);
  SetPitch(base, holder, slot, pitch, 0, frame, call);
  return slot;
}

uint32_t EngineLayerStart(uint8_t* base, uint32_t layer, uint32_t view, const Voices& voices,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D410 EngineLayer::Start");
  for (uint32_t at = 4; at <= 16; at += 4) {
    Store(base, layer + at, 0);
  }
  // memset (sub_825A71D0) with 0: the same bytes.
  for (uint32_t i = 0; i < 24; i += 4) {
    Store(base, layer + 20 + i, 0);
  }
  uint32_t r3 = 0;
  for (uint32_t wave = 0; wave < 4; wave++) {
    SetRegs(call, Args({layer + 44, view, wave, 1}));
    r3 = AddVoice(base, layer + 44, view, wave, 1, voices,
                  xa::FrameBelow(base, frame, kAddVoiceFrame), call);
  }
  return r3;
}

uint32_t EngineSoundStart(uint8_t* base, uint32_t engine, uint64_t index, uint32_t views,
                          const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D6C0 EngineSound::Start");
  SetRegs(call, Args({engine + 84, index, Load(base, views)}));
  EngineLayerStart(base, engine + 84, Load(base, views), voices, xa::FrameBelow(base, frame, 112),
                   call);
  SetRegs(call, Args({engine, index, Load(base, views + 4)}));
  const uint32_t r3 = EngineLayerStart(base, engine, Load(base, views + 4), voices,
                                       xa::FrameBelow(base, frame, 112), call);
  StoreF(base, engine + 168, kZero);
  StoreF(base, engine + 172, kEngineRest);
  return r3;
}

uint32_t TireSoundStart(uint8_t* base, uint32_t part, uint32_t index, uint32_t view,
                        const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D7F8 TireSound::Start");
  StoreB(base, part + 68, uint8_t(index));
  StoreF(base, part + 80, kZero);
  StoreF(base, part + 72, kZero);
  Store(base, part + 64, view);
  guest::StoreU16(base, part + 70, 0);
  guest::StoreU16(base, part + 76, 0);
  uint32_t r3 = 0;
  for (int i = 0; i < 2; i++) {
    SetRegs(call, Args({part + 24, view, 0, 1}));
    r3 = AddVoice(base, part + 24, view, 0, 1, voices,
                  xa::FrameBelow(base, frame, kAddVoiceFrame), call);
  }
  Store(base, part + 0, 0);
  Store(base, part + 4, 0);
  StoreB(base, part + 8, 0);
  StoreB(base, part + 9, 0);
  StoreB(base, part + 10, 0);
  Store(base, part + 12, 1);
  Store(base, part + 16, 0);
  StoreB(base, part + 20, 0);
  StoreB(base, part + 21, 0);
  StoreB(base, part + 22, 0);
  return r3;
}

uint32_t CarSoundStart(uint8_t* base, uint32_t sound, uint32_t views, uint32_t view,
                       const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E030 CarSound::Start");
  const auto index = [&] { return guest::LoadU8(base, sound + 404); };
  uint32_t i = index();
  SetRegs(call, Args({sound + 408, i, views}));
  EngineSoundStart(base, sound + 408, i, views, voices, xa::FrameBelow(base, frame, 112), call);
  i = index();
  SetRegs(call, Args({sound + 652, i, view}));
  TireSoundStart(base, sound + 652, i, view, voices, xa::FrameBelow(base, frame, 128), call);
  constexpr uint32_t kNone = 0xFFFFFFFFu;
  i = index();
  Store(base, sound + 624, view);
  StoreB(base, sound + 628, uint8_t(i));
  Store(base, sound + 632, 0);
  Store(base, sound + 636, 0);
  Store(base, sound + 640, kNone);
  Store(base, sound + 644, 0);
  Store(base, sound + 648, kNone);
  i = index();
  Store(base, sound + 776, view);
  Store(base, sound + 784, 0);
  Store(base, sound + 788, kNone);
  StoreB(base, sound + 780, uint8_t(i));
  i = index();
  Store(base, sound + 880, view);
  StoreB(base, sound + 885, 0);
  StoreB(base, sound + 884, uint8_t(i));
  i = index();
  Store(base, sound + 832, view);
  StoreB(base, sound + 836, uint8_t(i));
  SetRegs(call, Args({sound + 792, view, 1, 1}));
  const uint32_t r3 = AddVoice(base, sound + 792, view, 1, 1, voices,
                               xa::FrameBelow(base, frame, kAddVoiceFrame), call);
  i = index();
  Store(base, sound + 928, view);
  Store(base, sound + 936, 0);
  Store(base, sound + 940, kNone);
  StoreF(base, sound + 944, kPitchFloor);
  StoreB(base, sound + 932, uint8_t(i));
  StoreF(base, sound + 948, kPitchFloor);
  i = index();
  StoreF(base, sound + 1004, kZero);
  Store(base, sound + 992, view);
  StoreF(base, sound + 1012, kZero);
  Store(base, sound + 1000, kNone);
  guest::StoreU16(base, sound + 1008, 0);
  StoreB(base, sound + 1010, 0);
  StoreB(base, sound + 996, uint8_t(i));
  StoreB(base, sound + 1011, 0);
  i = index();
  Store(base, sound + 1056, view);
  Store(base, sound + 1064, 0);
  Store(base, sound + 1068, kNone);
  StoreB(base, sound + 1060, uint8_t(i));
  i = index();
  Store(base, sound + 1112, view);
  Store(base, sound + 1120, kNone);
  Store(base, sound + 1124, kNone);
  Store(base, sound + 1128, 0);
  StoreB(base, sound + 1116, uint8_t(i));
  Store(base, sound + 1132, 0);
  i = index();
  Store(base, sound + 1176, view);
  Store(base, sound + 1184, kNone);
  Store(base, sound + 1188, 0);
  StoreB(base, sound + 1180, uint8_t(i));
  StoreF(base, sound + 1256, kZero);
  return r3;
}

namespace {

uint32_t ClearHolders(uint8_t* base, uint32_t sound, uint32_t frame, const GuestCall& call) {
  uint32_t r3 = 0;
  for (const uint32_t offset : kClearHolders) {
    SetRegs(call, Args({sound + offset}));
    r3 = Clear(base, sound + offset, xa::FrameBelow(base, frame, 128), call);
  }
  return r3;
}

uint32_t SoundAt(uint8_t* base, uint32_t offset) {
  const uint32_t array = Load(base, kSounds);
  return array != 0 ? array + offset : 0;
}

}  // namespace

uint32_t Stop(uint8_t* base, uint32_t sound, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E218 CarSound::Stop");
  const uint32_t r3 = ClearHolders(base, sound, frame, call);
  Store(base, sound + 1248, 0);
  return r3;
}

uint32_t StopAll(uint8_t* base, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210F7C8 CarSounds::StopAll");
  uint32_t r3 = 0;
  for (uint32_t offset = 0; offset < 7 * kCarSoundBytes; offset += kCarSoundBytes) {
    const uint32_t sound = SoundAt(base, offset);
    SetRegs(call, Args({sound}));
    r3 = Stop(base, sound, xa::FrameBelow(base, frame, 112), call);
  }
  return r3;
}

uint32_t Release(uint8_t* base, uint32_t sound, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E170 CarSound::Release");
  ClearHolders(base, sound, frame, call);
  const uint32_t matrix = Load(base, sound + 176);
  SetRegs(call, Args({matrix}));
  uint32_t r3 = matrix;  // the native free leaves r3 the block
  if (!memory::crt::OperatorDeleteArray(matrix)) {
    r3 = call(0x825A71C0, 0x8210E1F4, Args({matrix}));
  }
  Store(base, sound + 1248, 0);
  return r3;
}

uint32_t CarSoundsRelease(uint8_t* base, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210F750 CarSounds::Release");
  for (uint32_t offset = 0; offset < 7 * kCarSoundBytes; offset += kCarSoundBytes) {
    const uint32_t sound = SoundAt(base, offset);
    SetRegs(call, Args({sound}));
    Release(base, sound, xa::FrameBelow(base, frame, 112), call);
  }
  const uint32_t array = Load(base, kSounds);
  if (array != 0) {
    SetRegs(call, Args({array, 3}));
    const Destroyed done = CarSoundsDestroy(base, array, 3);
    if (done.to_free != 0) {
      const bool freed = done.array ? memory::crt::OperatorDeleteArray(done.to_free)
                                    : memory::crt::OperatorDelete(done.to_free);
      if (!freed) {
        call(done.array ? 0x825A71C0 : 0x825A70E8, 0x8210F7A4, Args({done.to_free}));
      }
    }
    SetRegs(call, Args({done.r3}));
  }
  const uint32_t submix = Load(base, kSubmix);
  SetRegs(call, Args({submix}));
  xa::FrameBelow(base, frame, kApiFrame);
  return xa::ApiRelease(base, submix, 0, 0, 0, 0, call);
}

}  // namespace car_sounds
